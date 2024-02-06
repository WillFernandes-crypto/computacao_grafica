#ifndef PERIF_H
#define PERIF_H

#include <GL/gl.h>
#include <vector>
#include <functional>

extern GLfloat currColorArray[3];
extern GLubyte targetColorArray[3];
extern std::vector<std::pair<float, float>> trianglePoints;
extern std::vector<std::pair<float, float>> pontos;
extern std::vector<std::pair<float, float>> rectPoints;
extern std::vector<std::vector<std::pair<float, float>>> elipsesPoints;
extern std::vector<std::pair<float, float>> apagaCurvas;
extern std::vector<std::pair<float, float>> lineCurvas;
extern std::vector<std::pair<float, float>> linhaCurvas;
extern std::vector<std::pair<float, float>> paintedPoints;
extern std::vector<std::function<void()>> functionsToCall;

void display();
void mouseClick(int button, int state, int x, int y);
void mouseMover(int x, int y);
void keyboard(unsigned char key, int x, int y);

#endif // PERIF_H
