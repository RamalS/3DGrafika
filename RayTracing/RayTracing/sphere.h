#pragma once
#include "geometry.h"
#include "object.h"
struct Sphere : Object
{
    Vec3f c; // centar
    float r; // radius

    Sphere(const Vec3f& c, const float& r, const Material& m) : c(c), r(r)
    {
        Object::material = m;
    }

    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const
    {
        Vec3f v = c - p; // vektor izmedju izvora zrake i centra

        if (d * v < 0) // skalarni produkt
        {
            // sfera se nalazi iza zrake
            return false;
        }
        else
        {
            // izracunaj projekciju
            Vec3f pc = p + d * ((d * v) / d.norm());
            if ((c - pc) * (c - pc) > r * r)
            {
                // nema sjeciste
                return false;
            }
            else
            {
                float dist = sqrt(r * r - (c - pc) * (c - pc));

                if (v * v > r * r) // izvor pogleda izvan sfere
                {
                    t = (pc - p).norm() - dist;
                }
                else // izvor pogleda unutar sfere
                {
                    t = (pc - p).norm() + dist;
                }

                return true;
            }
        }
    }

    Vec3f normal(const Vec3f& p) const
    {
        return (p - c).normalize();
    }
};