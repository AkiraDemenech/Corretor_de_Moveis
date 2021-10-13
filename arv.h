#ifndef __ARV__H
#define __ARV__H
#define ARV_TAM 2

int maior (int x, int y); // retorna o maior dos dois valores
int arv_get_altura (void * no); //	retorna a altura de um dado nó na árvore. Retorna 0 para NULL
void arv_set_altura (void * no); //	corrige a altura do nó e das raízes das superárvores se for preciso

void * arv_get_pai (void * no); //	retorna o nó pai do nó dado
void * arv_get_dir (void * no); //	retorna a subárvore direita de uma árvore dada
void * arv_get_esq (void * no); //	retorna a subárvore esquerda de uma árvore dada
void* arv_get_filho (void* no, int i); //	retorna o nó filho de índice dado

float arv_get_min (void *no); //	retorna a chave mínima dessa subárvore
void arv_set_min (void * no, float min);

float arv_get_max (void *no); //	retorna a chave máxima dessa subárvore
void arv_set_max (void * no, float max);

void arv_reset_m_all (void* no, float max, float min);
void arv_reset_m (void * no); 

void arv_del_all (void * r); // apaga a árvore

void* arv_m (void *r, int l);
void * arv_max (void * raiz);
void * arv_min (void * raiz);

void * avl_insert_all (void * raiz, void * tudo);

float arv_get_chave (void * no); //	retorna a chave de um dado nó
void* arv_get_valor (void * no); //	retorna o valor armazenado por um nó dado

void * avl_eq (void * raiz); // equilibra a árvore AVL e retorna a nova raiz

void * avl_del (void * r, float chave);  // remove o elemento da avl e retorna a nova raiz

void * avl_add (void** arv, void * valor, float chave, float min, float max); 
void * arv_add (void** arv, void * valor, float chave, float min, float max); 
//	retornam o novo nó e atualiza a raiz da árvore

void arv_ver (void * raiz, int margem);
// Printa a árvore

void * arv_get (void * a, float chave); // retorna o nó na chave

#endif