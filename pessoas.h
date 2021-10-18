#ifndef __PESSOAS__H
#define __PESSOAS__H
#define CPF_TAM 15	// xxx.xxx.xxx-yy 
#define NASC_TAM 11	// dd/mm/aaaa 
#define MASC 'M'
#define FEM 'F'
#define IDK '~'

void * new_pessoa (char sexo, char * cpf, char * nasc, char * nome, char * sobrenome);

char pessoa_gen (void * pm);
char * pessoa_cpf (void * pm);
char * pessoa_datanas (void * pm);
char * pessoa_sobrenome (void * pm);
char * pessoa_nome (void * pm);

char * cpf_validacao (int cpf_val); // retorna a mensagem de valicação para o retorno informado

int cpf_validar (char * cpf);
// retorna -3 se cpf for null, -2 se houver problema no segundo dígito verificador e -1 no primeiro
// retorna 0 se for válido e um valor positivo indicando a quantidade de dígitos que faltam, se faltarem


#endif