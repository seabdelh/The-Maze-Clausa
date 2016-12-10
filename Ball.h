#pragma once
#include "GLTexture.h"
#include <glut.h>

class Ball
{
public:
	GLdouble radius;
	GLUquadricObj *quadratic = nullptr;   // Storage For Our Quadratic Objects  										  //texture
	GLTexture tex_ball;

public:
	Ball();
	void drawBall(GLdouble x, GLdouble y, GLdouble z);
	void moveBall(GLdouble startX, GLdouble startY, GLdouble startZ, GLdouble endX, GLdouble endY, GLdouble endZ);
};