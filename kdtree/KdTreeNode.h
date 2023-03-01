#pragma once

#include <vector>
#include "../objects/Object.h"
#include "../Ray.h"
#include "../RayIntersection.h"

class KdTreeNode {
public:
	// Axis-aligned bounding box that encapsulates all the objects inside the subtree of this node
	Box box = Box(vec3(), vec3());
	
	// The left child of the node
	KdTreeNode* left;

	// The right child of the node
	KdTreeNode* right;

	// List of primitives; only populated for leaf nodes
	std::vector<Object*> primitives;

	// The axis, X, Y or Z, along which the split is done
	int axis;

	// The index of the object where the split into two subnodes is made
	int split;
};