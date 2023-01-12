#pragma once

#include "Const.h"

struct Ray {
    vec3 origin;
    vec3 direction;

    Ray(vec3 origin, vec3 direction) {
        this->origin = origin;
        this->direction = direction;
    }
};