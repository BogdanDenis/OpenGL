#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <glm.hpp>
#include <vector>
#include <glfw3.h>
#include <stdio.h>

using glm::vec3;
using glm::mat4;
using glm::vec4;
using std::vector;
using glm::mix;
using glm::degrees;

#define c_Side 1
#define c_Corner 2

#define BOOLINT(x) x?1:0

class Chunk {
	vec3 UL, UR, BL, BR, centre;
	vector <Chunk> sides;
	vector <Chunk> corners;
	vector <vec3> data;
public:
	static int count;
	Chunk () {}
	Chunk (vec3 UL, vec3 UR, vec3 BL, vec3 BR);
	~Chunk ();

	vector <vec3> &getData () { return data; }

	bool ChunkInClip (mat4 MVP);
	bool ChunkInClip (vec3 cameraPos, float viewAngle);
	bool VertexInClip (mat4 MVP, vec3 p);
	bool VertexInClip (float viewAngle, vec3 p, vec3 cameraFront);
	void operator () (Chunk c, int type);
};

#endif