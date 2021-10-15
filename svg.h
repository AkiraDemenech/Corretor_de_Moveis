#ifndef __SVG__H
#define __SVG__H


void svg_open (void* arq);
void svg_close (void* arq);

void svg_comment (void * arq, char * text);
void svg_comment_open (void *svg);
void svg_comment_close (void *svg);

void svg_text (void * arq, char * text, char * cor, float x, float y);
void svg_text_open (void *svg, char * color, float x, float y);
void svg_text_close (void *svg);

void svg_rect (void * arq, char * id, char * cp, char * cc, char * esp, float x, float y, float w, float h);
void svg_rect_open (void *svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float width, float height, float opacity, float dash);
void svg_rect_close (void *svg);

void svg_ellipse (void * img, char * id, char * cp, char * cc, char * swid, float x, float y, float w, float h);
void svg_ellipse_open (void * svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float width, float height);
void svg_ellipse_close (void *svg);

void svg_circle (void * arq, char * id, char * cp, char * cc, char * swid, float x, float y, float d);
void svg_line (void * svg, char * color, char * width, float xi, float yi, float xf, float yf);


#endif