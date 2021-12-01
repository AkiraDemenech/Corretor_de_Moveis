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

void svg_line (void * svg, char * color, char * width, float xi, float yi, float xf, float yf, float dash) {
	if(svg == NULL)
		return;
	fprintf(svg,"\t\t<line x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\"\tstroke=\"",xi,xf,yi,yf);	
	if(color == NULL)
		fprintf(svg,"black");
	else fprintf(svg, color);
	if(width != NULL)	
		fprintf(svg,"\" stroke-width=\"%s",width);
	if(dash >= 0)
		fprintf(svg,"\" stroke-dasharray=\"%f",dash);	
	fprintf(svg,"\"/>\n");	

}
void svg_circle (void * arq, char * id, char * cp, char * cc, char * swid, float x, float y, float d) {
	svg_circle_open(arq, id, cp, cc, swid, x, y, d/2);
	svg_circle_close(arq);
	svg_text(arq, id, cc, 0, x, y);	
}
void svg_circle_open (void * svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float r) {
	if(svg == NULL)
		return;
	fprintf(svg,"\t\t<circle cx=\"%f\" cy=\"%f\" r=\"%f\" \tstyle=\"",x,y,r);	
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
void svg_circle_close (void *svg) {
	if(svg != NULL)
		fprintf(svg,"</circle> \n");
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


void svg_path_open (void * img) {
	if(img != NULL) 
		fprintf(img, "<path");	
}
void svg_path_id_open (void * img) {
	if(img != NULL) 
		fprintf(img, " id=\"");
	
}
void svg_path_points_open (void * img) {
	if(img != NULL) 
		fprintf(img, "\"\td=\"");	
}
void svg_path_point (void * img, char t, float x, float y) {
	if(img != NULL) 
		fprintf(img, "%c %f,%f ", t, x, y);
}
void svg_path_close (void * img, char * stroke, char * width) {
	if(img != NULL) {
		fprintf(img, "\"\tfill=\"none\"");
		if(stroke != NULL)
			fprintf(img," stroke=\"%s\"",stroke);
		if(width != NULL)
			fprintf(img," stroke-width=\"%s\"",width);
		fprintf(img, " />\n");		
	}	
	
}

void svg_animation_open (void * img, char * repeat, float duration) {
	if(img != NULL) {
		fprintf(img, "\t<animateMotion dur=\"%fs\"",duration);
		if(repeat != NULL)
			fprintf(img,"\trepeatCount=\"%s\"",repeat);
		fprintf(img, "> \n");	
	}
}
void svg_animation_path_open (void * img) {
	if(img != NULL)
		fprintf(img, "\t\t<mpath xlink:href=\"#");
}
void svg_animation_path_close (void * img) {
	if(img != NULL)
		fprintf(img, "\"/>\t\n");
}
void svg_animation_close (void * img) {
	if(img != NULL) 
		fprintf(img,"\t</animateMotion>\n");	
	
}

void hex64 (char *c, int v) {
	c[HEX - 1] = '\0';
	*c = '#';
	
	int u;
	int i;
	for(i = 1; i <= HEX - 2; i++) {
		c[i] = (v % 2) * ('F' - '0') + '0';
		v /= 2;
	}	
	for(u = 2; u <= 3; u++) 
		for(i = HEX - u; i > 2; i -= 2)
			if(c[i] != c[i - 2])
				return;
	i = 1 + (v % (HEX - 2));			
	c[i] = (c[i] == '0') ? ('f') : ('0');
//	printf(c);
}