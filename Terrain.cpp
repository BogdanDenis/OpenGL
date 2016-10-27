#include "Terrain.h"

Terrain::Terrain () {
	terrain.push_back (new QuadTree (NULL, vec3 (-5000.0, 0.0, -5000.0), vec3 (5000.0, 0.0, -5000.0),
		vec3 (-5000.0, 0.0, 5000.0), vec3 (5000.0, 0.0, 5000.0), 1, TOP));
	usedChildren.reserve (10000);
}

Terrain::~Terrain () {
	for each (QuadTree *c in terrain)
		c->~QuadTree ();
}

GLfloat Terrain::distToChunk (vec3 cameraPos, QuadTree *c) {
	GLfloat dist = distance (cameraPos, c->getUL ());
	if (distance (cameraPos, c->getUR ()) < dist)
		dist = distance (cameraPos, c->getUR ());
	if (distance (cameraPos, c->getBL ()) < dist)
		dist = distance (cameraPos, c->getBL ());
	if (distance (cameraPos, c->getBR ()) < dist)
		dist = distance (cameraPos, c->getBR ());
	if (distance (cameraPos, c->getCentre ()) < dist)
		dist = distance (cameraPos, c->getCentre ());
	return dist;
}

void Terrain::Traverse (QuadTree *q) {
	q->verticesAdded = false;
	if (q->getChildren ().size ()) {
		for each (QuadTree *c in q->getChildren ())
			Traverse (c);
	}
}

void Terrain::baseLOD (vec3 cameraPos, QuadTree *q) {
	if (q->getDiam () > 50) {
		q->incLOD ();
		for each (QuadTree *c in q->getChildren ()) {
			baseLOD (cameraPos, c);
		}
	}
	if (q == baseQuad ()) {
		usedChildren.push_back (q);
	}
}

void Terrain::traverseChildren (vec3 cameraPos, vector <vec3> &data) {
	vector <QuadTree *> addChildren;
	if (usedChildren.size ())
		for (vector <QuadTree *>::iterator i = usedChildren.begin (); i != usedChildren.end (); ) {
			if (distToChunk (cameraPos, (*i)) / (*i)->getLevel () > (*i)->getDiam ()) {
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
					if (distToChunk (cameraPos, c) / c->getLevel () < c->getDiam ())
						isFound = false;
				}
				if (!isFound) {
					for each (QuadTree *c in (*i)->getChildren ()) {
						if (find (usedChildren.begin (), usedChildren.end (), c) == usedChildren.end ()) {
							usedChildren.push_back (c);
							isFound = false;
						}
					}
					i = usedChildren.erase (i);
					continue;
				}
			}
			i++;
		}
	
	if (usedChildren.size ())
		for each (QuadTree *c in usedChildren) {
			for each (vec3 v in c->getData ())
				data.push_back (v);
		}
	printf ("Size :%d\n", usedChildren.size ());
}

void Terrain::terrainData (vec3 cameraPos, QuadTree *q) {
	GLfloat dist = distToChunk (cameraPos, q);
	if (q->getChildren ().size ()) {
		if (dist / q->getLevel () <= q->getDiam ()) {
			for each (QuadTree *c in q->getChildren ()) {
				terrainData (cameraPos, c);
			}
		}
		else {
			if (q->getParent ()) {
				usedChildren.push_back (q->getParent ());
			}
			else
				usedChildren.push_back (q);
		}
	}
	else {
		if (dist / q->getLevel () <= q->getDiam ()) {
			usedChildren.push_back (q);
		}
		else {
			if (q->getParent ())
				usedChildren.push_back (q->getParent ());
			else
				usedChildren.push_back (q);
		}
	}
}
