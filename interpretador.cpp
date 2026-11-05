#include "interpretador.h"
#include "sculptor.h"

#include "putvoxel.h"
#include "cutvoxel.h"
#include "putbox.h"
#include "cutbox.h"
#include "putsphere.h"
#include "cutsphere.h"
#include "putellipsoid.h"
#include "cutellipsoid.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace {

std::string minusculo(std::string texto) {
    std::transform(texto.begin(), texto.end(), texto.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return texto;
}

std::unique_ptr<FiguraGeometrica> criarFigura(const std::string &comando,
                                              std::istringstream &entrada) {
    if (comando == "putvoxel") {
        int x, y, z;
        float r, g, b, a;
        if (entrada >> x >> y >> z >> r >> g >> b >> a)
            return std::make_unique<PutVoxel>(x, y, z, r, g, b, a);
    }
    else if (comando == "cutvoxel") {
        int x, y, z;
        if (entrada >> x >> y >> z)
            return std::make_unique<CutVoxel>(x, y, z);
    }
    else if (comando == "putbox") {
        int x0, x1, y0, y1, z0, z1;
        float r, g, b, a;
        if (entrada >> x0 >> x1 >> y0 >> y1 >> z0 >> z1
                    >> r >> g >> b >> a)
            return std::make_unique<PutBox>(
                x0, x1, y0, y1, z0, z1, r, g, b, a);
    }
    else if (comando == "cutbox") {
        int x0, x1, y0, y1, z0, z1;
        if (entrada >> x0 >> x1 >> y0 >> y1 >> z0 >> z1)
            return std::make_unique<CutBox>(x0, x1, y0, y1, z0, z1);
    }
    else if (comando == "putsphere") {
        int x, y, z, raio;
        float r, g, b, a;
        if (entrada >> x >> y >> z >> raio >> r >> g >> b >> a)
            return std::make_unique<PutSphere>(x, y, z, raio, r, g, b, a);
    }
    else if (comando == "cutsphere") {
        int x, y, z, raio;
        if (entrada >> x >> y >> z >> raio)
            return std::make_unique<CutSphere>(x, y, z, raio);
    }
    else if (comando == "putellipsoid") {
        int x, y, z, rx, ry, rz;
        float r, g, b, a;
        if (entrada >> x >> y >> z >> rx >> ry >> rz >> r >> g >> b >> a)
            return std::make_unique<PutEllipsoid>(
                x, y, z, rx, ry, rz, r, g, b, a);
    }
    else if (comando == "cutellipsoid") {
        int x, y, z, rx, ry, rz;
        if (entrada >> x >> y >> z >> rx >> ry >> rz)
            return std::make_unique<CutEllipsoid>(x, y, z, rx, ry, rz);
    }
    return nullptr;
}

bool comandoConhecido(const std::string &comando) {
    static const char *comandos[] = {
        "putvoxel", "cutvoxel", "putbox", "cutbox",
        "putsphere", "cutsphere", "putellipsoid", "cutellipsoid"
    };
    for (const char *c : comandos)
        if (comando == c) return true;
    return false;
}

}

bool lerArquivoDeDesenho(const std::string &nomeArquivo,
                         ArquivoDeDesenho &saida,
                         std::string &erro) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        erro = "Não foi possível abrir o arquivo: " + nomeArquivo;
        return false;
    }

    saida = ArquivoDeDesenho{};
    bool leuDim = false;
    std::string linha;
    int numeroLinha = 0;

    while (std::getline(arquivo, linha)) {
        numeroLinha++;

        std::istringstream entrada(linha);
        std::string comando;

        // Ignora linhas vazias e comentários
        if (!(entrada >> comando) || comando[0] == '#')
            continue;

        comando = minusculo(comando);
        const std::string onde = " (linha " + std::to_string(numeroLinha) + ")";

        if (comando == "dim") {
            if (leuDim) {
                erro = "Instrução dim repetida" + onde;
                return false;
            }
            if (!(entrada >> saida.nx >> saida.ny >> saida.nz) ||
                saida.nx <= 0 || saida.ny <= 0 || saida.nz <= 0) {
                erro = "Dimensões inválidas" + onde;
                return false;
            }
            leuDim = true;
            continue;
        }

        if (!comandoConhecido(comando)) {
            erro = "Comando desconhecido '" + comando + "'" + onde;
            return false;
        }

        if (!leuDim) {
            erro = "A primeira instrução do arquivo deve ser 'dim nx ny nz'";
            return false;
        }

        auto figura = criarFigura(comando, entrada);
        if (!figura) {
            erro = "Parâmetros inválidos para '" + comando + "'" + onde;
            return false;
        }

        saida.figuras.push_back(std::move(figura));
    }

    if (!leuDim) {
        erro = "O arquivo não contém a instrução 'dim nx ny nz'";
        return false;
    }

    return true;
}

Sculptor *criarEsculturaDeArquivo(const std::string &nomeArquivo,
                                  std::string &erro) {
    ArquivoDeDesenho desenho;
    if (!lerArquivoDeDesenho(nomeArquivo, desenho, erro))
        return nullptr;

    Sculptor *escultor = new Sculptor(desenho.nx, desenho.ny, desenho.nz);

    for (const auto &figura : desenho.figuras)
        figura->draw(*escultor);

    return escultor;
}
