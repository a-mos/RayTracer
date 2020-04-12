#pragma once
#include "Object.h"
#include <array>

struct Cube : public Object {
    Material material;
    std::array<Vec3f, 2> bounds;
    Vec3f centre;
    Cube(const Vec3f &v1, const Vec3f &v2, const Material &m) : material(m) {
        bounds[0] = v1;
        bounds[1] = v2;
        centre = (bounds[0] + bounds[1]) * (0.5);
    }

    ~Cube() override = default;

    Material& getMaterial() override {
        return material;
    }

    Vec3f getNvec(Vec3f &hit) const override {
        Vec3f p = hit - centre;
        Vec3f d = (bounds[0] - bounds[1]) * 0.5;
        float bias = 1.0001;
        auto a = float(int(p.x / fabs(d.x) * bias));
        auto b = float(int(p.y / fabs(d.y) * bias));
        auto c = float(int(p.z / fabs(d.z) * bias));
        return  Vec3f(a, b, c).normalize();
    }

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const override {
        Vec3f invdir = 1.0f / dir;
        Vec3f sign((float)(invdir.x < 0), (float)(invdir.y < 0), (float)(invdir.z < 0));

        auto tmin  = (bounds[    sign[0]].x - orig.x) * invdir.x;
        auto tmax  = (bounds[1 - sign[0]].x - orig.x) * invdir.x;
        auto tymin = (bounds[    sign[1]].y - orig.y) * invdir.y;
        auto tymax = (bounds[1 - sign[1]].y - orig.y) * invdir.y;

        if (tmin > tymax || tymin > tmax) return false;
        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        auto tzmin = (bounds[    sign[2]].z - orig.z) * invdir.z;
        auto tzmax = (bounds[1 - sign[2]].z - orig.z) * invdir.z;

        if ((tmin > tzmax) || (tzmin > tmax)) return false;
        if (tzmin > tmin) tmin = tzmin;
        if (tzmax < tmax) tmax = tzmax;

        t0 = tmin;

        if (t0 < 0) {
            t0 = tmax;
            if (t0 < 0) return false;
        }

        return true;
    }
};


//Double precision AABBbox
struct cube : public object {
    Material material;
    std::array<Vec3d, 2> bounds;
    Vec3d centre;
    cube(const Vec3d &vmin, const Vec3d &vmax, const Material &m): material(m) {
        bounds[0] = vmin;
        bounds[1] = vmax;
        centre = (bounds[0] + bounds[1]) * (1/2.);
    }

    ~cube() override = default;

    Material& getMaterial() override {
        return material;
    }

    Vec3d getNvec(Vec3d &hit) const override {
        Vec3d p = hit - centre;
        Vec3d d = (bounds[0] - bounds[1]) * 0.5;
        double bias = 1.0001;
        auto a = double(int(p.x / fabs(d.x) * bias));
        auto b = double(int(p.y / fabs(d.y) * bias));
        auto c = double(int(p.z / fabs(d.z) * bias));
        return  Vec3d(a, b, c).normalize();
    }

    bool ray_intersect(const Vec3d &orig, const Vec3d &dir, double &t0) const override {
        Vec3d invdir = 1.0f / dir;
        Vec3d sign((double)(invdir.x < 0), (double)(invdir.y < 0), (double)(invdir.z < 0));

        auto tmin  = (bounds[    sign[0]].x - orig.x) * invdir.x;
        auto tmax  = (bounds[1 - sign[0]].x - orig.x) * invdir.x;
        auto tymin = (bounds[    sign[1]].y - orig.y) * invdir.y;
        auto tymax = (bounds[1 - sign[1]].y - orig.y) * invdir.y;

        if (tmin > tymax || tymin > tmax) return false;
        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        auto tzmin = (bounds[    sign[2]].z - orig.z) * invdir.z;
        auto tzmax = (bounds[1 - sign[2]].z - orig.z) * invdir.z;

        if ((tmin > tzmax) || (tzmin > tmax)) return false;
        if (tzmin > tmin) tmin = tzmin;
        if (tzmax < tmax) tmax = tzmax;

        t0 = tmin;

        if (t0 < 0) {
            t0 = tmax;
            if (t0 < 0) return false;
        }

        return true;
    }

    Vec3d getColor() const override  {
        return {material.diffuse_color[0], material.diffuse_color[1], material.diffuse_color[2]};
    }
    Vec3d getEmission() const override {
        return {material.albedo[0], material.albedo[1], material.albedo[2]};
    }

};