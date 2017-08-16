/*
 * shaders.c
 *
 * Ficheiro de implementacao do modulo SHADERS.
 *
 * J. Madeira - Out/2012
 */


#include <stdio.h>


#define GLEW_STATIC /* Necessario se houver problemas com a lib */

#include <GL/glew.h>

#include <GL/freeglut.h>


#include "globals.h"


// Create a NULL-terminated string by reading the provided file
// Author : Ed Angel

static char* readShaderSource(const char* shaderFile)
{
    long size;

    char* buf;

    FILE* fp = fopen(shaderFile, "r");

    if ( fp == NULL ) { return NULL; }

    fseek(fp, 0L, SEEK_END);

    size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    buf = (char*) calloc( size + 1, sizeof( char ) );

    fread(buf, 1, size, fp);

    buf[size] = '\0';

    fclose(fp);

    return buf;
}

/* VERTEX-SHADER e FRAGMENT-SHADER */

int initResources( void )
{
    const char* vsSource;

    const char* fsSource;

    char* attribute_coord3d_name;

    char* attribute_corRGB_name;

    const char* uniform_matriz_model_view_name;

    const char* uniform_matriz_proj_name;

    GLint compileOK = GL_FALSE;

    GLint linkOK = GL_FALSE;

    /* String contendo o codigo (GLSL) do vertex-shader */

    vsSource = readShaderSource( "vShader.glsl" );

    if( vsSource == NULL )
    {
	    fprintf(

            stderr,

            "ERRO no LEITURA do VERTEX-SHADER\n"

        );

	    exit( EXIT_FAILURE );
	}

    /* Para passar vertices 3D ao Vertex-Shader */

    attribute_coord3d_name = "v_coord3d";

    /* Para passar cores ao Vertex-Shader */

    attribute_corRGB_name = "v_corRGB";

    /* Para passar a matriz de projeccao ao Vertex-Shader */

    uniform_matriz_proj_name = "matrizProj";

    /* Para passar a matriz de model-view ao Vertex-Shader */

    uniform_matriz_model_view_name = "matrizModelView";

    /* String contendo o codigo (GLSL) do fragment-shader */

    fsSource = readShaderSource( "fShader.glsl" );

    if( fsSource == NULL )
    {
	    fprintf(

            stderr,

            "ERRO no LEITURA do FRAGMENT-SHADER\n"

        );

	    exit( EXIT_FAILURE );
	}
    /* Criar e compilar o Vertex-Shader */

    vs = glCreateShader( GL_VERTEX_SHADER );

    glShaderSource( vs, 1, &vsSource, NULL );

    glCompileShader( vs );

    glGetShaderiv( vs, GL_COMPILE_STATUS, &compileOK );

    if( compileOK == 0 )
    {
        fprintf( stderr, "Error in vertex shader\n" );

        return 0;
    }

    /* Criar e compilar o Fragment-Shader */

    fs = glCreateShader( GL_FRAGMENT_SHADER );

    glShaderSource( fs, 1, &fsSource, NULL );

    glCompileShader( fs );

    glGetShaderiv( fs, GL_COMPILE_STATUS, &compileOK );

    if( compileOK == 0 )
    {
        fprintf( stderr, "Error in fragment shader\n" );

        return 0;
    }

    /* Criando o programa em GLSL */

    programaGLSL = glCreateProgram();

    glAttachShader( programaGLSL, vs );

    glAttachShader( programaGLSL, fs );

    glLinkProgram( programaGLSL );

    glGetProgramiv( programaGLSL, GL_LINK_STATUS, &linkOK );

    if( linkOK == 0 )
    {
        fprintf( stderr, "Error while linking\n" );

        return 0;
    }

    /* Para input ao Vertex-Shader */

    attribute_coord3d = glGetAttribLocation( programaGLSL, attribute_coord3d_name );

    if( attribute_coord3d == -1 )
    {
        fprintf( stderr, "Could not bind attribute %s\n", attribute_coord3d_name );

        return 0;
    }

    attribute_corRGB = glGetAttribLocation( programaGLSL, attribute_corRGB_name );

    if( attribute_corRGB == -1 )
    {
        fprintf( stderr, "Could not bind attribute %s\n", attribute_corRGB_name );

        return 0;
    }

    uniform_matriz_proj = glGetUniformLocation( programaGLSL, uniform_matriz_proj_name );

    if( uniform_matriz_proj == -1 )
    {
        fprintf( stderr, "Could not bind uniform %s\n", uniform_matriz_proj_name );

        return 0;
    }

    uniform_matriz_model_view = glGetUniformLocation( programaGLSL, uniform_matriz_model_view_name );

    if( uniform_matriz_model_view == -1 )
    {
        fprintf( stderr, "Could not bind uniform %s\n", uniform_matriz_model_view_name );

        return 0;
    }

    return 1;
}


void freeResources( void )
{
    glDetachShader( programaGLSL, vs );

    glDetachShader( programaGLSL, fs );

    glDeleteProgram( programaGLSL );
}

