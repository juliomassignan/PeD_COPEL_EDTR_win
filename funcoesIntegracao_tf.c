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


void converteGrafo_TFtoSDR(TF_GRAFO *grafo_tf,long int numeroBarras_tf,TF_DRAM *ramos_tf,long int nramos_tf,GRAFO **grafoSDRParam,DADOSREGULADOR **dadosReguladorSDRParam,long int* numeroBarras, long int *numeroTrafos, long int *numeroChaves)
{
    
    int i,k,contador;
    int nreg=0;


    (*numeroChaves)=0;
    (*numeroTrafos)=0;
    // declaracao de variaveis auxiliares
    // alocacao de memoria
    (*numeroBarras)=numeroBarras_tf;
    
    //realiza a alocação da estrutura em grafo para armazenar o SDR e verifica se o processo foi bem sucedido
    //caso não seja possível realizar a alocação o programa é encerrado
    //a MRAM COMECA A INDEXACAO DO GRAFO EM 1

    if (((*grafoSDRParam) = (GRAFO *)malloc((numeroBarras_tf+1) * sizeof(GRAFO)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para o grafo do SDR !!!!");
        exit(1); 
    }

    for (i=0;i<numeroBarras_tf;i++)
    {
        contador=i+1;//MARAN indexa o grafo a partir de 1
        (*grafoSDRParam)[contador].idNo=grafo_tf[i].idNo+1;//MARAN indexa o grafo a partir de 1 o idNo é usado para indexar a MRAN ?
        //preenche tipo do nó
        if (grafo_tf[i].tipoNo== 0)(*grafoSDRParam)[contador].tipoNo= semCarga;
        else if (grafo_tf[i].tipoNo== 1)(*grafoSDRParam)[contador].tipoNo= comCarga;   
        else if (grafo_tf[i].tipoNo==2)(*grafoSDRParam)[contador].tipoNo= capacitor;   
        else if (grafo_tf[i].tipoNo==3)(*grafoSDRParam)[contador].tipoNo= raizAlimentador;

        if(grafo_tf[i].nmedPQ>0)
        {
            (*grafoSDRParam)[contador].medicao=medidorClasseA;
        }
        else
        {
            (*grafoSDRParam)[contador].medicao=semMedidor;
        }    
        // preecnhe o indice que indica a área de medição
        (*grafoSDRParam)[contador].idAM=grafo_tf[i].idAM+1;//comeca do zero ou do 1?
        // preenche o numero de nos adjacentes
        (*grafoSDRParam)[contador].numeroAdjacentes=grafo_tf[i].numeroAdjacentes;
        // preencher os alica os adjacentes (precisa preencher)
        (*grafoSDRParam)[contador].adjacentes=Malloc(NOADJACENTE,11);

        //preencher info dos adjacentes
        for (k=0;k<grafo_tf[i].numeroAdjacentes;k++)
        {
            
            (*grafoSDRParam)[contador].adjacentes[k].idNo=grafo_tf[i].adjacentes[k].idNo+1;//MARAN indexa o grafo a partir de 1
            //conferir com o julio
            sprintf((*grafoSDRParam)[contador].adjacentes[k].idAresta,"%ld",grafo_tf[i].adjacentes[k].idram);
            // informa se há medidores nos ramos
            if(grafo_tf[i].adjacentes[k].nmed>0)(*grafoSDRParam)[contador].adjacentes[k].medicao=medidorRamo;
            else (*grafoSDRParam)[contador].adjacentes[k].medicao=semMedidor;
 
            
            if(grafo_tf[i].adjacentes[k].tipo==ramal||grafo_tf[i].adjacentes[k].tipo==trafo)
            {
                (*grafoSDRParam)[contador].adjacentes[k].tipoAresta=trecho;
                if (grafo_tf[i].adjacentes[k].tipo==trafo)
                {
                  (*numeroTrafos)=(*numeroTrafos)+1;
                }
            }
            
            else if (grafo_tf[i].adjacentes[k].tipo==regulador) (*grafoSDRParam)[contador].adjacentes[k].tipoAresta=reguladorTensao;
            else if (grafo_tf[i].adjacentes[k].tipo==chave)
            {
                (*grafoSDRParam)[contador].adjacentes[k].tipoAresta=chaveManual; //tipos de chave, onde obtêlos
                (*numeroChaves)=(*numeroChaves)+1;
            }
            
            //grafo trifasico não le chave automatica?

            //preenche o indice AM
            (*grafoSDRParam)[contador].adjacentes[k].idAM=grafo_tf[i].adjacentes[k].idAM+1;//sera q tem q adicionar 1?
            
            //descobrir
            
            if(grafo_tf[i].adjacentes[k].estado==fechado)
            {
                (*grafoSDRParam)[contador].adjacentes[k].condicao=emOperacao;
            }

            
            // descobrir deve ficar junto com o tipo de aresta
            (*grafoSDRParam)[contador].adjacentes[k].subTipoAresta=outrosSubTipo;

            //valores de sequ pos?
            (*grafoSDRParam)[contador].adjacentes[k].resistencia=0;
            (*grafoSDRParam)[contador].adjacentes[k].reatancia=0;
            //O que é isso?
            (*grafoSDRParam)[contador].adjacentes[k].ampacidade=ramos_tf[grafo_tf[i].adjacentes[k].idram].ampacidade;

        }

        (*grafoSDRParam)[contador].valorPQ.p=__real__ grafo_tf[i].S[0]+ grafo_tf[i].S[1]+grafo_tf[i].S[2];//unidade?
        (*grafoSDRParam)[contador].valorPQ.q=__imag__ grafo_tf[i].S[0]+ grafo_tf[i].S[1]+grafo_tf[i].S[2];//unidade?
        (*grafoSDRParam)[contador].priorizacoes.eusdGrupoA=0; // nao sei de onde vem
        (*grafoSDRParam)[contador].priorizacoes.eusdGrupoB=0; // nao sei de onde vem
        (*grafoSDRParam)[contador].priorizacoes.qtdConsumidores=0;
        (*grafoSDRParam)[contador].priorizacoes.qtdConsumidoresEspeciais=0;
        // infos abaixo devem ser lidas
        // (*grafoSDRParam)[contador].compensacoes.metaDicAnual = dadosCompensacao.metaDicAnual;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.metaDicMensal = dadosCompensacao.metaDicMensal;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.metaDicTrimestral = dadosCompensacao.metaDicTrimestral;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.metaDmicMensal = dadosCompensacao.metaDmicMensal;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.metaFicAnual = dadosCompensacao.metaFicAnual;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.metaFicMensal = dadosCompensacao.metaFicMensal;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.metaFicTrimestral = dadosCompensacao.metaFicTrimestral;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.resulDicAcumAno = dadosCompensacao.resulDicAcumAno;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.resulDicAcumMes = dadosCompensacao.resulDicAcumMes;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.resulDicAcumTrimestre = dadosCompensacao.resulDicAcumTrimestre;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.resulDmicMes = dadosCompensacao.resulDmicMes;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.resulFicAcumAno = dadosCompensacao.resulFicAcumAno;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.resulFicAcumMes = dadosCompensacao.resulFicAcumMes;// de onde vem essas infos
        // (*grafoSDRParam)[contador].compensacoes.resulFicAcumTrimestre = dadosCompensacao.resulFicAcumTrimestre;// de onde vem essas infos
        // (*grafoSDRParam)[contador].priorizacoes.eusdGrupoA = dadosPriorizacao.eusdGrupoA;// de onde vem essas infos
        // (*grafoSDRParam)[contador].priorizacoes.eusdGrupoB = dadosPriorizacao.eusdGrupoB;// de onde vem essas infos
        // (*grafoSDRParam)[contador].priorizacoes.qtdConsumidores = dadosPriorizacao.qtdConsumidores;// de onde vem essas infos
        // (*grafoSDRParam)[contador].priorizacoes.qtdConsumidoresEspeciais = dadosPriorizacao.qtdConsumidoresEspeciais;// de onde vem essas infos 
       
        // (*grafoSDRParam)[i].idSetor= //esta info esta no grafo tf??

    }

    for (i=0;i<nramos_tf;i++)
    {
        if (ramos_tf[i].tipo==regulador)
        {
            nreg++;
        }
    }


    //realiza a alocação da estrutura em grafo para armazenar o SDR e verifica se o processo foi bem sucedido
    //caso não seja possível realizar a alocação o programa é encerrado

    if (((*dadosReguladorSDRParam) = (DADOSREGULADOR *)malloc((nreg+1) * sizeof(DADOSREGULADOR)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para o grafo do SDR !!!!");
        exit(1); 
    }
    k=0;
    for (i=0;i<nramos_tf;i++)
    {
        if (ramos_tf[i].tipo==regulador)
        {
        sprintf((*dadosReguladorSDRParam)[i].idRegulador,"%ld",i+1);
        // (*dadosReguladorSDRParam)[i].tipoRegulador= //??
        (*dadosReguladorSDRParam)[k].tipoRegulador=ramos_tf[i].ampacidade;
        (*dadosReguladorSDRParam)[k].tipoRegulador=ramos_tf[i].regulador.ntaps;
        k++;
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

    int contador, i;
    //declara variaveis
    //aloca estrutura 
    //lafuncoesInicializacaoco for
    if (((*dadosAlimentadorSDRParam)= (DADOSALIMENTADOR *)malloc( (numeroAlimentadores+1) * sizeof(DADOSALIMENTADOR)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para alimentadores !!!!");
        exit(1); 
    }


    for ( i = 0; i < numeroAlimentadores; i++)
    {
        contador=i+1;
        (*dadosAlimentadorSDRParam)[contador].barraAlimentador = alimentadores_tf[i].noRaiz; //é usado para indexar?
        sprintf((*dadosAlimentadorSDRParam)[contador].idAlimentador,"%ld",alimentadores_tf[i].idAlim); //passa o id do alimentador, é um int no código do estimador
        (*dadosAlimentadorSDRParam)[contador].idTrafo = alimentadores_tf[i].idRaiz;// nao tem equivalente no alimentadores_tf. conferir com o Julio
        //codigo operacional?
        (*dadosAlimentadorSDRParam)[contador].numeroSetores=alimentadores_tf[i].numeroNos;//Cada setor é um nó do alimentador


    }
    

    


}