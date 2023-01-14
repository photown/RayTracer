#pragma once

#include "Camera.h"
#include <vector>
#include "Light.h"
#include "objects/Object.h"

class World {
public:
	std::vector<Light*>* lights;
	Camera* camera;

	int maxdepth;

	std::vector<vec3*>* vertices;
	std::vector<std::pair<vec3*, vec3*>*>* vertexnormals;
	std::vector<Object*>* objects;
};