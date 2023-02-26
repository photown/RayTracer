#pragma once

#include "Const.h"
#include "Ray.h"
#include "BoxIntersection.h"

class Box {
public:
	vec3 bottomLeftBack;
	vec3 topRightFront;

	Box(vec3 bottomLeftBack, vec3 topRightFront);

	vec3 center();

	virtual void Intersect(Ray& ray, BoxIntersection& result);

	static Box Union(Box& box1, Box& box2);
};