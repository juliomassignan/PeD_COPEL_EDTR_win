#include <stdlib.h>
#include <stdio.h>

#include "data_structures.h"
/**
 * @brief A função RandomInteger devolve um inteiro 
* aleatório entre inferior e superior.
 * @param inferior
 * @param superior
 * @return 
 */
int inteiroAleatorio( int inferior, int superior)
{
    int k;
    int d;
   // d = inferior + rand( ) % superior;
    d=  rand() % (superior - inferior + 1) + inferior;
   // k = d * (superior - inferior + 1);
    return d;
}

void alocaMatrizInteiros(long int ***matrizParam, long int numeroLinhas, long int numeroColunas)
{
    long int contadorL;
    //faz a alocação da matriz
    if (((*matrizParam) = (long int **)malloc((numeroLinhas) * sizeof(long int *)))==NULL)
    {
        printf("ERRO ao alocar memoria para matriz de Inteiros!!!");
        exit(1);
    }
    for(contadorL= 0; contadorL< numeroLinhas; contadorL++)
    {
        (*matrizParam)[contadorL] = (long int*) malloc(numeroColunas * sizeof(long int));
    }
}

void desalocaMatrizInteiros(long int **matrizParam, long int numeroLinhas)
{
    long int contadorL;
    //faz a desalocação da matriz
     for(contadorL= 0; contadorL< numeroLinhas; contadorL++)
    {
        free(matrizParam[contadorL]);
    }
    free(matrizParam);
}

void desalocaGrafoSDR(GRAFO *grafoSDRParam, long int numeroBarras)
{
    long int contador;
    for(contador = 1; contador<numeroBarras; contador++)
    {
        free(grafoSDRParam[contador].adjacentes);
    }
    free(grafoSDRParam);

}

void alocaMatrizDouble(double ***matrizParam, long int numeroLinhas, long int numeroColunas)
{
    long int contadorL;
    //faz a alocação da matriz
    if (((*matrizParam) = (double **)malloc((numeroLinhas) * sizeof(double *)))==NULL)
    {
        printf("ERRO ao alocar memoria para matriz de Doubles!!!");
        exit(1);
    }
    for(contadorL= 0; contadorL< numeroLinhas; contadorL++)
    {
        (*matrizParam)[contadorL] = (double*) malloc(numeroColunas * sizeof(double));
    }
}
