#pragma once

#include "Object.h"
#include "Const.h"

struct Object;

struct Intersection {
    bool isHit;
    Object* object;
    vec3 hitVector;
    vec3 hitNormal;
    vec3 rayDir;
};