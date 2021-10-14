#include<stdlib.h>
#include"quadra.h"
#include "pessoas.h"
#include"moradias.h"
#include"loc.h"
#include"arq.h"

void loc_del (void * l) {
	if(l != NULL) {
		free(loc_id(l));
		free(l);
	}	
}


typedef struct loca {
	void * moradia;
	char * id;
	char sit;
	float valor;
	float area;
} aluguel;

char * loc_id (void * loc) {
	if(loc == NULL)
		return NULL;
	return ((aluguel *) loc)->id;	
}

void * new_loc (float custo, float m2, char * id, void * imovel) {
	aluguel * a = malloc(sizeof(aluguel));
	a->id = malloc(sizeof(char)*(comprimento(id)+1));
	loc_set_moradia(a,imovel);
	loc_set_area(a, m2);
	loc_set_valor(a,custo);
	copy(id,a->id);	
	return a;
}

void * loc_get_moradia (void * loc) {
	if(loc != NULL)
		return ((aluguel*)loc)->moradia;
	return NULL;	
}

void loc_set_moradia (void * loc, void * mor) {
	if(loc != NULL)
		((aluguel*)loc)->moradia = mor;
	loc_set_disponibilidade(loc,(((moradia_get_cpf == NULL))?(DISP):(LOCADA)));	
	moradia_set_loc(mor,loc);
	
}

void loc_set_area (void * loc, float a) {
	if(loc != NULL)
		((aluguel*)loc)->area = a;
}

void loc_set_valor (void * loc, float v) {
	if(loc != NULL)
		((aluguel*)loc)->valor = v;
}

float loc_get_valor (void * loc) {
	if(loc == NULL)
		return 0;
	return ((aluguel*)loc)->valor;
}

float loc_get_area (void * loc) {
	if(loc == NULL)
		return 0;
	return ((aluguel*)loc)->area;
}



char loc_get_disponibilidade (void * loc) {
	if(loc == NULL)
		return IDK;
	return ((aluguel*)loc)->sit;
}

void loc_set_disponibilidade (void * loc, char disp) {
	if(loc != NULL)
		((aluguel*)loc)->sit = disp;
}

