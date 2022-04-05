/**
 * @file funcoesFluxoVarredura.c
 * @author Julio Augusto Druzina Massignan
 * @date 31 jul 2020
 * @brief Contém a implementação das funções utilizadas para o cálculo de fluxo de potência trifásico por Varredura Direta/Inversa por soma de correntes.
 * Este arquivo implementa as funções definidas na biblioteca funcoesFluxoVarredura.h
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <complex.h>
#include <omp.h> 

#include "data_structures.h"
#include "funcoesTopologia.h"
#include "funcoesFluxoVarredura.h"
#include "funcoesCalculoEletrico.h"
#include "funcoesMatematicas.h"
#include "data_structures_modcarga.h"

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
        MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam, BOOL copiarDadosEletricos) {
    long int contador;
    double quedaMaxima, menorTensao, VF, perdasTotais;
    perdasTotais = 0;
    //verifica se deve manter os dados elétricos para cada configuração gerada individualmente. Para isso é necessário copiar os dados elétricos de uma configuração para outra
    if (idNovaConfiguracaoParam != idAntigaConfiguracaoParam) {
        if (copiarDadosEletricos) {
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.iJusante = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.corrente = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.potencia = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.vBarra = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            for (contador = 1; contador <= numeroBarrasParam; contador++) {
                configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.iJusante[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.iJusante[contador];
                configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.corrente[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.corrente[contador];
                configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.potencia[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.potencia[contador];
                configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.vBarra[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.vBarra[contador];
            }
        } else { //não mantém os dados elétricos individualmente, somente copia os ponteiros para os vetores com dados elétricos
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.iJusante = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.iJusante;
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.corrente = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.corrente;
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.potencia = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.potencia;
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.vBarra = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.vBarra;
        }
    }

    

    configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo = malloc((numeroTrafosParam + 1) * sizeof (__complex__ double));
    configuracoesParam[idNovaConfiguracaoParam].objetivo.menorTensao = 100000;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoRede = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.perdasResistivas = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.ponderacao = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.quedaMaxima = 0;
    if (todosAlimentadores) //calcula os valores de fitness para todos os alimentadores
    {
        fluxoCargaTodosAlimentadores(numeroBarrasParam,dadosAlimentadorParam,dadosTrafoParam, configuracoesParam, idNovaConfiguracaoParam, matrizB, ZParam, indiceRegulador, dadosRegulador, maximoCorrenteParam);
    } else { //calcula o fluxo de carga somente para os alimentadores que foram alterados.
        VF = 1000*dadosTrafoParam[dadosAlimentadorParam[rnpP+1].idTrafo].tensaoReal/sqrt(3);
        fluxoCargaAlimentador(numeroBarrasParam, configuracoesParam, VF, rnpP, idNovaConfiguracaoParam, matrizB, ZParam, maximoCorrenteParam, indiceRegulador, dadosRegulador);
        VF = 1000*dadosTrafoParam[dadosAlimentadorParam[rnpA+1].idTrafo].tensaoReal/sqrt(3);
        fluxoCargaAlimentador(numeroBarrasParam, configuracoesParam, VF, rnpA, idNovaConfiguracaoParam, matrizB, ZParam, maximoCorrenteParam, indiceRegulador, dadosRegulador);
    }
    int indiceRNP;
    for (indiceRNP = 0; indiceRNP < configuracoesParam[idNovaConfiguracaoParam].numeroRNP; indiceRNP++) {
      //  printf("alimentador %d carregamento %lf\n",indiceRNP+1,configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.maiorCarregamentoCorrente*100);
        quedaMaxima = 100000;
        VF = 1000*dadosTrafoParam[dadosAlimentadorParam[indiceRNP+1].idTrafo].tensaoReal/sqrt(3);
        menorTensao = configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.menorTensao;
        if (quedaMaxima > menorTensao) // encontra a menor tensão do sistema
            quedaMaxima = menorTensao;
        //atualiza a menor tensão do individuo
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.menorTensao > menorTensao)
            configuracoesParam[idNovaConfiguracaoParam].objetivo.menorTensao = menorTensao;
        quedaMaxima = ((VF - quedaMaxima) / VF)*100;
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.quedaMaxima < quedaMaxima)
            configuracoesParam[idNovaConfiguracaoParam].objetivo.quedaMaxima = quedaMaxima;
        //potencia
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoRede < configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.carregamentoRede)
            configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoRede = configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.carregamentoRede;
        //máximo de corrente
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente < configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.maiorCarregamentoCorrente)
            configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente = configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.maiorCarregamentoCorrente;
        //total perdas
        //if (configuracoesParam[idNovaConfiguracaoParam].objetivo.perdasResistivas < configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.perdasResistivas)
          //  configuracoesParam[idNovaConfiguracaoParam].objetivo.perdasResistivas = configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.perdasResistivas;
        perdasTotais+=configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.perdasResistivas;

    }
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente = configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente * 100;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.perdasResistivas = perdasTotais;
    carregamentoTrafo(dadosTrafoParam, numeroTrafosParam, numeroAlimentadoresParam, dadosAlimentadorParam, configuracoesParam, idNovaConfiguracaoParam, idAntigaConfiguracaoParam, todosAlimentadores, rnpP, rnpA);
    calculaPonderacao(configuracoesParam, idNovaConfiguracaoParam, VFParam);
}