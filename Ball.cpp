#include "Ball.h"

Ball::Ball() {
	quadratic = gluNewQuadric();         // Create A Pointer To The Quadric Object  
	tex_ball.Load("Textures/sky4-jpg.bmp"); //change later /in load assets

	gluQuadricTexture(quadratic, GL_TRUE);      // Create Texture Coords  //this line has a weird effect on other elements texture or the lights
	gluQuadricNormals(quadratic, GLU_SMOOTH);   // Create Smooth Normals  
	
	
}
void Ball::drawBall(GLdouble x, GLdouble y, GLdouble z)
{
	/*
	glPushMatrix();
	glTranslated(x, y, z);
	glutSolidSphere(radius, 100, 100);
	glPopMatrix();*/


	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ball.texture[0]);  //activating the texture to create the ball
	glTranslatef(x, y, z);
	gluSphere(quadratic, radius, 100, 100);
	glBindTexture(GL_TEXTURE_2D, NULL);    //disabling the texture again                                                    //off when you move the camera

	glPopMatrix();

}
void Ball::moveBall(GLdouble startX, GLdouble startY, GLdouble startZ, GLdouble endX, GLdouble endY, GLdouble endZ) {
	//move in straight line 
}



