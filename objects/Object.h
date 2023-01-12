#pragma once

#include "../Const.h"
#include "../Ray.h"

enum Shape { 
	ShapeCube, 
	ShapeTriangle, 
	ShapeSphere, 
	ShapeTeapot 
};

class Intersection; 

class Object {
public:
	Shape type;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
	mat4 transform;

	mat3 normal_transform;
	mat4 transform_inverse;

	virtual void Intersect(Ray* rayInObjectSpace, Object* object, Intersection* result) = 0;
};