#include "Triangle.h"

void Triangle::Intersect(Ray* rayInObjectSpace, Object* object, Intersection* intersection) {
    if (!hasNormals) {
        // vertices in world coords
        vec3 vertex1 = *vertices[index1];
        vec3 vertex2 = *vertices[index2];
        vec3 vertex3 = *vertices[index3];

        vec3 N = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
        float D = -glm::dot(N, vertex1);
        float m = glm::dot(N, rayInObjectSpace->direction);
        if (m == 0) {
            // no intersection because ray parallel to plane
        }
        else {
            float t = -(glm::dot(N, rayInObjectSpace->origin) + D) / m;
            if (t < 0) {
                // no intersection because ray goes away from triange plane
            }
            else {
                vec3 Phit = rayInObjectSpace->origin + t * rayInObjectSpace->direction;
                vec3 edge1 = vertex2 - vertex1;
                vec3 edge2 = vertex3 - vertex2;
                vec3 edge3 = vertex1 - vertex3;
                vec3 c1 = Phit - vertex1;
                vec3 c2 = Phit - vertex2;
                vec3 c3 = Phit - vertex3;
                if (glm::dot(N, glm::cross(edge1, c1)) >= 0
                    && glm::dot(N, glm::cross(edge2, c2)) >= 0
                    && glm::dot(N, glm::cross(edge3, c3)) >= 0) {
                    intersection->isHit = true;
                    intersection->hitVector = Phit;
                    intersection->hitNormal = N;
                }
            }
        }

    } // TODO: handle case with normals as well
}