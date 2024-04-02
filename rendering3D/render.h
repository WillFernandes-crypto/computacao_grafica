#include <GL/glut.h>
#include <stdlib.h>

#ifndef RENDER_H
#define RENDER_H

void init(void);
void teapot();
void display(void);
void reshape(int w, int h);
void update(int value);
void keyboard(unsigned char key, int x, int y);

#endif

