#pragma once
#include "GLTexture.h"
#include <glut.h>

class Ball
{
public:
	GLdouble radius;
	GLUquadricObj *quadratic = nullptr;   // Storage For Our Quadratic Objects  										  //texture
	GLTexture tex_ball;
	//motion
	GLdouble moveX = 0;
	GLdouble moveZ = 0;
	GLdouble movefactor = 0.06; //move in z axis by this factor
	GLdouble rotfactor = 1.5;
	GLdouble rotaroundZ = 0;//1 or zero
	GLdouble rotaroundX = 0;//1 or zero
	GLdouble rotangle = 0;
	//slow motion flag //true-->slow down translation and rotation 
	BOOLEAN slowmotion = false;
	int state = 1;//1 forward; 2 right ; 3 left; 4 backward


public:
	Ball();
	void drawBall(GLdouble x, GLdouble y, GLdouble z);
	void moveBall();
	void SpecialInput(int key, int x, int y);

};