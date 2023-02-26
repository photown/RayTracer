#pragma once

#include <vector>
#include "KdTreeNode.h"
#include "../objects/Object.h"

class KdTree {
public:
	const static int MAX_TRIANGLES_PER_LEAF = 5;

	static KdTreeNode* constructTree(std::vector<Object*>& objects, int depth);
	
	static bool traverse(KdTreeNode& node, Ray& ray, std::vector<Object*>& result);

};