#ifndef __SVG__H
#define __SVG__H


void svg_open (void* arq);
void svg_close (void* arq);

void svg_coment (void * arq, char * text);
void svg_coment_open (void *svg);
void svg_coment_close (void *svg);

void svg_text (void * arq, char * text, char * cor, float x, float y);
void svg_text_open (void *svg, char * color, float x, float y);
void svg_text_close (void *svg);

void svg_rect (void * arq, char * id, char * cp, char * cc, char * esp, float x, float y, float w, float h);
void svg_rect_open (void *svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float width, float height, float opacity);
void svg_rect_close (void *svg);
#endif