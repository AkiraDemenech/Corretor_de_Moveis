#include"svg.h"
#include<stdio.h>
//#include<stdlib.h>

void svg_open (void* arq) {	
	if(arq != NULL)
		fprintf(arq,"<svg id='body' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink'> \n");	
}
void svg_close (void* arq) {
	if(arq != NULL)
		fprintf(arq,"\n</svg> ");
}
void svg_comment_open (void *svg) {
	if(svg != NULL)
		fprintf(svg,"\n\t<!--");
}
void svg_comment_close (void *svg) {
	if(svg != NULL)
		fprintf(svg,"-->\t\n\n");
}
void svg_comment (void * arq, char * text) {
	svg_comment_open(arq);
	if(text != NULL && arq != NULL)
		fprintf(arq,text);
	svg_comment_close(arq);
}

void svg_text (void * arq, char * text, char * cor, float tam, float x, float y) {
	if(arq == NULL)
		return;
	svg_text_open(arq,cor,tam,x,y);
	if(text != NULL)
		fprintf(arq,text);
	svg_text_close(arq);
	fprintf(arq,"\n");
}
void svg_text_open (void *svg, char * color, float size, float x, float y) {
	if(svg == NULL)
		return;
	fprintf(svg,"\t\t<text y=\"%f\" x=\"%f\"",y,x);
	if(size > 0)	
		fprintf(svg," font-size=\"%f\"",size);
	if(color != NULL)
		fprintf(svg,"\tfill=\"%s\"",color);		
	fprintf(svg,">");	
}
void svg_text_close (void *svg) {
	if(svg != NULL)
		fprintf(svg,"</text> ");
}

void svg_rect (void * arq, char * id, char * cp, char * cc, char * esp, float x, float y, float w, float h) {
	if(arq != NULL) {
		svg_rect_open(arq,id,cp,cc,esp,x,y,w,h,1,0);		
		svg_rect_close(arq);
		svg_text(arq,id,cc,0,x+(w*0.1),y+(h*0.9));		
	}
}
void svg_rect_open (void *svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float width, float height, float opacity, float dash) {
	if(svg == NULL)
		return;
	fprintf(svg,"\t\t<rect height=\"%f\" y=\"%f\" x=\"%f\" width=\"%f\"\tstyle=\"",height,y,x,width);
	if(fill == NULL)
		opacity = 0;
	else fprintf(svg,"fill:%s;",fill);	
	fprintf(svg,"fill-opacity:%f;stroke",opacity);
	if(stroke == NULL)
		fprintf(svg,"-opacity:0");
	else fprintf(svg,":%s",stroke);
	if(dash >= 0)
		fprintf(svg,";stroke-dasharray:%f",dash);
	if(swid != NULL)
		fprintf(svg,";stroke-width:%s",swid);
	if(id != NULL)	
		fprintf(svg,"\"\tid=\"%s",id);
	fprintf(svg,"\">");	
}
void svg_rect_close (void *svg) {
	if(svg != NULL)
		fprintf(svg,"</rect> \n");
}

void svg_line (void * svg, char * color, char * width, float xi, float yi, float xf, float yf) {
	if(svg == NULL)
		return;
	fprintf(svg,"\t\t<line x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\"\tstyle=\"",xi,xf,yi,yf);	
	if(color != NULL)
		fprintf(svg,"stroke:%s;",color);
	if(width != NULL)	
		fprintf(svg,"stroke-width:%s",width);
	fprintf(svg,"\"/>\n");	

}
void svg_circle (void * arq, char * id, char * cp, char * cc, char * swid, float x, float y, float d) {
	svg_ellipse(arq, id, cp, cc, swid, x, y, d, d);
}
void svg_ellipse (void * img, char * id, char * cp, char * cc, char * swid, float x, float y, float w, float h) {
	svg_ellipse_open(img,id,cp,cc,swid,x,y,w,h);
	svg_ellipse_close(img);
	svg_text(img, id, cc, 0, x, y + (h/2));	
}
void svg_ellipse_open (void * svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float width, float height) {
	if(svg == NULL)
		return;
	fprintf(svg,"\t\t<ellipse cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\"\tstyle=\"",x,y,width/2,height/2);	
	if(fill != NULL)
		fprintf(svg,"fill:%s;",fill);	
	fprintf(svg,"stroke");
	if(stroke == NULL)
		fprintf(svg,"-opacity:0");
	else fprintf(svg,":%s",stroke);
	if(swid != NULL)
		fprintf(svg,";stroke-width:%s",swid);
	if(id != NULL)	
		fprintf(svg,"\"\tid=\"%s",id);
	fprintf(svg,"\">");
}
void svg_ellipse_close (void *svg) {
	if(svg != NULL)
		fprintf(svg,"</ellipse> \n");
}