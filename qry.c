#include <stdio.h>
#include<stdlib.h>
#include"cidade.h"
#include"pessoas.h"
#include"moradias.h"
#include"quadra.h"
#include"hash.h"
#include"list.h"
#include "qry.h"
#include "arq.h"



void registrar_aluguel (void * loc, FILE * arq) {
	if(loc != NULL && arq != NULL) {

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

int registrar_gen_por_moradia (FILE * txt, FILE * svg, void * cidade, char gen, float x, float y, float w, float h) {
	void * moradias = cidade_get_moradias_em(cidade, x, y, w, h);
	void * pessoa;	
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
				p++;
			}
			t++;
		}
	}
	printf("%d pessoas possíveis em %d moradias promissoras.\n",t,list_get_len(moradias));
	list_del_all(moradias);
	return p;
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
	FILE * txt_out = fopen(txt,"w");
	FILE * svg_out = fopen(svg,"w");
	
	unsigned int total = 0;
	unsigned int exec = 0;
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
					if(b == NULL) 
						fprintf(txt_out,"A quadra %s não foi encontrada.\n",cep);
					else for(c = 0; c < list_get_len(b); c++) {
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
					registrar_pessoa(a, txt_out);
					fprintf(txt_out, "CPF:\t%s\n", cpf);					
					if(a == NULL) printf("\tCPF %s não encontrado!\n", cpf);					
					if(b != NULL) {
						fprintf(txt_out,"\nEndereço antigo:\n");
						registrar_moradia(b,txt_out);
						if(moradia_get_loc(b) != NULL) {
							printf(" Moradia %s/%c/%d\t%s é alugada por %s ",moradia_get_cep(b),moradia_get_face(b),moradia_get_num(b),moradia_get_compl(b),moradia_get_cpf(b));
							if(a != NULL)
								printf("%s\t%c\t%s %s",pessoa_datanas(a),pessoa_gen(a),pessoa_nome(a),pessoa_sobrenome(a));
							printf("\n");
							// e se for alugada? o que fazemos? encerramos o contrato? mudamos o contrato também?							
							moradia_set_cpf(b,NULL);
							b = NULL;
						} else fprintf(txt_out,"\nEndereço novo");
					/*	void * l = hash_get(cidade_get_moradias_cep(cid),moradia_get_cep(b));
						int d = list_get_len(l);
						while(d > 0) {
							d--;
							if(li_get_valor(list_get(l,d)) == b || (moradia_get_num(li_get_valor(list_get(l,d))) == moradia_get_num(b) && moradia_get_face(li_get_valor(list_get(l,d))) == moradia_get_face(b) && 0 == compare(moradia_get_compl(b),moradia_get_compl(li_get_valor(list_get(l,d))))))
								list_del(l,d);
						} //*/
						cidade_del_moradia_cpf(cid,cpf);						
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
					exec++;
				} else if(com[2] == 'l') { // mul					
					fscanf(q, "%f %f %f %f", &u, &v, &t, &s);
					fprintf(txt_out,"\nmul\t%f %f %f %f\n",u,v,t,s);
					printf("\n%u\tmul\t%f %f %f %f\n", exec, u, v, t, s);					
					printf("%d mulheres encontradas.\n", registrar_gen_por_moradia(txt_out, svg_out, cid, FEM, u,v,t,s));
					exec++;
				}
			break;

			case 'h': // hom
				if(com[2] == 'm') {
					fscanf(q, "%f %f %f %f", &u, &v, &t, &s);
					fprintf(txt_out,"\nhom\t%f %f %f %f\n",u,v,t,s);
					printf("\n%u\thom\t%f %f %f %f\n", exec, u, v, t, s);															
					printf("%d homens encontrados.\n", registrar_gen_por_moradia(txt_out, svg_out, cid, MASC, u,v,t,s));
					exec++;
				}								
			break;

			case 'c': // catac

			break;

			case 'd':
				clean(cep,CEP_TAM);
				clean(cpf,CPF_TAM);
				switch(com[1]){
					case 'l': //	dloc 	
						exec++;
						break;

					case 'e': //	del	
						exec++;
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
							registrar_aluguel(moradia_get_loc(b),txt_out);
							exec++;	
						} else if(com[2] == 't' && com[3] == 'p') { // dmpt
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

					exec++;
				} else if(com[2] == 'c' && com[1] == 'o') { // loc  
					clean(cpf,CPF_TAM);
					fscanf(q,"%s %s",sfx,cpf);
					
					fprintf(txt_out,"\n%s\t%s %s\n",com,sfx,cpf);
					printf("\n%u\t%s\t%s %s\n",exec,com,sfx,cpf);

					exec++;
				}
				break;
		
			case 'o':				
				
				if(com[4] == '?') { // oloc?
					fscanf(q,"%s",sfx);
					fprintf(txt_out,"\noloc?\t%s\n",sfx);
					printf("\n%u\toloc?\t%s\n",exec,sfx);

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
					cidade_set_moradia(cid,r);
					fprintf(txt_out,"Imóvel do endereço:\n");
					registrar_moradia(r,txt_out);
					fprintf(txt_out,"Oferta de Locação: \n");
					// criar LOC
					registrar_aluguel(moradia_get_loc(r),txt_out);



					
					

					exec++;
				}								
		}

	}

	printf("\nExecutadas %u consultas dentre %u potenciais comandos lidos.\n",exec,total);
	

	
	fclose(svg_out);
	fclose(txt_out);
	fclose(q);
	return cid;
}
