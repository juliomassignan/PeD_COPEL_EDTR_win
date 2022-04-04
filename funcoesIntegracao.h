/* 
 * File:   funcoesIntegracao.h
 * Author: Julio Massignan
 *
 * Created on 24 de Fevereiro de 2017, 14:03
 */

#ifndef funcoesCalculoEletrico_H
#define	funcoesCalculoEletrico_H

//
/**
 * @brief 
 *
 * Essa função realiza 
 * 
 * @param 
 * @return c
 * @see 
 * @note
 * @warning .
 */
void avaliaConfiguracao(BOOL todosAlimentadores, CONFIGURACAO *configuracoesParam, int rnpA, int rnpP, long int idNovaConfiguracaoParam, DADOSTRAFO *dadosTrafoParam, int numeroTrafosParam,
        int numeroAlimentadoresParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulador, DADOSALIMENTADOR *dadosAlimentadorParam, double VFParam, int idAntigaConfiguracaoParam, RNPSETORES *matrizB, MATRIZCOMPLEXA *ZParam,
        MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam, BOOL copiarDadosEletricos);

#endif	/* funcoesIntegracao_H */