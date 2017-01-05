#include <iostream>
#include "Vector3.h"
#include "Shape.h"
#include "GL/freeglut.h"
#include <time.h>

void display();
void init();
void keyRead(unsigned char, int, int);

const int screenWidth = 1280;
const int screenHeight = 720;

Shape *f = new Shape();

int main(int argc, char** argv)
{
	srand(time(NULL)); //Seed the random number Gen

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Ico-Subdivide");

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyRead);

	init();

	glutMainLoop();

	return 0;
}

void init()
{
	glViewport(0, 0, screenWidth, screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screenWidth/(GLfloat)screenHeight, 0.0f, 100.0f);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	f->loadMesh("icohedron.obj");	
}

void display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	glTranslatef(0, 0, -5);
	glRotatef(f->getXAngle(), 1, 0, 0);
	glRotatef(f->getYAngle(), 0, 1, 0);
	f->renderMesh();

	glFlush();
	glutSwapBuffers();
}

void keyRead(unsigned char k, int x, int y)
{
	switch(k)
	{
		case 'd':
			f->incY();
		break;

		case 'a':
			f->decY();
		break;

		case 'w':
			f->incX();
		break;

		case 's':
			f->decX();
		break;

		case 'r':
			f->reset();
		break;

		case 'p':
			f->distanceToEveryPoint();
			f->printCVS();
		break;

		case 'q':
			f->subdivide();
		break;

		case 'f':
			f->switchDrawState();
		break;

		case 'e':
			f->switchDrawMode();
		break;

		case 't':
			f->pointSample();
		break;

		default:
		break;
	}
}
