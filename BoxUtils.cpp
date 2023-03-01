#include "BoxUtils.h"

Box CalculateBoxForObjects(std::vector<Object*> objects) {
	Box result = Box(vec3(), vec3());

	for (Object* object : objects) {
		result.ExpandToInclude(*object->axisAlignedBoundingBox);
	}

	return result;
}