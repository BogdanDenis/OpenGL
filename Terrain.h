#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "QuadTree.h"

class Terrain {
	vector <QuadTree *> terrain;
	vector <QuadTree *> usedChildren;
public:
	Terrain ();
	~Terrain ();


	QuadTree *baseQuad () { return terrain[0]; }
	void baseLOD (vec3 cameraPos, QuadTree *q);
	GLfloat distToChunk (vec3 cameraPos, QuadTree *c);
	void terrainData (vec3 cameraPos, QuadTree *q);
	void Traverse (QuadTree *q);
	void traverseChildren (vec3 cameraPos, vector <vec3> &data);
};

#endif