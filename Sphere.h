#ifndef _SPHERE_
#define _SPHERE_

#include <GL/glew.h>
#include <glfw3.h>
#include <GL/freeglut.h>
#include <SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <stdio.h>
#include <iostream>
#include <complex>
#include <vector>
#include <algorithm>
#include <time.h>
#include <stdlib.h>

#include "MyVertexMath.h"
#include "QuadTree.h"
#include "simplexnoise.h"

using std::vector;
using glm::vec3;
using glm::mix;
using glm::normalize;
using glm::distance;
using glm::radians;
using glm::scale;

class Sphere {
	GLuint VBO;
	vector <QuadTree *> quadTrees;
	vector <vec3> data;
	vector <QuadTree *> usedChildren;
	vec3 position;
	float radius;
	int minLOD;
	int maxLOD;
	int trianglesNum;
	int verticesNum;
public:
	Sphere (GLfloat radius, vec3 cameraPos, vec3 position);
	~Sphere ();

	void baseLOD (QuadTree *quad);
	void buildVertexData (vec3 cameraPos);
	void Traverse (QuadTree *q);


	GLuint getVBOId () { return VBO; }
	float getRadius () { return radius; }
	int getTrianglesNum () { return trianglesNum; }
	int getVerticesNum () { return verticesNum; }
	int getMinLOD () { return minLOD; }
	vector <QuadTree *> getQuadTrees () { return quadTrees; }
	vec3 *getDataEl (int i) { return &data[i]; }
	vector <vec3> *getData () { return &data; }

	void setDataEl (int i, vec3 d) { data[i] = d; }
};

#endif