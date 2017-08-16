#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define GLEW_STATIC 

#include <GL/glew.h>
#include <GL/freeglut.h>


#include "globals.h"
#include "callbacks.h"
#include "shading.h"
#include "gameLogic.h"

/*Camera Position*/
float cam[3] = {0,0,2.0};
/*Postion of the spheres in the world except nextColor*/	
float coord [125][3];
/*Time left to add a new sphere*/
float moveTimer;
/*Color of all the spheres excluding nextColor sphere*/
Color colors [125];
/*Color of the next sphere to be added*/
Color nextColor;
/*Score and number of visible spheres*/
unsigned int score, nSpheres;
/*Animation arrays*/
int animIN[10],animOUT[10];
/*Inverse of the projection matrix*/
mat4x4* invProj;

void initGameLogic( void ){
	/*Initialize the inverse of projection matrix (hard coded for efficency)*/
	invProj = (mat4x4*) malloc( sizeof( struct Matrix ));
	
	resetGameLogic();
	invProj->m[0]=0.577350; invProj->m[4]=0;         invProj->m[8]=0;           invProj->m[12]=0;
	invProj->m[1]=0;        invProj->m[5]=0.577350;  invProj->m[9]=0;           invProj->m[13]=0;
	invProj->m[2]=0;        invProj->m[6]=0;         invProj->m[10]=0;          invProj->m[14]=-1;
	invProj->m[3]=0;        invProj->m[7]=0;         invProj->m[11]=-0.487500;  invProj->m[15]=0.512499;

	/*Set the game state to INIT*/
	gameState = INIT;
}

void resetGameLogic(void){
	int i;
	/*Hide all the models setting their scale to 0*/
	for(i=0; i < 125; i++){
		arrayModelos[i]->factorEscX=0;
		arrayModelos[i]->factorEscY=0;
		arrayModelos[i]->factorEscZ=0;
	}

	/*Assign a random color to the sphere on the center of the screen and make it visible
		62 base5 = 000 base10*/
	setModelColor(62,getRandomColor());
	arrayModelos[62]->factorEscX=0.1;
	arrayModelos[62]->factorEscY=0.1;
	arrayModelos[62]->factorEscZ=0.1;

	/*Get new random color and assing it to the nextColor Sphere*/
	nextColor = getRandomColor();
	setModelColor(125,nextColor);

	for(i=0; i < 10; i++){
		animIN[i]=-1;
		animOUT[i]=-1;
	}

	/*Reset moveTimer, number of spheres and score*/
	moveTimer = 7.0;
	nSpheres=1;

	score = 0;	
}

void gameLoop( void ){
	
	if(gameState==RUNNING){
		/*If the number of spheres o  screen exceeds the maximum allowed set the state to gameover*/
		//fprintf(stderr, "\n %d",nSpheres);
		animate();

		if(nSpheres >125){
			moveTimer = 0;
			gameState = GAMEOVER;
		}
		/*Decrease the timer if possible else set state to gameover*/
		if(moveTimer > 0){

			moveTimer -= 0.033;

		}
		else{
			moveTimer = 0; 
			gameState = GAMEOVER;
		}
	}	 
}

/*Check groups of 4 or more spheres, starting on index i, of the same color and acts accordingly */
void checkMatches(int i){
	/*Next check -> next position on matches[] to evaluate
	   numChecks -> number of spheres left to evaluate
	   count     -> number of spheres in macthes[] 
	   exist,c,j -> auxiliar variables */
	int nextCheck=0, numChecks=1, count=1, exists=0, c, j;
	/*Array that store the index of the adjacent spheres of the same color*/
	int matches[25];
	/*Reciver for the indices sent by getContig*/
	int* cntg;
	/*Add the index of the sphere being evaluated*/
	matches[0]=i;

	/*While there are indicies left to evaluate*/
	do{
		/*Obtain the the indices of the adjacent spheres of nextCheck of the same color*/
		cntg = getContig(matches[nextCheck]);
		numChecks--;

		/*Add them to matches avoiding duplicates*/
		for(c=0; c < 6; c++){
			if(cntg[c]!=-1){
				exists=0;
				for(j=0; j < count; j++){
					if(matches[j]==cntg[c]){exists=1;j=count+1;}
				}
				if(!exists){matches[count]=cntg[c]; count++; numChecks++;}
			}
		}

		nextCheck++;
	}while(numChecks > 0);

	/*If the number of matches is greater than the required minimum*/
	if(count>=4){

		/*Increment the score*/
		score += 400 + (((count - 4)*(count - 4)) * 200);

		/*Hide the respective models setting their scale to 0*/
		for(j=0; j < count; j++){
			
			addAnimOUT(matches[j]);

			colors[matches[j]] = EMPTY;
		}

		nSpheres -= count;

	}
}

/*Returns the adjacent spheres of i in the x,y and z directions that are of the same color*/
int* getContig(int i){

	int* array = malloc(6 * sizeof(int));
	int cnt = 0;  

	/*Get the x,y,z of the sphere bases on the linear index*/
	int x = i%5;
	int y = (i/5)%5;
	int z = (i/25)%5;

	/*Check the X direction*/
	if(x < 4 && arrayModelos[i+1]->factorEscX > 0 && colors[i] == colors[i+1]){
		array[cnt] = i + 1;
		cnt++;
	}
	if(x > 0 && arrayModelos[i-1]->factorEscX > 0 && colors[i] == colors[i-1]){
		array[cnt] = i - 1;
		cnt++;
	}

	/*Check the Y direction*/
	if(y < 4 && arrayModelos[i+5]->factorEscX > 0 && colors[i] == colors[i+5]){
		array[cnt] = i + 5;
		cnt++;
	}
	if(y > 0 && arrayModelos[i-5]->factorEscX > 0 && colors[i] == colors[i-5]){
		array[cnt] = i - 5;
		cnt++;
	}

	/*Check the Z direction*/
	if(z < 4 && arrayModelos[i+25]->factorEscX > 0 && colors[i] == colors[i+25]){
		array[cnt] = i + 25;
		cnt++;
	}
	if(z > 0 && arrayModelos[i-25]->factorEscX > 0 && colors[i] == colors[i-25]){
		array[cnt] = i - 25;
		cnt++;
	}

	/*Fill the remaining positions with -1 to obtain an array length 6*/
	int c;
	for(c=cnt; c < 6; c++){
		array[c]=-1;
	}

	return array;
}

void onClick(int x, int y){

	/*If there are no spheres on screen place it on the center of the screen*/
	if(nSpheres == 0){
		setModelColor(62,nextColor);
    	nextColor = getRandomColor();
    	setModelColor(125,nextColor);
    	moveTimer = 7.0;
    	nSpheres ++;

    	addAnimIN(62);

    	glutPostRedisplay();

    	return;		
	}

	/*Obtain the ray casted by the mouse click*/
	float ray_clip[4] = {(2.0f * x) / 600 - 1.0f,1.0f - (2.0f * y) / 600, -1.0, 1.0};
    float* ray_eye = multiplyVectorByMatrix(invProj, ray_clip);
    ray_eye[2]=-1.0;
    ray_eye[3]=0;
                                         
    float ray_wor[3] = {ray_eye[0],ray_eye[1],ray_eye[2]};
    convertToUnitVector(ray_wor);

    /*hit->index of the visible intersected sphere closest to the camera
      hitCount -> number of invisible intesected spheres 	
      hitList  -> indices of invisible intesected spheres 
      i,dest2Cam -> auxiliar variables*/
    int i,hit=-1,hitCnt=0;
    float dest2Cam = 999.0;
    int hitList[10];

    /*Check the intersection with all the spheres saving the index visible intersected sphere closest to the camera and all the hidden ones*/
    for(i=0; i <125; i++){                     
        float oc[3] = {0-coord[i][0],0-coord[i][1],2-coord[i][2]};

        float b = computeDotProduct(ray_wor,oc);
        float c = computeDotProduct(oc,oc) - 0.0144;

        if((b*b) - c >= 0){                                                   
                                                    
            hitList[hitCnt]=i;
            hitCnt++;

            if(arrayModelos[i]->factorEscX>0){

                if( hit==-1 || distanceN(cam, coord[i]) < dest2Cam){
                    hit=i;
                    dest2Cam = distanceN(cam, coord[i]);
                }
            }
        }                    
    }

    /*if there were intersections with any visible spheres*/
    if(hit != -1){

    	int lhit = -1;			//index intersected invisible sphere closest to hit sphere
    	float lHitDist = 999;	//distance between hit and hlit
    	float dC,dH;			//auxiliar variable distance to camera and distance to hit


    	/*Obtain the sphere between hit and the camera thats closest to the first*/
    	for(i=0;i < hitCnt; i++){

    		dC = distanceN(cam, coord[hitList[i]]);
    		dH = distanceN(coord[hit], coord[hitList[i]]);

    		if(dC < dest2Cam && dH < lHitDist){
    			lhit = hitList[i];
    			lHitDist = dH; 
    		}
    	}

    	/*Check if there is a valid sphere intersected*/
    	if( lhit != -1 && isValid(hit,lhit)){
    		/*Set lhit sphere with nextColor*/
    		setModelColor(lhit,nextColor);
    		
    		/*Get new nextColor*/
    		nextColor = getRandomColor();
    		setModelColor(125,nextColor);
    		
    		/*Reset timer*/
    		moveTimer = 7.0;

    		nSpheres ++;
			
			addAnimIN(lhit);

			/*Check if this new sphere created a match*/	
    		checkMatches(lhit);

    		glutPostRedisplay();
    	}
	}
}	

void updateCoord(int idx, float x,float y, float z){
	coord[idx][0] = x;
	coord[idx][1] = y;
	coord[idx][2] = z;
}

/*Check if 2 indices are non-diagonaly adjacent */
int isValid(int a, int b){
    int dX = abs((b%5)-(a%5));
    int dY = abs(((b/5)%5)-((a/5)%5));
    int dZ = abs(((b/25)%5)-((a/25)%5));

    return (dX+dY+dZ) < 2 ? 1 : 0;
}	

/*Change the model with index idx to the color corresponding to c and store that information in 
colors[]*/
void setModelColor(int idx, Color c){
	switch(c){

		case RED:
				arrayModelos[idx]->kAmb[0] = 1.0;
				arrayModelos[idx]->kAmb[1] = 0.0;
				arrayModelos[idx]->kAmb[2] = 0.0;
				arrayModelos[idx]->kAmb[3] = 1.0;

				arrayModelos[idx]->kDif[0] = 1.0;
				arrayModelos[idx]->kDif[1] = 0.0;
				arrayModelos[idx]->kDif[2] = 0.0;
				arrayModelos[idx]->kDif[3] = 1.0;

				arrayModelos[idx]->kEsp[0] = 0.7;
				arrayModelos[idx]->kEsp[1] = 0.7;
				arrayModelos[idx]->kEsp[2] = 0.7;
				arrayModelos[idx]->kEsp[3] = 1.0;

				if(idx!=125){colors[idx]=c;}

				break;
		case GREEN:
				arrayModelos[idx]->kAmb[0] = 0.0;
				arrayModelos[idx]->kAmb[1] = 1.0;
				arrayModelos[idx]->kAmb[2] = 0.0;
				arrayModelos[idx]->kAmb[3] = 1.0;

				arrayModelos[idx]->kDif[0] = 0.0;
				arrayModelos[idx]->kDif[1] = 1.0;
				arrayModelos[idx]->kDif[2] = 0.0;
				arrayModelos[idx]->kDif[3] = 1.0;

				arrayModelos[idx]->kEsp[0] = 0.7;
				arrayModelos[idx]->kEsp[1] = 0.7;
				arrayModelos[idx]->kEsp[2] = 0.7;
				arrayModelos[idx]->kEsp[3] = 1.0;

				if(idx!=125){colors[idx]=c;}

				break;
		case BLUE:
				arrayModelos[idx]->kAmb[0] = 0.0;
				arrayModelos[idx]->kAmb[1] = 0.0;
				arrayModelos[idx]->kAmb[2] = 1.0;
				arrayModelos[idx]->kAmb[3] = 1.0;

				arrayModelos[idx]->kDif[0] = 0.0;
				arrayModelos[idx]->kDif[1] = 0.0;
				arrayModelos[idx]->kDif[2] = 1.0;
				arrayModelos[idx]->kDif[3] = 1.0;

				arrayModelos[idx]->kEsp[0] = 0.7;
				arrayModelos[idx]->kEsp[1] = 0.7;
				arrayModelos[idx]->kEsp[2] = 0.7;
				arrayModelos[idx]->kEsp[3] = 1.0;

				if(idx!=125){colors[idx]=c;}
				
				break;
		case YELLOW:
				arrayModelos[idx]->kAmb[0] = 1.0;
				arrayModelos[idx]->kAmb[1] = 1.0;
				arrayModelos[idx]->kAmb[2] = 0.0;
				arrayModelos[idx]->kAmb[3] = 1.0;

				arrayModelos[idx]->kDif[0] = 1.0;
				arrayModelos[idx]->kDif[1] = 1.0;
				arrayModelos[idx]->kDif[2] = 0.0;
				arrayModelos[idx]->kDif[3] = 1.0;

				arrayModelos[idx]->kEsp[0] = 0.7;
				arrayModelos[idx]->kEsp[1] = 0.7;
				arrayModelos[idx]->kEsp[2] = 0.7;
				arrayModelos[idx]->kEsp[3] = 1.0;

				if(idx!=125){colors[idx]=c;}
				
				break;

		case PINK:
				arrayModelos[idx]->kAmb[0] = 1.0;
				arrayModelos[idx]->kAmb[1] = 0.0;
				arrayModelos[idx]->kAmb[2] = 1.0;
				arrayModelos[idx]->kAmb[3] = 1.0;

				arrayModelos[idx]->kDif[0] = 1.0;
				arrayModelos[idx]->kDif[1] = 0.0;
				arrayModelos[idx]->kDif[2] = 1.0;
				arrayModelos[idx]->kDif[3] = 1.0;

				arrayModelos[idx]->kEsp[0] = 0.7;
				arrayModelos[idx]->kEsp[1] = 0.7;
				arrayModelos[idx]->kEsp[2] = 0.7;
				arrayModelos[idx]->kEsp[3] = 1.0;

				if(idx!=125){colors[idx]=c;}

				break;

		case EMPTY : 
				break;		
	}
}

/*Generate color randomly*/
Color getRandomColor( void ){

	int r = rand() % 5;

	switch(r){

		case 0: return RED;    
		case 1: return GREEN;  
		case 2: return BLUE;   
		case 3: return YELLOW;
		case 4: return PINK; 

	}

	return -1;
}

/*Add an index to the animateIn list and remove from animateOUT if it exists*/
void addAnimIN(int idx){
	int i;
	
	for(i=0;i < 10;i++){
		if(animOUT[i]==idx){
			animOUT[i]=-1;
			break;	
		}	
	}

	for(i=0;i < 10;i++){
		if(animIN[i]==-1){
			animIN[i]=idx;
			break;	
		}	
	}	
}

/*Add an index to the animateOUT list and remove from animateIN if it exists*/
void addAnimOUT(int idx){
	int i;

	for(i=0;i < 10;i++){
		if(animIN[i]==idx){
			animIN[i]=-1;
			break;	
		}	
	}


	for(i=0;i < 10;i++){
		if(animOUT[i]==-1){
			animOUT[i]=idx;
			break;	
		}	
	}	
}

/*Animate corresponding spheres*/
void animate( void ){

	int i;

	for(i=0;i < 10;i++){
		if(animIN[i]>=0){
			arrayModelos[animIN[i]]->factorEscX+=0.025;
			arrayModelos[animIN[i]]->factorEscY+=0.025;
			arrayModelos[animIN[i]]->factorEscZ+=0.025;
			if(arrayModelos[animIN[i]]->factorEscX>=0.1){
				arrayModelos[animIN[i]]->factorEscX=0.1;
				arrayModelos[animIN[i]]->factorEscY=0.1;
				arrayModelos[animIN[i]]->factorEscZ=0.1;
				animIN[i]=-1;
			}
		}
		if(animOUT[i]>=0){
			arrayModelos[animOUT[i]]->factorEscX-=0.025;
			arrayModelos[animOUT[i]]->factorEscY-=0.025;
			arrayModelos[animOUT[i]]->factorEscZ-=0.025;
			if(arrayModelos[animOUT[i]]->factorEscX<=0){
				arrayModelos[animOUT[i]]->factorEscX=0;
				arrayModelos[animOUT[i]]->factorEscY=0;
				arrayModelos[animOUT[i]]->factorEscZ=0;
				animOUT[i]=-1;
			}
		}

	}

}


int getScore( void ){return score;}

float getTime2Move( void ){return moveTimer;}