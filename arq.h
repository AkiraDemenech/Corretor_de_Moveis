
#ifndef __ARQ__H
#define __ARQ__H 

int comprimento (char * s); // retorna o comprimento da string (índice da primeira aparição de '\0')
int ult_ind (char ch, char * s); // retorna o índice da última aparição de um caractere dado antes do fim da string. Retorna -1 se não o encontrar e -2 para NULL

char * arq_nome_concat (unsigned int cauda, char sep, char ult, char * x, char * y); 
//	retorna a concatenação das substrings x e y antes da última aparição do caractere ult, 
//	colocando o caractere sep entre elas. Adiciona à cauda o caractere ult na quantidade informada.
//	Não aloca memória nenhuma caso alguma das duas strings seja NULL e simplesmente retorna a outra nesta situação.

char * arq_nome_completo (char * caminho, char * nome);
//	Retorna o nome completo do arquivo dado seu nome e seu caminho, sem alocar memória caso algum seja NULL

char * arq_nome (char * nome_completo);
//	Retorna o ponteiro começando depois da última barra.

#endif
