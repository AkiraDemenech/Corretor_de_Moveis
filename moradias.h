#ifndef __MORADIAS__H
#define __MORADIAS__H 
#define NORTE 'N'
#define SUL 'S'
#define LESTE 'L'
#define OESTE 'O'

void * moradia_get_loc (void * m);
void moradia_set_loc (void * m, void * l); 

void moradia_set_num (void * m, int numero);
int moradia_get_num (void * m);

void moradia_set_face (void * m, char face);
char moradia_get_face (void * m);

void moradia_set_compl (void * m, char * complemento);
char * moradia_get_compl (void * m);

void moradia_set_cep (void * m, char * cep);
char * moradia_get_cep (void * m);

char * moradia_get_cpf (void * m);
void moradia_set_cpf (void * m, char * cpf);

float moradia_get_x (void * endereco, void * cep);
float moradia_get_y (void * endereco, void * cep);

void * new_moradia (int num, char face, char * compl, char * cep, char * cpf);

#endif