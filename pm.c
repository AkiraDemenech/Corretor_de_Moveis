#include <stdlib.h>
#include "quadra.h"
#include"pessoas.h"
#include"moradias.h"


typedef struct pes {
	char gen;
	char cpf[CPF_TAM];
	char nasc[NASC_TAM];	 
	char * nome;
	char * sobrenome;		
} pessoa;

void * new_pessoa (char sexo, char * cpf, char * nasc, char * nome, char * sobrenome) {
	if(sexo > 'Z')
		sexo -= 'a' - 'A';		
	pessoa * p = (pessoa *) malloc(sizeof(pessoa));
	p->sobrenome = sobrenome;
	p->nome = nome;
	p->gen = sexo;
	copy(nasc,p->nasc);
	copy(cpf,p->cpf);
	return p;
}

char * pessoa_nome (void * pm) {
	if(pm != NULL)
		return ((pessoa*)pm)->nome;
	return NULL;	
}
char * pessoa_sobrenome (void * pm) {
	if(pm != NULL)
		return ((pessoa*)pm)->sobrenome;
	return NULL;	
}
char * pessoa_cpf (void * pm) {
	if(pm != NULL)
		return ((pessoa*)pm)->cpf;
	return NULL;	
}
char * pessoa_datanas (void * pm) {
	if(pm != NULL)
		return ((pessoa*)pm)->nasc;
	return NULL;	
}
char pessoa_gen (void * pm) {
	if(pm != NULL)
		return ((pessoa*)pm)->gen;
	return IDK;	
}

typedef struct mor {
	char * cpf;
	char * cep;
	char * compl;
	char face;
	int num;
} moradia;

void * new_moradia (int num, char face, char * compl, char * cep, char * cpf) {	
	moradia * m = (moradia *) malloc(sizeof(moradia));
	moradia_set_cpf(m,cpf);
	moradia_set_cep(m,cep);
	moradia_set_compl(m,compl);
	moradia_set_face(m,face);
	moradia_set_num(m,num);
/*	m->num = num;//
	m->face = face;//
	m->compl = compl;//
	m->cpf = cpf;//
	m->cep = cep;//*/
	return m;
}

char * moradia_get_cpf (void * m) {
	if(m == NULL)
		return NULL;
	return ((moradia*)m)->cpf;	
}
char * moradia_get_cep (void * m) {
	if(m == NULL)
		return NULL;
	return ((moradia*)m)->cep;	
}
char * moradia_get_compl (void * m) {
	if(m == NULL)
		return NULL;
	return ((moradia*)m)->compl;	
}
char moradia_get_face (void * m) {
	if(m == NULL)
		return IDK;
	return ((moradia*)m)->face;	
}
int moradia_get_num (void * m) {
	if(m == NULL)
		return -1;
	return ((moradia*)m)->num;
}

void moradia_set_cpf (void * m, char * cpf) {
	if(m != NULL)
		((moradia*)m)->cpf = cpf;	
}
void moradia_set_cep (void * m, char * cep) {
	if(m != NULL)
		((moradia*)m)->cep = cep;	
}
void moradia_set_compl (void * m, char * complemento) {
	if(m != NULL)
		((moradia*)m)->compl = complemento;
}
void moradia_set_face (void * m, char face) {
	if(face > 'Z')
		face -= 'a' - 'A';
	if(m != NULL)
		((moradia*)m)->face = face; 
}
void moradia_set_num (void * m, int numero) {
	if(m != NULL)
		((moradia*)m)->num = numero;
}



