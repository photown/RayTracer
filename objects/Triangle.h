#pragma once

#include "Object.h"
//#include "../variables.h"
#include "../Intersection.h"
#include <vector>

class Triangle : public Object {
public:
    int index1;
    int index2;
    int index3;
    std::vector<vec3*> vertices;
    bool hasNormals;

    Triangle(int index1, int index2, int index3, std::vector<vec3*> vertices, bool hasNormals);

    void Intersect(Ray* rayInObjectSpace, Object* object, Intersection* intersection);
};