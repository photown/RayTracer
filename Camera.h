#pragma once

#include "Const.h"
#include "Transform.h"

// Data class which describes the properties of a camera, such as eye, up and 
// center coordinates, field of view, transformation matrices, aspect ratio, etc.
class Camera {
public:
    vec3 eye;
    vec3 up;
    vec3 center;
    float fovy;
    mat4 modelview;
    mat3 normal_modelview;
    float aspectRatio;

    Camera(vec3& eye, vec3& up, vec3& center, float fovy);
};