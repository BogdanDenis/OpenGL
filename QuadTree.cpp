#include "QuadTree.h"

map <GLfloat, GLfloat> mmap;
map <GLfloat, GLfloat> QuadTree::LODDiam = mmap;
GLfloat QuadTree::time = 0.0;


QuadTree::QuadTree (QuadTree *parent, vec3 UL, vec3 UR, vec3 BL, vec3 BR, int level, FACE face) {
	children.clear ();
	verticesAdded = false;
	this->parent = parent;
	this->level = level;
	this->face = face;
	this->diameter = distance (UL, UR);
	this->UL = UL;
	this->UR = UR;
	this->BL = BL;
	this->BR = BR;


	this->centre = vec3 ((this->UR.x + this->UL.x) / 2, (this->UR.y + this->BR.y) / 2, (this->UR.z + this->UL.z) / 2);
	GLfloat dx, dy, dz;
	dx = (this->UR.x - this->UL.x) / 2;
	dy = (this->UR.y - this->BR.y) / 2;
	dz = (this->BL.z - this->UL.z) / 2 + (this->UR.z - this->UL.z) / 2;

	vec3 sUL, sUR, sBL, sBR;
	//TOP LEFT SQUARE
	sUL = this->UL;
	sUR = this->UR;
	sBL = this->BL;
	sBR = this->BR;
	//data.push_back (normalize (sUR));
	data.push_back (sUR);
	data.push_back (sUL);

	//data.push_back (normalize (sUL));
	data.push_back (sBL);
	//data.push_back (normalize (sBL));
	data.push_back (sUR);
	data.push_back (sBL);


	data.push_back (sBR);
	//data.push_back (normalize (sBR));
	
}

QuadTree::~QuadTree () {
	if (children.size ())
		for each (QuadTree *q in children)
			if (q) q->~QuadTree ();
}

bool QuadTree::inHorison (const vec3 &cameraPos, const vec3 &spherePos, const GLfloat &radius) {
	GLfloat height = distance (cameraPos, spherePos);
	GLfloat dist = distToChunk (cameraPos, spherePos, radius);
	GLfloat hor = sqrt (pow (height, 2) - pow (radius, 2));
	//printf ("Height : %f, Horizon : %f, Distance : %f, Radius : %f\n", height, hor, dist, radius);
	return hor >= dist;
}

GLfloat QuadTree::distToChunk (const vec3 &cameraPos, const vec3 &spherePos, GLfloat radius) {
	vec3 UL = mix (this->UL, normalize (this->UL) * radius, 1.0) + spherePos;
	vec3 UR = mix (this->UR, normalize (this->UR) * radius, 1.0) + spherePos;
	vec3 BL = mix (this->BL, normalize (this->BL) * radius, 1.0) + spherePos;
	vec3 BR = mix (this->BR, normalize (this->BR) * radius, 1.0) + spherePos;
	vec3 centre = mix (this->centre, normalize (this->centre) * radius, 1.0) + spherePos;

	GLfloat dist = distance (cameraPos, UL);
	if (distance (cameraPos, UR) < dist)
		dist = distance (cameraPos, UR);
	if (distance (cameraPos, BL) < dist)
		dist = distance (cameraPos, BL);
	if (distance (cameraPos, BR) < dist)
		dist = distance (cameraPos, BR);
	if (distance (cameraPos, centre) < dist)
		dist = distance (cameraPos, centre);
	return dist;
}

GLfloat QuadTree::desiredDiam (GLfloat dist) {
	map <GLfloat, GLfloat>::iterator it;
	for (it = QuadTree::LODDiam.begin (); it != QuadTree::LODDiam.end (); it++) {
		if (it->first > dist) {
			printf ("%f ", dist);
			printf ("%f ", it->second);
			printf ("%f\n", diameter);
			return it->second;
		}
	}
	return 1000.0;
}

void QuadTree::changeLOD (const vec3 &cameraPos, const vec3 &spherePos, int minLOD, int maxLOD, GLfloat radius, vector <QuadTree *> &usedChildren) {
	vec3 centre = mix (this->centre + spherePos, normalize (this->centre + spherePos) * radius, 1.0);
	GLfloat dist = distToChunk (cameraPos, spherePos, radius);
	verticesAdded = false;
	bool deleted = false;
	bool increasedLOD = false;
	if (children.size ()) {
		if (level < minLOD) {
			for each (QuadTree *c in children)
				c->changeLOD (cameraPos, spherePos, minLOD, maxLOD, radius, usedChildren);
		}
		else if (level == minLOD) {
			if (dist / 10 < diameter) {
				increasedLOD = true;
				for each (QuadTree *c in children)
					c->changeLOD (cameraPos, spherePos, minLOD, maxLOD, radius, usedChildren);
			}
			else if (dist / 10 > diameter) {
				if (inHorison (cameraPos, spherePos, radius))
					usedChildren.push_back (this);
			}
		}
		else if (level > minLOD && level <= maxLOD) {
			if (dist / 10 < diameter) {
				increasedLOD = true;
				for each (QuadTree *c in children)
					c->changeLOD (cameraPos, spherePos, minLOD, maxLOD, radius, usedChildren);
			}
			else if (dist / 10 > diameter) {
				if (this->parent && parent->inHorison (cameraPos, spherePos, radius))
					usedChildren.push_back (this->parent);
				else if (inHorison (cameraPos, spherePos, radius))
					usedChildren.push_back (this);
			}
		}
	}
	else {
		if (level < minLOD) {
			increasedLOD = true;
			for each (QuadTree *c in children)
				c->changeLOD (cameraPos, spherePos, minLOD, maxLOD, radius, usedChildren);
		}
		else if (level == minLOD) {
			if (desiredDiam (dist) < diameter) {
				increasedLOD = true;
				for each (QuadTree *c in children)
					c->changeLOD (cameraPos, spherePos, minLOD, maxLOD, radius, usedChildren);
			}
			else if (dist / 10 > diameter) {
				if (inHorison (cameraPos, spherePos, radius))
					usedChildren.push_back (this);
			}
		}
		else if (level > minLOD && level <= maxLOD) {
			if (dist / 10 < diameter) {
				increasedLOD = true;
				for each (QuadTree *c in children)
					c->changeLOD (cameraPos, spherePos, minLOD, maxLOD, radius, usedChildren);
			}
			else if (dist / 10 > diameter) {
				if (this->parent && parent->inHorison (cameraPos, spherePos, radius))
					usedChildren.push_back (this->parent);
				else if (inHorison (cameraPos, spherePos, radius))
					usedChildren.push_back (this);
			}
		}
	}
}

void QuadTree::fullDetail (const vec3 &cameraPos, const vec3 &spherePos, int minLOD, int maxLOD, GLfloat radius, vector <vec3> &data, vector <QuadTree *> &usedChildren) {
	if (level < maxLOD) {
		incLOD ();
		for each (QuadTree *c in children) {
			c->fullDetail (cameraPos, spherePos, minLOD, maxLOD, radius, data, usedChildren);
		}
	}
}

void QuadTree::incLOD () {
	GLfloat dx, dy, dz;
	//vec3 UL, UR, BL, BR;
	dx = (UR.x - UL.x) / 2;
	dy = (UR.y - BR.y) / 2;
	dz = (BL.z - UL.z) / 2 + (UR.z - UL.z) / 2;
	vec3 sUL, sUR, sBL, sBR;
	switch (face) {
	case FRONT:
	case BACK:
		sUL = UL;
		sUR = MOVE (UL, dx, 0.0, 0.0);
		sBL = MOVE (UL, 0.0, -dy, 0.0);
		sBR = MOVE (UL, dx, -dy, 0.0);
		break;
	case TOP:
	case BOTTOM:
		sUL = UL;
		sUR = MOVE (UL, dx, 0.0, 0.0);
		sBL = MOVE (UL, 0.0, 0.0, dz);
		sBR = MOVE (UL, dx, 0.0, dz);
		break;
	case LEFT:
	case RIGHT:
		sUL = UL;
		sUR = MOVE (UL, 0.0, 0.0, dz);
		sBL = MOVE (UL, 0.0, -dy, 0.0);
		sBR = MOVE (UL, 0.0, -dy, dz);
		break;
	}
	addChild (new QuadTree (this, sUL, sUR, sBL, sBR, level + 1, face));
	//TOP RIGHT SQUARE
	switch (face) {
	case FRONT:
	case BACK:
	case TOP:
	case BOTTOM:
		ParallelMove (sUL, sUR, sBL, sBR, dx, 0.0, 0.0);
		break;
	case LEFT:
	case RIGHT:
		ParallelMove (sUL, sUR, sBL, sBR, 0.0, 0.0, dz);
		break;
	}
	addChild (new QuadTree (this, sUL, sUR, sBL, sBR, level + 1, face));
	//BOTTOM LEFT
	switch (face) {
	case FRONT:
	case BACK:
		ParallelMove (sUL, sUR, sBL, sBR, -dx, -dy, 0.0);
		break;
	case TOP:
	case BOTTOM:
		ParallelMove (sUL, sUR, sBL, sBR, -dx, 0.0, dz);
		break;
		break;
	case LEFT:
	case RIGHT:
		ParallelMove (sUL, sUR, sBL, sBR, 0.0, -dy, -dz);
		break;
	}
	addChild (new QuadTree (this, sUL, sUR, sBL, sBR, level + 1, face));
	//BOTTOM RIGHT
	switch (face) {
	case FRONT:
	case BACK:
	case TOP:
	case BOTTOM:
		ParallelMove (sUL, sUR, sBL, sBR, dx, 0.0, 0.0);
		break;
	case LEFT:
	case RIGHT:
		ParallelMove (sUL, sUR, sBL, sBR, 0.0, 0.0, dz);
		break;
	}
	addChild (new QuadTree (this, sUL, sUR, sBL, sBR, level + 1, face));
}

void QuadTree::traverse (const vec3 &cameraPos, const vec3 &spherePos, int minLOD, int maxLOD, GLfloat radius, vector <vec3> &data, vector <QuadTree *> &usedChildren) {
	vec3 centre = mix (this->centre + spherePos, normalize (this->centre + spherePos) * radius, 1.0);
	GLfloat dist = distToChunk (cameraPos, spherePos, radius);
	if (dist > sqrt (2) * radius)
		return;
}