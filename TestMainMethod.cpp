//#include <string>
//#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "glut.h" 
#include "mazeWall.h"

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

double WidthX = glutGet(GLUT_SCREEN_WIDTH);
double HeightY = glutGet(GLUT_SCREEN_HEIGHT);
double ratio = WidthX/HeightY ;


//harde coded 
bool ** mazeWallMapTest ; 
int n = 3 ;

//end of hard coded


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

void drawCord() {
	glPushMatrix();
	glColor3f(1, 0, 0); //z
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 0.5*90);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1); // y
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0.5*30, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 1, 0); //x
	glVertex3d(0, 0, 0);
	glVertex3d(0.5*30, 0, 0);
	glEnd();
	glPopMatrix();
}
void axis(double length) { // draw a z-axis, with cone at end  
		glPushMatrix(); 
		
		glBegin(GL_LINES); 
		glVertex3d(0, 0, 0);
		glVertex3d(0,0,length); // along the z-axis 
		glEnd(); 

		glTranslated(0, 0,length -0.2); 
		glutWireCone(0.04, 0.2, 12, 9); 
	
		glPopMatrix(); 
	}  


void display(void){  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(95, ratio, 0.001, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(5, 50, 5, 5, 0, 30, 0.0, 1.0, 0.0); 
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       

 // code here

	drawCord();
	// test of calls 
		mazeWallMapTest[5][3]=false ;
		mazeWallMapTest[4][3]=false ;
		mazeWallMapTest[4][0]=false ;
		mazeWallMapTest[8][3]=false ;// 3 =1  && 0 = 2 lakn a7na daymn bn3mll check 3la 3 & 0 
	drawMaze ( mazeWallMapTest , 3) ;
	

	//createMazeSingleWall (0,0,0, true  ) ;
	//createMazeSingleWall (0,0,0, false  ) ;

	

	//end of code 
	
	glFlush(); 
}

void key(unsigned char k, int x,int y){
	
	glutPostRedisplay();
}

void keyUp(unsigned char k, int x,int y){
	if(k=='q')
		exit(0);	
}


void passM(int oldx,int oldy){
	

	glutPostRedisplay();
}


void actM(int button, int state, int x, int y){  // player can move the trajectory left/right/up/down by keyboard
	if(button==GLUT_KEY_LEFT){
		
	}
	if(button==GLUT_KEY_RIGHT){
		
	}
	if(button==GLUT_KEY_UP){
		
	}
	if(button==GLUT_KEY_DOWN ){
		
	}
	glutPostRedisplay();
}


void Anim_move(){   

	glutPostRedisplay();
}


void Keyboard(unsigned char key, int x, int y) {
	float d = 1;

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
		break;
	case 'e':
		camera.moveZ(-d);
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void main(int argc, char **argv) {
		glutInit(&argc, argv);  
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 
		glutCreateWindow("testMazeWall"); 


				//start test 
			mazeWallMapTest= new bool*[n *n];
for(int i = 0; i < n*n; ++i)
    mazeWallMapTest[i] = new bool[4];

// put wall by default 
for(int i = 0; i < n*n; ++i) {
for(int j = 0; j < 4; ++j) {
    mazeWallMapTest[i][j]=true;
// printf("\n deh value l pointer l bool : %p \n",mazeWallMap[i][j] ) ;
}
}
		//end of test


		glutDisplayFunc(display);
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f); 
		glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
		glutIdleFunc(Anim_move);
		
	   //glutPassiveMotionFunc(passM);
	  //glutSpecialFunc(actM);
		//glutKeyboardFunc(key);
		//glutKeyboardUpFunc(keyUp);
	 
	 // glutTimerFunc(0,time,0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);
		glEnable(GL_COLOR_MATERIAL);

		glShadeModel(GL_SMOOTH);
	  



		glutFullScreen();
		glutMainLoop();
}