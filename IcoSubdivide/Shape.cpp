#include "Shape.h"


Shape::Shape(void)
{
	nVerts = 0;
	nTris = 0;
	xAngle = 0;
	yAngle = 0;
	levelOfSub = 0;
	drawState = 0;
	tessOrRand = 1;
}


Shape::~Shape(void)
{
}


//Read in mesh from an OBJ file
void Shape::loadMesh(char *fileName)
{
	FILE *f;

	Vector3 temp;
	triangle triTemp;

	int vertexAmt = 0;
	int faceAmt = 0;
	char string[256];
	char fChar;

	if((f = fopen(fileName, "rt")) == NULL)
		std::cout << "Error opening file!" << std:: endl;

	while(!feof(f))
	{
		fscanf(f, "%c" , &fChar);
		if(fChar == '\n')
			fscanf(f, "%c", &fChar);

		switch(fChar)
		{
		default:
			fgets(string, 256,f);
		break;

		case 'v':
			fscanf(f, "%f %f %f",  &temp.x, &temp.y, &temp.z);      //&vertices[vertexAmt].x, &vertices[vertexAmt].y, &vertices[vertexAmt].z);
			vertices.push_back(temp);
			vertexAmt++;
		break;

		case 'f':
			fscanf(f, "%d %d %d",	&triTemp.t[0], & triTemp.t[1], & triTemp.t[2]);					//tris[faceAmt].t[0], &tris[faceAmt].t[0], &tris[faceAmt].t[0]);
			tris.push_back(triTemp);
			faceAmt++;
		break;
		}
	}

	fclose(f);

	nVerts = vertexAmt;
	nTris = faceAmt;
}


//Draw shape using list of faces
void Shape::renderMesh()
{
	glColor3f(1.0, 1.0, 1.0);

	switch(tessOrRand)
	{
		case 1:
			if(drawState == 0)
			{
				glBegin(GL_TRIANGLES);

				for(int i = 0; i < tris.size(); i++)
				{
					glVertex3f(vertices[tris[i].t[0] - 1].x,
							   vertices[tris[i].t[0] - 1].y,
							   vertices[tris[i].t[0] - 1].z);

					glVertex3f(vertices[tris[i].t[1] - 1].x,
							   vertices[tris[i].t[1] - 1].y,
							   vertices[tris[i].t[1] - 1].z);


					glVertex3f(vertices[tris[i].t[2] - 1].x,
							   vertices[tris[i].t[2] - 1].y,
							   vertices[tris[i].t[2] - 1].z);			
				}	
		
				glEnd();
			}
			else
			{
				glPointSize(3);
				glBegin(GL_POINTS);

				for(int i = 0; i < tris.size(); i++)
				{
					glVertex3f(vertices[tris[i].t[0] - 1].x,
							   vertices[tris[i].t[0] - 1].y,
							   vertices[tris[i].t[0] - 1].z);


					glVertex3f(vertices[tris[i].t[1] - 1].x,
							   vertices[tris[i].t[1] - 1].y,
							   vertices[tris[i].t[1] - 1].z);


					glVertex3f(vertices[tris[i].t[2] - 1].x,
							   vertices[tris[i].t[2] - 1].y,
							   vertices[tris[i].t[2] - 1].z);			
				}

				glEnd();
			}

			calculateMidPoints();
		break;

		case 0:
			if(randVert.size() == 0)
				randomisePoints();

			glBegin(GL_POINTS);

			for(int i = 0; i < randVert.size(); i++)
			{
				glVertex3f(randVert[i].pos.x, randVert[i].pos.y, randVert[i].pos.z);
			}

			glEnd();
		break;
	}

	
}

void Shape::incX()
{
	xAngle += 2;
}

void Shape::decX()
{
	xAngle -= 2;
}

void Shape::incY()
{
	yAngle += 2;
}

void Shape::decY()
{
	yAngle -= 2;
}

int Shape::getXAngle()
{
	return xAngle;
}

int Shape::getYAngle()
{
	return yAngle;
}

void Shape::reset()
{
	xAngle = 0;
	yAngle = 0;
}


//Calculate centroid of all faces and populate a list.  Later used for placement of Vector points.
void Shape::calculateMidPoints()
{
	if(centroids.size() > 0)
		centroids.clear();

	Vector3 *temp = new Vector3();
	centroid *tCen;

	for(int i = 0; i < tris.size(); i++)
	{
		tCen = new centroid();

		temp->x = (vertices[tris[i].t[0] - 1].x + vertices[tris[i].t[1] - 1].x +vertices[tris[i].t[2] - 1].x) / 3;
		temp->y = (vertices[tris[i].t[0] - 1].y + vertices[tris[i].t[1] - 1].y +vertices[tris[i].t[2] - 1].y) / 3;
		temp->z = (vertices[tris[i].t[0] - 1].z + vertices[tris[i].t[1] - 1].z +vertices[tris[i].t[2] - 1].z) / 3;

		tCen->midpoint = *temp;

		centroids.push_back(*tCen);
	}

	//normaliseMidPoints();
	drawMidPoints();

	glutPostRedisplay();

	delete(temp);
	delete(tCen);
}


void Shape::drawMidPoints()
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(2);
	glBegin(GL_POINTS);

	for(int i = 0; i < centroids.size(); i++)
	{
		glVertex3f(centroids[i].midpoint.x, centroids[i].midpoint.y, centroids[i].midpoint.z);
		
	}

	glEnd();
}


void Shape::printCVS()
{
	FILE *w;
	
	if((w = fopen("CSV.txt", "w")) == NULL)
		std::cout << "Error opening file!" << std::endl;

	fprintf(w, "Test writing\n\n");

	for(int i = 0; i < centroids.size(); i++)
	{
		fprintf(w, "%.3f,  %.3f,  %.3f\n", centroids[i].midpoint.x, centroids[i].midpoint.y, centroids[i].midpoint.z);
	}	

	fclose(w);
}


void Shape::subdivide()
{
	levelOfSub++;

	//Create temp vectors for new triangle positions
	Vector3 t12, t23, t31;
	triangle  tTemp;
	int faceSize = tris.size();
	int current = vertices.size() + 1;	//Due to the way the objLoader and render work with - 1

	//normalisePoints();

	//Loop through each face in the list
	for(int i = 0; i < faceSize; i++)
	{
		//Create new X Pos
		t12.x = vertices[tris[i].t[0] - 1].x + vertices[tris[i].t[1] - 1].x;
		t23.x = vertices[tris[i].t[1] - 1].x + vertices[tris[i].t[2] - 1].x;
		t31.x = vertices[tris[i].t[2] - 1].x + vertices[tris[i].t[0] - 1].x;

		//Create new Y Pos
		t12.y = vertices[tris[i].t[0] - 1].y + vertices[tris[i].t[1] - 1].y;
		t23.y = vertices[tris[i].t[1] - 1].y + vertices[tris[i].t[2] - 1].y;
		t31.y = vertices[tris[i].t[2] - 1].y + vertices[tris[i].t[0] - 1].y;

		//Create new Z Pos
		t12.z = vertices[tris[i].t[0] - 1].z + vertices[tris[i].t[1] - 1].z;
		t23.z = vertices[tris[i].t[1] - 1].z + vertices[tris[i].t[2] - 1].z;
		t31.z = vertices[tris[i].t[2] - 1].z + vertices[tris[i].t[0] - 1].z;

		//Normalise new Vectors
		t12.normalise();
		t23.normalise();
		t31.normalise();

		//Pushing newly created vertices to the vertex list
		vertices.push_back(t12);
		vertices.push_back(t23);
		vertices.push_back(t31);

		//Create temp triangles out of new vertices index
		//1st
		tTemp.t[0] = tris[i].t[0];
		tTemp.t[1] = current;
		tTemp.t[2] = current + 2;

		//Push new triangle to list
		tris.push_back(tTemp);

		//2nd
		tTemp.t[0] = tris[i].t[1];
		tTemp.t[1] = current + 1;
		tTemp.t[2] = current;

		tris.push_back(tTemp);

		//3rd
		tTemp.t[0] = tris[i].t[2];
		tTemp.t[1] = current + 2;
		tTemp.t[2] = current + 1;

		tris.push_back(tTemp);

		//Final
		tTemp.t[0] = current;
		tTemp.t[1] = current + 1;
		tTemp.t[2] = current + 2;

		tris.push_back(tTemp);

		tris[i] = tTemp;

		current += 3;
	}

	glutPostRedisplay();
}


void Shape::normalisePoints()
{
	for(int i = 0; i < vertices.size(); i++)
	{
		vertices[i].normalise();
	}
}


void Shape::switchDrawState()
{
	if(drawState == 0)
		drawState = 1;
	else
		drawState = 0;
}


void Shape::normaliseMidPoints()
{
	for(int i = 0; i < centroids.size(); i++)
	{
		centroids[i].midpoint.normalise();
	}
}

void Shape::randomisePoints()
{
	Vector3 *temp = new Vector3();
	randVertices *holding = new randVertices();
	std::string choice;
	int i = 0;

	if(randVert.size() > 0)
		randVert.clear();

	std::cout << "Enter number of points[20, 80, 320, 1280, 5120 ,20480]: ";
	std::getline(std::cin, choice);

	if(choice == "20")
		i = 20;
	else if( choice == "80")
		i = 80;
	else if(choice == "320")
		i = 320;
	else if(choice == "1280")
		i = 1280;
	else if(choice == "5120")
		i = 5120;
	else if(choice == "20480")
		i = 20480;
	else
		i = 20;

	for(int j = 0; j < i; j++)
	{
		temp->x = temp->getRandom();
		temp->y = temp->getRandom();
		temp->z = temp->getRandom();

		holding->pos = *temp;

		randVert.push_back(*holding);
	}

	for(int i = 0; i < randVert.size(); i++)
	{
		randVert[i].pos.normalise();
	}

	delete(temp);
	delete(holding);
}

void Shape::switchDrawMode()
{
	if(tessOrRand == 1)
		tessOrRand = 0;
	else
		tessOrRand = 1;
}

void Shape::distanceToEveryPoint()
{
	Vector3 *temp = new Vector3();
	float d = 0;

	if(tessOrRand == 1)	//Calcuate distance between every point for tessellation
	{
		for(int i = 0; i < centroids.size(); i++)
		{
			for(int j = 0; j < centroids.size(); j++)
			{
				if(j == i)
				{
					//Do nothing
				}
				else
				{
					d = acos(temp->dotProduct(&centroids[i].midpoint, &centroids[j].midpoint));
					centroids[i].distance.push_back(d);
				}
			}
		}
	}
	else		//Calculate distance between every point for random
	{
		for(int i = 0; i < randVert.size(); i++)
		{
			for(int j = 0; j < randVert.size(); j++)
			{
				if(j == i)
				{
					//Do nothing
				}
				else
				{
					d = acos(temp->dotProduct(&randVert[i].pos, &randVert[j].pos));
					randVert[i].pointDistance.push_back(d);
				}
			}
		}
	}
}

void Shape::pointSample()
{
	if(randVert.size() == 0)
		randomisePoints();

	Vector3 *circleCenter = new Vector3();
	float radius = 0.75;
	float d = 0;
	int inside = 0;

	circleCenter->x = circleCenter->getRandom();
	circleCenter->y = circleCenter->getRandom();
	circleCenter->z = circleCenter->getRandom();

	circleCenter->normalise();

	for(int i = 0; i < randVert.size(); i++)
	{
		d = acos(circleCenter->dotProduct(circleCenter, &randVert[i].pos));

		if(d <= radius)
			inside++;
	}

	std::cout << "Points inside sample space: " << inside << std::endl;
}
