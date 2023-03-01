#pragma once

#include "Const.h"

// Data class which contains ray-box intersection metadata such as whether a hit happened, hit vector, distance of ray, etc.
struct BoxIntersection {
    bool isHit;
    vec3 hitVector;
    float tStart;
    float tEnd;
    float distance;
};
