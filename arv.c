#include<stdlib.h>
#include<stdio.h>
#include"arv.h"

void * arv_get (void * a, float chave) {
	while(a != NULL) {
		if(arv_get_chave(a) == chave)
			return a;
		if(arv_get_chave(a) > chave)	
			a = arv_get_esq(a);
		else a = arv_get_dir(a);	
	}
	return NULL;
}

void * avl_insert (void * r, void * a);

void arv_set_filho (void * no, int i, void * filho);
void arv_set_chave (void * no, float chave);
void arv_set_valor (void * no, void *valor);
void arv_set_esq (void * no, void * esq); 
void arv_set_dir (void * no, void * dir);
void arv_set_pai (void * no, void * pai);

typedef struct no {
	int altura;
	float min;
	float max;
	float max_l;
	float min_l;	
	float chave;
	void* valor;

	void * pai;
	void * filhos[ARV_TAM];
} arv;

void arv_set_chave (void* no, float chave) {
	if(no != NULL) 
		((arv *)no)->chave = chave;
}
void arv_set_valor (void* no, void * valor) {
	if(no != NULL)
		((arv *)no)->valor = valor;
}

void* arv_get_valor (void * no) {
	if(no == NULL)
		return NULL;
	return ((arv*) no)->valor;		
}
float arv_get_chave (void * no) {
	if(no == NULL)
		return 0;
	return ((arv*) no)->chave;
}

float arv_get_min_l (void * no) {
	if(no == NULL)
		return 0;
	return ((arv*)no)->min_l;	
}
float arv_get_max_l (void * no) {
	if(no == NULL)
		return 0;
	return ((arv *) no)->max_l;	
}

float arv_get_max (void * no) {
	if(no == NULL)
		return 0;
	return ((arv *) no)->max;	
}
float arv_get_min (void * no) {
	if(no == NULL)
		return 0;
	return ((arv*)no)->min;	
}

void arv_set_min (void * no, float min) {
	while(no != NULL) 
		if(arv_get_min(no) > min) {
			((arv*)no)->min = min;	
			no = arv_get_pai(no);
		} else return;		
}
void arv_set_max (void * no, float max) {
	while(no != NULL) 
		if(arv_get_max(no) < max) {
			((arv *) no)->max = max;
			no = arv_get_pai(no);
		} else return;					
}

void arv_reset_m_all (void * no, float max, float min) {
	if(no == NULL)
		return;
	((arv *)no)->max_l = max;
	((arv*) no)->min_l = min;	
	int c;
	do {						
		arv_reset_m(no);				
		for(c = 0; c < ARV_TAM; c++)
			if(arv_get_filho(no,c) != NULL) {
				arv_set_max(no, arv_get_max(arv_get_filho(no, c)));
				arv_set_min(no, arv_get_min(arv_get_filho(no, c)));
			}			
		no = arv_get_pai(no);	
	//	min = ((arv*)no)->min_l;
	//	max = ((arv *) no)->max_l;
	} while(no != NULL);
}

void arv_reset_m (void * no) {
	if(no == NULL)
		return;
	arv* a = (arv *) no;
	a->max = a->max_l;
	a->min = a->min_l;
}

void arv_set_pai (void * no, void * pai) {
	if(no == NULL)
		return;
	arv* a = (arv *) no;	
	if(a->pai == pai)
		return;
	if(a->pai != NULL) {
	//	arv_reset_m(a->pai);
		int c;
		for(c = 0; c < ARV_TAM; c++)
			if(arv_get_filho(a->pai,c) == a) 
				((arv*)a->pai)->filhos[c] = NULL;					
		arv_set_altura(a->pai);		
		arv_reset_m_all(a->pai,arv_get_max_l(a->pai),arv_get_min_l(a->pai));
	}
	a->pai = pai;
	if(pai == NULL) 
		return;
	arv_set_altura(pai);		
//	if(arv_get_max(a->pai) < arv_get_max(a)) 
	arv_set_max(a->pai, arv_get_max(a));
//	if(arv_get_min(a->pai) > arv_get_min(a)) 
	arv_set_min(a->pai, arv_get_min(a));	
}
void* arv_get_pai (void * no) {
	if(no == NULL)
		return NULL;
	return ((arv *) no)->pai;
}

void * arv_get_filho (void * no, int i) {
	if(no == NULL)
		return NULL;
	return ((arv *) no)->filhos[i];
}
void arv_set_filho (void * no, int i, void * filho) {
	if(no == NULL)
		return;
	arv *p = (arv*) no;	
	if(p->filhos[i] != filho) {		
		arv_set_pai(p->filhos[i],NULL);
		p->filhos[i] = filho;
		arv_set_pai(filho,no);
	}
}

void* arv_get_esq (void * no) {
	return arv_get_filho(no,0);
}
void arv_set_esq (void * no, void * esq) {
	arv_set_filho(no,0,esq);
}
void arv_set_dir (void * no, void * dir) {
	arv_set_filho(no, ARV_TAM - 1, dir);
}
void* arv_get_dir (void * no) {
	return arv_get_filho(no, ARV_TAM - 1);
}

void * avl_rot (void * raiz, int filho) {	
	void * f = arv_get_filho(raiz,filho);
	int g = ARV_TAM - 1 - filho;

//	arv_set_pai(f, arv_get_pai(raiz));
	arv_set_filho(raiz,filho,arv_get_filho(f,g));
	arv_set_filho(f, g, raiz);
	return f;
}
void * avl_drot (void * raiz, int lado) {
	arv_set_filho(raiz,lado,avl_rot(arv_get_filho(raiz,lado),ARV_TAM - 1 - lado));
	return avl_rot(raiz,lado);
}

void * avl_rot_dir (void * a) {	return avl_rot(a, 0);	}
void * avl_drot_dir (void *a) {	return avl_drot(a,0);	}
void * avl_drot_esq (void *a) {	return avl_drot(a,ARV_TAM-1);	}
void * avl_rot_esq (void * a) {	return avl_rot(a, ARV_TAM - 1);	}

int avl_dif_alt (void * a) {
	return arv_get_altura(arv_get_esq(a)) - arv_get_altura(arv_get_dir(a));
}

int maior (int x, int y) {
	if(x > y)
		return x;
	return y;	
}

int maior_altura (int c, void ** v) {
	int m = 0;
	while(c-- > 0)
		m = maior(m,arv_get_altura(v[c]));
	return m;
}

void arv_set_altura (void * no) {
	int h;
	arv * a = (arv*) no;
	while(a != NULL) {
		h = 1 + maior_altura(ARV_TAM,a->filhos);//maior(arv_get_altura(arv_get_dir(a)), arv_get_altura(arv_get_esq(a)));
		if(h != a->altura)
			a->altura = h;			
		else break;	
		a = a->pai;
	}
}

int arv_get_altura (void * no) {
	if(no != NULL)
		return ((arv *)no)->altura;
	return 0;	
}

arv * novo_no (void * valor, float chave, float min, float max) {
	arv* novo = malloc(sizeof(arv));	
	novo->pai = NULL;
	novo->altura = 1;
	novo->max_l = max;
	novo->min_l = min;
	novo->min = min;
	novo->max = max;	
//	arv_set_max(novo, chave);
//	arv_set_min(novo, chave);
	arv_set_chave(novo,chave);
	arv_set_valor(novo,valor);
	int c = ARV_TAM;
	while(c-- > 0) 
		novo->filhos[c]	= NULL;			
	return novo;
}

void * avl_eq (void * raiz) {
	int dalt = avl_dif_alt(raiz);
//	printf("\n%f\t%d",arv_get_chave(r),dalt);
	if(dalt > 1) {
		if(avl_dif_alt(arv_get_esq(raiz)) < 0)
			raiz = avl_drot_dir(raiz);
		else raiz = avl_rot_dir(raiz);	
	} else if(dalt < -1) {
		if(avl_dif_alt(arv_get_dir(raiz)) > 0)
			raiz = avl_drot_esq(raiz); 
		else raiz = avl_rot_esq(raiz);
	} //*/
	return raiz;
}

void * avl_insert (void * r, void * a) {
	if(r == NULL)
		return a;	
	int l = 0;	
	if(arv_get_chave(r) < arv_get_chave(a)) 
		l = ARV_TAM - 1; 	

	arv_set_filho(r,l,avl_insert(arv_get_filho(r,l),a));		
	return avl_eq(r);
}

void * avl_insert_all (void * raiz, void * tudo) {
	if(tudo == NULL)
		return raiz;

	int c;
	for(c = 0; c < ARV_TAM; c++) 		
		raiz = avl_insert_all(raiz, arv_get_filho(tudo, c));
	arv_set_pai(tudo,NULL);	

	return avl_insert(raiz,tudo);	
		
}

void * avl_deprecated_del (void * r, float chave) {
	// fazia remoção e reinsert das árvores filho
	if(r == NULL)
		return NULL;	
	if(arv_get_chave(r) == chave) {
		void * p = arv_get_pai(r);
		arv_set_pai(r,NULL);
		int c;
		for(c = 0; c < ARV_TAM; c++)
			p = avl_insert_all(p,arv_get_filho(r,c));
		arv_set_pai(p, NULL);	
		return p;	
	} 	
	int l = 0;	
	if(arv_get_chave(r) < chave) 
		l = ARV_TAM - 1; 			

	void * f = arv_get_filho(r,l);
	void * g = avl_deprecated_del(f, chave);
	if(arv_get_chave(f) == chave)
		return g;
	arv_set_filho(r,l,g);		
	return avl_eq(r);
}

void * avl_del (void * r, float chave) {

	if(r == NULL)
		return NULL;		

	int l = 0;
	if(arv_get_chave(r) < chave)	
		l = ARV_TAM - 1;	 			
	else if(arv_get_chave(r) == chave) {
		void * f;
		if(arv_get_esq(r) == NULL)
			f = arv_get_dir(r);
		else if(arv_get_dir(r) == NULL)	
			f = arv_get_esq(r);
		else {	
			f = arv_min(arv_get_dir(r));
			arv_set_chave(r, arv_get_chave(f));
			arv_set_valor(r, arv_get_valor(f));
			void * s = arv_get_dir(r);
			arv_set_dir(r,NULL);
			arv_reset_m_all(r, arv_get_max_l(f), arv_get_min_l(f));
			arv_set_dir(r,avl_del(s,arv_get_chave(f))); 						
			return r;
		}	
		arv_set_pai(r,NULL);				
		arv_set_dir(r,NULL);
		arv_set_esq(r,NULL);
		free(r);
		return f;
	}	

//	arv_reset_m_all(r,arv_get_max_l(r),arv_get_min_l(r));
	arv_set_filho(r,l,avl_del(arv_get_filho(r,l),chave));		
	return avl_eq(r);
}

void * avl_add (void **raiz, void * valor, float chave, float min, float max) {
	void * n = novo_no(valor, chave, min, max);			
	*raiz = avl_insert(*raiz, n);	
	return n;
}

void * arv_add (void ** raiz, void * valor, float chave, float min, float max) {
	arv * n = novo_no(valor, chave, min, max);	
	if(*raiz == NULL) {
		*raiz = n;
		return n;
	}	
		

	arv * p = *raiz, *a;
	while(p != NULL) { 
		a = p;
		if(chave < arv_get_chave(p)) 
			p = arv_get_esq(p);
		else p = arv_get_dir(p);			
	}

	if(chave < arv_get_chave(a)) {
		arv_set_esq(a, n);
		if(chave < arv_get_min(a)) 
			arv_set_min(a, chave);
	} else { 
		arv_set_dir(a, n);
		if(chave < arv_get_max(a))
			arv_set_max(a, chave);
	}	
	
	
//	arv_set_altura(a);

	return n;
}

void arv_ver (void * raiz, int margem) {
	if(raiz != NULL) {		
		printf("\n");
		char b = '{';
		int c;
		for(c = 0; c < margem; c++)
			printf(" ");
		printf("(%d) %f [%.2f %.2f]:\t",arv_get_altura(raiz),arv_get_chave(raiz), arv_get_min(raiz), arv_get_max(raiz));
		for(c=0;c<ARV_TAM;c++) {
			if(arv_get_filho(raiz,c) == NULL)
				printf("%c nULL", b);
			else printf("%c %.3f",b,arv_get_chave(arv_get_filho(raiz,c)));			
			b = ',';
		}			
		printf(" };");
		for(c = 0; c < ARV_TAM; c++)
			arv_ver(arv_get_filho(raiz, c),margem + ARV_TAM);
		printf("\n");	
	}	
}

void * arv_max (void * raiz) {
	return arv_m(raiz, ARV_TAM - 1);
}

void * arv_min (void * raiz) {
	return arv_m(raiz,0);
}

void * arv_m (void * r, int l) {
	void * m = NULL;
	while(r != NULL) {
		m = r;
		r = arv_get_filho(r,l);
	}
	return m;
}

void arv_del_all (void * r) {
	void * s;
	void * t;
	while(r != NULL) {		
		t = arv_get_esq(r);
		s = arv_get_dir(r);
		arv_set_dir(r,NULL);
		arv_set_esq(r,NULL);
		arv_set_pai(r,NULL);
		arv_del_all(s);
		free(r);		
		r = t;
	}	
}