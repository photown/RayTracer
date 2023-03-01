#include "Sphere.h"

Sphere::Sphere() {
}

Box* Sphere::CalculateBoundingBox(mat4& transform) {
    vec3 center = vec3(transform * vec4(this->center, 1.0f));
    vec3 bottomLeftBack = center - radius;
    vec3 topRightFront = center + radius;
    return new Box(bottomLeftBack, topRightFront);
}

void Sphere::Intersect(Ray& rayInObjectSpace, RayIntersection& intersection) {
    bool found = false;
    float t;
    vec3 p0 = rayInObjectSpace.origin;
    vec3 p1 = rayInObjectSpace.direction;
    float a = glm::dot(p1, p1);
    vec3 center2 = center; // world coords
    float b = 2 * glm::dot(p1, (p0 - center2));
    float c = glm::dot((p0 - center2), (p0 - center2)) - radius * radius;
    float D = b * b - 4 * a * c;
    if (D > 0) {
        // two roots
        float sqrtD = glm::sqrt(D);
        float root1 = (-b + sqrtD) / (2 * a);
        float root2 = (-b - sqrtD) / (2 * a);
        if (root1 > 0 && root2 > 0) {
            t = glm::min(root1, root2);
            found = true;
        }
        else if (root2 < 0 && root1 >= 0) {
            t = root1;
            found = true;
        }
        else {
            // should not happen, implies sthat both intersection points are behind the ray
        }
    }
    else if (D == 0) {
        // one root
        float root = -b / (2 * a);
        t = root;
        found = true;
    }
    else if (D < 0) {
        // no roots
       // continue;
    }
    if (found) {
        intersection.isHit = true;
        intersection.hitVector = p0 + p1 * t;
        intersection.hitNormal = glm::normalize(intersection.hitVector - center2);
    }
}