#include "Ball.h"


Ball::Ball() {
		quadratic = gluNewQuadric();         // Create A Pointer To The Quadric Object  
		tex_ball.Load("Textures/ground.bmp"); //change later /in load assets 

		gluQuadricTexture(quadratic, GL_TRUE);      // Create Texture Coords  
		gluQuadricNormals(quadratic, GLU_SMOOTH);   // Create Smooth Normals  



	}

	void Ball:: drawBall(GLdouble x, GLdouble y, GLdouble z)
	{
		
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, tex_ball.texture[0]);

		glTranslatef(x + moveX, y, z + moveZ);
		glRotated(rotangle, rotaroundX, 0, rotaroundZ);
		gluSphere(quadratic, radius, 200, 200);
		glBindTexture(GL_TEXTURE_2D, NULL);    //disabling the texture again 

		glPopMatrix();

	
	}
	void Ball:: moveBall() { //glutidlefunc

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
			//ball was moving forward 
		case 1: { if (rightclicked) { //move in the +ve x-axis
			rotaroundZ = 1; //rotate in x
			rotaroundX = 0;
			if (!slowmotion) {
				moveX += movefactor;
				rotangle += rotfactor;
			}
			else {
				moveX += movefactor / 2;
				rotangle += rotfactor / 2;
			}
		}
				else if (leftclicked) {//move in the -ve x-axis
					rotaroundZ = 1; //rotate in x
					rotaroundX = 0;
					if (!slowmotion) {
						moveX -= movefactor;
						rotangle += rotfactor;

					}
					else {
						moveX -= movefactor / 2;
						rotangle += rotfactor / 2;

					}
				}
				else if (downclicked) {//move in the -ve z-axis
					rotaroundX = 1;//rotate in z
					rotaroundZ = 0;
					if (!slowmotion) {
						moveZ -= movefactor;
						rotangle -= rotfactor;
					}
					else {
						moveZ -= movefactor / 2;
						rotangle -= rotfactor / 2;
					}
				}
		}
				break;
				//ball was moving to the right
		case 2: {
			if (rightclicked) { //move in the -ve z-axis
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
			else if (leftclicked) {//move in the +ve z-axis
				rotaroundZ = 0;
				rotaroundX = 1;
				if (!slowmotion) {
					moveZ += movefactor;
					rotangle += rotfactor / 2;
				}
				else {
					moveZ += movefactor / 2;
					rotangle += rotangle / 2;
				}
			}
			else if (downclicked) {//move in the -ve x-axis
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
		}
				break;
				//ball was moving to the left
		case 3: {
			if (rightclicked) { //move in the +ve z-axis
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
			else if (leftclicked) {//move in the -ve z-axis
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
			else if (downclicked) {//move in the +ve x-axis
				rotaroundZ = 1;
				rotaroundX = 0;
				if (!slowmotion) {
					moveX += movefactor;
					rotangle += rotfactor;

				}
				else {
					moveX += movefactor / 2;
					rotangle += rotfactor / 2;

				}
			}
		}
				break;
				//ball moving backward
		case 4: {
			if (rightclicked) { //move in the -ve x-axis
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
			else if (leftclicked) {//move in the +ve x-axis
				rotaroundZ = 1;
				rotaroundX = 0;

				if (!slowmotion) {
					moveX += movefactor;
					rotangle += rotfactor;

				}
				else {
					moveX += movefactor / 2;
					rotangle += rotfactor / 2;

				}
			}
			else if (downclicked) {//move in the +ve z-axis
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
		}
				break;
		default:
			//always move in the +ve z-axis unless stated otherwise
			//move in straight line 
		{
			rotaroundX = 1;
			rotaroundZ = 0;
			if (!slowmotion) {
				rotangle += rotfactor;
				moveZ += movefactor;
			}
			else {
				//slow down to half the speed
				rotangle += rotfactor / 2;
				moveZ += movefactor / 2;
			}

		}
		break;
		}		glutPostRedisplay();

	}
	


void Ball::SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	
	case GLUT_KEY_DOWN:
		//do something here
		downclicked = true;

		rightclicked = false;
		leftclicked = false;
		switch (state)
		{
		case 1:state = 4; break;
		case 2:state = 3; break;
		case 3:state = 2; break;
		case 4:state = 1; break;

		default:
				break;
		}
		break;
	case GLUT_KEY_LEFT:
		//do something here
		leftclicked = true;
		downclicked = false;
		rightclicked = false;
		switch (state)
		{
		case 1:state = 3; break;
		case 2:state = 1; break;
		case 3:state = 4; break;
		case 4:state = 2; break;

		default:
			break;
		}
		break;
		break;
	case GLUT_KEY_RIGHT:
		//do something here
		rightclicked = true;
		downclicked = false;
		leftclicked = false;
		switch (state)
		{
		case 1:state = 2; break;
		case 2:state = 4; break;
		case 3:state = 1; break;
		case 4:state = 3; break;

		default:
			break;
		}
		break;
		break;
	}


	glutPostRedisplay();
}

	 



