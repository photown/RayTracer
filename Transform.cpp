// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.  
    const mat3 identity(1.0f);
    const float radians = glm::radians(degrees);
    const mat3 crossProductMatrix(0, axis.z, -axis.y, -axis.z, 0, axis.x, axis.y, -axis.x, 0);
    const mat3 outerProductMatrix(axis * axis.x, axis * axis.y, axis * axis.z);

    // You will change this return call
    return identity * cos(radians) + sin(radians) * crossProductMatrix + (1 - cos(radians)) * outerProductMatrix;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1. 
    const mat3 transform = rotate(degrees, up);
    eye = transform * eye;
    up = transform * up;
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1.  
    const mat3 transform = rotate(degrees, glm::normalize(-glm::cross(up, eye)));
    eye = transform * eye;
    up = transform * up;
}

mat4 Transform::lookAt(const vec3& eye, const vec3& center, const vec3& up)
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.
    const vec3 a = eye - center;
    const vec3 b = up;
    const vec3 w = glm::normalize(a);
    const vec3 u = glm::normalize(glm::cross(b, w));
    const vec3 v = glm::cross(w, u);
    const mat4 resultRotation(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0, 0, 0, 0, 1);
    mat4 resultTranslation(1.0f);
    resultTranslation[3] = vec4(-eye, 1);

    // You will change this return call
    return resultRotation * resultTranslation;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    float fovyRad = glm::radians(fovy);
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well. 
    mat4 ret(0.0f);
    float d = 1 / glm::tan(fovyRad / 2); //cot
    ret[0][0] = d / aspect;
    ret[1][1] = d;
    ret[2][2] = -(zFar + zNear) / (zFar - zNear);
    ret[3][2] = -2 * zFar * zNear / (zFar - zNear);
    ret[2][3] = -1;
    return ret;
}

mat4 Transform::scale(const float& sx, const float& sy, const float& sz)
{
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
    mat4 ret(1.0f);
    ret[0][0] = sx;
    ret[1][1] = sy;
    ret[2][2] = sz;
    return ret;
}

mat4 Transform::translate(const float& tx, const float& ty, const float& tz)
{
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    mat4 ret(1.0f);
    ret[3] = vec4(tx, ty, tz, 1);
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3& up, const vec3& zvec)
{
    vec3 x = glm::cross(up, zvec);
    vec3 y = glm::cross(zvec, x);
    vec3 ret = glm::normalize(y);
    return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
