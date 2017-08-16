/*
 * shading.h
 *
 * Ficheiro cabecalho do moduloSHADING.
 *
 * J. Madeira - Nov/2012
 */


#ifndef _shading_h
#define _shading_h


#define GLEW_STATIC /* Necessario se houver problemas com a lib */

#include <GL/glew.h>

#include <GL/freeglut.h>


void flatShading( int indiceModelo );

void smoothShading( int indiceModelo );


#endif
