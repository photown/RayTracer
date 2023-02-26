#include "Box.h"
#include <limits>
#include <utility>
#include "AMath.h"

Box::Box(vec3 bottomLeftBack, vec3 topRightFront) {
	this->bottomLeftBack = bottomLeftBack;
	this->topRightFront = topRightFront;
}

Box Box::Union(Box& box1, Box& box2) {
	return Box(
		vec3(
			std::min(box1.bottomLeftBack.x, box2.bottomLeftBack.x),
			std::min(box1.bottomLeftBack.y, box2.bottomLeftBack.y),
			std::min(box1.bottomLeftBack.z, box2.bottomLeftBack.z)),
		vec3(
			std::min(box1.topRightFront.x, box2.topRightFront.x),
			std::min(box1.topRightFront.y, box2.topRightFront.y),
			std::min(box1.topRightFront.z, box2.topRightFront.z)
		));
}

vec3 Box::center() {
	return (this->topRightFront + this->bottomLeftBack) * 0.5f;
}

void Box::Intersect(Ray& ray, BoxIntersection& result) {
	result.isHit = false;
	float tStart = std::numeric_limits<float>::min();
	float tEnd = std::numeric_limits<float>::max();

	// TODO: maybe swap topLeft/bottomRight orientation based on ray directi
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