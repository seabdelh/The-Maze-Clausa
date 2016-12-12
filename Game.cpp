#include <string>
#include <time.h>

#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
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

Model_3DS model_star; // hna el star
Ball ball;
double WidthX = glutGet(GLUT_SCREEN_WIDTH);
double HeightY = glutGet(GLUT_SCREEN_HEIGHT);
bool isEasy = false, isMedium = false, isHard = false, game_start = false, game_over = false;
double arrowX = WidthX / 2 - 360, arrowY = HeightY / 2 + 100, level = 3; // level 3 means that this level does not exist 
GLuint texID;
GLuint texID2;
double ratio = WidthX / HeightY;
//maze 
bool ** maze;
int n = 3;
double camera_rot_ang = 360;


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

	Camera() {  }//never used but force

	Camera(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
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

	//camera stuff
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 16 / 9, 0.001, 300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera.look();
}


void timer(int k) {

	if (camera_rot_ang != 0) {
		camera_rot_ang -= 1;
	}

	glutPostRedisplay();
	glutTimerFunc(10, timer, ++k);

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
	setupLights();

	game_start = true;
	ball = Ball();
	ball.radius = 1;
	n = (level + 1) * 3;
	Maze m = Maze(n, 0, n*n - 1);
	maze = m.map;
	glutIdleFunc(NULL); //stopping the AnimFunction after beginning the game
	glutTimerFunc(0, timer, 0);
	camera = Camera(0.5*n*wallLength, 3 * n / 4 * wallLength, -0.2*wallLength, 0.5*n*wallLength, 0, 0.5*n*wallLength, 0, 1, 0);

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
void gameover() {//new nada p2

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glBindTexture(GL_TEXTURE_2D, texID2);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.7, 0);
	glVertex3f(WidthX + 1200, 0, 0);
	glTexCoord2f(1.7, 1.7);
	glVertex3f(WidthX + 1200, HeightY + 700, 0);
	glTexCoord2f(0, 1.7);
	glVertex3f(0, HeightY + 700, 0);
	glEnd();
	glPopMatrix();

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
} //new
void welcomegame() { //new nada p2

					 //glColor3f(0.6, 0.6, 0.6);
	glPushMatrix();
	glColor3f(0.0f, 0.20f, 0.10f); // green color
								   //  glColor3f(0.6, 0.6, 0.6);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1, 0.0f); glVertex3f(WidthX + 1200, 0, 0);
	glTexCoord2f(1, 1); glVertex3f(WidthX + 1200, HeightY + 600, 0);
	glTexCoord2f(0.0f, 1); glVertex3f(0, HeightY + 600, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NULL);
	glPopMatrix();
	//glColor3f(1, 1, 1);
}
void drawStar(int x, int y, int z) { //nada p2
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.02, 0.02, 0.02);
	model_star.Draw();
	glPopMatrix();

}

void display(void) {

	if (!game_start) {
		glClear(GL_COLOR_BUFFER_BIT);
		welcomegame();
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
		if (game_over == true) {  //////game over Part
			glClear(GL_COLOR_BUFFER_BIT);
			gameover();
			std::string s2 = " GAME OVER";
			printString(WidthX / 2 - 250, HeightY / 2 + 100, 0, 1, 0, 0, s2);
			printString(WidthX / 2 - 300, HeightY / 2 + 50, 0, 1, 0, 0, " Your Score : " + parse(level));
		}
		else {

			setupCamera();
			glBindTexture(GL_TEXTURE_2D, NULL); //no texture by default: if you want to use it activate it then disable it again  

			glPushMatrix();
			glTranslated(0.5*n*wallLength, 0, 0.5*n*wallLength);
			glRotated(camera_rot_ang, 0, 1, 0);
			glTranslated(-0.5*n*wallLength, 0, -0.5*n*wallLength);
			drawCord();
			// test of calls 		
			drawMaze(maze, n);
			drawStar(n*wallLength - 0.5*wallLength, 0.5*0.2*wallLength, n*wallLength - 0.5*wallLength);
			//createMazeSingleWall (0,0,0, true  ) ;
			//createMazeSingleWall (0,0,0, false  ) ;
			ball.drawBall(wallLength / 2, wallLength, wallLength / 2); //this line draw the ball at <x,y,z> but when it does , the light goes
			glPopMatrix();

		}
	}
	glFlush();
	glutSwapBuffers();
}
void LoadAssets() { // nadaaaaa
					// Loading Model files
	model_star.Load("Models/Toy.3ds");
	// Loading texture files
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
	if (key == 'z') {
		game_over = true;
	}
	else game_over = false;

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

			init();
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
	LoadAssets();
	loadBMP(&texID, "textures/dpic.bmp", false); //welcome game
	loadBMP(&texID2, "textures/max (3).bmp", false);//game over

	gluOrtho2D(0.0, WidthX, 0.0, HeightY);

	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutIdleFunc(Anim_Move);

	glutFullScreen();
	glutMainLoop();
}