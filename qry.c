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
	printf("Iniciando busca ");
	void * moradias = cidade_get_moradias_em(cidade, x, y, w, h);
	void * pessoa;
	printf("terminada");
	int c, p = 0;
	for(c = 0; c < list_get_len(moradias); c++) {
		if(moradia_get_cpf(li_get_valor(list_get(moradias,c))) != NULL) {
			pessoa = cidade_get_pessoa(cidade, moradia_get_cpf(li_get_valor(list_get(moradias, c))));
			if(pessoa_gen(pessoa) == gen) {
				fprintf(txt,"\n");
				registrar_pessoa(pessoa,txt);
				fprintf(txt,"CPF:\t%s\n",moradia_get_cpf(li_get_valor(list_get(moradias,c))));
				registrar_moradia(li_get_valor(list_get(moradias, c)), txt);
				p++;
			}
		}
	}
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
	int c,com_tam = 2*COM_TAM;
	float s,t,u,v;
	void *r;
	char *m,*n;
	char cpf[CPF_TAM];
	char cep[CEP_TAM];
	char com[com_tam];	
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
					/*	void * l = hash_get(cidade_get_moradias_cep(cid),moradia_get_cep(b));
						int d = list_get_len(l);
						while(d > 0) {
							d--;
							if(li_get_valor(list_get(l,d)) == b || (moradia_get_num(li_get_valor(list_get(l,d))) == moradia_get_num(b) && moradia_get_face(li_get_valor(list_get(l,d))) == moradia_get_face(b) && 0 == compare(moradia_get_compl(b),moradia_get_compl(li_get_valor(list_get(l,d))))))
								list_del(l,d);
						} //*/
						cidade_del_moradia_cpf(cid,cpf);
						fprintf(txt_out,"\nEndereço novo");
					} else {
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

			case 'h':
				if(com[2] == 'm') {
					fscanf(q, "%f %f %f %f", &u, &v, &t, &s);
					fprintf(txt_out,"\nhom\t%f %f %f %f\n",u,v,t,s);
					printf("\n%u\thom\t%f %f %f %f\n", exec, u, v, t, s);										
					printf("%d homens encontrados.\n", registrar_gen_por_moradia(txt_out, svg_out, cid, MASC, u,v,t,s));
					exec++;
				}
				clean(cep,CEP_TAM);
				
			break;
		
			case 'n':				
				printf("Tamanho das tabelas\n");				
		}

	}

	printf("\nExecutadas %u consultas dentre %u potenciais comandos lidos.\n",exec,total);
	

	
	fclose(svg_out);
	fclose(txt_out);
	fclose(q);
	return cid;
}
