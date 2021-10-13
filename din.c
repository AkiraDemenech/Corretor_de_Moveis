#include<stdlib.h>
#include"list.h"

typedef struct no {
	void * valor;
	void * chave;
	struct no * conecta[LIST_CON];
} item;

void li_set_valor (void * elemento, void * valor) {
	if(elemento != NULL)
		((item*)elemento)->valor = valor;		
}
void * li_get_valor (void * elemento) {
	if(elemento != NULL)
		return ((item*)elemento)->valor;
	return NULL;	
}
void li_set_chave (void * elemento, void * chave) {
	if(elemento != NULL)
		((item*)elemento)->chave = chave; 
}
void * li_get_chave (void * elemento) {
	if(elemento != NULL)
		return ((item*)elemento)->chave;
	return NULL;	
}
void * li_get_prox (void * elemento) {
	if(elemento != NULL)
		return ((item*)elemento)->conecta[LIST_CON-1];
	return NULL;
}
void * li_get_prev (void * elemento) {
	if(elemento != NULL)
		return ((item*)elemento)->conecta[LIST_CON-1];
	return NULL;
}


typedef struct din {
	int len, index;
	item * atual;	
} lista;


int list_set_index (void * list, int index) {
	
	if(list == NULL)
		return -1;
	if(index < 0)
		index = 0;	
	lista * l = (lista*)list;			
	if(index >= l->len) {
		index = l->len - 1;
		if(index < 0)
			return 0;
	}		
	int inc = -1, next = 0;
	if(index > l->index) {		
		next = LIST_CON - 1;
		inc = 1;
	}
	while(index != l->index) {		
		if(l->atual->conecta[next] == NULL) 			
			return l->index;						
		l->index += inc;
		l->atual = l->atual->conecta[next];
	}	
	return index;
}
int list_get_index (void * list) {
	if(list == NULL)
		return -1;
	return ((lista*)list)->index;	
}
int list_get_len (void * list) {
	if(list == NULL)
		return -1;
	return ((lista*)list)->len;	
}
void * list_get_atual (void * list) {
	if(list == NULL)
		return NULL;
	return ((lista*)list)->atual;	
}
void * list_get (void * list, int index) {
	if(index < 0 || index >= list_get_len(list))
		return NULL;
	list_set_index(list, index);
	return list_get_atual(list);
}
int list_set (void * list, int index, void * valor) {
	if(list == NULL)
		return -1;
	if(index < 0)
		index = 0;	
	list_set_index(list,index);
	lista * l = (lista *) list;			
	item * prev;
	item * prox = NULL;			
	item * novo = (item *) malloc(sizeof(item));
	if(l->atual != NULL) {
		prev = l->atual->conecta[0];
		prox = l->atual;				
	}
	if(list_get_len(list) <= index) {
		l->index = list_get_len(list);
		prev = prox;
		prox = NULL;		
	}  
		
	if(prox != NULL) prox->conecta[0] = novo;						
	if(prev != NULL) prev->conecta[LIST_CON-1] = novo;
	l->atual = novo;	
	l->atual->valor = valor;
	l->atual->conecta[LIST_CON - 1] = prox;
	l->atual->conecta[0] = prev;	
	l->len++;
	return l->index; 
}
int list_add (void * list, void * valor) {	
	return list_set(list,list_get_len(list),valor);
}
void list_del (void * list, int index) {	
	if(list_get_len(list) <= 0)
		return; // null ou sem elementos		
	list_set_index(list,index);	
	lista * l = (lista *) list;	
	list = l->atual;		
	if(l->atual->conecta[0] != NULL)	
		l->atual->conecta[0]->conecta[LIST_CON - 1] = l->atual->conecta[LIST_CON-1];	
	if(l->atual->conecta[LIST_CON-1] != NULL) {	
		l->atual->conecta[LIST_CON - 1]->conecta[0] = l->atual->conecta[0];
		l->atual = l->atual->conecta[LIST_CON-1];
	} else {
		l->atual = l->atual->conecta[0];
		l->index--;							
	}	
	
	l->len--;
	free(list); //*/
}
void list_del_all (void * list) {
	while(list_get_len(list) > 0)
		list_del(list, list_get_index(list));
	if(list != NULL)
		free(list);	
} 
int list_insert (void * list, void * valor) {	
	return list_set(list,list_get_index(list),valor);
}
void * new_list (int size) {
	lista *l = (lista*)malloc(sizeof(lista));
	l->atual = NULL;
	l->index = 0;
	l->len = 0;	
//	while(list_get_len(l) < size) 	list_add(l, NULL);
	return l;
}