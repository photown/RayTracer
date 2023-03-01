#include "BoxUtils.h"

Box CalculateBoxForObjects(std::vector<Object*> objects) {
	if (objects.size() == 0) {
		return Box(vec3(), vec3());
	}
	Box result = Box(
		vec3(objects[0]->axisAlignedBoundingBox->bottomLeftBack), 
		vec3(objects[0]->axisAlignedBoundingBox->topRightFront));

	for (Object* object : objects) {
		result.ExpandToInclude(*object->axisAlignedBoundingBox);
	}

	return result;
}