#pragma once

#include "Const.h"

// Data class which describes the properties of a light object, such as intensity, position, color, etc.  
class Light {
public:
    float intensity = 1; // between 0 and 1
    vec4 position; //xyzw
    vec4 color; //rgba
    vec4 lighttransf;  // Light transformed by modelview
    vec3 attenuation;
};