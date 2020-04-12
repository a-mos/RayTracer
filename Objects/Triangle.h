#pragma once
#include "Utils/Vectors.h"
#include "Utils/Material.h"
#include "Object.h"

struct Triangle: public Object {
    Material material;
    Vec3f v0, v1, v2, N;
    Vec3f edge1, edge2;
    Triangle(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, const Material &m) : v0(v0), v1(v1), v2(v2), material(m) {
        N = (cross(v1 - v0, v2 - v0)).normalize();
        edge1 = v1 - v0;
        edge2 = v2 - v0;
    }

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const override {
        Vec3f h, s, q;
        float a, f, u, v;
        h = cross(dir, edge2);
        a = edge1 * h;
        if (a < 0.0000001)
            return false; // Parallel to triangle
        f = 1.0 / a;
        s = orig - v0;
        u = f * (s * h);
        if (u < 0.0 || u > 1.0)
            return false;
        q = cross(s, edge1);
        v = f * (dir * q);
        if (v < 0.0 || u + v > 1.0)
            return false;
        // Find intersection point
        float t = f * (edge2 * q);
        if (t > 0.0000001) {
            t0 = t;
            return true;
        } else {
            return false;
        }
    }

    Vec3f getNvec(Vec3f &hit) const override {
        return N;
    }

    Material& getMaterial() override {
        return material;
    }
};