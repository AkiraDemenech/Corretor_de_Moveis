#ifndef __MORADIAS__H
#define __MORADIAS__H 

void moradia_set_num (void * m, int numero);
int moradia_get_num (void * m);

void moradia_set_face (void * m, char face);
char moradia_get_face (void * m);

void moradia_set_compl (void * m, char * complemento);
char * moradia_get_compl (void * m);

void moradia_set_cep (void * m, char * cpf);
char * moradia_get_cep (void * m);

char * moradia_get_cpf (void * m);
void moradia_set_cpf (void * m, char * cpf);

void * new_moradia (int num, char face, char * compl, char * cep, char * cpf);

#endif