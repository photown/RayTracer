#pragma once

#include "Camera.h"
#include <vector>
#include "Intersection.h"
#include "Ray.h"
#include "Light.h"
#include "World.h"
#include "objects/Object.h"
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <cmath>

class Raytracer
{
public:
    /**
     * Casts a {@link Ray} for each pixel from the {@link World}'s camera towards each pixel on the "screen", from (0, 0) to (width, height), and gathers its color information.
     * 
     * Takes into consideration the objects in the {@link World}, their material properties, the {@link World} lights and reflections.
     *
     * @return a 3 * width * height array containing the R, G and B color components for each pixel.
     */
    unsigned char* Raytrace(World& world, int width, int height);
private:
    /**
     * Constructs a {@link Ray} in world coordinates from the {@link Camera} through the "screen"'s (x, y) point.
     */
    Ray RayThruPixel(Camera& camera, int x, int y, int width, int height);

    /**
     * Calculates the light attenuation using the following formula:
     * 
     * attenuation = intensity / (const + linear * distance + quadratic * (distance^2))
     * 
     * Where (const, linear, quadratic) are the attennuation components, distance is the distance from the given point to the light, and intensity is the light intensity between [0, 1].
     * 
     * The idea is that light becomes weaker the further away it is. Point lights have attennuation components (0, 0, 1), whereas directional lights are not affected and have attenuation components (1, 0, 0).
     */
    float getAttenuation(float intensity, float distance, vec3& attenuationComponents);

    /**
     * Populates an {@link Intersection} object with intersection data from shooting a given {@link Ray} towards {@code objects}, in world space.
     * 
     * <p>To acommodate object transformations, the ray is transformed into object space, shot through the object, and the hit vector and normal are transformed back into world space.
     */
    void Intersect(Ray& ray, std::vector<Object*>& objects, Intersection& result);

    /**
     * Recursively determines the color for a given {@link Intersection} considering other {@link World} data, with recursion depth determined by the depth parameter.
     * 
     * This method uses the light equation:
     * 
     * I = A + E + Sum(Vi * Li / (c0 + c1*r + c2*r^2) * (D * max(dot(N, L), 0) + S * max(dot(S, H), 0) ^ S) + S * Ir
     * 
     * Where:
     * I is the final intensity
     * A is the ambient term
     * E is the self-emission
     * Vi is the visibility term - a binary 0 or 1 depending on whether the point has direct line of sight to light
     * Li is the color * light intensity
     * D is the diffuse
     * S is the specular
     * c0, c1, c2 are the const, linear and quadratic atennuation components
     * r is the distance from the given point to the light source
     * N is the surface normal
     * L is the direction towards the light
     * H is the half-angle (see https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model#Description)
     * 
     * Ir is a recursively calculated light intensity from a ray shot in the mirror direction with the hit vector as the origin.
     *
     * The mirror direction is calculated with the following formula:
     * 
     * mirrorDirection = originalDirection - 2 * dot(originalDirection, originalNormal) * originalNormal
     */
    void getColor(World& world, vec4& result, Intersection& intersection, int depth);
};