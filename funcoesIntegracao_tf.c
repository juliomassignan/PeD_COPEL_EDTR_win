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

#include "data_structures_tf.h"
#include "funcoesTopologia_tf.h"
#include "funcoesFluxoVarredura_tf.h"
#include "funcoesCalculoEletrico_tf.h"
#include "funcoesMatematicas_tf.h"
#include "data_structures_modcarga_tf.h"


#include "funcoesLeituraDados.h"
#include "funcoesInicializacao.h"
#include "funcoesAuxiliares.h"
#include "funcoesRNP.h"
#include "funcoesProblema.h"
#include "funcoesSetor.h"


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


void converteGrafo_TFtoSDR(TF_GRAFO *grafo_tf,long int numeroBarras_tf,GRAFO **grafoSDRParam,DADOSREGULADOR **dadosReguladorSDRParam,long int *numeroBarras, long int *numeroTrafos, long int *numeroChaves)
{
    // declaracao de variaveis auxiliares
    // alocacao de memoria
    // seguindo o padrao do leitura
    // 
    //laço for 0 a numerodebarras
        // converte dados de barras
        // converte dados de linhas
        // converte dados de trafos
        
    //preencher dadosRegulador
    //
}

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


void converteDadosAlimentadores_TFtoSDR(TF_ALIMENTADOR *alimentadores_tf,long int numerosAlimentadores_Tf, DADOSALIMENTADOR **dadosAlimentadorSDRParam ){

    //declara variaveis
    //aloca estrutura 
    //lafuncoesInicializacaoco for

}