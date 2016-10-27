#include "Chunk.h"

Chunk::Chunk (vec3 UL, vec3 UR, vec3 BL, vec3 BR) {
	this->UL = UL;
	this->UR = UR;
	this->BL = BL;
	this->BR = BR;
	GLfloat delta = 100;
	for (GLfloat x = UL.x; x < UR.x; x += delta) {
		for (GLfloat z = UL.z; z < BL.z; z += delta) {
			data.push_back (vec3 (x, 0.0, z));
			data.push_back (vec3 (x, 0.0, z + delta));
			data.push_back (vec3 (x + delta, 0.0, z + delta));
			data.push_back (vec3 (x + delta, 0.0, z));
		}
	}
}

Chunk::~Chunk () {
	sides.clear ();
	corners.clear ();
}