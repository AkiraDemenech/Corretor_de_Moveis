#include"svg.h"
#include<stdio.h>
//#include<stdlib.h>

void svg_open (void* arq) {
	fprintf(arq,"<svg> \n");	
}
void svg_close (void* arq) {
	fprintf(arq,"\n</svg> ");
}
void svg_coment_open (void *svg) {
	fprintf(svg,"\n\t<!--");
}
void svg_coment_close (void *svg) {
	fprintf(svg,"-->\t\n\n");
}
void svg_coment (void * arq, char * text) {
	svg_coment_open(arq);
	if(text != NULL)
		fprintf(arq,text);
	svg_coment_close(arq);
}

void svg_text (void * arq, char * text, char * cor, float x, float y) {
	svg_text_open(arq,cor,x,y);
	if(text != NULL)
		fprintf(arq,text);
	svg_text_close(arq);
	fprintf(arq,"\n");
}
void svg_text_open (void *svg, char * color, float x, float y) {
	fprintf(svg,"\t\t<text y=\"%f\" x=\"%f\"",y,x);
	if(color != NULL)
		fprintf(svg,"\tfill=\"%s\"",color);
	fprintf(svg,">");	
}
void svg_text_close (void *svg) {
	fprintf(svg,"</text>");
}

void svg_rect (void * arq, char * id, char * cp, char * cc, char * esp, float x, float y, float w, float h) {
	svg_rect_open(arq,id,cp,cc,esp,x,y,w,h,1);
	svg_text_open(arq,cc,x+(w*0.1),y+(h*0.9));
	if(id != NULL) fprintf(arq,id);
	svg_text_close(arq);
	svg_rect_close(arq);
}
void svg_rect_open (void *svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float width, float height, float opacity) {
	fprintf(svg,"\t\t<rect height=\"%f\" y=\"%f\" x=\"%f\" width=\"%f\"\tstyle=\"",height,y,width,x);
	if(fill == NULL)
		opacity = 0;
	else fprintf(svg,"fill:%s;",fill);	
	fprintf(svg,"opacity:%f;stroke",opacity);
	if(stroke == NULL)
		fprintf(svg,"-opacity:0");
	else fprintf(svg,":%s",stroke);
	if(swid != NULL)
		fprintf(svg,";stroke-width:%s",swid);
	if(id != NULL)	
		fprintf(svg,"\"\tid=\"%s",id);
	fprintf(svg,"\">");	
}
void svg_rect_close (void *svg) {
	fprintf(svg,"</rect> \n");
}




// círculo
// elipse
// linha