#include "Sphere.h"

Sphere::Sphere (GLfloat radius, vec3 cameraPos, vec3 spherePos) {
	this->radius = radius;
	vector <vec3> data;
	data.reserve (30000);
	this->minLOD = 4;
	this->maxLOD = 4;
	this->position = spherePos;
	//this->quadTrees.reserve (6);
	quadTrees.push_back (new QuadTree (NULL, vec3 (-radius, radius, radius), vec3 (radius, radius, radius),
		vec3 (-radius, -radius, radius), vec3 (radius, -radius, radius), 1, FRONT));
	quadTrees.push_back (new QuadTree (NULL, vec3 (radius, radius, -radius), vec3 (-radius, radius, -radius),
		vec3 (radius, -radius, -radius), vec3 (-radius, -radius, -radius), 1, BACK));
	quadTrees.push_back (new QuadTree (NULL, vec3 (-radius, radius, -radius), vec3 (radius, radius, -radius),
		vec3 (-radius, radius, radius), vec3 (radius, radius, radius), 1, TOP));
	quadTrees.push_back (new QuadTree (NULL, vec3 (-radius, -radius, radius), vec3 (radius, -radius, radius),
		vec3 (-radius, -radius, -radius), vec3 (radius, -radius, -radius), 1, BOTTOM));
	quadTrees.push_back (new QuadTree (NULL, vec3 (-radius, radius, -radius), vec3 (-radius, radius, radius),
		vec3 (-radius, -radius, -radius), vec3 (-radius, -radius, radius), 1, LEFT));
	quadTrees.push_back (new QuadTree (NULL, vec3 (radius, radius, radius), vec3 (radius, radius, -radius),
		vec3 (radius, -radius, radius), vec3 (radius, -radius, -radius), 1, RIGHT));
	usedChildren.reserve (1000);
	for each (QuadTree *q in quadTrees) {
		q->fullDetail (cameraPos, position, minLOD, maxLOD, radius, data, usedChildren);
		//usedChildren.push_back (q);//q->changeLOD (cameraPos, spherePos, minLOD, maxLOD, radius, usedChildren);
	}
	for each (QuadTree *q in quadTrees) {
		Traverse (q);
	}
	glGenBuffers (1, &VBO);
}

void Sphere::Traverse (QuadTree *q) {
	if (q->getLevel () == maxLOD) {
		for each (vec3 v in q->getData ()) {
			data.push_back (v);
		}
	}
	else {
		for each (QuadTree *c in q->getChildren ()) {
			Traverse (c);
		}
	}
}

void Sphere::baseLOD (QuadTree *q) {
	if (q->getLevel () >= minLOD) {
		return;
	}
	GLfloat dx, dy, dz;
	vec3 UL, UR, BL, BR;
	FACE face = q->getFace ();
	UL = q->getUL ();
	UR = q->getUR ();
	BL = q->getBL ();
	BR = q->getBR ();
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
	q->addChild (new QuadTree (q, sUL, sUR, sBL, sBR, q->getLevel () + 1, face));
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
	q->addChild (new QuadTree (q, sUL, sUR, sBL, sBR, q->getLevel () + 1, face));
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
	q->addChild (new QuadTree (q, sUL, sUR, sBL, sBR, q->getLevel () + 1, face));
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
	q->addChild (new QuadTree (q, sUL, sUR, sBL, sBR, q->getLevel () + 1, face));
	for each (QuadTree *c in q->getChildren ()) {
		baseLOD (c);
	}
}

void Sphere::buildVertexData (vec3 cameraPos) {
	data.clear ();
	data.reserve (300000);
	GLfloat t = glfwGetTime ();
	printf ("C: %d\n", usedChildren.size ());
	/*if (usedChildren.size ())
	for (vector <QuadTree *>::iterator i = usedChildren.begin (); i < usedChildren.end (); ) {
	if ((*i)->distToChunk (cameraPos, position, radius) / 10 > (*i)->getDiam ()) {
	if ((*i)->getParent ()) {
	if (find (usedChildren.begin (), usedChildren.end (), (*i)->getParent ()) == usedChildren.end ())
	(*i) = (*i)->getParent ();
	else {
	i = usedChildren.erase (i);
	continue;
	}
	}
	}
	else if ((*i)->getChildren ().size ()) {
	bool isFound = true;
	for each (QuadTree *c in (*i)->getChildren ()) {
	if (find (usedChildren.begin (), usedChildren.end (), c) == usedChildren.end ()) {
	if ((*i)->inHorison (cameraPos, position, radius)) {
	usedChildren.push_back (c);
	isFound = false;
	}
	}
	}
	if (!isFound) {
	i = usedChildren.erase (i);
	continue;
	}
	}
	i++;
	}*/
	for each (QuadTree *c in usedChildren) {
		//if (c->inHorison (cameraPos, position, radius))
		for each (vec3 v in c->getData ()) {
			data.push_back (v);
		}
	}
	printf ("Time :%f\n", glfwGetTime () - t);
	printf ("%d\n", usedChildren.size ());
	//printf ("%d\n", data.size ());
}

Sphere::~Sphere () {
	for each (QuadTree *q in quadTrees)
		q->~QuadTree ();
}