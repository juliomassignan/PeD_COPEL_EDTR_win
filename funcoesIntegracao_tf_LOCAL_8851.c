/**
 * @file funcoesIntegracao.c
 * @author Julio Augusto Druzina Massignan e Vitor Henrique Pereira de Melo
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
#include "data_structures_modcarga_tf.h"
#include "funcoesTopologia_tf.h"
#include "funcoesLeitura_tf.h"
#include "funcoesFluxoVarredura_tf.h"
#include "funcoesCalculoEletrico_tf.h"
#include "funcoesMatematicas_tf.h"
#include "data_structures.h"
#include "funcoesIntegracao_tf.h"


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
        (*dadosReguladorSDRParam)[k].tipoRegulador= comFluxoReverso; //o tipo de todos os reguladores é iniciado como com fluxo reverso
        (*dadosReguladorSDRParam)[k].ampacidade=ramos_tf[i].ampacidade; //preenche a amapacidade
        (*dadosReguladorSDRParam)[k].numeroTaps=ramos_tf[i].regulador.ntaps; // preenche o numero de taps
        k++;
        }
    }




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


void converteDadosAlimentadores_TFtoSDR(TF_GRAFO *grafo_tf,long int numeroBarras,long int numerosAlimentadores_tf, DADOSALIMENTADOR **dadosAlimentadorSDRParam )
{

    int contador, i=0; //variaveis utilizadas para contar e indexar
    
    int k=0;
    extern long int numeroAlimentadores; //variavel global com o numero de alimentadores
    // numerosAlimentadores_tf[0]=0; //recebe o numero de alimentadores do trifasico
    int idraiz[numerosAlimentadores_tf];
    
    for(i=0; i<numeroBarras; i++){ 
        if(grafo_tf[i].tipo == 2){
            idraiz[k]=grafo_tf[i].idNo;
            // numerosAlimentadores_tf[0]++;
            k++;   
        }
    }

    if (((*dadosAlimentadorSDRParam)= (DADOSALIMENTADOR *)malloc( (numerosAlimentadores_tf+1) * sizeof(DADOSALIMENTADOR)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para alimentadores !!!!");
        exit(1); 
    }
    numeroAlimentadores=numerosAlimentadores_tf;



    //aloca memoria para a estrutura DADOSALIMENTADOR, caso nao seja possivel encerra o programa

    //loop que preenche os dados dos alimentadores de acordo com o trifásico

    for ( i = 0; i < numerosAlimentadores_tf; i++)
    {
        contador=i+1;
        (*dadosAlimentadorSDRParam)[contador].barraAlimentador = idraiz[i]+1; // preenche a barraAlimentador com o noRaiz
        //sprintf((*dadosAlimentadorSDRParam)[contador].idAlimentador,"%ld",alimentadores_tf[i].idAlim); //passa o id do alimentador, é um int no código do estimador
        (*dadosAlimentadorSDRParam)[contador].idTrafo = idraiz[i]+1;// preenche com idRaiz do no raiz
        (*dadosAlimentadorSDRParam)[contador].numeroSetores=0;// inicia o numero de setores com zero
    }


}


void converteDadosAlimentadores_TFtoSDR_alt(TF_ALIMENTADOR *alimentadores_tf,long int numerosAlimentadores_tf, DADOSALIMENTADOR **dadosAlimentadorSDRParam){

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


void compatibiliza_chaveSetoresFicticia_tf(TF_GRAFO** grafo_tf,TF_DBAR **barras, long int *numeroBarras, TF_DRAM **ramos, long int *numeroRamos,GRAFO* grafoSDR, long int numeroNos)
{

    int aux=numeroBarras[0];
    int de;
    int para;
    int idAdj;

    for(int i=aux;i<numeroNos;i++)
    {
        idAdj=grafoSDR[i+1].adjacentes[0].idNo-1;
        includeDBAR(barras,numeroBarras,(*grafo_tf)[idAdj].Vbase);
        includeNografoTF(grafo_tf,(*barras),numeroBarras[0]-1);
        for (size_t j = 0; j < grafoSDR[i+1].numeroAdjacentes; j++)
        {
           de=grafoSDR[i+1].idNo-1;
           para=grafoSDR[i+1].adjacentes[j].idNo-1; 
           includeDSWTC(ramos,numeroRamos,de,para,grafoSDR[i+1].adjacentes[j].estadoChave);
           includeAdjGrafoTF((*grafo_tf),(*barras),i,(*ramos),numeroRamos[0]-1);
        }
        
        

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
BOOL todosAlimentadores, CONFIGURACAO* configuracaoParam,RNPSETORES *matrizB,int indiceRNP,int indiceConfiguracao,int root)
{

    long int indice, indice1, noS, noR, noN,no_prev, idSetorS, idSetorR, idBarra1, idBarra2, indice2, indice3;
    long int iniAlim;
    int k;
    double IMod, IAng;
    extern BOOL control_REG_OPT;
    long int noProf[1000]; //armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
   __complex__ double iAcumulada;
    RNPSETOR rnpSetorSR;
    __complex__ double Valim[3];
    __complex__ double VTran[3];



    //inicializa o indice que percorre a rpn maior

    indice=0;

    //pega o nó do inicio do alimentador

    iniAlim=configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
    
    

    noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = iniAlim;



    // percorre aquela rnp em questão a partir do no 1
    for (indice = 1; indice < configuracaoParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
            // varre as rnps de setores
            
            //pega o no da profundidade imediatamente anterior
            noR = noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
            //pega o no atual
            noS = configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo; // pega o id do proximo no
            
            //pega a tensão daquele no inicial       
            
            //pega o id do nó na profundidade anterior
            
            // pega a RNP do setor interna, entre aquele nó e o nó da profundidade imediatamente anterior   
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
            
            //percorre a rpn interna (do setor)
            for (indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {

                //percorre todos os nos da RNP daquele setor
                noN = rnpSetorSR.nos[indice1].idNo;                
                grafo[noN-1].V[0]=grafo[root-1].V[0];
                grafo[noN-1].V[1]=grafo[root-1].V[1];
                grafo[noN-1].V[2]=grafo[root-1].V[2];
                atualizaInjecoes(&grafo[noN-1]);

       
            }
        //atualiza a profundidade do nó com seu indice       
        noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo; // preenche a próxima profundidade
    }            

    //faz uma varredura incializando as tensoes em todas as varras da rede antes do fluxo de potencia. 
    //Talvez não precise ser modificada

}

void inicializaTensaoSDR_alimentadores_tf (TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores,
BOOL todosAlimentadores, CONFIGURACAO* configuracaoParam,RNPSETORES *matrizB,int indiceConfiguracao)
{
    int indiceRNP;
    int iniAlim,root,noS,noR;
    long int BarraProf[1000],noProf[1000];
    RNPSETOR rnpSetorSR;
     


    //Percorre todas as RNPs do nível mais alto da configuracao[indiceConfiguracao] 



    for (indiceRNP = 0; indiceRNP < configuracaoParam[indiceConfiguracao].numeroRNP; indiceRNP++)
    {
        //inicia a tensao do primeiro no do alimentador
        
            
        iniAlim=configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[0].idNo;    
        noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[0].profundidade] = iniAlim;
        noS = configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[1].idNo;
        noR = noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[1].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
        //No raiz do alimentador
        root=rnpSetorSR.nos[0].idNo;   
        
        iniAlim=configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[0].idNo;//compatibilizacao idex sdr itf
        grafo[root-1].V[0]=grafo[root-1].barra->Vinicial[0];// conferir com o julio se é isso mesmo 
        grafo[root-1].V[1]=grafo[root-1].barra->Vinicial[1];
        grafo[root-1].V[2]=grafo[root-1].barra->Vinicial[2];

        inicializaTensaoSDR_alimentador_tf(grafo,numeroBarras,alimentadores,numeroAlimentadores,todosAlimentadores,configuracaoParam,matrizB,indiceRNP,indiceConfiguracao,root);

    }

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


void compatibiliza_profundidadegrafo_tf(TF_GRAFO *grafo_tf,CONFIGURACAO *configuracoesParam,int indiceConfiguracao,RNPSETORES *matrizB,long int indiceRNP, int *nvar)
{

    int noRaiz,noR,noS,noN;
    int prfundidade;
    int idadj;
    int raiz_inter;
    int indiceRNPsetor;
    int iniAlim;
    int n_var=0;
    long int noProf[1000];
    long int BarraProf[1000];
    RNPSETOR rnpSetorSR;
    int indice,indice1;
    
    iniAlim=configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].idNo;    
    noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].profundidade] = iniAlim;
    noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[1].idNo;
    noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[1].profundidade - 1];

    for (indice = 1; indice < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
        noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
        noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
        BarraProf[rnpSetorSR.nos[0].profundidade] = rnpSetorSR.nos[0].idNo;
        for (indice1 = 0; indice1 < rnpSetorSR.numeroNos; indice1++) {
            // int noM = noProf[rnpSetorSR.nos[indice1].profundidade - 1];
            noN = rnpSetorSR.nos[indice1].idNo;
            
            grafo_tf[noN-1].profundidade= 0; 
            
            BarraProf[rnpSetorSR.nos[indice1].profundidade] = rnpSetorSR.nos[indice1].idNo;
        }
        //armazena o nó setor na sua profundidade
        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
    }

    iniAlim=configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].idNo;    
    noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].profundidade] = iniAlim;

    for (indice = 1; indice < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
        noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
        noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
        BarraProf[rnpSetorSR.nos[0].profundidade] = rnpSetorSR.nos[0].idNo;
        for (indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {
            // int noM = noProf[rnpSetorSR.nos[indice1].profundidade - 1];
            noN = rnpSetorSR.nos[indice1].idNo;
            
            grafo_tf[noN-1].profundidade = rnpSetorSR.nos[indice1].profundidade+grafo_tf[rnpSetorSR.nos[0].idNo-1].profundidade; 
            n_var=n_var+6;
            BarraProf[rnpSetorSR.nos[indice1].profundidade] = rnpSetorSR.nos[indice1].idNo;
        }
        //armazena o nó setor na sua profundidade
        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
    }


    (*nvar)=n_var+6;



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
TF_PFSOLUTION fluxoPotencia_BFS_Alimentador_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR alimentador, TF_DRAM *ramos,double Sbase,
        /*int numeroBarrasParam,*/ CONFIGURACAO *configuracoesParam, 
        /*double VFParam,*/ DADOSALIMENTADOR *dadosAlimentadorParam ,int indiceConfiguracao, RNPSETORES *matrizB/*,
        MATRIZCOMPLEXA *ZParam,*/ /* MATRIZMAXCORRENTE *maximoCorrenteParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulado*/ ){
     int it, nvar, k = 0,i, conv = 0;
    double *DV=NULL, nDV = 0, loading = 0, cargaAtivaTotal = 0 ;
    long int *RNP = NULL;
    BOOL control_action = 0;
    double fatorDesbalanco;
    double tempoBFS;
    int indiceRNP;
    int iniAlim;
    int indiceRNPsetores;
    int indice1, indice;
    long int noS, noR, noN;
    int nvar2;

    RNPSETOR rnpSetorSR;

    long int noProf[1000];
    long int root;

    // FILE *teste_barrasin=fopen("testesbarras.csv","w");



    TF_FILABARRAS *barraAtual = NULL, *barraProx = NULL;
    TF_NOADJACENTE *ramoAdj = NULL;

    TF_PFSOLUTION powerflow_result;
    // recebe um alimentador por vez

    // Vai rodar o fluxo na RNP
    //----------------------------------------------------------------------
    //RNP no formato de vetor
    //

    // Determina o inicio do alimentador
    indiceRNP=alimentador.idAlim;
    long int BarraProf[1000];
    iniAlim=configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].idNo;    
    noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].profundidade] = iniAlim;
    noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[1].idNo;
    noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[1].profundidade - 1];
    rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
    //No raiz do alimentador
    root=rnpSetorSR.nos[0].idNo;        


    // compatbiliza a profundidade do grafo e calcula o numero de variáveis
    compatibiliza_profundidadegrafo_tf(grafo,configuracoesParam,indiceConfiguracao,matrizB,indiceRNP,&nvar);

    
    //incia todos os resultados com zero
    DV = aloca_vetor(nvar);
    powerflow_result.tap_change_flag = false;
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
    powerflow_result.correnteNeutroSE = 0;
    powerflow_result.menorTensaoABC[0] = 1.0;
    powerflow_result.menorTensaoABC[1] = 1.0;
    powerflow_result.menorTensaoABC[2] = 1.0;
     
    //Correntes já são inicializadas na função de inicalização de Tensões
           
    ////----------------------------------------------------------------------
    // Fluxo de  Potência por Varredura Direta/Inversa via Soma de Correntes
    for (it = 0;it < MAXIT; it ++){
        //---------------------------------------------------------------------- 
        //Tensões Anteriores para Conevergência
        
        // fazer varrendo rnp setores

        k=0;

        indiceRNPsetores=0;

        iniAlim=configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;

        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade] = iniAlim;


        for (indiceRNPsetores = 1; indiceRNPsetores < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indiceRNPsetores++)
        {

            noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade - 1];
            //pega o no atual
            noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);

            for (size_t ind_inter = 1; ind_inter < rnpSetorSR.numeroNos; ind_inter++)
            {
                noN = rnpSetorSR.nos[ind_inter].idNo;
                for(i=0;i<3;i++){
                //modulo e angulo de todas as tensoes da rede
                DV[k] =  cabs(grafo[noN-1].V[i]);
                k++;
                DV[k] =  carg(grafo[noN-1].V[i]);
                k++;                      
                }
            }
            noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;
        }



       
        //---------------------------------------------------------------------- 
        //Backward Sweep 
        for (int indice = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos - 1; indice > 0; indice--) {
            int indice2 = indice - 1;
            //busca pelo nó raiz
            while (indice2 >= 0 && configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice2].profundidade != (configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1)) indice2--;                
            int idSetorS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
            int idSetorR = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice2].idNo;
            rnpSetorSR = buscaRNPSetor(matrizB, idSetorS, idSetorR);
            for (indice2 = rnpSetorSR.numeroNos - 1; indice2 > 0; indice2--) {
                int indice3 = indice2 - 1;
                while (indice3 >= 0 && rnpSetorSR.nos[indice3].profundidade != (rnpSetorSR.nos[indice2].profundidade - 1)) {
                    indice3--;
                }
                int idBarra1 = rnpSetorSR.nos[indice3].idNo;
                int idBarra2 = rnpSetorSR.nos[indice2].idNo;
                // chama a etapa backward do BF
                backward(&grafo[idBarra2-1], grafo);
   
            }

        }
        


        for (int indice = 1; indice < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
            noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
            noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
            BarraProf[rnpSetorSR.nos[0].profundidade] = rnpSetorSR.nos[0].idNo;

            for (int indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {
                
                noN = rnpSetorSR.nos[indice1].idNo;
                
                control_action = forward(&grafo[noN-1], grafo);
                if (control_action) powerflow_result.tap_change_flag = true;
                //armazena o nó barra na sua profundidade
                BarraProf[rnpSetorSR.nos[indice1].profundidade] = rnpSetorSR.nos[indice1].idNo;
            }
            //armazena o nó setor na sua profundidade
            noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
        }

        
        k=0;

        indiceRNPsetores=0;

        iniAlim=configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;

        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade] = iniAlim;

        
        
        for (indiceRNPsetores = 1; indiceRNPsetores < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indiceRNPsetores++)
        {

            noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade - 1];
            //pega o no atual
            noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);

            for (size_t ind_inter = 1; ind_inter < rnpSetorSR.numeroNos; ind_inter++)
            {

                noN = rnpSetorSR.nos[ind_inter].idNo;
                
                for(i=0;i<3;i++){
                //modulo e angulo de todas as tensoes da rede
                DV[k] = DV[k] - cabs(grafo[noN-1].V[i]);
                k++;
                DV[k] = DV[k] - carg(grafo[noN-1].V[i]);
                k++;                      
                }
            }
            noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;


        }

        
        nDV = norma_inf(DV,nvar);
        if ((fabs(nDV) < tolerance) && !control_action){
            conv = 1;
            powerflow_result.convergencia = 1;
            powerflow_result.iteracoes = it + 1;
            break;
        }
    }

            
    //calculo de resultados
    
    ////----------------------------------------------------------------------
    // Calcula resultados condensados de fluxo de potência  para o alimentador
    if(powerflow_result.convergencia == 1){

        
        k=0;

        indiceRNPsetores=0;

        iniAlim=configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;

        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade] = iniAlim;


        for (indiceRNPsetores = 1; indiceRNPsetores < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indiceRNPsetores++)
        {

            noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade - 1];
            //pega o no atual
            noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);

            for (size_t ind_inter = 1; ind_inter < rnpSetorSR.numeroNos; ind_inter++)
            {
            noN = rnpSetorSR.nos[ind_inter].idNo;

            powerflow_result.perdasResistivas += calculaPerdas(&grafo[noN-1], grafo);
            loading = calculaCarregamento(&grafo[noN-1], grafo, 1000*Sbase);
            cargaAtivaTotal += creal(cabs(grafo[noN-1].S[0]) + cabs(grafo[noN-1].S[1]) + cabs(grafo[noN-1].S[2]));

            if (loading > powerflow_result.maiorCarregamentoCorrente)
                powerflow_result.maiorCarregamentoCorrente = loading;
            
//            Menor tensão por fase
            if (cabs(grafo[noN-1].V[0]) < powerflow_result.menorTensaoABC[0])
                powerflow_result.menorTensaoABC[0] = cabs(grafo[noN-1].V[0]);
            if (cabs(grafo[noN-1].V[1]) < powerflow_result.menorTensaoABC[1])
                powerflow_result.menorTensaoABC[1] = cabs(grafo[noN-1].V[1]);
            if (cabs(grafo[noN-1].V[2]) < powerflow_result.menorTensaoABC[2])
                powerflow_result.menorTensaoABC[2] = cabs(grafo[noN-1].V[2]);
            
//            Menor tensão entre as três fases
            if (cabs(grafo[noN-1].V[0]) < powerflow_result.menorTensao){
                powerflow_result.menorTensao = cabs(grafo[noN-1].V[0]);
                powerflow_result.quedaMaxima = (cabs(grafo[root-1].V[0]) - powerflow_result.menorTensao)/cabs(grafo[root-1].V[0]) * 100;
            }
            if (cabs(grafo[noN-1].V[1]) < powerflow_result.menorTensao){
                powerflow_result.menorTensao = cabs(grafo[noN-1].V[1]);
                powerflow_result.quedaMaxima = (cabs(grafo[root-1].V[1]) - powerflow_result.menorTensao)/cabs(grafo[root-1].V[1]) * 100;
            }
            if (cabs(grafo[noN-1].V[2]) < powerflow_result.menorTensao){
                powerflow_result.menorTensao = cabs(grafo[noN-1].V[2]);
                powerflow_result.quedaMaxima = (cabs(grafo[root-1].V[2]) - powerflow_result.menorTensao)/cabs(grafo[root-1].V[2]) * 100;
            }
            
            if (grafo[noN-1].fases == ABC){
                fatorDesbalanco = desbalancoFasorialSeq(grafo[noN-1].V);
                if (fatorDesbalanco > powerflow_result.desbalancoTensaoMaximo)
                    powerflow_result.desbalancoTensaoMaximo = fatorDesbalanco;
            }
            
            if (grafo[noN-1].fases == ABC){
                for (int j =0; j < grafo[noN-1].numeroAdjacentes; j++){
                    if (grafo[grafo[noN-1].adjacentes[j].idNo].profundidade < grafo[noN-1].profundidade){
                        
                        if (( cabs(grafo[noN-1].adjacentes[j].Cur[0]) != 0) && (cabs(grafo[noN-1].adjacentes[j].Cur[1]) !=0) && (cabs(grafo[noN-1].adjacentes[j].Cur[2]) != 0 ))
                            fatorDesbalanco = desbalancoFasorialSeq(grafo[noN-1].adjacentes[j].Cur);
                        else
                            fatorDesbalanco = 0;
                            
                        if (fatorDesbalanco > powerflow_result.desbalancoCorrenteMaximo)
                            powerflow_result.desbalancoCorrenteMaximo = fatorDesbalanco;
                        }
                    }
            }

            }
            noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;
        }



        powerflow_result.carregamentoRede = powerflow_result.perdasResistivas + cargaAtivaTotal;
        powerflow_result.correnteNeutroSE = cabs(grafo[root-1].adjacentes[0].Cur[0] + grafo[root-1].adjacentes[0].Cur[1] + grafo[root-1].adjacentes[0].Cur[2]);
        
        powerflow_result.carregamentoRedeABC[0] = creal(grafo[root-1].V[0]*conj(grafo[root-1].adjacentes[0].Cur[0]));
        powerflow_result.carregamentoRedeABC[1] = creal(grafo[root-1].V[1]*conj(grafo[root-1].adjacentes[0].Cur[1]));
        powerflow_result.carregamentoRedeABC[2] = creal(grafo[root-1].V[2]*conj(grafo[root-1].adjacentes[0].Cur[2]));
        
        if( cabs(grafo[root-1].adjacentes[0].Cur[0] + grafo[root-1].adjacentes[0].Cur[1] + grafo[root-1].adjacentes[0].Cur[2]) != 0 )
            powerflow_result.desbalancoCorrenteAlim = desbalancoFasorialSeq(grafo[root-1].adjacentes[0].Cur);
        else
            powerflow_result.desbalancoCorrenteAlim = 0;
    }
    
    free(DV); 
    free(RNP);
    free(barraProx);
    free(barraAtual);
    free(ramoAdj);
    return(powerflow_result);
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

void fluxoPotencia_Niveis_BFS_Multiplos_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos,double Sbase,
 long int **interfaceNiveis,long int numeroInterfaces, BOOL opt_flow,/* long int numeroBarrasParam, */
        DADOSALIMENTADOR *dadosAlimentadorParam, /*DADOSTRAFO *dadosTrafoParam,*/GRAFO *grafoSDRParam,
        CONFIGURACAO *configuracoesParam, long int indiceConfiguracao, RNPSETORES *matrizB/*,
        MATRIZCOMPLEXA *ZParam*//*, int *indiceRegulador, DADOSREGULADOR *dadosRegulador, MATRIZMAXCORRENTE * maximoCorrenteParam*/){

    // recebe a rede completa  e chama a fluxoPotencia_BFS_Alimentador_tf
    //rede completa
    long int nmed,nvar,nmedTotal;
    int i,j, idAlim, it, n_threads;
    TF_FILABARRAS *barraAtual = NULL;
    TF_GRAFO *no  = NULL;
    extern BOOL iteration_multiple_level_OPT;
    BOOL flag_niveis[numeroAlimentadores];
    int level_iteration =0;
    
    BOOL tap_modified = true;      // variable that checks if there is a change on tap position on the upper level of power flow
    BOOL runPF_alimentador[numeroAlimentadores];
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++) runPF_alimentador[idAlim] = true;
    
    TF_PFSOLUTION powerflow_result[numeroAlimentadores];
    TF_PFSOLUTION powerflow_result_rede;
    
    powerflow_result_rede.convergencia = 0;
    powerflow_result_rede.maiorCarregamentoCorrente = 0;
    powerflow_result_rede.perdasResistivas = 0;
    powerflow_result_rede.maiorCarregamentoTrafo = 0;
    powerflow_result_rede.carregamentoRede = 0;
    powerflow_result_rede.menorTensao = 100000;
    powerflow_result_rede.quedaMaxima = 0;
    powerflow_result_rede.iteracoes = 0;
    powerflow_result_rede.carregamentoRedeABC[0] = 0;
    powerflow_result_rede.carregamentoRedeABC[1] = 0;
    powerflow_result_rede.carregamentoRedeABC[2] = 0;
    powerflow_result_rede.menorTensaoABC[0] = 0;
    powerflow_result_rede.menorTensaoABC[1] = 0;
    powerflow_result_rede.menorTensaoABC[2] = 0;
    
    
    
    
    FILE *arquivo;
    arquivo = fopen("tempos_alimentadores.txt","w+");
    double tempoAUX;
    
    //Calculo de fluxo de potência para todos os alimentadores individualmente
    double tol = 0.000001;
    clock_t t1 = clock(); 
        
    double tIni1 = omp_get_wtime(); 
    double tIni;
    double tEnd;

    
    
    // varre todos os alimentadores e seta a flag_niveis false para 34.5 e true para 13.8  
    // Prepara vetor booleano com flag de níveis
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
        flag_niveis[idAlim] = false;
        
        // Todos alimentadores no nível de tensão = 13.8
        if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 <= 13.8) flag_niveis[idAlim] = true;
                    
        // Para Estações de Chave
        for (int idInter = 0; idInter < numeroInterfaces; idInter++){
            // Se a barra a jusante da interface corresponde a raiz do alimnetador então ele entra no nivel jusante
            if (interfaceNiveis[idInter][0] == alimentadores[idAlim].noRaiz){ //Para mútliplos níveis, até o momento somente dois
                flag_niveis[idAlim] = true;
            }
        }
    }
    
    
    
    // runs multiple times if there is a tap change on the upper level (34.5)
    while (tap_modified){
    tap_modified = false;
    
    printf("\nIteracao %d do calculo Multinivel...\n",level_iteration  );
        
    // Cálculo do Fluxo de Potência no nível de 13.8 kV
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){//varre a lista de alimentadores
        if (runPF_alimentador[idAlim]){//está hard coded como true para todos nesta versao
            // run power flow for feeder if true, selected on list
            tIni = omp_get_wtime();
            
            // Run lowest level - Multiplos Niveis - Primeiro os de 13.8 kV e alimentadores indicados como jusante
            if (flag_niveis[idAlim]){// este if calcula primeiro os fluxos para os alimentadores de 13.8 // verificar o alimentador pelo no raiz
                if (opt_flow) powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador_tf(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase,configuracoesParam,dadosAlimentadorParam,0,matrizB);
                // if (opt_flow) powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                
                else powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador_IteracaoUnica(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                
                runPF_alimentador[idAlim] = false;     // finished calculating power flow on the feeder
                
                fprintf(arquivo,"\n %d \t13.8\t%ld\t%d\t%.12lf\t%lf",i,alimentadores[idAlim].numeroNos,powerflow_result[idAlim].iteracoes,tempoAUX, powerflow_result[idAlim].menorTensao );
            }
            tEnd = omp_get_wtime(); 
            tempoAUX = tEnd - tIni;
        }
    }//Fim do loop dos alimentadores  
    // terminou de calcular para todos os alimentadores de 13.8

    //Atualiza as Interfaces das SEs de 34.5 kV com a carga calculada em 13.8 kV

    // primeiro zera as injeções nas interfaces
    for (int idInter = 0; idInter < numeroInterfaces; idInter++){
        if (interfaceNiveis[idInter][2] == 0){ //Para mútliplos níveis, até o momento somente dois
            int bar_13_8 = interfaceNiveis[idInter][0];
            int bar_34_5 = interfaceNiveis[idInter][1];
            grafo[bar_34_5].barra->nloads = 0;

            grafo[bar_34_5].Cur[0] = 0;
            grafo[bar_34_5].Cur[1] = 0;
            grafo[bar_34_5].Cur[2] = 0;

            grafo[bar_34_5].S[0]=0;
            grafo[bar_34_5].S[1]=0;
            grafo[bar_34_5].S[2]=0;
        }
    }
    // depois ele reflete a carga do 13.8kv para o 34.5 kv
    for (int idInter = 0; idInter < numeroInterfaces; idInter++){
        if (interfaceNiveis[idInter][2] == 0){ //Para mútliplos níveis, até o momento somente dois (não entendi isso)
            int bar_13_8 = interfaceNiveis[idInter][0];// grava as barras da interface
            int bar_34_5 = interfaceNiveis[idInter][1];// grava as barras da interface

            grafo[bar_34_5].barra->nloads++;
            grafo[bar_34_5].barra->loads[0].lig = YN;


            //Reflete injeções de potência do 13.8 no 34.5 (Revisar e incluir um trfo YnD e Regulador de tensão)
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Pnom[0] = creal(grafo[bar_13_8].V[0]*conj(grafo[bar_13_8].adjacentes[0].Cur[0]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Pnom[1] = creal(grafo[bar_13_8].V[1]*conj(grafo[bar_13_8].adjacentes[0].Cur[1]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Pnom[2] = creal(grafo[bar_13_8].V[2]*conj(grafo[bar_13_8].adjacentes[0].Cur[2]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Qnom[0] = cimag(grafo[bar_13_8].V[0]*conj(grafo[bar_13_8].adjacentes[0].Cur[0]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Qnom[1] = cimag(grafo[bar_13_8].V[1]*conj(grafo[bar_13_8].adjacentes[0].Cur[1]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Qnom[2] = cimag(grafo[bar_13_8].V[2]*conj(grafo[bar_13_8].adjacentes[0].Cur[2]));
            // soma por que podem ter vários 13.8 ligados no 34.5
            grafo[bar_34_5].Cur[0] += grafo[bar_13_8].adjacentes[0].Cur[0];
            grafo[bar_34_5].Cur[1] += grafo[bar_13_8].adjacentes[0].Cur[1];
            grafo[bar_34_5].Cur[2] += grafo[bar_13_8].adjacentes[0].Cur[2];                

            grafo[bar_34_5].S[0] += grafo[bar_13_8].V[0]*conj(grafo[bar_13_8].adjacentes[0].Cur[0]);
            grafo[bar_34_5].S[1] += grafo[bar_13_8].V[1]*conj(grafo[bar_13_8].adjacentes[0].Cur[1]);
            grafo[bar_34_5].S[2] += grafo[bar_13_8].V[2]*conj(grafo[bar_13_8].adjacentes[0].Cur[2]);
        }
    }

    // próximo loop ele roda o fluxo para o 34.5kV
    // Cálculo do Fluxo de Potência no nível de 34.5 kV
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
        if (runPF_alimentador[idAlim]){
            // run power flow for feeder if true, selected on list
            tIni = omp_get_wtime();
            
            // Run highest level - Multiplos Niveis - Segunda rodada com alimentadores a montante - 34.5kV
            if (!flag_niveis[idAlim]){
                if (opt_flow) powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador_tf(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase,configuracoesParam,dadosAlimentadorParam,0,matrizB);
                // if (opt_flow) powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                else powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador_IteracaoUnica(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                
                runPF_alimentador[idAlim] = false;     // finished calculating power flow on the feeder
                fprintf(arquivo,"\n %d \t34.5\t%ld\t%d\t%.12lf\t%lf",i,alimentadores[idAlim].numeroNos,powerflow_result[idAlim].iteracoes,tempoAUX, powerflow_result[idAlim].menorTensao );

                
            }
            tEnd = omp_get_wtime(); 
            tempoAUX = tEnd - tIni;
            
            //verfica a mudança de tap, a partir do power_flow result se mudou o tap em algum dos alimentadores
            //-----------------------------------------------------------------------
            // for tap changing control at upper level   (check only at upper levels)
            if (powerflow_result[idAlim].tap_change_flag && iteration_multiple_level_OPT){
                tap_modified = true;
                
                printf("\n Mudanca de tap em 34.5 kV (Alimentador %d)- Fluxo de potência será recalculado em 13.8kV\n", idAlim);
                runPF_alimentador[idAlim] = true;  // will run the power flow another time due to updated tap position
                // só roda o power flow para os alimentadores em 34.5kV que o tap mudou

                // selects also the downstream feeders to run power flow (13.8kV)
                for (int idInter = 0; idInter < numeroInterfaces; idInter++){ // seta os alimentadores em 13.8kV que tem que rodar tbm
                    if (interfaceNiveis[idInter][2] == 0){ //Para mútliplos níveis, até o momento somente dois
                        int bar_13_8 = interfaceNiveis[idInter][0];
                        int bar_34_5 = interfaceNiveis[idInter][1];

                        if (grafo[bar_34_5].idAlim == idAlim){
                            runPF_alimentador[grafo[bar_13_8].idAlim] = true;  // will run the power flow another time due to updated tap position
                            
                            // root node votlage in 13.8 kV updated according 34.5 kV
                            grafo[bar_13_8].V[0] = cabs(grafo[bar_34_5].V[0])/cabs(grafo[bar_13_8].V[0])*grafo[bar_13_8].V[0];
                            grafo[bar_13_8].V[1] = cabs(grafo[bar_34_5].V[1])/cabs(grafo[bar_13_8].V[1])*grafo[bar_13_8].V[1];
                            grafo[bar_13_8].V[2] = cabs(grafo[bar_34_5].V[2])/cabs(grafo[bar_13_8].V[2])*grafo[bar_13_8].V[2];
                        }
                    }
                }
            }
            //-----------------------------------------------------------------------
        }
    }//Fim do loop dos alimentadores  
    tEnd = omp_get_wtime();
    printf("\nTempo BFS: %lf\n", tEnd - tIni1);
    
    // Força uma iteração entre níveis
    if (level_iteration == 0){
        
        // selects also the downstream feeders to run power flow (13.8kV e estacoes de chave - todos que estiverem no DINTERSE)
        for (int idInter = 0; idInter < numeroInterfaces; idInter++){
            if (interfaceNiveis[idInter][2] == 0){ //Para mútliplos níveis, até o momento somente dois
                int bar_13_8 = interfaceNiveis[idInter][0];
                int bar_34_5 = interfaceNiveis[idInter][1];

                runPF_alimentador[grafo[bar_13_8].idAlim] = true;  // will run the power flow another time
                runPF_alimentador[grafo[bar_34_5].idAlim] = true;  // will run the power flow another time

                // root node votlage in 13.8 kV updated according 34.5 kV
                grafo[bar_13_8].V[0] = cabs(grafo[bar_34_5].V[0])/cabs(grafo[bar_13_8].V[0])*grafo[bar_13_8].V[0];
                grafo[bar_13_8].V[1] = cabs(grafo[bar_34_5].V[1])/cabs(grafo[bar_13_8].V[1])*grafo[bar_13_8].V[1];
                grafo[bar_13_8].V[2] = cabs(grafo[bar_34_5].V[2])/cabs(grafo[bar_13_8].V[2])*grafo[bar_13_8].V[2];

            }
        }
         
        // força a flag retornar no inicio
        tap_modified = true;
    }
    level_iteration++;
    
    // Check if the option for multiple iterations is on
    if (!iteration_multiple_level_OPT){
        tap_modified = false;
    }

    // se ocorrerem mais de 20 iterações ele para
    // To avoid infinite loop
    if (level_iteration  > 20){
        tap_modified = false;
    }
    
    
    } // end of loop of iterations among different levels (34.5kV and 13.8kV)
    
    
    
    fclose(arquivo);

    //Função Atualiza SEs
        
         
    

//    FILE *arq_alim;
//    arq_alim = fopen("resultadoAlim.csv","a+");
//    
//    fprintf(arq_alim,"ID\tNUMERO_NOS\tBARRA\tTENSAO_NOM\tNOS\tITERACOES\tMENOR_TENSAO_A\tMENOR_TENSAO_B\tMENOR_TENSAO_C\tCARGA_TOTAL_A\tCARGA_TOTAL_B\tCARGA_TOTAL_C\tPERDAS\tCARREGAMENTO_REDE\tCARREG_CORRENTE\tMENRO_TENSAO\tQUEDA_MAX\tDESBALANCO_TENSAO\tDESBALANCO_CORRENTE\tDESBALANCO_ALIM\n");
    int id_menorV, id_maxCar, id_maxCurNeutro, id_maxQueda, id_itmax, id_maxDesbalanco, id_maxDesbalancoCorrente, id_maxDesbalancoCorrenteAlim;
    powerflow_result_rede.iteracoes = 0;
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
//        if ((idAlim == 18) || (idAlim ==94)){
//            fprintf(arq_alim,"%d\t%d\t%ld\t%.1lf\t%ld\t%d",idAlim,powerflow_result[idAlim].convergencia,alimentadores[idAlim].noRaiz,grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000,alimentadores[idAlim].numeroNos,powerflow_result[idAlim].iteracoes);
//            fprintf(arq_alim,"\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf",powerflow_result[idAlim].menorTensaoABC[0],powerflow_result[idAlim].menorTensaoABC[1],powerflow_result[idAlim].menorTensaoABC[2], powerflow_result[idAlim].carregamentoRedeABC[0],powerflow_result[idAlim].carregamentoRedeABC[1],powerflow_result[idAlim].carregamentoRedeABC[2]);
//            fprintf(arq_alim,"\t%lf\t%lf\t%lf\t%lf\t%lf", powerflow_result[idAlim].perdasResistivas* Sbase,powerflow_result[idAlim].carregamentoRede* Sbase,powerflow_result[idAlim].maiorCarregamentoCorrente,powerflow_result[idAlim].menorTensao, powerflow_result[idAlim].quedaMaxima);
//            fprintf(arq_alim,"\t%lf\t%lf\t%lf\n", powerflow_result[idAlim].desbalancoTensaoMaximo * 100, powerflow_result[idAlim].desbalancoCorrenteMaximo *100, powerflow_result[idAlim].desbalancoCorrenteAlim*100);
//        }
        powerflow_result_rede.perdasResistivas += powerflow_result[idAlim].perdasResistivas;
        powerflow_result_rede.carregamentoRede += powerflow_result[idAlim].carregamentoRede;
        powerflow_result_rede.carregamentoRedeABC[0] += powerflow_result[idAlim].carregamentoRedeABC[0];
        powerflow_result_rede.carregamentoRedeABC[1] += powerflow_result[idAlim].carregamentoRedeABC[1];
        powerflow_result_rede.carregamentoRedeABC[2] += powerflow_result[idAlim].carregamentoRedeABC[2];
        
        //Atualiza as Interfaces das SEs de 34.5 kV com a carga calculada em 13.8 kV
        
        if (powerflow_result[idAlim].iteracoes > powerflow_result_rede.iteracoes){
            id_itmax = idAlim;
            powerflow_result_rede.iteracoes = powerflow_result[idAlim].iteracoes;
        }
        if (powerflow_result[idAlim].maiorCarregamentoCorrente > powerflow_result_rede.maiorCarregamentoCorrente){
            id_maxCar = idAlim;
            powerflow_result_rede.maiorCarregamentoCorrente = powerflow_result[idAlim].maiorCarregamentoCorrente;
        }
        if (powerflow_result[idAlim].correnteNeutroSE > powerflow_result_rede.correnteNeutroSE){
            id_maxCurNeutro = idAlim;
            double Ibase =  Sbase / (sqrt(3) * grafo[alimentadores[idAlim].idRaiz].Vbase);
            powerflow_result_rede.correnteNeutroSE = powerflow_result[idAlim].correnteNeutroSE * Ibase;
        }

        if (powerflow_result[idAlim].menorTensao < powerflow_result_rede.menorTensao){
            id_menorV = idAlim;
            powerflow_result_rede.menorTensao = powerflow_result[idAlim].menorTensao;
        }
        if (powerflow_result[idAlim].quedaMaxima > powerflow_result_rede.quedaMaxima){
            id_maxQueda = idAlim;
            powerflow_result_rede.quedaMaxima = powerflow_result[idAlim].quedaMaxima;
        }

        if (powerflow_result[idAlim].desbalancoTensaoMaximo > powerflow_result_rede.desbalancoTensaoMaximo){
            id_maxDesbalanco = idAlim;
            powerflow_result_rede.desbalancoTensaoMaximo = powerflow_result[idAlim].desbalancoTensaoMaximo;
        }

        if (powerflow_result[idAlim].desbalancoCorrenteMaximo > powerflow_result_rede.desbalancoCorrenteMaximo){
            id_maxDesbalancoCorrente = idAlim;
            powerflow_result_rede.desbalancoCorrenteMaximo = powerflow_result[idAlim].desbalancoCorrenteMaximo;
        }

        if (powerflow_result[idAlim].desbalancoCorrenteAlim> powerflow_result_rede.desbalancoCorrenteAlim){
            id_maxDesbalancoCorrenteAlim = idAlim;
            powerflow_result_rede.desbalancoCorrenteAlim = powerflow_result[idAlim].desbalancoCorrenteAlim;
        } 
            
    }
    for (int idInterfaces = 0; idInterfaces < numeroInterfaces; idInterfaces++){
    if (interfaceNiveis[idInterfaces][2] == 0){ //Para mútliplos níveis, até o momento somente dois
        idAlim = grafo[interfaceNiveis[idInterfaces][0]].idAlim;
        powerflow_result_rede.carregamentoRede -= powerflow_result[idAlim].carregamentoRede;
        powerflow_result_rede.carregamentoRedeABC[0] -= powerflow_result[idAlim].carregamentoRedeABC[0];
        powerflow_result_rede.carregamentoRedeABC[1] -= powerflow_result[idAlim].carregamentoRedeABC[1];
        powerflow_result_rede.carregamentoRedeABC[2] -= powerflow_result[idAlim].carregamentoRedeABC[2];
    }
    }
    
//    fclose(arq_alim);
    
    powerflow_result_rede.carregamentoRede = powerflow_result_rede.carregamentoRedeABC[0] + powerflow_result_rede.carregamentoRedeABC[1] + powerflow_result_rede.carregamentoRedeABC[2];
    printf("Numero maximo de iteracoes: %d  (alim: %d  / %ld)\n", powerflow_result_rede.iteracoes, id_itmax,numeroAlimentadores);
    printf("Vmin: %lf p.u. (alim: %d)\n", powerflow_result_rede.menorTensao, id_menorV);
//    printf("tensão mínima por fase: %.4lf \t %.4lf \t %.4lf kW\n", powerflow_result[18].menorTensaoABC[0] , powerflow_result[18].menorTensaoABC[1], powerflow_result[18].menorTensaoABC[2] );
    printf("quedaMax: %lf  %%  (alim: %d)\n", powerflow_result_rede.quedaMaxima,id_maxQueda);
    printf("maior carregamentoCorrente: %lf %%  (alim: %d)\n", powerflow_result_rede.maiorCarregamentoCorrente,id_maxCar);
    printf("carregamentoRede: %lf kW\n", powerflow_result_rede.carregamentoRede * Sbase);
    printf("carregamentoRede por fase: %.2lf kW \t %.2lf kW\t %.2lf kW\n", powerflow_result_rede.carregamentoRedeABC[0] * Sbase, powerflow_result_rede.carregamentoRedeABC[1] * Sbase, powerflow_result_rede.carregamentoRedeABC[2] * Sbase);
    printf("perdas totais: %lf kW\n", powerflow_result_rede.perdasResistivas * Sbase);
    printf("maximo desbalanco de tensão: %lf %%  (alim: %d)\n", powerflow_result_rede.desbalancoTensaoMaximo *100, id_maxDesbalanco);
//    printf("maximo desbalanco de corrente: %lf %%  (alim: %d)\n", powerflow_result_rede.desbalancoCorrenteMaximo *100, id_maxDesbalanco);
    printf("maximo desbalanco de corrente de saida de alimentador: %lf %%  (alim: %d)\n", powerflow_result_rede.desbalancoCorrenteAlim *100, id_maxDesbalancoCorrenteAlim);
    printf("maxima corrente de neutro de saida de alimentador: %.2lf A  (alim: %d)\n", powerflow_result_rede.correnteNeutroSE *100, id_maxCurNeutro);
    
    
    
    //Impressão de resultados em arquivos
    int ppt_aux = 0;
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
            if (ppt_aux == 0){
                salvaTensoesNodais("stateVT.txt","w+",alimentadores[idAlim],grafo);
                salvaCorrentesRamos("loadingI.txt", "w+", alimentadores[idAlim],grafo, numeroBarras, Sbase);
                ppt_aux=1;
            }
            else{
                salvaTensoesNodais("stateVT.txt","a+",alimentadores[idAlim],grafo);
                salvaCorrentesRamos("loadingI.txt", "a+", alimentadores[idAlim],grafo, numeroBarras, Sbase);
            }
    }
    



}
