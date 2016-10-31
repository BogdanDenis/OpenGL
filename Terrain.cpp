#include "Terrain.h"

Terrain::Terrain () {
	GLfloat plainw, plaind;
	GLfloat delta = 1000.0;
	plainw = plaind = 10 * delta;

	for (GLfloat x = -plainw / 2; x < plainw / 2; x += delta) {
		for (GLfloat z = -plaind / 2; z < plaind / 2; z += delta) {
			Chunk c (vec3 (x, 0.0, z), vec3 (x + delta, 0.0, z),
				vec3 (x, 0.0, z + delta), vec3 (x + delta, 0.0, z + delta));
			chunks.push_back (c);
		}
	}
}

Terrain::~Terrain () {
	chunks.clear ();
}

void Terrain::BuildVertexData (vector <vec3> &data, mat4 MVP) {
	for each (Chunk c in chunks) {
		if (c.ChunkInClip (MVP)) {
			for each (vec3 v in c.getData ())
				data.push_back (v);
		}
		else {
			for each (vec3 v in c.getData ())
				if (c.VertexInClip (MVP, v))
					data.push_back (v);
		}
	}
}