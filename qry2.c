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
		c--;
		
		if(list_get_len(li_get_valor(list_get(compfortconex, c))) > 1)
			hex64(cor, c);
		else {	
			cor[0] = 'g';
			cor[1] = 'r';
			cor[2] = 'a' + (('e' - 'a') * (c % 2));
			cor[3] = 'y';
			cor[4] = '\0';
		}	
	
		
		for(i = 0; i < list_get_len(li_get_valor(list_get(compfortconex, c))); i++) 
			svg_ellipse(svg,NULL,cor,NULL,NULL,vert_x(li_get_valor(list_get(li_get_valor(list_get(compfortconex,c)),i)))-5,vert_y(li_get_valor(list_get(li_get_valor(list_get(compfortconex,c)),i)))-5,16+(i%6),12+(c%4));		
	}
}

void orient (char * cardcola, float xi, float yi, float xf, float yf) {
	if(yi != yf) {
		*cardcola = (yi > yf) ? 'S' : 'N';
		cardcola++;
	}
	if(xi != xf) 
		*cardcola = (xi > xf) ? 'O' : 'L';
	cardcola++;
	*cardcola = '\0';	

}

int lado (void * i, void * f) {	
	int r = 1;
	if(vert_x(via_get_de(i)) == vert_x(via_get_de(f))) {
		if(vert_x(via_get_de(f)) == vert_x(via_get_para(f)))
			return 0;
		if(vert_y(via_get_de(i)) > vert_y(via_get_de(f)))
			r = -1;
		if(vert_x(via_get_de(i)) > vert_x(via_get_para(f)))
			return -r;	
		return r;
	} 
	if(vert_x(via_get_de(i)) > vert_x(via_get_de(f)))
		r = -1;
	float m = (vert_y(via_get_de(i)) - vert_y(via_get_de(f)))/(vert_x(via_get_de(i)) - vert_x(via_get_de(f)));	
	float p = vert_y(via_get_de(f)) + ((vert_x(via_get_para(f)) - vert_x(via_get_de(f))) * m);	
	if(p == vert_y(via_get_para(f)))
		return 0;
	if(p < vert_y(via_get_para(f)))
		return -r;
	return r;	
}

void op (FILE * svg, FILE * txt, void * cidade, void * origem, char * ocep, char oface, int onum, float ox, float oy, void * destino, char * dcep, char dface, int dnum, float dx, float dy, char * cor, int tipo) {
	if(svg == NULL || txt == NULL)
		return;
	void * caminho = cidade_caminho(cidade, origem, destino, tipo);
	char dir[COM_TAM];
	int desloc = 2*((2 * tipo) - 1);
	printf("Caminho mais ");
	fprintf(txt,"\nCaminho mais ");
	if(tipo == 0) {
		fprintf(txt,"curto");
		printf("curto");
	} else {
		printf("rápido");
		fprintf(txt,"rápido");		
	}
	if(list_get_len(caminho) <= 0) {
		printf(" inexistente.\n");
		fprintf(txt, " inexistente. \n");
		svg_line(svg, cor, "3px", ox, oy, dx, dy, 4);
	} else {
		fprintf(txt, ":\n");		
		svg_path_open(svg);
		svg_path_id_open(svg);
		fprintf(svg,"%s___%s__%f_%f__%d",vert_id(origem),vert_id(destino),dx,dy,tipo);
		svg_path_points_open(svg);
		svg_path_point(svg, 'M', ox, oy); // */
		void * a = NULL;
		float t, d, s = 0;
		int c, l;
		for(c = 0, d = 0, t = 0; c < list_get_len(caminho); c++) {
			if(compare(via_nome(a), via_nome(li_get_valor(list_get(caminho, c)))) != 0 && a != NULL) {
		//		printf("\n%s\t(%f m %s)\n",via_nome(a),d,dir);
				orient(dir, vert_x(via_get_de(li_get_valor(list_get(caminho, c)))), vert_y(via_get_de(li_get_valor(list_get(caminho, c)))), vert_x(via_get_para(li_get_valor(list_get(caminho, c)))), vert_y(via_get_para(li_get_valor(list_get(caminho, c)))));
				fprintf(txt,"\tSiga %.1f metros em %s e ",d,via_nome(a));				
				l = lado(a, li_get_valor(list_get(caminho, c)));
				if(l == 0) 
					fprintf(txt,"continue em frente");
				else {
					fprintf(txt, "vire à ");
					if(l < 0)
						fprintf(txt, "esquerda");
					else fprintf(txt, "direita");	
				}	
				fprintf(txt," em %s (sentido %s)\n",via_nome(li_get_valor(list_get(caminho,c))),dir);
				s += d;
				d = 0;
			}			
			
			a = li_get_valor(list_get(caminho, c));
			d += via_get_cmp(a);
			t += via_get_cmp(a)/via_get_vm(a);
	//		printf("\nDe:\t(%f %f)\t%s\nPara:\t(%f %f)\t%s\n\t(%f m/s)\t%f m\t%f s\n",vert_x(via_get_de(a))+desloc,vert_y(via_get_de(a))+desloc,vert_id(via_get_de(a)),vert_x(via_get_para(a))+desloc,vert_y(via_get_para(a))+desloc,vert_id(via_get_para(a)),via_get_vm(a),via_get_cmp(a),via_get_cmp(a)/via_get_vm(a));
		//	svg_line(svg, cor, "2px", vert_x(via_get_de(a)) + desloc, vert_y(via_get_de(a)) + desloc, vert_x(via_get_para(a)) + desloc, vert_y(via_get_para(a)) + desloc);
			svg_path_point(svg, 'L', vert_x(via_get_de(a)) + desloc, vert_y(via_get_de(a)) + desloc);
		}
	//	printf("\n%s \t(%f m %s) \n", via_nome(a), d, dir);
		fprintf(txt, "\tSiga mais %.2f metros em %s e você chegará em %s/%c/%d", d, via_nome(a), dcep, dface, dnum);
	//	printf("\t(%f %f)\t%s\n",vert_x(via_get_para(a))+desloc,vert_y(via_get_para(a))+desloc,vert_id(via_get_para(a)));
		svg_path_point(svg, 'L', desloc + vert_x(via_get_para(a)), desloc + vert_y(via_get_para(a)));
		svg_path_point(svg, 'L', dx, dy);
		svg_path_close(svg, cor, "2px"); // */
		

	//	float v = d/t;
		float w;// = vert_x(via_get_para(a)) - dx;
		float h;// = vert_y(via_get_para(a)) - dy;
		s += d;
	/*	d = (w * w) + (h * h);
		t += t * d / s; // */
		printf(":\t%f metros em %f segundos.\n", s, t);

		fprintf(txt, " em ");			
		if(t >= 3600)
			fprintf(txt, "%dh", ((int)t)/3600);
		if(t >= 60) {			
			if(t >= 3600 && (((int)t/60)%60) < 10)
				fprintf(txt,"0");
			fprintf(txt, "%dmin", (((int)t)%3600)/60);	
			if(((int)t)%60 < 10)
				fprintf(txt,"0");
		}	
		fprintf(txt, "%ds \n", ((int) t) % 60);	

		h = 0;
		d = 3;
		w = 240;
		while(w > 1 && d > 0) {
			while(t >= w) {
				t /= w;
				h += d;
			}	
			w /= 2;
			d /= 2;
		}	

		svg_circle_open(svg, NULL, cor, NULL, NULL, 0, 0, 6);
			svg_animation_open(svg, "indefinite", h);
				svg_animation_path_open(svg);
				fprintf(svg, "%s___%s__%f_%f__%d", vert_id(origem), vert_id(destino), dx, dy, tipo);
				svg_animation_path_close(svg);
			svg_animation_close(svg);  // */
		svg_circle_close(svg);
	}
	list_del_all(caminho);
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
			decremento_em = decremento_prox;	
			decremento_prox = 0;
			fprintf(txt, "\n\tReduzindo a %f%%:\t%d vértices (de origem ou destino) \n", reduz * 100, decremento_em);
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
			svg_line(svg,cor,"2px",vert_x(via_get_de(li_get_valor(list_get(adj,c)))),vert_y(via_get_de(li_get_valor(list_get(adj,c)))),vert_x(via_get_para(li_get_valor(list_get(adj,c)))),vert_y(via_get_para(li_get_valor(list_get(adj,c)))),0);
			fprintf(txt,"\n\tDe:\t%s\t(%f %f)\n\tPara:\t%s\t(%f %f)\n\t%f m\t(%f m/s)\n\t %s %s\t%s\n",vert_id(via_get_de(li_get_valor(list_get(adj,c)))),vert_x(via_get_de(li_get_valor(list_get(adj,c)))),vert_y(via_get_de(li_get_valor(list_get(adj,c)))),vert_id(via_get_para(li_get_valor(list_get(adj,c)))),vert_x(via_get_para(li_get_valor(list_get(adj,c)))),vert_y(via_get_para(li_get_valor(list_get(adj,c)))),via_get_cmp(li_get_valor(list_get(adj,c))),via_get_vm(li_get_valor(list_get(adj,c))),via_esq(li_get_valor(list_get(adj,c))),via_dir(li_get_valor(list_get(adj,c))),via_nome(li_get_valor(list_get(adj,c))));
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
	int i,c,d,com_tam = 2*COM_TAM;
	float s,t,u,v,f;
	float ox, oy, px, py; 
	int on, pn;	// num
	char oface[COM_TAM], pface[COM_TAM]; // 
	char pcor[2][CEP_TAM];
	void *r;
	void *m,*n;	
	void *ov = NULL, *pv;
	char ocep[CEP_TAM];		
	char cep[CEP_TAM];
	char com[com_tam];
	while(!feof(q)) {

		clean(com,com_tam);
		fscanf(q,"%s",com);
		total++;

		switch (com[0]) {

			case '@': // @o?
				clean(ocep,CEP_TAM);
				fscanf(q,"%s %s %d",ocep,oface,&on);
				fprintf(txt_out, "\n@o?\t%s %c %d \n", ocep, oface[0], on);
				printf("\n%u\t@o?\t%s %c %d \n", exec, ocep, oface[0], on);
				hash_del_all(cidade_get_vias_cmp(cid));
				hash_del_all(cidade_get_vias_vm(cid));
				cidade_set_vias_vm(cid, NULL);
				cidade_set_vias_cmp(cid,NULL);
				r = cidade_get_quadra_hash(cid, ocep);
				if(r == NULL) {
					fprintf(txt_out, "Quadra %s não encontrada. \n", ocep);
					printf("Quadra %s não encontrada.\n",ocep);
				} else {
					ox = cep_get_x(quadra_get_x(r), quadra_get_w(r), oface[0], on);
					oy = cep_get_y(quadra_get_y(r), quadra_get_h(r), oface[0], on);
					fprintf(txt_out, "(%f %f) \n", ox, oy);
					printf("\t(%f %f)\n",ox,oy);
					ov = cidade_get_ponto_vizinho(cid, ox, oy, quadra_get_w(r), quadra_get_h(r));
					if(ov == NULL) {
						fprintf(txt_out,"Não foi encontrado ponto de origem próximo o suficiente.\n");
						printf("Não foi encontrado ponto de origem próximo o suficiente.\n");
					} else {	
						printf("\t(%f %f)\t%s\n",vert_x(ov),vert_y(ov),vert_id(ov));
						fprintf(txt_out, "(%f %f)\t%s\n", vert_x(ov), vert_y(ov), vert_id(ov));
						svg_line(svg_out, "#AA0044", NULL, ox, oy, ox, -32, 0);
						svg_text_open(svg_out, "#AA0044", 27, ox, -30);
						if(svg_out != NULL)
							fprintf(svg_out,"%s/%c/%d",quadra_get_cep(r),oface[0],on);
						svg_text_close(svg_out);
					}	
				}	
				qtd_o++;
				exec++;
				break;

			case 'p': // p?
				clean(cep, CEP_TAM);
				fscanf(q, "%s %s %d", cep, pface, &pn);
				fprintf(txt_out, "\np?\t%s %c %d ", cep, pface[0], pn);
				printf("\n%u\tp?\t%s %c %d ", exec, cep, pface[0], pn);
				for(c = 0; c < 2; c++) {
					clean(pcor[c],CEP_TAM);
					fscanf(q,"%s",pcor[c]);
					fprintf(txt_out, " %s", pcor[c]);
					printf(" %s ", pcor[c]);
				}
				printf("\n");
				fprintf(txt_out, " \n");
				
				r = cidade_get_quadra_hash(cid, cep);
				if(r == NULL) {
					printf("Quadra %s não encontrada.\n", cep);
					fprintf(txt_out,"Quadra %s não encontrada.\n",cep);
				} else if(ov == NULL)	{
					printf("Não há ponto de início conhecido.\n");
					fprintf(txt_out, "Não há ponto de origem conhecido.\n");
				} else {
					px = cep_get_x(quadra_get_x(r), quadra_get_w(r), pface[0], pn);
					py = cep_get_y(quadra_get_y(r), quadra_get_h(r), pface[0], pn);
					fprintf(txt_out, "(%f %f)\n", px, py);
					printf("\t(%f %f) \n",px,py);
					pv = cidade_get_ponto_vizinho(cid, px, py, quadra_get_w(r), quadra_get_h(r));
					if(pv == NULL)
						printf("Não foi encontrado ponto de destino próximo o suficiente.\n");
					else {	
						printf("\t(%f %f)\t%s \n",vert_x(pv),vert_y(pv),vert_id(pv));
						fprintf(txt_out, "(%f %f)\t%s \n", vert_x(pv), vert_y(pv), vert_id(pv));

						for(c = 0; c < 2; c++) 
							op(svg_out, txt_out, cid, ov, ocep, oface[0], on, ox, oy, pv, cep, pface[0], pn, px, py, pcor[c], c);
					}	
				}

				qtd_p++;
				exec++;
				break;	

			case 'r': // rv	
				fscanf(q, "%f %f %f %f %f", &u, &v, &t, &s, &f);
				fprintf(txt_out, "\nrv\t%f %f %f %f %f\n", u, v, t, s, f);
				printf("\n%u\trv\t%f %f %f %f %f\n", exec, u, v, t, s, f);
				svg_rect_open(svg_out, com, NULL, "#AB37C8", "4px", u, v, t, s, 1, 10);
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
					d = rv(svg_out, txt_out, b, a, li_get_valor(list_get(n, c)), "#65D948", f); 
					fprintf(txt_out, "\n%d velocidades reduzidas. \nRaiz:\t%s\n", d, vert_id(li_get_valor(list_get(n, c))));
					if(d > 0) {
						if(cidade_get_vias_vm(cid) != NULL)
							printf("invalidando caminhos mais rápidos.\n");
						hash_del_all(cidade_get_vias_vm(cid));	
						cidade_set_vias_vm(cid, NULL);
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
				i = 0;
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
						fprintf(txt_out, "\n\t%s\tRegião %d\t[%d vértice", (list_get_len(li_get_valor(list_get(cx2,c)))>1)?cep:NULL, c, list_get_len(li_get_valor(list_get(cx2, c))));
						if(list_get_len(li_get_valor(list_get(cx2,c))) > 1) {
							i++;
							fprintf(txt_out, "s");
						} else fprintf(txt_out," isolado");
						fprintf(txt_out, "]: \n");	
						for(d = 0; d < list_get_len(li_get_valor(list_get(cx2, c))); d++) 
							fprintf(txt_out,"\t%s\t(%f %f)\n",vert_id(li_get_valor(list_get(li_get_valor(list_get(cx2,c)),d))),vert_x(li_get_valor(list_get(li_get_valor(list_get(cx2,c)),d))),vert_y(li_get_valor(list_get(li_get_valor(list_get(cx2,c)),d))));
					}
					fprintf(txt_out,"\n%d regiões isoladas encontradas [%d com mais de um vértice]. \n",c,i);
					printf("\t%d regiões isoladas [%d vértices isolados]\n",c,c-i);
					qtd_cx++;
				} else { // catac
					fscanf(q, "%f %f %f", &v, &t, &s);
					svg_rect_open(svg_out, com, "#AB37C8", "#AA0044", "5px", u, v, t, s, 0.5, 0);
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
