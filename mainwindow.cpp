#include "mainwindow.h"
#include "sculptorview.h"
#include "sculptor.h"
#include <algorithm>
#include <cmath>

#include <QColorDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    escultor(new Sculptor(30, 30, 30)),
    visualizacao(nullptr),
    ferramenta(nullptr),
    posX(nullptr),
    posY(nullptr),
    posZ(nullptr),
    dimX(nullptr),
    dimY(nullptr),
    dimZ(nullptr),
    tamanhoX(nullptr),
    tamanhoY(nullptr),
    tamanhoZ(nullptr),
    corAtual(Qt::red)
{
    setWindowTitle("Escultor 3D");
    resize(1100, 700);

    configurarInterface();
    carregarExemplo();
}

MainWindow::~MainWindow()
{
    delete escultor;
}

void MainWindow::configurarInterface()
{
    QWidget *central = new QWidget(this);
    auto *layoutPrincipal = new QHBoxLayout(central);

    // Visualização 3D
    visualizacao = new SculptorView(escultor, central);
    layoutPrincipal->addWidget(visualizacao, 1);

    // Painel lateral
    QWidget *painel = new QWidget(central);
    painel->setMinimumWidth(250);
    painel->setMaximumWidth(300);

    auto *layoutPainel = new QVBoxLayout(painel);

    auto *titulo = new QLabel("Ferramentas", painel);
    titulo->setStyleSheet(
        "font-size: 20px; font-weight: bold; padding: 5px;"
        );
    layoutPainel->addWidget(titulo);

    // Dimensões da matriz
    layoutPainel->addWidget(
        new QLabel("Dimensões da matriz", painel)
        );

    tamanhoX = new QSpinBox(painel);
    tamanhoY = new QSpinBox(painel);
    tamanhoZ = new QSpinBox(painel);

    for (QSpinBox *campo : {tamanhoX, tamanhoY, tamanhoZ}) {
        campo->setRange(1, 100);
        campo->setValue(30);
    }

    auto *formDimensoes = new QFormLayout;
    formDimensoes->addRow("Tamanho X:", tamanhoX);
    formDimensoes->addRow("Tamanho Y:", tamanhoY);
    formDimensoes->addRow("Tamanho Z:", tamanhoZ);
    layoutPainel->addLayout(formDimensoes);

    auto *botaoCriar = new QPushButton("Criar matriz", painel);
    connect(botaoCriar, &QPushButton::clicked,
            this, &MainWindow::criarEscultura);
    layoutPainel->addWidget(botaoCriar);

    // Ferramentas disponíveis
    ferramenta = new QComboBox(painel);
    ferramenta->addItems({
        "Voxel",
        "Caixa",
        "Esfera",
        "Elipsoide",
        "Apagar voxel",
        "Apagar caixa",
        "Apagar esfera",
        "Apagar elipsoide"
    });

    layoutPainel->addWidget(new QLabel("Forma:", painel));
    layoutPainel->addWidget(ferramenta);

    // Posição
    posX = new QSpinBox(painel);
    posY = new QSpinBox(painel);
    posZ = new QSpinBox(painel);

    for (QSpinBox *campo : {posX, posY, posZ}) {
        campo->setRange(0, 29);
        campo->setValue(15);
    }

    // Atualiza o plano Z quando o usuário altera a coordenada.
    connect(posZ, &QSpinBox::valueChanged,
            visualizacao, &SculptorView::setPlanoZ);

    // Desenhar ao clicar ou arrastar com o botão esquerdo.
    connect(
        visualizacao,
        &SculptorView::voxelSobMouse,
        this,
        [this](int x, int y) {
            posX->setValue(x);
            posY->setValue(y);
            aplicarForma();
        }
        );

    // Dimensões das formas
    dimX = new QSpinBox(painel);
    dimY = new QSpinBox(painel);
    dimZ = new QSpinBox(painel);

    for (QSpinBox *campo : {dimX, dimY, dimZ}) {
        campo->setRange(0, 29);
        campo->setValue(3);
    }

    auto *form = new QFormLayout;
    form->addRow("Posição X:", posX);
    form->addRow("Posição Y:", posY);
    form->addRow("Posição Z:", posZ);
    form->addRow("Raio / dimensão X:", dimX);
    form->addRow("Dimensão Y:", dimY);
    form->addRow("Dimensão Z:", dimZ);
    layoutPainel->addLayout(form);

    // Seleção de cor
    auto *botaoCor = new QPushButton("Escolher cor", painel);
    botaoCor->setStyleSheet(
        "background-color: red; color: white; padding: 6px;"
        );

    connect(botaoCor, &QPushButton::clicked, this,
            [this, botaoCor]() {
                QColor novaCor = QColorDialog::getColor(
                    corAtual, this, "Escolher cor"
                    );

                if (novaCor.isValid()) {
                    corAtual = novaCor;

                    botaoCor->setStyleSheet(
                        QString(
                            "background-color: %1; color: white; padding: 6px;"
                            ).arg(corAtual.name())
                        );
                }
            });

    layoutPainel->addWidget(botaoCor);

    // Aplicar forma manualmente
    auto *botaoAplicar = new QPushButton("Aplicar forma", painel);
    connect(botaoAplicar, &QPushButton::clicked,
            this, &MainWindow::aplicarForma);
    layoutPainel->addWidget(botaoAplicar);

    // Carregar exemplo
    auto *botaoExemplo = new QPushButton("Carregar exemplo", painel);
    connect(botaoExemplo, &QPushButton::clicked,
            this, &MainWindow::carregarExemplo);
    layoutPainel->addWidget(botaoExemplo);

    // Exportar OFF
    auto *botaoExportar = new QPushButton("Exportar OFF", painel);
    connect(botaoExportar, &QPushButton::clicked,
            this, &MainWindow::exportarOFF);
    layoutPainel->addWidget(botaoExportar);

    layoutPainel->addStretch();
    layoutPrincipal->addWidget(painel);

    setCentralWidget(central);
}

void MainWindow::criarEscultura()
{
    int nx = tamanhoX->value();
    int ny = tamanhoY->value();
    int nz = tamanhoZ->value();

    Sculptor *novaEscultura = new Sculptor(nx, ny, nz);

    delete escultor;
    escultor = novaEscultura;

    visualizacao->setSculptor(escultor);

    posX->setRange(0, nx - 1);
    posY->setRange(0, ny - 1);
    posZ->setRange(0, nz - 1);

    posX->setValue(nx / 2);
    posY->setValue(ny / 2);
    posZ->setValue(nz / 2);

    visualizacao->atualizarEscultura();
}

void MainWindow::aplicarForma()
{
    int x = posX->value();
    int y = posY->value();
    int z = posZ->value();

    int dx = dimX->value();
    int dy = dimY->value();
    int dz = dimZ->value();

    escultor->setColor(
        corAtual.redF(),
        corAtual.greenF(),
        corAtual.blueF(),
        corAtual.alphaF()
        );

    switch (ferramenta->currentIndex()) {
    case 0:
        escultor->putVoxel(x, y, z);
        break;

    case 1:
        escultor->putBox(
            x, x + dx, y, y + dy, z, z + dz
            );
        break;

    case 2:
        escultor->putSphere(x, y, z, dx);
        break;

    case 3:
        escultor->putEllipsoid(x, y, z, dx, dy, dz);
        break;

    case 4:
        escultor->cutVoxel(x, y, z);
        break;

    case 5:
        escultor->cutBox(
            x, x + dx, y, y + dy, z, z + dz
            );
        break;

    case 6:
        escultor->cutSphere(x, y, z, dx);
        break;

    case 7:
        escultor->cutEllipsoid(x, y, z, dx, dy, dz);
        break;
    }

    visualizacao->atualizarEscultura();
}

void MainWindow::exportarOFF()
{
    QString nome = QFileDialog::getSaveFileName(
        this,
        "Exportar escultura",
        "escultura.off",
        "Arquivos OFF (*.off)"
        );

    if (nome.isEmpty()) {
        return;
    }

    if (!nome.endsWith(".off", Qt::CaseInsensitive)) {
        nome += ".off";
    }

    if (escultor->writeOFF(nome.toLocal8Bit().constData())) {
        QMessageBox::information(
            this,
            "Exportação",
            "Arquivo OFF salvo com sucesso."
            );
    } else {
        QMessageBox::critical(
            this,
            "Erro",
            "Não foi possível salvar o arquivo OFF."
            );
    }
}

void MainWindow::carregarExemplo()
{
    const int nx = escultor->getNx();
    const int ny = escultor->getNy();
    const int nz = escultor->getNz();

    // Limpa a matriz.
    for (int x = 0; x < nx; ++x) {
        for (int y = 0; y < ny; ++y) {
            for (int z = 0; z < nz; ++z) {
                escultor->cutVoxel(x, y, z);
            }
        }
    }

    // Centro e raio da Pokébola.
    const int cx = nx / 2;
    const int cy = ny / 2;
    const int cz = nz / 2;

    const int raio = std::max(
        1,
        std::min({nx / 2 - 1, ny / 2 - 2, nz / 2 - 1})
        );

    // Constrói a esfera voxel por voxel.
    for (int x = cx - raio; x <= cx + raio; ++x) {
        for (int y = cy - raio; y <= cy + raio; ++y) {
            for (int z = cz - raio; z <= cz + raio; ++z) {

                if (x < 0 || x >= nx ||
                    y < 0 || y >= ny ||
                    z < 0 || z >= nz) {
                    continue;
                }

                const int dx = x - cx;
                const int dy = y - cy;
                const int dz = z - cz;

                if (dx * dx + dy * dy + dz * dz > raio * raio) {
                    continue;
                }

                // Faixa preta central.
                if (std::abs(dz) <= std::max(1, raio / 9)) {
                    escultor->setColor(0.06, 0.06, 0.06, 1.0);
                }
                // Metade superior vermelha.
                else if (dz > 0) {
                    escultor->setColor(0.92, 0.025, 0.045, 1.0);
                }
                // Metade inferior branca.
                else {
                    escultor->setColor(0.94, 0.94, 0.90, 1.0);
                }

                escultor->putVoxel(x, y, z);
            }
        }
    }

    // Botão central preto, na face frontal.
    const int yBotao = std::min(ny - 1, cy + raio - 1);

    escultor->setColor(0.03, 0.03, 0.03, 1.0);
    escultor->putSphere(cx, yBotao, cz, std::max(1, raio / 5));

    // Centro branco do botão.
    const int yCentro = std::min(ny - 1, yBotao + std::max(1, raio / 5));

    escultor->setColor(1.0, 1.0, 1.0, 1.0);
    escultor->putSphere(cx, yCentro, cz, std::max(1, raio / 9));

    visualizacao->atualizarEscultura();
}
