#pragma once

#include "objects/Object.h"
#include "Const.h"

struct Object;

// Data class which contains ray-primitive intersection metadata such as whether a hit happened, the hit object, hit vector, hit normal, etc.
struct RayIntersection {
    bool isHit;
    Object* object;
    vec3 hitVector;
    vec3 hitNormal;
    vec3 rayDir;
};