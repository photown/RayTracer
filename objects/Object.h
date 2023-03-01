#pragma once

#include "../RayIntersection.h"
#include "../Box.h"
#include "../Const.h"
#include "../Ray.h"

enum Shape { 
	ShapeTriangle, 
	ShapeSphere
};

class RayIntersection; 


class Object {
public:
	Shape type;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
	mat4 transform;

	// Transformation matrix for the normals of the object, see https://stackoverflow.com/a/26477624/2566395
	mat3 normal_transform;

	// Inverse transform of the object transformation
	mat4 transform_inverse;

	// World view axis-aligned bounding box
	Box* axisAlignedBoundingBox;
	
	Object();

	// Sets the object transformation and pre-calculates other useful matrices such as the normal transform and inverse transform
	void SetTransform(mat4& transform);

	// Populates `result` with the results of the intersection of the ray with the object
	virtual void Intersect(Ray& rayInObjectSpace, RayIntersection& result) = 0;

	// Calculates this object's axis-aligned bounding box in world coordinates
	virtual Box* CalculateBoundingBox(mat4& transform) = 0;
};