#include <GL/glut.h>
#include <stdlib.h>

#ifndef SCENE_H
#define SCENE_H

void init(void);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void update(int value);
void loadTexture();
void updateProjectionMatrix(int width, int height);
void keyboard(unsigned char key, int x, int y);

#endif
