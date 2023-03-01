#include "Triangle.h"
#include <limits>

Triangle::Triangle(int index1, int index2, int index3, std::vector<vec3*>& vertices, bool hasNormals) {
    this->index1 = index1;
    this->index2 = index2;
    this->index3 = index3;
    this->vertices = &vertices;
    this->hasNormals = hasNormals;
}

Box* Triangle::CalculateBoundingBox(mat4& transform) {
    vec3 bottomLeftBack = 
        vec3(
            std::numeric_limits<float>::max(), 
            std::numeric_limits<float>::max(), 
            std::numeric_limits<float>::max());
    vec3 topRightFront = 
        vec3(
            std::numeric_limits<float>::min(),
            std::numeric_limits<float>::min(), 
            std::numeric_limits<float>::min());
    vec3 vertex1 = vec3(transform * vec4(*vertices->at(index1), 1.0f));
    vec3 vertex2 = vec3(transform * vec4(*vertices->at(index2), 1.0f));
    vec3 vertex3 = vec3(transform * vec4(*vertices->at(index3), 1.0f));

    for (int i = 0; i < 3; i++) {
        if (vertex1[i] < bottomLeftBack[i]) {
            bottomLeftBack[i] = vertex1[i];
        }
        if (vertex2[i] < bottomLeftBack[i]) {
            bottomLeftBack[i] = vertex2[i];
        }
        if (vertex3[i] < bottomLeftBack[i]) {
            bottomLeftBack[i] = vertex3[i];
        }

        if (vertex1[i] > topRightFront[i]) {
            topRightFront[i] = vertex1[i];
        }
        if (vertex2[i] > topRightFront[i]) {
            topRightFront[i] = vertex2[i];
        }
        if (vertex3[i] > topRightFront[i]) {
            topRightFront[i] = vertex3[i];
        }
    }
    return new Box(bottomLeftBack, topRightFront);
}

void Triangle::Intersect(Ray& rayInObjectSpace, RayIntersection& intersection) {
    if (!hasNormals) {
        // vertices in object coords
        vec3 vertex1 = *vertices->at(index1);
        vec3 vertex2 = *vertices->at(index2);
        vec3 vertex3 = *vertices->at(index3);

        vec3 N = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
        float D = -glm::dot(N, vertex1);
        float m = glm::dot(N, rayInObjectSpace.direction);
        if (m == 0) {
            // no intersection because ray parallel to plane
        }
        else {
            float t = -(glm::dot(N, rayInObjectSpace.origin) + D) / m;
            if (t < 0) {
                // no intersection because ray goes away from triange plane
            }
            else {
                vec3 Phit = rayInObjectSpace.origin + t * rayInObjectSpace.direction;
                vec3 edge1 = vertex2 - vertex1;
                vec3 edge2 = vertex3 - vertex2;
                vec3 edge3 = vertex1 - vertex3;
                vec3 c1 = Phit - vertex1;
                vec3 c2 = Phit - vertex2;
                vec3 c3 = Phit - vertex3;
                if (glm::dot(N, glm::cross(edge1, c1)) >= 0
                    && glm::dot(N, glm::cross(edge2, c2)) >= 0
                    && glm::dot(N, glm::cross(edge3, c3)) >= 0) {
                    intersection.isHit = true;
                    intersection.hitVector = Phit;
                    intersection.hitNormal = N;
                }
            }
        }

    } // TODO: handle case with normals as well
}