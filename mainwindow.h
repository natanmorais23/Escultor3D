
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>

class QSpinBox;
class QComboBox;
class QPushButton;
class Sculptor;
class SculptorView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void aplicarForma();
    void exportarOFF();
    void carregarExemplo();

private:
    Sculptor *escultor;
    SculptorView *visualizacao;

    QComboBox *ferramenta;
    QSpinBox *posX;
    QSpinBox *posY;
    QSpinBox *posZ;
    QSpinBox *dimX;
    QSpinBox *dimY;
    QSpinBox *dimZ;


    QColor corAtual;

    void configurarInterface();

    QSpinBox *tamanhoX;
    QSpinBox *tamanhoY;
    QSpinBox *tamanhoZ;

    void criarEscultura();
};

#endif
