#ifndef __CIDADE__H // o mesmo de cidade.h
#define __CIDADE__H	// há várias funções homônimas
#define COM_TAM 3
#define CEP_TAM 32
#define NOME_TAM 128

void quadras_svg (void * img, void * arvore);

char * cidade_get_nome (void * cid);
void * cidade_geo (char * geo);
void cidade_svg (char * svg, void * cidade);
void * new_cidade (char * nome, int tamanho);

void cidade_vias (void * cid, char * vias);
void cidade_del_all (void * cid);

int cidade_get_tamanho (void * cid);
void cidade_set_tamanho (void * cid, int tamanho);

void * cidade_get_pontos_em (void * cid, float x, float y, float w, float h);
void * cidade_get_quadras_em (void *cid, float x, float y, float w, float h);

void * cidade_get_quadras_avl (void *cid);
void * cidade_get_quadra_avl (void * cid, float x, float y);
void cidade_del_quadra_avl (void * cid, float x, float y);

void * cidade_get_quadras_hash (void *cid);
void * cidade_get_quadra_hash (void * cid, char * cep);
void cidade_del_quadra_hash (void * cid, char * cep);

void cidade_set_quadras (void *cid, int n);
void cidade_set_quadra (void * cid, void * quadra);

void cidade_insort_quadra (void * lista, void * quadra);
int busca_bin_quadra (void * lista, float y);

void cidade_insort_ponto (void * lista, void * ponto);
int busca_bin_ponto (void * lista, float y);

void * cidade_get_pontos (void *cid);
void * cidade_get_ponto (void * cid, float x, float y);
void cidade_set_ponto (void * cid, void * p);

int cidade_del_ponto (void * cid, float x, float y);

void * cidade_get_vias_list (void * cid);
void cidade_set_vias_list (void * cid, void * vias);

void * cidade_get_vias_vm (void * cid);
void cidade_set_vias_vm (void * cid, void * vias);

void * cidade_get_vias_cmp (void * cid);
void cidade_set_vias_cmp (void * cid, void * vias);

void cidade_set_c_cmp (void * cid, int cmp_c);
int cidade_get_c_cmp (void * cid);

int cidade_get_c_vm (void * cid);
void cidade_set_c_vm (void * cid, int vm_c);

int dijkstra (void * grafo, void * final, void * fila, void * caminhos, float cv, int c);

#endif