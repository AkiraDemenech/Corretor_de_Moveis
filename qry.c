#include <stdio.h>
#include<stdlib.h>
#include"cidade.h"
#include"pessoas.h"
#include"moradias.h"
#include"quadra.h"
#include"hash.h"
#include"list.h"
#include "qry.h"
#include "svg.h"
#include "arq.h"
#include "arv.h"
#include "loc.h"




void registrar_aluguel (void * loc, FILE * arq) {
	if(loc != NULL && arq != NULL) {
		fprintf(arq,"ID:\t%s",loc_id(loc));
		fprintf(arq,"\nR$%f\t%f m²\n",loc_get_valor(loc),loc_get_area(loc));
	}
}

void registrar_moradia (void * mor, FILE * arq) {
	if(mor == NULL || arq == NULL)
		return;
	fprintf(arq, "CEP:\t%s/%c/%d\n", moradia_get_cep(mor), moradia_get_face(mor), moradia_get_num(mor));
	fprintf(arq, "Complemento:\t%s\n", moradia_get_compl(mor));
}
void registrar_pessoa (void * pes, FILE * arq) {
	if(pes == NULL || arq == NULL) 
		return;
	fprintf(arq, "Nome:\t%s %s\n", pessoa_nome(pes), pessoa_sobrenome(pes));
	fprintf(arq, "Gênero:\t%c\nNascimento:\t%s\n", pessoa_gen(pes), pessoa_datanas(pes));	
}

int registrar_gen_por_moradia (FILE * txt, FILE * svg, void * cidade, char * cor, char gen, float x, float y, float w, float h) {
	void * moradias = cidade_get_moradias_em(cidade, x, y, w, h);
	void * pessoa, * quadra;	
	int c, p = 0, t = 0;
	for(c = 0; c < list_get_len(moradias); c++) {
		if(moradia_get_cpf(li_get_valor(list_get(moradias,c))) != NULL) {
			pessoa = cidade_get_pessoa(cidade, moradia_get_cpf(li_get_valor(list_get(moradias, c))));
			if(pessoa_gen(pessoa) == gen) {
				fprintf(txt,"\n");
				registrar_pessoa(pessoa,txt);
				fprintf(txt,"CPF:\t%s\n",moradia_get_cpf(li_get_valor(list_get(moradias,c))));
				registrar_moradia(li_get_valor(list_get(moradias, c)), txt);
				registrar_aluguel(moradia_get_loc(li_get_valor(list_get(moradias, c))), txt);
				quadra = cidade_get_quadra_hash(cidade, moradia_get_cep(li_get_valor(list_get(moradias, c))));
				if(quadra == NULL)
					printf("\tQuadra CEP %s indisponível\n",moradia_get_cep(li_get_valor(list_get(moradias,c))));
				else svg_ellipse(svg,pessoa_cpf(pessoa),cor,NULL,NULL,moradia_get_x(li_get_valor(list_get(moradias,c)),quadra),moradia_get_y(li_get_valor(list_get(moradias,c)),quadra),7,15);					
				p++;
			}
			t++;
		}
	}
	printf("%d pessoas possíveis em %d moradias promissoras.\n",t,list_get_len(moradias));
	list_del_all(moradias);
	return p;
}

int del (void * cidade, char * cep, FILE * res) {
	if(cep == NULL || res == NULL)
		return 0;		
	void * quadra = cidade_get_quadra_hash(cidade, cep);	
	cidade_del_quadra_hash(cidade, cep);	
	if(quadra == NULL) {
		printf("Quadra de CEP %s não encontrada.\n",cep);
		fprintf(res,"\nQuadra de CEP %s não encontrada.\n",cep);		
	} else {
		fprintf(res,"\n\tCEP:\t%s\n\t(%f %f) %fx%f\n\t %s  %s %s \n",quadra_get_cep(quadra),quadra_get_x(quadra),quadra_get_y(quadra),quadra_get_w(quadra),quadra_get_h(quadra),quadra_get_esp(quadra),quadra_get_strk(quadra),quadra_get_fill(quadra));
		free(quadra);
	}	
	int t = 0;
	void * moradias = hash_get(cidade_get_moradias_cep(cidade),cep);	
//	printf("%d moradias\n",list_get_len(moradias));
	while(list_get_len(moradias) > 0) {		
		fprintf(res,"\n");		
		if(li_get_valor(list_get_atual(moradias)) != NULL) {			
			if(moradia_get_cpf(li_get_valor(list_get_atual(moradias))) != NULL) {
				registrar_pessoa(cidade_get_pessoa(cidade,moradia_get_cpf(li_get_valor(list_get_atual(moradias)))),res);
				fprintf(res,"CPF:\t%s\n",moradia_get_cpf(li_get_valor(list_get_atual(moradias))));			
				hash_del(cidade_get_moradias_cpf(cidade),moradia_get_cpf(li_get_valor(list_get_atual(moradias))));						
			}	
			registrar_moradia(li_get_valor(list_get_atual(moradias)),res);
			registrar_aluguel(moradia_get_loc(li_get_valor(list_get_atual(moradias))),res);
			if(moradia_get_loc(li_get_valor(list_get_atual(moradias))) != NULL) {
				hash_del(cidade_get_alugueis(cidade),loc_id(moradia_get_loc(li_get_valor(list_get_atual(moradias)))));
				fprintf(res,"Este contrato estava ");
				switch(loc_get_disponibilidade(moradia_get_loc(li_get_valor(list_get_atual(moradias))))) {
					case DISP:
						fprintf(res,"disponível");
					break;

					case LOCADA:
						fprintf(res,"vigente");
					break;	

					case ENCERR:
						fprintf(res,"encerrado.");					
				}
				fprintf(res,"\n");
				loc_del(moradia_get_loc(li_get_valor(list_get_atual(moradias))));				
			}
			t++;
			free(li_get_valor(list_get_atual(moradias)));
		}	
		list_del(moradias,list_get_index(moradias));					
	}
	return t;
}

void arv_arq (FILE * arq, void * arv) {
	while(arq != NULL && arv != NULL) {
		fprintf(arq,"\n\t\t%f [label=\"x = %f \n\t\t\txMin = %f \n\t\t\txMax = %f  \n\t\t\t %d quadras: ",arv_get_chave(arv),arv_get_chave(arv),arv_get_min(arv),arv_get_max(arv),list_get_len(arv_get_valor(arv)));
		int c, d = list_get_len(arv_get_valor(arv));
		if(d > 4)
			d = 4;
		for(c = 0; c < d; c++) 				
			fprintf(arq," \n\t\t\t\tCEP[%d] %s\ty = %f",c,quadra_get_cep(li_get_valor(list_get(arv_get_valor(arv),c))),quadra_get_y(li_get_valor(list_get(arv_get_valor(arv),c))));
		if(c < list_get_len(arv_get_valor(arv)))	
			fprintf(arq,"\n\t\t\t\t....");	
	//	else fprintf(arq,"\n\t\t");	
		fprintf(arq,"\n\t\t\tAltura = %d \n\t\t\tFat. Balanc. = %d\"]",arv_get_altura(arv),avl_dif_alt(arv));	
		if(arv_get_esq(arv) != NULL) 
			fprintf(arq,"\n\t\t %f -> %f ;",arv_get_chave(arv),arv_get_chave(arv_get_esq(arv)));			
		if(arv_get_dir(arv) != NULL) 
			fprintf(arq,"\n\t\t %f -> %f ;",arv_get_chave(arv),arv_get_chave(arv_get_dir(arv)));
		fprintf(arq,"\n");	
		arv_arq(arq,arv_get_dir(arv));
		arv = arv_get_esq(arv);		
		
	} 
}

void dmpt (char * dot, void * avl, int size) {
	FILE * grafo = fopen(dot,"w");
	if(grafo == NULL || dot == NULL)
		return;
	fprintf(grafo, "digraph Gadm");	
	int c;
	for(c = 1 + ult_ind('-',dot); dot[c] != '\0'; c++)
		if((dot[c] >= '0' && dot[c] <= '9') || (dot[c] <= 'Z' && dot[c] >= 'A') || (dot[c] <= 'z' && dot[c] >= 'a'))
			fprintf(grafo,"%c",dot[c]);
		else fprintf(grafo,"_");	
	fprintf(grafo, "{\n\tsize=\"%d\"\n\tnode [color=aquamarine, style=filled]\n",size);
	arv_arq(grafo,avl);
	/*	
	x = %f
	xMin = %f
	xMax = %f
	Altura = %d
	Fat. Balanc. = %d
	%d quadras:
		CEP[0]  
		CEP[1] 
		CEP[2] 
		CEP[3] 
		....
	*/
	fprintf(grafo,"\n} ");
	fclose(grafo);
	printf("Criado o arquivo \"%s\" \n",dot);
}

void * cidade_qry (void * cid, char * qry, char * svg, char * txt) {
	if(qry == NULL || cid == NULL)
		return NULL;

	void * a,*b;	
	FILE * q = fopen(qry,"r");	
	if(q == NULL) {
		printf("Arquivo \"%s\" não encontrado.\n",qry);
		return cid;
	}
	char * svg_parcial = arq_nome_concat(0,'_','.',svg,"parcial.svg.");
	FILE * svg_out = fopen(svg_parcial,"w");
	FILE * txt_out = fopen(txt,"w");
	if(svg_out == NULL) {
		printf(" Não foi possível criar o arquivo %s\n",svg_parcial);
		svg_out = fopen(svg,"w");
	} else fprintf(svg_out,"<svg>Parcial \n");	
	if(txt_out == NULL)	
		printf(" Não foi possível criar o arquivo %s\n",txt);
	
	unsigned int total = 0;
	unsigned int exec = 0;
	unsigned int qtd_catac = 0, qtd_del = 0, qtd_dm = 0, qtd_dmpt = 0, qtd_dloc = 0, qtd_loc = 0, qtd_loci = 0, qtd_oloc = 0, qtd_oloci = 0, qtd_hom = 0, qtd_mul = 0, qtd_mud = 0, qtd_m = 0;
	int id_tam = 3*CEP_TAM;
	int c,com_tam = 2*COM_TAM;
	float s,t,u,v;
	void *r;
	char *m,*n;
	char cpf[CPF_TAM];
	char cep[CEP_TAM];
	char com[com_tam];
	char sfx[id_tam];	
	while(!feof(q)) {

		clean(com,com_tam);
		fscanf(q,"%s",com);
		total++;

		switch (com[0]) {
			case 'm':				
				clean(cep,CEP_TAM);				
				if(com[1] == '?') { // m?
					fscanf(q,"%s",cep);					
					fprintf(txt_out,"\nm?\t%s\n",cep);
					printf("\n%u\tm?\t%s\n",exec,cep);
					b = hash_get(cidade_get_moradias_cep(cid),cep);					
					for(c = 0; c < list_get_len(b); c++) { // lista nula tem len = -1
						a = hash_get(cidade_get_pessoas(cid),moradia_get_cpf(li_get_valor(list_get(b,c))));
						fprintf(txt_out,"\n");
						registrar_pessoa(a,txt_out);
						if(moradia_get_cpf(li_get_valor(list_get(b, c))) == NULL)
							printf(" Moradia %s/%c/%d\t%s sem CPF associado\n",moradia_get_cep(li_get_valor(list_get(b,c))),moradia_get_face(li_get_valor(list_get(b,c))),moradia_get_num(li_get_valor(list_get(b,c))),moradia_get_compl(li_get_valor(list_get(b,c))));
						else fprintf(txt_out,"CPF:\t%s\n",moradia_get_cpf(li_get_valor(list_get(b,c))));
						registrar_moradia(li_get_valor(list_get(b, c)), txt_out);
						registrar_aluguel(moradia_get_loc(li_get_valor(list_get(b, c))), txt_out);
					}	
					exec++;
					qtd_m++;
					if(c <= 0)
						fprintf(txt_out,"Moradias com CEP %s não foram encontradas.\n",cep);
					
				} else if(com[2] == 'd') { // mud
					clean(cpf,CPF_TAM);
					fscanf(q,"%s %s %s %d",cpf,cep,com,&c);					
					n = malloc(sizeof(char)*comprimento(cep));
					copy(cep,n);
					clean(cep,CEP_TAM);
					fscanf(q,"%s",cep);
					m = malloc(sizeof(char)*comprimento(cep));
					copy(cep,m);
					printf("\n%u\tmud\t%s %s %c %d %s\n",exec,cpf,n,com[0],c,cep);
					fprintf(txt_out,"\nmud\t%s %s %c %d %s\n",cpf,n,com[0],c,m);
					b = hash_get(cidade_get_moradias_cpf(cid), cpf);
					a = hash_get(cidade_get_pessoas(cid), cpf);
					v = -1;
					u = -1;					
					t = -1;
					s = -1;					
					loc_set_disponibilidade(moradia_get_loc(b),DISP);
					registrar_pessoa(a, txt_out);
					fprintf(txt_out, "CPF:\t%s\n", cpf);					
					if(a == NULL) printf("\tCPF %s não encontrado!\n", cpf);					
					if(b != NULL) {
						fprintf(txt_out,"\nEndereço antigo:\n");
						registrar_moradia(b,txt_out);
						fprintf(txt_out,"\n");
						r = cidade_get_quadra_hash(cid,moradia_get_cep(b));
						if(r == NULL)
							printf("\tQuadra CEP %s não encontrada!\n", moradia_get_cep(b));
						else {	
							u = moradia_get_x(b,r);
							v = moradia_get_y(b,r);							
							svg_circle(svg_out,NULL,"red","white","4px",u,v,16);
						} 
						if(moradia_get_loc(b) != NULL) {
							printf(" Moradia %s/%c/%d\t%s é alugada por %s ",moradia_get_cep(b),moradia_get_face(b),moradia_get_num(b),moradia_get_compl(b),moradia_get_cpf(b));
							if(a != NULL)
								printf("%s\t%c\t%s %s",pessoa_datanas(a),pessoa_gen(a),pessoa_nome(a),pessoa_sobrenome(a));							
							// e se for alugada? o que fazemos? encerramos o contrato? mudamos o contrato também?							
							hash_del(cidade_get_moradias_cpf(cid),cpf);							
							moradia_set_cpf(b,NULL);							
							printf("\t%c\n",loc_get_disponibilidade(moradia_get_loc(b)));
							b = NULL;
						} else {
							cidade_del_moradia_cpf(cid, cpf);						
							fprintf(txt_out,"Endereço novo");							
						}	
					/*	void * l = hash_get(cidade_get_moradias_cep(cid),moradia_get_cep(b));
						int d = list_get_len(l);
						while(d > 0) {
							d--;
							if(li_get_valor(list_get(l,d)) == b || (moradia_get_num(li_get_valor(list_get(l,d))) == moradia_get_num(b) && moradia_get_face(li_get_valor(list_get(l,d))) == moradia_get_face(b) && 0 == compare(moradia_get_compl(b),moradia_get_compl(li_get_valor(list_get(l,d))))))
								list_del(l,d);
						} //*/						
					} 
					if(b == NULL) {
						fprintf(txt_out, "Novo endereço");						
						printf("\tMoradia registrada no CPF %s não encontrada!\n",cpf);
						b = new_moradia(c, com[0], m, n, pessoa_cpf(a));						
						if(moradia_get_cpf(b) == NULL || pessoa_cpf(a) == NULL || a == NULL) {
							moradia_set_cpf(b, malloc(sizeof(char) * CPF_TAM));
							copy(cpf, moradia_get_cpf(b));
						}	
					} 										
					fprintf(txt_out, ": \n");
					moradia_set_face(b,com[0]);
					moradia_set_compl(b,m);
					moradia_set_cep(b,n);
					moradia_set_num(b,c);					
					cidade_set_moradia(cid,b);
					registrar_moradia(b,txt_out);										
					r = cidade_get_quadra_hash(cid,n);
					if(r == NULL)
						printf("\tQuadra CEP %s não encontrada!\n", n);
					else {	
						s = moradia_get_x(b, r);
						t = moradia_get_y(b, r);							
						if(u != -1 || v != -1) 
							svg_line(svg_out, "red", "3px", u, v, s, t);						
						svg_circle(svg_out,NULL,"blue","white","4px",s,t,16);
					}					
					exec++;
					qtd_mud++;
				} else if(com[2] == 'l') { // mul					
					fscanf(q, "%f %f %f %f", &u, &v, &t, &s);
					fprintf(txt_out,"\nmul\t%f %f %f %f\n",u,v,t,s);
					printf("\n%u\tmul\t%f %f %f %f\n", exec, u, v, t, s);					
					printf("%d mulheres encontradas.\n", registrar_gen_por_moradia(txt_out, svg_out, cid, "magenta", FEM, u,v,t,s));
					qtd_mul++;
					exec++;
				}
			break;

			case 'h': // hom
				if(com[2] == 'm') {
					fscanf(q, "%f %f %f %f", &u, &v, &t, &s);
					fprintf(txt_out,"\nhom\t%f %f %f %f\n",u,v,t,s);
					printf("\n%u\thom\t%f %f %f %f\n", exec, u, v, t, s);															
					printf("%d homens encontrados.\n", registrar_gen_por_moradia(txt_out, svg_out, cid, "cyan", MASC, u,v,t,s));
					qtd_hom++;
					exec++;
				}								
			break;

			case 'c': // catac
				fscanf(q, "%f %f %f %f", &u, &v, &t, &s);
				svg_rect_open(svg_out,com,"#AB37C8","#AA0044",NULL,u,v,t,s,0.5,0);
				svg_rect_close(svg_out);
				fprintf(txt_out,"\ncatac\t%f %f %f %f\n",u,v,t,s);
				printf("\n%u\tcatac\t%f %f %f %f\n", exec, u, v, t, s);
				r = cidade_get_quadras_em(cid,u,v,t,s);
				printf("%d quadras inteiramente na área.\n",list_get_len(r));
				for(c = 0; c < list_get_len(r); c++) 
					printf("%d moradias apagadas em %s\n",del(cid,quadra_get_cep(li_get_valor(list_get(r,c))),txt_out),quadra_get_cep(li_get_valor(list_get(r,c))));
				exec++;
				qtd_catac++;
			break;

			case 'd':
				clean(cep,CEP_TAM);
				clean(cpf,CPF_TAM);
				switch(com[1]){
					case 'l': //	dloc 	
						clean(sfx,id_tam);
						fscanf(q,"%s",sfx);
						fprintf(txt_out,"\ndloc\t%s\n",sfx);
						printf("\n%u\tdloc\t%s\n",exec,sfx);
						m = NULL;
						a = NULL;
						b = hash_get(cidade_get_alugueis(cid),sfx);
						loc_set_disponibilidade(b,ENCERR);
						if(b == NULL) 
							fprintf(txt_out,"Locação ID %s não encontrada.",sfx);
						else {								
							if(moradia_get_cpf(loc_get_moradia(b)) == NULL) {
								printf(" Locação ID %s",loc_id(b));
								if(loc_get_moradia(b) != NULL)
									printf("\t%s/%c/%d\t%s",moradia_get_cep(loc_get_moradia(b)),moradia_get_face(loc_get_moradia(b)),moradia_get_num(loc_get_moradia(b)),moradia_get_compl(loc_get_moradia(b)));
								printf(" não possui CPF relacionado. N");	
							} else {
								m = moradia_get_cpf(loc_get_moradia(b));
								a = cidade_get_pessoa(cid,m);
								hash_del(cidade_get_moradias_cpf(cid),m);
								registrar_pessoa(a,txt_out);
								fprintf(txt_out,"CPF:\t%s\n",m);								
								printf("CPF %s n",m);
							}	
							if(a == NULL)
								printf("ão identificado.");							
						}	
						printf("\n");	
						registrar_moradia(loc_get_moradia(b),txt_out);
						registrar_aluguel(b,txt_out);
						if(loc_get_disponibilidade(b) == ENCERR)
							fprintf(txt_out,"Encerrado\n");
						exec++;
						qtd_dloc++;
						break;

					case 'e': //	del	
						fscanf(q,"%s",cep);
						fprintf(txt_out,"\ndel\t%s\n",cep);
						printf("\n%u\tdel\t%s\n",exec,cep);
						printf("%d moradias apagadas.\n",del(cid,cep,txt_out));
						exec++;
						qtd_del++;
						break;

					case 'm':	
						if(com[2] == '?') { // dm?
							fscanf(q,"%s",cpf);
							fprintf(txt_out,"\n%s\t%s\n",com,cpf);
							printf("\n%u\t%s\t%s\n",exec,com,cpf);
							b = hash_get(cidade_get_moradias_cpf(cid),cpf);
							a = hash_get(cidade_get_pessoas(cid),cpf);
							registrar_pessoa(a, txt_out);
							fprintf(txt_out,"CPF:\t%s\n",cpf);					
							registrar_moradia(b,txt_out);
							if(a == NULL) {
								printf("\tCPF %s não encontrado!\n",cpf);
								fprintf(txt_out,"Não há informações sobre essa pessoa.\n");
							} 
							if(b == NULL)	{							
								fprintf(txt_out,"Não há moradia associada.\n");
								printf("\tMoradia para CPF %s não encontrada!\n",cpf);
							} else if(moradia_get_loc(b) != NULL) 								
								fprintf(txt_out,"A moradia é alugada:\n");
							else fprintf(txt_out, "Moradia própria \n");	
							registrar_aluguel(moradia_get_loc(b),txt_out);
							qtd_dm++;
							exec++;	
						} else if(com[3] == 't' && com[2] == 'p') { // dmpt
							clean(sfx, id_tam);
							fscanf(q,"%s",sfx);
							fprintf(txt_out,"\n%s\t%s\n",com,sfx);
							printf("\n%u\t%s\t%s\n",exec,com,sfx);
							dmpt(arq_nome_concat(0,'.','.',arq_nome_concat(1,'-','.',svg,sfx),"dot"),cidade_get_quadras_avl(cid),hash_get_len(cidade_get_quadras_hash(cid))*16);
							qtd_dmpt++;
							exec++;	
						}
				}
			break;

			case 'l':
				clean(sfx,id_tam);
				if(com[3] == '?') { // loc?
					fscanf(q,"%s",sfx);
					fprintf(txt_out,"\nloc?\t%s\n",sfx);
					printf("\n%u\tloc?\t%s\n",exec,sfx);
					r = hash_get(cidade_get_alugueis(cid),sfx);
					registrar_moradia(loc_get_moradia(r),txt_out);
					registrar_aluguel(r,txt_out);
					if(r == NULL)
						fprintf(txt_out,"Oferta não encontrada.\n");
					else {						
						fprintf(txt_out,"Contrato de locação ");
						switch (loc_get_disponibilidade(r))
						{
							case DISP:
								fprintf(txt_out,"disponível");
								break;

							case ENCERR:
								fprintf(txt_out,"encerrado.");
								break;	

							case LOCADA:
								fprintf(txt_out,"em vigência:");																						
						}
						fprintf(txt_out,"\n");
						if(moradia_get_cpf(loc_get_moradia(r)) != NULL) {
							fprintf(txt_out,"CPF:\t%s\n",moradia_get_cpf(loc_get_moradia(r)));							
							registrar_pessoa(cidade_get_pessoa(cid, moradia_get_cpf(loc_get_moradia(r))), txt_out);
						}
						
					}	
					qtd_loci++;
					exec++;
				} else if(com[2] == 'c' && com[1] == 'o') { // loc  					
					clean(cpf,CPF_TAM);						
					fscanf(q,"%s",sfx);
					fscanf(q,"%s",cpf);
					fprintf(txt_out,"\n%s\t%s %s\n",com,sfx,cpf);
					printf("\n%u\t%s\t%s %s\n",exec,com,sfx,cpf);
					b = hash_get(cidade_get_alugueis(cid),sfx);
					if(b == NULL)
						fprintf(txt_out,"Locação ID %s não encontrada.\n",cpf);
					else {	
						
						a = cidade_get_pessoa(cid,cpf);
						m = pessoa_cpf(a);
						registrar_pessoa(a,txt_out);												
						if(a == NULL) {							
							m = malloc(sizeof(char)*(1 + comprimento(cpf)));
							copy(cpf,m);
							printf("\tCPF %s não registrado\n",m);
						}							
						if(loc_get_moradia(b) != NULL) {
							r = hash_get(cidade_get_moradias_cpf(cid), cpf);
							if(r != NULL) {
								printf(" CPF %s já mora em %s/%c/%d\t%s\n",cpf,moradia_get_cep(r),moradia_get_face(r),moradia_get_num(r),moradia_get_compl(r));
								if(moradia_get_loc(r) == NULL) 
									cidade_del_moradia_cpf(cid,cpf);
								else hash_del(cidade_get_moradias_cpf(cid), cpf);
								loc_set_disponibilidade(moradia_get_loc(r), DISP);
								moradia_set_cpf(r,NULL);
							} 
							if(moradia_get_cpf(loc_get_moradia(b)) != NULL) {
								printf(" Moradia %s/%c/%d\t%s já é alugada por %s ",moradia_get_cep(loc_get_moradia(b)),moradia_get_face(loc_get_moradia(b)),moradia_get_num(loc_get_moradia(b)),moradia_get_compl(loc_get_moradia(b)),moradia_get_cpf(loc_get_moradia(b)));
								r = cidade_get_pessoa(cid,moradia_get_cpf(loc_get_moradia(b)));
								if(r != NULL)
									printf("%s\t%c\t%s %s",pessoa_datanas(r),pessoa_gen(r),pessoa_nome(r),pessoa_sobrenome(r));
								printf("\n");	
								hash_del(cidade_get_moradias_cpf(cid),moradia_get_cpf(loc_get_moradia(b)));
							}	
							hash_set(cidade_get_moradias_cpf(cid),m,loc_get_moradia(b));
							moradia_set_cpf(loc_get_moradia(b),m);						
							loc_set_disponibilidade(b,LOCADA);
							fprintf(txt_out,"CPF:\t%s\n",moradia_get_cpf(loc_get_moradia(b)));							
						} else printf("\tMoradia não identificada.\n");	
						registrar_moradia(loc_get_moradia(b), txt_out);												
					}	
					registrar_aluguel(b,txt_out);																
					qtd_loc++;
					exec++;
				}
				break;
		
			case 'o':				
				
				if(com[4] == '?') { // oloc?
					fscanf(q,"%f %f %f %f",&u,&v,&t,&s);
					svg_rect_open(svg_out,com,NULL,"red",NULL,u,v,t,s,0,10);
					svg_rect_close(svg_out);
					fprintf(txt_out,"\noloc?\t%f %f %f %f\n",u,v,t,s);
					printf("\n%u\toloc?\t%f %f %f %f\n",exec,u,v,t,s);
					r = cidade_get_moradias_em(cid, u, v, t, s);
					for(c = 0; c < list_get_len(r); c++)
						if(moradia_get_loc(li_get_valor(list_get(r,c))) != NULL && loc_get_disponibilidade(moradia_get_loc(li_get_valor(list_get(r,c)))) == DISP) {
							registrar_aluguel(moradia_get_loc(li_get_valor(list_get(r, c))), txt_out);
							registrar_moradia(li_get_valor(list_get(r, c)), txt_out);							
							fprintf(txt_out,"\n");
						}
					qtd_oloci++;
					exec++;
				} else if(com[3] == 'c' && com[2] == 'o' && com[1] == 'l') { // oloc  
					fprintf(txt_out,"\n%s\t",com);
					printf("\n%u\t%s\t",exec,com);
					
					clean(cep,CEP_TAM);
					clean(sfx,id_tam);
					fscanf(q,"%s %s %s %d",sfx,cep,com,&c);
					fprintf(txt_out,"%s %s %c %d",sfx,cep,com[0],c);
					printf("%s %s %c %d ", sfx, cep, com[0], c);
					n = malloc(sizeof(char)*(1+comprimento(cep)));
					copy(cep,n);
					clean(cep,CEP_TAM);
					fscanf(q,"%s %f %f",cep,&u,&v);
					printf("%s %f %f\n",cep,u,v);
					fprintf(txt_out," %s %f %f\n",cep,u,v);
					r = new_moradia(c,com[0],malloc(sizeof(char) * (1 + comprimento(cep))),n,NULL);
					copy(cep,moradia_get_compl(r));
				//	cidade_set_moradia(cid,r);
					fprintf(txt_out,"Imóvel do endereço:\n");
					registrar_moradia(r,txt_out);
					fprintf(txt_out,"Oferta de Locação: \n");
					moradia_set_loc(r,new_loc(v,u,sfx,r));// criar LOC
					cidade_set_aluguel(cid,moradia_get_loc(r));
					registrar_aluguel(moradia_get_loc(r),txt_out);



					
					
					qtd_oloc++;
					exec++;
				}								
		}

	}

	svg_close(svg_out);
	if(svg_out != NULL)
		fclose(svg_out);
	
	printf("\nArquivo \"%s\" ",svg);	
	svg_out = fopen(svg,"w");
	svg_open(svg_out);
	svg_comment(svg_out,"\n202000560125\nGuilherme Akira Demenech Mori\n\t");
	svg_comment(svg_out,svg);	
	quadras_svg(svg_out,cidade_get_quadras_avl(cid));
	FILE * svg_append = fopen(svg_parcial,"r");
	com_tam = 0;
	if(svg_append != NULL) {
		while(!feof(svg_append)) {
			c = fgetc(svg_append);
			if(c == ' ')
				com_tam = 1;
			c *= com_tam;	
			if(c > 0)
				fprintf(svg_out,"%c",c);
		}		
		fclose(svg_append);
	} // o parcial já fechou			
//	svg_close(svg_out); 

	printf("\nExecutadas %u consultas dentre %u potenciais comandos lidos.\n",exec,total);
	if(qtd_catac != 0)
		printf("catac\t%u\t",qtd_catac);
	if(qtd_del != 0)	
		printf("del\t%u\n",qtd_del);

	if(qtd_dloc != 0)	
		printf("dloc\t%u\t",qtd_dloc);
	if(qtd_loc != 0)	
		printf("loc\t%u\n",qtd_loc);
	
	if(qtd_oloci != 0)
		printf("oloc?\t%u\t",qtd_oloci);
	if(qtd_loci != 0)
		printf("loc?\t%u\n",qtd_loci);

	if(qtd_oloc != 0)	
		printf("oloc\t%u\t",qtd_oloc);
	if(qtd_dmpt != 0)	
		printf("dmpt\t%u\n",qtd_dmpt);

	if(qtd_mud != 0)	
		printf("mud\t%u\t",qtd_mud);
	if(qtd_dm != 0)	
		printf("dm?\t%u\n",qtd_dm);
	
	if(qtd_mul != 0)
		printf("mul\t%u\t",qtd_mul);
	if(qtd_m != 0)		
		printf("m?\t%u\n",qtd_m);
	if(qtd_hom != 0)	
		printf("hom\t%u\n",qtd_hom);
	printf("\n");
	
	
	

	if(svg_out != NULL)
		fclose(svg_out);
	if(txt_out != NULL)	
		fclose(txt_out);
	fclose(q);
	return cid;
}
