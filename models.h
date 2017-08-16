/*
 * models.h
 *
 * Ficheiro cabecalho do modulo MODELS.
 *
 * J. Madeira - Out/2012
 */


#ifndef _models_h
#define _models_h


#define GLEW_STATIC /* Necessario se houver problemas com a lib */

#include <GL/glew.h>

#include <GL/freeglut.h>


void lerVerticesDeFicheiro( char* nome, int* numVertices, GLfloat** arrayVertices );

void escreverVerticesEmFicheiro( char* nome, int numVertices, GLfloat* arrayVertices );

void lerFicheiroOBJ( char* nome, int* numVertices, GLfloat** arrayVertices, GLfloat** arrayNormais );

/* ------------------------- */

GLfloat* calcularNormaisTriangulos( int numVertices, GLfloat* arrayVertices );

#endif
