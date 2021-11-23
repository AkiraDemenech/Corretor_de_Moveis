
#include<stdlib.h>
#include <stdio.h>
#include"list.h"

typedef struct vert {

	void * edges;

	char * id;
	float x;
	float y;

} vertex;

float vert_x (void * v) {
	if(v == NULL)
		return 0;
	return ((vertex*)v)->x;	
}

float vert_y (void * v) {
	if(v == NULL)
		return 0;
	return ((vertex*)v)->y;	
}

char * vert_id (void * v) {
	if(v == NULL)
		return NULL;
	return ((vertex*)v)->id;	
}

void * vert_get_vias (void * v) {
	if(v == NULL)
		return NULL;
	return ((vertex*)v)->edges;	
}

void vert_set_vias (void * v, void * caminhos) {
	if(v != NULL)
		((vertex*)v)->edges = caminhos;	
}

void * new_vert (char * id, float x, float y) {
	vertex * v = malloc(sizeof(vertex));
	vert_set_vias(v, new_list(0));
	if(v != NULL) {
		v->id = id;
		v->x = x;
		v->y = y;
	}
	return v;
}

typedef struct arest {

	void * from;
	void * to;

	char * dir;
	char * esq;

	char * nome;

	float cmp;
	float vm;

} edge;

void * new_via (char * nome, char * dir, char * esq) {
	edge * e = malloc(sizeof(edge));
	if(e != NULL) {
		via_lugar(e, nome, dir, esq);
		e->from = NULL;
		e->to = NULL;
	}
	return e;
}

void via_lugar (void * v, char * nome, char * dir, char * esq) {
	if(v == NULL)
		return;
	edge * e = (edge *) v;	
	e->dir = dir;
	e->esq = esq;
	e->nome = nome;
}

char * via_nome (void * v) {
	if(v == NULL)
		return NULL;
	return ((edge *) v)->nome;	
}

char * via_dir (void * v) {
	if(v == NULL)
		return NULL;
	return ((edge *) v)->dir;	
}

char * via_esq (void * v) {
	if(v == NULL)
		return NULL;
	return ((edge *) v)->esq;	
}

void * via_get_de (void *v) {
	if(v == NULL)
		return NULL;
	return ((edge *) v)->from;	
}

void * via_get_para (void *v) {
	if(v == NULL)
		return NULL;
	return ((edge *) v)->to;	
}

void via_set_para (void *v, void * destino) {
	if(via_get_para(v) == destino || v == NULL)
		return;
	((edge *) v)->to = destino;	
}

void via_set_de (void *v, void * origem) {
	if(via_get_de(v) == origem || v == NULL)
		return;		
	((edge *) v)->from = origem;	
	
}

void via_set_cmp (void *v, float comprimento) {
	if(v != NULL)
		((edge *) v)->cmp = comprimento;
}

float via_get_cmp (void *v) {
	if(v != NULL)
		return ((edge *) v)->cmp;
	return 0;	
}

float via_get_vm (void *v) {
	if(v != NULL)
		return ((edge *) v)->vm;
	return 0;	
}

void via_set_vm (void *v, float velocidade) {
	if(v != NULL)
		((edge *) v)->vm = velocidade;
}

