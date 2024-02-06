#ifndef DESENHOS_H
#define DESENHOS_H

#include <GL/gl.h>
#include <vector>
#include <functional>
#include "salvar.h"

#define TIPO_DESENHO 0
#define TIPO_LINHA 1
#define TIPO_RETANGULO 2
#define TIPO_ELIPSE 3
#define TIPO_TRIANGULO 4
#define TIPO_PONTO 5

extern Desenho meuDesenho;

extern bool pencilTool, eraserTool, lineTool, paintTool, rectTool, elipseTool, triangleTool, pointTool;
extern bool isDraw, isErase, isLine, isDrawRect, isDrawElipse, isDrawTriangle, isDrawPoint;
extern float scale;

extern GLfloat currColorArray[3];
extern GLubyte targetColorArray[3];
extern std::vector<std::pair<float, float>> trianglePoints;
extern std::vector<std::pair<float, float>> pont;
extern std::vector<std::pair<float, float>> point;
extern std::vector<std::pair<float, float>> rectPoints;
extern std::vector<std::vector<std::pair<float, float>>> elipsesPoints;
extern std::vector<std::pair<float, float>> apagaCurvas;
extern std::vector<std::pair<float, float>> lineCurvas;
extern std::vector<std::pair<float, float>> linhaCurvas;
extern std::vector<std::pair<float, float>> paintedPoints;
extern std::vector<std::function<void()>> functionsToCall;

void draw(std::vector<std::pair<float, float>> tempPoints, const float* tempColorArray);
void drawPoint(std::vector<std::pair<float, float>> tempPont, const float* tempColorArray);
void linha(std::vector<std::pair<float, float>> tempLinePoints, const float* tempCorArray);
void retangulo(std::vector<std::pair<float, float>> tempRectPoints, const float* tempColorArr);
void elipse(std::vector<std::vector<std::pair<float, float>>> tempElipsePoints, const float* tempColorArray);
void baldeTinta(int x, int y, GLfloat fillColor[3], GLubyte targetColor[3]);
void triangle(std::vector<std::pair<float, float>> tempTrianglePoints, const float* tempColorArr);
void eraser(std::vector<std::pair<float, float>> tempApagaCurvas);
void triangleRT();
void drawRT();
void drawPointRT();
void linhaRT();
void retanguloRT();
void elipseRT();
void eraseRT();
void drawPencil();
void drawEraser();
void drawLine();
void drawRect();
void drawElipse();
void drawTriangle();

#endif // DESENHOS_H
