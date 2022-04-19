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
#include "data_structures.h"

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


void converteGrafo_TFtoSDR(TF_GRAFO *grafo_tf,long int numeroBarras_tf,GRAFO **grafoSDRParam,DADOSREGULADOR **dadosReguladorSDRParam,long int* numeroBarras, long int *numeroTrafos, long int *numeroChaves)
{
    
    int i,k;
    
    // declaracao de variaveis auxiliares
    // alocacao de memoria
    (*numeroBarras)=numeroBarras_tf;
    //realiza a alocação da estrutura em grafo para armazenar o SDR e verifica se o processo foi bem sucedido
    //caso não seja possível realizar a alocação o programa é encerrado

    if (((*grafoSDRParam) = (GRAFO *)malloc((numeroBarras_tf+1) * sizeof(GRAFO)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para o grafo do SDR !!!!");
        exit(1); 
    }

    for (i=0;i<numeroBarras_tf;i++)
    {
        (*grafoSDRParam)[i].idNo=grafo_tf[i].idNo;
        //preenche tipo do nó
        if (grafo_tf[i].tipoNo== 0)(*grafoSDRParam)[i].tipoNo= semCarga;
        else if (grafo_tf[i].tipoNo== 1)(*grafoSDRParam)[i].tipoNo= comCarga;   
        else if (grafo_tf[i].tipoNo==2)(*grafoSDRParam)[i].tipoNo= capacitor;   
        else if (grafo_tf[i].tipoNo==3)(*grafoSDRParam)[i].tipoNo= raizAlimentador;

        //Perguntar ao julio sobre o medidor RAMO
        if(grafo_tf[i].nmedPQ>0)
        {
            (*grafoSDRParam)[i].medicao=medidorClasseA;
        }
        else
        {
            (*grafoSDRParam)[i].medicao=semMedidor;
        }    
        // preecnhe o indice que indica a área de medição
        (*grafoSDRParam)[i].idAM=grafo_tf[i].idAM;
        // preenche o numero de nos adjacentes
        (*grafoSDRParam)[i].numeroAdjacentes=grafo_tf[i].numeroAdjacentes;
        // preencher os alica os adjacentes (precisa preencher)
        (*grafoSDRParam)[i].adjacentes=Malloc(NOADJACENTE,11);

        //preencher info dos adjacentes
        for (k=0;k<grafo_tf[i].numeroAdjacentes;k++)
        {
            (*grafoSDRParam)[i].adjacentes[k].idNo=grafo_tf[i].adjacentes[k].idNo;
            //conferir com o julio
            sprintf((*grafoSDRParam)[i].adjacentes[k].idAresta,"%ld",grafo_tf[i].adjacentes[k].idram);
            // informa se há medidores nos ramos
            if(grafo_tf[i].adjacentes[k].nmed>0)(*grafoSDRParam)[i].adjacentes[k].medicao=medidorRamo;
            else (*grafoSDRParam)[i].adjacentes[k].medicao=semMedidor;
            //
 
            
            if(grafo_tf[i].adjacentes[k].tipo==ramal||grafo_tf[i].adjacentes[k].tipo==trafo)(*grafoSDRParam)[i].adjacentes[k].tipoAresta=trecho;
            else if (grafo_tf[i].adjacentes[k].tipo==regulador) (*grafoSDRParam)[i].adjacentes[k].tipoAresta=reguladorTensao;
            else if (grafo_tf[i].adjacentes[k].tipo==chave) (*grafoSDRParam)[i].adjacentes[k].tipoAresta=chaveManual;
            //grafo trifasico não le chave automatica?

            //preenche o indice AM
            (*grafoSDRParam)[i].adjacentes[k].idAM=grafo_tf[i].adjacentes[k].idAM;
            
            //descobrir
            (*grafoSDRParam)[i].adjacentes[k].condicao=emOperacao;
            // descobrir deve ficar junto com o tipo de aresta
            (*grafoSDRParam)[i].adjacentes[k].subTipoAresta=outrosSubTipo;

            //valores de sequ pos?
            (*grafoSDRParam)[i].adjacentes[k].resistencia=0;
            (*grafoSDRParam)[i].adjacentes[k].reatancia=0;
            //O que é isso?
            (*grafoSDRParam)[i].adjacentes[k].ampacidade=0;


        }


    }
    printf("sucesso\n\n");
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