#ifndef __HASH__H
#define __HASH__H 

void clean (char * v, int c);
int compare (char * x, char * y);
unsigned int hash (char * str);
void * new_hash_table (int size);
void * hash_get_chave (void * tabesp, int index);
void * hash_get_all (void * tabesp, char * chave);
void * hash_get (void * tabesp, char * chave);
void hash_del (void * tabesp, char * chave);
void hash_set (void * tabesp, char * chave, void * valor);

void hash_del_all (void * tabesp);

int hash_get_len (void * tabesp);
int hash_get_chaves_len (void * tabesp);
#endif