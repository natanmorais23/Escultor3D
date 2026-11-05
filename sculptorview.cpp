#include "sculptorview.h"
#include "sculptor.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPolygonF>
#include <QPointF>
#include <QtMath>
#include <algorithm>

namespace {

struct Projecao {
    double unidade;
    double centroX;
    double baseY;
};

Projecao calcularProjecao(int largura, int altura, int nx, int ny, int nz)
{
    const double topo = 60.0;
    const double areaAltura = qMax(1.0, altura - topo);

    const double unidade = qMin(
                               largura / ((nx + ny) / 2.0),
                               areaAltura / ((nx + ny) / 4.0 + nz / 2.0)
                               ) * 0.85;

    Projecao p;
    p.unidade = unidade;
    p.centroX = largura / 2.0 - (ny - nx) * unidade / 4.0;
    p.baseY = topo + areaAltura / 2.0
              - ((nx + ny) * unidade / 4.0 - nz * unidade / 2.0) / 2.0;
    return p;
}

}

SculptorView::SculptorView(Sculptor *escultura, QWidget *parent)
    : QWidget(parent),
    escultor(escultura),
    planoZ(15)
{
    setMinimumSize(500, 500);
    setMouseTracking(true);
}

void SculptorView::atualizarEscultura()
{
    update();
}

void SculptorView::setSculptor(Sculptor *novoEscultor)
{
    escultor = novoEscultor;

    if (escultor) {
        planoZ = qBound(0, planoZ, escultor->getNz() - 1);
    } else {
        planoZ = 0;
    }

    update();
}

void SculptorView::setPlanoZ(int z)
{
    if (!escultor) {
        planoZ = 0;
        return;
    }

    planoZ = qBound(0, z, escultor->getNz() - 1);
    update();
}

void SculptorView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(245, 245, 245));

    if (!escultor) {
        return;
    }

    const int nx = escultor->getNx();
    const int ny = escultor->getNy();
    const int nz = escultor->getNz();

    if (nx <= 0 || ny <= 0 || nz <= 0) {
        return;
    }

    const Projecao proj = calcularProjecao(width(), height(), nx, ny, nz);
    const double unidade = proj.unidade;
    const double centroX = proj.centroX;
    const double baseY = proj.baseY;

    auto projetar = [&](double x, double y, double z) -> QPointF {
        return QPointF(
            centroX + (y - x) * unidade / 2.0,
            baseY + (x + y) * unidade / 4.0 - z * unidade / 2.0
            );
    };

    painter.setPen(QPen(QColor(70, 70, 70), 1));

    // Desenha os voxels visíveis.
    const int profundidadeMaxima = (nx - 1) + (ny - 1) + (nz - 1);

    for (int s = 0; s <= profundidadeMaxima; ++s) {
        for (int z = 0; z < nz; ++z) {
            for (int x = 0; x < nx; ++x) {
                const int y = s - x - z;

                if (y < 0 || y >= ny) {
                    continue;
                }

                auto voxel = escultor->getVoxel(x, y, z);

                if (!voxel.show) {
                    continue;
                }

                QColor cor(
                    qBound(0, qRound(voxel.r * 255), 255),
                    qBound(0, qRound(voxel.g * 255), 255),
                    qBound(0, qRound(voxel.b * 255), 255),
                    qBound(0, qRound(voxel.a * 255), 255)
                    );

                QPointF p000 = projetar(x,     y,     z);
                QPointF p100 = projetar(x + 1, y,     z);
                QPointF p010 = projetar(x,     y + 1, z);
                QPointF p110 = projetar(x + 1, y + 1, z);
                QPointF p001 = projetar(x,     y,     z + 1);
                QPointF p101 = projetar(x + 1, y,     z + 1);
                QPointF p011 = projetar(x,     y + 1, z + 1);
                QPointF p111 = projetar(x + 1, y + 1, z + 1);

                // Face superior
                bool topoLivre = (z + 1 >= nz) ||
                                 !escultor->getVoxel(x, y, z + 1).show;

                if (topoLivre) {
                    QPolygonF topo;
                    topo << p001 << p101 << p111 << p011;

                    QColor corTopo = cor.lighter(115);
                    painter.setBrush(corTopo);
                    painter.drawPolygon(topo);
                }

                // Face voltada para a frente/esquerda
                bool faceX = (x + 1 >= nx) ||
                             !escultor->getVoxel(x + 1, y, z).show;

                if (faceX) {
                    QPolygonF ladoX;
                    ladoX << p100 << p110 << p111 << p101;

                    QColor corX = cor.darker(115);
                    painter.setBrush(corX);
                    painter.drawPolygon(ladoX);
                }

                // Face voltada para a frente/direita
                bool faceY = (y + 1 >= ny) ||
                             !escultor->getVoxel(x, y + 1, z).show;

                if (faceY) {
                    QPolygonF ladoY;
                    ladoY << p010 << p110 << p111 << p011;

                    QColor corY = cor.darker(130);
                    painter.setBrush(corY);
                    painter.drawPolygon(ladoY);
                }

                Q_UNUSED(p000);
            }
        }
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);

    painter.drawText(
        10,
        25,
        QString("Escultor 3D — matriz %1 x %2 x %3")
            .arg(nx)
            .arg(ny)
            .arg(nz)
        );

    painter.drawText(
        10,
        45,
        QString("Plano de desenho: Z = %1").arg(planoZ)
        );
}

bool SculptorView::telaParaVoxel(
    const QPoint &pos,
    int &x,
    int &y
    ) const
{
    if (!escultor) {
        return false;
    }

    const int nx = escultor->getNx();
    const int ny = escultor->getNy();
    const int nz = escultor->getNz();

    if (nx <= 0 || ny <= 0 || nz <= 0) {
        return false;
    }

    const Projecao proj = calcularProjecao(width(), height(), nx, ny, nz);
    const double unidade = proj.unidade;

    if (unidade <= 0.0) {
        return false;
    }

    // Converte a posição do mouse para coordenadas do plano Z.
    const double u = (pos.x() - proj.centroX) / (unidade / 2.0);

    const double v =
        (pos.y() - proj.baseY + (planoZ + 0.5) * unidade / 2.0)
        / (unidade / 4.0);

    const double coordenadaX = (v - u) / 2.0;
    const double coordenadaY = (v + u) / 2.0;

    x = qFloor(coordenadaX);
    y = qFloor(coordenadaY);

    return x >= 0 && x < nx && y >= 0 && y < ny;
}

void SculptorView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int x, y;

        if (telaParaVoxel(event->pos(), x, y)) {
            emit voxelSobMouse(x, y);
        }

        event->accept();
        return;
    }

    QWidget::mousePressEvent(event);
}

void SculptorView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int x, y;

        if (telaParaVoxel(event->pos(), x, y)) {
            emit voxelSobMouse(x, y);
        }

        event->accept();
        return;
    }

    QWidget::mouseMoveEvent(event);
}
