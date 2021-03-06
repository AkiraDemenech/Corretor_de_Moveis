#include<stdio.h>
#include<stdlib.h>
#include"cidade.h"
#include"quadra.h"
#include"moradias.h"
#include"pessoas.h"
#include"list.h"
#include"hash.h"
#include"arv.h"
#include"arq.h"
#include"svg.h"
#include"loc.h"


void * list_get_quadra (void * lista, float y) {
	return list_get(lista,busca_bin_quadra(lista,y));
}

int busca_bin_quadra (void * lista, float y) {

	if(lista == NULL)
		return -2;

	int i = 0, f = list_get_len(lista);	
	int c;
	float t;
	while(i <= f) {
		c = (i + f) / 2;
		t = quadra_get_y(li_get_valor(list_get(lista,c)));		
		if(y > t)
			i = c + 1;
		else if(y < t)
			f = c - 1;
		else return c;			
	}		
	return -1;
}

void cidade_insort_quadra (void * lista, void * quadra) {
	int c;
	for(c = 0; c < list_get_len(lista); c++)
	//	if(li_get_chave(list_get(lista, c)) != NULL && *(float*)li_get_chave(list_get(lista,c)) >= quadra_get_y(quadra))
		if(quadra_get_y(li_get_valor(list_get(lista, c))) >= quadra_get_y(quadra))
			break;
	list_set(lista,c,quadra);
//	li_set_chave(list_get_atual(lista),malloc(sizeof(float)));
//	*((float*)li_get_chave(list_get_atual(lista))) = quadra_get_y(quadra);	
}

void arv_del_quadras (void * avl) {
	while(avl != NULL) {
		list_del_all(arv_get_valor(avl));
		arv_del_quadras(arv_get_dir(avl));
		avl = arv_get_esq(avl);
	}	
}

int arv_get_quadras_em (void * avl_in, void * list_out, float xi, float yi, float xf, float yf) {
	void * q;
	int c, busca = 0;	
	while(avl_in != NULL && arv_get_max(avl_in) >= xi && arv_get_min(avl_in) <= xf) {				
		busca++;	
	//	printf("\n%d\t%f\t",busca,arv_get_chave(avl_in));
		if(arv_get_chave(avl_in) >= xi) {			
			if(arv_get_chave(avl_in) <= xf) {				
			//	if(list_out == NULL)
			//		list_out = new_list(0);
				c = list_get_len(arv_get_valor(avl_in));
				while(c > 0) {
					c--;
					q = li_get_valor(list_get(arv_get_valor(avl_in), c));
					if(q != NULL && quadra_get_y(q) >= yi && quadra_get_y(q) + quadra_get_h(q) <= yf && quadra_get_x(q) + quadra_get_w(q) <= xf && quadra_get_x(q) >= xi) 
						list_add(list_out, q);
				//	printf("%s ",quadra_get_cep(q));	
				}							
			}				
		//	if(arv_get_chave(avl_in) > xi) // muito improv??vel que ele seja exatamente igual, e mesmo se for, m??nimo e m??ximo do filho esquerdo (menor) v??o excluir imediatamente
			busca += arv_get_quadras_em(arv_get_esq(avl_in), list_out, xi, yi, xf, yf);  										
		}
		if(arv_get_chave(avl_in) < xf) {
			avl_in = arv_get_dir(avl_in);
		//	arv_get_quadras_em(arv_get_dir(avl_in), list_out, xi, yi, xf, yf);		
			continue;
		}	
		break;
	}
	
	return busca;
}
void quadras_svg (void * img, void * arvore) {  
	int c;
	void * q;
	while(img != NULL && arvore != NULL) {		
		quadras_svg(img,arv_get_esq(arvore));
		c = list_get_len(arv_get_valor(arvore));
		while(c > 0) {
			c--;
			q = li_get_valor(list_get(arv_get_valor(arvore),c));
			if(q != NULL) {
				svg_rect(img,quadra_get_cep(q),quadra_get_fill(q),quadra_get_strk(q),quadra_get_esp(q),quadra_get_x(q),quadra_get_y(q),quadra_get_w(q),quadra_get_h(q));
			//	printf(" Quadra %s\t%s %s %s\t(%f %f)\t%fx%f\n",quadra_get_cep(q),quadra_get_fill(q),quadra_get_strk(q),quadra_get_esp(q),quadra_get_x(q),quadra_get_y(q),quadra_get_w(q),quadra_get_h(q));
			}	
		}
		arvore = arv_get_dir(arvore);
	}
}	
void cidade_svg (char * svg, void * cidade) {
	if(svg == NULL)
		return;
	FILE * res = fopen(svg,"w");
	if(res != NULL) {
		svg_open(res);
		svg_comment_open(res);
		printf(svg);
		fprintf(res,"\n202000560125 \nGuilherme Akira Demenech Mori \n%s\n\t",arq_nome(svg));
		svg_comment_close(res);
		quadras_svg(res,cidade_get_quadras_avl(cidade));
		svg_close(res);
		fclose(res);
	} else printf("Arquivo %s n??o p??de ser aberto.",svg);
	printf("\n");
}

void * cidade_geo (char * geo) {
	if(geo == NULL)
		return NULL;

	void * b;
	void * c = new_cidade(geo,16);	
	FILE * g = fopen(geo,"r");
	if(g == NULL) {
		printf("Arquivo \"%s\" n??o encontrado.\n",geo);
		return c;
	}

	int l, cont = 1, quad = 0;	
	float t0,t1,t2,t3;
	char * fill = NULL;
	char * strk = NULL;
	char * espessura = NULL;
	char cep[CEP_TAM];
	char com[COM_TAM];	
	while(!feof(g)) {

		clean(com,COM_TAM);
		fscanf(g,"%s",com);

		switch (com[0]) {
			case 'c':
				printf("As cores %s e %s ",fill,strk);
				if(cont == 0) {
					printf("n??o foram usadas.\n");
					free(espessura);
					free(strk);
					free(fill);
				} else printf("foram aplicadas a %d quadras.\n",cont);
				espessura = calloc(16,sizeof(char));
				clean(cep,CEP_TAM);
				fscanf(g,"%s %s",espessura,cep);
				fill = malloc(sizeof(char)*(comprimento(cep)+1));
				copy(cep,fill);
				clean(cep,CEP_TAM);
				fscanf(g,"%s",cep);
				strk = malloc(sizeof(char) * (comprimento(cep) + 1));				
				cont = 0;
				copy(cep,strk);
				printf("Novas cores = %s %s %s\n",espessura,fill,strk);
			break;

			case 'q':
				clean(cep,CEP_TAM);
				fscanf(g,"%s",cep);
				l = comprimento(cep) + 1;
				fscanf(g,"%f %f %f %f",&t0,&t1,&t2,&t3);
				b = new_quadra(t0,t1,t2,t3,espessura,strk,fill,malloc(sizeof(char)*l));				
				clean(quadra_get_cep(b), l);
				copy(cep,quadra_get_cep(b));				
				cidade_set_quadra(c, b);
				cont++;
				quad++;
			break;
		
			case 'n':
				fscanf(g,"%d",&l);
				cidade_set_tamanho(c,l);
				printf("Tamanho das tabelas = %d\n",l);				
		}

	}

	if(cont == 0) {		
		printf("As cores %s e %s n??o foram usadas!\n",fill,strk);
		free(fill);
		free(strk);
	}
	printf("%d quadras adicionadas!\n",quad);
	

	fclose(g);
	return c;
}

void cidade_pm (void * cid, char * pm) {
	if(pm == NULL || cid == NULL)
		return;

	void * m;
	FILE * pes = fopen(pm,"r");
	if(pes == NULL) {
		printf("Arquivo \"%s\" n??o encontrado.\n",pm);
		return;
	}

	int n, p = 0, c = 0, nb = 0, masc = 0, fem = 0;
	int norte = 0, sul = 0, leste = 0, oeste = 0, outros = 0;
	char nome[NOME_TAM], sobrenome[NOME_TAM], datanas[NASC_TAM], sexo[COM_TAM];
	char cpf[CPF_TAM], cep[CEP_TAM];
	char com[COM_TAM];
	while(!feof(pes)) {
		clean(com, COM_TAM);
		fscanf(pes,"%s",com);
		printf(com);
		switch (com[0]) {
			case 'p':				
				clean(cpf,CPF_TAM);
				clean(sexo,COM_TAM);
				clean(datanas,NASC_TAM);				
				clean(nome, NOME_TAM);
				clean(sobrenome, NOME_TAM);
				fscanf(pes,"%s %s %s %s %s",cpf,nome,sobrenome,sexo,datanas);
				m = new_pessoa(sexo[0],cpf,datanas,malloc(sizeof(char)*(1+comprimento(nome))),malloc(sizeof(char)*(1+comprimento(sobrenome))));				
				copy(sobrenome,pessoa_sobrenome(m));
				copy(nome,pessoa_nome(m));				
				cidade_set_pessoa(cid,m);				
				printf("%c",pessoa_gen(m)); 
				if(pessoa_gen(m) == MASC)
					masc++;
				else if(pessoa_gen(m) == FEM)
					fem++;
				else nb++;		//*/
				p++;
				break;
			
			case 'm':
				clean(cpf, CPF_TAM);
				clean(cep, CEP_TAM);
				clean(sexo,COM_TAM);
				clean(nome,NOME_TAM);
				fscanf(pes,"%s %s %s %d %s",cpf,cep,sexo,&n,nome);
				m = new_moradia(n,sexo[0],malloc(sizeof(char)*(1+comprimento(nome))),malloc(sizeof(char)*(1+comprimento(cep))),malloc(sizeof(char)*(1+comprimento(cpf))));				
				printf("%c",moradia_get_face(m));
				copy(nome, moradia_get_compl(m));
				copy(cep,moradia_get_cep(m));
				copy(cpf,moradia_get_cpf(m));
				cidade_set_moradia(cid,m);	//*/			
				c++;
				switch (moradia_get_face(m)) {
					case NORTE:
						norte++;
						break;

					case SUL:
						sul++;
						break;	

					case LESTE:
						leste++;
						break;

					case OESTE:
						oeste++;
						break;								
				
					default:
						outros++;
				}
				break;

			default:	
				printf("?");
		}
	}
	fclose(pes); 	
	printf("\nAdicionadas %d pessoas (%d NB, %d Fem e %d Masc) e %d moradias (%d Norte, %d Sul, %d Leste, %d Oeste e %d em outros).\n",p,nb,fem,masc,c,norte,sul,leste,oeste,outros);
}

typedef struct cid {
	int tam;
	char * nome_cid;
	void * quadras_avl;
	void * quadras_hash;
	void * pessoas_hash;
	void * moradias_cpf;
	void * moradias_cep;
	void * aluguel_hash;
} cidade;

void * new_cidade (char * nome, int tamanho) {
	cidade * c = (cidade*)malloc(sizeof(cidade));
	c->aluguel_hash	= NULL;
	c->pessoas_hash	= NULL;
	c->quadras_hash	= NULL;
	c->quadras_avl	= NULL;
	c->moradias_cep = NULL;
	c->moradias_cpf = NULL;
	
	c->nome_cid 	= nome;
	c->tam = tamanho;
	return c;
}

char * cidade_get_nome (void * cid) {
	if(cid != NULL) 
		return ((cidade*)cid)->nome_cid;
	return NULL;
}

int cidade_get_tamanho (void * cid) {
	if(cid != NULL) 
		return ((cidade*)cid)->tam;
	return -1;	
}		

void cidade_set_tamanho (void * cid, int tamanho) {
	if(cid != NULL) 
		((cidade*)cid)->tam = tamanho;
	cidade_set_pessoas(cid, tamanho);	
	cidade_set_quadras(cid, tamanho);
	cidade_set_moradias(cid,tamanho);
	cidade_set_alugueis(cid,tamanho);
}

void cidade_set_aluguel (void * cid, void * loc) {
	if(loc != NULL) 
		hash_set(cidade_get_alugueis(cid),loc_id(loc),loc);
	if(loc_get_moradia(loc) != NULL)
		cidade_set_moradia(cid,loc_get_moradia(loc));
}

void cidade_set_alugueis (void * cid, int n) {

	if(cid != NULL)
		((cidade *) cid)->aluguel_hash = new_hash_table(n);

}

void * cidade_get_alugueis (void * cid) {

	if(cid != NULL)
		return ((cidade *) cid)->aluguel_hash;
	return NULL;	

}

void * cidade_get_moradias_em (void * cid, float x, float y, float w, float h) {
	void * q = cidade_get_quadras_em(cid, x, y, w, h);
	void * t;
	int c, b;	
	for(b = list_get_len(q) - 1; b >= 0; b--) {		
		t = hash_get(cidade_get_moradias_cep(cid), quadra_get_cep(li_get_valor(list_get(q, b))));
		list_del(q, b);
		for(c = 0; c < list_get_len(t); c++) 
			list_set(q,b,li_get_valor(list_get(t,c)));
	}
	printf("%d quadras inteiramente na ??rea.\n",list_get_len(q));
	return q;
}

void * cidade_get_quadras_em (void * cid, float x, float y, float w, float h) {	
	printf("??rea de busca:\t%f \t[(%f %f) (%f %f)]\n",h*w,x,y,x+w,y+h);
	void * quadras = new_list(0);
	printf("%d fileiras de quadras plaus??veis.\n",arv_get_quadras_em(cidade_get_quadras_avl(cid),quadras,x,y,x+w,y+h));
	return quadras;
}

void * cidade_get_quadras_hash (void * cid) {
	if(cid == NULL)
		return NULL;
	return ((cidade *) cid)->quadras_hash;
}

void * cidade_get_quadras_avl (void * cid) {
	if(cid != NULL)
		return ((cidade *) cid)->quadras_avl;
	return NULL;
}

void cidade_set_quadras (void * cid, int n) {
	if(cid == NULL)
		return;

	void * antiga = ((cidade*)cid)->quadras_hash;			 
	((cidade *) cid)->quadras_hash = new_hash_table(n);	

	arv_del_all(((cidade*)cid)->quadras_avl);
	((cidade *) cid)->quadras_avl = NULL;

	int c;
	for(c = hash_get_chaves_len(antiga) - 1; c >= 0; c--)
		cidade_set_quadra(cid,hash_get(antiga,hash_get_chave(antiga,c)));
	hash_del_all(antiga);

}

void cidade_set_quadra (void * cid, void * quadra) {
	if(cid == NULL || quadra == NULL)
		return;
	hash_set(cidade_get_quadras_hash(cid), quadra_get_cep(quadra), quadra);	
	void * n = arv_get(cidade_get_quadras_avl(cid),quadra_get_x(quadra));
	if(n == NULL) {
		n = new_list(0);
		avl_add(&(((cidade*)cid)->quadras_avl),n,quadra_get_x(quadra),quadra_get_x(quadra),quadra_get_x(quadra)+quadra_get_w(quadra));
	} else {
		arv_set_max(n, quadra_get_x(quadra) + quadra_get_w(quadra));
		arv_set_min(n, quadra_get_x(quadra));
		n = arv_get_valor(n);
	}
	cidade_insort_quadra(n,quadra);
	
	//	list_avl_set_quadra(,quadra);  	
}

void cidade_del_quadra_avl (void * cid, float x, float y) {
	void * a = arv_get(cidade_get_quadras_avl(cid), x);
	int b = busca_bin_quadra(arv_get_valor(a), y);
	if(b < 0) {		
		printf("\n Quadra em (%f %f) n??o encontrada na busca bin??ria %d.\n",x,y,b);		
		int c;
		for(c = 0; c < list_get_len(arv_get_valor(a)); c++)
			if(quadra_get_y(li_get_valor(list_get(arv_get_valor(a),c))) == y) {
				printf("[Encontrada ");
				if(b < 0)
					printf("primeira ocorr??ncia ");
				printf("%s em %d]\n",quadra_get_cep(li_get_valor(list_get(arv_get_valor(a),c))),c);	
				b = c;
			} else printf(" %d\t%f\t%s\n",c,quadra_get_y(li_get_valor(list_get(arv_get_valor(a),c))),quadra_get_cep(li_get_valor(list_get(arv_get_valor(a),c))));		 						 
	}  	
	if(b >= 0) {		
	//	printf("Apagando quadra %s (%f %f)\n",quadra_get_cep(li_get_valor(list_get(arv_get_valor(a),b))),quadra_get_x(li_get_valor(list_get(arv_get_valor(a),b))),quadra_get_y(li_get_valor(list_get(arv_get_valor(a),b))));
		hash_del(cidade_get_quadras_hash(cid),quadra_get_cep(li_get_valor(list_get(arv_get_valor(a),b))));
		list_del(arv_get_valor(a), b);
		if(list_get_len(arv_get_valor(a)) <= 0) {
			list_del_all(arv_get_valor(a));
			a = avl_del(cidade_get_quadras_avl(cid), x);
			if(cid != NULL) 
				((cidade *) cid)->quadras_avl = a;
			printf("Removido n?? %f da ??rvore AVL\n", x);	
		}						
	} else printf("\tE continua desaparecida!\n");	 
}

void cidade_del_quadra_hash (void * cid, char * cep) {
	void * q = cidade_get_quadra_hash(cid, cep);
	cidade_del_quadra_avl(cid,quadra_get_x(q),quadra_get_y(q));
}

void * cidade_get_quadra_avl (void * cid, float x, float y) {	 
	return li_get_valor(list_get_quadra(arv_get_valor(arv_get(cidade_get_quadras_avl(cid),x)),y));
}
void * cidade_get_quadra_hash (void * cid, char * cep) {	
	return hash_get(cidade_get_quadras_hash(cid), cep);
}

void cidade_set_moradias (void * cid, int n) {
	void * cpf = cidade_get_moradias_cpf(cid);
	void * cep = cidade_get_moradias_cep(cid);			
	if(cid != NULL) {
		((cidade *) cid)->moradias_cep = new_hash_table(n);	
		((cidade *) cid)->moradias_cpf = new_hash_table(n);
	} else return;	
	int c;
	for(c = hash_get_chaves_len(cpf) - 1; c >= 0; c--)
		cidade_set_moradia(cid,hash_get(cpf,hash_get_chave(cpf,c)));	
	hash_del_all(cep);
	hash_del_all(cpf);
}

void cidade_set_moradia (void * cid, void * moradia) {
	if(cid != NULL && moradia != NULL) {
		if(moradia_get_cpf(moradia) == NULL)
			printf(" Moradia em %s/%c/%d\t%s sem CPF registrado\n",moradia_get_cep(moradia),moradia_get_face(moradia),moradia_get_num(moradia),moradia_get_compl(moradia));
		else hash_set(cidade_get_moradias_cpf(cid), moradia_get_cpf(moradia), moradia);
		void* q = hash_get(cidade_get_moradias_cep(cid), moradia_get_cep(moradia));
		if(q == NULL) {
			q = new_list(0);
			hash_set(cidade_get_moradias_cep(cid),moradia_get_cep(moradia),q);			
		}
		list_add(q,moradia);
		li_set_chave(list_get_atual(q),moradia_get_cpf(moradia));		
	}
}

void * cidade_get_moradias_cep (void * cid) {
	if(cid == NULL)
		return NULL;
	return ((cidade*)cid)->moradias_cep;	
}

void * cidade_get_moradias_cpf (void * cid) {
	if(cid == NULL)
		return NULL;
	return ((cidade*)cid)->moradias_cpf;	
}

void cidade_del_moradia_cpf (void * cid, char * cpf) {
	void * m = hash_get(cidade_get_moradias_cpf(cid),cpf);
	if(m != NULL) {
		hash_del(cidade_get_moradias_cpf(cid), cpf);
		void * l = hash_get(cidade_get_moradias_cep(cid),moradia_get_cep(m));
		int c;
		for(c = 0; c < list_get_len(l); c++) 
			if(compare(li_get_chave(list_get(l,c)),moradia_get_cpf(m)) == 0 || m == li_get_valor(list_get(l,c))) {
				list_del(l,c);
				c--;
			}						
	}
}

void cidade_del_moradia_cep (void * cid, char * cep) {
	void * l = hash_get(cidade_get_moradias_cep(cid),cep);
	if(l != NULL) {
		hash_del(cidade_get_moradias_cep(cid), cep);
		while(list_get_len(l) > 0) {
			hash_del(cidade_get_moradias_cpf(cid), li_get_chave(list_get_atual(l)));
			list_del(l,list_get_index(l));			
		}						
	}
}

void cidade_del_moradias (void * cid) {
	void * l;
	int c, d;
	for(c = 0; c < hash_get_chaves_len(cidade_get_moradias_cep(cid)); c++) {
		l = hash_get(cidade_get_moradias_cep(cid), hash_get_chave(cidade_get_moradias_cep(cid), c));
		for(d = 0; d < list_get_len(l); d++)
			free(li_get_valor(list_get(l, d)));
		list_del_all(l);	
	}	
	hash_del_all(cidade_get_moradias_cep(cid));
	hash_del_all(cidade_get_moradias_cpf(cid));
}

void * cidade_get_pessoa (void * cid, char * cpf) {
	return hash_get(cidade_get_pessoas(cid), cpf);
}

void * cidade_get_pessoas (void * cid) {
	if(cid != NULL)
		return ((cidade *) cid)->pessoas_hash;
	return NULL;	
}

void cidade_set_pessoas (void * cid, int n) {		
	void * pes = cidade_get_pessoas(cid);		
	if(cid != NULL)
		((cidade *) cid)->pessoas_hash = new_hash_table(n);	
	else return;	
	int c;
	for(c = hash_get_chaves_len(pes) - 1; c >= 0; c--)
		cidade_set_pessoa(cid,hash_get(pes,hash_get_chave(pes,c)));
	//	hash_set(cidade_get_pessoas(cid),hash_get_chave(pes,c),hash_get(pes,hash_get_chave(pes,c))); //*/	
	hash_del_all(pes);
	
}

void cidade_set_pessoa (void * cid, void * pessoa) {
	if(cid != NULL && pessoa != NULL)
		hash_set(cidade_get_pessoas(cid),pessoa_cpf(pessoa),pessoa);
//	printf("{%s\t%p}",pessoa_cpf(pessoa),pessoa);	
}

void cidade_del_pessoa (void * cid, char * cpf) {
	hash_del(cidade_get_pessoas(cid),cpf);
}

void cidade_del_pessoas (void * cid) {
	void * p = cidade_get_pessoas(cid);
	int c;
	for(c = 0; c < hash_get_chaves_len(p); c++) 
		free(hash_get(p, hash_get_chave(p, c)));	
	hash_del_all(p);
	if(cid != NULL)
		((cidade*)cid)->pessoas_hash = NULL;
}

void cidade_del_all (void * cid) {
	cidade_del_pessoas(cid);
	cidade_del_moradias(cid);
	arv_del_quadras(cidade_get_quadras_avl(cid));
	arv_del_all(cidade_get_quadras_avl(cid));
	hash_del_all(cidade_get_quadras_hash(cid));
	hash_del_all(cidade_get_alugueis(cid));
	if(cid != NULL) 				
		free(cid);
	
}

