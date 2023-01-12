#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
const float pi = 3.14159265f; // For portability across platforms