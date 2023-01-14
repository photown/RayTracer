#pragma once

#include "Const.h"
#include "Transform.h"

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