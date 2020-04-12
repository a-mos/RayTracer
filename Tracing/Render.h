#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <omp.h>
#include <random>
#include "Utils/Vectors.h"
#include "Utils/Material.h"
#include "Objects/Object.h"
#include "Objects/Light.h"
#include "ToneMapping.h"

std::vector<Vec3f> envmap;
int envmap_w, envmap_h;
bool USE_ENV_MAP = false;
int MAX_DEPTH = 4;
int CHECKERBOARD_TYPE = 1;


template<typename T>
void save_img(std::string &name, int width, int height, std::vector<T> &framebuffer) {
    auto* pixels = new uint8_t[width * height * 3];
    for (size_t i = 0; i < height * width; ++i) {
        T &c = framebuffer[i];
        float max = std::max(c[0], std::max(c[1], c[2]));
        if (max > 1) c = c * (1. / max);
        for (size_t j = 0; j < 3; j++) {
            pixels[i * 3 + j] = (char) (255 * clamp(framebuffer[i][j], 0.0, 1.0));
        }
    }
    stbi_write_bmp(name.c_str(), width, height, 3, pixels);
    delete[]  pixels;
}


Vec3f refract(const Vec3f &I, const Vec3f &N, const float eta_t, const float eta_i = 1.f) {
    float cosi = -std::max(-1.f, std::min(1.f, I * N));
    if (cosi < 0) {
        // inside the object
        return refract(I, -N, eta_i, eta_t);
    }
    float eta = eta_i / eta_t;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vec3f(1, 0, 0) : I * eta + N * (eta * cosi - sqrtf(k)); // total reflection
}

template<typename T>
T reflect(const T &I, const T &N) {
    return I - N * 2.f * (I * N);
}

bool scene_intersect(const Vec3f &orig, const Vec3f &dir, const std::vector<Object*> &objects, Vec3f &hit, Vec3f &N, Material &material) {
    float object_dist = std::numeric_limits<float>::max();
    float checkerboard_dist = object_dist;
    for (auto object : objects) {
        float dist_i;
        if (object->ray_intersect(orig, dir, dist_i) && dist_i < object_dist) {
            object_dist = dist_i;
            hit = orig + dir * dist_i;
            N = object->getNvec(hit);
            material = object->getMaterial();
        }
    }

    switch (CHECKERBOARD_TYPE) {
        case 1: {
            if (fabs(dir.y) > 1e-5) {
                float d = -(orig.y + 5.99) / dir.y;
                Vec3f P = orig + dir * d;
                if (d > 0 && fabs(P.x) < 11 && P.z < 0 && P.z > -30 && d < object_dist) {
                    checkerboard_dist = d;
                    N = Vec3f(0, 1, 0);
                    hit = P;
                    material.diffuse_color = Vec3f(41.0, 80.0, 46.0) * (1 / 255.);
                    material.specular_exponent = 10;
                    material.albedo = Vec4f(Vec4f(0.4, 0.1, 0.0, 0.0));
                }
            }
            break;
        }
        case 2: {
            if (fabs(dir.y) > 1e-5) {
                float d = -(orig.y + 4) / dir.y;
                Vec3f P = orig + dir * d;
                if (d > 0 && fabs(P.x) < 100 && P.z < 0 && d < object_dist) {
                    checkerboard_dist = d;
                    N = Vec3f(0, 1, 0);
                    hit = P;
                    material.diffuse_color = (int(P.x * 0.5 + 1000) + int(P.z * 0.5)) & 1 ? Vec3f(0.0, 0.0, 0.0) : Vec3f(1.0, 1.0, 1.0);
                    material.albedo = Vec4f(Vec4f(0.8, 0.1, 0.5, 0.0));
                }
            }
            break;
        }
        default:
            break;
    }
    return std::min(object_dist, checkerboard_dist) < 1000;
}

Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir, const std::vector<Object *> &objects, const std::vector<Light> &lights, size_t depth = 0) {
    Material material;
    Vec3f point, N;
    if (depth > MAX_DEPTH || !scene_intersect(orig, dir, objects, point, N, material)) {
        if (USE_ENV_MAP) {
            int xx = clamp(static_cast<int>((atan2(dir.z, dir.x) / (2 * M_PI) + 0.5) * envmap_w), 0, envmap_w - 1);
            int yy = clamp(static_cast<int>(acos(dir.y) / M_PI * envmap_h), 0, envmap_h - 1);
            return envmap[xx + yy * envmap_w];
        } else if (CHECKERBOARD_TYPE == 3) {
            return Vec3f(149/255., 128/255., 193/255.); // background
        } else {
            return Vec3f(105/255., 165/255., 205/255.); // background
        }
    }

    Vec3f reflect_dir = reflect(dir, N).normalize();
    Vec3f refract_dir = refract(dir, N, material.refractive_index).normalize();

    // Avoiding self crossing
    Vec3f reflect_orig = reflect_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
    Vec3f refract_orig = refract_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;

    Vec3f reflect_color = cast_ray(reflect_orig, reflect_dir, objects, lights, depth + 1);
    Vec3f refract_color = cast_ray(refract_orig, refract_dir, objects, lights, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;

    for (auto light : lights) {
        Vec3f light_dir = (light.position - point).normalize();
        float light_distance = (light.position - point).norm();
        Vec3f shadow_orig = light_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
        Vec3f shadow_pt, shadow_N;
        Material tmp;
        if (scene_intersect(shadow_orig, light_dir, objects, shadow_pt, shadow_N, tmp) &&
            (shadow_pt - shadow_orig).norm() < light_distance)
            continue;

        diffuse_light_intensity += light.intensity * std::max(0.f, light_dir * N);
        specular_light_intensity += powf(std::max(0.f, reflect(light_dir, N) * dir), material.specular_exponent) * light.intensity;
    }

    return material.diffuse_color * diffuse_light_intensity * material.albedo[0] +
           Vec3f(1., 1., 1.) * specular_light_intensity * material.albedo[1] +
           reflect_color * material.albedo[2] +
           refract_color * material.albedo[3];
}


void render(std::string &name, int width, int height, const std::vector<Object *> &objects, const std::vector<Light> &lights, int MDepth = 4, int board_type = 1, bool USE_TONE_MAPPING = false, const char *env_map_path = nullptr, int AA_Samles = 1) {
    const int fov = M_PI / 2.;
    MAX_DEPTH = MDepth;
    CHECKERBOARD_TYPE = board_type;
    std::vector<Vec3f> framebuffer(width * height);

    std::random_device rd;
    std::mt19937 gen(rd());
    float val = AA_Samles == 1 ? 0.0 : 0.5;
    std::uniform_real_distribution<> dis(0.0, val);

    if (env_map_path != nullptr) {
        USE_ENV_MAP = true;
        int channels;
        unsigned char *pixmap = stbi_load(env_map_path, &envmap_w, &envmap_h, &channels, 0);
        if (!pixmap) {
            std::cerr << "Check path!" << std::endl;
        }
        envmap = std::vector<Vec3f>(envmap_w * envmap_h);
        for (int j = envmap_h-1; j>=0 ; --j) {
            for (int i = 0; i<envmap_w; ++i) {
                envmap[i+j*envmap_w] = Vec3f(
                        pixmap[(i+j*envmap_w)*3+0],
                        pixmap[(i+j*envmap_w)*3+1],
                        pixmap[(i+j*envmap_w)*3+2]) * (1/255.);
            }
        }
        stbi_image_free(pixmap);
    }

    #pragma omp parallel for
    for (size_t j = 0; j < height; ++j) {
        for (size_t i = 0; i < width; ++i) {
            Vec3f Sum;
            for (size_t AA = 0; AA < AA_Samles; ++AA) {
                float dir_x = (i + 0.5 + dis(gen)) - width / 2.;
                float dir_y = -(j + 0.5 + dis(gen)) + height / 2.;
                float dir_z = -height / (2. * tan(fov / 2.));
                Sum = Sum + cast_ray(Vec3f(0, 0, 0), Vec3f(dir_x, dir_y, dir_z).normalize(), objects, lights);
                //std::cout << "Done:" << (float)(j * width + i) / float(width*height) << std::endl;
            }
            framebuffer[j * width + i] = Sum * (1. / AA_Samles);
        }
    }
    if (USE_TONE_MAPPING) {
        ToneMapping(height, width, framebuffer);
    }
    save_img(name, width, height, framebuffer);
}