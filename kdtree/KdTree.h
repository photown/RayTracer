#pragma once

#include <vector>
#include "KdTreeNode.h"
#include "../objects/Object.h"

class KdTree {
public:
	// The maximum number of primitives that a leaf can contain
	const static int MAX_PRIMITIVES_PER_LEAF = 5;

	// Constructs a KdTreeNode hierarchy from the given list of objects
	// The depth is used to determine the axis along which to split the objects
	static KdTreeNode* constructTree(std::vector<Object*>& objects, int depth);
	
	// Populates the `result` list with objects that are likely to intersect the `ray`
	static void traverse(KdTreeNode& node, Ray& ray, std::vector<Object*>& result);

};