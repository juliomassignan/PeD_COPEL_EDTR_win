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
 * @brief Função de integracao da MRAN para converter o grafo utilizado trifasico no grafo utilizado na MRAN e para preencher os dados dos reguladores. 
 *
 * Essa função recebe como parâmetro o grafo do tipo TF_GRAFO, do fluxo de potência trifásico e realiza a conversão de suas informações para 
 * para preencher o grafo @p **grafoSDRParam do tipo GRAFO. A partir do grafo trifásico também é preenchida a estrutura @p **dadosReguladorSDRParam , do tipo DADOSREGULADOR. Ambas estruturas são alocadas nessa funcao
 * Além disso, ela retorna número de nós da rede @p numeroBarras , o número de transformadores em @p numeroTrafos , e o número de chaves em @p numeroChaves , que são ponteiros para long int.
 * Todas estas informacoes serao necessarias para utilizar as funcoes da MRAN e o fluxo de potência por varredura de RNP
 * @param grafo_tf grafo trifásico da rede com as informações que serão fornecidas nesta função
 * @param  numeroBarras_tf inteiro com o número de barras da rede, segundo o fluxo trifásico
 * @param ramos_tf vetor do tipo TF_DRAM com as informações dos ramos da rede, ulizada para preencher o grafoSDR e dadosRegulador
 * @param nramos_tf inteiro que conta o número de ramos da rede
 * @param grafoSDRParam ponteiro para um vetor do tipo GRAFO, retorna o grafo da rede a ser utilizado na MRAN 
 * @param dadosReguladorSDRParam ponteiro para um vetor do tipo DADOSREGULADOR com as informacoes dos reguladores a ser utilizado na MRAN
 * @param numeroBarras retorna o número de nos no grafo grafoSDRParam 
 * @param numeroTrafos retorna o número de trafos
 * @param numeroChaves retorna o numero de chaves na rede
 * @return void.
 * @see leituraBarras
 * @see leituraLinhas
 * @see leituraTrafos
 * @see leituraDados
 * @see geraGrafo
 * @note
 * @warning .
 */


void converteGrafo_TFtoSDR(TF_GRAFO *grafo_tf,long int numeroBarras_tf,TF_DRAM *ramos_tf,long int nramos_tf,GRAFO **grafoSDRParam,DADOSREGULADOR **dadosReguladorSDRParam,long int* numeroBarras, long int *numeroTrafos, long int *numeroChaves)
{
    
    int i,k,contador; // variaveis utilizadas como indexadores e contadores em loops
    int nreg=0; // variável que conta o numero de reguladores

    (*numeroChaves)=0; // variável que conta o numero de chaves e é inicializada com zero
    (*numeroTrafos)=0; // variavel que conta o numero de trafos e é inicializada com zero
    
    (*numeroBarras)=numeroBarras_tf; //que conta o numero de barras e é inicializada com o numero de barras trifasicas
    
    //realiza a alocação da estrutura em grafo para armazenar o SDR e verifica se o processo foi bem sucedido
    //a MRAM COMECA A INDEXACAO DO GRAFO EM 1
    if (((*grafoSDRParam) = (GRAFO *)malloc((numeroBarras_tf+1) * sizeof(GRAFO)))==NULL) 
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para o grafo do SDR !!!!");
        exit(1); 
    }

    // loop que percorre o grafo trifasico
    for (i=0;i<numeroBarras_tf;i++)
    {
        contador=i+1;//MARAN indexa o grafo a partir de 1
        (*grafoSDRParam)[contador].idNo=grafo_tf[i].idNo+1; // preenche o idNo no grafo trifasico

        //preenche tipo do nó
        if (grafo_tf[i].tipoNo== 0)(*grafoSDRParam)[contador].tipoNo= semCarga;
        else if (grafo_tf[i].tipoNo== 1)(*grafoSDRParam)[contador].tipoNo= comCarga;   
        else if (grafo_tf[i].tipoNo==2)(*grafoSDRParam)[contador].tipoNo= capacitor;   
        else if (grafo_tf[i].tipoNo==3)(*grafoSDRParam)[contador].tipoNo= raizAlimentador;

        
        //indica se há medidor no nó
        if(grafo_tf[i].nmedPQ>0)
        {
            (*grafoSDRParam)[contador].medicao=medidorClasseA;
        }
        else
        {
            (*grafoSDRParam)[contador].medicao=semMedidor;
        }    

        // preecnhe o indice que indica a área de medição
        (*grafoSDRParam)[contador].idAM=grafo_tf[i].idAM+1;
        // preenche o numero de nos adjacentes
        (*grafoSDRParam)[contador].numeroAdjacentes=grafo_tf[i].numeroAdjacentes;
        // preencher os alica os adjacentes (precisa preencher)
        (*grafoSDRParam)[contador].adjacentes=Malloc(NOADJACENTE,11);

        //loop para preencher a info dos adjacentes
        for (k=0;k<grafo_tf[i].numeroAdjacentes;k++)
        {
            
            (*grafoSDRParam)[contador].adjacentes[k].idNo=grafo_tf[i].adjacentes[k].idNo+1;// preenche o idNo

            //deixar vazio os outro também 
            // sprintf((*grafoSDRParam)[contador].adjacentes[k].idAresta,"%ld",grafo_tf[i].adjacentes[k].idram);
            
            // informa preenche a informacao se há medidor nos ramos
            if(grafo_tf[i].adjacentes[k].nmed>0)(*grafoSDRParam)[contador].adjacentes[k].medicao=medidorRamo;
            else (*grafoSDRParam)[contador].adjacentes[k].medicao=semMedidor;
 
           //preenche a informacao dos ramos tipo trecho 
            if(grafo_tf[i].adjacentes[k].tipo==ramal||grafo_tf[i].adjacentes[k].tipo==trafo)
            {
                (*grafoSDRParam)[contador].adjacentes[k].tipoAresta=trecho;
                (*grafoSDRParam)[contador].adjacentes[k].estadoChave=outrasArestas;
                (*grafoSDRParam)[contador].adjacentes[k].subTipoAresta=outrosSubTipo;
                if (grafo_tf[i].adjacentes[k].tipo==trafo)
                {
                  (*numeroTrafos)=(*numeroTrafos)+1;
                }
            }   
            //preenche a informacao dos ramos tipo regulador
            else if (grafo_tf[i].adjacentes[k].tipo==regulador)
            {
                (*grafoSDRParam)[contador].adjacentes[k].tipoAresta=reguladorTensao;
                (*grafoSDRParam)[contador].adjacentes[k].estadoChave=outrasArestas; 
                (*grafoSDRParam)[contador].adjacentes[k].subTipoAresta=outrosSubTipo;
            } 
            //preenche a informacao dos ramos tipo chave
            else if (grafo_tf[i].adjacentes[k].tipo==chave)
            {
                (*grafoSDRParam)[contador].adjacentes[k].tipoAresta=chaveManual; 
                (*numeroChaves)=(*numeroChaves)+1;
                if (grafo_tf[i].adjacentes[k].estado==fechado)
                {
                    (*grafoSDRParam)[contador].adjacentes[k].estadoChave=normalmenteFechada;
                }
                else if (grafo_tf[i].adjacentes[k].estado==aberto)
                {
                    (*grafoSDRParam)[contador].adjacentes[k].estadoChave=normalmenteAberta;
                }
            }
            
            // preenche o idAM
            (*grafoSDRParam)[contador].adjacentes[k].idAM=grafo_tf[i].adjacentes[k].idAM+1;//sera q tem q adicionar 1?
            
            // inicia todos os ramos como em operacao
            
            (*grafoSDRParam)[contador].adjacentes[k].condicao=emOperacao;



            //incia os valores da resistencia e reatancia que serão preenchidos em outro momento
            (*grafoSDRParam)[contador].adjacentes[k].resistencia=0;
            (*grafoSDRParam)[contador].adjacentes[k].reatancia=0;
            //preenche o valor da ampacidade
            (*grafoSDRParam)[contador].adjacentes[k].ampacidade=ramos_tf[grafo_tf[i].adjacentes[k].idram].ampacidade;

        }
        //inicia os valores da potencia no no
        (*grafoSDRParam)[contador].valorPQ.p=__real__ grafo_tf[i].S[0]+ grafo_tf[i].S[1]+grafo_tf[i].S[2];
        (*grafoSDRParam)[contador].valorPQ.q=__imag__ grafo_tf[i].S[0]+ grafo_tf[i].S[1]+grafo_tf[i].S[2];
        
        (*grafoSDRParam)[contador].priorizacoes.eusdGrupoA=0;
        (*grafoSDRParam)[contador].priorizacoes.eusdGrupoB=0; 
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


    //conta o numero de reguladores
    for (i=0;i<nramos_tf;i++)
    {
        if (ramos_tf[i].tipo==regulador)
        {
            nreg++;
        }
    }


    //realiza a alocação da estrutura em grafo para armazenar o SDR e verifica se o processo foi bem sucedido
    //caso não seja possível realizar a alocação o programa é encerrado


    //aloca a estrutura com os dados dos reguladores
    if (((*dadosReguladorSDRParam) = (DADOSREGULADOR *)malloc((nreg+1) * sizeof(DADOSREGULADOR)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para o grafo do SDR !!!!");
        exit(1); 
    }
    k=0;

    //preenche os dados do regulador de acordo com os dados do trifásico

    for (i=0;i<nramos_tf;i++)
    {
        if (ramos_tf[i].tipo==regulador)
        {
        // nao preencher
        //sprintf((*dadosReguladorSDRParam)[k].idRegulador,"%ld",i+1);
        (*dadosReguladorSDRParam)[k].tipoRegulador= comFluxoReverso; //o tipo de todos os reguladores é iniciado como com fluxo reverso
        (*dadosReguladorSDRParam)[k].ampacidade=ramos_tf[i].ampacidade; //preenche a amapacidade
        (*dadosReguladorSDRParam)[k].numeroTaps=ramos_tf[i].regulador.ntaps; // preenche o numero de taps
        k++;
        }
    }




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
 * @brief Função de integracao da MRAN, para preencher a estrutura dadosAlimentadorSDRParam do tipo DADOSALIMENTADOR que será utilizada nas funcoes da MRAN. 
 * 
 * Essa função recebe como parâmetro o vetor @p alimentadores_tf do tipo TF_ALIMENTADOR, com as informacoes dos alimentadores da rede, lidas para o calculo do fluxo de potencia trifasico
 * e o long int @p numerosAlimentadores_tf com o numero de alimentadores de acordo com o fluxo trifásico. Em posse destes dados, esta funcao aloca o espaço necessário para a estrutura @p dadosAlimentadorSDRParam
 * e a preenche, com as informacoes necessarias para utilizar as funcoes da rnp. 
 *
 * 
 * @param alimentadores_tf vetor do tipo TF_ALIMENTADOR com as informacoes sobre o alimentador trifasico
 * @param numero_alimentadores numero inteiro que conta o numero de alimentadores presentes no sistema
 * @param dadosAlimentadorSDRParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @return void
 * @see 
 * @note
 * @warning .
 */


void converteDadosAlimentadores_TFtoSDR(TF_ALIMENTADOR *alimentadores_tf,long int numerosAlimentadores_tf, DADOSALIMENTADOR **dadosAlimentadorSDRParam ){

    int contador, i=0; //variaveis utilizadas para contar e indexar


    extern long int numeroAlimentadores ; //variavel global com o numero de alimentadores
    numeroAlimentadores = numerosAlimentadores_tf; //recebe o numero de alimentadores do trifasico

    //aloca memoria para a estrutura DADOSALIMENTADOR, caso nao seja possivel encerra o programa
    if (((*dadosAlimentadorSDRParam)= (DADOSALIMENTADOR *)malloc( (numerosAlimentadores_tf+1) * sizeof(DADOSALIMENTADOR)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para alimentadores !!!!");
        exit(1); 
    }

    //loop que preenche os dados dos alimentadores de acordo com o trifásico
    for ( i = 0; i < numerosAlimentadores_tf; i++)
    {
        contador=i+1;
        (*dadosAlimentadorSDRParam)[contador].barraAlimentador = alimentadores_tf[i].noRaiz+1; // preenche a barraAlimentador com o noRaiz
        //sprintf((*dadosAlimentadorSDRParam)[contador].idAlimentador,"%ld",alimentadores_tf[i].idAlim); //passa o id do alimentador, é um int no código do estimador
        (*dadosAlimentadorSDRParam)[contador].idTrafo = alimentadores_tf[i].idRaiz;// preenche com idRaiz do no raiz
        (*dadosAlimentadorSDRParam)[contador].numeroSetores=0;// inicia o numero de setores com zero


    }
    

    


}

void preenche_powerflowresult_SDRalim (TF_PFSOLUTION* powerflow_result, long int idIndividuoInicialParam, long int numeroConfiguracoesParam, CONFIGURACAO **configuracao)
{
    int i,contador;

    for (i=0;i<(idIndividuoInicialParam+numeroConfiguracoesParam);i++)
    {
        (*configuracao)[i].objetivo.maiorCarregamentoCorrente=powerflow_result[i].maiorCarregamentoCorrente;
        (*configuracao)[i].objetivo.perdasResistivas=powerflow_result[i].perdasResistivas;
        (*configuracao)[i].objetivo.maiorCarregamentoTrafo=powerflow_result[i].maiorCarregamentoTrafo;
        (*configuracao)[i].objetivo.menorTensao=powerflow_result[i].menorTensao;
        (*configuracao)[i].objetivo.quedaMaxima=powerflow_result[i].quedaMaxima;

        
    }
    

}

//Função inicialização do vetor x para sistemas radiais através de uma varredura para compensar os taps fora da nominal (sem o arquivo Vinicial)
/**
 * @brief Função auxiliar para inicializar as tensões complexas nodais trifásicas de todos os alimentadores através de um processo de varredura direta
 * 
 * Essa função inicializa as tensões complexas nodais por fase (fase-neutro) de todas as barras de todos os alimentadores da rede elértrica. 
 * O valor de inicilização é obtido através de uma varredura direta partindo do nó raiz de cada alimentador para compensar possíveis valores de tap
 * fora do nominal (no caso de reguladores e transformadores). Recebe como parâmetros de entrada e saída o grafo da
 * rede elétrica @p grafo com as tensões complexas inicializadas e como parâmertros de entrada os alimentadores @p alimentadores e quantidades
 * totais de barras e alimentadores da rede elétrica @p numeroBarras e @p numeroAlimentadores ,respectivamente. 
 * A função retorna @c void.
 * 
 * @param *grafo ponteiro para o grafo da rede elétrica para inicilizar as tensões nodais
 * @param numeroBarras quatidade total de barras
 * @param *alimentadores ponteiro par os alimentadores da rede elétrica
 * @param numeroAlimentadores quantidade total de alimentadores
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
*/
void inicializaTensaoSDR_alimentador_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores,
BOOL todosAlimentadores, CONFIGURACAO configuracaoParam,RNPSETORES *matrizB,int idRNP)
{

    long int indice, indice1, noS, noR, noN, idSetorS, idSetorR, idBarra1, idBarra2, indice2, indice3;
    double IMod, IAng;
    long int noProf[200]; //armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
   __complex__ double iAcumulada;
    RNPSETOR rnpSetorSR;

    for (indice = 1; indice < configuracaoParam.rnp[idRNP].numeroNos; indice++) {
            noS = configuracaoParam.rnp[idRNP].nos[indice].idNo;
            noR = noProf[configuracaoParam.rnp[idRNP].nos[indice].profundidade - 1];
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);// igual da funcao anterior, como percorre
            for (indice1 = 0; indice1 < rnpSetorSR.numeroNos; indice1++) {
                noN = rnpSetorSR.nos[indice1].idNo; 

                //colocar igual na varredura original  
                // entender a partir da L 430          

            }

    }            

    //faz uma varredura incializando as tensoes em todas as varras da rede antes do fluxo de potencia. 
    //Talvez não precise ser modificada

}

void inicializaTensaoSDR_alimentadores_tf (TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores,
BOOL todosAlimentadores, CONFIGURACAO configuracaoParam,RNPSETORES *matrizB,int idRNP)
{
    // chama a anterior
}

void avaliaConfiguracaoSDR_tf(BOOL todosAlimentadores, CONFIGURACAO *configuracoesParam, int rnpA, int rnpP, long int idNovaConfiguracaoParam, DADOSTRAFO *dadosTrafoParam, int numeroTrafosParam,
        int numeroAlimentadoresParam, /*int *indiceRegulador, DADOSREGULADOR *dadosRegulador,*/ DADOSALIMENTADOR *dadosAlimentadorParam, /*double VFParam, */int idAntigaConfiguracaoParam, RNPSETORES *matrizB, /*MATRIZCOMPLEXA *ZParam,*/
        /*MATRIZMAXCORRENTE *maximoCorrenteParam,*/ long int numeroBarrasParam, BOOL copiarDadosEletricos,
        TF_GRAFO *grafo, TF_ALIMENTADOR *alimentadores, 
        TF_DRAM *ramos,double Sbase, long int **interfaceNiveis,long int numeroInterfaces, BOOL opt_flow)
{
    long int contador;
    double quedaMaxima, menorTensao, VF, perdasTotais;
    perdasTotais = 0;

    //if (idNovaConfiguracaoParam != idAntigaConfiguracaoParam) {
    // if (copiarDadosEletricos) {
            // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.iJusante = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.corrente = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.potencia = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.vBarra = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            // for (contador = 1; contador <= numeroBarrasParam; contador++) {
                // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.iJusante[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.iJusante[contador];
                // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.corrente[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.corrente[contador];
                // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.potencia[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.potencia[contador];
                // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.vBarra[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.vBarra[contador];
            // }
    // } else { //não mantém os dados elétricos individualmente, somente copia os ponteiros para os vetores com dados elétricos
            // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.iJusante = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.iJusante;
            // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.corrente = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.corrente;
            // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.potencia = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.potencia;
            // configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.vBarra = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.vBarra;
    // }
    //}
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
        //fluxoCargaTodosAlimentadores(numeroBarrasParam,dadosAlimentadorParam,dadosTrafoParam, configuracoesParam, idNovaConfiguracaoParam, matrizB, ZParam, indiceRegulador, dadosRegulador, maximoCorrenteParam);
    } else { //calcula o fluxo de carga somente para os alimentadores que foram alterados.
        // rnpA e P só usa quando roda pra 2 alimentadores, neste else
        // VF = 1000*dadosTrafoParam[dadosAlimentadorParam[rnpP+1].idTrafo].tensaoReal/sqrt(3);
        // fluxoCargaAlimentador(numeroBarrasParam, configuracoesParam, VF, rnpP, idNovaConfiguracaoParam, matrizB, ZParam, maximoCorrenteParam, indiceRegulador, dadosRegulador);
        // VF = 1000*dadosTrafoParam[dadosAlimentadorParam[rnpA+1].idTrafo].tensaoReal/sqrt(3);
        // fluxoCargaAlimentador(numeroBarrasParam, configuracoesParam, VF, rnpA, idNovaConfiguracaoParam, matrizB, ZParam, maximoCorrenteParam, indiceRegulador, dadosRegulador);
    }
    

    //revisar funcionalidade bool dados eletricos
    //fluxo de potencia

    //inicializa tensão sdr_tf todas as entradas mais o configuracoes param
    //fluxode potencia BFS_Multiplos todas as entradas mais configuracoes param
    // preenche_powerflowresult_SDRalim 

}


// Em consutrução
//Rotina para Cálculo de Fluxo de Potência em múltiplos alimentadores de um sistema utilizando RNP de distribuição na sequeência hierárquica dos níveis de tensão (Primeiro 13.8kV e depois 34.5kV)
/**
 * @brief Função principal para cálculo de fluxo de potência (Primeiro 13.8kV e depois 34.5kV).
 *
 * Essa função realiza a leitura da pasta com os dados da rede elétrica em arquivos separados. 
 * A pasta e subpasta são indicadas no arquivo "config.txt". A função assume que o nome do arquivo é padrão da e os arquivos 
 * lidos são: DBAR.csv; DSHNT.csv; DGD.csv; DLIN.csv; DTRF.csv; DREG.csv; DSWTC.csv; DSUBESTACAP.csv; e Viniciail.csv . 
 * Todos os arquivos são opcionais de leitura exceto o DBAR.csv. Além disso o arquivo possui como separação dos dados os marcadores especificados.
 * Realiza alocação de memória para armazenar os dados da rede elétrica.
 * Recebe como parâmetros de entrada e saída um ponteiro para ponteiro do tipo DBAR @p **barras que armazena dados das barras da rede elétrica e
 * um ponteiro para ponteiro do tipo DBAR @p **barras que armazena dados das barras da rede elétrica. Além disto recebe como 
 * parâmetros de entrada e saída inteiros que indicam: @p numeroBarras a quantidade total de barras na rede; @p numeroRamos a 
 * quantidade total de ramos na rede; e @p numeroAlimentadores a quantidade total de alimentadores na rede.
 * A função retorna @c char* indicando a pasta selecionada para os arquivos serem lidos.
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * char *folder = NULL;
 * DBAR *barra = NULL;
 * DRAM *ramo = NULL; 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * @endcode
 * 
 * @param barra parâmetro de entrada e saída que armazena os dados de barras da rede elétrica
 * @param ramo  parâmetro de entrada e saída que armazena os dados de ramos da rede elétrica
 * @param numeroBarras parâmetro de entrada e saída que armazena a quantidade total de barras da rede elétrica
 * @param numeroRamos parâmetro de entrada e saída que armazena a quantidade total de ramos da rede elétrica
 * @param numeroAlimentadores parâmetro de entrada e saída que armazena a quantidade total de alimentadores da rede elétrica
 * @return char* com string referente ao endereço da pasta com os arquivos da rede elétrica lidos
 * @see leituraDBAR
 * @see leituraDSHNT
 * @see leituraDGD
 * @see leituraDLIN
 * @see leituraDTRF
 * @see leituraDREG
 * @see leituraDSWTC
 * @see leituraDSUBESTACAO
 * @see leituraVinicial
 * @note
 * @warning .
 */

void fluxoPotencia_Niveis_BFS_Multiplos_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos,double Sbase,
 long int **interfaceNiveis,long int numeroInterfaces,/* long int numeroBarrasParam, */
        DADOSALIMENTADOR *dadosAlimentadorParam, DADOSTRAFO *dadosTrafoParam,
        CONFIGURACAO *configuracoesParam, long int indiceConfiguracao, RNPSETORES *matrizB/*,
        MATRIZCOMPLEXA *ZParam*/, int *indiceRegulador, DADOSREGULADOR *dadosRegulador, MATRIZMAXCORRENTE * maximoCorrenteParam){

    // recebe a rede completa  e chama a fluxoPotencia_BFS_Alimentador_tf
    //rede completa

}


//Cálculo de Fluxo de Potência para um Alimentador de Distribuição Baseado na Varredura Direta/Inversa Trifasica Utilizando RPN
/**
 * @brief Função auxiliar para o cálculo de fluxo de potência trifásico em um alimentador específico via método de Varredura Direta/Inversa
 * 
 * Essa função efetua
 * A função retorna @c TF_PFSOLUTION.
 * 
 * @param grafo grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param numeroBarras
 * @param alimentador
 * @param ramos
 * @param Sbase
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
*/
void fluxoPotencia_BFS_Alimentador_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR alimentador, TF_DRAM *ramos,double Sbase,
        /*int numeroBarrasParam,*/ CONFIGURACAO *configuracoesParam, 
        /*double VFParam,*/ int indiceRNP, int indiceConfiguracao, RNPSETORES *matrizB,
       /* MATRIZCOMPLEXA *ZParam,*/ MATRIZMAXCORRENTE *maximoCorrenteParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulado )
{
    //por alimentador utilizando a rpn interna

}


//Cálculo de Fluxo de Potência para um Alimentador de Distribuição Baseado na Varredura Direta/Inversa Trifasica
/**
 * @brief Função auxiliar para o cálculo de fluxo de potência trifásico em um alimentador específico via método de Varredura Direta/Inversa
 * 
 * Essa função efetua
 * A função retorna @c TF_PFSOLUTION.
 * 
 * @param grafo grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param numeroBarras
 * @param alimentador
 * @param ramos
 * @param Sbase
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
TF_PFSOLUTION fluxoPotencia_BFS_Alimentador_IteracaoUnica_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR alimentador, TF_DRAM *ramos,double Sbase){
    int it, nvar, k = 0,i, conv = 0;
    double *DV, nDV = 0, loading = 0, cargaAtivaTotal = 0 ;
    long int *RNP = NULL;
    BOOL control_action = 0;
    double fatorDesbalanco;

    TF_FILABARRAS *barraAtual = NULL, *barraProx = NULL;
    TF_NOADJACENTE *ramoAdj = NULL;

    TF_PFSOLUTION powerflow_result;

    //----------------------------------------------------------------------
    //RNP no formato de vetor
    //
    RNP = aloca_vetor_int(alimentador.numeroNos+1);
    barraAtual = &alimentador.rnp[0];
    while(barraAtual != NULL){
        RNP[k] = barraAtual->idNo;
        k++;
        barraAtual = barraAtual->prox;
    }
    nvar = alimentador.numeroNos*6;
    DV = aloca_vetor(nvar);
    powerflow_result.convergencia = 0;
    powerflow_result.maiorCarregamentoCorrente = 0;
    powerflow_result.perdasResistivas = 0;
    powerflow_result.maiorCarregamentoTrafo = 0;
    powerflow_result.carregamentoRede = 0;
    powerflow_result.menorTensao = 1.00;
    powerflow_result.quedaMaxima = 0;
    powerflow_result.iteracoes = 0;
    powerflow_result.desbalancoTensaoMaximo = 0;
    powerflow_result.desbalancoCorrenteMaximo = 0;
    powerflow_result.desbalancoCorrenteAlim = 0;
    powerflow_result.menorTensaoABC[0] = 1.0;
    powerflow_result.menorTensaoABC[1] = 1.0;
    powerflow_result.menorTensaoABC[2] = 1.0;
     
        //Backward Sweep
        for(k = alimentador.numeroNos-1; k >= 0; k--){
            backward(&grafo[RNP[k]], grafo);
        }

        //---------------------------------------------------------------------- 
        //Forward Sweep
        for(k = 0; k < alimentador.numeroNos; k++){
            control_action = forward(&grafo[RNP[k]], grafo);
            if (control_action) powerflow_result.tap_change_flag = true;
        }
        

    powerflow_result.convergencia = 1;
    powerflow_result.iteracoes = 1;
    ////----------------------------------------------------------------------
    // Calcula resultados condensados de fluxo de potência  para o alimentador
    if(powerflow_result.convergencia == 1){
        //---------------------------------------------------------------------- 
        //Calculos das grandezas de interesse nos ramos - via varredura backward
        for(k = alimentador.numeroNos-1; k > 0; k--){
            powerflow_result.perdasResistivas += calculaPerdas(&grafo[RNP[k]], grafo);
            loading = calculaCarregamento(&grafo[RNP[k]], grafo, 1000*Sbase);
            cargaAtivaTotal += creal(cabs(grafo[RNP[k]].S[0]) + cabs(grafo[RNP[k]].S[1]) + cabs(grafo[RNP[k]].S[2]));

            if (loading > powerflow_result.maiorCarregamentoCorrente)
                powerflow_result.maiorCarregamentoCorrente = loading;
            
//            Menor tensão por fase
            if (cabs(grafo[RNP[k]].V[0]) < powerflow_result.menorTensaoABC[0])
                powerflow_result.menorTensaoABC[0] = cabs(grafo[RNP[k]].V[0]);
            if (cabs(grafo[RNP[k]].V[1]) < powerflow_result.menorTensaoABC[1])
                powerflow_result.menorTensaoABC[1] = cabs(grafo[RNP[k]].V[1]);
            if (cabs(grafo[RNP[k]].V[2]) < powerflow_result.menorTensaoABC[2])
                powerflow_result.menorTensaoABC[2] = cabs(grafo[RNP[k]].V[2]);
            
//            Menor tensão entre as três fases
            if (cabs(grafo[RNP[k]].V[0]) < powerflow_result.menorTensao){
                powerflow_result.menorTensao = cabs(grafo[RNP[k]].V[0]);
                powerflow_result.quedaMaxima = (cabs(grafo[alimentador.noRaiz].V[0]) - powerflow_result.menorTensao)/cabs(grafo[alimentador.noRaiz].V[0]) * 100;
            }
            if (cabs(grafo[RNP[k]].V[1]) < powerflow_result.menorTensao){
                powerflow_result.menorTensao = cabs(grafo[RNP[k]].V[1]);
                powerflow_result.quedaMaxima = (cabs(grafo[alimentador.noRaiz].V[1]) - powerflow_result.menorTensao)/cabs(grafo[alimentador.noRaiz].V[1]) * 100;
            }
            if (cabs(grafo[RNP[k]].V[2]) < powerflow_result.menorTensao){
                powerflow_result.menorTensao = cabs(grafo[RNP[k]].V[2]);
                powerflow_result.quedaMaxima = (cabs(grafo[alimentador.noRaiz].V[2]) - powerflow_result.menorTensao)/cabs(grafo[alimentador.noRaiz].V[2]) * 100;
            }
            
            if (grafo[RNP[k]].fases == ABC){
                fatorDesbalanco = desbalancoFasorialSeq(grafo[RNP[k]].V);
                if (fatorDesbalanco > powerflow_result.desbalancoTensaoMaximo)
                    powerflow_result.desbalancoTensaoMaximo = fatorDesbalanco;
            }
            
            if (grafo[RNP[k]].fases == ABC){
                for (int j =0; j < grafo[RNP[k]].numeroAdjacentes; j++){
                    if (grafo[grafo[RNP[k]].adjacentes[j].idNo].profundidade < grafo[RNP[k]].profundidade){
                        
                        if (( cabs(grafo[RNP[k]].adjacentes[j].Cur[0]) != 0) && (cabs(grafo[RNP[k]].adjacentes[j].Cur[1]) !=0) && (cabs(grafo[RNP[k]].adjacentes[j].Cur[2]) != 0 ))
                            fatorDesbalanco = desbalancoFasorialSeq(grafo[RNP[k]].adjacentes[j].Cur);
                        else
                            fatorDesbalanco = 0;
                            
                        if (fatorDesbalanco > powerflow_result.desbalancoCorrenteMaximo)
                            powerflow_result.desbalancoCorrenteMaximo = fatorDesbalanco;
                        }
                    }
            }
            
        }
        powerflow_result.carregamentoRede = powerflow_result.perdasResistivas + cargaAtivaTotal;
        
        powerflow_result.carregamentoRedeABC[0] = creal(grafo[RNP[0]].V[0]*conj(grafo[RNP[0]].adjacentes[0].Cur[0]));
        powerflow_result.carregamentoRedeABC[1] = creal(grafo[RNP[0]].V[1]*conj(grafo[RNP[0]].adjacentes[0].Cur[1]));
        powerflow_result.carregamentoRedeABC[2] = creal(grafo[RNP[0]].V[2]*conj(grafo[RNP[0]].adjacentes[0].Cur[2]));
        
        if( cabs(grafo[RNP[0]].adjacentes[0].Cur[0] + grafo[RNP[0]].adjacentes[0].Cur[1] + grafo[RNP[0]].adjacentes[0].Cur[2]) != 0 )
            powerflow_result.desbalancoCorrenteAlim = desbalancoFasorialSeq(grafo[RNP[0]].adjacentes[0].Cur);
        else
            powerflow_result.desbalancoCorrenteAlim = 0;
    }
    
    free(DV); free(RNP); free(barraAtual); free(barraProx); free(ramoAdj);
    return(powerflow_result);
}