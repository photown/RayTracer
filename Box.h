#pragma once

#include "Const.h"
#include "Ray.h"
#include "BoxIntersection.h"
#include <vector>

struct Object;

// Wrapper which describes an axis-aligned bounding box via its bottom left back and top right front points.
class Box {
public:
	vec3 bottomLeftBack;
	vec3 topRightFront;

	Box(vec3 bottomLeftBack, vec3 topRightFront);

	vec3 center();

	// Checks whether the given ray intersects the box.
	virtual void Intersect(Ray& ray, BoxIntersection& result);

	void ExpandToInclude(Box& box);
};