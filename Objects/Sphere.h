#pragma once
#include "Utils/Vectors.h"
#include "Utils/Material.h"
#include "Object.h"

struct Sphere: public Object {
    Vec3f center;
    float radius;
    Material material;

    Sphere(const Vec3f &c, const float &r, const Material &m) : center(c), radius(r), material(m) {}

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const override {
        Vec3f L = center - orig;
        float tca = L * dir;
        float d2 = L * L - tca * tca;
        if (d2 > radius * radius) return false;
        float thc = sqrtf(radius * radius - d2);
        t0 = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) t0 = t1;
        return t0 >= 0;
    }

    Vec3f getNvec(Vec3f &hit) const override {
        return (hit - center).normalize();
    }
    Material& getMaterial() override {
        return material;
    }
};


//Double precision sphere
struct sphere : public object {
    Vec3d center;
    double radius;
    Material material;
    sphere(const Vec3d &c, const double &r, const Material &m) : center(c), radius(r), material(m) {}
    bool ray_intersect(const Vec3d &orig, const Vec3d &dir, double &t0) const override {
        Vec3d L = center - orig;
        double tca = L * dir;
        double d2 = L * L - tca * tca;
        if (d2 > radius * radius) {
            return false;
        }
        double thc = sqrt(radius * radius - d2);
        t0 = tca - thc;
        double t1 = tca + thc;
        if (t0 < 0) t0 = t1;
        return t0 >= 0;
    }

    Vec3d getNvec(Vec3d &hit) const override {
        return (hit - center).normalize();
    }
    Vec3d getColor() const override  {
        return {material.diffuse_color[0], material.diffuse_color[1], material.diffuse_color[2]};
    }
    Vec3d getEmission() const override {
        return {material.albedo[0], material.albedo[1], material.albedo[2]};
    }
    Material& getMaterial() override {
        return material;
    }
};