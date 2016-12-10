//#include "Texturebuilder.h"
#include "GLTexture.h"
#include <glut.h>

class Ball
{
public:
	GLdouble radius = 0.5;
	GLUquadricObj *quadratic = nullptr;   // Storage For Our Quadratic Objects  
	//texture
	GLTexture tex_ball;

	void drawBall(GLdouble x, GLdouble y, GLdouble z)
	{/*
		glPushMatrix();
		glTranslated(x, y, z);
		glutSolidSphere(radius, 100, 100);
		glPopMatrix();*/
		quadratic = gluNewQuadric();         // Create A Pointer To The Quadric Object  
		gluQuadricTexture(quadratic, GL_TRUE);      // Create Texture Coords  
		gluQuadricNormals(quadratic, GLU_SMOOTH);   // Create Smooth Normals  
		
		//tex_ball.Load("Textures/ground.bmp"); //change later /in load assets 

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex_ball.texture[0]);
		glEnable(GL_DEPTH_TEST);

		glPushMatrix();
		glTranslatef(x, y, z);
		gluSphere(quadratic, radius, 200, 200);
		glPopMatrix();
	}
	void moveBall(GLdouble startX,GLdouble startY ,GLdouble startZ, GLdouble endX,GLdouble endY,GLdouble endZ) {
			//move in straight line 


	}
	
	 


};



