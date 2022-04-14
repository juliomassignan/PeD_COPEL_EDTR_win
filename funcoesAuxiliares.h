/* 
 * File:   funcoesAuxiliares.h
 * Author: sony
 *
 * Created on 20 de Novembro de 2013, 19:49
 */

#ifndef FUNCOESAUXILIARES_H
#define	FUNCOESAUXILIARES_H
/**
 * @brief A função RandomInteger devolve um inteiro 
* aleatório entre inferior e superior.
 * @param inferior
 * @param superior
 * @return 
 */

int inteiroAleatorio( int inferior, int superior);

void alocaMatrizInteiros(long int ***matrizParam, long int numeroLinhas, long int numeroColunas);
void alocaMatrizDouble(double ***matrizParam, long int numeroLinhas, long int numeroColunas);
void desalocaMatrizInteiros(long int **matrizParam, long int numeroLinhas);
void desalocaGrafoSDR(GRAFO *grafoSDRParam, long int numeroBarras);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FUNCOESAUXILIARES_H */

