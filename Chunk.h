#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <glm.hpp>
#include <vector>
#include <glfw3.h>
#include <fstream>

using glm::vec3;
using glm::mat4;
using glm::vec4;
using std::vector;
using glm::mix;

class Chunk {
	vec3 UL, UR, BL, BR;
	vector <Chunk> sides;
	vector <Chunk> corners;
	vector <vec3> data;
public:
	Chunk () {}
	Chunk (vec3 UL, vec3 UR, vec3 BL, vec3 BR);
	~Chunk ();

	vector <vec3> &getData () { return data; }
};

#endif