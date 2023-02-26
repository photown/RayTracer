#pragma once

#include "Object.h"

Object::Object() {

}

void Object::SetTransform(mat4& transform) {
	this->transform = mat4(transform);
	this->normal_transform = mat3(glm::transpose(glm::inverse(transform)));
	this->transform_inverse = glm::inverse(transform);
	this->axisAlignedBoundingBox = CalculateBoundingBox(this->transform);
}