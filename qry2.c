#include <stdio.h>
#include<stdlib.h>
#include"quadra.h"
#include"vias.h"
#include"mapa.h"
#include"hash.h"
#include"list.h"
#include "qry.h"
#include "svg.h"
#include "arq.h"
#include "arv.h"
#define MIN 0.01

void cx_svg (FILE * svg, void * compfortconex) {
	char cor[HEX];
	int c = list_get_len(compfortconex), i;
	while(c > 0) {
		hex64(cor, c);
	
		c--;
		for(i = 0; i < list_get_len(li_get_valor(list_get(compfortconex, c))); i++) 
			svg_ellipse(svg,NULL,cor,NULL,NULL,vert_x(li_get_valor(list_get(li_get_valor(list_get(compfortconex,c)),i)))-5,vert_y(li_get_valor(list_get(li_get_valor(list_get(compfortconex,c)),i)))-5,16+(i%6),12+(c%4));		
	}
}

int rv (FILE * svg, FILE * txt, void * vis, void * arv, void * raiz, char * cor, float fator) {
	if(raiz == NULL || txt == NULL)
		return -2;
	svg_circle(svg,NULL,cor,cor,NULL,vert_x(raiz),vert_y(raiz),10);	
	int c;
	int reduzidos = -1, decremento_em = 0, decremento_prox = 1;
	float reduz = 1;
	void * fila = new_list(0);
	void * adj, * prox;
	list_add(fila, raiz);
	while(list_get_len(fila) > 0) {
		raiz = li_get_valor(list_get(fila, 0));
		adj = hash_get(arv, vert_id(raiz));
		list_del(fila, 0);
		

		if(decremento_em == 0) {			
			reduzidos += decremento_prox;
			reduz -= fator;
			if(reduz <= MIN) 
				reduz = MIN;
			else decremento_em = decremento_prox;	
			decremento_prox = 0;
			fprintf(txt, "\n\tReduzindo a %f%%: \n", reduz * 100);
		}
		
		for(c = 0; c < list_get_len(adj); c++) {
			prox = via_get_para(li_get_valor(list_get(adj, c)));
			if(prox == raiz)
				prox = via_get_de(li_get_valor(list_get(adj, c)));
			if(hash_get(vis, vert_id(prox)) != NULL)	
				continue;
			hash_set(vis, vert_id(prox), raiz);	
			list_add(fila, prox);	

			via_set_vm(li_get_valor(list_get(adj,c)),reduz*via_get_vm(li_get_valor(list_get(adj,c))));
			svg_line(svg,cor,"4px",vert_x(via_get_de(li_get_valor(list_get(adj,c)))),vert_y(via_get_de(li_get_valor(list_get(adj,c)))),vert_x(via_get_para(li_get_valor(list_get(adj,c)))),vert_y(via_get_para(li_get_valor(list_get(adj,c)))));
			fprintf(txt,"\n\tDe:\t%s\t(%f %f)\n\tPara:\t%s\t(%f %f)\n\t%f m\t(%f m/s)\n\t %s %s\n",vert_id(via_get_de(li_get_valor(list_get(adj,c)))),vert_x(via_get_de(li_get_valor(list_get(adj,c)))),vert_y(via_get_de(li_get_valor(list_get(adj,c)))),vert_id(via_get_para(li_get_valor(list_get(adj,c)))),vert_x(via_get_para(li_get_valor(list_get(adj,c)))),vert_y(via_get_para(li_get_valor(list_get(adj,c)))),via_get_cmp(li_get_valor(list_get(adj,c))),via_get_vm(li_get_valor(list_get(adj,c))),via_esq(li_get_valor(list_get(adj,c))),via_dir(li_get_valor(list_get(adj,c))));
			decremento_prox++;
		}						
		decremento_em--;
	}
	list_del_all(fila);
	return reduzidos;
}

int del (void * cidade, char * cep, FILE * res) {
	if(cep == NULL || res == NULL)
		return 0;		
	void * quadra = cidade_get_quadra_hash(cidade, cep);	
	cidade_del_quadra_hash(cidade, cep);	
	if(quadra == NULL) {
		printf("Quadra de CEP %s não encontrada.\n",cep);
		fprintf(res,"\nQuadra de CEP %s não encontrada.\n",cep);		
		return 0;
	} 
	fprintf(res,"\n\tCEP:\t%s\n\t(%f %f) %fx%f\n\t %s  %s %s \n",quadra_get_cep(quadra),quadra_get_x(quadra),quadra_get_y(quadra),quadra_get_w(quadra),quadra_get_h(quadra),quadra_get_esp(quadra),quadra_get_strk(quadra),quadra_get_fill(quadra));
	free(quadra);		
	return 1;
}

void * cidade_qry (void * cid, char * qry, char * svg, char * txt) {
	if(qry == NULL || cid == NULL)
		return NULL;

	void * a, * b, * cx2 = NULL;	
	FILE * q = fopen(qry, "r");	
	if(q == NULL) {
		printf("Arquivo \"%s\" não encontrado.\n",qry);
		return cid;
	}
	char * svg_parcial = arq_nome_concat(0,'.','.',svg,"parcial.svg.");
	FILE * svg_out = fopen(svg_parcial, "w");
	FILE * txt_out = fopen(txt, "w");
	if(svg_out == NULL) {
		printf(" Não foi possível criar o arquivo %s\n",svg_parcial);
		svg_out = fopen(svg, "w");
	} else fprintf(svg_out,"<svg>%s \n",svg_parcial);	
	if(txt_out == NULL)	
		printf(" Não foi possível criar o arquivo %s\n",txt);
	else fprintf(txt_out,"202000560125\tGuilherme Akira Demenech Mori \n");	
	
	unsigned int total = 0;
	unsigned int exec = 0;
	unsigned int qtd_catac = 0, qtd_del = 0, qtd_cx = 0, qtd_rv = 0, qtd_o = 0, qtd_p = 0;
	int id_tam = 3*CEP_TAM, linha = 0;
	int c,d,com_tam = 2*COM_TAM;
	float s,t,u,v,f;
	void *r;
	char *m,*n;
	char cep[CEP_TAM];
	char com[com_tam];
	char sfx[id_tam];	
	while(!feof(q)) {

		clean(com,com_tam);
		fscanf(q,"%s",com);
		total++;

		switch (com[0]) {

			case '@': // @o?
				qtd_o++;
				exec++;
				break;

			case 'p': // p?
				qtd_p++;
				exec++;
				break;	

			case 'r': // rv	
				fscanf(q, "%f %f %f %f %f", &u, &v, &t, &s, &f);
				fprintf(txt_out, "\nrv\t%f %f %f %f %f\n", u, v, t, s, f);
				printf("\n%u\trv\t%f %f %f %f %f\n", exec, u, v, t, s, f);
				svg_rect_open(svg_out, com, NULL, "#AB37C8", "5px", u, v, t, s, 1, 10);
				svg_rect_close(svg_out);
				n = cidade_get_pontos_em(cid, u, v, t, s);
				m = cidade_get_vias_em(n);
				if(f < 0 || f > 1)
					printf("fator f %f fora do domínio.\n",f);
			//	list_del_all(n);
				r = new_list(0);
				a = kruskal(m, r);
				b = new_hash_table(list_get_len(r));
				printf("%d árvores geradoras mínimas na área \n", list_get_len(r));
				fprintf(txt_out,"%d árvores geradoras mínimas:\n",list_get_len(r));
				for(c = 0; c < list_get_len(n); c++) {
					if(hash_get(b,vert_id(li_get_valor(list_get(n,c))))!=NULL)
						continue;
					d = rv(svg_out, txt_out, b, a, li_get_valor(list_get(n, c)), "hotpink", f); 
					fprintf(txt_out, "\n%d velocidades reduzidas. \nRaiz:\t%s\n", d, vert_id(li_get_valor(list_get(n, c))));
					if(d > 0) {
						if(cidade_get_vias_cmp(cid) != NULL)
							printf("invalidando caminhos mais rápidos.\n");
						hash_del_all(cidade_get_vias_cmp(cid));	
						cidade_set_vias_cmp(cid, NULL);
					}
					
				}
				hash_del_all(b);
				hash_del_all(a);
				list_del_all(n);
				list_del_all(m);
				list_del_all(r);
				exec++;
				qtd_rv++;
			break;

			case 'c': 
				fscanf(q, "%f", &u);
				if(com[1] == 'x') { // cx
					fprintf(txt_out,"\ncx\t%f\n",u);
					printf("\n%u\tcx\t%f\n",exec,u);
					if(cx2 != NULL) {
						fprintf(txt_out,"recalculando regiões isoladas\n");
						printf("recalculando regiões isoladas....\n");
					}	
					list_del_all(cx2);
					cx2 = kosaraju(cidade_get_pontos(cid),u);
					for(c = 0; c < list_get_len(cx2); c++) {
						hex64(cep, c + 1);
						fprintf(txt_out, "\n\t%s\tRegião %d\t[%d vértices]: \n", cep, c, list_get_len(li_get_valor(list_get(cx2, c))));
						for(d = 0; d < list_get_len(li_get_valor(list_get(cx2, c))); d++) 
							fprintf(txt_out,"\t%s\t(%f %f)\n",vert_id(li_get_valor(list_get(li_get_valor(list_get(cx2,c)),d))),vert_x(li_get_valor(list_get(li_get_valor(list_get(cx2,c)),d))),vert_y(li_get_valor(list_get(li_get_valor(list_get(cx2,c)),d))));
					}
					fprintf(txt_out,"\n%d regiões isoladas encontradas. \n",list_get_len(cx2));
					qtd_cx++;
				} else { // catac
					fscanf(q, "%f %f %f", &v, &t, &s);
					svg_rect_open(svg_out, com, "#AB37C8", "#AA0044", NULL, u, v, t, s, 0.5, 0);
					svg_rect_close(svg_out);
					fprintf(txt_out, "\ncatac\t%f %f %f %f\n", u, v, t, s);
					printf("\n%u\tcatac\t%f %f %f %f\n", exec, u, v, t, s);
					r = cidade_get_quadras_em(cid,u,v,t,s);
					d = 0;
					printf("%d quadras inteiramente na área.\n", list_get_len(r));
					for(c = 0; c < list_get_len(r); c++) 
						d += del(cid, quadra_get_cep(li_get_valor(list_get(r, c))), txt_out);				
					fprintf(txt_out,"\n%d quadras apagadas.\n",d);
					printf("%d quadras apagadas \n",d);	
					r = cidade_get_pontos_em(cid,u,v,t,s);
					printf("%d vértices inteiramente na área.\n", list_get_len(r));	
					for(c = 0; c < list_get_len(r); c++) { 
						fprintf(txt_out,"\n\n\tDe:\t%s\n\t(%f %f)\n",vert_id(li_get_valor(list_get(r,c))),vert_x(li_get_valor(list_get(r,c))),vert_y(li_get_valor(list_get(r,c))));
						for(d = 0; d < list_get_len(vert_get_vias(li_get_valor(list_get(r,c)))); d++) 
							if(via_get_de(li_get_valor(list_get(vert_get_vias(li_get_valor(list_get(r,c))),d))) == li_get_valor(list_get(r,c)))
								fprintf(txt_out,"\n\tPara:\t%s\n\t%f m\t(%f m/s)\n\t %s %s \n",vert_id(via_get_para(li_get_valor(list_get(vert_get_vias(li_get_valor(list_get(r,c))),d)))),via_get_cmp(li_get_valor(list_get(vert_get_vias(li_get_valor(list_get(r,c))),d))),via_get_vm(li_get_valor(list_get(vert_get_vias(li_get_valor(list_get(r,c))),d))),via_esq(li_get_valor(list_get(vert_get_vias(li_get_valor(list_get(r,c))),d))),via_dir(li_get_valor(list_get(vert_get_vias(li_get_valor(list_get(r,c))),d))));							
						d = cidade_del_ponto(cid, vert_x(li_get_valor(list_get(r, c))), vert_y(li_get_valor(list_get(r, c))));						 						
						if(d > 0) { 
							printf("\t%d referências a arestas apagadas em %s \n", d, vert_id(li_get_valor(list_get(r, c))));
							for(d = 0; d < 2; d++) {
								if(cidade_get_vias(cid, d) != NULL)
									printf("[%d]\tInvalidando caminhos calculados\n",d);
								hash_del_all(cidade_get_vias(cid, d));
								cidade_set_vias(cid, NULL, d);	
							}	
						}
					}	
					fprintf(txt_out,"\n\n%d vértices apagados. \n",list_get_len(r));
					qtd_catac++;
				}				
				exec++;
			break;

			case 'd': // del
				clean(cep,CEP_TAM);
				fscanf(q,"%s",cep);
				fprintf(txt_out, "\ndel\t%s\n", cep);
				printf("\n%u\tdel\t%s\n", exec, cep);
				del(cid, cep, txt_out);				
				qtd_del++;
				exec++;
			

			
		
											
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
	cx_svg(svg_out,cx2);
	list_del_all(cx2);
	vias_svg(svg_out,cidade_get_pontos(cid));
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
 

	printf("\nExecutadas %u consultas dentre %u potenciais comandos lidos.\n",exec,total);
	if(qtd_catac != 0)
		printf("catac\t%u\t",qtd_catac);
	if(qtd_del != 0)	
		printf("del\t%u\n",qtd_del);

	
	
	if(qtd_cx != 0)
		printf("cx\t%u\t",qtd_cx);
	if(qtd_rv != 0)
		printf("rv\t%u\n",qtd_rv);

	if(qtd_p != 0)	
		printf("p?\t%u\t",qtd_p);
	if(qtd_o != 0)	
		printf("@o?\t%u\n",qtd_o);	

	

	
	
	
	printf("\n");
	
	
	

	if(svg_out != NULL)
		fclose(svg_out);
	if(txt_out != NULL)	
		fclose(txt_out);
	fclose(q);
	return cid;
}
