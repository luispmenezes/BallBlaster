
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define GLEW_STATIC 
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "globals.h"
#include "callbacks.h"
#include "init.h"
#include "shaders.h"
#include "gameLogic.h"


int main( int argc, char** argv )
{
   GLenum glewInitResult;

   /* Inicializacao do GLUT */

   glutInit( &argc, argv );

   inicializarJanela();

   /* Inicializacao do GLEW */

   glewInitResult = glewInit();

   if( GLEW_OK != glewInitResult )
   {

        exit( EXIT_FAILURE );
    }

    /* Inicializar o estado da aplicacao */

    inicializarEstado();

    /* Inicializar a cena */

    inicializarModelos();

    inicializarFontesDeLuz();

    /* Inicializar os shaders */

    if ( initResources() == 1 )
    {
        /* Registando callback functions se OK */

        registarCallbackFunctions();

        //infosConsola();

        initGameLogic();

        glutMainLoop();

   }

   /* Housekeeping */

   freeResources();

   libertarModelos();

   return EXIT_SUCCESS;
}
