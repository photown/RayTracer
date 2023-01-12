#include "Camera.h"

Camera::Camera(vec3 eye, vec3 up, vec3 center, float fovy) {
    this->eye = eye;
    this->up = up;
    this->center = center;
    this->fovy = fovy;
    modelview = Transform::lookAt(eye, center, up);
    normal_modelview = mat3(glm::transpose(glm::inverse(modelview)));
};