#ifndef __CIDADE__H
#define __CIDADE__H
#define COM_TAM 3
#define CEP_TAM 32
#define NOME_TAM 128

char * cidade_get_nome (void * cid);
void * cidade_geo (char * geo);
void * new_cidade (char * nome, int tamanho);

void cidade_pm (void * cid, char * pm);
void cidade_del_all (void * cid);

int cidade_get_tamanho (void * cid);
void cidade_set_tamanho (void * cid, int tamanho);

void *cidade_get_pessoas (void* cid);
void *cidade_get_pessoa (void * cid, char * cpf);
void cidade_set_pessoas (void * cid, int n);
void cidade_set_pessoa (void * cid, void * pessoa);
void cidade_del_pessoa (void * cid, char * cpf);
void cidade_del_pessoas (void* cid);

void * cidade_get_quadras_em (void * cid, float x, float y, float w, float h);

void * cidade_get_quadras_avl (void * cid);
void * cidade_get_quadra_avl (void * cid, float x, float y);
void cidade_del_quadra_avl (void * cid, float x, float y);

void * cidade_get_quadras_hash (void * cid);
void * cidade_get_quadra_hash (void * cid, char * cep);
void cidade_del_quadra_hash (void * cid, char * cep);

void cidade_set_quadras (void * cid, int n);
void cidade_set_quadra (void * cid, void * quadra);

void cidade_insort_quadra (void * lista, void * quadra);
int busca_bin_quadra (void * lista, float y);

void cidade_set_moradia (void * cid, void * moradia);
void cidade_set_moradias (void *cid, int n);
void * cidade_get_moradias_cpf (void * cid);
void * cidade_get_moradias_cep (void * cid);


#endif