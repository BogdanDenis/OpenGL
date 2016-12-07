#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "Chunk.h"

template <typename T>
std::vector<T> &operator+=(std::vector<T> &A, const std::vector<T> &B) {
	A.insert (A.end (), B.begin (), B.end ());
	return A;
}

class Terrain {
	vector <Chunk> chunks;
public:
	Terrain ();
	~Terrain ();

	void BuildVertexData (vector <vec3> &data, mat4 MVP);
	void BuildVertexData (vector <vec3> &data, float viewAngle, vec3 cameraPos);
};

#endif