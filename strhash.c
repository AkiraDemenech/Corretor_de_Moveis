#include<stdlib.h>
#include <stdio.h>
#include"hash.h"
#include"list.h"

typedef struct table {
	int len;
	void ** vetor;
	void * chaves;
} tabela;


void ** hash_v_get_list (tabela * t, int pos) {
	if(t == NULL)
		return NULL;
	return t->vetor + pos;
}

void * new_hash_table (int size) {	
	tabela * t = (tabela*)malloc(sizeof(tabela));
	t->vetor = calloc(size,sizeof(void*));		
	t->len = size;
//	while(size > 0) 	t->vetor[--size] = new_list(0);	
	return t;
}

int hash_get_len (void * tabesp) {
	if(tabesp == NULL) 
		return -1;
	return ((tabela*)tabesp)->len;	
}

int hash_get_chaves_len (void * tabesp) {
	if(tabesp != NULL)
		return list_get_len(((tabela*)tabesp)->chaves);
	return list_get_len(NULL);	
}

void * hash_get_chave (void * tabesp, int index) {
	if(tabesp != NULL)
		return li_get_valor(list_get(((tabela*)tabesp)->chaves,index));
	return NULL;	
}

void * hash_get_all (void * tabesp, char * chave) {
	if(tabesp == NULL) 
		return NULL;
	void * l = * hash_v_get_list(tabesp, hash(chave) % hash_get_len(tabesp));	
	void * r = new_list(0);
	int c;
	for(c = 0; c < list_get_len(l); c++)
		if(compare(li_get_chave(list_get(l,c)),chave) == 0) 
			list_insert(r, li_get_valor(list_get(l, c)));
					
	return r;
}

void * hash_get (void * tabesp, char * chave) {
	if(tabesp == NULL) 
		return NULL;
	void * l = * hash_v_get_list(tabesp, hash(chave) % hash_get_len(tabesp));	
//	void * v = NULL;	
	int c;
	for(c = 0; c < list_get_len(l); c++) 		
		if(compare(li_get_chave(list_get(l,c)),chave) == 0) 
			return li_get_valor(list_get(l, c));							
	return NULL;
}

void hash_set (void * tabesp, char * chave, void * valor) {
	if(tabesp == NULL) 
		return;	

	int h = hash(chave) % hash_get_len(tabesp);	
	void ** l = hash_v_get_list(tabesp, h);
	if(*l == NULL)
		*l = new_list(0);
//	else printf("[%d]\tColisão!\t %s\tjá existentes %d com mesma hash \n",h,chave,list_get_len(*l));	
//	printf("{%d}[%d]\n",h,list_get_len(*l));
	list_insert(((tabela*)tabesp)->chaves,chave);
	list_insert(*l,valor);	
	li_set_chave(list_get_atual(*l),chave);
}

void hash_del (void * tabesp, char * chave) {
	if(tabesp == NULL) 
		return;
	void ** l = hash_v_get_list(tabesp, hash(chave) % hash_get_len(tabesp));	
	void * ch = ((tabela *) tabesp)->chaves;
	int c;
	for(c = list_get_len(*l) - 1; c >= 0; c--)
		if(compare(li_get_chave(list_get(*l, c)), chave) == 0) {
			list_del(* l,c);
			for(c = list_get_len(ch) - 1; c >= 0; c--)
				if(compare(li_get_valor(list_get(ch, c)), chave) == 0) {
					list_del(ch, c);					
					return;
				}	
			break; // se não tiver encontrado na lista de chaves da tabela
		}	
	// somente se a deleção for reiterada
	if(list_get_len(* l) <= 0) {	 
		list_del_all(*l);
		*l = NULL;
	}								
}

void hash_del_all (void * tabesp) {
	tabela * t = (tabela*)tabesp;
	if(tabesp == NULL) return;	
	while(t->len-- > 0) 
		list_del_all(t->vetor[t->len]);		
	list_del_all(t->chaves);	
	free(t->vetor);
	free(tabesp);	
}

int compare (char * x, char * y) {
	if(x == y)
		return 0;
	if(x == NULL)	
		return 4;
	if(y == NULL)	
		return -4;
	while(*x != '\0' && *y != '\0') {
		if(*x < *y)
			return 1;
		if(*x > *y)	
			return -1;			
		x++;
		y++;	
	}
//	if(*x == *y)
	//	return 0;
	if(*x == '\0') {		
		if(*y != '\0')
			return 2;				
	} else if(*y == '\0')		 	
		return -2;
	return 0;	
}

void clean (char * v, int c) {
	if(v != NULL)
		while(c-- > 0) 
			v[c] = '\0';
}

unsigned int hash (char * str) {
	if(str == NULL)
		return 0;
	int r = 1;	
	while(*str != '\0') {
		r = (r * 31) + (* str);
		str++;
	}
	if(r < 0)
		return -r;
	return r;
}