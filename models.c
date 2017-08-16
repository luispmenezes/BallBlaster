/*
 * models.c
 *
 * Ficheiro de implementacao do modulo MODELS.
 *
 * J. Madeira - Out/2012
 */


/* Incluir os ficheiros cabecalhos necessarios */


#include <stdio.h>

#include <stdlib.h>

#include <math.h>


#include "models.h"

#include "mathUtils.h"


/* Ler um modelo de ficheiro */

/* FORMATO SIMPLES: x y z */

/* VERTICES REPLICADOS */

/* Atribuida MEMORIA ao array dos vertices */

/* Nao sao verificados erros de formatacao do ficheiro */

void lerVerticesDeFicheiro( char* nome, int* numVertices, GLfloat** arrayVertices )
{
    int i;

    int j;

    int n;

    int indexArrayCoords;

    GLfloat* coordenadas;

    FILE* fp = fopen( nome, "r" );

    if ( fp == NULL )
    {
        fprintf( stderr, "ERRO na leitura do ficheiro %s\n", nome );

        exit( EXIT_FAILURE );
    }

    /* Ler o numero de vertices */

    fscanf( fp, "%d", &n );

    *numVertices = n;

    /* Criar os arrays */

    /* Coordenadas ( x, y, z ) dos vertices */

    *arrayVertices = (GLfloat*) malloc( 3 * n * sizeof( GLfloat ) );

    coordenadas = *arrayVertices;

    /* Ler a informacao de cada vertice */

    indexArrayCoords = 0;

    for( i = 0; i < n; i++ )
    {
        for( j = 0; j < 3; j++ )
        {
            fscanf( fp, "%f", &( coordenadas[ indexArrayCoords++ ] ) );
        }
    }

    fclose( fp );
}


void escreverVerticesEmFicheiro( char* nome, int numVertices, GLfloat* arrayVertices )
{
    int i;

    int j;

    int indexArrayCoords;

    FILE* fp = fopen( nome, "w");

    if ( fp == NULL )
    {
        fprintf( stderr, "ERRO na escrita do ficheiro %s\n", nome );

        exit( EXIT_FAILURE );
    }

    /* Escrever o numero de vertices */

    fprintf( fp, "%d\n", numVertices );

    /* Percorrer os arrays */

    /* Coordenadas ( x, y, z ) dos vertices */

    indexArrayCoords = 0;

    for( i = 0; i < numVertices; i++ )
    {
        for( j = 0; j < 3; j++ )
        {
            fprintf( fp, "%f ", arrayVertices[ indexArrayCoords++ ] );
        }

        fprintf( fp, "\n" );
    }

    fclose( fp );
}


void lerFicheiroOBJ( char* nome, int* numVertices, GLfloat** arrayVertices, GLfloat** arrayNormais )
{
    /* Ler versao SIMPLIFICADA do formato OBJ */

    /* SAO NECESSARIOS OS VERTICES REPLICADOS */

    /* NAO SE LE A INFORMACAO DE FACE */

    FILE* pFicheiro;

    char linha[255];

    char car;

    GLfloat x, y, z;

    int contadorVertices = 0;

    int contadorNormais = 0;

    int indVert = 0;

    int indNorm = 0;

    /* Le apenas a informacao geometrica */

    /* E aceita apenas TRIANGULOS */

    /* Verificar se o argumento da funcao e validos */

    if( !nome )
    {
        exit( EXIT_FAILURE );
    }

    /* Estrategia:

       - percorrer uma vez o ficheiro, para contar vertices

       - criar os arrays de vertices e de normais

       - percorrer uma segunda vez, lendo a informacao para os arrays

    */

    /* Abrir o ficheiro para leitura */

    pFicheiro = fopen( nome, "r" );

    if( !pFicheiro )
    {
        fprintf( stderr, "ERRO na leitura do ficheiro %s\n", nome );

        exit( EXIT_FAILURE );
    }

   	while( !feof( pFicheiro ) )
	{
		/* Primeiro caracter da linha corrente */

		car = fgetc( pFicheiro );

		switch( car )
		{
		  case '\n' :    /* Linha em branco */

		      break;

		  case '#' :    /* Comentario */

		      /* Saltar o resto da linha */

		      fgets( linha, 200, pFicheiro );

		      break;

		  case 'v' :	/* Vertice ou vector normal */

              car = fgetc( pFicheiro );

              if( car == ' ' )
              {
                 contadorVertices++;
              }
              else if( car == 'n' )
              {
                 contadorNormais++;
              }

		      /* Saltar o resto da linha */

		      fgets( linha, 200, pFicheiro );

		      break;

		  case 'f' :	/* Face */

		      /* Saltar o resto da linha */

		      fgets( linha, 200, pFicheiro );

		      break;
        }
    }

    /* Fechar o ficheiro */

    fclose( pFicheiro );

    /* Arrays */

    *numVertices = contadorVertices;

    *arrayVertices = (GLfloat*) malloc( 3 * contadorVertices * sizeof( GLfloat ) );

    *arrayNormais = (GLfloat*) malloc( 3 * contadorVertices * sizeof( GLfloat ) );

    /* Abrir, de novo, o ficheiro */

    pFicheiro = fopen( nome, "r" );

   	while( !feof( pFicheiro ) )
	{
		/* Primeiro caracter da linha corrente */

		car = fgetc( pFicheiro );

		switch( car )
		{
		  case '\n' :    /* Linha em branco */

		      break;

		  case '#' :    /* Comentario */

		      /* Saltar o resto da linha */

		      fgets( linha, 200, pFicheiro );

		      break;

		  case 'v' :	/* Vertice ou vector normal */

              car = fgetc( pFicheiro );

              if( car == ' ' )
              {
                 /* Ler as coordenadas do vertice */

                 fscanf( pFicheiro, "%f%f%f", &x, &y, &z );

                 (*arrayVertices)[indVert++] = x;

                 (*arrayVertices)[indVert++] = y;

                 (*arrayVertices)[indVert++] = z;
              }
              else if( car == 'n' )
              {
                 /* Ler as componentes do vector */

                 fscanf( pFicheiro, "%f%f%f", &x, &y, &z );

                 (*arrayNormais)[indNorm++] = x;

                 (*arrayNormais)[indNorm++] = y;

                 (*arrayNormais)[indNorm++] = z;
              }

		      /* Saltar o resto da linha */

		      fgets( linha, 200, pFicheiro );

		      break;

		  case 'f' :	/* Face */

		      /* Saltar o resto da linha */

		      fgets( linha, 200, pFicheiro );

		      break;
        }
    }

    /* Fechar o ficheiro */

    fclose( pFicheiro );

    /* Calcular as normais, caso nao tenham sido lidas */

    if( indNorm == 0 )
    {
        free( *arrayNormais );

        *arrayNormais = calcularNormaisTriangulos( *numVertices, *arrayVertices );
    }
}


GLfloat* calcularNormaisTriangulos( int numVertices, GLfloat* arrayVertices )
{
    GLfloat* arrayNormais = (GLfloat*) malloc( 3 *numVertices * sizeof( GLfloat ) );

    int index = 0; /* Indice para o proximo elemento */

    int j = 0;

    /* Para cada face triangular do modelo */

    for( index = 0; index < (3 * numVertices); index += 9 )
    {
        GLfloat* normalVector;

        normalVector = computeUnitNormalToTriangle( &arrayVertices[index] );

        /* Store the unit normal vector for each vertex */

        for( j = 0; j < 3; j++ )
        {
            arrayNormais[index + j] = normalVector[j];

            arrayNormais[index + j + 3] = normalVector[j];

            arrayNormais[index + j + 6] = normalVector[j];
        }

        free( normalVector );
    }

    return arrayNormais;
}
