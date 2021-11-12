#include<stdio.h>
#include<stdlib.h>
#include"quadra.h"
#include"mapa.h"
#include"vias.h"
#include"list.h"
#include"hash.h"
#include"arv.h"
#include"arq.h"
#include"svg.h"

void * dijkstra (void * grafo, float cv) {
	if(grafo == NULL)
		return NULL;
	int i, j, c = 0;
	void * fila = new_list(0);
	void * caminhos = new_hash_table(8191);
	void * v = new_via(vert_id(grafo),NULL,NULL);
	void * u;
	float p = c;
	via_set_para(v,grafo);
	via_set_cmp(v,p);
	via_set_vm(v, c);
	list_set(fila,c,v);
	while(c < list_get_len(fila)) {
		v = li_get_valor(list_get(fila, c));
		grafo = via_get_para(v);
		for(i = 0; i < list_get_len(vert_get_vias(grafo)); i++)
			if(via_get_para(li_get_valor(list_get(vert_get_vias(grafo), i))) != grafo && li_get_valor(list_get(vert_get_vias(grafo),i)) != NULL) { 				
				p = via_get_cmp(li_get_valor(list_get(vert_get_vias(grafo),i)))/((cv == 0)?(1):(cv*via_get_vm(li_get_valor(list_get(vert_get_vias(grafo),i)))));// + via_get_cmp(v);
				u = NULL;
				for(j = 0; j < list_get_len(fila); j++) 
					if(via_get_para(li_get_valor(list_get(vert_get_vias(grafo),i))) == via_get_para(li_get_valor(list_get(fila,j)))) {
						u = li_get_valor(list_get(fila, j));
						if(via_get_vm(u) > p + via_get_cmp(v)) 														
							list_del(fila, j);
						else j = -1;
						break;
					}					 				
				if(u == NULL) {	
					u = new_via(vert_id(via_get_para(li_get_valor(list_get(vert_get_vias(grafo), i)))), NULL, NULL);
					via_set_para(u, via_get_para(li_get_valor(list_get(vert_get_vias(grafo), i))));										
				}	
				if(j >= 0) {
					via_set_de(u, v);
					via_set_vm(u, p + via_get_cmp(v));
					via_set_cmp(u, p);
					while(j > 0 && via_get_vm()) 
						j--;
						
												
							
						
					
				}

			}	
		c++;
	}
	list_del_all(fila);
	return caminhos;
}

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
		if(quadra_get_y(li_get_valor(list_get(lista, c))) >= quadra_get_y(quadra))
			break;
	list_set(lista,c,quadra);
}

int busca_bin_ponto (void * lista, float y) {

	if(lista == NULL)
		return -2;

	int i = 0, f = list_get_len(lista);	
	int c;
	float t;
	while(i <= f) {
		c = (i + f) / 2;
		t = vert_y(li_get_valor(list_get(lista,c)));		
		if(y > t)
			i = c + 1;
		else if(y < t)
			f = c - 1;
		else return c;			
	}		
	return -1;
}

void cidade_insort_ponto (void * lista, void * ponto) {
	int c;
	for(c = 0; c < list_get_len(lista); c++)
		if(vert_y(li_get_valor(list_get(lista, c))) >= vert_y(ponto))
			break;
	list_set(lista,c,ponto);
}

void arv_del_listas (void * avl) {
	while(avl != NULL) {
		list_del_all(arv_get_valor(avl));
		arv_del_quadras(arv_get_dir(avl));
		avl = arv_get_esq(avl);
	}	
}

int arv_get_pontos_em (void * avl_in, void * list_out, float xi, float yi, float xf, float yf) {
	void * p;
	int c, busca = 0;
	while(avl_in != NULL && arv_get_max(avl_in) >= xi && arv_get_min(avl_in) <= xf) {
		busca++;
		if(arv_get_chave(avl_in) <= xf) {
			if(arv_get_chave(avl_in) >= xi) {
				for(c = 0; c < list_get_len(arv_get_valor(avl_in)); c++) {
					p = li_get_valor(list_get(arv_get_valor(avl_in), c));
					if(p != NULL && vert_y(p) >= yi && vert_y(p) <= yf && vert_x(p) <= xf && vert_x(p) >= xi)
						list_add(list_out, p);
				}	
			}	
			busca += arv_get_pontos_em(arv_get_dir(avl_in), list_out, xi, yi, xf, yf);	
		}	
		if(arv_get_chave(avl_in) > xi)
			avl_in = arv_get_esq(avl_in);			
		else break;	
	}
	return busca;
}

int arv_get_quadras_em (void * avl_in, void * list_out, float xi, float yi, float xf, float yf) {
	void * q;
	int c, busca = 0;	
	while(avl_in != NULL && arv_get_max(avl_in) >= xi && arv_get_min(avl_in) <= xf) {				
		busca++;	
		if(arv_get_chave(avl_in) >= xi) {			
			if(arv_get_chave(avl_in) <= xf) {				
				c = list_get_len(arv_get_valor(avl_in));
				while(c > 0) {
					c--;
					q = li_get_valor(list_get(arv_get_valor(avl_in), c));
					if(q != NULL && quadra_get_y(q) >= yi && quadra_get_y(q) + quadra_get_h(q) <= yf && quadra_get_x(q) + quadra_get_w(q) <= xf && quadra_get_x(q) >= xi) 
						list_add(list_out, q);
				}							
			}				
			busca += arv_get_quadras_em(arv_get_esq(avl_in), list_out, xi, yi, xf, yf);  										
		}
		if(arv_get_chave(avl_in) < xf) {
			avl_in = arv_get_dir(avl_in);
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
	} else printf("Arquivo %s não pôde ser aberto.",svg);
	printf("\n");
}

void * cidade_geo (char * geo) {
	if(geo == NULL)
		return NULL;

	void * b;
	void * c = new_cidade(geo,16);	
	FILE * g = fopen(geo,"r");
	if(g == NULL) {
		printf("Arquivo \"%s\" não encontrado.\n",geo);
		return c;
	}

	cidade_set_tamanho(c,1024);

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
					printf("não foram usadas.\n");
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
			
		}

	}

	if(cont == 0) {		
		printf("As cores %s e %s não foram usadas!\n",fill,strk);
		free(fill);
		free(strk);
	}
	printf("%d quadras adicionadas!\n",quad);
	

	fclose(g);
	return c;
}

void cidade_vias (void * cid, char * vias) {
	if(vias == NULL || cid == NULL)
		return;

	void * m;
	FILE * pes = fopen(vias,"r");
	if(pes == NULL) {
		printf("Arquivo \"%s\" não encontrado.\n",vias);
		return;
	}

	void * pontos = new_hash_table(8191);

	int p = 0, c = 0;
	float a,b;
	char i[CEP_TAM], f[CEP_TAM], k[CEP_TAM], l[CEP_TAM * 3];
	char cep[CEP_TAM];
	char com[COM_TAM];
	while(!feof(pes)) {
		clean(com, COM_TAM);
		fscanf(pes,"%s",com);
		printf(com);
		switch (com[0]) {
			case 'v':				
				clean(cep,CEP_TAM);
				fscanf(pes,"%s %f %f",cep,&a,&b);
				m = new_vert(malloc(sizeof(char) * (1 + comprimento(cep))), a, b);		//*/
				vert_set_vias(m, new_list(0));
				copy(cep, vert_id(m));
				hash_set(pontos,vert_id(m),m);
				cidade_set_ponto(cid,m);
				p++;
				break;
			
			case 'e':
				clean(cep,CEP_TAM);
				clean(l,3*CEP_TAM);
				clean(i,CEP_TAM);
				clean(f,CEP_TAM);
				clean(k,CEP_TAM);				
				fscanf(pes,"%s %s %s %s %f %f %c",i,f,k,cep,&a,&b,l);
				m = new_via(malloc(sizeof(char) * (1 + comprimento(l))),malloc(sizeof(char) * (comprimento(k) + 1)),malloc(sizeof(char) * (comprimento(cep) + 1)));				
				via_set_de(m, hash_get(pontos, i));
				via_set_para(m,hash_get(pontos,f));
				copy(l, via_nome(m));
				copy(cep,via_esq(m));
				copy(k,via_dir(m)); 
				
				
					//*/			
				c++;
				

			
		}
	}	
	printf("\nAdicionadas %d pontos e %d segmentos.\n",p,c);	
	hash_del_all(pontos); // não será mais  necessário, o ID dos vértices não será mais útil para busca
	fclose(pes); 	
}

typedef struct cid {
	int tam;
	char * nome_cid;
	void * pontos_avl;
	void * quadras_avl;
	void * quadras_hash;
	void * vias_hash;
} cidade;

void * new_cidade (char * nome, int tamanho) {
	cidade * c = (cidade*)malloc(sizeof(cidade));
	c->quadras_hash	= NULL;
	c->quadras_avl	= NULL;
	c->pontos_avl 	= NULL;
	c->vias_hash	= NULL;
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
	cidade_set_quadras(cid, tamanho);
}

void * cidade_get_pontos_em (void * cid, float x, float y, float w, float h) {
	printf("Área de busca:\t%f \t[(%f %f) (%f %f)]\n",h*w,x,y,x+w,y+h);
	void * pontos = new_list(0);
	printf("%d fileiras de pontos plausíveis.\n",arv_get_pontos_em(cidade_get_quadras_avl(cid),pontos,x,y,x+w,y+h));
	return pontos;
}

void * cidade_get_quadras_em (void * cid, float x, float y, float w, float h) {	
	printf("Área de busca:\t%f \t[(%f %f) (%f %f)]\n",h*w,x,y,x+w,y+h);
	void * quadras = new_list(0);
	printf("%d fileiras de quadras plausíveis.\n",arv_get_quadras_em(cidade_get_quadras_avl(cid),quadras,x,y,x+w,y+h));
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
}

void cidade_del_quadra_avl (void * cid, float x, float y) {
	void * a = arv_get(cidade_get_quadras_avl(cid), x);
	int b = busca_bin_quadra(arv_get_valor(a), y);
	if(b < 0) {		
		printf("\n Quadra em (%f %f) não encontrada na busca binária %d.\n",x,y,b);		
		int c;
		for(c = 0; c < list_get_len(arv_get_valor(a)); c++)
			if(quadra_get_y(li_get_valor(list_get(arv_get_valor(a),c))) == y) {
				printf("[Encontrada ");
				if(b < 0)
					printf("primeira ocorrência ");
				printf("%s em %d]\n",quadra_get_cep(li_get_valor(list_get(arv_get_valor(a),c))),c);	
				b = c;
			} else printf(" %d\t%f\t%s\n",c,quadra_get_y(li_get_valor(list_get(arv_get_valor(a),c))),quadra_get_cep(li_get_valor(list_get(arv_get_valor(a),c))));		 						 
	}  	
	if(b >= 0) {			
		hash_del(cidade_get_quadras_hash(cid),quadra_get_cep(li_get_valor(list_get(arv_get_valor(a),b))));
		list_del(arv_get_valor(a), b);
		if(list_get_len(arv_get_valor(a)) <= 0) {
			list_del_all(arv_get_valor(a));
			a = avl_del(cidade_get_quadras_avl(cid), x);
			if(cid != NULL) 
				((cidade *) cid)->quadras_avl = a;
			printf("Removido nó %f da árvore AVL\n", x);	
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

void * cidade_get_vias (void * cid) {
	if(cid != NULL)
		return ((cidade *) cid)->vias_hash;
	return NULL;
}

void cidade_set_vias (void * cid, void * vias) {
	if(cid != NULL)
		((cidade *) cid)->vias_hash = vias;
}

void cidade_del_all (void * cid) {
	arv_del_listas(cidade_get_quadras_avl(cid));
	arv_del_listas(cidade_get_pontos(cid));
	arv_del_all(cidade_get_pontos(cid));
	arv_del_all(cidade_get_quadras_avl(cid));
	hash_del_all(cidade_get_quadras_hash(cid));	
	if(cid != NULL) 				
		free(cid);	
}

void * cidade_get_pontos (void * cid) {
	if(cid != NULL)
		return ((cidade *) cid)->pontos_avl;
	return NULL;
}

void * cidade_get_ponto (void * cid, float x, float y) {
	void * l = arv_get(cidade_get_pontos(cid), x);
	int c = busca_bin_ponto(l,y);
	if(c >= 0)
		return li_get_valor(list_get(l, c));
	for(c = 0; c < list_get_len(l); c++) 
		if(vert_y(li_get_valor(list_get(l, c))) == y)
			return li_get_valor(list_get(l,c));	
	return NULL; //*/
	
}

int cidade_del_ponto (void * cid, float x, float y) {
	void * l = arv_get(cidade_get_pontos(cid), x);
	int c, d = 0;
	for(c = list_get_len(l) - 1; c >= 0; c--) 
		if(vert_y(li_get_valor(list_get(l,c))) == y) {
			list_del(l, c);		
			d++;
		}	
	return d;	
}

void cidade_set_ponto (void * cid, void * p) {
	void * l = arv_get(cidade_get_pontos(cid),vert_x(p));
	if(l == NULL && cid != NULL) {
		l = new_list(0);
		avl_add(&(((cidade*)cid)->pontos_avl),l,vert_x(p),vert_x(p),vert_x(p));
	}	
	if(p != NULL)
		cidade_insort_ponto(l,p);
	//	list_insert(l,p);
}