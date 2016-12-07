#include "Chunk.h"

int Chunk::count = 0;

Chunk::Chunk (vec3 UL, vec3 UR, vec3 BL, vec3 BR) {
	this->UL = UL;
	this->UR = UR;
	this->BL = BL;
	this->BR = BR;
	centre.x = (UL + UR + BL + BR).x / 4;
	centre.y = (UL + UR + BL + BR).y / 4;
	centre.z = (UL + UR + BL + BR).z / 4;
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
	bool v1, v2, v3, v4, v5;
	v1 = VertexInClip (MVP, UL);
	v2 = VertexInClip (MVP, UR);
	v3 = VertexInClip (MVP, BL);
	v4 = VertexInClip (MVP, BR);
	v5 = VertexInClip (MVP, centre);
	bool res = v1 || v2;
	res = res || v3;
	res = res || v4;
	res = res || v5;
	return res;
}

bool Chunk::ChunkInClip (vec3 cameraPos, float viewAngle) {
	count++;
	FILE *file = NULL;
	if (count <= 10000) {
		file = fopen ("Chunk clipping.txt", "a");
		if (!file)
			printf ("File couldn't be opened!\n");
	}
	bool v1, v2, v3, v4, v5;
	v1 = VertexInClip (viewAngle, UL, cameraPos);
	v2 = VertexInClip (viewAngle, UR, cameraPos);
	v3 = VertexInClip (viewAngle, BL, cameraPos);
	v4 = VertexInClip (viewAngle, BR, cameraPos);
	v5 = VertexInClip (viewAngle, centre, cameraPos);
	bool res = v1 || v2;
	res = res || v3;
	res = res || v4;
	res = res || v5;
	if (file) {
		fprintf (file, "%d   %d %d %d %d %d - %d\n", count, BOOLINT (v1), BOOLINT (v2), BOOLINT (v3), 
			BOOLINT (v4), BOOLINT (v5), BOOLINT (res));
		fclose (file);
	}
	return res;
}

bool Chunk::VertexInClip (mat4 MVP, vec3 p) {
	vec4 Pclip = MVP * vec4 (p, 1.0);
	return abs (Pclip.x) < Pclip.w &&
		abs (Pclip.y) < Pclip.w &&
		0 < Pclip.z &&
		Pclip.z < Pclip.w;
}

bool Chunk::VertexInClip (float viewAngle, vec3 p, vec3 cameraFront) {
	p = normalize (p);
	p.y = cameraFront.y;
	float angle = degrees (acos (dot (cameraFront, p)));
	return fabs (angle) < viewAngle;
}