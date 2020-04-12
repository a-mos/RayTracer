#pragma once
#include "Objects/Triangle.h"
#include <fstream>
#include <sstream>

auto MakeFig(const char *name, Material &m, Vec3f shift) {
    std::vector<Vec3f> vert;
    std::vector<Vec3i> faces;
    std::ifstream in(name, std::ifstream::in);
    std::string line;
    Vec3f v;
    Vec3i f;
    char trash;
    int idx, cnt;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line);
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            for (int i = 0; i < 3; ++i) iss >> v[i];
            vert.push_back(v + shift);
        } else if (!line.compare(0, 2, "f ")) {
            iss >> trash;
            cnt = 0;
            while (iss >> idx) {
                --idx;
                f[cnt++] = idx;
            }
            if (3 == cnt) faces.push_back(f);
        }
    }

    std::vector<Object *> figures;
    for (const auto &face : faces) {
        auto *TMP = new Triangle(vert[face.x], vert[face.y], vert[face.z], m);
        figures.push_back(TMP);
    }
    return figures;
}