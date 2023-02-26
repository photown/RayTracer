#include "KdTree.h"
#include <algorithm>
#include<iterator>
#include <glm/gtx/matrix_decompose.hpp>
#include "../AMath.h"
#include <vector>

Box ComputeBoundingBox(std::vector<Object*>& objects) {
	Box box = Box(
		vec3(
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max()),
		vec3(
			std::numeric_limits<float>::min(),
			std::numeric_limits<float>::min(),
			std::numeric_limits<float>::min()));

	for (auto object : objects) {
		if (IsGreaterThan(box.bottomLeftBack.x, object->axisAlignedBoundingBox->bottomLeftBack.x)) {
			box.bottomLeftBack.x = object->axisAlignedBoundingBox->bottomLeftBack.x;
		}
		if (IsGreaterThan(box.bottomLeftBack.y, object->axisAlignedBoundingBox->bottomLeftBack.y)) {
			box.bottomLeftBack.y = object->axisAlignedBoundingBox->bottomLeftBack.y;
		}
		if (IsGreaterThan(box.bottomLeftBack.z, object->axisAlignedBoundingBox->bottomLeftBack.z)) {
			box.bottomLeftBack.z = object->axisAlignedBoundingBox->bottomLeftBack.z;
		}

		if (IsLessThan(box.topRightFront.x, object->axisAlignedBoundingBox->topRightFront.x)) {
			box.topRightFront.x = object->axisAlignedBoundingBox->topRightFront.x;
		}
		if (IsLessThan(box.topRightFront.y, object->axisAlignedBoundingBox->topRightFront.y)) {
			box.topRightFront.y = object->axisAlignedBoundingBox->topRightFront.y;
		}
		if (IsLessThan(box.topRightFront.z, object->axisAlignedBoundingBox->topRightFront.z)) {
			box.topRightFront.z = object->axisAlignedBoundingBox->topRightFront.z;
		}
	}

	return box;
}

KdTreeNode* KdTree::constructTree(std::vector<Object*>& objects, int depth) {
	KdTreeNode* node = new KdTreeNode();
	node->box = ComputeBoundingBox(objects);

	if (objects.size() < MAX_TRIANGLES_PER_LEAF) {
		node->triangles = std::move(objects);
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

// prepares a list of objects likely to have an intersection
bool KdTree::traverse(KdTreeNode& node, Ray& ray, std::vector<Object*>& result) {
	BoxIntersection boxIntersection;
	node.box.Intersect(ray, boxIntersection);
	if (!boxIntersection.isHit) {
		return false;
	}

	for (Object* object : node.triangles) {
		result.push_back(object);
	}

	bool hit_left = false;
	bool hit_right = false;
	if (ray.direction[node.axis] < 0) {
		hit_left = node.left && traverse(*node.left, ray, result);
		hit_right = node.right && traverse(*node.right, ray, result);
	}
	else {
		hit_right = node.right && traverse(*node.right, ray, result);
		hit_left = node.left && traverse(*node.left, ray, result);
	}

	return hit_left || hit_right;
}