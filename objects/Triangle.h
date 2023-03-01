#pragma once

#include "Object.h"
#include "../Box.h"
#include "../RayIntersection.h"
#include <vector>

class Triangle : public Object {
public:
    int index1;
    int index2;
    int index3;
    std::vector<vec3*>* vertices;
    bool hasNormals;

    Triangle(int index1, int index2, int index3, std::vector<vec3*>& vertices, bool hasNormal);

    void Intersect(Ray& rayInObjectSpace, RayIntersection& intersection);

    Box* CalculateBoundingBox(mat4& transform);
};