#pragma once
#include "Vec.h"
// TODO: Need to implement SoA later when i start rendering and calculating thousands of spheres:
struct Sphere {
    Vec3 center;
    float radius;
    Vec3 material;
    Vec3 velocity;
    float mass;
};