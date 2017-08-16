
#ifndef _globals_h
#define _globals_h


#define GLEW_STATIC 
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "mathUtils.h"


/* Variaveis GLOBAIS !! */

int windowHandle;

/* O identificador do programa em GLSL combinando os SHADERS */

GLuint programaGLSL;

/* Os identificadores dos SHADERS */

GLuint vs;

GLuint fs;

/* Para passar coordenadas, cores a a matriz de transformacao ao Vertex-Shader */

GLint attribute_coord3d;

GLint attribute_corRGB;

GLint uniform_matriz_proj;

GLint uniform_matriz_model_view;

/* Texturas */

GLuint menu;

/* Matriz de projeccao */

mat4x4 matrizProj;

/*** NOVO ***/

/* Para os MODELOS */

typedef struct {

    /* O modelo */

    /* Entidades */

    GLsizei numVertices;

    GLfloat* arrayVertices;

    GLfloat* arrayNormais;

    /* Cores para cada vertice */

    GLfloat* arrayCores;

    /* Propriedades */

    /* Arrays */

    GLfloat kAmb[4];

    GLfloat kDif[4];

    GLfloat kEsp[4];

    GLfloat coefDePhong;

    /* Parametros das transformacoes */

	float deslX, deslY, deslZ;

	float angRotXX, angRotYY, angRotZZ;

	float factorEscX, factorEscY, factorEscZ;

    /* Controlos de animacao */

    int rotacaoOnXX, rotacaoOnYY, rotacaoOnZZ;

} Registo_Modelo;

typedef Registo_Modelo *pontModelo;


/* O ARRAY de modelos deste exemplo */

pontModelo* arrayModelos;

int numModelos;

/* Para os FOCOS PONTUAIS */

/* Registo para guardar as caracteristicas e os parametros globais
   de transformacao de um foco                                      */

typedef struct {

    int focoIsOn;

    /* Posicao em Coordenada Homogeneas */

    GLfloat posicao[4];

    GLfloat intensidade[4];

    GLfloat luzAmbiente[4];

    /* Parametros das transformacoes */

	float translX, translY, translZ;

	float angRotXX, angRotYY, angRotZZ;

    /* Controlos de animacao */

    int rotacaoOnXX, rotacaoOnYY, rotacaoOnZZ;

} Registo_Foco;

typedef Registo_Foco *pontFoco;

/* O ARRAY de focos de luz deste exemplo */

pontFoco* arrayFocos;

int numFocos;

/* Matriz global de transformacao */

mat4x4 matrizModelView;

/* Parametros de transformacao da CENA */

float angRotXX, angRotYY, angRotZZ;

/* FLAGS para controlar a animacao */

GLboolean animacaoModelosON;

GLboolean animacaoFocosON;

GLboolean animacaoCenaON;

typedef enum{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    PINK,
    EMPTY
}Color;

typedef enum{
    INIT,
    RUNNING,
    GAMEOVER
}GameStates;

GameStates gameState;

#endif
