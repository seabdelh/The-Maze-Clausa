#include <string>

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
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
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
bool startBallMove = false;
bool ** maze;
int n = -1;
double camera_current_ang = 0;
int rotBY90 = 0;
double game_status_time = 0;

bool slow_motion_pressed = false;
double slow_motion_activation_time = 10; //10 secs

bool wall_breaker_pressed = false;
double wall_breaker_activation_time = 10; //10 secs

bool sound_of_time_fast_not_run_before = true;


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

void printString(double x, double y, double z, double r, double g, double b, std::string s) {

	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);

	for (unsigned int i = 0; i < s.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);

}

void ready_for_2D()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WidthX, 0, HeightY, -5, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void ready_for_3D()
{

	//to switch back to 3D after printing
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void to_game_over() {
	//3D stuff 
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
	glShadeModel(NULL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_NORMALIZE);
	ready_for_2D();
}

void timer(int k) {

	if (game_start && !game_over) {
		if (camera_current_ang < 360 && !startBallMove) {
			if (camera_current_ang < 360)
				camera_current_ang += 1;
		}
		else {
			startBallMove = true;
			game_status_time += 0.01;

			if (slow_motion_activation_time <= 0) {
				ball.slowmotion = false;
				if (sound_of_time_fast_not_run_before) {
					PlaySound(TEXT("sound/time fast.wav"), NULL, SND_ASYNC);
					sound_of_time_fast_not_run_before = false;
				}
			}
			else {
				slow_motion_activation_time -= 0.01;
			}

			if (rotBY90 > 0) {
				camera_current_ang += 1;
				rotBY90--;
			}
			else if (rotBY90 < 0) {
				camera_current_ang += -1;
				rotBY90++;
			}
			ball.moveBall();

			//wall breaker power up
			if (wall_breaker_pressed&&wall_breaker_activation_time >= 0) {
				if (amICollide(ball.moveZ + wallLength / 2, ball.moveX + wallLength / 2, ball.radius)
					&& coliisionCellSide != 1 && coliisionCellSide != 2 && !(collisionCell%n == 0 && coliisionCellSide == 3)
					&& !(collisionCell < n && coliisionCellSide == 0)) {

					maze[collisionCell][coliisionCellSide] = false;
					PlaySound(TEXT("sound/wall breaking.wav"), NULL, SND_ASYNC);
				}
				else {
					game_over = amICollide(ball.moveZ + wallLength / 2, ball.moveX + wallLength / 2, ball.radius);
				}

				wall_breaker_activation_time -= 0.01; //10 mili
			}
			else {
				game_over = amICollide(ball.moveZ + wallLength / 2, ball.moveX + wallLength / 2, ball.radius);
			}

			if (game_over)
				to_game_over();
		}
		glutPostRedisplay();
		glutTimerFunc(10, timer, ++k);
	}
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
	amIWinner = false;
	game_start = true;
	isGridCreatedBefore = false;
	game_status_time = 0;
	rotBY90 = 0;
	camera_current_ang = 0;
	slow_motion_pressed = false;
	wall_breaker_pressed = false;
	sound_of_time_fast_not_run_before = true;

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
	glBindTexture(GL_TEXTURE_2D, NULL);
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
			printString(WidthX / 2 - 300, HeightY / 2 + 50, 0, 1, 0, 0, " Your Time : " + std::to_string(game_status_time));
		}
		else {
			setupCamera();
			glBindTexture(GL_TEXTURE_2D, NULL); //no texture by default: if you want to use it activate it then disable it again  

			glPushMatrix();
			glTranslated(0.5*n*wallLength, 0, 0.5*n*wallLength);
			glRotated(camera_current_ang, 0, 1, 0);
			glTranslated(-0.5*n*wallLength, 0, -0.5*n*wallLength);
			drawCord();
			// test of calls 		
			drawMaze(maze, n);
			drawStar(n*wallLength - 0.5*wallLength, 0.5*0.2*wallLength, n*wallLength - 0.5*wallLength);
			//createMazeSingleWall (0,0,0, true  ) ;
			//createMazeSingleWall (0,0,0, false  ) ;
			ball.drawBall(wallLength / 2, 0.5*0.2*wallLength, wallLength / 2); //this line draw the ball at <x,y,z> but when it does , the light goes
			glPopMatrix();

			//game status
			ready_for_2D(); //2D

			glPushMatrix();
			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glVertex3f(5, HeightY, 0.0);
			glVertex3f(WidthX, HeightY, 0.0);
			glVertex3f(WidthX, HeightY - 40, 0.0);
			glVertex3f(5, HeightY - 40, 0.0);
			glEnd();
			glPopMatrix();

			printString(WidthX / 6, HeightY - 24, 0, 1, 0, 0, "Time:" + std::to_string(game_status_time));

			//wall breaker special power
			if (game_status_time > 2) {
				if (!wall_breaker_pressed) {
					printString(4 * WidthX / 6, HeightY - 24, 0, 1, 0, 0, "Wall Breaker: press (x)");
				}
				else if (wall_breaker_activation_time >= 0) {
					printString(4 * WidthX / 6, HeightY - 24, 0, 1, 0, 0, "Wall Breaker: (" + std::to_string(wall_breaker_activation_time) + ")");
				}
				else {
					printString(4 * WidthX / 6, HeightY - 24, 0, 1, 0, 0, "Wall Breaker: (disabled)");
				}
			}

			//time breaker special power
			if (game_status_time > 1) {
				if (!slow_motion_pressed) {
					printString(2 * WidthX / 6, HeightY - 24, 0, 1, 0, 0, "Time Breaker: press (c)");
				}
				else {
					if (slow_motion_activation_time >= 0) {
						printString(2 * WidthX / 6, HeightY - 24, 0, 1, 0, 0, "Time Breaker: (" + std::to_string(slow_motion_activation_time) + ")");
					}
					else {
						printString(2 * WidthX / 6, HeightY - 24, 0, 1, 0, 0, "Time Breaker: (disabled)");
					}
				}
			}

			ready_for_3D();  //3D again
		}
	}
	glFlush();
	glutSwapBuffers();
}
void LoadAssets() { // nadaaaaa
					// Loading Model files
	model_star.Load("Models/Toy.3ds");
	ball.tex_ball.Load("Textures/Ball.bmp");
	texx.LoadBMP("Textures/wall final 4k.bmp");
	texx2.LoadBMP("Textures/Wall.bmp");
	//  createMazeSingleWall.load("Textures/wall.bmp");

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
	if (game_start && game_over) {
		if (key == 'z') {
			game_over = false;
			game_start = false;
			glutIdleFunc(Anim_Move); //enabling the AnimFunction again
			glutPostRedisplay();
		}
	}
	//for testing 
	if (game_start && !game_over) {
		switch (key) {
		case 'w':
			camera.moveY(d);
			break;
		case 's':
			camera.moveY(-d);
			break;
		case 'a':
			camera.moveX(d);
			break;
		case 'd':
			camera.moveX(-d);

			break;
		case 'q':
			camera.moveZ(d);
			glutPostRedisplay();
			break;
		case 'e':
			camera.moveZ(-d);
			break;
		case 'c':
			if (!slow_motion_pressed) {
				ball.slowmotion = true;
				slow_motion_pressed = true;
				PlaySound(TEXT("sound/time slow.wav"), NULL, SND_ASYNC);
				slow_motion_activation_time = 10;
			}
			break;
		case 'x':
			if (!wall_breaker_pressed) {
				wall_breaker_pressed = true;
				wall_breaker_activation_time = 10;
			}
			break;
		}
	}
	if (key == GLUT_KEY_ESCAPE) {
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
		if (rotBY90 == 0) {
			switch (key)
			{
			case GLUT_KEY_LEFT:
				switch (ball.state)
				{
				case 1:ball.state = 3;  break;
				case 2:ball.state = 4;  break;
				case 3:ball.state = 2;  break;
				case 4:ball.state = 1;  break;

				}
				rotBY90 = -90; break;
			case GLUT_KEY_RIGHT:
				switch (ball.state)
				{
				case 1:ball.state = 4; break;
				case 2:ball.state = 3; break;
				case 3:ball.state = 1; break;
				case 4:ball.state = 2; break;
				}
				rotBY90 = 90; break;
			}
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