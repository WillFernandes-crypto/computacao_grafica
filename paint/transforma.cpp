#include <GL/glut.h>
#include <iostream>
#include <limits>
#include <functional>
#include <vector>
#include <cmath>
#include "transforma.h"
#include "desenhos.h"

bool isSelectionMode = false;
int selectedObjectIndex = -1;

GLfloat modelview[16], projection[16];

float intersectRayWithObject(const Ray& ray, const std::function<void()>& renderFunction) {
    // Defina a posição do objeto
    GLfloat objectPosition[3] = {0.0f, 0.0f, 0.0f};

    // Calcule a distância entre a origem do raio e a origem do objeto
    GLfloat distance = sqrt(pow(ray.origin[0] - objectPosition[0], 2) +
                             pow(ray.origin[1] - objectPosition[1], 2) +
                             pow(ray.origin[2] - objectPosition[2], 2));

    return distance;
}

//Declaração da função raio
Ray createRayFromClick(int x, int y, const GLfloat* projection, const GLfloat* modelview) {
    GLint viewport[4];
    GLdouble modelviewMatrix[16], projectionMatrix[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    // Obtém as matrizes de modelo-visão e projeção
    glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Obter as coordenadas de clique na janela OpenGL
    winX = static_cast<GLfloat>(x);
    winY = static_cast<GLfloat>(viewport[3]) - static_cast<GLfloat>(y);
    glReadPixels(x, static_cast<int>(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    // Desfaz o processo de projeção para obter as coordenadas de clique no espaço 3D
    gluUnProject(winX, winY, winZ, modelviewMatrix, projectionMatrix, viewport, &posX, &posY, &posZ);

    // A origem do raio é a posição da câmera
    GLfloat cameraPos[3] = {0.0f, 0.0f, 0.0f};

    // A direção do raio é a posição do clique menos a posição da câmera
    GLfloat direction[3] = {
        static_cast<GLfloat>(posX - cameraPos[0]),
        static_cast<GLfloat>(posY - cameraPos[1]),
        static_cast<GLfloat>(posZ - cameraPos[2])
    };

    // Normaliza a direção do raio
    GLfloat length = sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);
    direction[0] /= length;
    direction[1] /= length;
    direction[2] /= length;

    return Ray{cameraPos, direction};
}

void selecionarObjeto(int x, int y, const std::vector<std::function<void()>>& functionsToCall) {
    // Obter a matriz de projeção e modelo-visão
    glMatrixMode(GL_PROJECTION);
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glMatrixMode(GL_MODELVIEW);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLubyte pixel[3];
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

    if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255) {
        std::cout << "Nenhum objeto selecionado.\n";
                // Encerrar o modo de seleção
        isSelectionMode = false;
        selectedObjectIndex = -1;
        std::cout << "Modo de selecao encerrado.\n";
        return;
    }

    // Cria um raio a partir da câmera
    Ray ray = createRayFromClick(x, y, projection, modelview);

    // Interseccionar o raio com cada objeto
    float minDistance = std::numeric_limits<float>::max();
    selectedObjectIndex = -1; // Corrigindo para atualizar a variável global
    for (int i = 0; i < functionsToCall.size(); ++i) {
        float distance = intersectRayWithObject(ray, functionsToCall[i]);
        if (distance < minDistance) {
            minDistance = distance;
            selectedObjectIndex = i;
        }
    }

    // Selecionar o objeto mais próximo
    if (selectedObjectIndex != -1) {
        std::cout << "Objeto selecionado: " << selectedObjectIndex << std::endl;
        glutPostRedisplay();
    } else {
        std::cout << "Nenhum objeto selecionado.\n";
    }
}


void excluirObjeto() {
    if (isSelectionMode && selectedObjectIndex != -1) {
        // Excluir o objeto selecionado
        functionsToCall.erase(functionsToCall.begin() + selectedObjectIndex);
        std::cout << "Objeto excluido.\n";

        // Encerrar o modo de seleção
        isSelectionMode = false;
        selectedObjectIndex = -1;
        std::cout << "Modo de selecao encerrado.\n";

        glutPostRedisplay();  // Atualizar a exibição após a exclusão
    } else {
        std::cout << "Nenhum objeto selecionado para exclusao.\n";
    }
}

void calcularLimitesObjeto(const std::function<void()>& renderFunction, GLfloat& minX, GLfloat& minY, GLfloat& maxX, GLfloat& maxY) {
    glPushMatrix();

    // Renderiza o objeto e atualiza os limites no espaço de modelo
    glBegin(GL_POINTS);
    renderFunction();
    glEnd();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLubyte* pixels = new GLubyte[viewport[2] * viewport[3] * 3];
    glReadPixels(viewport[0], viewport[1], viewport[2], viewport[3], GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Obtém as matrizes de modelo-visão e projeção
    GLdouble modelview[16], projection[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    // Correção: Converte as variáveis para GLdouble antes de chamar gluUnProject
    GLdouble winX, winY, winZ;
    gluUnProject(static_cast<GLdouble>(viewport[0]), static_cast<GLdouble>(viewport[1]), 0.0, modelview, projection, viewport, &winX, &winY, &winZ);
    minX = minY = std::numeric_limits<GLfloat>::max();
    maxX = maxY = std::numeric_limits<GLfloat>::min();

    for (int y = 0; y < viewport[3]; ++y) {
        for (int x = 0; x < viewport[2]; ++x) {
            int index = (y * viewport[2] + x) * 3;
            if (pixels[index] != 255 || pixels[index + 1] != 255 || pixels[index + 2] != 255) {
                gluUnProject(static_cast<GLdouble>(x + viewport[0]), static_cast<GLdouble>(y + viewport[1]), 0.0, modelview, projection, viewport, &winX, &winY, &winZ);
                updateLimits(winX, winY, minX, minY, maxX, maxY);
            }
        }
    }

    delete[] pixels;

    glPopMatrix();
}

// Função para atualizar os limites
void updateLimits(GLdouble x, GLdouble y, GLfloat& minX, GLfloat& minY, GLfloat& maxX, GLfloat& maxY) {
    if (x < minX) minX = static_cast<GLfloat>(x);
    if (x > maxX) maxX = static_cast<GLfloat>(x);
    if (y < minY) minY = static_cast<GLfloat>(y);
    if (y > maxY) maxY = static_cast<GLfloat>(y);
}

void transladarObjeto(float translateX, float translateY) {
    if (isSelectionMode && selectedObjectIndex != -1) {
        // Verificar se o objeto já foi rotacionado
        bool objetoRotacionado = false;

        for (const auto& func : functionsToCall) {
            // Simular a execução da função para verificar se ela contém rotação
            glPushMatrix();
            func();
            glPopMatrix();

            // Verificar se a matriz de rotação foi aplicada
            GLfloat matrix[16];
            glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

            if (matrix[1] != 0.0f || matrix[4] != 0.0f) {
                objetoRotacionado = true;
                break;
            }
        }

        if (objetoRotacionado) {
            std::cout << "Não é possível transladar um objeto que foi rotacionado.\n";
        } else {
            // Copiar a função original do objeto selecionado
            auto originalFunction = functionsToCall[selectedObjectIndex];

            // Remover a função original do objeto selecionado
            functionsToCall.erase(functionsToCall.begin() + selectedObjectIndex);

            // Adicionar a função de translação à lista de funções a serem chamadas
            functionsToCall.push_back([translateX, translateY, originalFunction]() {
                glPushMatrix();
                glTranslatef(translateX, translateY, 0.0f);
                originalFunction();  // Renderiza o objeto após a translação
                glPopMatrix();
            });

            std::cout << "Objeto transladado.\n";

            // Encerrar o modo de seleção
            isSelectionMode = false;
            selectedObjectIndex = -1;
            std::cout << "Modo de selecao encerrado.\n";

            glutPostRedisplay();  // Atualizar a exibição após a translação
        }
    } else {
        std::cout << "Nenhum objeto selecionado para translacao.\n";
    }
}

void rotacionaObjeto(float angleDegrees) {
    if (isSelectionMode && selectedObjectIndex != -1) {
        auto originalFunction = functionsToCall[selectedObjectIndex];
        GLfloat minX, minY, maxX, maxY;
        calcularLimitesObjeto(originalFunction, minX, minY, maxX, maxY);

        GLfloat centerX = (minX + maxX) / 2.0f;
        GLfloat centerY = (minY + maxY) / 2.0f;

        // Remover a função original do objeto selecionado
        functionsToCall.erase(functionsToCall.begin() + selectedObjectIndex);

        // Adicionar a função de rotação à lista de funções a serem chamadas
        functionsToCall.push_back([angleDegrees, centerX, centerY, originalFunction]() {
            glPushMatrix();
            glTranslatef(centerX, centerY, 0.0f);
            glRotatef(angleDegrees, 0.0f, 0.0f, 1.0f);
            glTranslatef(-centerX, -centerY, 0.0f);
            originalFunction();
            glPopMatrix();
        });

        std::cout << "Objeto rotacionado." << std::endl;

        // Encerrar o modo de seleção
        isSelectionMode = false;
        selectedObjectIndex = -1;
        std::cout << "Modo de selecao encerrado.\n";

        glutPostRedisplay();
    } else if (!isSelectionMode) {
        std::cout << "Nenhum objeto selecionado para rotação." << std::endl;
    }
}

void escalaObjeto(float scaleX, float scaleY) {
    if (isSelectionMode && selectedObjectIndex != -1) {
        auto originalFunction = functionsToCall[selectedObjectIndex];
        GLfloat minX, minY, maxX, maxY;
        calcularLimitesObjeto(originalFunction, minX, minY, maxX, maxY);

        GLfloat centerX = (minX + maxX) / 2.0f;
        GLfloat centerY = (minY + maxY) / 2.0f;

        // Remover a função original do objeto selecionado
        functionsToCall.erase(functionsToCall.begin() + selectedObjectIndex);

        // Adicionar a função de escala à lista de funções a serem chamadas
        functionsToCall.push_back([scaleX, scaleY, centerX, centerY, originalFunction]() {
            glPushMatrix();
            glTranslatef(centerX, centerY, 0.0f);
            glScalef(scaleX, scaleY, 1.0f);
            glTranslatef(-centerX, -centerY, 0.0f);
            originalFunction();
            glPopMatrix();
        });

        std::cout << "Objeto escalado." << std::endl;

        // Encerrar o modo de seleção
        isSelectionMode = false;
        selectedObjectIndex = -1;
        std::cout << "Modo de selecao encerrado.\n";

        glutPostRedisplay();
    } else if (!isSelectionMode) {
        std::cout << "Nenhum objeto selecionado para escala." << std::endl;
    }
}

void cisalhaObjeto(float shearX, float shearY) {
    if (isSelectionMode && selectedObjectIndex != -1) {
        auto originalFunction = functionsToCall[selectedObjectIndex];

        // Remover a função original do objeto selecionado
        functionsToCall.erase(functionsToCall.begin() + selectedObjectIndex);

        // Adicionar a função de cisalhamento à lista de funções a serem chamadas
        functionsToCall.push_back([shearX, shearY, originalFunction]() {
            glPushMatrix();
            GLfloat matrix[16];
            glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

            // Aplicar cisalhamento à matriz de transformação
            matrix[4] += shearX;
            matrix[1] += shearY;
            glMultMatrixf(matrix);

            originalFunction();  // Renderiza o objeto após o cisalhamento
            glPopMatrix();
        });

        std::cout << "Objeto cisalhado.\n";

        glutPostRedisplay();  // Atualiza a exibição após o cisalhamento
    } else {
        std::cout << "Nenhum objeto selecionado para cisalhamento.\n";
    }
}

void refleteObjeto(bool refletirX, bool refletirY) {
    if (isSelectionMode && selectedObjectIndex != -1) {
        auto originalFunction = functionsToCall[selectedObjectIndex];

        // Remover a função original do objeto selecionado
        functionsToCall.erase(functionsToCall.begin() + selectedObjectIndex);

        // Adicionar a função de reflexão à lista de funções a serem chamadas
        functionsToCall.push_back([refletirX, refletirY, originalFunction]() {
            glPushMatrix();
            if (refletirX) {
                glScalef(-1.0f, 1.0f, 1.0f);  // Reflete em relação ao eixo X
            }
            if (refletirY) {
                glScalef(1.0f, -1.0f, 1.0f);  // Reflete em relação ao eixo Y
            }
            originalFunction();
            glPopMatrix();
        });

        std::cout << "Objeto refletido." << std::endl;

        // Encerrar o modo de seleção
        isSelectionMode = false;
        selectedObjectIndex = -1;
        std::cout << "Modo de selecao encerrado.\n";

        glutPostRedisplay();
    } else if (!isSelectionMode) {
        std::cout << "Nenhum objeto selecionado para reflexão." << std::endl;
    }
}
