#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <functional>
#include <stack>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>
#include <cstring>
#include "transforma.h"
#include "desenhos.h"
#include "perif.h"
#include "salvar.h"

std::string nomeArquivoSalvar;
std::string nomeArquivoCarregar;
std::vector<Desenho> desenhosParaSalvar;
std::vector<Desenho> desenhosCarregados;


void setBool(bool borracha, bool pont, bool caneta, bool linha, bool tinta, bool rect, bool elipse, bool triangle) {
    eraserTool = borracha;
    pointTool = pont;
    pencilTool = caneta;
    lineTool = linha;
    paintTool = tinta;
    rectTool = rect;
    elipseTool = elipse;
    triangleTool = triangle;
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {

    //Ativa ferramenta de apagar
    case 'E':
    case 'e':
        setBool(true, false, false, false, false, false, false, false);
        break;
    //Ativa ferramenta de ponto
    case 'O':
    case 'o':
        setBool(false, true, false, false, false, false, false, false);
        break;
    //Ativa ferramenta de caneta
    case 'P':
    case 'p':
        setBool(false, false, true, false, false, false, false, false);
        break;
    //Ativa ferramenta de linha
    case 'L':
    case 'l':
        setBool(false, false, false, true, false, false, false, false);
        break;
    //Ativa ferramenta de balde de tinta
    case 'F':
    case 'f':
        setBool(false, false, false, false, true, false, false, false);
        break;
    //Ativa ferramenta de desenhar retângulo
    case 'Q':
    case 'q':
        setBool(false, false, false, false, false, true, false, false);
        break;
    //Ativa ferramenta de desenhar circulo
    case 'C':
    case 'c':
        setBool(false, false, false, false, false, false, true, false);
        break;
    //Ativa ferramenta de desenhar triangulo
    case 'T':
    case 't':
        setBool(false, false, false, false, false, false, false, true);
        break;

    //Transformações
    case 'X':
    case 'x':
        excluirObjeto();
        break;
    case 'M':
    case 'm':
        transladarObjeto(0.0f, 0.1f);
        break;
    case 'R':
    case 'r':
        rotacionaObjeto(10.0f);
        break;
    case 'K':
    case 'k':
        escalaObjeto(1.1f, 1.1f);
        break;
    case 'N':
    case 'n':
        cisalhaObjeto(0.1f, 0.0f);
        break;
    case 'A':
    case 'a':
        refleteObjeto(true, false);
        break;
    //Desfazer ação
    case 'Z':
    case 'z':
        if (!functionsToCall.empty()) {
            functionsToCall.pop_back();
        }
        glutPostRedisplay();
        break;

    //Seleção das cores
    case '0':
        currColorArray[0] = 0.0f;
        currColorArray[1] = 0.0f;
        currColorArray[2] = 0.0f;
        break;
    case '1':
        currColorArray[0] = 1.0f;
        currColorArray[1] = 0.0f;
        currColorArray[2] = 0.0f;
        break;
    case '2':
        currColorArray[0] = 0.0f;
        currColorArray[1] = 1.0f;
        currColorArray[2] = 0.0f;
        break;
    case '3':
        currColorArray[0] = 0.0f;
        currColorArray[1] = 0.0f;
        currColorArray[2] = 1.0f;
        break;
    case '4':
        currColorArray[0] = 1.0f;
        currColorArray[1] = 1.0f;
        currColorArray[2] = 0.0f;
        break;
    case '5':
        currColorArray[0] = 1.0f;
        currColorArray[1] = 0.0f;
        currColorArray[2] = 1.0f;
        break;
    case '6':
        currColorArray[0] = 0.0f;
        currColorArray[1] = 1.0f;
        currColorArray[2] = 1.0f;
        break;

    //Salvar e carregar desenhos
    case 'I':
    case 'i':
        std::cout << "Digite o nome do arquivo para carregar: ";
        std::cin >> nomeArquivoCarregar;
        carregarDesenho(nomeArquivoCarregar);  // Supõe que o arquivo possui extensão .txt
        glutPostRedisplay();
        break;
    case 'S':
    case 's':
        // Chamar a função de salvar desenhos
        std::cout << "Digite o nome do arquivo para salvar: ";
        std::cin >> nomeArquivoSalvar;

        // Criar um vetor de desenhos e adicionar o desenho atual (meuDesenho) a ele
        std::vector<Desenho> desenhosParaSalvar;
        desenhosParaSalvar.push_back(meuDesenho);

        salvarArquivo(nomeArquivoSalvar.c_str(), desenhosParaSalvar);
        std::cout << "Desenhos salvos com sucesso no arquivo: " << nomeArquivoSalvar << ".txt\n";
        break;
    }
}

void mouseClick(int button, int state, int x, int y) {
    float posX = (2.0f * static_cast<float>(x) / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    float posY = 1.0f - (2.0f * static_cast<float>(y) / glutGet(GLUT_WINDOW_HEIGHT));

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (pencilTool) {
                pontos.push_back(std::make_pair(posX, posY));
                isDraw = true;
            }
            if (pointTool) {
                pontos.push_back(std::make_pair(posX, posY));
                isDrawPoint = true;
            }
            if(triangleTool) {
                trianglePoints.push_back(std::make_pair(posX, posY));
                trianglePoints.push_back(std::make_pair(posX, posY));
                trianglePoints.push_back(std::make_pair(posX, posY));
                isDrawTriangle = true;
            }
            if (eraserTool) {
                apagaCurvas.push_back(std::make_pair(posX, posY));
                isErase = true;
            }
            if(lineTool) {
                lineCurvas[0] = std :: make_pair(posX, posY);
                lineCurvas[1] = std :: make_pair(posX, posY);
                isLine = true;
            }
            if(rectTool) {
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                isDrawRect = true;
            }
            if (elipseTool) {
                elipsesPoints.push_back(std::vector<std::pair<float, float>>());
                elipsesPoints.back().push_back(std::make_pair(posX, posY));
                elipsesPoints.back().push_back(std::make_pair(posX, posY));
                isDrawElipse = true;
            }
            if(paintTool) {
                glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT)-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, targetColorArray);
                baldeTinta(x, y, currColorArray, targetColorArray);
                //Código para função de vetores.
                std::vector<std::pair<float, float>> tempPaintingPoints = paintedPoints;
                float tempPaintColorArray[3];
                for (int i = 0; i< 3; ++i) {
                    tempPaintColorArray[i] = currColorArray[i];
                }
                functionsToCall.push_back([tempPaintingPoints, tempPaintColorArray]() {
                    float colorPaintCopy[3] = {tempPaintColorArray[0], tempPaintColorArray[1], tempPaintColorArray[2]};
                    draw(tempPaintingPoints, colorPaintCopy);
                });
                paintedPoints.clear();

            }
        } else if (state == GLUT_UP) {
            if (pencilTool) {
                isDraw = false;
                std::vector<std::pair<float, float>> tempPoints = pontos;
                float tempCorArray[3];
                for (int i = 0; i< 3; ++i) {
                    tempCorArray[i] = currColorArray[i];
                }
                functionsToCall.push_back([tempPoints, tempCorArray]() {
                    float colorCopy[3] = {tempCorArray[0], tempCorArray[1], tempCorArray[2]};
                    draw(tempPoints, colorCopy);
                });
                pontos.clear();
            }

            if (triangleTool) {
                trianglePoints[1].first = trianglePoints[0].first + (scale * posX);
                trianglePoints[1].second = trianglePoints[0].second;
                trianglePoints[2].first = trianglePoints[0].first + (scale * posX / 2.0f);
                trianglePoints[2].second = trianglePoints[0].second + (scale * posY / 2.0f);

                isDrawTriangle = false;
                std::vector<std::pair<float, float>> tempTrianglePoints = trianglePoints;
                float tempColorArr[3];
                for (int i = 0; i < 3; ++i) {
                    tempColorArr[i] = currColorArray[i];
                }
                functionsToCall.push_back([tempTrianglePoints, tempColorArr]() {
                    float coresCopy[3] = {tempColorArr[0], tempColorArr[1], tempColorArr[2]};
                    triangle(tempTrianglePoints, coresCopy);
                });

                trianglePoints.clear();
            }
            if (pointTool) {
                isDrawPoint = false;
                std::vector<std::pair<float, float>> tempPont = pontos;
                float tempCorArray[3];
                for (int i = 0; i < 3; ++i) {
                    tempCorArray[i] = currColorArray[i];
                }
                functionsToCall.push_back([tempPont, tempCorArray]() {
                    float colorCopy[3] = {tempCorArray[0], tempCorArray[1], tempCorArray[2]};
                    drawPoint(tempPont, colorCopy);
                });
                pontos.clear();
            }
            if (lineTool) {
                isLine = false;
                std::vector<std::pair<float, float>> tempLinePoints = lineCurvas;
                float tempCorArray[3];
                for (int i = 0; i< 3; ++i) {
                    tempCorArray[i] = currColorArray[i];
                }
                functionsToCall.push_back([tempLinePoints, tempCorArray]() {
                    float corCopy[3] = {tempCorArray[0], tempCorArray[1], tempCorArray[2]};
                    linha(tempLinePoints, corCopy);
                });
                lineCurvas = linhaCurvas;
            }
            if (rectTool) {

                rectPoints[1].first = rectPoints[0].first;
                rectPoints[1].second = posY;
                rectPoints[2].first = posX;
                rectPoints[2].second = posY;
                rectPoints[3].first = posX;
                rectPoints[3].second = rectPoints[0].second;
                rectPoints[4].first = rectPoints[0].first;
                rectPoints[4].second = rectPoints[0].second;

                isDrawRect = false;
                std::vector<std::pair<float, float>> tempRectanPoints = rectPoints;
                float tempColorArr[3];
                for (int i = 0; i< 3; ++i) {
                    tempColorArr[i] = currColorArray[i];
                }
                functionsToCall.push_back([tempRectanPoints, tempColorArr]() {
                    float coresCopy[3] = {tempColorArr[0], tempColorArr[1], tempColorArr[2]};
                    linha(tempRectanPoints, coresCopy);
                });
                rectPoints.clear();
            }
            if (elipseTool) {

                isDrawElipse = false;

                std::vector<std::vector<std::pair<float, float>>> tempLineElipses = elipsesPoints;

                float tempColorArr[3];
                for (int i = 0; i< 3; ++i) {
                    tempColorArr[i] = currColorArray[i];
                }
                functionsToCall.push_back([tempLineElipses, tempColorArr]() {
                    float coresCopy[3] = {tempColorArr[0], tempColorArr[1], tempColorArr[2]};
                    elipse(tempLineElipses, coresCopy);
                });
                elipsesPoints.clear();
            }
            if (eraserTool) {
                isErase = false;
                std::vector<std::pair<float, float>> tempErasePoints = apagaCurvas;
                functionsToCall.push_back([tempErasePoints]() {
                    eraser(tempErasePoints);
                });
                apagaCurvas.clear();

                // Aqui é onde você pode adicionar chamadas para funções relacionadas à exclusão do objeto
                if (selectedObjectIndex != -1) {
                    excluirObjeto();
                } else{

                }
            }
            glutPostRedisplay();
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (!isSelectionMode) {
            // Iniciar a seleção de objetos
            isSelectionMode = true;
            selectedObjectIndex = -1;  // Nenhum objeto selecionado inicialmente
            std::cout << "Modo de selecao ativado.\n";

            // Aqui, adicionamos uma chamada para a função de seleção ao clicar com o botão direito
            selecionarObjeto(x, y, functionsToCall);
        } else {
            // Se já estiver em modo de seleção e o botão direito for pressionado novamente
            isSelectionMode = false;
            selectedObjectIndex = -1;
            std::cout << "Modo de selecao desativado.\n";
            glutPostRedisplay();
        }
    }


}


void mouseMover(int x, int y) {

    float currX = (2.0f * (float)x / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    float currY = 1.0f - (2.0f * (float)y / glutGet(GLUT_WINDOW_HEIGHT));
    if (pencilTool && isDraw) {
        pontos.push_back(std::make_pair(currX, currY));
    }
    if (triangleTool && isDrawTriangle) {
        trianglePoints[1].first = trianglePoints[0].first + (scale * currX);
        trianglePoints[1].second = trianglePoints[0].second;
        trianglePoints[2].first = trianglePoints[0].first + (scale * currX / 2.0f);
        trianglePoints[2].second = trianglePoints[0].second + (scale * currY / 2.0f);
    }
    if (eraserTool && isErase) {
        apagaCurvas.push_back(std::make_pair(currX, currY));
    }
    if (lineTool && isLine) {
        lineCurvas[1] = std::make_pair(currX, currY);
    }
    if(rectTool && isDrawRect) {
        rectPoints[1].first = rectPoints[0].first;
        rectPoints[1].second = currY;
        rectPoints[2].first = currX;
        rectPoints[2].second = currY;
        rectPoints[3].first = currX;
        rectPoints[3].second = rectPoints[0].second;
        rectPoints[4].first = rectPoints[0].first;
        rectPoints[4].second = rectPoints[0].second;
    }
    if (elipseTool && isDrawElipse) {
        elipsesPoints.back()[1] = std::make_pair(currX, currY);
    }
    glutPostRedisplay();

}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 0.0f);

    // Exibe os desenhos carregados
    for (const auto& desenho : desenhosCarregados) {
        switch (desenho.tipo) {
        case 0:
            draw(desenho.todasCoordenadas, desenho.cor);
            break;
        case 1:
            linha(desenho.todasCoordenadas, desenho.cor);
            break;
        case 2:
            retangulo(desenho.todasCoordenadas, desenho.cor);
            break;
        case 3:
            triangle(desenho.todasCoordenadas, desenho.cor);
            break;
        case 4:
            elipse({desenho.todasCoordenadas}, desenho.cor);
            break;
        default:
            std::cerr << "Tipo de desenho inválido: " << desenho.tipo << std::endl;
        }
    }

    for(const auto& func : functionsToCall) {
        func();
    }
    drawPointRT();
    drawRT();
    linhaRT();
    retanguloRT();
    elipseRT();
    eraseRT();
    triangleRT();


    glutSwapBuffers();

}
