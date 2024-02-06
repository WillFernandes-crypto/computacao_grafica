#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <functional>
#include <stack>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "transforma.h"
#include "desenhos.h"
#include "perif.h"
#include "salvar.h"

Desenho meuDesenho;

bool pencilTool, eraserTool, lineTool, paintTool, rectTool, elipseTool, triangleTool, pointTool = true;
bool isDraw, isErase, isLine, isDrawRect, isDrawElipse, isDrawTriangle, isDrawPoint = false;
float scale = 0.85f;

GLfloat currColorArray[3];
GLubyte targetColorArray[3];
std::vector<std::pair<float, float>> points;
std::vector<std::pair<float, float>> pont;
std::vector<std::pair<float, float>> trianglePoints;
std::vector<std::pair<float, float>> pontos;
std::vector<std::pair<float, float>> rectPoints;
std::vector<std::vector<std::pair<float, float>>> elipsesPoints;
std::vector<std::pair<float, float>> apagaCurvas;
std::vector<std::pair<float, float>> lineCurvas(2);
std::vector<std::pair<float, float>> linhaCurvas(2);
std::vector<std::pair<float, float>> paintedPoints;
std::vector<std::function<void()>> functionsToCall;

void drawRT() {

    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& ponto : pontos) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();

}

void draw(std::vector<std::pair<float, float>> tempPoints, const float* tempColorArray) {
    glColor3f(tempColorArray[0], tempColorArray[1], tempColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& ponto : tempPoints) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();

// Adiciona os pontos ao objeto meuDesenho
    meuDesenho.tipo = TIPO_DESENHO;  // Substitui com o tipo correto
    meuDesenho.cor[0] = tempColorArray[0];
    meuDesenho.cor[1] = tempColorArray[1];
    meuDesenho.cor[2] = tempColorArray[2];
    meuDesenho.pontos = tempPoints;

    // Adiciona todas as coordenadas ao vetor todasCoordenadas
    meuDesenho.todasCoordenadas.insert(meuDesenho.todasCoordenadas.end(), tempPoints.begin(), tempPoints.end());

}

//Desenhar o ponto
void drawPoint(std::vector<std::pair<float, float>> tempPont, const float* tempColorArray) {
    glColor3f(tempColorArray[0], tempColorArray[1], tempColorArray[2]);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    for (const auto& ponto : tempPont) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();

    meuDesenho.tipo = TIPO_PONTO;
    meuDesenho.cor[0] = tempColorArray[0];
    meuDesenho.cor[1] = tempColorArray[1];
    meuDesenho.cor[2] = tempColorArray[2];
    meuDesenho.pontos = tempPont;

    // Adiciona as coordenadas ao vetor todasCoordenadas
    meuDesenho.todasCoordenadas.insert(meuDesenho.todasCoordenadas.end(), tempPont.begin(), tempPont.end());
}

void drawPointRT() {

    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glBegin(GL_POINTS);
    for (const auto& ponto : pont) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();

}

void triangleRT() {
    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (const auto& ponto : trianglePoints) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();
}

void triangle(std::vector<std::pair<float, float>> tempTrianglePoints, const float* tempColorArr) {
    glColor3f(tempColorArr[0], tempColorArr[1], tempColorArr[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (const auto& ponto : tempTrianglePoints) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();

    // Adiciona os pontos ao objeto meuDesenho
    meuDesenho.tipo = TIPO_TRIANGULO;  // Substitui com o tipo correto
    meuDesenho.cor[0] = tempColorArr[0];
    meuDesenho.cor[1] = tempColorArr[1];
    meuDesenho.cor[2] = tempColorArr[2];
    meuDesenho.pontos = tempTrianglePoints;

    // Adiciona todas as coordenadas ao vetor todasCoordenadas
    meuDesenho.todasCoordenadas.insert(meuDesenho.todasCoordenadas.end(), tempTrianglePoints.begin(), tempTrianglePoints.end());

}

// Função apagador em tempo real
void eraseRT() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(20.0f);
    glBegin(GL_POINTS);
    for (const auto& apaga : apagaCurvas) {
        glVertex2f(apaga.first, apaga.second);
    }
    glEnd();
}

void eraser(std::vector<std::pair<float, float>> tempApagaCurvas) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(20.0f);
    glBegin(GL_POINTS);
    for (const auto& apaga : tempApagaCurvas) {
        glVertex2f(apaga.first, apaga.second);
    }
    glEnd();

}

// Função de desenhar linha em tempo real
void linhaRT() {

    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& ponto : lineCurvas) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();

}

void linha(std::vector<std::pair<float, float>> tempLinePoints, const float* tempCorArray) {
    glColor3f(tempCorArray[0], tempCorArray[1], tempCorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto& ponto : tempLinePoints) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();

    // Adiciona os pontos ao objeto meuDesenho
    meuDesenho.tipo = TIPO_LINHA;  // Substitui isso com o tipo correto para a linha
    meuDesenho.cor[0] = tempCorArray[0];
    meuDesenho.cor[1] = tempCorArray[1];
    meuDesenho.cor[2] = tempCorArray[2];
    meuDesenho.pontos = tempLinePoints;

    // Adiciona todas as coordenadas ao vetor todasCoordenadas
    meuDesenho.todasCoordenadas.insert(meuDesenho.todasCoordenadas.end(), tempLinePoints.begin(), tempLinePoints.end());

}

// Função de desenhar retangulo em tempo real
void retanguloRT() {

    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (const auto& ponto : rectPoints) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();

}

void retangulo(std::vector<std::pair<float, float>> tempRectPoints, const float* tempColorArr) {

    glColor3f(tempColorArr[0], tempColorArr[1], tempColorArr[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (const auto& ponto : tempRectPoints) {
        glVertex2f(ponto.first, ponto.second);
    }
    glEnd();

    // Adiciona os pontos ao objeto meuDesenho
    meuDesenho.tipo = TIPO_RETANGULO;  // Substitui isso com o tipo correto para o retângulo
    meuDesenho.cor[0] = tempColorArr[0];
    meuDesenho.cor[1] = tempColorArr[1];
    meuDesenho.cor[2] = tempColorArr[2];
    meuDesenho.pontos = tempRectPoints;

    // Adiciona todas as coordenadas ao vetor todasCoordenadas
    meuDesenho.todasCoordenadas.insert(meuDesenho.todasCoordenadas.end(), tempRectPoints.begin(), tempRectPoints.end());


}

//Função de desenhar elipse em tempo real
void elipseRT() {
    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glLineWidth(2.0f);
    for (const auto& elipsePoints : elipsesPoints) {
        glBegin(GL_LINE_LOOP);
        float rx = std::abs(elipsePoints[1].first - elipsePoints[0].first) / 2.0f;
        float ry = std::abs(elipsePoints[1].second - elipsePoints[0].second) / 2.0f;
        float xc = (elipsePoints[0].first + elipsePoints[1].first) / 2.0f;
        float yc = (elipsePoints[0].second + elipsePoints[1].second) / 2.0f;

        for (int i = 0; i < 360; i += 5) {
            float angle = i * (M_PI / 180.0f);
            float x = xc + rx * cos(angle);
            float y = yc + ry * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
    }
}

void elipse(std::vector<std::vector<std::pair<float, float>>> tempElipsePoints, const float* tempColorArray) {
    glColor3f(tempColorArray[0], tempColorArray[1], tempColorArray[2]);
    glLineWidth(2.0f);
    for (const auto& elipsePoints : tempElipsePoints) {  // Alteração aqui
        glBegin(GL_LINE_LOOP);
        float rx = std::abs(elipsePoints[1].first - elipsePoints[0].first) / 2.0f;
        float ry = std::abs(elipsePoints[1].second - elipsePoints[0].second) / 2.0f;
        float xc = (elipsePoints[0].first + elipsePoints[1].first) / 2.0f;
        float yc = (elipsePoints[0].second + elipsePoints[1].second) / 2.0f;

        for (int i = 0; i < 360; i += 5) {
            float angle = i * (M_PI / 180.0f);
            float x = xc + rx * cos(angle);
            float y = yc + ry * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
    }
    // Adiciona os pontos ao objeto meuDesenho
    meuDesenho.tipo = TIPO_ELIPSE;  // Substitui isso com o tipo correto para a elipse
    meuDesenho.cor[0] = tempColorArray[0];
    meuDesenho.cor[1] = tempColorArray[1];
    meuDesenho.cor[2] = tempColorArray[2];
    meuDesenho.pontos = elipsesPoints[0];  // Utilize apenas os pontos iniciais da elipse

    // Adiciona todas as coordenadas ao vetor todasCoordenadas
    for (const auto& ponto : elipsesPoints[0]) {
        meuDesenho.todasCoordenadas.push_back(ponto);

    }
}

    void baldeTinta(int x, int y, GLfloat fillColor[3], GLubyte targetColor[3]) {
        std::stack<std::pair<int, int>> area;
        area.push(std::make_pair(x, y));

        bool visited[glutGet(GLUT_WINDOW_WIDTH)][glutGet(GLUT_WINDOW_HEIGHT)];
        std::fill(&visited[0][0], &visited[0][0]+sizeof(visited), false);

        while(!area.empty()) {
            std::pair<int,int> current = area.top();
            area.pop();
            x = current.first;
            y = current.second;

            if(x < 0 || x>glutGet(GLUT_WINDOW_WIDTH) || y < 0 || y > glutGet(GLUT_WINDOW_HEIGHT) || visited[x][y]) {
                continue;
            }

            visited[x][y] = true;
            GLubyte pixelColor[3];
            glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT)-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixelColor);

            if(pixelColor[0] == targetColor[0] && pixelColor[1] == targetColor[1] && pixelColor[2] == targetColor[2]) {
                float jPosX = (2.0f * (float)x / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
                float jPosY = 1.0f - (2.0f * (float)y / glutGet(GLUT_WINDOW_HEIGHT));
                paintedPoints.push_back(std::make_pair(jPosX, jPosY));

                glFlush();

                area.push(std::make_pair(x + 1, y));
                area.push(std::make_pair(x - 1, y));
                area.push(std::make_pair(x, y + 1));
                area.push(std::make_pair(x, y - 1));
            }
        }
    }
