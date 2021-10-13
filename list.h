#ifndef __LIST__H
#define __LIST__H
#define LIST_CON 2

int list_set_index (void * list, int index); // posiciona a lista no índice dado
int list_get_index (void * list); 

int list_get_len (void * list); 

void * list_get_atual (void * list); 

void * list_get (void * list, int index); // retorna o elemento do índice fornecido
int list_set (void * list, int index, void * valor); // insere um novo valor
int list_add (void * list, void * valor); 
int list_insert (void * list, void * valor);


void * new_list (int size); 

void list_del_all (void * list);
void list_del (void * list, int index);

void * li_get_prev (void * elemento);
void * li_get_prox (void * elemento);

void * li_get_valor (void * elemento);
void li_set_valor (void * elemento, void * valor);

void li_set_chave (void * elemento, void * chave);
void * li_get_chave (void * elemento);

#endif