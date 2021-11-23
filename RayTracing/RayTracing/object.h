#pragma once
#include "geometry.h"
struct Material
{
    Vec2f albedo; // difuzni i spekularni koeficijenti refleksije
    Vec3f diffuse_color;
    float specular_exponent;

    Material(const Vec2f& a, const Vec3f& color, const float& coef) : albedo(a), diffuse_color(color), specular_exponent(coef) {}
    Material() : albedo(Vec2f(1, 0)), diffuse_color(), specular_exponent(1.f) {}
};

struct Object
{
    Material material;
    virtual bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const = 0;
    virtual Vec3f normal(const Vec3f& p) const = 0;
};