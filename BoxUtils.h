#pragma once
#include "Box.h"
#include "objects/Object.h"
#include<vector>

// Returns an axis-aligned bounding box that encapsulates the given objects
Box CalculateBoxForObjects(std::vector<Object*> objects);