#ifndef _MY_VERTEX_MATH_
#define _MY_VERTEX_MATH_

#include <glm.hpp>

using glm::vec3;
using glm::cross;
using glm::normalize;

inline vec3 normal (vec3 v1, vec3 v2, vec3 v3) {
	vec3 edge1 = v2 - v1;
	vec3 edge2 = v3 - v1;
	vec3 normal = normalize (cross (edge1, edge2));
	return normal;
}

#endif