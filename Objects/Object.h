#pragma once
#include "Utils/Vectors.h"
#include "Utils/Material.h"

struct Object {
    Object() = default;
    virtual ~Object() = default;
    virtual bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const = 0;
    virtual Vec3f getNvec(Vec3f &hit) const = 0;
    virtual Material& getMaterial() = 0;
};

struct object {
    object() = default;
    virtual ~object() = default;
    virtual bool ray_intersect(const Vec3d &orig, const Vec3d &dir, double &t0) const = 0;
    virtual Vec3d getNvec(Vec3d &hit) const = 0;
    virtual Vec3d getEmission() const = 0;
    virtual Material& getMaterial() = 0;
    virtual Vec3d getColor() const = 0;
};