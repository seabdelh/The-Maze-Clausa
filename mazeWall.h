#ifndef _mazeWall_H// must be unique name in the project
#define _mazeWall_H
#include <math.h>
#include "glut.h"
#include <stdio.h>
#include <vector>


int incUPCounter  ;
int incUP  ;
int wallLength = 10; //cube length
bool isGridCreatedBefore = false ;
bool ** mazeWallMap ; 
GLTexture texx;
GLTexture texx2;
//the new part of collision 
std::vector<float>  mazeCollisionMapZLessThan ;
std::vector<float>  mazeCollisionMapZMoreThan ;
std::vector<float>  mazeCollisionMapXLessThan ;
std::vector<float>  mazeCollisionMapXMoreThan ;
std::vector<int>  mazeCollisionMapcollisionCell;
std::vector<int>  mazeCollisionMapcoliisionCellSide ;
int iteratorCounter =0;
int collisionCell = -1 ;
int coliisionCellSide = -1 ;




//end of the new part
void createNbyNGrid (int n ) {

	if(isGridCreatedBefore == false ) {
		 printf("\n \n da5lt l create %i \n  \n"  ,1 ) ;
		isGridCreatedBefore = true ;
mazeWallMap= new bool*[n *n];
for(int i = 0; i < n*n; ++i)
    mazeWallMap[i] = new bool[4];

// put wall by default 
for(int i = 0; i < n*n; ++i) {
for(int j = 0; j < 4; ++j) {
    mazeWallMap[i][j]=true;

}
}
	}
}


static void
drawBox(GLfloat size, GLenum type)
{
	static GLfloat n[6][3] =
	{
		{ -1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, -1.0 }
	};
	static GLint faces[6][4] =
	{
		{ 0, 1, 2, 3 },
		{ 3, 2, 6, 7 },
		{ 7, 6, 5, 4 },
		{ 4, 5, 1, 0 },
		{ 5, 6, 2, 1 },
		{ 7, 4, 0, 3 }
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

	for (i = 5; i >= 0; i--) {
		glBegin(type);
		glNormal3fv(&n[i][0]);
		glTexCoord3f(0, 0,0);
		glVertex3fv(&v[faces[i][0]][0]);
		glTexCoord3f(1, 0, 0.4);
		glVertex3fv(&v[faces[i][1]][0]);
		glTexCoord3f(1, 1, 0.4);
		glVertex3fv(&v[faces[i][2]][0]);
		glTexCoord3f(0, 1,0.4);
		glVertex3fv(&v[faces[i][3]][0]);
		//glTexCoord3f(0, 0.6, 0.6);
		glEnd();
	}
}

void APIENTRY
glutSolidCube(GLdouble size)
{
	drawBox(size, GL_QUADS);
}

void createMazeSingleWall ( int x , int y , int z ,bool isItRow ) {
	
	//create one side wall
	if (isItRow == false ){
	
	 glPushMatrix();
	 glBindTexture(GL_TEXTURE_2D, texx.texture[0]);
	 glColor3d(0.5f,0.5f,0.5f);
	 glTranslated(x+(wallLength/2), y+(wallLength*0.2/2), z+(wallLength*0.2/2)); 
	 glScaled(1,0.2,0.2);
	 glutSolidCube(wallLength);
	 glBindTexture(GL_TEXTURE_2D, NULL);
	 glPopMatrix(); 
	
	}else {
	
		glPushMatrix(); 
		glBindTexture(GL_TEXTURE_2D, texx.texture[0]);
		glColor3d(0.5f,0.5f,0.5f);
	 glTranslated(x+(wallLength*0.2/2), y+(wallLength*0.2/2), z+(wallLength/2)); 
	 glScaled(0.2,0.2,1);
	 glutSolidCube(wallLength);
	 glBindTexture(GL_TEXTURE_2D, NULL);
	 glPopMatrix(); 
	
	}

}





void destroySomeWalls ( bool** generatedReaMazeWall , int n  ) {
	createNbyNGrid ( n );
for(int i = 0; i < n*n; ++i) {
for(int j = 0; j < 4; ++j) {
    mazeWallMap[i][j]=generatedReaMazeWall[i][j];
// printf("\n deh value l pointer l bool : %p \n",mazeWallMap[i][j] ) ;
}
}

}


void drawMazeGround(int n ){
	glPushMatrix(); 
	glBindTexture(GL_TEXTURE_2D, texx2.texture[0]);
	glColor3d(0.5f,0.5f,0.5f);
	 glTranslated((wallLength*n)/2+wallLength*0.2/2, -((wallLength*0.1)/2), (wallLength*n)/2+wallLength*0.2/2); 
	 glScaled(n+wallLength*0.2/2,0.1,n+wallLength*0.2/2);
	 glutSolidCube(wallLength);
	 glBindTexture(GL_TEXTURE_2D, NULL);
	 glPopMatrix(); 


}




//new Collision methods 

void createCollisionMapRange (bool ** generatedReaMazeWall , int n  ) {
	//re set all vectors to bot have an error 
	 mazeCollisionMapZLessThan.clear();
		 mazeCollisionMapZMoreThan.clear();
		 mazeCollisionMapXLessThan.clear();
		 mazeCollisionMapXMoreThan.clear();
		 mazeCollisionMapcollisionCell.clear();
		 mazeCollisionMapcoliisionCellSide.clear();
	//end of reset

	createNbyNGrid ( n );
	destroySomeWalls (   generatedReaMazeWall  , n) ;

	//draw columns 
	incUP = 0 ;
	incUPCounter = 0 ;
		for(int i = 0; i < n*n; i++) {
			
			if(incUPCounter == n  ){
				//printf("\n da5lt gwa l if  %i  \n",1) ;
				incUP++ ;
				incUPCounter = 0 ;
			}
			
			//printf("\n deh value l pointer l incUP : %i  ,we deh l incUPCounter : %i \n", incUP , incUPCounter ) ;
			//printf("\n deh value l pointer l IIIII : %i  ,we deh l j : %i \n", incUP*wallLength ,  ((i%n)*wallLength) ) ;
	 if( mazeWallMap[i][3]== true){
   // ha3ml add fl avoidness map 
		 mazeCollisionMapZLessThan.push_back(((i%n)*wallLength)+(wallLength*0.2));
		 mazeCollisionMapZMoreThan.push_back(((i%n)*wallLength));
		 mazeCollisionMapXLessThan.push_back(incUP*wallLength+wallLength);
		 mazeCollisionMapXMoreThan.push_back(incUP*wallLength);
		 mazeCollisionMapcollisionCell.push_back(i);
		 mazeCollisionMapcoliisionCellSide.push_back(3);

	}

	  if( mazeWallMap[i][1]== true && i%n == n-1 ){
    // ha3ml add fl avoidness map 
		 mazeCollisionMapZLessThan.push_back(((i%n)*wallLength)+(wallLength*0.2)+wallLength);
		 mazeCollisionMapZMoreThan.push_back(((i%n)*wallLength)+wallLength);
		 mazeCollisionMapXLessThan.push_back(incUP*wallLength+wallLength);
		 mazeCollisionMapXMoreThan.push_back(incUP*wallLength);
		  mazeCollisionMapcollisionCell.push_back(i);
		 mazeCollisionMapcoliisionCellSide.push_back(1);
	}

	

// printf("\n deh value l pointer l bool : %p \n",mazeWallMap[i][j] ) ;
		incUPCounter++;
}
	// end of draw columns 

	// draw row 

		incUP = 0 ;
	incUPCounter = 0 ;
	
		for(int i = 0; i < n*n; i++) {
			
			if(incUPCounter == n  ){
				//printf("\n da5lt gwa l if  %i  \n",1) ;
				incUP++ ;
				incUPCounter = 0 ;
			}
			
			//printf("\n deh value l pointer l incUP : %i  ,we deh l incUPCounter : %i \n", incUP , incUPCounter ) ;
			//printf("\n deh value l pointer l IIIII : %i  ,we deh l j : %i \n", incUP*wallLength ,  ((i%n)*wallLength	) ) ;
	 if( mazeWallMap[i][0]== true){
    // ha3ml add fl avoidness map 
		 mazeCollisionMapZLessThan.push_back(((i%n)*wallLength)+(wallLength));
		 mazeCollisionMapZMoreThan.push_back(((i%n)*wallLength));
		 mazeCollisionMapXLessThan.push_back(incUP*wallLength+(wallLength*0.2));
		 mazeCollisionMapXMoreThan.push_back(incUP*wallLength);
		  mazeCollisionMapcollisionCell.push_back(i);
		 mazeCollisionMapcoliisionCellSide.push_back(0);
	}

	  if( mazeWallMap[i][2]== true && incUP == n-1 ){
   // ha3ml add fl avoidness map 
		 mazeCollisionMapZLessThan.push_back(((i%n)*wallLength)+(wallLength));
		 mazeCollisionMapZMoreThan.push_back(((i%n)*wallLength));
		 mazeCollisionMapXLessThan.push_back(incUP*wallLength+(wallLength*0.2)+wallLength);
		 mazeCollisionMapXMoreThan.push_back(incUP*wallLength+wallLength);
		  mazeCollisionMapcollisionCell.push_back(i);
		 mazeCollisionMapcoliisionCellSide.push_back(2);
	}
	

// printf("\n deh value l pointer l bool : %p \n",mazeWallMap[i][j] ) ;
		incUPCounter++;
}
	// end of draw rows 
	
}

// check if i collide 

bool amICollide  (float z , float x , float radius ){
	iteratorCounter=0;
	for(std::vector<float>::iterator it = mazeCollisionMapXLessThan.begin(); it != mazeCollisionMapXLessThan.end(); ++it) {
    /* std::cout << *it; ... */
	/*printf("\n dah l vector bta3y fl X   %f <X < %f  \n",mazeCollisionMapXMoreThan[iteratorCounter] ,mazeCollisionMapXLessThan[iteratorCounter]) ;
	printf(" dah l vector bta3y fl Z  %f <Z < %f  \n",mazeCollisionMapZMoreThan[iteratorCounter] ,mazeCollisionMapZLessThan[iteratorCounter]) ;
	printf(" dah l vector bta3y am i collide : %i \n",((mazeCollisionMapXMoreThan[iteratorCounter]) <= (x-radius*3/4) && (mazeCollisionMapXLessThan[iteratorCounter]) >= (x+radius*3/4) && mazeCollisionMapZMoreThan[iteratorCounter] <= z-radius*3/4 &&  mazeCollisionMapZLessThan[iteratorCounter] >= z+radius*3/4 )) ;
	printf(" dah l vector bta3y l kora bt3ty  Z`<%f<Z`` && X`<%f<X`` \n",z+radius*(3/4),x+radius*(3/4) ) ;
	*/
	if((mazeCollisionMapXMoreThan[iteratorCounter]) <= (x-radius*(0.1)) && (mazeCollisionMapXLessThan[iteratorCounter]) >= (x+radius*(0.1)) && mazeCollisionMapZMoreThan[iteratorCounter] <= z-radius*(0.1) &&  mazeCollisionMapZLessThan[iteratorCounter] >= z+radius*(0.1) ){
		
		collisionCell =  mazeCollisionMapcollisionCell[iteratorCounter];
		coliisionCellSide =  mazeCollisionMapcoliisionCellSide[iteratorCounter];
		//printf(" it collide at cell : %i &  side :%i  : %i \n \n \n \n" , collisionCell, coliisionCellSide);
		return true ;
	}
	
	iteratorCounter++;
			

}

	collisionCell = -1;
	coliisionCellSide = -1;
	return false ;
}


// end of new methods 



void drawMazeWall (bool ** generatedReaMazeWall , int n  ) {
	createNbyNGrid ( n );
	destroySomeWalls (   generatedReaMazeWall  , n) ;
	 createCollisionMapRange (generatedReaMazeWall ,  n  ) ;

	//draw columns 
	incUP = 0 ;
	incUPCounter = 0 ;
		for(int i = 0; i < n*n; i++) {
			
			if(incUPCounter == n  ){
				//printf("\n da5lt gwa l if  %i  \n",1) ;
				incUP++ ;
				incUPCounter = 0 ;
			}
			
			//printf("\n deh value l pointer l incUP : %i  ,we deh l incUPCounter : %i \n", incUP , incUPCounter ) ;
			//printf("\n deh value l pointer l IIIII : %i  ,we deh l j : %i \n", incUP*wallLength ,  ((i%n)*wallLength) ) ;
	 if( mazeWallMap[i][3]== true){
   createMazeSingleWall (  incUP*wallLength ,  0 ,  ((i%n)*wallLength)  , false );
	}

	  if( mazeWallMap[i][1]== true && i%n == n-1 ){
   createMazeSingleWall (  incUP*wallLength ,  0 ,  ((i%n)*wallLength) +wallLength , false );
	}

	

// printf("\n deh value l pointer l bool : %p \n",mazeWallMap[i][j] ) ;
		incUPCounter++;
}
	// end of draw columns 

	// draw row 

		incUP = 0 ;
	incUPCounter = 0 ;
	
		for(int i = 0; i < n*n; i++) {
			
			if(incUPCounter == n  ){
				//printf("\n da5lt gwa l if  %i  \n",1) ;
				incUP++ ;
				incUPCounter = 0 ;
			}
			
			//printf("\n deh value l pointer l incUP : %i  ,we deh l incUPCounter : %i \n", incUP , incUPCounter ) ;
			//printf("\n deh value l pointer l IIIII : %i  ,we deh l j : %i \n", incUP*wallLength ,  ((i%n)*wallLength	) ) ;
	 if( mazeWallMap[i][0]== true){
   createMazeSingleWall (  incUP*wallLength ,  0 ,  ((i%n)*wallLength)  , true );
	}

	  if( mazeWallMap[i][2]== true && incUP == n-1 ){
   createMazeSingleWall (  incUP*wallLength+wallLength ,  0 ,  ((i%n)*wallLength)  , true );
	}
	

// printf("\n deh value l pointer l bool : %p \n",mazeWallMap[i][j] ) ;
		incUPCounter++;
}
	// end of draw rows 
	
}



void drawMaze ( bool ** generatedReaMazeWall ,int n  ){
	drawMazeWall (  generatedReaMazeWall , n );
	 drawMazeGround( n );


}

void destroySingleWell (int i , int j , int n  ) {
	createNbyNGrid ( n );
	if(j == 0 || j==2){
	mazeWallMap[i][0]=false ;
	mazeWallMap[i][2]=false ;

	}

	if(j == 1 || j==3){
	mazeWallMap[i][1]=false ;
	mazeWallMap[i][3]=false ;

	}
	drawMaze (  mazeWallMap , n  );

}





#endif 