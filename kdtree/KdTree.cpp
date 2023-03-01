#include "KdTree.h"
#include <algorithm>
#include<iterator>
#include <glm/gtx/matrix_decompose.hpp>
#include "../AMath.h"
#include <vector>
#include "../BoxUtils.h"

// Recursively constructs a KdTreeNode hierarchy.
// On each recursive level it sorts the objects along the [depth % 3] axis, splits them through the middle and repeats.
// Only leaves contain objects. 
KdTreeNode* KdTree::constructTree(std::vector<Object*>& objects, int depth) {
	KdTreeNode* node = new KdTreeNode();
	node->box = CalculateBoxForObjects(objects);

	if (objects.size() < MAX_PRIMITIVES_PER_LEAF) {
		node->primitives = std::move(objects);
		return node;
	}

	node->axis = depth % 3;

	std::sort(objects.begin(), objects.end(), [node](const Object* a, const Object* b) {
		return IsLessThan(a->axisAlignedBoundingBox->center()[node->axis], b->axisAlignedBoundingBox->center()[node->axis]);
	});

	int mid = objects.size() / 2;
	node->split = mid;

	auto vec1 = std::vector<Object*>(objects.begin(), objects.begin() + mid);
	node->left = constructTree(vec1, depth + 1);
	auto vec2 = std::vector<Object*>(objects.begin() + mid, objects.end());
	node->right = constructTree(vec2, depth + 1);

	return node;
}

// Populates a list of objects that are potential hit targets of the ray.
// Depending on the direction of the ray, we traverse either the left node first and the right second, or vice-versa.
// Note that only leaves contain objects.
// Also note that the ray is in world space.
// TODO: Probably better to do the object intersection here to return earlier and make use of the traversal order
void KdTree::traverse(KdTreeNode& node, Ray& ray, std::vector<Object*>& result) {
	BoxIntersection boxIntersection;
	node.box.Intersect(ray, boxIntersection);
	if (!boxIntersection.isHit) {
		return;
	}

	for (Object* object : node.primitives) {
		result.push_back(object);
	}

	if (ray.direction[node.axis] < 0) {
		if (node.left) {
			traverse(*node.left, ray, result);
		}
		if (node.right) {
			traverse(*node.right, ray, result);
		}
	} else {
		if (node.right) {
			traverse(*node.right, ray, result);
		}
		if (node.left) {
			traverse(*node.left, ray, result);
		}
	}
}