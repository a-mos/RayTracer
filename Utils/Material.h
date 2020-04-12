#pragma once
#include "Utils/Vectors.h"

struct Material {
    Material(const float &r, const Vec4f &a, const Vec3f &color, const float &spec) : refractive_index(r),
                                                                                      albedo(a),
                                                                                      diffuse_color(color),
                                                                                      specular_exponent(spec) {}

    Material() : refractive_index(1), albedo(1, 0, 0, 0), diffuse_color(), specular_exponent() {}
    float refractive_index;
    Vec4f albedo;
    Vec3f diffuse_color;
    float specular_exponent;
};

namespace Materials {
    Material mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1500.);
    Material wood(1.0, Vec4f(0.6, 0.3, 0.1, 0.0), Vec3f(68, 39, 28) * (1/255.), 50.);
    Material black(1.0, Vec4f(0.5, 0.3, 0.1, 0.0), Vec3f(0, 0, 0) * (1/255.), 50.);
    Material yellow(1.0, Vec4f(0.5, 0.3, 0.2, 0.0), Vec3f(230, 212, 62) * (1/255.), 50.);
    Material green(1.0, Vec4f(0.5, 0.3, 0.2, 0.0), Vec3f(45, 194, 45) * (1/255.), 50.);
    Material shiraz(1.0, Vec4f(0.5, 0.3, 0.2, 0.0), Vec3f(182, 1, 46) * (1/255.), 50.);
    Material thunderbird(1.0, Vec4f(0.5, 0.3, 0.2, 0.0), Vec3f(208, 0, 14) * (1/255.), 50.);
    Material violet(1.0, Vec4f(0.5, 0.3, 0.2, 0.0), Vec3f(164, 0, 213) * (1/255.), 50.);
    Material orange(1.0, Vec4f(0.5, 0.3, 0.2, 0.0), Vec3f(217, 129, 30) * (1/255.), 50.);
    Material blue(1.0, Vec4f(0.5, 0.3, 0.2, 0.0), Vec3f(14, 77, 187) * (1/255.), 50.);
    Material white(1.0, Vec4f(0.5, 0.3, 0.2, 0.0), Vec3f(255, 255, 255) * (1/255.), 50.);

    Material fractal(1.5, Vec4f(0.5, 0.5, 0.4, 0.2), Vec3f(160.0, 160.0, 160.0) * (1/255.), 10.);

    Material ball1(1.0, Vec4f(0, 0, 0, 0), Vec3f(.8,.20,.25), 0.0);
    Material ball2(1.0, Vec4f(0, 0, 0, 0), Vec3f(.999,.999,.999), 1.0);
    Material left(1.0, Vec4f(0, 0, 0, 0), Vec3f(0.3,.75,.25), 0.0);
    Material right(1.0, Vec4f(0, 0, 0, 0), Vec3f(0.3,.15,.45), 0.0);
    Material back(1.0, Vec4f(0, 0, 0, 0), Vec3f(0.8,.75,.75), 0.0);
    Material bottom(1.0, Vec4f(0, 0, 0, 0), Vec3f(.50,.30,.35), 0.0);
    Material light(1.0, Vec4f(2., 2., 2., 1.), Vec3f(0,0,0), 0.0);
}