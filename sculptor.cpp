
#include "sculptor.h"
#include <fstream>
#include <iostream>
#include <vector>

Sculptor::Sculptor(int _nx, int _ny, int _nz) {
    nx = _nx;
    ny = _ny;
    nz = _nz;

    r = g = b = 0.0f;
    a = 1.0f;

    if (nx <= 0 || ny <= 0 || nz <= 0) {
        nx = ny = nz = 0;
        v = nullptr;
        return;
    }

    v = new Voxel**[nx];

    for (int x = 0; x < nx; x++) {
        v[x] = new Voxel*[ny];

        for (int y = 0; y < ny; y++) {
            v[x][y] = new Voxel[nz];

            for (int z = 0; z < nz; z++) {
                v[x][y][z] = {0, 0, 0, 1, false};
            }
        }
    }
}

Sculptor::~Sculptor() {
    if (!v) return;

    for (int x = 0; x < nx; x++) {
        for (int y = 0; y < ny; y++) {
            delete[] v[x][y];
        }
        delete[] v[x];
    }
    delete[] v;
}

void Sculptor::setColor(float r, float g, float b, float a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void Sculptor::putVoxel(int x, int y, int z) {
    if (x < 0 || x >= nx || y < 0 || y >= ny ||
        z < 0 || z >= nz) return;

    v[x][y][z] = {r, g, b, a, true};
}

void Sculptor::cutVoxel(int x, int y, int z) {
    if (x < 0 || x >= nx || y < 0 || y >= ny ||
        z < 0 || z >= nz) return;

    v[x][y][z].show = false;
}

void Sculptor::putBox(int x0, int x1, int y0, int y1,
                      int z0, int z1) {
    for (int x = x0; x <= x1; x++)
        for (int y = y0; y <= y1; y++)
            for (int z = z0; z <= z1; z++)
                putVoxel(x, y, z);
}

void Sculptor::cutBox(int x0, int x1, int y0, int y1,
                      int z0, int z1) {
    for (int x = x0; x <= x1; x++)
        for (int y = y0; y <= y1; y++)
            for (int z = z0; z <= z1; z++)
                cutVoxel(x, y, z);
}

void Sculptor::putSphere(int xc, int yc, int zc, int radius) {
    if (radius < 0) return;

    for (int x = xc - radius; x <= xc + radius; x++)
        for (int y = yc - radius; y <= yc + radius; y++)
            for (int z = zc - radius; z <= zc + radius; z++) {
                int dx = x - xc, dy = y - yc, dz = z - zc;
                if (dx*dx + dy*dy + dz*dz <= radius*radius)
                    putVoxel(x, y, z);
            }
}

void Sculptor::cutSphere(int xc, int yc, int zc, int radius) {
    if (radius < 0) return;

    for (int x = xc - radius; x <= xc + radius; x++)
        for (int y = yc - radius; y <= yc + radius; y++)
            for (int z = zc - radius; z <= zc + radius; z++) {
                int dx = x - xc, dy = y - yc, dz = z - zc;
                if (dx*dx + dy*dy + dz*dz <= radius*radius)
                    cutVoxel(x, y, z);
            }
}

void Sculptor::putEllipsoid(int xc, int yc, int zc,
                            int rx, int ry, int rz) {
    if (rx <= 0 || ry <= 0 || rz <= 0) return;

    for (int x = xc - rx; x <= xc + rx; x++)
        for (int y = yc - ry; y <= yc + ry; y++)
            for (int z = zc - rz; z <= zc + rz; z++) {
                double dx = double(x - xc) / rx;
                double dy = double(y - yc) / ry;
                double dz = double(z - zc) / rz;

                if (dx*dx + dy*dy + dz*dz <= 1.0)
                    putVoxel(x, y, z);
            }
}

void Sculptor::cutEllipsoid(int xc, int yc, int zc,
                            int rx, int ry, int rz) {
    if (rx <= 0 || ry <= 0 || rz <= 0) return;

    for (int x = xc - rx; x <= xc + rx; x++)
        for (int y = yc - ry; y <= yc + ry; y++)
            for (int z = zc - rz; z <= zc + rz; z++) {
                double dx = double(x - xc) / rx;
                double dy = double(y - yc) / ry;
                double dz = double(z - zc) / rz;

                if (dx*dx + dy*dy + dz*dz <= 1.0)
                    cutVoxel(x, y, z);
            }
}

bool Sculptor::writeOFF(const char *filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    struct Face {
        int a, b, c, d;
        float r, g, bcolor, alpha;
    };

    std::vector<float> vertices;
    std::vector<Face> faces;

    // Cada face visível recebe quatro vértices próprios.
    // Isso evita faces internas entre voxels vizinhos.
    auto addFace = [&](float x0, float y0, float z0,
                       float x1, float y1, float z1,
                       float x2, float y2, float z2,
                       float x3, float y3, float z3,
                       const Voxel &voxel) {
        int base = static_cast<int>(vertices.size() / 3);

        vertices.insert(vertices.end(), {
                                            x0,y0,z0, x1,y1,z1, x2,y2,z2, x3,y3,z3
                                        });

        faces.push_back({base, base+1, base+2, base+3,
                         voxel.r, voxel.g, voxel.b, voxel.a});
    };

    for (int x = 0; x < nx; x++)
        for (int y = 0; y < ny; y++)
            for (int z = 0; z < nz; z++) {
                const Voxel &voxel = v[x][y][z];
                if (!voxel.show) continue;

                float x0=x, x1=x+1, y0=y, y1=y+1, z0=z, z1=z+1;

                if (x == 0 || !v[x-1][y][z].show)
                    addFace(x0,y0,z0, x0,y1,z0, x0,y1,z1, x0,y0,z1, voxel);

                if (x == nx-1 || !v[x+1][y][z].show)
                    addFace(x1,y0,z0, x1,y0,z1, x1,y1,z1, x1,y1,z0, voxel);

                if (y == 0 || !v[x][y-1][z].show)
                    addFace(x0,y0,z0, x0,y0,z1, x1,y0,z1, x1,y0,z0, voxel);

                if (y == ny-1 || !v[x][y+1][z].show)
                    addFace(x0,y1,z0, x1,y1,z0, x1,y1,z1, x0,y1,z1, voxel);

                if (z == 0 || !v[x][y][z-1].show)
                    addFace(x0,y0,z0, x1,y0,z0, x1,y1,z0, x0,y1,z0, voxel);

                if (z == nz-1 || !v[x][y][z+1].show)
                    addFace(x0,y0,z1, x0,y1,z1, x1,y1,z1, x1,y0,z1, voxel);
            }

    file << "OFF\n";
    file << vertices.size()/3 << " " << faces.size() << " 0\n";

    for (size_t i = 0; i < vertices.size(); i += 3)
        file << vertices[i] << " " << vertices[i+1] << " "
             << vertices[i+2] << "\n";

    for (const Face &face : faces)
        file << "4 " << face.a << " " << face.b << " "
             << face.c << " " << face.d << " "
             << face.r << " " << face.g << " "
             << face.bcolor << " " << face.alpha << "\n";

    return file.good();
}

int Sculptor::getNx() const {
    return nx;
}

int Sculptor::getNy() const {
    return ny;
}

int Sculptor::getNz() const {
    return nz;
}

Voxel Sculptor::getVoxel(int x, int y, int z) const {
    if (x < 0 || x >= nx ||
        y < 0 || y >= ny ||
        z < 0 || z >= nz) {
        return {0, 0, 0, 0, false};
    }

    return v[x][y][z];
}
