#include <stdio.h>
#include <stdlib.h>
#include "quadra.h"
#include"pessoas.h"
#include"moradias.h"
#include"loc.h"

char * cpf_validacao (int cpf_val) {
	switch(cpf_val) {
		case 0:
			return "Válido";

		case -1:	
			return "Inválido:\tDV1 incorreto";

		case -2:	
			return "Inválido:\tDV2 incorreto";		

		case -3:	
			return NULL;
	}
	if(cpf_val < 0)
		return "Inválido";
	
	char * m = calloc(40, sizeof(char));
	sprintf(m,"Inválido: falta%s %02d dígito%s",(cpf_val == 1)?"":"m",cpf_val,(cpf_val == 1)?"":"s");
	return m;
}


int cpf_validar (char * cpf) {
	if(cpf == NULL)
		return -3;
	int c, d, dv1 = 0, dv2 = 0, v, m = 11;	
	for(c=0;cpf[c]!='\0';c++) 	
		if(cpf[c] >= '0' && cpf[c] <= '9') {
			d = cpf[c] - '0';			
			v = d * m;			
			m--;			
			if(m == 1) {
				if(((dv1 * 10) % 11) % 10 != d)
					return -1;
			} else if (m == 0) {
				if(((dv2 * 10) % 11) % 10 != d)
					return -2;
				break;	
			} 			
			dv1 += v - d;			
			dv2 += v;
		}
	return m;			
}

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
/*	int c = cpf_validar(p->cpf);
	if(c != 0)	printf("\tCPF %s %s\n",p->cpf,cpf_validacao(c)); // */
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
	void * loc;
} moradia;

void * new_moradia (int num, char face, char * compl, char * cep, char * cpf) {	
	moradia * m = (moradia *) malloc(sizeof(moradia));
	moradia_set_loc(m,NULL);
	moradia_set_cpf(m, cpf);
	moradia_set_cep(m, cep);
	moradia_set_compl(m,compl);
	moradia_set_face(m,face);
	moradia_set_num(m, num);
/*	m->num = num;//
	m->face = face;//
	m->compl = compl;//
	m->cpf = cpf;//
	m->cep = cep;//*/
	return m;
}

void moradia_set_loc (void * m, void * l) {
	if(m != NULL)
		((moradia*)m)->loc = l;
	if(loc_get_moradia(l) != m)	
		loc_set_moradia(l, m);
}

void * moradia_get_loc (void * m) {
	if(m != NULL)
		return ((moradia*)m)->loc;
	return NULL;
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
	loc_set_moradia(moradia_get_loc(m),m); // recarregando indiretamente a disponibilidade	
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

float moradia_get_x (void * endereco, void * cep) {
	return cep_get_x(quadra_get_x(cep),quadra_get_w(cep),moradia_get_face(endereco),moradia_get_num(endereco));
}

float moradia_get_y (void * endereco, void * cep) {
	return cep_get_y(quadra_get_y(cep),quadra_get_h(cep),moradia_get_face(endereco),moradia_get_num(endereco));
}

