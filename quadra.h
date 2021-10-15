#ifndef __QUADRAS__H
#define __QUADRAS__H 

void copy (char * x, char * y);

float cep_get_y (float y, float h, char f, int n);
float cep_get_x (float x, float w, char f, int n);

void * new_quadra (float x, float y, float w, float h, char* esp, char * cc, char * cp, char * cep);

void quadra_set_fill (void * q, char * cp);
char * quadra_get_fill (void * q);

void quadra_set_strk (void * q, char * cc);
char * quadra_get_strk (void * q); 

void quadra_set_cep (void * q, char * zip);
char * quadra_get_cep (void * q);

char * quadra_get_esp (void * q);
void quadra_set_esp (void * q, char * esp);

void quadra_set_x (void * q, float x);
void quadra_set_y (void * q, float y);
void quadra_set_w (void * q, float w);
void quadra_set_h (void * q, float h);

float quadra_get_x (void * q);
float quadra_get_y (void * q);
float quadra_get_w (void * q);
float quadra_get_h (void * q);

#endif