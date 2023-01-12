#pragma once

#include "Camera.h"
#include <vector>
#include "Intersection.h"
#include "Ray.h"
#include "Object.h"
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <cmath>
#include "variables.h"

class Raytracer
{
public:
    unsigned char* Raytrace(Camera* camera, std::vector<Object*> objects, int width, int height);
private:
    vec4* lighttemp = new vec4();

    Ray* RayThruPixel(Camera* camera, int x, int y);

    float getAttenuation(float intensity, float distance, vec3 attenuation);

    void Intersect(Ray* ray, std::vector<Object*> objects, Intersection* result);

    void getColor(Camera* camera, std::vector<Object*> objects, std::vector<Light*> lights, vec4* result, Intersection* intersection, int depth);
};