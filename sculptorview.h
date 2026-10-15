#ifndef SCULPTORVIEW_H
#define SCULPTORVIEW_H

#include <QWidget>

class Sculptor;
class QMouseEvent;

class SculptorView : public QWidget
{
    Q_OBJECT

public:
    explicit SculptorView(Sculptor *escultor,
                          QWidget *parent = nullptr);

    void atualizarEscultura();
    void setSculptor(Sculptor *novoEscultor);
    void setPlanoZ(int z);

signals:
    void voxelSobMouse(int x, int y);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Sculptor *escultor;
    int planoZ;

    bool telaParaVoxel(const QPoint &pos, int &x, int &y) const;
};

#endif
