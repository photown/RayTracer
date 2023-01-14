#pragma once

#include <vector>
#include "Light.h"
#include "World.h"

struct Config {
    std::string outputLocation;

    int width;
    int height;

    World world;
};

void matransform (stack<mat4> &transfstack, float * values) ;
void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, float * values) ;
Config readfile (const char * filename) ;
