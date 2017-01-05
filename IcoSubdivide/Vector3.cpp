#include "Vector3.h"

#define MAX 1
#define MIN -1

Vector3::Vector3(void)
{
	float x  = 0;
	float y = 0;
	float z = 0;
}


Vector3::~Vector3(void)
{
}

void Vector3::normalise()
{
	float d = sqrt((this->x * this->x) + (this->y * this->y) + ( this->z * this->z));

	if(d == 0.0)
	{
		std::cout << "Zero length vector" << std::endl;
	}

	this->x /= d;
	this->y /= d;
	this->z /= d;
}

float Vector3::getRandom()
{
	return ((float(rand()) / float(RAND_MAX)) * (MAX - MIN)) + MIN;
}

float Vector3::dotProduct(Vector3 *a, Vector3 *b)
{
	float r = (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
	return r;
}
