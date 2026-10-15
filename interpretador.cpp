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

#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <iostream>

bool interpretarArquivo(const std::string &nomeArquivo,
                        Sculptor &escultor) {
    std::ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir: " << nomeArquivo << '\n';
        return false;
    }

    std::string linha;
    int numeroLinha = 0;

    while (std::getline(arquivo, linha)) {
        numeroLinha++;

        // Ignora linhas vazias e comentários
        if (linha.empty() || linha[0] == '#')
            continue;

        std::istringstream entrada(linha);
        std::string comando;
        entrada >> comando;

        std::unique_ptr<FiguraGeometrica> figura;

        if (comando == "putvoxel") {
            int x, y, z;
            float r, g, b, a;
            if (entrada >> x >> y >> z >> r >> g >> b >> a)
                figura = std::make_unique<PutVoxel>(x, y, z, r, g, b, a);
        }
        else if (comando == "cutvoxel") {
            int x, y, z;
            if (entrada >> x >> y >> z)
                figura = std::make_unique<CutVoxel>(x, y, z);
        }
        else if (comando == "putbox") {
            int x0, x1, y0, y1, z0, z1;
            float r, g, b, a;
            if (entrada >> x0 >> x1 >> y0 >> y1 >> z0 >> z1
                >> r >> g >> b >> a)
                figura = std::make_unique<PutBox>(
                    x0, x1, y0, y1, z0, z1, r, g, b, a);
        }
        else if (comando == "cutbox") {
            int x0, x1, y0, y1, z0, z1;
            if (entrada >> x0 >> x1 >> y0 >> y1 >> z0 >> z1)
                figura = std::make_unique<CutBox>(
                    x0, x1, y0, y1, z0, z1);
        }
        else if (comando == "putsphere") {
            int x, y, z, raio;
            float r, g, b, a;
            if (entrada >> x >> y >> z >> raio >> r >> g >> b >> a)
                figura = std::make_unique<PutSphere>(
                    x, y, z, raio, r, g, b, a);
        }
        else if (comando == "cutsphere") {
            int x, y, z, raio;
            if (entrada >> x >> y >> z >> raio)
                figura = std::make_unique<CutSphere>(x, y, z, raio);
        }
        else if (comando == "putellipsoid") {
            int x, y, z, rx, ry, rz;
            float r, g, b, a;
            if (entrada >> x >> y >> z >> rx >> ry >> rz
                >> r >> g >> b >> a)
                figura = std::make_unique<PutEllipsoid>(
                    x, y, z, rx, ry, rz, r, g, b, a);
        }
        else if (comando == "cutellipsoid") {
            int x, y, z, rx, ry, rz;
            if (entrada >> x >> y >> z >> rx >> ry >> rz)
                figura = std::make_unique<CutEllipsoid>(
                    x, y, z, rx, ry, rz);
        }
        else {
            std::cerr << "Comando desconhecido na linha "
                      << numeroLinha << ": " << comando << '\n';
            return false;
        }

        if (!figura) {
            std::cerr << "Parametros invalidos na linha "
                      << numeroLinha << '\n';
            return false;
        }

        figura->draw(escultor);
    }

    return true;
}
