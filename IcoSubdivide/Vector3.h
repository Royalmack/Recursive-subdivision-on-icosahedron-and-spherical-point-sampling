#pragma once
#include <cmath>
#include <iostream>

class Vector3
{
public:

	float x,y,z;

	Vector3(void);
	~Vector3(void);

	void normalise();
	float getRandom();
	float dotProduct(Vector3*, Vector3*);
};

