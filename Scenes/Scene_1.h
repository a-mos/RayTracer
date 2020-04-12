#pragma once
#include "Objects/Light.h"
#include "Objects/Sphere.h"
#include "Objects/Cube.h"
#include "Tracing/Render.h"

void scene_1(std::string &name) {
    std::vector<Object *> objects;
    float sphere_r = 0.6;
    float bias = -16;

    auto *S1 = new Sphere(Vec3f(0, -6 + sphere_r, -11.5), sphere_r * 0.9, Materials::white);
    objects.push_back(S1);

    auto *S2 = new Sphere(Vec3f(0, -6 + sphere_r, bias), sphere_r, Materials::yellow);
    objects.push_back(S2);

    auto *S3 = new Sphere(Vec3f(-sphere_r, -6 + sphere_r, bias - 2 * sphere_r), sphere_r, Materials::green);
    objects.push_back(S3);

    auto *S4 = new Sphere(Vec3f(sphere_r, -6 + sphere_r, bias - 2 * sphere_r), sphere_r, Materials::shiraz);
    objects.push_back(S4);

    auto *S5 = new Sphere(Vec3f(- 2 * sphere_r, -6 + sphere_r, bias - 4 * sphere_r), sphere_r, Materials::thunderbird);
    objects.push_back(S5);

    auto *S6 = new Sphere(Vec3f(0, -6 + sphere_r, bias - 4 * sphere_r), sphere_r, Materials::black);
    objects.push_back(S6);

    auto *S7 = new Sphere(Vec3f(2 * sphere_r, -6 + sphere_r, bias - 4 * sphere_r), sphere_r, Materials::violet);
    objects.push_back(S7);

    auto *S8 = new Sphere(Vec3f(-3 * sphere_r, -6 + sphere_r, bias - 6 * sphere_r), sphere_r, Materials::orange);
    objects.push_back(S8);

    auto *S9 = new Sphere(Vec3f(-sphere_r, -6 + sphere_r, bias - 6 * sphere_r), sphere_r, Materials::blue);
    objects.push_back(S9);

    auto *S10 = new Sphere(Vec3f(sphere_r, -6 + sphere_r, bias - 6 * sphere_r), sphere_r, Materials::shiraz);
    objects.push_back(S10);

    auto *S11 = new Sphere(Vec3f(3 * sphere_r, -6 + sphere_r, bias - 6 * sphere_r), sphere_r, Materials::green);
    objects.push_back(S11);

    auto *S12 = new Sphere(Vec3f(-4 * sphere_r, -6 + sphere_r, bias - 8 * sphere_r), sphere_r, Materials::blue);
    objects.push_back(S12);

    auto *S13 = new Sphere(Vec3f(-2 * sphere_r, -6 + sphere_r, bias - 8 * sphere_r), sphere_r, Materials::yellow);
    objects.push_back(S13);

    auto *S14 = new Sphere(Vec3f(0, -6 + sphere_r, bias - 8 * sphere_r), sphere_r, Materials::violet);
    objects.push_back(S14);

    auto *S15 = new Sphere(Vec3f(2 * sphere_r, -6 + sphere_r, bias - 8 * sphere_r), sphere_r, Materials::orange);
    objects.push_back(S15);

    auto *S16 = new Sphere(Vec3f(4 * sphere_r, -6 + sphere_r, bias - 8 * sphere_r), sphere_r, Materials::thunderbird);
    objects.push_back(S16);

    auto *S17 = new Sphere(Vec3f(2.55, 7.05, -15), 3, Materials::mirror);
    objects.push_back(S17);

    auto *O1 = new Cube(Vec3f(-11, -6, -30), Vec3f(-10, -5, 0), Materials::wood);
    objects.push_back(O1);

    auto *O2 = new Cube(Vec3f(10, -6, -30), Vec3f(11, -5, 0), Materials::wood);
    objects.push_back(O2);

    auto *O3 = new Cube(Vec3f(-11, -6, -31), Vec3f(11, -5, -30), Materials::wood);
    objects.push_back(O3);

    auto *O4 = new Cube(Vec3f(-11, -6, -1), Vec3f(11, -5, 0), Materials::wood);
    objects.push_back(O4);

    std::vector<Light> lights;
    lights.emplace_back(Vec3f(10, 8, -25), 1.7);
    lights.emplace_back(Vec3f(-5, 10, -10), 1.5);

    //Render scene with ray-depth=1+1, AA with 4 samples
    render(name, 1280, 720, objects, lights, 1, 1, false, "../Scenes/Scene_0_ENV.jpg", 4);
    for (auto i: objects) {
        delete i;
    }
}
