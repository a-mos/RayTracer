#pragma once
#include "Utils/Vectors.h"
#include "Utils/Material.h"
#include "Object.h"

struct Quadratic: Object {
    Vec3f centre;
    Material material;
    float A, B, C, G, H, I, J, height;
    int axis;
    Quadratic(int ax, float a, float b, float c, float g, float h, float i, float j, float height, Vec3f centre, Material m) : centre(centre), material(m), height(height) {
        A = a;
        B = b;
        C = c;
        G = g;
        H = h;
        I = i;
        J = j;
        axis = ax;
    }

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t) const override {
        Vec3f center = orig - centre;

        float a = dir.x * dir.x * A + dir.y * dir.y * B + dir.z * dir.z * C;
        float b = 2 * center.x * dir.x * A + 2 * center.y * dir.y * B + 2 * center.z * dir.z * C + G * dir.x + H * dir.y + I * dir.z;
        float c = center.x * center.x * A +  center.y * center.y * B + center.z * center.z * C + G * center.x + H * center.y + I * center.z + J;

        float D = b * b - 4 * a * c;

        if (D < 0) return false;

        t = (-b - sqrtf(D)) / (2 * a);
        if (t < 0) {
            return false;
        }
        Vec3f hit = orig + dir * t;

        if (axis == 0) {
            return std::fabs(hit.y - center.y) < height;
        } else if (axis == 1) {
            return std::fabs(hit.x - center.x) < height;
        }
    }

    Vec3f getNvec(Vec3f &hit) const override {
        Vec3f move = hit - centre;
        return Vec3f(2 * A * move.x + G,
                     2 * B * move.y + H,
                     2 * C * move.z + I).normalize();
    }

    Material& getMaterial() override {
        return material;
    }
};