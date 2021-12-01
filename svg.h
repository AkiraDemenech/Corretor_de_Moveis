#ifndef __SVG__H
#define __SVG__H
#define HEX 8

void hex64 (char *c, int v);


void svg_open (void* arq);
void svg_close (void* arq);

void svg_comment (void * arq, char * text);
void svg_comment_open (void *svg);
void svg_comment_close (void *svg);

void svg_text (void * arq, char * text, char * cor, float tam, float x, float y);
void svg_text_open (void *svg, char * color, float size, float x, float y);
void svg_text_close (void *svg);

void svg_rect (void * arq, char * id, char * cp, char * cc, char * esp, float x, float y, float w, float h);
void svg_rect_open (void *svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float width, float height, float opacity, float dash);
void svg_rect_close (void *svg);

void svg_ellipse (void * img, char * id, char * cp, char * cc, char * swid, float x, float y, float w, float h);
void svg_ellipse_open (void * svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float width, float height);
void svg_ellipse_close (void *svg);

void svg_circle (void * arq, char * id, char * cp, char * cc, char * swid, float x, float y, float d);
void svg_circle_open (void * svg, char * id, char * fill, char * stroke, char * swid, float x, float y, float r);
void svg_circle_close (void *svg);

void svg_animation_open (void * img, char * repeat, float duration);
void svg_animation_path_open (void * img);
void svg_animation_path_close (void * img);
void svg_animation_close (void * img);

void svg_path_open (void * img);
void svg_path_id_open (void * img);
void svg_path_points_open (void * img);
void svg_path_point (void * img, char t, float x, float y);
void svg_path_close (void * img, char * stroke, char * width);

void svg_line (void * svg, char * color, char * width, float xi, float yi, float xf, float yf, float dash);


#endif