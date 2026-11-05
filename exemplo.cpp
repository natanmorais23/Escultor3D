#include "sculptor.h"
#include "figurageometrica.h"
#include "putvoxel.h"
#include "cutvoxel.h"
#include "putbox.h"
#include "cutbox.h"
#include "putsphere.h"
#include "cutsphere.h"
#include "putellipsoid.h"
#include "cutellipsoid.h"
#include "interpretador.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#ifndef ESCULTOR_EXEMPLOS
#define ESCULTOR_EXEMPLOS "exemplos"
#endif

namespace {

int falhas = 0;

void verificar(bool condicao, const std::string &descricao) {
    std::cout << (condicao ? "  [OK]    " : "  [FALHA] ") << descricao << '\n';
    if (!condicao) falhas++;
}

int contarAtivos(const Sculptor &s) {
    int total = 0;
    for (int x = 0; x < s.getNx(); x++)
        for (int y = 0; y < s.getNy(); y++)
            for (int z = 0; z < s.getNz(); z++)
                if (s.getVoxel(x, y, z).show) total++;
    return total;
}

void testarSculptor() {
    std::cout << "\n== Etapa 1: metodos da classe Sculptor ==\n";

    Sculptor s(20, 20, 20);
    verificar(contarAtivos(s) == 0, "matriz alocada comeca vazia");

    s.setColor(1, 0, 0, 1);
    s.putVoxel(0, 0, 0);
    verificar(s.getVoxel(0, 0, 0).show, "putVoxel ativa o voxel");
    verificar(s.getVoxel(0, 0, 0).r == 1.0f, "setColor define a cor do voxel");
    s.cutVoxel(0, 0, 0);
    verificar(!s.getVoxel(0, 0, 0).show, "cutVoxel desativa o voxel");

    s.putVoxel(-1, 50, 3);
    verificar(contarAtivos(s) == 0, "coordenadas fora da matriz sao ignoradas");

    s.setColor(0.6f, 0.4f, 0.2f, 1);
    s.putBox(2, 17, 2, 17, 0, 3);
    verificar(contarAtivos(s) == 16 * 16 * 4, "putBox ativa 16x16x4 voxels");
    s.cutBox(6, 13, 6, 13, 0, 3);
    verificar(contarAtivos(s) == 16 * 16 * 4 - 8 * 8 * 4, "cutBox remove 8x8x4 voxels");

    s.setColor(0, 0.4f, 1, 1);
    s.putSphere(10, 10, 10, 4);
    verificar(s.getVoxel(10, 10, 10).show, "putSphere preenche o centro");
    s.cutSphere(10, 10, 14, 2);
    verificar(!s.getVoxel(10, 10, 14).show, "cutSphere remove o topo da esfera");

    s.setColor(0, 0.8f, 0.2f, 1);
    s.putEllipsoid(10, 10, 17, 6, 2, 2);
    verificar(s.getVoxel(15, 10, 17).show, "putEllipsoid alcanca o semieixo x");
    verificar(!s.getVoxel(10, 14, 17).show, "putEllipsoid respeita o semieixo y");
    s.cutEllipsoid(10, 10, 17, 2, 1, 1);
    verificar(!s.getVoxel(10, 10, 17).show, "cutEllipsoid remove o centro");

    verificar(s.writeOFF("exemplo_sculptor.off"), "writeOFF grava exemplo_sculptor.off");
}

void testarFiguras() {
    std::cout << "\n== Etapa 2: classes derivadas de FiguraGeometrica ==\n";

    Sculptor s(20, 20, 20);

    std::vector<std::unique_ptr<FiguraGeometrica>> figuras;
    figuras.push_back(std::make_unique<PutBox>(0, 19, 0, 19, 0, 1, 0.5f, 0.5f, 0.5f, 1));
    figuras.push_back(std::make_unique<CutBox>(8, 11, 8, 11, 0, 1));
    figuras.push_back(std::make_unique<PutSphere>(10, 10, 8, 5, 1, 0.8f, 0, 1));
    figuras.push_back(std::make_unique<CutSphere>(10, 10, 13, 2));
    figuras.push_back(std::make_unique<PutEllipsoid>(10, 10, 16, 7, 3, 2, 0.2f, 0.6f, 1, 1));
    figuras.push_back(std::make_unique<CutEllipsoid>(10, 10, 16, 3, 1, 1));
    figuras.push_back(std::make_unique<PutVoxel>(0, 0, 2, 1, 0, 0, 1));
    figuras.push_back(std::make_unique<CutVoxel>(19, 19, 1));

    for (const auto &f : figuras)
        f->draw(s);

    verificar(!s.getVoxel(9, 9, 0).show, "CutBox abriu o furo na base");
    verificar(s.getVoxel(10, 10, 6).show, "PutSphere desenhou a esfera");
    verificar(!s.getVoxel(10, 10, 13).show, "CutSphere removeu o topo");
    verificar(s.getVoxel(16, 10, 16).show, "PutEllipsoid desenhou o elipsoide");
    verificar(!s.getVoxel(10, 10, 16).show, "CutEllipsoid removeu o centro");
    verificar(s.getVoxel(0, 0, 2).show, "PutVoxel ativou o voxel");
    verificar(!s.getVoxel(19, 19, 1).show, "CutVoxel desativou o voxel");

    verificar(s.writeOFF("exemplo_figuras.off"), "writeOFF grava exemplo_figuras.off");
}

void testarArquivo(const std::string &caminho) {
    std::cout << "\n== Etapa 3: arquivo de desenho ==\n";
    std::cout << "  Lendo " << caminho << '\n';

    std::string erro;
    std::unique_ptr<Sculptor> s(criarEsculturaDeArquivo(caminho, erro));

    verificar(s != nullptr, "arquivo lido sem erros");
    if (!s) {
        std::cout << "  Erro: " << erro << '\n';
        return;
    }

    std::cout << "  Dimensoes: " << s->getNx() << " x " << s->getNy()
              << " x " << s->getNz() << ", voxels ativos: "
              << contarAtivos(*s) << '\n';

    verificar(s->writeOFF("exemplo_arquivo.off"), "writeOFF grava exemplo_arquivo.off");
}

}

int main(int argc, char *argv[]) {
    testarSculptor();
    testarFiguras();

    std::string caminho = argc > 1
        ? argv[1]
        : std::string(ESCULTOR_EXEMPLOS) + "/boneco_de_neve.txt";
    testarArquivo(caminho);

    std::cout << '\n' << (falhas == 0 ? "Todos os testes passaram."
                                      : "Alguns testes falharam.")
              << "\nAbra os arquivos .off gerados no MeshLab para visualizar.\n";

    return falhas == 0 ? 0 : 1;
}
