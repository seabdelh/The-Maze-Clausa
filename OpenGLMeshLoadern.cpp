
#include <string>
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>


int WIDTH = 1280;
int HEIGHT = 720;


double WidthX= glutGet(GLUT_SCREEN_WIDTH);
double HeightY = glutGet(GLUT_SCREEN_HEIGHT);
double i=0;

GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_star; // hna el star

int sec=0;
int min=0;
// std::string parse(int x) {
//	std::ostringstream buffer; 
//    buffer << x;
//    return buffer.str(); 
//}
//
//void printString(double x, double y, double z, double r, double g, double b, std::string s){
//	
//	glColor3f(r, g, b);
//	glRasterPos3f(x, y, z);
//
//	for (unsigned int i = 0; i < s.length(); i++)
//		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
//
//}
//int timer(){
//  
//  if (sec<60){
//    sec++ ;
//  }else {
//    sec=0;
//    min++;
//  }
//  glutPostRedisplay();
//}
//
//void printMessage(float x, float y, float z, void *font, std::string message)
//{
//	//this method resposible for printing 'message' on the screen
//	glColor3f(0.726, 0, 0);
//	glRasterPos3f(x, y, z);
//	int len, i;
//	len = message.length();
//	for (i = 0; i < len; i++) {
//		if (message[i] == '\n') {
//			y -= 28;
//			glRasterPos3f(x, y, z);
//		}
//		glutBitmapCharacter(font, message[i]);
//	}
//}

//to use:printMessage(x,y,z, GLUT_BITMAP_TIMES_ROMAN_24, "Time: " + std::to_string(timer));
//time is int :) 

void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

void drawStar(){ //nada p2
    glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(0.02, 0.02, 0.02);
	model_star.Draw();
	glPopMatrix();

}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 

	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	//draw star model
	drawStar(); //nada p2
	//printMessage(0,0,0, GLUT_BITMAP_TIMES_ROMAN_24, "Time: " + std::to_string(timer));
//   printString(0, 0, 0, 0, 0, 0 ,"min :"+ parse(min) +"sec :"+ parse(sec));
	glutSwapBuffers();
}

void LoadAssets(){ // nadaaaaa
	// Loading Model files
	model_star.Load("Models/Toy.3ds");
	// Loading texture files
}

void main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(100, 150);
	glutCreateWindow(title);
	glutDisplayFunc(myDisplay);
	myInit();
	LoadAssets();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}