#include <string>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include "TextureBuilder.h"
#include "glut.h"
#define GLUT_KEY_ESCAPE 27
double WidthX = glutGet(GLUT_SCREEN_WIDTH);
double HeightY = glutGet(GLUT_SCREEN_HEIGHT);
bool isEasy=false,isMedium=false,isHard=false ,game_start=false;
double arrowX= WidthX/2 -360 , arrowY= HeightY/2 +100  ,level =3; // level 3 means that this level does not exist 
GLuint texID;

std::string parse(int x) {
    std::ostringstream buffer;
    buffer << x;
    return buffer.str();
}
void printString(double x, double y, double z, double r, double g, double b, std::string s){
    
    glColor3f(r, g, b);
    glRasterPos3f(x, y, z);
    
    for (unsigned int i = 0; i < s.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    
}
void drawArrow(){
glPushMatrix();
    glTranslated(arrowX,arrowY,0);
	glRotated(-90,0,0,1);
	glPushMatrix();
	    glScaled(0.3,0.3,1);
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
		glTranslated(30,15,0);
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

void Display() {
	if(game_start==false){
		glClear(GL_COLOR_BUFFER_BIT );  
		
		glPushMatrix();
		glColor3f(0.10f, 0.20f, 0.10f);
		glBindTexture(GL_TEXTURE_2D, texID);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);glVertex3f(0, 0, 0);
		glTexCoord2f(1, 0.0f); glVertex3f(WidthX+100, 0, 0);
		glTexCoord2f(1, 1);glVertex3f(WidthX+600, HeightY, 0);
		glTexCoord2f(0.0f, 1); glVertex3f(0, HeightY+600, 0);
		glEnd();
		glPopMatrix();
        
        std::string s = " Choose The Maze Mode :";
		printString(WidthX/2 -360 , HeightY/2 +100 ,0, 1, 0, 0, s);
        
		std::string t = " Easy ";
        printString(WidthX/2 -300 , HeightY/2 +50, 0, 1, 0, 0, t);
        std::string o = " Medium  ";
        printString(WidthX/2 -300 , HeightY/2 , 0, 1, 0, 0, o);
        std::string d = " Hard ";
        printString(WidthX/2 -300 , HeightY/2 -50, 0,1, 0, 0, d);
        
		std::string r = "  ESC : Exit  ";
        printString(WidthX/2 -800 , HeightY/2 -500, 0, 1, 0, 0, r);

		std::string e = " press  --> : Enter  ";
        printString(WidthX/2 -600 , HeightY/2 -500, 0, 1, 0, 0, e);
		
		drawArrow();
		
	}else{
	    glClear(GL_COLOR_BUFFER_BIT );  
        std::string s = " Choose The Maze Mode :";
		printString(WidthX/2 -300 , HeightY/2 +100 ,0, 1, 0, 0, s);
		
		  printString(WidthX/2 -300 , HeightY/2 +10 ,0, 1, 0, 0 ,"level :"+ parse(level));
		  //printString(WidthX/2 -200 , HeightY/2 +50 ,0, 1, 0, 0 ,"isEasy :"+ parse(isEasy));
		  //printString(WidthX/2 -100 , HeightY/2  ,0, 1, 0, 0 ,"isM :"+ parse(isMedium));
		  //printString(WidthX/2 -90 , HeightY/2 -10 ,0, 1, 0, 0 ,"isH :"+ parse(isHard));
	
	}
	
       glFlush(); 
	glutSwapBuffers();
}
void keyfunc(unsigned char k, int x,int y) {
    if (k == GLUT_KEY_ESCAPE){
		exit(EXIT_SUCCESS);
    }
    glutPostRedisplay();
}

void keyPressed(int k, int x,int y){
	if(k==GLUT_KEY_DOWN){
	if(arrowX== WidthX/2 -360  && arrowY ==HeightY/2 +100){
		arrowY = HeightY/2 +50;
	}else if(arrowX== WidthX/2 -360  && arrowY ==HeightY/2 +50){
		arrowY=HeightY/2  ;
	}else if(arrowX== WidthX/2 -360  && arrowY ==HeightY/2 ){
		arrowY=HeightY/2 +100 ;
	}
}
	if(k==GLUT_KEY_RIGHT){
	  if(level==0){
		  game_start=true;
		  printf("game start",0);
	 } else if(level==1){
		   game_start=true;
		  printf("game start",1);
	 } else if(level==2){
		   game_start=true;
		  printf("game start",2);
	 }
	
	}
    glutPostRedisplay();
}
void Anim_Move(){
	if(arrowX== WidthX/2 -360  && arrowY ==HeightY/2 +100){
		isEasy=true;
		isHard=false;
		level=0;
		//printf("iseasy flag:",isEasy);
		//printf("ismedium flag:",isMedium);
		//printf("ishard flag:",isHard);
	}else if(arrowX== WidthX/2 -360  && arrowY ==HeightY/2 +50){
		isMedium=true;
		isEasy=false ;
		level=1;
	//	printf("iseasy flag:",isEasy);
	//	printf("ismedium flag:",isMedium);
	//	printf("ishard flag:",isHard);
	}else if(arrowX== WidthX/2 -360  && arrowY ==HeightY/2 ){
		isHard=true;
		isMedium=false;
		level=2;
		/*printf("iseasy flag:",isEasy);
		printf("ismedium flag:",isMedium);
		printf("ishard flag:",isHard);*/
	}//else isHard=false;

	glutPostRedisplay();
}

int main(int argc, char** argr) {
	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("The Maza ");
	glutDisplayFunc(Display);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);
   
	loadBMP(&texID, "textures/dpic.bmp", false);
	
	glutFullScreen();
	glutIdleFunc(Anim_Move);
	glutSpecialFunc(keyPressed);
	glutKeyboardFunc(keyfunc);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	gluOrtho2D(0.0,WidthX,0.0,HeightY);
	glutMainLoop();    
}
