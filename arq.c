
#include<stdlib.h>
#include"arq.h"

int comprimento (char * s) {
	int c = 0;
	if(s != NULL)
		while(s[c] != '\0')
			c++;
	return c;	
}

int ult_ind (char ch, char * s) {	
	if(s == NULL)
		return -2;
	int i = -1, c;
	for(c = 0; s[c] != '\0'; c++)
		if(s[c] == ch)
			i = c;
	return i;		
}

char * arq_nome (char * nome_completo) {
	if(nome_completo != NULL) {
		int c = 0, b = -1;
		while(nome_completo[c] != '\0') {
			if(nome_completo[c] == '/' || nome_completo[c] == '\\')
				b = c;
			c++; 	
		}	
		nome_completo += b + 1;		
	}
	return nome_completo;	
}

char * arq_nome_completo (char * caminho, char * nome) {
	if(caminho == NULL)
		return nome;
	if(nome == NULL)
		return caminho;		

	char * nome_completo = malloc(sizeof(char) * (comprimento(caminho) + comprimento(nome)));			
	int i, c = 0;
	for(i = 0; caminho[i] != '\0'; i++) 
		nome_completo[c++] = caminho[i]; 
	if(caminho[i-1] != '\\' && caminho[i-1] != '/' && nome[0] != '/' && nome[0] != '\\')
		nome_completo[c++] = '/';	
	else if((caminho[i-1] == '\\' || caminho[i-1] == '/') && (nome[0] == '/' || nome[0] == '\\'))	
		c--;
	for(i = 0; nome[i] != '\0'; i++)
		nome_completo[c++] = nome[i];	
	nome_completo[c++] = '\0';	
	return nome_completo;
	
}

char * arq_nome_concat (unsigned int cauda, char sep, char ult, char * x, char * y) {
	if(x == NULL)
		return y;
	if(y == NULL)	
		return x;
	int lx = ult_ind(ult,x);
	int ly = ult_ind(ult,y);
	if(lx < 0)
		lx = comprimento(x);
	if(ly < 0)
		ly = comprimento(y);
//	ly += cauda;	
	
	char * nome_concat = malloc(sizeof(char) * (2 + cauda + lx + ly));
	
	int i, j;
	for(i = 0; i < lx; i++)
		nome_concat[i] = x[i];
	nome_concat[i++] = sep;
	for(j = 0; j < ly; j++)
		nome_concat[i + j] = y[j];	
	while(cauda-- > 0)	
		nome_concat[i + j++] = ult;
	nome_concat[i + j] = '\0';		
	return nome_concat;
}