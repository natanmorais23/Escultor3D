#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include "figurageometrica.h"

#include <memory>
#include <string>
#include <vector>

struct ArquivoDeDesenho {
    int nx = 0, ny = 0, nz = 0;
    std::vector<std::unique_ptr<FiguraGeometrica>> figuras;
};

bool lerArquivoDeDesenho(const std::string &nomeArquivo,
                         ArquivoDeDesenho &saida,
                         std::string &erro);

Sculptor *criarEsculturaDeArquivo(const std::string &nomeArquivo,
                                  std::string &erro);

#endif
