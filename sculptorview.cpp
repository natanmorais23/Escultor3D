#include "sculptorview.h"
#include "sculptor.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPolygonF>
#include <QPointF>
#include <QtMath>
#include <algorithm>

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

    const double unidade = qMin(
                               width() / double(nx + ny + 2),
                               height() / double((nx + ny) * 0.5 + nz + 2)
                               ) * 0.8;

    const double centroX = width() / 2.0;
    const double baseY = height() * 0.82;

    auto projetar = [&](double x, double y, double z) -> QPointF {
        return QPointF(
            centroX + (x - y) * unidade / 2.0,
            baseY - (x + y) * unidade / 4.0 - z * unidade
            );
    };

    painter.setPen(QPen(QColor(70, 70, 70), 1));

    // Desenha os voxels visíveis.
    for (int z = 0; z < nz; ++z) {
        for (int x = 0; x < nx; ++x) {
            for (int y = 0; y < ny; ++y) {
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

                // Face voltada para a frente/direita
                bool faceX = (x + 1 >= nx) ||
                             !escultor->getVoxel(x + 1, y, z).show;

                if (faceX) {
                    QPolygonF ladoX;
                    ladoX << p100 << p110 << p111 << p101;

                    QColor corX = cor.darker(115);
                    painter.setBrush(corX);
                    painter.drawPolygon(ladoX);
                }

                // Face voltada para a frente/esquerda
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

    const double unidade = qMin(
                               width() / double(nx + ny + 2),
                               height() / double((nx + ny) * 0.5 + nz + 2)
                               ) * 0.8;

    if (unidade <= 0.0) {
        return false;
    }

    const double centroX = width() / 2.0;
    const double baseY = height() * 0.82;

    // Converte a posição do mouse para coordenadas do plano Z.
    const double u = (pos.x() - centroX) / (unidade / 2.0);

    const double v =
        (baseY - pos.y() - (planoZ + 0.5) * unidade)
        / (unidade / 4.0);

    const double coordenadaX = (u + v) / 2.0;
    const double coordenadaY = (v - u) / 2.0;

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
