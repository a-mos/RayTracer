#pragma once
#include <cmath>
#include "Utils/Vectors.h"

struct xyz {
    float x, y, z;
};

double GammaCorr(float x) {
    if (x <= 0.05) {
        return x * 2.64;
    }
    return 1.099 * pow(x, 0.9 / 2.2) - 0.099;
}

// Adaptive Logarithmic Mapping For Displaying High Contrast Scenes //
void ToneMapping(int height, int width, std::vector<Vec3f> &input) {
    xyz tmp[height][width];
    std::vector<Vec3f> output(width * height);
    float r, g, b;
    float lwmax = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            r = input[j + i * width][0] / 255.0;
            g = input[j + i * width][1] / 255.0;
            b = input[j + i * width][2] / 255.0;
            tmp[i][j].x = (0.4124*r + 0.3576*g + 0.1805*b);
            tmp[i][j].y = (0.2126*r + 0.7152*g + 0.0722*b);
            tmp[i][j].z = (0.0193*r + 0.1192*g + 0.9505*b);
            lwmax = std::fmax(lwmax, tmp[i][j].y);
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float xx = tmp[i][j].x / (tmp[i][j].x + tmp[i][j].y + tmp[i][j].z);
            float yy = tmp[i][j].y / (tmp[i][j].x + tmp[i][j].y + tmp[i][j].z);

            tmp[i][j].y = 1.0 * log(tmp[i][j].y + 1) / log(2 + 8.0*pow((tmp[i][j].y / lwmax), log(0.8) / log(0.5))) / log10(lwmax + 1);
            float x = tmp[i][j].y / yy*xx;
            float y = tmp[i][j].y;
            float z = tmp[i][j].y / yy*(1 - xx - yy);

            r = clamp(3.2410*x - 1.5374*y - 0.4986*z, 0.0, 1.0);
            g = clamp(-0.9692*x + 1.8760*y + 0.0416*z, 0.0, 1.0);
            b = clamp(0.0556*x - 0.2040*y + 1.0570*z, 0.0, 1.0);

            r = GammaCorr(r);
            g = GammaCorr(g);
            b = GammaCorr(b);

            output[i * width + j][0] = r;
            output[i * width + j][1] = g;
            output[i * width + j][2] = b;
        }
    }
    input = output;
}