#include <stdio.h>
#include<stdlib.h>
#include"mapa.h"
#include"qry.h"
#include"arq.h"


int main (int argc, char *argv[]) {

	char* bsd = NULL;// -o
	char* bed = NULL;// -e
	char* geo = NULL;//	-f
	char* qry = NULL;//	-q
	char* via = NULL;//	-v

	char * u = NULL;

	int c = argc;
	while(c-- > 1) {
		
		if(argv[c][0] == '-')
			switch (argv[c][1])
			{
				case 'e':
					printf("\nBED:\t%s",u);
					bed = u;					
					continue;

				case 'o': 	
					printf("\nBSD:\t%s",u);
					bsd = u;
					continue;

				case 'f':	
					printf("\nGEO:\t%s",u);
					geo = u;
					continue;

				case 'q':
					printf("\nQRY:\t%s",u);
					qry = u;	
					continue;

				case 'v':	
					via = u;					
					printf("\nVIA:\t%s",u);
					continue;
			}

		u = argv[c];
	}

	printf("\n");	

	if(bsd == NULL || geo == NULL) {

		if(geo == NULL) 
			printf("\t-f GEO file not found\n");		

		if(bsd == NULL) 
			printf("\t-o BSD directory not found\n");
		
		return -1;
	}

	char * geo_completo = arq_nome_completo(bed,geo);
	char * qry_completo = qry;	
	char * via_completo = via;
	if(via != NULL)
		via_completo = arq_nome_completo(bed, via);
	if(qry != NULL)
		qry_completo = arq_nome_completo(bed,qry);	

	

	printf("BED + VIA:\t%s\n", via_completo);
	printf("BED + GEO:\t%s\n",geo_completo);
	printf("BED + QRY:\t%s\n",qry_completo);

	char * arq = arq_nome_completo(bsd,arq_nome_concat(1,'-','.',arq_nome(geo),arq_nome(qry)));
//	arq_nome_completo(bsd,arq_nome_concat(0,'.','.',arq,"EXT"));
	
	printf("BSD/<ARQ>:\t%s\n\n",arq);
	
	
	
	// ler GEO 
	printf("Lendo %s:\n",geo_completo);
	void * cidade = cidade_geo(geo_completo);
	printf("%s lida com sucesso.\n",cidade_get_nome(cidade));

	

	printf("\n");

	if(via_completo != NULL)
	{
		printf("Lendo %s:\n",via_completo);
		cidade_vias(cidade,via_completo); // ler VIAS
		printf("Vias lidas com sucesso.\n");			
	}

	// escrever SVG inicial
	printf("Escrevendo SVG:\t");
	cidade_svg(arq_nome_completo(bsd,arq_nome_concat(0,'.','.',arq_nome(geo),"svg")),cidade);	

	if(qry_completo != NULL)
	{
		printf("\nIniciando leitura de %s\n",qry_completo);
		cidade_qry(cidade,qry_completo,arq_nome_concat(0,'.','.',arq,"svg"),arq_nome_concat(0,'.','.',arq,"txt"));// rodar QRY
		printf("Finalizadas as consultas.\n");
	}	
	printf("\n");

	
	printf("Iniciando limpeza");
	
	cidade_del_all(cidade);
	printf(".");

	char opt = '.';
	if(geo == geo_completo)
		opt = '\'';
	else free(geo_completo);
	printf("%c", opt);
	
	opt = '.';
	if(qry_completo != qry)
		free(qry_completo);
	else opt = ',';	
	printf("%c",opt);

	if(via_completo != via)
		free(via_completo);
	else opt = ';'; 	
	printf("%c",opt);

	

	// */
	printf("\nLimpeza concluída com sucesso!\n\n ");
	return 0;
}