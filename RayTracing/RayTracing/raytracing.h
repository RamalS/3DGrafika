#pragma once
#include <fstream>
#include "geometry.h"
#include "object.h"
#include "light.h"
#include <windows.h>

typedef std::vector<Light> Lights;
typedef std::vector<Object*> Objects;
typedef std::vector<Vec3f> Image;

using namespace std;

bool scene_intersect(const Vec3f& orig, const Vec3f& dir, const Objects& objs, Vec3f& hit, Material& material, Vec3f& N)
{
    // inicijalno, pretp. da je predmet daleko
    float dist = 1e6;
    float obj_dist = dist;

    for (auto obj : objs)
    {

        if (obj->ray_intersect(orig, dir, obj_dist) && obj_dist < dist)
        {
            dist = obj_dist;
            hit = orig + dir * obj_dist;
            N = obj->normal(hit);
            material = obj->material;
        }
    }

    // provjeri je li sjecište predaleko
    return dist < 1000;
}

Vec3f reflect(const Vec3f& I, const Vec3f& N)
{
    return I - (N * (2 * (I * N)));
}

// funkcija koja vraca udaljenost sjecista pravca i sfere
Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, const Objects& objs, const Lights& lights, const float& depth)
{
    int maxDepth = 12;
    float reflexivity = 0.3;

    Vec3f hit_point;
    Vec3f hit_normal; // normala na povrsinu
    Material hit_material;

    if (!scene_intersect(orig, dir, objs, hit_point, hit_material, hit_normal) || depth > maxDepth)
    {
        return Vec3f(0.7, 0.9, 0.7); // vrati boju pozadine
    }
    else
    {
        float diffuse_light_intensity = 0;
        float specular_light_intensity = 0;

        for (auto light : lights)
        {
            Vec3f light_dir = (light.position - hit_point).normalize(); // smjer svjetla
            float light_dist = (light.position - hit_point).norm();     // udaljenost do svjetla

            // sjene
            Vec3f shadow_orig;
            Vec3f shadow_hit_point;
            Vec3f shadow_hit_normal;
            Material shadow_material;

            // epsilon pomak od tocke sjecista
            if (light_dir * hit_normal < 0)
            {
                shadow_orig = hit_point - hit_normal * 0.001;
                hit_normal = -hit_normal;
            }
            else
            {
                shadow_orig = hit_point + hit_normal * 0.001;
            }

            if (scene_intersect(shadow_orig, light_dir, objs, shadow_hit_point, shadow_material, shadow_hit_normal) && (shadow_hit_point - shadow_orig).norm() < light_dist)
            {
                continue;
            }

            // sjencanje
            // lambertov model
            diffuse_light_intensity += light.intensity * max(0.f, light_dir * hit_normal);

            // blinn-phongov model
            // smjer pogleda
            Vec3f view_dir = (orig - hit_point).normalize();
            // poluvektor
            Vec3f half_vec = (view_dir + light_dir).normalize();
            specular_light_intensity += light.intensity * powf(max(0.f, half_vec * hit_normal), hit_material.specular_exponent);
        }

        Vec3f R = reflect(dir, hit_normal);

        Vec3f hitColor = hit_material.diffuse_color * hit_material.albedo[0] * diffuse_light_intensity // diffuse dio
            + Vec3f(1, 1, 1) * hit_material.albedo[1] * specular_light_intensity;         // specular dio

        float epsilon = 0.1f;

        return hitColor + cast_ray(hit_point + hit_normal * epsilon, R, objs, lights, depth + 1) * reflexivity;
    }
}


void render(const Objects& objects, const Lights& lights)
{
    const int width = 1024;
    const int height = 768;
    const int fov = 3.14159265358979323846 / 2.0; // pi / 2

    Image buffer(width * height);


    for (size_t j = 0; j < height; ++j)
    {
        for (size_t i = 0; i < width; ++i)
        {
            // pošalji zraku u svaki piksel
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);

            // definiraj smjer
            Vec3f dir = Vec3f(x, y, -1).normalize();

            buffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir, objects, lights, 0);
        }
    }

    std::ofstream ofs;
    ofs.open("./slika.ppm", std::ofstream::binary);
    // oblikuj po ppm formatu
    ofs << "P6\n"
        << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height * width; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            unsigned char color = (unsigned char)(255.f * max(0.f, min(1.f, buffer[i][j])));
            ofs << color;
        }
    }

    ofs.close();
}

void render_consol(const Objects& objects, const Lights& lights)
{
    const int width = 1024;
    const int height = 768;
    const int fov = 3.14159265358979323846 / 2.0;

    Image buffer(width * height);

    for (size_t j = 0; j < height; ++j)
    {
        for (size_t i = 0; i < width; ++i)
        {
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);

            Vec3f dir = Vec3f(x, y, -1).normalize();

            buffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir, objects, lights, 0);
        }
    }


    HDC hdc = GetDC(GetConsoleWindow());
    #pragma omp parallel 
    for (size_t i = 0; i < height; ++i)
    {
        for (size_t j = 0; j < width; ++j)
        {
            auto colr = 255.f * max(0.f, min(1.f, buffer[j + i * width][0]));
            auto colg = 255.f * max(0.f, min(1.f, buffer[j + i * width][1]));
            auto colb = 255.f * max(0.f, min(1.f, buffer[j + i * width][2]));

            SetPixel(hdc, j, i, RGB(colr, colg, colb));
        }
    }
}