#pragma once

#include "Camera.h"
#include <vector>
#include "Light.h"
#include "objects/Object.h"

// Wraps all objects, lights, cameras in the world
class World {
public:
	// A list of all the lights in the world
	std::vector<Light*>* lights;

	// The world camera
	Camera* camera;

	// The maximum depth of the world
	int maxdepth;

	// A list of vertices that's the union of the vertices of all the triangles in the world
	// Triangles are defined by storing indexes in this list
	std::vector<vec3*>* vertices;

	// [Not currently used] A list of <vertex, normal> pairs where the normals can be arbitrary 
	std::vector<std::pair<vec3*, vec3*>*>* vertexnormals;

	// A list of all the geometric objects in the world, such as triangles, spheres, etc.
	std::vector<Object*>* objects;
};