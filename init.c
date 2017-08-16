
#include <stdlib.h>
#include <time.h>

#include <stdio.h>
#include <string.h>


#define GLEW_STATIC

#include <GL/glew.h>

#include <GL/freeglut.h>


#include "globals.h"

#include "mathUtils.h"

#include "models.h"


void inicializarEstado( void )
{
	 
   glutIgnoreKeyRepeat(1);	
	
   /* DOUBLE-BUFFERING + DEPTH-TESTING */

   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );

   /* Definir a cor do fundo */

   glClearColor( 1.0, 1.0, 1.0, 1.0 );

   /* Atributos das primitivas */

   glPointSize( 4.0 );

   glLineWidth( 3.0 );

   /* Modo de desenho dos poligonos */

   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

   /* Back-Face Culling */

   glCullFace( GL_BACK );

   glFrontFace( GL_CCW );

   glEnable( GL_CULL_FACE );

   /* Transparency */

   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   /* Depth-Buffer */

   glEnable( GL_DEPTH_TEST );


   matrizProj = CreateProjectionMatrix( 60, 1, 1, 40 );
   
   Translate( &matrizProj, 0, 0, -2 );

   matrizModelView = IDENTITY_MATRIX;

   /* Para rodar globalmente a cena */

   animacaoCenaON = 0;
   animacaoModelosON = 0;
   animacaoFocosON = 0;
}

/*Window resize disable work-around*/
void resize(int width, int height){
  glutReshapeWindow( 600,600);
}


void inicializarJanela( void )
{
   /* Caracteristicas da janela de saida */

   glutInitWindowSize( 600, 600 ); 

   glutInitWindowPosition( 300, 300 );

   /* Para terminar de modo apropriado */

   glutSetOption(

        GLUT_ACTION_ON_WINDOW_CLOSE,

        GLUT_ACTION_GLUTMAINLOOP_RETURNS

    );

   /* Criar a janela de saida */

   windowHandle = glutCreateWindow( "Ball Blaster" );

   if( windowHandle < 1 )
   {
      exit( EXIT_FAILURE );
   }

   glutReshapeFunc(resize);
}

void inicializarFontesDeLuz( void )
{
    numFocos = 1;

    /* Criar o array */

    arrayFocos = (pontFoco*) malloc( numFocos * sizeof(pontFoco) );

    /* Foco 0 */

    arrayFocos[0] = (pontFoco) malloc( sizeof(Registo_Foco) );

    arrayFocos[0]->focoIsOn = 1;

    arrayFocos[0]->posicao[0] = 0.0;
    arrayFocos[0]->posicao[1] = 4.0;
    arrayFocos[0]->posicao[2] = 5.0;
    arrayFocos[0]->posicao[3] = 1.0; /* Foco PONTUAL */

    arrayFocos[0]->intensidade[0] = 1.0;
    arrayFocos[0]->intensidade[1] = 1.0;
    arrayFocos[0]->intensidade[2] = 1.0;
    arrayFocos[0]->intensidade[3] = 1.0;

    arrayFocos[0]->luzAmbiente[0] = 0.2;
    arrayFocos[0]->luzAmbiente[1] = 0.2;
    arrayFocos[0]->luzAmbiente[2] = 0.2;
    arrayFocos[0]->luzAmbiente[3] = 1.0;

    arrayFocos[0]->translX = 0.0;
    arrayFocos[0]->translY = 0.0;
    arrayFocos[0]->translZ = 0.0;

    arrayFocos[0]->angRotXX = 0.0;
    arrayFocos[0]->angRotYY = 0.0;
    arrayFocos[0]->angRotZZ = 0.0;

    arrayFocos[0]->rotacaoOnXX = 1;
    arrayFocos[0]->rotacaoOnYY = 0;
    arrayFocos[0]->rotacaoOnZZ = 0;
}

void inicializarModelos( void )
{
    numModelos = 126;

    /*inicializar Modelos*/

    arrayModelos = (pontModelo*) malloc( numModelos * sizeof(pontModelo) );

	  int i=0;
	
   for(i=0; i < 126; i++){
    arrayModelos[i] = (pontModelo) malloc( sizeof(Registo_Modelo) );

    lerVerticesDeFicheiro( "modeloEsferaV1.txt", &(arrayModelos[i]->numVertices),
      &(arrayModelos[i]->arrayVertices) );
		/* Determinar as normais unitarias a cada triangulo */

    arrayModelos[i]->arrayNormais = calcularNormaisTriangulos( arrayModelos[i]->numVertices,
      arrayModelos[i]->arrayVertices );
		/* Array vazio para guardar a cor calculada para cada vertice */

    arrayModelos[i]->arrayCores = (GLfloat*) calloc( 3 * arrayModelos[i]->numVertices, sizeof( GLfloat) );

		/* Propriedades do material */

    arrayModelos[i]->kAmb[0] = 1.0;
    arrayModelos[i]->kAmb[1] = 1.0;
    arrayModelos[i]->kAmb[2] = 1.0;
    arrayModelos[i]->kAmb[3] = 1.0;

    arrayModelos[i]->kDif[0] = 1.0;
    arrayModelos[i]->kDif[1] = 1.0;
    arrayModelos[i]->kDif[2] = 1.0;
    arrayModelos[i]->kDif[3] = 1.0;

    arrayModelos[i]->kEsp[0] = 1.0;
    arrayModelos[i]->kEsp[1] = 1.0;
    arrayModelos[i]->kEsp[2] = 1.0;
    arrayModelos[i]->kEsp[3] = 1.0;

    arrayModelos[i]->coefDePhong = 100;

		/* Parametros das transformacoes */

    arrayModelos[i]->deslX = ((i%5)-2)*0.2;
    arrayModelos[i]->deslY = (((i/5)%5)-2)*0.2;
    arrayModelos[i]->deslZ = (((i/25)%5)-2)*0.2;

    arrayModelos[i]->angRotXX = 0.0;
    arrayModelos[i]->angRotYY = 0.0;
    arrayModelos[i]->angRotZZ = 0.0;

    arrayModelos[i]->factorEscX = 0;
    arrayModelos[i]->factorEscY = 0;
    arrayModelos[i]->factorEscZ = 0;

    arrayModelos[i]->rotacaoOnXX = 0;
    arrayModelos[i]->rotacaoOnYY = 0;
    arrayModelos[i]->rotacaoOnZZ = 0;
  }

  /*Inicialize nextColor Sphere*/

  arrayModelos[125]->deslX = 0;
  arrayModelos[125]->deslY = 0.9;
  arrayModelos[125]->deslZ = 0;

  arrayModelos[125]->factorEscX = 0.07;
  arrayModelos[125]->factorEscY = 0.07;
  arrayModelos[125]->factorEscZ = 0.07;

}

void libertarArraysModelo( int i )
{
     free( arrayModelos[i]->arrayVertices );

     free( arrayModelos[i]->arrayNormais );

     free( arrayModelos[i]->arrayCores );
}


void libertarModelos( void )
{
    int i;

    for( i = 0; i < numModelos; i++ )
    {
        free( arrayModelos[i]->arrayVertices );

        free( arrayModelos[i]->arrayNormais );

        free( arrayModelos[i]->arrayCores );

        free( arrayModelos[i] );
    }

    free( arrayModelos );

    numModelos = 0;
}
