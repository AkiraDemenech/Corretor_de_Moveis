#include<stdlib.h>
#include"quadra.h"
#include"moradias.h"

typedef struct quad {
	float x, y, w, h;
	char * cep;
	char * esp;	
	char * strk;
	char * fill;		
} quadra;

void * new_quadra (float x, float y, float w, float h, char * esp, char * cc, char * cp, char * cep) {
	quadra * q = (quadra *) malloc(sizeof(quadra));
	quadra_set_x(q,x);//q->x = x;
	quadra_set_y(q,y);//q->y = y;
	quadra_set_h(q,h);//q->h = h;
	quadra_set_w(q,w);//q->w = w;

	quadra_set_fill(q,cp);// q->fill = cp;
	quadra_set_strk(q,cc);// q->strk = cc;
	quadra_set_esp(q,esp);// q->esp = esp;
	
	quadra_set_cep(q,cep); //q->cep = cep;

	return q;

}

float quadra_get_w (void * q) {
	if(q != NULL)
		return ((quadra*)q)->w;	
	return 0;	
}

float quadra_get_h (void * q) {
	if(q != NULL)
		return ((quadra*)q)->h;	
	return 0;	
}

float quadra_get_y (void * q) {
	if(q != NULL)
		return ((quadra*)q)->y;	
	return 0;	
}

float quadra_get_x (void * q) {
	if(q != NULL)
		return ((quadra*)q)->x;	
	return 0;	
}

void quadra_set_x (void * q, float x) {
	if(q != NULL)
		((quadra*)q)->x = x;	
}

void quadra_set_y (void * q, float y) {
	if(q != NULL)
		((quadra*)q)->y = y;	
}

void quadra_set_w (void * q, float w) {
	if(q != NULL)
		((quadra*)q)->w = w;
}

void quadra_set_h (void * q, float h) {
	if(q != NULL)
		((quadra*)q)->h = h;
}

void quadra_set_esp (void * q, char * esp) {
	if(q != NULL)
		((quadra*)q)->esp = esp;	
}

char * quadra_get_esp (void * q) {
	if(q == NULL)
		return NULL;
	return ((quadra*)q)->esp;	
}


char * quadra_get_strk (void * q) {
	if(q == NULL)
		return NULL;
	return ((quadra*)q)->strk;	
}

char * quadra_get_fill (void * q) {
	if(q == NULL)
		return NULL;
	return ((quadra*)q)->fill;	
}

char * quadra_get_cep (void * q) {
	if(q == NULL)
		return NULL;
	return ((quadra*)q)->cep;	
}

void quadra_set_cep (void * q, char * zip) {
	if(q != NULL)
		((quadra*)q)->cep = zip;	
}

void quadra_set_strk (void * q, char * cc) {
	if(q != NULL)
		((quadra*)q)->strk = cc;	
}

void quadra_set_fill (void * q, char * cp) {
	if(q != NULL)
		((quadra*)q)->fill = cp;	
}

float cep_get_x (float x, float w, char f, int n) {
	if(f == NORTE || f == SUL)	
		return x + n;
	if(f == OESTE)	
		return x + w;
	return x; // leste	
}

float cep_get_y (float y, float h, char f, int n) {
	if(f == LESTE || f == OESTE)
		return y + n;
	if(f == NORTE)
		return y + h;
	return y; // sul
}

void copy (char * x, char * y) {
	if(x != NULL && y != NULL) {
		while(*x != '\0') {
			*y = *x;
			y++;
			x++;
		}			
		*y = '\0';
	}		
}