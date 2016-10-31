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

void Chunk::operator() (Chunk c, int type) {
	if (type == c_Side) {
		sides.push_back (c);
	}
	else corners.push_back (c);
}

bool Chunk::ChunkInClip (mat4 MVP) {
	bool v1, v2, v3, v4;
	v1 = VertexInClip (MVP, UL);
	v2 = VertexInClip (MVP, UR);
	v3 = VertexInClip (MVP, BL);
	v4 = VertexInClip (MVP, BR);
	bool res = v1 || v2;
	res = res || v3;
	res = res || v4;
	return res;
}

bool Chunk::VertexInClip (mat4 MVP, vec3 p) {
	vec4 Pclip = MVP * vec4 (p, 1.0);
	return abs (Pclip.x) < Pclip.w &&
		abs (Pclip.y) < Pclip.w &&
		0 < Pclip.z &&
		Pclip.z < Pclip.w;
}