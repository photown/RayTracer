#pragma once

#include "Object.h"
#include "../variables.h"
#include "../Intersection.h"

struct Triangle : Object {
    int index1;
    int index2;
    int index3;
    bool hasNormals;

    Triangle(int index1, int index2, int index3, bool hasNormals) {
        this->index1 = index1;
        this->index2 = index2;
        this->index3 = index3;
        this->hasNormals = hasNormals;
    }

    void Intersect(Ray* rayInObjectSpace, Object* object, Intersection* intersection);
};