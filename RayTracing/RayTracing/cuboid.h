#pragma once
#include "geometry.h"
#include "object.h"

using namespace std;

struct Cuboid : Object
{
    Vec3f c1;
    Vec3f c2;

    Cuboid(const Vec3f& c1, const Vec3f& c2, const Material& m) : c1(c1), c2(c2)
    {
        Object::material = m;
    }

    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const
    {
        float tNear = 0;
        float tFar = 1e6;

        float minX = min(c1.x, c2.x);
        float minY = min(c1.y, c2.y);
        float minZ = min(c1.z, c2.z);
        float maxX = max(c1.x, c2.x);
        float maxY = max(c1.y, c2.y);
        float maxZ = max(c1.z, c2.z);

        if (d.x == 0)
        {
            if (p.x < minX || p.x > maxX)
            {
                return false;
            }
        }
        else
        {
            float t1 = (minX - p.x) / d.x;
            float t2 = (maxX - p.x) / d.x;

            if (t1 > t2)
            {
                swap(t1, t2);
            }

            tNear = max(tNear, t1);
            tFar = min(tFar, t2);
            if (tNear > tFar || tFar < 0)
                return false;
        }

        t = tNear;

        if (d.y == 0)
        {
            if (p.y < minY || p.y > maxY)
            {
                return false;
            }
        }
        else
        {
            float t1 = (minY - p.y) / d.y;
            float t2 = (maxY - p.y) / d.y;

            if (t1 > t2)
            {
                swap(t1, t2);
            }

            tNear = max(tNear, t1);
            tFar = min(tFar, t2);
            if (tNear > tFar || tFar < 0)
                return false;
        }

        t = tNear;

        if (d.z == 0)
        {
            if (p.z < minZ || p.z > maxZ)
            {
                return false;
            }
        }
        else
        {
            float t1 = (minZ - p.z) / d.z;
            float t2 = (maxZ - p.z) / d.z;

            if (t1 > t2)
            {
                swap(t1, t2);
            }

            tNear = max(tNear, t1);
            tFar = min(tFar, t2);

            if (tNear > tFar || tFar < 0)
                return false;
        }

        t = tNear;

        return true;
    }

    Vec3f normal(const Vec3f& p) const
    {
        Vec3f normal;

        if (abs(p.x - c1.x) < 0.01)
            normal = Vec3f(-1, 0, 0);
        else if (abs(p.x - c2.x) < 0.01)
            normal = Vec3f(1, 0, 0);
        else if (abs(p.y - c1.y) < 0.01)
            normal = Vec3f(0, -1, 0);
        else if (abs(p.y - c2.y) < 0.01)
            normal = Vec3f(0, 1, 0);
        else if (abs(p.z - c1.z) < 0.01)
            normal = Vec3f(0, 0, -1);
        else if (abs(p.z - c2.z) < 0.01)
            normal = Vec3f(0, 0, 1);

        return normal;
    }
};