/*
 * shading.c
 *
 * Ficheiro de implementacao do modulo SHADING.
 *
 * J. Madeira - Nov/2012
 */


/* Incluir os ficheiros cabecalhos necessarios */


#include <stdio.h>

#include "globals.h"


/* FLAT SHADING usando o modelo de iluminacao de Phong */

void flatShading( int indiceModelo )
{
    /* COMPLETAR */

}


/* SMOOTH SHADING usando o modelo de iluminacao de Phong */

void smoothShading( int indiceModelo )
{
    int i;

    int f;

    int indexArrayVertices;

    int indexArrayCores;

    float auxP[4];

    float* pontoP = NULL;

    float auxN[4];

    float* vectorN = NULL;

    float* vectorL = NULL;

    float* vectorV = NULL;

    float* vectorH = NULL;

    float cosNL;

    float cosNH;

    GLfloat ambientTerm[3];

    GLfloat diffuseTerm[3];

    GLfloat specularTerm[3];

    mat4x4 matrizTransfFoco;

    for( indexArrayCores = 0; indexArrayCores < ( 3 * arrayModelos[indiceModelo]->numVertices ); indexArrayCores++ )
    {
        arrayModelos[indiceModelo]->arrayCores[indexArrayCores] = 0.0;
    }

    /* SMOOTH-SHADING : Calcular para cada vertice de cada triangulo */

    for( indexArrayVertices = 0; indexArrayVertices < ( 3 * arrayModelos[indiceModelo]->numVertices ); indexArrayVertices += 3 )
    {
        /* Para cada vértice */

        for( i = 0; i < 3; i++ )
        {
            auxP[i] = arrayModelos[indiceModelo]->arrayVertices[ indexArrayVertices + i ];

            auxN[i] = arrayModelos[indiceModelo]->arrayNormais[ indexArrayVertices + i ];
        }

        /* The 4th homogeneous coordinate */

        auxP[3] = 1.0;

        auxN[3] = 0.0;

        /* Aplicar a matriz Model-View */

        pontoP = multiplyPointByMatrix( &matrizModelView, auxP );

        vectorN = multiplyVectorByMatrix( &matrizModelView, auxN );

        convertToUnitVector( vectorN );

        vectorV = computeSymmetricVector( pontoP );

        convertToUnitVector( vectorV );

        /* PARA CADA FONTE DE LUZ PONTUAL */

        for( f = 0; f < numFocos; f++ )
        {
            if( arrayFocos[f]->focoIsOn == 0 )
            {
                continue;
            }

            for( i = 0; i < 3; i++ )
            {
                ambientTerm[i] = arrayFocos[f]->luzAmbiente[i] * arrayModelos[indiceModelo]->kAmb[i];

                diffuseTerm[i] = arrayFocos[f]->intensidade[i] * arrayModelos[indiceModelo]->kDif[i];

                specularTerm[i] = arrayFocos[f]->intensidade[i] * arrayModelos[indiceModelo]->kEsp[i];
            }

            matrizTransfFoco = IDENTITY_MATRIX;

            if( arrayFocos[f]->rotacaoOnXX )
            {
                RotateAboutX( &matrizTransfFoco, DegreesToRadians( arrayFocos[f]->angRotXX ) );
            }

            if( arrayFocos[f]->rotacaoOnYY )
            {
                RotateAboutY( &matrizTransfFoco, DegreesToRadians( arrayFocos[f]->angRotYY ) );
            }

            if( arrayFocos[f]->rotacaoOnZZ )
            {
                RotateAboutZ( &matrizTransfFoco, DegreesToRadians( arrayFocos[f]->angRotZZ ) );
            }

            if( arrayFocos[f]->posicao[3] == 0.0 )
            {
                /* Foco DIRECCIONAL */

                vectorL = multiplyVectorByMatrix( &matrizTransfFoco, &( arrayFocos[f]->posicao[0] ) );
            }
            else
            {
                /* Foco PONTUAL */

                vectorL = multiplyPointByMatrix( &matrizTransfFoco, &( arrayFocos[f]->posicao[0] ) );

                for( i = 0; i < 3; i++ )
                {
                    vectorL[ i ] -= pontoP[ i ];
                }
            }

            convertToUnitVector( vectorL );

            cosNL = computeScalarProduct( vectorN, vectorL );

            if( cosNL < 0.0 )
            {
                cosNL = 0.0;
            }

            /* COMPONENTE ESPECULAR */

            vectorH = addVector( vectorL, vectorV );

            convertToUnitVector( vectorH );

            cosNH = computeScalarProduct( vectorN, vectorH );

            if( (cosNH < 0.0) || (cosNL <= 0.0) )
            {
                cosNH = 0.0;
            }

            /* EVITAR valores superiores a 1.0 */

            indexArrayCores = indexArrayVertices; /* ATENCAO !! */

            arrayModelos[indiceModelo]->arrayCores[indexArrayCores] += ambientTerm[0]
                                                         + diffuseTerm[0] * cosNL
                                                         + specularTerm[0] * pow(cosNH, arrayModelos[indiceModelo]->coefDePhong);

            if( arrayModelos[indiceModelo]->arrayCores[indexArrayCores] > 1.0 )
            {
                arrayModelos[indiceModelo]->arrayCores[indexArrayCores] = 1.0;
            }

            indexArrayCores++;

            arrayModelos[indiceModelo]->arrayCores[indexArrayCores] += ambientTerm[1]
                                                         + diffuseTerm[1] * cosNL
                                                         + specularTerm[1] * pow(cosNH, arrayModelos[indiceModelo]->coefDePhong);

            if( arrayModelos[indiceModelo]->arrayCores[indexArrayCores] > 1.0 )
            {
                arrayModelos[indiceModelo]->arrayCores[indexArrayCores] = 1.0;
            }

            indexArrayCores++;

            arrayModelos[indiceModelo]->arrayCores[indexArrayCores] += ambientTerm[2]
                                                         + diffuseTerm[2] * cosNL
                                                         + specularTerm[2] * pow(cosNH, arrayModelos[indiceModelo]->coefDePhong);

            if( arrayModelos[indiceModelo]->arrayCores[indexArrayCores] > 1.0 )
            {
                arrayModelos[indiceModelo]->arrayCores[indexArrayCores] = 1.0;
            }

            indexArrayCores++;

            free( vectorL );

            free( vectorH );

        }

        free( pontoP );

        free( vectorN );

        free( vectorV );
    }
}


