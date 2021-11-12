#ifndef __VIAS__H
#define __VIAS__H

void * new_vert (char * id, float x, float y);

char * vert_id (void * v);
float vert_y (void * v);

float vert_x (void * v);

void * vert_get_vias (void * v);
void vert_set_vias (void * v, void * caminhos);

void * via_get_de (void * v);
void * via_get_para (void * v);
void via_set_para (void * v, void * destino);
void via_set_de (void * v, void * origem);

void via_set_cmp (void *v, float comprimento);
void via_set_vm (void * v, float velocidade);
float via_get_vm (void * v);
float via_get_cmp (void *v);

char * via_nome (void * v);
char * via_esq (void * v);
char * via_dir (void * v);

void * new_via (char * nome, char * dir, char * esq);

#endif