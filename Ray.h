#pragma once

#include "Const.h"

// Data class which specifies ray origin and direction
struct Ray {
    vec3 origin;
    vec3 direction;

    Ray(vec3 origin, vec3 direction);
};