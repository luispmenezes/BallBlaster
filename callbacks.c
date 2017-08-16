
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

/*Rotation value for X and Y in each program loop */
float rotationDeltaX=0.0, rotationDeltaY=0.0;

void myDisplay( void){

  /* Alter whats rendered on screen based on the state of the game  */
  switch(gameState){

    case RUNNING: renderRunning();
                  break;
    case GAMEOVER:renderGameover();
                  break;
    case INIT: renderInit();
                  break;                               
  }

}

void renderInit( void ){
  
  /*Clear the screen*/
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glUseProgram( programaGLSL );

  /*Set the position where the text will be rendered*/
  matrizModelView = IDENTITY_MATRIX;

  glUniformMatrix4fv( uniform_matriz_proj, 1, GL_FALSE, matrizModelView.m);

  Translate( &matrizModelView, -0.4, 0.4, 0 );
  glUniformMatrix4fv( uniform_matriz_model_view, 1, GL_FALSE, matrizModelView.m);

  glRasterPos2i(0,0);
  glDisable(GL_LIGHTING);
  glColor3f(1.0f,1.0f,1.0f);
  /*Render the string*/
  unsigned char* buf = "            BALL BLASTER \n\n\n\n\n\n\n\n\n\n         Press Enter to Start\n\n\n\n\n\n\n\nBy Valter Correia & Luis Menezes ";
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, buf); 

  glutSwapBuffers();
}

void renderGameover( void ){
  
  /*Clear the screen*/
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


  /*Set the position where the text will be rendered*/
  matrizModelView = IDENTITY_MATRIX;

  glUniformMatrix4fv( uniform_matriz_proj, 1, GL_FALSE, matrizModelView.m);

  Translate( &matrizModelView, -0.4, 0.4, 0 );
  glUniformMatrix4fv( uniform_matriz_model_view, 1, GL_FALSE, matrizModelView.m);

  glRasterPos2i(0,0);

  /*Render the string*/
  unsigned char buf[200];
  sprintf(buf,"            GAME OVER\n\n      Your Score: %5d  \n\n\n\n     Press Enter to Restart ",getScore());
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, buf); 

  glutSwapBuffers();

}

void renderRunning( void )
{ 
    int m;

    /* Limpar a janela */

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* SHADERS */

    glUseProgram( programaGLSL );

    /* Para cada um dos modelos da cena */

    for( m = 0; m < numModelos; m++ )
    {

        /* Input para o Vertex-Shader */

        glEnableVertexAttribArray( attribute_coord3d );
        glEnableVertexAttribArray( attribute_corRGB );

        /* Caracteristicas do array de coordenadas */

        glVertexAttribPointer( attribute_coord3d, // attribute

                          3,                 // number of elements per vertex, here (x,y,z)

                          GL_FLOAT,          // the type of each element

                          GL_FALSE,          // take our values as-is

                          0,                 // no extra data between each position

                          arrayModelos[m]->arrayVertices );   // pointer to the C array

        /* Caracteristicas do array de cores */

        glVertexAttribPointer( attribute_corRGB, // attribute

                          3,                 // number of elements per vertex, here (R,G,B)

                          GL_FLOAT,          // the type of each element

                          GL_FALSE,          // take our values as-is

                          0,                 // no extra data between each position

                          arrayModelos[m]->arrayCores );    // pointer to the C array

        matrizModelView = IDENTITY_MATRIX;

        /* Applies rotation to all the spheres except the nextColor sphere */
        if(m!=125){
          RotateAboutX( &matrizModelView, DegreesToRadians( angRotXX ) );
          RotateAboutY( &matrizModelView, DegreesToRadians( angRotYY ) );
        }    
        /* Transformacoes proprias do m-esimo modelo */

        Translate( &matrizModelView, arrayModelos[m]->deslX, arrayModelos[m]->deslY, arrayModelos[m]->deslZ );

        Scale( &matrizModelView, arrayModelos[m]->factorEscX, arrayModelos[m]->factorEscY, arrayModelos[m]->factorEscZ );
        
        /*Update coordinates of all the spheres except nextColor*/
        if(m!=125){
          updateCoord(m, matrizModelView.m[12], matrizModelView.m[13], matrizModelView.m[14]); 
        }  
       
        /* Matriz de projeccao */

        glUniformMatrix4fv( uniform_matriz_proj, 1, GL_FALSE, matrizProj.m);

        /* Matriz de transformacao */

        glUniformMatrix4fv( uniform_matriz_model_view, 1, GL_FALSE, matrizModelView.m);

        /* Aplicar o Modelo de Iluminação ao m-esimo modelo */

        smoothShading( m );

        glDrawArrays( GL_TRIANGLES, 0, arrayModelos[m]->numVertices );

        glDisableVertexAttribArray( attribute_coord3d );
        glDisableVertexAttribArray( attribute_corRGB );

    }

  /*Render the HUD text */  
  glDisable( GL_DEPTH_TEST );  

  /*Set the position where the text will be rendered*/
  matrizModelView = IDENTITY_MATRIX;

  glUniformMatrix4fv( uniform_matriz_proj, 1, GL_FALSE, matrizModelView.m);

  Translate( &matrizModelView, -0.95, 0.9, 0 );
  glUniformMatrix4fv( uniform_matriz_model_view, 1, GL_FALSE, matrizModelView.m);

  glRasterPos2i(0,0);

  /*Format and render the string*/
  unsigned char buf[200];
  sprintf(buf, "Score: %5d                                 Next:                            Time Left %3.2f", getScore(),getTime2Move());
  glutBitmapString(GLUT_BITMAP_HELVETICA_18, buf); 

  glEnable( GL_DEPTH_TEST );

  glutSwapBuffers();
}

void mySpecialKeys( int key, int x, int y )
{
    /*Alter the rotation of the model based on the arrow keys while the game is running*/

    if(gameState == RUNNING){

      switch( key )
      {
          case GLUT_KEY_LEFT :
      			/*Ignores the key press if the opposing arrow is down*/
            if(rotationDeltaY == 0){
      			    rotationDeltaY= 7.0;
                  }
                  break;

          case GLUT_KEY_RIGHT :
      			if(rotationDeltaY == 0){
      			    rotationDeltaY= -7.0;
                  }    
                  break;

          case GLUT_KEY_UP :
      			if(rotationDeltaX == 0){
      			    rotationDeltaX= -7.0;
                  }    
                  break;

          case GLUT_KEY_DOWN :
      			if(rotationDeltaX == 0){
      			    rotationDeltaX= 7.0;
                  }
                  break;
      }
    }  
}

void mySpecialKeysUp( int key, int x, int y )
{
  /*Stops the rotation if the respective key is released*/  

  if(gameState == RUNNING){	
    switch( key )
      {
          case GLUT_KEY_LEFT :
  			rotationDeltaY = 0;
              break;

          case GLUT_KEY_RIGHT :
  			rotationDeltaY = 0;
              break;

          case GLUT_KEY_UP :
  			rotationDeltaX = 0;
              break;

          case GLUT_KEY_DOWN :
  			rotationDeltaX = 0;
              break;
      }
  }    
}

void myKeyboard( unsigned char key, int x, int y )
{
    switch( key )
    {
        /*If the escape key is pressed  end the game if it is running else close the program*/
        case 27  :  if(gameState==RUNNING){
                        gameState=INIT;
                    }
                    else{exit( EXIT_SUCCESS );}
                    break;

        /*If the game is stopped and the enter key is pressed  a new game is started*/              
        case 13  :  if( gameState == INIT || gameState == GAMEOVER ){
                        rotationDeltaX = 0;
                        rotationDeltaY = 0;  
                        resetGameLogic();
                        gameState = RUNNING;
                    }
                    break;

    }     

}

void myMouseFunc(int button, int state, int x, int y){
  
  /*When the game is running and the left key pressed event occurs the game logic
   click action is triggered */

  if(gameState==RUNNING){
    
    switch(button){
      case GLUT_LEFT_BUTTON: 
        if(state==GLUT_DOWN){
          onClick(x, y);
        }
        break;										
    }
  }

}

void myTimer( int value )
{
  /*Game Logic is updated every loop to update the timers and gameover conditions*/

  gameLoop();
	
  /*Update the spheres Y rotation angle*/
	angRotYY += rotationDeltaY;

	if( angRotYY > 360.0 ){
		angRotYY = 0;
	}	

  /*Update the spheres X rotation angle*/	
	angRotXX += rotationDeltaX;

	if( angRotXX > 360.0 ){
		angRotXX = 0;
	}
	        
  
  /*The display function is called every loop in order to draw at least the new 
    timer value*/
  glutPostRedisplay();
   
  glutTimerFunc( 33, myTimer, 0 );
}

void registarCallbackFunctions( void )
{
   glutDisplayFunc( myDisplay );

   glutTimerFunc( 33, myTimer, 0 );

   glutSpecialFunc( mySpecialKeys );

   glutKeyboardFunc( myKeyboard );
   
   glutSpecialUpFunc( mySpecialKeysUp );
   
   glutMouseFunc( myMouseFunc );
}

