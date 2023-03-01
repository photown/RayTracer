#pragma once

#include "Object.h"
#include "../RayIntersection.h"
#include "../Box.h"

class Sphere : public Object {
public:
    float radius;
    vec3 center;
    
    Sphere();

    void Intersect(Ray& rayInObjectSpace, RayIntersection& intersection);

    Box* CalculateBoundingBox(mat4& transform);
};