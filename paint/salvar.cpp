#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include "transforma.h"
#include "desenhos.h"
#include "perif.h"
#include "salvar.h"

extern Desenho meuDesenho;

void salvarDesenho(const char *nomeArquivo, const Desenho *desenho) {
    std::ofstream arquivo(nomeArquivo, std::ios::app);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return;
    }

    arquivo << "Tipo: " << desenho->tipo << std::endl;
    arquivo << "Cor: " << desenho->cor[0] << " " << desenho->cor[1] << " " << desenho->cor[2] << std::endl;

    // Adicionando todas as coordenadas do desenho
    for (const auto& ponto : desenho->todasCoordenadas) {
        arquivo << ponto.first << " " << ponto.second << std::endl;
    }

    arquivo << std::endl;

    arquivo.close();
}


void salvarArquivo(const char *nomeArquivo, const std::vector<Desenho>& desenhos) {
    std::ofstream arquivo(nomeArquivo, std::ios::app);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return;
    }

    for (const Desenho &desenho : desenhos) {
        salvarDesenho(nomeArquivo, &desenho);
    }

    arquivo.close();
}

void carregarDesenho(const std::string& filename) {
    std::ifstream file(filename.c_str());

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
        return;
    }

    std::vector<Desenho> desenhosCarregados;
    std::string line;

    if (!std::getline(file, line)) {
        std::cerr << "Arquivo vazio: " << filename << std::endl;
        file.close();
        return;
    }

    std::cout << "Lendo arquivo: " << filename << std::endl;

    int tipoDesenho;
    float r, g, b;

    while (file >> tipoDesenho >> r >> g >> b) {
        std::vector<std::pair<float, float>> coordenadas;
        float x, y;
        while (file >> x >> y) {
            coordenadas.push_back({x, y});
        }

        Desenho desenho(tipoDesenho, new float[3]{r, g, b}, coordenadas);
        desenhosCarregados.push_back(desenho);
    }

    file.close();

    std::cout << "Leitura do arquivo concluída." << std::endl;

    for (const Desenho& desenho : desenhosCarregados) {
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
    desenhosCarregados.insert(desenhosCarregados.end(), desenhosCarregados.begin(), desenhosCarregados.end());
    glutPostRedisplay();
}

