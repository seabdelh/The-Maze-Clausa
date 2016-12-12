#include "Ball.h"


Ball::Ball() {
	quadratic = gluNewQuadric();         // Create A Pointer To The Quadric Object  
	tex_ball.Load("Textures/Ball(1).bmp"); //change later /in load assets	

	gluQuadricTexture(quadratic, GL_TRUE);      // Create Texture Coords  
	gluQuadricNormals(quadratic, GLU_SMOOTH);   // Create Smooth Normals  

}

void Ball::drawBall(GLdouble x, GLdouble y, GLdouble z)
{

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ball.texture[0]);

	glTranslatef(x + moveX, y, z + moveZ);
	glRotated(rotangle, rotaroundX, 0, rotaroundZ);
	gluSphere(quadratic, radius, 200, 200);
	glBindTexture(GL_TEXTURE_2D, NULL);    //disabling the texture again 

	glPopMatrix();

}
void Ball::moveBall() { //glutidlefunc

	////always move in the +ve z-axis unless stated otherwise
	////move in straight line 
	//if(!slowmotion){
	//	rotangle += rotfactor;
	//	moveZ += movefactor;
	//}
	//else {
	//	//slow down to half the speed
	//	rotangle += rotfactor/2;
	//	moveZ += movefactor/2;
	 //}

	switch (state)
	{
	case 1: {
		//move in the +ve Z-axis
		 //rotate in x	
		rotaroundZ = 0;
		rotaroundX = 1;

		if (!slowmotion) {
			moveZ += movefactor;
			rotangle += rotfactor;
		}
		else {
			moveZ += movefactor / 2;
			rotangle += rotfactor / 2;
		}
	}
			break;
			//ball was moving to the right
	case 2: {
		//move in the -ve z-axis
		rotaroundZ = 0;
		rotaroundX = 1;
		if (!slowmotion) {
			moveZ -= movefactor;
			rotangle -= rotfactor;

		}
		else {
			moveZ -= movefactor / 2;
			rotangle -= rotfactor / 2;

		}
	}
			break;
	case 3: {
		//move in the +ve x-axis
		rotaroundZ = 1;
		rotaroundX = 0;
		if (!slowmotion) {
			moveX += movefactor;
			rotangle -= rotfactor;
		}
		else {
			moveX += movefactor / 2;
			rotangle -= rotfactor / 2;

		}
	}
			break;
			//ball moving backward
	case 4: {
		//move in the -ve x-axis
		rotaroundZ = 1;
		rotaroundX = 0;
		if (!slowmotion) {
			moveX -= movefactor;
			rotangle -= rotfactor;
		}
		else {
			moveX -= movefactor / 2;
			rotangle -= rotfactor / 2;
		}
	}
			break;
	}		glutPostRedisplay();

}



void Ball::SpecialInput(int key, int x, int y)
{

	glutPostRedisplay();
}





