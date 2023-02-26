#pragma once

#include <vector>
#include "../objects/Object.h"
#include "../Ray.h"
#include "../Intersection.h"

class KdTreeNode {
public:
	Box box = Box(vec3(), vec3());
	KdTreeNode* left;
	KdTreeNode* right;
	std::vector<Object*> triangles;
	int axis;
	int split;
};