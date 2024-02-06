#ifndef SALVAR_H
#define SALVAR_H

#include <iostream>
#include <vector>
#include <fstream>
#include "desenhos.h"
#include "transforma.h"
#include "perif.h"

struct Desenho {
    int tipo;
    float cor[3];
    std::vector<std::pair<float, float>> pontos;
    // Adicione um vetor para armazenar todas as coordenadas do desenho
    std::vector<std::pair<float, float>> todasCoordenadas;

    // Construtor padrão
    Desenho() : tipo(0), cor{0.0f, 0.0f, 0.0f} {}  // Pode ajustar os valores padrão conforme necessário

    // Construtor para inicializar o objeto Desenho
    Desenho(int tipoDesenho, float cor[], const std::vector<std::pair<float, float>>& coordenadas)
        : tipo(tipoDesenho), cor{cor[0], cor[1], cor[2]}, todasCoordenadas{coordenadas} {}

};


struct Arquivo {
    char nome[256];
    std::vector<Desenho> desenhos;
};

void salvarDesenho(const char *nomeArquivo, const Desenho *desenho);
void salvarArquivo(const char *nomeArquivo, const std::vector<Desenho>& desenhos);
void carregarDesenho(const std::string& filename);

#endif
