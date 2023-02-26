#pragma once

#include "Object.h"
#include "../Intersection.h"
#include "../Box.h"

class Sphere : public Object {
public:
    float radius;
    vec3 center;
    
    Sphere();

    void Intersect(Ray& rayInObjectSpace, Object& object, Intersection& intersection);

    Box* CalculateBoundingBox(mat4& transform);
};