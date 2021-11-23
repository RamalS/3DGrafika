#pragma once
#include "geometry.h"
#include "object.h"

using namespace std;

struct Cylinder : Object
{
    Vec3f cen;
    float r;
    float h;

    Cylinder(const Vec3f& cen, const float& r, const float& h, const Material& m) : cen(cen), r(r), h(h)
    {
        Object::material = m;
    }

    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const
    {
        Vec3f v = cen - p;

        if (v * d < 0)
        {
            return false;
        }
        else
        {
            float a = (d.x * d.x) + (d.z * d.z);
            float b = 2 * ((d.x * (p.x - cen.x)) + (d.z * (p.z - cen.z)));
            float c = ((p.x - cen.x) * (p.x - cen.x)) + ((p.z - cen.z) * (p.z - cen.z)) - (r * r);

            float diskr = b * b - 4 * (a * c);


            if (diskr < 0.0)
            {
                return false;
            }
            else
            {
                float t1 = (-b + sqrt(diskr)) / (2 * a);
                float t2 = (-b - sqrt(diskr)) / (2 * a);

                float t3 = min(t1, t2);
                float t4 = max(t1, t2);

                if ((p.y + t3 * d.y >= cen.y) && (p.y + t3 * d.y <= cen.y + h))
                {
                    t = t3;
                    return true;
                }
                else if ((p.y + t4 * d.y >= cen.y) && (p.y + t4 * d.y <= cen.y + h))
                {
                    t = t4;
                    return true;
                }
                else
                    return false;
            }
        }
    }

    Vec3f normal(const Vec3f& p) const
    {
        Vec3f n = (p - cen).normalize();
        n.y = 0;
        return n;
    }
};