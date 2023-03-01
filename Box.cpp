#include "Box.h"
#include <limits>
#include <utility>
#include "AMath.h"

struct Object;

Box::Box(vec3 bottomLeftBack, vec3 topRightFront) {
	this->bottomLeftBack = bottomLeftBack;
	this->topRightFront = topRightFront;
}

void Box::ExpandToInclude(Box& box) {
	this->bottomLeftBack = vec3(
		std::min(this->bottomLeftBack.x, box.bottomLeftBack.x),
		std::min(this->bottomLeftBack.y, box.bottomLeftBack.y),
		std::min(this->bottomLeftBack.z, box.bottomLeftBack.z));
	this->topRightFront = vec3(
		std::max(this->topRightFront.x, box.topRightFront.x),
		std::max(this->topRightFront.y, box.topRightFront.y),
		std::max(this->topRightFront.z, box.topRightFront.z));
}

vec3 Box::center() {
	return (this->topRightFront + this->bottomLeftBack) * 0.5f;
}

// Checks whether the given ray intersects the box.
void Box::Intersect(Ray& ray, BoxIntersection& result) {
	result.isHit = false;
	float tStart = std::numeric_limits<float>::min();
	float tEnd = std::numeric_limits<float>::max();

	for (int i = 0; i < 3; i++) {
		if (ray.direction[i] == 0
			&& (IsLessThan(ray.origin[i], bottomLeftBack[i]) || IsGreaterThan(ray.origin[i], topRightFront[i]))) {
			return;
		}
	
		float t1 = (bottomLeftBack[i] - ray.origin[i]) / ray.direction[i];
		float t2 = (topRightFront[i] - ray.origin[i]) / ray.direction[i];
		if (IsGreaterThan(t1, t2)) {
			std::swap(t1, t2);
		}
		if (IsGreaterThan(t1, tStart)) {
			tStart = t1;
		}
		if (IsLessThan(t2, tEnd)) {
			tEnd = t2;
		}
	}

	float tMin = 0;
	if (IsGreaterThan(tStart, tEnd)) {
		// no intersection
		return;
	}
	if (IsLessThan(tEnd, tMin)) {
		// box behind eye
		return;
	}
	float t;
	if (IsGreaterThan(tStart, tMin)) {
		t = tStart;
	}
	else {
		// eye inside box
		t = tEnd;
	}
	result.tStart = tStart;
	result.tEnd = tEnd;
	result.distance = t;
	result.isHit = true;
	result.hitVector = ray.origin + ray.direction * t;
}