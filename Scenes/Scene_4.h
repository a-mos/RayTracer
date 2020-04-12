#pragma once
#include <random>
#include "../Tracing/Render.h"
#include "../Utils/Material.h"

std::default_random_engine gen;
std::uniform_real_distribution<double> dis_u(0., 1.);
std::normal_distribution<double> dis(0., 0.50);
std::vector<object *> objects;

Vec3d Sample(const Vec3d& N) {
    double r1 = dis_u(gen);
    double r2 = dis_u(gen);
    double phi = 2 * M_PI * r2;
    double x = sqrt(r1) * cos(phi);
    double z = sqrt(r1) * sin(phi);
    Vec3d sample = Vec3d(x, sqrt(1.0 - r1), z).normalize();
    Vec3d Nt, Nb;
    //Avoiding near zero axis-vector
    //Creating Cartesian coords
    if (std::fabs(N.x) > std::fabs(N.y)) {
        Nt = Vec3d(N.z, 0, -N.x) * (1 / sqrt(N.x * N.x + N.z * N.z));
    } else {
        Nt = Vec3d(0, -N.z, N.y) * (1 / sqrt(N.y * N.y + N.z * N.z));
    }
    Nb = cross(N, Nt);
    Vec3d Oriented( sample.x * Nt.x + sample.y * N.x + sample.z * Nb.x,
                    sample.x * Nt.y + sample.y * N.y + sample.z * Nb.y,
                    sample.x * Nt.z + sample.y * N.z + sample.z * Nb.z );
    return Oriented.normalize();
}

void intersect(const Vec3d &orig, const Vec3d &dir, int &id, double &t0) {
    double t;
    for (int i = 0; i < objects.size(); ++i) {
        if (objects[i]->ray_intersect(orig, dir, t) && t < t0) {
            id = i;
            t0 = t;
        }
    }
}

Vec3d radiance(const Vec3d &orig, const Vec3d &dir, size_t depth = 0) {
    int id = -1;
    double t0 = 99999.;
    intersect(orig, dir, id, t0);
    if (id == -1) return {};
    Vec3d P = orig + (dir * t0);
    object *obj = objects[id];
    Vec3d N = obj->getNvec(P);
    if (depth > 2) {
        return obj->getEmission();
    }
    // Diffuse surface
    if (obj->getMaterial().specular_exponent < 0.5) {
        Vec3d sample = Sample(N);
        // 0.01 for avoiding self crossing
        return obj->getEmission() + mul(obj->getColor(), radiance(P + sample * 0.01, sample, depth + 1));
    }
    // Specular reflection
    Vec3d reflected = reflect(dir, N);
    // 0.01 for avoiding self crossing
    return obj->getEmission() + mul(obj->getColor(), radiance(P + reflected * 0.01, reflected, depth + 1));
}


void render(std::string &name) {
    int w = 512, h = 512;
    int samples = 1536;
    Vec3d dz(0, 0, -1);
    Vec3d dx = Vec3d(1, 0, 0);
    Vec3d dy = cross(dx, dz).normalize();
    std::vector<Vec3d> pixels(w * h);
    #pragma omp parallel for
    for (int row = 0; row < h; ++row) {
        for (int col = 0; col < w; ++col) {
            Vec3d res;
            for (int i = 0; i < samples; ++i) {
                //Tracing with priority to central pixel
                Vec3d d = dx*((dis(gen)/2 + col)/w - 0.5) + dy*((dis(gen)/2 + row)/h - 0.5) + dz;
                res = res + radiance(Vec3d(), d.normalize());
            }
            res = res * (1.0 / samples);
            pixels[row * h + col] = Vec3d(res.x, res.y, res.z);
        }
    }
    // Gamma Correction
    for (int i = 0; i < w * h; ++i) {
        for (int j = 0; j < 3; ++j) {
            pixels[i][j] = powf(clamp(pixels[i][j], 0., 1.), 1 / 2.2);
        }
    }
    save_img(name, w, h, pixels);
    for (auto o: objects) {
        delete o;
    }
}

void scene_4(std::string &name) {
    auto *ball1 = new sphere(Vec3d(0, -12, -100), 12,  Materials::ball1); // ball1
    objects.push_back(ball1);
    auto *ball2 = new sphere(Vec3d(0, 30-15, -100), 15,  Materials::ball2); // ball2
    objects.push_back(ball2);
    auto *s3 = new sphere(Vec3d(0, -740, 0), 700,  Materials::light); // light
    objects.push_back(s3);

    auto *bottom = new cube(Vec3d(-35, 30, -150), Vec3d(30, 50, 0), Materials::bottom);
    objects.push_back(bottom);
    auto *left = new cube(Vec3d(-35, -100, -150), Vec3d(-30, 50, 0), Materials::left);
    objects.push_back(left);
    auto *back = new cube(Vec3d(-35, -100, -150), Vec3d(35, 50, -155), Materials::back);
    objects.push_back(back);
    auto *right = new cube(Vec3d(30, -100, -150), Vec3d(35, 50, 0), Materials::right);
    objects.push_back(right);

    render(name);
}
