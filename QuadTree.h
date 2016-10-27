#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#define SQ_FRONT vec3 (1.0, -1.0, 0.0)
#define SQ_BACK vec3 (1.0, -1.0, 0.0)
#define SQ_LEFT vec3 (0.0, 1.0, 1.0)
#define SQ_RIGHT vec3 (0.0, 1.0, 1.0)
#define SQ_TOP vec3 (1.0, 0.0, 1.0)
#define SQ_BOTTOM vec3 (1.0, 0.0, 1.0)

#define MOVE(v, a, b, c) vec3((v.x + a), (v.y + b), (v.z + c))

#include <glm.hpp>
#include <vector>
#include <map>
#include <windows.h>
#include <GL/gl.h>
#include <glfw3.h>
#include <fstream>

using glm::vec3;
using glm::mat4;
using glm::vec4;
using std::vector;
using std::map;
using glm::mix;
using std::ofstream;

enum FACE { FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT };


inline void ParallelMove (vec3 &sUL, vec3 &sUR, vec3 &sBL, vec3 &sBR, GLfloat dx, GLfloat dy, GLfloat dz) {
	sUL.x += dx;
	sUL.y += dy;
	sUL.z += dz;
	sUR.x += dx;
	sUR.y += dy;
	sUR.z += dz;
	sBL.x += dx;
	sBL.y += dy;
	sBL.z += dz;
	sBR.x += dx;
	sBR.y += dy;
	sBR.z += dz;
}

class QuadTree {
private:
	int level;
	FACE face;
	vector <QuadTree *> children;
	QuadTree *parent;
	vec3 UL, UR, BL, BR;
	vector <vec3> data;
	GLfloat diameter;
	vec3 centre;
public:
	bool verticesAdded;
	static map <GLfloat, GLfloat> LODDiam;
	static GLfloat time;

	QuadTree (QuadTree *parent, vec3 UL, vec3 UR, vec3 BL, vec3 BR, int level, FACE face);
	~QuadTree ();

	int getLevel () { return level; }
	GLfloat getDiam () { return diameter; }
	QuadTree *getParent () { return parent; }
	const vec3 getCentre () { return centre; }
	const vector <QuadTree *> &getChildren () { return children; }
	const vector <vec3> &getData () { return data; }
	FACE getFace () { return face; }
	vec3 getUL () { return UL; }
	vec3 getUR () { return UR; }
	vec3 getBL () { return BL; }
	vec3 getBR () { return BR; }

	void fullDetail (const vec3 &cameraPos, const vec3 &spherePos, int minLOD, int maxLOD, GLfloat radius, vector <vec3> &data, vector <QuadTree *> &usedChildren);
	void changeLOD (const vec3 &cameraPos, const vec3 &spherePos, int minLOD, int maxLOD, GLfloat radius, vector <QuadTree *> &usedChildren);
	void traverse (const vec3 &cameraPos, const vec3 &spherePos, int minLOD, int maxLOD, GLfloat radius, vector <vec3> &data, vector <QuadTree *> &usedChildren);
	void incLOD ();
	bool inHorison (const vec3 &cameraPos, const vec3 &spherePos, const GLfloat &radius);
	GLfloat distToChunk (const vec3 &cameraPos, const vec3 &spherePos, GLfloat radius);
	GLfloat desiredDiam (GLfloat dist);

	void addChild (QuadTree* child) { children.push_back (child); }
};

#endif