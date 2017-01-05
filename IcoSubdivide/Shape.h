#pragma once
#include "Vector3.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include "GL/freeglut.h"


typedef struct triangle
{
	int t[3];
};

typedef struct centroid
{
	Vector3 midpoint;
	std::vector<float>distance;
};

typedef struct randVertices
{
	Vector3 pos;
	std::vector<float>pointDistance;
};

class Shape
{

private:
	int xAngle;
	int yAngle;
	int levelOfSub;
	int drawState;
	int tessOrRand;

public:
	Shape(void);
	~Shape(void);

	int nVerts;
	int nTris;

	void loadMesh(char*);
	void renderMesh();

	std::vector<Vector3>vertices;	//Array for all points of shape
	std::vector<randVertices>randVert;	//Used in the random distribution
	std::vector<triangle>tris;	//List faces containing int array for vertex access	
	std::vector<centroid>centroids;  //Center of all faces

	void incX();
	void incY();
	void decX();
	void decY();

	int getXAngle();
	int getYAngle();

	void reset();

	void calculateMidPoints();
	void drawMidPoints();
	void normaliseMidPoints();
	void printCVS();
	void subdivide();

	void normalisePoints();

	void switchDrawState();
	void switchDrawMode();

	void randomisePoints();
	void distanceToEveryPoint();
	void pointSample();
};