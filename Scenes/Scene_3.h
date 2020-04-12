#pragma once
#include "Objects/Light.h"
#include "Objects/Quadratic.h"
#include "Objects/Fractals.h"
#include "Tracing/Render.h"

void scene_3(std::string &name) {
    std::vector<Object *> objects;

    auto *S1 = new Quadratic(1, 0, 1.5, 1, -1, 0, 0, 0, 10, Vec3f(2, 2, -10), Materials::fractal);
    objects.push_back(S1);

    auto *S2 = new Quadratic(1, 0, 1.5, 1, 1, 0, 0, 0, 10, Vec3f(-2, 2, -10), Materials::fractal);
    objects.push_back(S2);

    auto *S3 = new Quadratic(1, 1.5, -1, 1, 0, 0, 0, -1, 10, Vec3f(0, 2, -10), Materials::fractal);
    objects.push_back(S3);

    auto *S4 = new Fractal(Vec3f(1., -1.0, -1.), 0.5, Materials::fractal);
    objects.push_back(S4);

    std::vector<Light> lights;
    lights.emplace_back(Vec3f(0, 0, 0), 1.7);

    render(name, 768, 512, objects, lights, 2, 3, true);
    for (auto i: objects) {
        delete i;
    }
}