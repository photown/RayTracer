#pragma once

#include "Const.h"

struct BoxIntersection {
    bool isHit;
    vec3 hitVector;
    float tStart;
    float tEnd;
    float distance;
};
