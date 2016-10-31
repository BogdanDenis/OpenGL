#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "Chunk.h"

class Terrain {
	vector <Chunk> chunks;
public:
	Terrain ();
	~Terrain ();

	void BuildVertexData (vector <vec3> &data, mat4 MVP);
};

#endif