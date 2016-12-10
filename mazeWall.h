#ifndef _mazeWall_H// must be unique name in the project
#define _mazeWall_H
#include <math.h>
#include "glut.h"
#include <stdio.h>

int incUPCounter  ;
int incUP  ;
int wallLength = 10; //cube length
bool isGridCreatedBefore = false ;
bool ** mazeWallMap ; 



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




void createMazeSingleWall ( int x , int y , int z ,bool isItRow ) {
	
	//create one side wall
	if (isItRow == false ){
		
	glPushMatrix(); 
	glColor3d(0.5f,0.5f,0.5f);
	 glTranslated(x+(wallLength/2), y+(wallLength*0.2/2), z); 
	 glScaled(1,0.2,0.2);
	 glutSolidCube(wallLength);
	 glPopMatrix(); 
	
	}else {
		glPushMatrix(); 
		glColor3d(0.5f,0.5f,0.5f);
	 glTranslated(x, y+(wallLength*0.2/2), z+(wallLength/2)); 
	 glScaled(0.2,0.2,1);
	 glutSolidCube(wallLength);
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
		glColor3d(0.3f,0.3f,0.3f);
	 glTranslated((wallLength*n)/2, -((wallLength*0.1)/2), (wallLength*n)/2); 
	 glScaled(n,0.1,n);
	 glutSolidCube(wallLength);
	 glPopMatrix(); 


}


void drawMazeWall (bool ** generatedReaMazeWall , int n  ) {
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