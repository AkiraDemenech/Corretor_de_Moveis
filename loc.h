#ifndef __LOC__H
#define __LOC__H
#define LOCADA '*'
#define ENCERR '#'
#define DISP '$'

void* new_loc (float custo, float m2, char *id, void * imovel);
char* loc_id (void * loc);

void loc_del (void * l);

void *loc_get_moradia (void* loc);
void loc_set_moradia (void * loc, void * mor);

void loc_set_valor (void * loc, float v);
float loc_get_valor (void *loc);

float loc_get_area (void *loc);
void loc_set_area (void * loc, float a);

void loc_set_disponibilidade (void * loc, char disp);
char loc_get_disponibilidade (void * loc);



#endif