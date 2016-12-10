#include <string>
#include <time.h>

#include "TextureBuilder.h"
#include "mazeWall.h"
#include "glut.h" 
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include "Maze.h"
#include "Ball.h"

#pragma comment(lib, "legacy_stdio_definitions.lib")

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

Ball ball;
double WidthX = glutGet(GLUT_SCREEN_WIDTH);
double HeightY = glutGet(GLUT_SCREEN_HEIGHT);
bool isEasy = false, isMedium = false, isHard = false, game_start = false;
double arrowX = WidthX / 2 - 360, arrowY = HeightY / 2 + 100, level = 3; // level 3 means that this level does not exist 
GLuint texID;
double ratio = WidthX / HeightY;
//maze 
bool ** maze;
int n = 3;



class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f &v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f &v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = -20.0f, float eyeY = 25.0f, float eyeZ = 20, float centerX = 20, float centerY = 0.0f, float centerZ = 20, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 16 / 9, 0.001, 300);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void setupLights() {
	//light config
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	GLfloat light_position[] = { 0.5*n*wallLength, 10.0f, 0.5*n*wallLength, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	GLfloat lightIntensity[] = { 0.5, 0.5, 0.5, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	//material config
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	GLfloat material_shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

}
void init()
{
	//3D stuff 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);	
}


void drawCord() {
	glPushMatrix();
	glColor3f(1, 0, 0); //z
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 0.5 * 90);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1); // y
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0.5 * 30, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 1, 0); //x
	glVertex3d(0, 0, 0);
	glVertex3d(0.5 * 30, 0, 0);
	glEnd();
	glPopMatrix();
}
std::string parse(int x) {
	std::ostringstream buffer;
	buffer << x;
	return buffer.str();
}
void printString(double x, double y, double z, double r, double g, double b, std::string s) {

	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);

	for (unsigned int i = 0; i < s.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);

}
void drawArrow() {
	glPushMatrix();
	glTranslated(arrowX, arrowY, 0);
	glRotated(-90, 0, 0, 1);
	glPushMatrix();
	glScaled(0.3, 0.3, 1);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); //red
	glVertex3f(150.0f, 200.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);//green
	glVertex3f(100.0f, 100.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);//blue
	glVertex3f(200.0f, 100.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(30, 15, 0);
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);//blue
	glVertex3f(10.0f, 10.0f, 0.0f);
	glVertex3f(10.0f, 20.0f, 0.0f);
	glVertex3f(20.0f, 20.0f, 0.0f);
	glVertex3f(20.0f, 10.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}


void display(void) {
	
	if (!game_start) {
		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix();
		glColor3f(0.10f, 0.20f, 0.10f);
		glBindTexture(GL_TEXTURE_2D, texID); //enabling the texture again  
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
		glTexCoord2f(1, 0.0f); glVertex3f(WidthX + 100, 0, 0);
		glTexCoord2f(1, 1); glVertex3f(WidthX + 600, HeightY, 0);
		glTexCoord2f(0.0f, 1); glVertex3f(0, HeightY + 600, 0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, NULL);    //disabling the texture again  
		glPopMatrix();

		std::string s = " Choose The Maze Mode :";
		printString(WidthX / 2 - 360, HeightY / 2 + 100, 0, 1, 0, 0, s);
		std::string t = " Easy ";
		printString(WidthX / 2 - 300, HeightY / 2 + 50, 0, 1, 0, 0, t);
		std::string o = " Medium  ";
		printString(WidthX / 2 - 300, HeightY / 2, 0, 1, 0, 0, o);
		std::string d = " Hard ";
		printString(WidthX / 2 - 300, HeightY / 2 - 50, 0, 1, 0, 0, d);

		std::string r = "  ESC : Exit  ";
		printString(WidthX / 2 - 750, HeightY / 2 - 400, 0, 1, 0, 0, r);

		std::string e = " press  --> : Enter  ";
		printString(WidthX / 2 - 600, HeightY / 2 - 400, 0, 1, 0, 0, e);

		drawArrow();

	}
	else {
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setupCamera();
		glBindTexture(GL_TEXTURE_2D, NULL); //no texture by default: if you want to use it activate it then disable it again  

		drawCord();
		// test of calls 		
		drawMaze(maze, n);
		//createMazeSingleWall (0,0,0, true  ) ;
		//createMazeSingleWall (0,0,0, false  ) ;

		ball.drawBall(wallLength / 2, wallLength, wallLength / 2); //this line draw the ball at <x,y,z> but when it does , the light goes

	}
	glFlush();
	glutSwapBuffers();
}

void Anim_Move() {
	if (arrowX == WidthX / 2 - 360 && arrowY == HeightY / 2 + 100) {
		isEasy = true;
		isHard = false;
		level = 0;
		//printf("iseasy flag:",isEasy);
		//printf("ismedium flag:",isMedium);
		//printf("ishard flag:",isHard);
	}
	else if (arrowX == WidthX / 2 - 360 && arrowY == HeightY / 2 + 50) {
		isMedium = true;
		isEasy = false;
		level = 1;
		//	printf("iseasy flag:",isEasy);
		//	printf("ismedium flag:",isMedium);
		//	printf("ishard flag:",isHard);
	}
	else if (arrowX == WidthX / 2 - 360 && arrowY == HeightY / 2) {
		isHard = true;
		isMedium = false;
		level = 2;
		/*printf("iseasy flag:",isEasy);
		printf("ismedium flag:",isMedium);
		printf("ishard flag:",isHard);*/
	}//else isHard=false;

	glutPostRedisplay();
}


void Keyboard(unsigned char key, int x, int y) {
	float d = 1;

	switch (key) {
	case 'w':
		camera.moveY(d);
		glutPostRedisplay();

		break;
	case 's':
		camera.moveY(-d);
		glutPostRedisplay();

		break;
	case 'a':
		camera.moveX(d);
		glutPostRedisplay();
		break;
	case 'd':
		camera.moveX(-d);
		glutPostRedisplay();

		break;
	case 'q':
		camera.moveZ(d);
		glutPostRedisplay();
		break;
	case 'e':
		camera.moveZ(-d);
		glutPostRedisplay();
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

}
void Special(int key, int x, int y) {

	if (!game_start) {
		if (key == GLUT_KEY_DOWN) {
			if (arrowX == WidthX / 2 - 360 && arrowY == HeightY / 2 + 100) {
				arrowY = HeightY / 2 + 50;
			}
			else if (arrowX == WidthX / 2 - 360 && arrowY == HeightY / 2 + 50) {
				arrowY = HeightY / 2;
			}
			else if (arrowX == WidthX / 2 - 360 && arrowY == HeightY / 2) {
				arrowY = HeightY / 2 + 100;
			}
			glutPostRedisplay();

		}
		if (key == GLUT_KEY_RIGHT) {
			if (level == 0) {
				printf("game start", 0);
			}
			else if (level == 1) {
				printf("game start", 1);
			}
			else if (level == 2) {
				printf("game start", 2);
			}
			game_start = true;
			init();

			setupLights();
			setupCamera();

			ball = Ball();
			ball.radius = 1;

			n = (level + 1) * 3;
			Maze m = Maze(n, 0, n*n - 1);
			maze = m.map;
			glutIdleFunc(NULL); //stopping the AnimFunction after beginning the game

			glutPostRedisplay();

		}
	}
	else {
		float a = 1.0;

		switch (key) {
		case GLUT_KEY_UP:
			camera.rotateX(a);
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			camera.rotateX(-a);
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			camera.rotateY(a);
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			camera.rotateY(-a);
			glutPostRedisplay();
			break;
		}
	}

}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("The Maze: Clausa");
	glutDisplayFunc(display);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);
	loadBMP(&texID, "textures/dpic.bmp", false); //welcome screen

	gluOrtho2D(0.0, WidthX, 0.0, HeightY);

	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutIdleFunc(Anim_Move);

	glutFullScreen();
	glutMainLoop();
}