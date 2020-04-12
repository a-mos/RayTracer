#pragma once
#include "Utils/Vectors.h"
#include "Utils/Material.h"
#include "Object.h"

struct Fractal : Object {
    Material material;
    const Vec3f centre;
    const float radius;
    const float max_dist = 50.0;
    const int max_iters = 256;

    Fractal(const Vec3f &centre, const float &radius, const Material &material) : centre(centre), radius(radius), material(material) {}

    float SDF(const Vec3f &p) const {
        float modulo = 1.5;
        Vec3f P(std::fabs(std::fmod(p.x, modulo)), std::fmod(p.y, modulo), std::fmod(p.z, modulo));
        return (P - centre).norm() - radius;
    }

    bool ray_intersect(const Vec3f &orig, const Vec3f &direction, float &t) const override {
        float sum = 0.0f;
        int i = 0;
        while (i < max_iters && sum < max_dist) {
            sum += SDF(orig + direction * sum);
            ++i;
        }
        t = sum;
        return sum < max_dist;
    }

    Vec3f getNvec(Vec3f &hit) const override {
        return Vec3f(SDF(hit + Vec3f(0.001, 0.0, 0.0)) - SDF(hit - Vec3f(0.001, 0.0, 0.0)),
                     SDF(hit + Vec3f(0.0, 0.001, 0.0)) - SDF(hit - Vec3f(0.0, 0.001, 0.0)),
                     SDF(hit + Vec3f(0.0, 0.0, 0.001)) - SDF(hit - Vec3f(0.0, 0.0, 0.001))).normalize();
    }

    Material& getMaterial() override {
        return material;
    }
};