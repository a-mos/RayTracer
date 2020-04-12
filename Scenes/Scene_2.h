#pragma once
#include "Objects/Light.h"
#include "Tracing/Render.h"
#include <Utils/ReadObj.h>

void scene_2(std::string &name) {
    std::vector<Object *> objects;
    auto mesh = MakeFig("../Scenes/teddy.obj", Materials::shiraz, Vec3f(0, 16, -100));

    std::vector<Light> lights;
    lights.emplace_back(Vec3f(-3, 0, -5), 3);

    render(name, 512, 512, mesh, lights, 1, 2, false);
    for (auto i: objects) {
        delete i;
    }
}
