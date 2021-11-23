#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "geometry.h"
#include "object.h"
#include "light.h"
#include "raytracing.h"
#include "cuboid.h"
#include "sphere.h"
#include "cylinder.h"

using namespace std;


int main()
{
    // definiranje materijala
    Material red = Material(Vec2f(0.6, 0.3), Vec3f(1, 0, 0), 60);
    Material green = Material(Vec2f(0.6, 0.3), Vec3f(0, 0.5, 0), 60);
    Material blue = Material(Vec2f(0.9, 0.1), Vec3f(0, 0, 1), 10);
    Material gray = Material(Vec2f(0.9, 0.1), Vec3f(0.5, 0.5, 0.5), 10);


    Cuboid surface(Vec3f(-25, -5.1, -30), Vec3f(25, -5, -9), gray);

    Sphere sphere(Vec3f(-1.0, -3.5, -12), 1.5, green);
    Cuboid cuboid(Vec3f(0, -7,  -15), Vec3f(3, -2,  -16), blue);
    Cylinder cylinder(Vec3f(-6, -5, -13), 2, 4, red);

    // definiraj objekte u sceni
    Objects objs = { &surface, &sphere, &cuboid, &cylinder };

    // definiraj svjetla
    Light l1 = Light(Vec3f(-20, 20, 20), 1);
    Light l2 = Light(Vec3f(20, 30, 20), 1.5);
    Lights lights = { l1, l2 };

    //render(objs, lights);
    render_consol(objs, lights);

    return 0;
}