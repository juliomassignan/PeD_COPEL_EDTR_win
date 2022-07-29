/* 
 * File:   funcoesIntegracao.h
 * Author: Julio Massignan e Vítor Henrique Pereira de Melo
 *
 * Created on 31 de Jul de 2022, 14:03
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

void converteGrafo_TFtoSDR(TF_GRAFO *grafo_tf,long int numeroBarras_tf,TF_DRAM *ramos_tf,long int nramos_tf,GRAFO **grafoSDRParam,DADOSREGULADOR **dadosReguladorSDRParam,long int* numeroBarras, long int *numeroChaves)
{
    
    int i,k,contador; // variaveis utilizadas como indexadores e contadores em loops
    int nreg=0; // variável que conta o numero de reguladores

    (*numeroChaves)=0; // variável que conta o numero de chaves e é inicializada com zero // variavel que conta o numero de trafos e é inicializada com zero
    
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
        // Aloca os adjacentes
        (*grafoSDRParam)[contador].adjacentes=Malloc(NOADJACENTE,11);

        //loop para preencher a info dos adjacentes
        for (k=0;k<grafo_tf[i].numeroAdjacentes;k++)
        {
            
            (*grafoSDRParam)[contador].adjacentes[k].idNo=grafo_tf[i].adjacentes[k].idNo+1;// preenche o idNo

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
                //   (*numeroTrafos)=(*numeroTrafos)+1;
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
 * Essa função recebe como parâmetro o vetor @p grafo_tf do tipo TF_GRAFO, com as informacoes dos alimentadores da rede, lidas para o calculo do fluxo de potencia trifasico
 * e o long int @p numerosAlimentadores_tf com o numero de alimentadores de acordo com o fluxo trifásico. Em posse destes dados, esta funcao aloca o espaço necessário para a estrutura @p dadosAlimentadorSDRParam
 * e a preenche, com as informacoes necessarias para utilizar as funcoes da rnp. 
 *
 * 
 * @param grafo_tf vetor do tipo TF_GRAFO com as informacoes trifasicas sobre a rede
 * @param numeroBarras numero inteiro com o numero de barras da rede
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
    int idraiz[numerosAlimentadores_tf]; //aloca o vetor com as raízes do sistema
    
    for(i=0; i<numeroBarras; i++){ 
        if(grafo_tf[i].tipo == 2){ // se for nó raíz guarda no grafo
            idraiz[k]=grafo_tf[i].idNo;
            k++;   
        }
    }


    //Aloca estrutura dado alimentadores
    if (((*dadosAlimentadorSDRParam)= (DADOSALIMENTADOR *)malloc( (numerosAlimentadores_tf+1) * sizeof(DADOSALIMENTADOR)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para alimentadores !!!!");
        exit(1); 
    }
    //Atribui o número de alimentadores de acordo com os dados do trifásico
    numeroAlimentadores=numerosAlimentadores_tf;

    //aloca memoria para a estrutura DADOSALIMENTADOR, caso nao seja possivel encerra o programa

    //loop que preenche os dados dos alimentadores de acordo com o trifásico
    for ( i = 0; i < numerosAlimentadores_tf; i++)
    {
        contador=i+1;
        (*dadosAlimentadorSDRParam)[contador].barraAlimentador = idraiz[i]+1; // preenche a barraAlimentador com o noRaiz
        (*dadosAlimentadorSDRParam)[contador].idTrafo = idraiz[i]+1;// preenche com idRaiz do no raiz
        (*dadosAlimentadorSDRParam)[contador].numeroSetores=0;// inicia o numero de setores com zero
    }


}


//
/**
 * @brief Função de integracao da MRAN, para compatibilizar as barras ficticias criadas no código da MRAN nos elementos do fluxo trifásico 
 * 
 * Essa função recebe como parâmetro o vetor @p grafoSDR do tipo GRAFO, com as informacoes da rede modificada pela MRAN, @p numeroNos que é um inteiro com o número de nós da rede
 *. de acordo com estas informacoes ela complementa os nós criados nas seguintes variáveis, que são utilizadas no fluxo trifásico:
  @p grafo_tf do tipo TF_GRAFo, @p barras do tipo TF_DBAR, @p ramos do tipo TF_DRAM, @p numeroRamos, @p numeroBarras 
 *
 * 
 * @param grafo_tf vetor do tipo TF_GRAFO com as informacoes trifasicas sobre a rede
 * @param barras vetor do tipo TF_DBAR com as informações trifásicas sobre as barras da rede
 * @param numeroBarras inteiro com o número de barras da rede trifásica
 * @param ramos numero inteiro que conta o numero de alimentadores presentes no sistema
 * @param numeroRamos inteiro que conta o número de ramos da rede
 * @param grafoSDR vetor do tipo GRAFO, o grafo da rede de acordo com a MRAN 
 * @param numeroNos inteiro com o número de nós da rede de acordo com a MRAN
 * @return void
 * @see 
 * @note
 * @warning .
 */



void compatibiliza_chaveSetoresFicticia_tf(TF_GRAFO** grafo_tf,TF_DBAR **barras, long int *numeroBarras, TF_DRAM **ramos, long int *numeroRamos,GRAFO* grafoSDR, long int numeroNos)
{

    int aux=numeroBarras[0]; //variável auxiliar com o número de barras da rede trifásica original
    int de; // inteiro que armazena a barra de 
    int para; // inteiro que armazena a barra para
    int idAdj; //inteiro guardar o id dos adjacentes

    for(int i=aux;i<numeroNos;i++) // for que começa do final do grafoSDR pois os ultimos nós que são os novos
    {
        idAdj=grafoSDR[i+1].adjacentes[0].idNo-1; // pega o id do adjacente ao ultimo nó da MRAN
        includeDBAR(barras,numeroBarras,(*grafo_tf)[idAdj].Vbase);// funcao que incui ele no DBAR
        includeNografoTF(grafo_tf,(*barras),numeroBarras[0]-1); // funcao que inclui ele no grafo trifásico
        for (size_t j = 0; j < grafoSDR[i+1].numeroAdjacentes; j++)//for que percorre os adjacentes  dos nós ficticios
        {
           de=grafoSDR[i+1].idNo-1; //salva no de
           para=grafoSDR[i+1].adjacentes[j].idNo-1; //salva no para 
           includeDSWTC(ramos,numeroRamos,de,para,grafoSDR[i+1].adjacentes[j].estadoChave); // inclui as novas chaves no ramos
           includeAdjGrafoTF((*grafo_tf),(*barras),i,(*ramos),numeroRamos[0]-1); //inclui os adjacentes no grafo no barras
        }
        
        

    }


}


/**
 * @brief Função de integracao da MRAN, para compatibilizar as saídas do fluxo trifásico com as estruturas da MRAN.
 * 
 * Essa função recebe como parâmetro o vetor @p powerflow_result do tipo TF_PFSOLUTION, com as informacoes do fluxo trifásico, @p idIndividuoInicialParam com o id do indivíduo inicial
 *. @p idIndividuoInicialParam inteiro com o número de configuracoes da MRAN @p configuracao vetor de estrutura do tipo CONFIGURACAO com as informacoes de cada configuracao, que recebera os resultados do fluxo de potencia
 *
 * 
 * @param powerflow_result vetor da estrutura do tipo TF_PFSOLUTION com os resultados do fluxo de potência
 * @param idIndividuoInicialParam inteiro com o id do indivíduo inicial da MRAN
 * @param numeroConfiguracoesParam inteiro com o número de configuracoes da MRAN
 * @param configuracao vetor do tipo CONFIGURACAO com as informacoes de cada configuracao da MRAN
 * @return void
 * @see 
 * @note
 * @warning .
 */


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
 * @brief Função auxiliar para inicializar as tensões complexas nodais trifásicas de todos os alimentadores através de um processo de varredura direta por RNP de Setores
 * 
 * Essa função inicializa as tensões complexas nodais por fase (fase-neutro) de todas as barras de um alimentador de acordo com a RNP de setores
 * O valor de inicilização é obtido através de uma varredura direta utilizando a RNP de setores disponível na  @p configuracaoParam que é um vetor com as configuracoes da rede
 * que é indexada pelo inteiro @p indiceConfiguracao com o id da configuracao atual, com o vetor @p matrizB do tipo RNPSETORES com as RNPS de setores, @p indiceRNP um inteiro com o índice da RNP de setores atual
 * @p todosAlimentadores do tipo BOLL que informa se a funcao seerá executada para todos alimentadores, @p root inteiro com o nó raiz do alimentador, @p grafo do tipo TF_GRAFO com as informações da rede trifásica e que recebera as tensoes  
 * @p numeroAlimentadores inteiro com o número de alimentaores da rede, @p numeroBarras com o número de barras da rede, @p alimentadores estrutura do tipo TF_ALIMENTADOR com os alimentadores da rede.
 * A função retorna @c void.
 * 
 * @param grafo ponteiro para o grafo da rede elétrica para inicilizar as tensões nodais
 * @param numeroBarras quatidade total de barras
 * @param alimentadores ponteiro par os alimentadores da rede elétrica
 * @param numeroAlimentadores quantidade total de alimentadores
 * @param todosAlimentadores BOLL que indica se deve ser executado para todos os alimentadores
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @param indiceRNP inteiro com o indice da RNP atual
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param root inteiro com o indice do no raiz
 * @param 
 * @return void
 * @see
 * @note 
*/
void inicializaTensaoSDR_alimentador_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, CONFIGURACAO* configuracaoParam, RNPSETORES *matrizB,int indiceRNP,int indiceConfiguracao,int root)
{

    long int indice, indice1, noS, noR, noN,no_prev, idSetorS, idSetorR, idBarra1, idBarra2, indice2, indice3; //inteiros que serão utilizados para contagem e indices de nos
    long int iniAlim; // inteiro que guarda o inicio do alimentador
    int k; // contador
    long int noProf[1000]; //armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
    RNPSETOR rnpSetorSR; // variavel que guarda a RNP de setores atuais
   
    //inicializa o indice que percorre a rpn de setores
    indice=0;

    //pega o nó do inicio da RNP de setores

    iniAlim=configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
    
    // coloca ele no vetor noProf
    noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = iniAlim;


    // percorre aquela rnp de setores em questão
    for (indice = 1; indice < configuracaoParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
            // varre as rnps de setores
            
            //pega o no da profundidade imediatamente anterior
            noR = noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
            //pega o no atual
            noS = configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo; // pega o id do proximo no
            
            // pega a RNP do setor, entre aquele nó e o nó da profundidade imediatamente anterior   
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
            
            //percorre a rpn interna do setor a inicializa as tensoes de todas as barras do setor de acordo com a raiz do alimentador
            for (indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {
                //percorre todos os nos da RNP daquele setor
                noN = rnpSetorSR.nos[indice1].idNo;                
                grafo[noN-1].V[0]=grafo[root-1].V[0];
                grafo[noN-1].V[1]=grafo[root-1].V[1];
                grafo[noN-1].V[2]=grafo[root-1].V[2];
                atualizaInjecoes(&grafo[noN-1]); // inicializa as indecoes de corrente

            }
        //atualiza a profundidade do nó com seu indice       
        noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo; // preenche a próxima profundidade
    }            

}


/**
 * @brief Função que inicializa a tenssão de todos os alimentadores
 * 
 * Essa função percorre todos os alimentadores que tem suas tensões incializadas individualmente a partir da fução inicializaTensaoSDR_alimentador_tf
 * Esta função recebe como parâmetros @p configuracaoParam que é um vetor com as configuracoes da rede que é indexada pelo inteiro @p indiceConfiguracao com o id da configuracao atual, com o vetor @p matrizB do tipo RNPSETORES com as RNPS de setores
 * @p todosAlimentadores do tipo BOLL que informa se a funcao seerá executada para todos alimentadores, @p grafo do tipo TF_GRAFO com as informações da rede trifásica e que recebera as tensoes  
 * @p numeroAlimentadores inteiro com o número de alimentaores da rede, @p numeroBarras com o número de barras da rede, @p alimentadores estrutura do tipo TF_ALIMENTADOR com os alimentadores da rede.
 * 
 * @param grafo ponteiro para o grafo da rede elétrica para inicilizar as tensões nodais
 * @param numeroBarras quatidade total de barras
 * @param alimentadores ponteiro par os alimentadores da rede elétrica
 * @param numeroAlimentadores quantidade total de alimentadores
 * @param todosAlimentadores BOLL que indica se deve ser executado para todos os alimentadores
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param 
 * @return void
 * @see
 * @note 
**/

void inicializaTensaoSDR_alimentadores_tf (TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores,
BOOL todosAlimentadores, CONFIGURACAO* configuracaoParam,RNPSETORES *matrizB,int indiceConfiguracao)
{
    int indiceRNP; //indice da RNP, corresponde ao alimentador
    int iniAlim,root,noS,noR;//indexa os nós 
    long int BarraProf[1000],noProf[1000]; // vetores auxiliares para percorrer a RNP, armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
    RNPSETOR rnpSetorSR; // estrutura que recebe cada RNP de setor

    //Percorre todas as RNPs do nível mais alto da configuracao[indiceConfiguracao] 


    for (indiceRNP = 0; indiceRNP < configuracaoParam[indiceConfiguracao].numeroRNP; indiceRNP++) // percorre todas as RPNs de Setores
    {

        //determina o no raíz do alimentador (incideRNP)    
        iniAlim=configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[0].idNo;    
        noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[0].profundidade] = iniAlim;
        noS = configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[1].idNo;
        noR = noProf[configuracaoParam[indiceConfiguracao].rnp[indiceRNP].nos[1].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
        //No raiz do alimentador
        root=rnpSetorSR.nos[0].idNo;   
        
        grafo[root-1].V[0]=grafo[root-1].barra->Vinicial[0];// inicializa tensão do no raiz 
        grafo[root-1].V[1]=grafo[root-1].barra->Vinicial[1];
        grafo[root-1].V[2]=grafo[root-1].barra->Vinicial[2];

        inicializaTensaoSDR_alimentador_tf(grafo,numeroBarras,alimentadores,numeroAlimentadores,configuracaoParam,matrizB,indiceRNP,indiceConfiguracao,root);

    }

}


/**
 * @brief Função Avalia configuracao, realiza o cálculo do fluxo de potência para uma configuracao específica e compila os resultados, 
 * recebe como parâmetros @p todosAlimentadores, um booleano que indica se o fluxo deve ser resolvido para todos os alimentadores ou apenas para dois
 * @p FirstEXEC um boleano que indica se é a primeira execução do fluxo de potência, @p *powerflow_result_rede uma estrutura do tipo TF_PFSOLUTION que armazena
 * os resultados todais da rede, @p powerflow_result_alim um vetor com os resultados de todos os alimentadores, @p configuracoesParam do tipo CONFIGURACAO
 * com as informações das rnps e os resultados das configuracoes da rede, @p idNovaConfiguracaoParam um inteiro com a identificação da configuração atual,
 * @p grafo_tf uma estrutura do tipo TF_GRAFO com as informações elétricas do grafo da rede, @p numeroBarras_tf com o número de barras totais de acordo com a rede trifásica
 * @p alimentador_tf vetor do tipo TF_ALIMENTADOR, @p numeroAlimentadores_tf inteiro com o número de alimentadores na rede, @p ramos_tf estrutura do tipo TF_DRAM
 * com as informações sobre os ramos da rede trifásica, @p Sbase double com a potência base da rede, @p interfaceNiveis_tf matriz de inteiro utilizada para fazer
 * a interface entre niveis, @p numeroInterfaces_tf inteiro com o númeor de interfaces na rede, @p opt_flow boleano da opção de fluxo de potência @p numeroTrafosSE
 * numero de trafos na subestação 
 *  
 * 
 * 
 * @param todosAlimentadores boleano que indica se o fluxo será realizado em todos os alimentadores
 * @param FirstEXEC booleano que indica se é a primeira execução
 * @param powerflow_result_rede estrutura do tipo TF_PFSOLUTION que guarda os resultados do fluxo de potência da rede
 * @param powerflow_result_alim vetor do tipo TF_PFSOLUTION que guarda os resultados do fluxo de potência dos alimentadores
 * @param configuracoesParam vetor do tipo CONFIGURACAO que guarda as informacoes das configuracoes
 * @param idNovaConfiguracaoParam inteiro com o indice da configuracao atual
 * @param dadosAlimentadorParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @param grafo_tf vetor do tipo TF_GRAFO com as informacoes trifasicas sobre a rede
 * @param numeroBarras_tf inteiro com o número de barras da rede, segundo o fluxo trifásico
 * @param alimentador_tf estrutura do tipo TF_ALIMENTADOR com as informações sobre os alimentadores 
 * @param numeroAlimentadores_tf inteiro com o numero de alimentadores trifásicos
 * @param ramos_tf vetor do tipo TF_DRAM com as informações dos ramos da rede, ulizada para preencher o grafoSDR e dadosRegulador
 * @param Sbase potencia base do sistema
 * @param interfaceNiveis_tf matriz de inteiros com o número de interfaces na rede
 * @param  opt_flow boleano
 * @param numeroTrafosSE numero de transformadores de subestacao
 * @return void
 * @see
 * @note 
 * @warning 
 * Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * */
void avaliaConfiguracaoSDR_tf(BOOL todosAlimentadores,BOOL FirstEXEC, TF_PFSOLUTION *powerflow_result_rede, TF_PFSOLUTION **powerflow_result_alim, CONFIGURACAO *configuracoesParam, long int idNovaConfiguracaoParam, /*DADOSTRAFO *dadosTrafoParam, int numeroTrafosParam,*/
        /* int numeroAlimentadoresParam,int *indiceRegulador, DADOSREGULADOR *dadosRegulador,*/ DADOSALIMENTADOR *dadosAlimentadorParam, /*double VFParam,*/ int idAntigaConfiguracaoParam, RNPSETORES *matrizB, int RNP_P,int RNP_A,/*MATRIZCOMPLEXA *ZParam,*/
        /*MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam, /*BOOL copiarDadosEletricos,*/
        TF_GRAFO *grafo_tf, long numeroBarras_tf, TF_ALIMENTADOR *alimentador_tf, int numeroAlimentadores_tf,
        TF_DRAM *ramos_tf,double Sbase, long int **interfaceNiveis_tf,long int numeroInterfaces_tf, BOOL opt_flow,long int numeroTrafosSE)

{    
    double VFparam,perdasTotais = 0; //variáveis para calculo de resultados


    RNPSETOR rnpSetorSR;

    // aloca memoria para a estrutura de resultados

    if (FirstEXEC == 1)
    {
        if (((*powerflow_result_alim) = (TF_PFSOLUTION *)malloc(numeroAlimentadores*sizeof(TF_PFSOLUTION)))==NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para os resultados !!!!");
            exit(1); 
        }

    }

    

    if (todosAlimentadores) //calcula os valores de fitness para todos os alimentadores
    {   
        inicializaTensaoSDR_alimentadores_tf(grafo_tf,numeroBarras_tf,alimentador_tf,numeroAlimentadores_tf,1,configuracoesParam,matrizB,idNovaConfiguracaoParam);
        fluxoPotencia_Niveis_BFS_Multiplos_tf(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores_tf, ramos_tf, Sbase/1000, interfaceNiveis_tf, numeroInterfaces_tf, true,dadosAlimentadorParam,configuracoesParam,idNovaConfiguracaoParam,matrizB,powerflow_result_rede,powerflow_result_alim);
        // true converge FP | false faz iteraçao única (P&D)
        
    } 
    else if(FirstEXEC == 0){ //calcula os valores de fitness para dois alimentadores específicos RNP_P e RNP_A

        fluxoPotencia_alimentador_P_A_tf(grafo_tf,numeroBarras_tf,alimentador_tf,ramos_tf,Sbase,configuracoesParam,dadosAlimentadorParam,idNovaConfiguracaoParam,matrizB,RNP_P,RNP_A,numeroInterfaces_tf,interfaceNiveis_tf,powerflow_result_rede,powerflow_result_alim);
    }
    else{
        printf("Erro -- Deve-se Executar para todos alimentadores ao menos uma vez !!!!");
        exit(1); 
    }
 

 
    //salva os resultados dentro da configuracao
    configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo = malloc((numeroTrafosSE + 1) * sizeof (__complex__ double));
    configuracoesParam[idNovaConfiguracaoParam].objetivo.menorTensao = (*powerflow_result_rede).menorTensao;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoRede = cabs((*powerflow_result_rede).maiorCarregamentoPotencia);
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente = (*powerflow_result_rede).maiorCarregamentoCorrente;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = (*powerflow_result_rede).maiorCarregamentoTrafo;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.perdasResistivas = (*powerflow_result_rede).perdasResistivas;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.ponderacao = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.quedaMaxima = (*powerflow_result_rede).quedaMaxima;
    
    //salva os resultados dentro da configuracao por alimentador
    for (int i=0;i<configuracoesParam[idNovaConfiguracaoParam].numeroRNP;i++)
    {
        configuracoesParam[idNovaConfiguracaoParam].rnp[i].fitnessRNP.potenciaAlimentador = (*powerflow_result_alim)[i].carregamentoRede;
        configuracoesParam[idNovaConfiguracaoParam].rnp[i].fitnessRNP.maiorCarregamentoCorrente = (*powerflow_result_alim)[i].maiorCarregamentoCorrente;
        configuracoesParam[idNovaConfiguracaoParam].rnp[i].fitnessRNP.carregamentoRede=(*powerflow_result_alim)[i].carregamentoRede;
        configuracoesParam[idNovaConfiguracaoParam].rnp[i].fitnessRNP.perdasResistivas = (*powerflow_result_alim)[i].perdasResistivas;
    }


    VFparam=cabs(grafo_tf[0].Vbase);//define o VFparam (verificar)
    calculaPonderacao(configuracoesParam,idNovaConfiguracaoParam,VFparam); //Calcula a ponderacao
    //calcula o carregamento do trafo
    carregamentoTrafo_tf(alimentador_tf,numeroAlimentadores_tf,numeroTrafosSE,configuracoesParam,idNovaConfiguracaoParam,idAntigaConfiguracaoParam,todosAlimentadores,RNP_P,RNP_A);

    
}

/**
 * @brief Função que compatibiliza a profundidade do grafo trifásico com a da RNP de setores
 * 
 * Essa função compatibiliza a profunidade na estrutura grafo do tipo TF_GRAFO e que contem as informações sobre a rede trifásica.
 * Esta função realiza uma varredura fowared pela RNP de setores e pelos setores e insere no grafo as profundidades totais da rede.
 * Esta função recebe como parâmetros @p grafo_tf que é um vetor do tipo TF_GRAFO com as informações da rede trifásica, do tipo TF_GRAFO com as informações da rede trifásica, 
 * @p configuracaoParam que é um vetor com as configuracoes da rede que é indexada pelo inteiro @p indiceConfiguracao com o id da configuracao atual, com o vetor @p matrizB do tipo RNPSETORES com as RNPS de setores 
 * @p indiceRNP trata-se de um inteiro com o alimentador que está sendo preenchido, @p nvar ponteiro para inteiro que retorna o número de variáveis da rede  
 * 
 * @param grafo ponteiro para o grafo da rede elétrica para inicilizar as tensões nodais
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param nvar ponteiro para inteiro que retorna o número de variáveis da rede 
 * @return void
 * @see
 * @note 
**/


void compatibiliza_profundidadegrafo_tf(TF_GRAFO *grafo_tf,CONFIGURACAO *configuracoesParam,int indiceConfiguracao,RNPSETORES *matrizB,long int indiceRNP, int *nvar)
{

    int noRaiz,noR,noS,noN; // indexadores dos nós 
    int idadj; // indexador dos nós adjacentes
    int raiz_inter; // salva raiz
    int indiceRNPsetor; // indice da RNP de setor
    int iniAlim; //inicio do alimentador
    int n_var=0; // variável que conta o número de variáveis
    long int noProf[1000]; // vetores auxiliares para percorrer a RNP, armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
    long int BarraProf[1000];  // vetores auxiliares para percorrer a RNP, armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
    RNPSETOR rnpSetorSR; // estrutura que recebe cada RNP de setor
    int indice,indice1;
    
    //determina o nó inicial do alimentador
    iniAlim=configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].idNo;    
    noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].profundidade] = iniAlim;
    noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[1].idNo;
    noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[1].profundidade - 1];

    // faz uma varredura direta na RNP do setores
    for (indice = 1; indice < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
        noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
        noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
        BarraProf[rnpSetorSR.nos[0].profundidade] = rnpSetorSR.nos[0].idNo;
        //faz uma varredura foward na rnp interna dos setores para inicializar a profundidade de todos os nós como 0
        for (indice1 = 0; indice1 < rnpSetorSR.numeroNos; indice1++) {
            noN = rnpSetorSR.nos[indice1].idNo;
            
            grafo_tf[noN-1].profundidade= 0; 
            
            BarraProf[rnpSetorSR.nos[indice1].profundidade] = rnpSetorSR.nos[indice1].idNo;
        }
        //armazena o nó setor na sua profundidade
        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
    }


    // faz uma varredura direta na RNP de setores
    iniAlim=configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].idNo;    
    noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[0].profundidade] = iniAlim;

    for (indice = 1; indice < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
        noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo; 
        noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR); // pega a rnp do setor entre estes dois nós
        BarraProf[rnpSetorSR.nos[0].profundidade] = rnpSetorSR.nos[0].idNo;// percorre a RNP de setores
        for (indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {
            
            noN = rnpSetorSR.nos[indice1].idNo;
            grafo_tf[noN-1].profundidade = rnpSetorSR.nos[indice1].profundidade+grafo_tf[rnpSetorSR.nos[0].idNo-1].profundidade; 
            n_var=n_var+6;
            BarraProf[rnpSetorSR.nos[indice1].profundidade] = rnpSetorSR.nos[indice1].idNo;
        }
        //armazena o nó setor na sua profundidade
        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
    }


    (*nvar)=n_var+6; //retorna o numero de variáveis total darede



}




//Cálculo de Fluxo de Potência para um Alimentador de Distribuição Baseado na Varredura Direta/Inversa Trifasica Utilizando RPN
/**
 * @brief Função auxiliar para o cálculo de fluxo de potência trifásico em um alimentador específico via método de Varredura Direta/Inversa através da RNP de setores,
 * utiliza as informacoes da rede na estrutura @p grafo do tipo TF_GRAFO, com as informacoes elétricas da rede, respeiando a configuacao topologica disponível na 
 * @p configuracoesParam ponteiro do tipo CONFIGURACAO com a configuracao da rede, @p matrizB ponteiro do tipo RNPSETORES com a RNP de setores da rede  
 * 
 * Essa função efetua
 * A função retorna @c TF_PFSOLUTION.
 * 
 * @param grafo grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param numeroBarras quatidade total de barras
 * @param alimentadores alimentadores da rede elétrica
 * @param ramos vetor do tipo TF_DRAM com as informações dos ramos da rede
 * @param Sbase double com a potencia base da rede
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param dadosAlimentadorParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
*/
TF_PFSOLUTION fluxoPotencia_BFS_Alimentador_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR alimentador, TF_DRAM *ramos,double Sbase,
        /*int numeroBarrasParam,*/ CONFIGURACAO *configuracoesParam, 
        /*double VFParam,*/ DADOSALIMENTADOR *dadosAlimentadorParam ,int indiceConfiguracao, RNPSETORES *matrizB/*,
        MATRIZCOMPLEXA *ZParam,*/ /* MATRIZMAXCORRENTE *maximoCorrenteParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulado*/ ){
   
    int it, nvar, k = 0,i, conv = 0; // declara as variáveis auxiliares da funcao, contadores indexadores e criterio de convergencia
    double *DV=NULL, nDV = 0, loading = 0, cargaAtivaTotal = 0 ;  // declara as variáveis auxiliares da funcao, contadores indexadores e criterio de convergencia
    BOOL control_action = 0; //Boleano que indica se os controles foram ativados
    double fatorDesbalanco; 
    double tempoBFS; //tempo de execucao
    int indiceRNP; // indice da rnp de setores
    int iniAlim; //no inicial 
    int indiceRNPsetores; // indice da rnpSetor
    int indice1, indice; //indices 
    long int noS, noR, noN; // nos para percorrer a rnp
    complex carregamentoNo;
    RNPSETOR rnpSetorSR; // estrutura que recebe cada RNP de setor

    long int noProf[1000];// vetor auxiliar para percorrer a RNP, armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
    long int root;//no raíz




    TF_NOADJACENTE *ramoAdj = NULL;

    TF_PFSOLUTION powerflow_result;


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
    powerflow_result.maiorCarregamentoPotencia = 0;
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

        //percorre a RNP de setores em uma varredura direta
        k=0;

        indiceRNPsetores=0;

        iniAlim=configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;

        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade] = iniAlim;


        for (indiceRNPsetores = 1; indiceRNPsetores < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indiceRNPsetores++)
        {

            noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].profundidade - 1];
            //pega o no atual
            noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indiceRNPsetores].idNo;
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR); //pega a RNP de um setor
            // inicializa o critério de convergencia
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
        //Varredura inversa do fluxo de potencia 
        for (int indice = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos - 1; indice > 0; indice--) {
            int indice2 = indice - 1;
            //busca pelo nó raiz
            while (indice2 >= 0 && configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice2].profundidade != (configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1)) indice2--;                
            int idSetorS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
            int idSetorR = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice2].idNo;
            rnpSetorSR = buscaRNPSetor(matrizB, idSetorS, idSetorR); //retorna a RNP do setor
            for (indice2 = rnpSetorSR.numeroNos - 1; indice2 > 0; indice2--) {
                int indice3 = indice2 - 1;
                while (indice3 >= 0 && rnpSetorSR.nos[indice3].profundidade != (rnpSetorSR.nos[indice2].profundidade - 1)) {
                    indice3--;
                }
                int idBarra1 = rnpSetorSR.nos[indice3].idNo;
                int idBarra2 = rnpSetorSR.nos[indice2].idNo;
                
                backward(&grafo[idBarra2-1], grafo); // chama a etapa backward do BF
   
            }

        }
        

        //etapa de varredura direta

        for (int indice = 1; indice < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
            noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
            noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
            BarraProf[rnpSetorSR.nos[0].profundidade] = rnpSetorSR.nos[0].idNo;

            for (int indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {
                
                noN = rnpSetorSR.nos[indice1].idNo;
                
                control_action = forward(&grafo[noN-1], grafo); //chama a foward do fluxo
                if (control_action) powerflow_result.tap_change_flag = true;
                //armazena o nó barra na sua profundidade
                BarraProf[rnpSetorSR.nos[indice1].profundidade] = rnpSetorSR.nos[indice1].idNo;
            }
            //armazena o nó setor na sua profundidade
            noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
        }

        

        // faz uma varredura foward para calcular o critério de convergência
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
    carregamentoNo=0;
            
    //calculo de resultados
    
    ////----------------------------------------------------------------------
    // Calcula resultados condensados de fluxo de potência  para o alimentador
    if(powerflow_result.convergencia == 1){



        //faz uma varredura foward na RNP de setores para calcular os resultados
        
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

            powerflow_result.perdasResistivas += calculaPerdas(&grafo[noN-1], grafo); // calcula perdas
            loading = calculaCarregamento(&grafo[noN-1], grafo, 1000*Sbase); // calcula carregamento
            cargaAtivaTotal += creal(grafo[noN-1].S[0]) + creal(grafo[noN-1].S[1]) + creal(grafo[noN-1].S[2]); // calcula carga ativa total
            carregamentoNo = grafo[noN-1].S[0]+ grafo[noN-1].S[1] + grafo[noN-1].S[2];
            // deermina o maior carregamento
            if (loading > powerflow_result.maiorCarregamentoCorrente) powerflow_result.maiorCarregamentoCorrente = loading;
            if (creal(carregamentoNo)>creal(powerflow_result.maiorCarregamentoPotencia)) powerflow_result.maiorCarregamentoPotencia =carregamentoNo;
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
            //calcula o desbalanço de tensão
            if (grafo[noN-1].fases == ABC){
                fatorDesbalanco = desbalancoFasorialSeq(grafo[noN-1].V);
                if (fatorDesbalanco > powerflow_result.desbalancoTensaoMaximo)
                    powerflow_result.desbalancoTensaoMaximo = fatorDesbalanco;
            }
            // calcula o desbalanço de corrente
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


        //atribui o carregamento total
        powerflow_result.carregamentoRede = powerflow_result.perdasResistivas + cargaAtivaTotal;
        //calculo da corrente de neutro da Subestação
        powerflow_result.correnteNeutroSE = cabs(grafo[root-1].adjacentes[0].Cur[0] + grafo[root-1].adjacentes[0].Cur[1] + grafo[root-1].adjacentes[0].Cur[2]);
        
        //calcula o carregamento da rede
        powerflow_result.carregamentoRedeABC[0] = creal(grafo[root-1].V[0]*conj(grafo[root-1].adjacentes[0].Cur[0]));
        powerflow_result.carregamentoRedeABC[1] = creal(grafo[root-1].V[1]*conj(grafo[root-1].adjacentes[0].Cur[1]));
        powerflow_result.carregamentoRedeABC[2] = creal(grafo[root-1].V[2]*conj(grafo[root-1].adjacentes[0].Cur[2]));
        
        //desbalanço de corrente na subestação
        if( cabs(grafo[root-1].adjacentes[0].Cur[0] + grafo[root-1].adjacentes[0].Cur[1] + grafo[root-1].adjacentes[0].Cur[2]) != 0 )
            powerflow_result.desbalancoCorrenteAlim = desbalancoFasorialSeq(grafo[root-1].adjacentes[0].Cur);
        else
            powerflow_result.desbalancoCorrenteAlim = 0;
    }
    
    free(DV); 
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
//Cálculo de Fluxo de Potência em todos os alimentadores realizada de acordo com o nível de tesão do alimentador
/**
 * @brief Função auxiliar para o cálculo de fluxo de potência trifásico nos alimentadores via método de Varredura Direta/Inversa através da RNP de setores,
 * , realiza a verradura inicialmente nos alimentadores de 13.8kV, realiza depois nos alimentadores de 34.5kV com a carga do alimentador anterior refletida nele
 * Verifica se há mudança de tap, caso positivo, reinicia o calculo do fluxo nos alimentadores de 13.8kV utiliza as informacoes da rede na estrutura @p grafo do tipo TF_GRAFO, com as informacoes elétricas da rede, respeiando a configuacao topologica disponível na 
 * @p configuracoesParam ponteiro do tipo CONFIGURACAO com a configuracao da rede, @p matrizB ponteiro do tipo RNPSETORES com a RNP de setores da rede  
 * Essa função efetua
 * A função retorna @c TF_PFSOLUTION.
 * 
 * @param grafo grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param numeroBarras quatidade total de barras
 * @param alimentadores alimentadores da rede elétrica
 * @param interfaceNiveis 
 * @param numeroInterfaces
 * @param opt_flow
 * @param ramos vetor do tipo TF_DRAM com as informações dos ramos da rede
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param dadosAlimentadorParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * 
 * */
void fluxoPotencia_Niveis_BFS_Multiplos_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos,double Sbase,
        long int **interfaceNiveis,long int numeroInterfaces, BOOL opt_flow,/* long int numeroBarrasParam, */
        DADOSALIMENTADOR *dadosAlimentadorParam, /*DADOSTRAFO *dadosTrafoParam,GRAFO *grafoSDRParam,*/
        CONFIGURACAO *configuracoesParam, long int indiceConfiguracao, RNPSETORES *matrizB,TF_PFSOLUTION *powerflow_result_rede, TF_PFSOLUTION **powerflow_result_alim/*,
        MATRIZCOMPLEXA *ZParam*//*, int *indiceRegulador, DADOSREGULADOR *dadosRegulador, MATRIZMAXCORRENTE * maximoCorrenteParam*/)
{
    long int nmed,nvar,nmedTotal; // declara as variáveis auxiliares
    int i,j, idAlim, it, n_threads;
    TF_GRAFO *no  = NULL;
    extern BOOL iteration_multiple_level_OPT;
    BOOL flag_niveis[numeroAlimentadores]; // flag dos alimentadores que devem ser rodados
    int level_iteration =0; 
    
    BOOL tap_modified = true;      // variable that checks if there is a change on tap position on the upper level of power flow
    BOOL runPF_alimentador[numeroAlimentadores];  // inicia variável informando que todos os alimentadores que vão rodar o fluxo
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++) runPF_alimentador[idAlim] = true; // inicia variável informando que todos os alimentadores que vão rodar o fluxo
    

    //inicializa as variaveis que retonam os resultados
    //TF_PFSOLUTION powerflow_result[numeroAlimentadores];


    




    //declara aquivos de saida
    
    FILE *arquivo;
    arquivo = fopen("tempos_alimentadores.txt","w+");
    double tempoAUX;
    
    //variaveis para calculo de tempo de execução
    clock_t t1 = clock(); 
    
    double tIni1 = omp_get_wtime(); 
    double tIni;
    double tEnd;

    double tol = 0.000001;
    //Calculo de fluxo de potência para todos os alimentadores individualmente
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
                if (opt_flow) (*powerflow_result_alim)[idAlim] = fluxoPotencia_BFS_Alimentador_tf(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase,configuracoesParam,dadosAlimentadorParam,indiceConfiguracao,matrizB);
                
                else (*powerflow_result_alim)[idAlim] = fluxoPotencia_BFS_Alimentador_IteracaoUnica(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                
                runPF_alimentador[idAlim] = false;     // finished calculating power flow on the feeder
                
                fprintf(arquivo,"\n %d \t13.8\t%ld\t%d\t%.12lf\t%lf",i,alimentadores[idAlim].numeroNos,(*powerflow_result_alim)[idAlim].iteracoes,tempoAUX,(*powerflow_result_alim)[idAlim].menorTensao );
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
                if (opt_flow) (*powerflow_result_alim)[idAlim] = fluxoPotencia_BFS_Alimentador_tf(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase,configuracoesParam,dadosAlimentadorParam,indiceConfiguracao,matrizB);
                else (*powerflow_result_alim)[idAlim] = fluxoPotencia_BFS_Alimentador_IteracaoUnica(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                
                runPF_alimentador[idAlim] = false;     // finished calculating power flow on the feeder
                fprintf(arquivo,"\n %d \t34.5\t%ld\t%d\t%.12lf\t%lf",i,alimentadores[idAlim].numeroNos,(*powerflow_result_alim)[idAlim].iteracoes,tempoAUX, (*powerflow_result_alim)[idAlim].menorTensao );

                
            }
            tEnd = omp_get_wtime(); 
            tempoAUX = tEnd - tIni;
            
            //verfica a mudança de tap, a partir do power_flow result se mudou o tap em algum dos alimentadores
            //-----------------------------------------------------------------------
            // for tap changing control at upper level   (check only at upper levels)
            if ((*powerflow_result_alim)[idAlim].tap_change_flag && iteration_multiple_level_OPT){
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
        
    //caculos de resultados
    (*powerflow_result_rede )= compilaResultadosRede(*powerflow_result_alim,numeroAlimentadores,alimentadores,grafo,numeroBarras,Sbase,numeroInterfaces,interfaceNiveis);

}


//Compila os resultados do Fluxo de Potência em todos os alimentadores para gerar os resultados totais da rede
/**
 * @brief Função auxiliar para calcular os resultados gerais da rede elétrica, a partir dos resultados de cada alimentador, recebe como entrada @p powerflow_result vetor de TF_PFSOLUTION
 * do tipo TF_PFSOLUTION com os resultados do fluxo de potencia por alimentador  a estrutura @p alimentadores do tipo TF_ALIMENTADOR com as informações trifásicas dos alimentadores, @p numeroAlimentadores um inteiro para inteiro com o número de alimentadores da rede
 * @p grafo do tipo TF_GRAFO, com as informacoes elétricas da rede, respeiando a configuacao topologica da RNP de setores @p numeroInterfaces inteiro com o número de interfaces entre niveis de tensão
 * @p interfaceNiveis matriz de inteiros para realizar a interface da rede , retorna uma estrutura do tipo TF_PFSOLUTION com as informacoes totais da rede. 
 * @param powerflow_result com os resultados do fluxo de potência
 * @param alimentadores alimentadores da rede elétrica
 * @param numeroAlimentadores numero de alimentadores da rede elétrica
 * @param grafo grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param interfaceNiveis 
 * @param numeroInterfaces
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * 
 **/

TF_PFSOLUTION compilaResultadosRede (TF_PFSOLUTION *powerflow_result,int numeroAlimentadores, TF_ALIMENTADOR *alimentadores ,TF_GRAFO *grafo, int numeroBarras ,double Sbase,
long int numeroInterfaces, long int **interfaceNiveis )
{
    int id_menorV, id_maxCar, id_maxCurNeutro, id_maxQueda, id_itmax, id_maxDesbalanco, id_maxDesbalancoCorrente, id_maxDesbalancoCorrenteAlim;
    int idAlim;
    
    TF_PFSOLUTION powerflow_result_rede;

    powerflow_result_rede.convergencia = 0;
    powerflow_result_rede.maiorCarregamentoCorrente = 0;
    powerflow_result_rede.perdasResistivas = 0;
    powerflow_result_rede.maiorCarregamentoTrafo = 0;
    powerflow_result_rede.carregamentoRede = 0;
    powerflow_result_rede.menorTensao = 100000;
    powerflow_result_rede.quedaMaxima = 0;
    powerflow_result_rede.iteracoes = 0;
    powerflow_result_rede.correnteNeutroSE =0;
    powerflow_result_rede.carregamentoRedeABC[0] = 0;
    powerflow_result_rede.carregamentoRedeABC[1] = 0;
    powerflow_result_rede.carregamentoRedeABC[2] = 0;
    powerflow_result_rede.menorTensaoABC[0] = 0;
    powerflow_result_rede.menorTensaoABC[1] = 0;
    powerflow_result_rede.menorTensaoABC[2] = 0;
    // powerflow_result[idAlim].maiorCarregamentoPotencia =0;


    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
      
        powerflow_result_rede.perdasResistivas += powerflow_result[idAlim].perdasResistivas;
        powerflow_result_rede.carregamentoRede += powerflow_result[idAlim].carregamentoRede;
        powerflow_result_rede.carregamentoRedeABC[0] += powerflow_result[idAlim].carregamentoRedeABC[0];
        powerflow_result_rede.carregamentoRedeABC[1] += powerflow_result[idAlim].carregamentoRedeABC[1];
        powerflow_result_rede.carregamentoRedeABC[2] += powerflow_result[idAlim].carregamentoRedeABC[2];
        
        //Atualiza as Interfaces das SEs de 34.5 kV com a carga calculada em 13.8 kV
        
        if (powerflow_result[idAlim].iteracoes > powerflow_result_rede.iteracoes){
            id_itmax = idAlim;
            powerflow_result_rede.iteracoes = powerflow_result[idAlim].iteracoes;// numero de iteracoes
        }

        if (creal(powerflow_result[idAlim].maiorCarregamentoPotencia)>creal(powerflow_result_rede.maiorCarregamentoPotencia))
        {
            powerflow_result_rede.maiorCarregamentoPotencia=powerflow_result[idAlim].maiorCarregamentoPotencia;
        }



        if (powerflow_result[idAlim].maiorCarregamentoCorrente > powerflow_result_rede.maiorCarregamentoCorrente){
            id_maxCar = idAlim;
            powerflow_result_rede.maiorCarregamentoCorrente = powerflow_result[idAlim].maiorCarregamentoCorrente; // maior carregamento de corrente
        }
        if (powerflow_result[idAlim].correnteNeutroSE > powerflow_result_rede.correnteNeutroSE){
            id_maxCurNeutro = idAlim;
            double Ibase =  Sbase / (sqrt(3) * grafo[alimentadores[idAlim].idRaiz].Vbase);
            powerflow_result_rede.correnteNeutroSE = powerflow_result[idAlim].correnteNeutroSE * Ibase; // maior corrente de neutro das SE da rede
        }

        if (powerflow_result[idAlim].menorTensao < powerflow_result_rede.menorTensao){ // menor tensão da rede
            id_menorV = idAlim;
            powerflow_result_rede.menorTensao = powerflow_result[idAlim].menorTensao;
        }
        if (powerflow_result[idAlim].quedaMaxima > powerflow_result_rede.quedaMaxima){ //maior queda de tensao da rede
            id_maxQueda = idAlim;
            powerflow_result_rede.quedaMaxima = powerflow_result[idAlim].quedaMaxima;
        }

        if (powerflow_result[idAlim].desbalancoTensaoMaximo > powerflow_result_rede.desbalancoTensaoMaximo){ // maior desbalanço da rede
            id_maxDesbalanco = idAlim;
            powerflow_result_rede.desbalancoTensaoMaximo = powerflow_result[idAlim].desbalancoTensaoMaximo;
        }

        if (powerflow_result[idAlim].desbalancoCorrenteMaximo > powerflow_result_rede.desbalancoCorrenteMaximo){ // maior desbalanço de corrente da rede
            id_maxDesbalancoCorrente = idAlim;
            powerflow_result_rede.desbalancoCorrenteMaximo = powerflow_result[idAlim].desbalancoCorrenteMaximo;
        }

        if (powerflow_result[idAlim].desbalancoCorrenteAlim> powerflow_result_rede.desbalancoCorrenteAlim){
            id_maxDesbalancoCorrenteAlim = idAlim;
            powerflow_result_rede.desbalancoCorrenteAlim = powerflow_result[idAlim].desbalancoCorrenteAlim;
        } 
            
    }
    for (int idInterfaces = 0; idInterfaces < numeroInterfaces; idInterfaces++){ // carregamento total da rede por fase e trifasico
    if (interfaceNiveis[idInterfaces][2] == 0){ //Para mútliplos níveis, até o momento somente dois
        idAlim = grafo[interfaceNiveis[idInterfaces][0]].idAlim;
        powerflow_result_rede.carregamentoRede -= powerflow_result[idAlim].carregamentoRede;
        powerflow_result_rede.carregamentoRedeABC[0] -= powerflow_result[idAlim].carregamentoRedeABC[0];
        powerflow_result_rede.carregamentoRedeABC[1] -= powerflow_result[idAlim].carregamentoRedeABC[1];
        powerflow_result_rede.carregamentoRedeABC[2] -= powerflow_result[idAlim].carregamentoRedeABC[2];
    }
    }
    powerflow_result_rede.carregamentoRede = powerflow_result_rede.carregamentoRedeABC[0] + powerflow_result_rede.carregamentoRedeABC[1] + powerflow_result_rede.carregamentoRedeABC[2];
    

    printf("Numero maximo de iteracoes: %d  (alim: %d  / %ld)\n", powerflow_result_rede.iteracoes, id_itmax,numeroAlimentadores);
    printf("Vmin: %lf p.u. (alim: %d)\n", powerflow_result_rede.menorTensao, id_menorV);
    printf("quedaMax: %lf  %%  (alim: %d)\n", powerflow_result_rede.quedaMaxima,id_maxQueda);
    printf("maior carregamentoCorrente: %lf %%  (alim: %d)\n", powerflow_result_rede.maiorCarregamentoCorrente,id_maxCar);
    printf("carregamentoRede: %lf kW\n", powerflow_result_rede.carregamentoRede * Sbase);
    printf("carregamentoRede por fase: %.2lf kW \t %.2lf kW\t %.2lf kW\n", powerflow_result_rede.carregamentoRedeABC[0] * Sbase, powerflow_result_rede.carregamentoRedeABC[1] * Sbase, powerflow_result_rede.carregamentoRedeABC[2] * Sbase);
    printf("perdas totais: %lf kW\n", powerflow_result_rede.perdasResistivas * Sbase);
    printf("maximo desbalanco de tensão: %lf %%  (alim: %d)\n", powerflow_result_rede.desbalancoTensaoMaximo *100, id_maxDesbalanco);
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
    

    return powerflow_result_rede;
}



//Cálculo de Fluxo de Potência em dois alimentadores os alimentadores 
/**
 * @brief Função auxiliar para o cálculo de fluxo de potência trifásico em dois alimentadores específicos via método de Varredura Direta/Inversa através da RNP de setores,
 * , realiza a verradura inicialmente nos alimentadores de 13.8kV, realiza depois nos alimentadores de 34.5kV com a carga do alimentador anterior refletida nele
 * Verifica se há mudança de tap, caso positivo, reinicia o calculo do fluxo nos alimentadores de 13.8kV utiliza as informacoes da rede na estrutura @p grafo do tipo TF_GRAFO, com as informacoes elétricas da rede, respeiando a configuacao topologica disponível na 
 * @p configuracoesParam ponteiro do tipo CONFIGURACAO com a configuracao da rede, @p matrizB ponteiro do tipo RNPSETORES com a RNP de setores da rede  
 * Essa função efetua
 * A função retorna @c TF_PFSOLUTION.
 * 
 * @param grafo_tf grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param numeroBarras_tf quatidade total de barras
 * @param alimentadores alimentadores da rede elétrica
 * @param interfaceNiveis 
 * @param numeroInterfaces
 * @param opt_flow
 * @param ramos vetor do tipo TF_DRAM com as informações dos ramos da rede
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param dadosAlimentadorParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * 
 * */
void fluxoPotencia_alimentador_P_A_tf(TF_GRAFO *grafo_tf ,long int numeroBarras_tf, TF_ALIMENTADOR *alimentador_tf, TF_DRAM *ramos_tf,double Sbase,
        /*int numeroBarrasParam,*/ CONFIGURACAO *configuracoesParam, 
        /*double VFParam,*/ DADOSALIMENTADOR *dadosAlimentadorParam ,int indiceConfiguracao, RNPSETORES *matrizB, long int RNP_P, long int RNP_A, TF_PFSOLUTION *powerflow_result_rede, TF_PFSOLUTION **powerflow_result_alim/*,
        MATRIZCOMPLEXA *ZParam,*/ /* MATRIZMAXCORRENTE *maximoCorrenteParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulado*/,
long int numeroInterfaces, long int **interfaceNiveis )
{
        long int iniAlim,noS,noR,root;
        long int noProf[1000];


     

        RNPSETOR rnpSetorSR;


        //determina o no raiz do alimentador P
        iniAlim=configuracoesParam[indiceConfiguracao].rnp[RNP_P].nos[0].idNo;    
        noProf[configuracoesParam[indiceConfiguracao].rnp[RNP_P].nos[0].profundidade] = iniAlim;
        noS = configuracoesParam[indiceConfiguracao].rnp[RNP_P].nos[1].idNo;
        noR = noProf[configuracoesParam[indiceConfiguracao].rnp[RNP_P].nos[1].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
        //No raiz do alimentador
        root=rnpSetorSR.nos[0].idNo;   
        
        grafo_tf[root-1].V[0]=grafo_tf[root-1].barra->Vinicial[0];// inicializa tensão do no raiz 
        grafo_tf[root-1].V[1]=grafo_tf[root-1].barra->Vinicial[1];
        grafo_tf[root-1].V[2]=grafo_tf[root-1].barra->Vinicial[2];

        inicializaTensaoSDR_alimentador_tf(grafo_tf,numeroBarras_tf,alimentador_tf,numeroAlimentadores,configuracoesParam,matrizB,RNP_P,indiceConfiguracao,root);

        (*powerflow_result_alim)[RNP_P]=fluxoPotencia_BFS_Alimentador_tf(grafo_tf, numeroBarras_tf, alimentador_tf[RNP_P], ramos_tf, Sbase,configuracoesParam,dadosAlimentadorParam,indiceConfiguracao,matrizB);
        
        // determina o no raiz do alimentador A
        iniAlim=configuracoesParam[indiceConfiguracao].rnp[RNP_A].nos[0].idNo;    
        noProf[configuracoesParam[indiceConfiguracao].rnp[RNP_A].nos[0].profundidade] = iniAlim;
        noS = configuracoesParam[indiceConfiguracao].rnp[RNP_A].nos[1].idNo;
        noR = noProf[configuracoesParam[indiceConfiguracao].rnp[RNP_A].nos[1].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
        //No raiz do alimentador
        root=rnpSetorSR.nos[0].idNo;   
        
        grafo_tf[root-1].V[0]=grafo_tf[root-1].barra->Vinicial[0];// inicializa tensão do no raiz 
        grafo_tf[root-1].V[1]=grafo_tf[root-1].barra->Vinicial[1];
        grafo_tf[root-1].V[2]=grafo_tf[root-1].barra->Vinicial[2];

        inicializaTensaoSDR_alimentador_tf(grafo_tf,numeroBarras_tf,alimentador_tf,numeroAlimentadores,configuracoesParam,matrizB,RNP_A,indiceConfiguracao,root);        
        (*powerflow_result_alim)[RNP_A]=fluxoPotencia_BFS_Alimentador_tf(grafo_tf, numeroBarras_tf, alimentador_tf[RNP_P], ramos_tf, Sbase,configuracoesParam,dadosAlimentadorParam,indiceConfiguracao,matrizB);

        (*powerflow_result_rede)=compilaResultadosRede((*powerflow_result_alim),numeroAlimentadores,alimentador_tf,grafo_tf,numeroBarras_tf,Sbase,numeroInterfaces,interfaceNiveis);

        


}

//Cálculo de Calculo do Carregamento dos transformadores da subestação
/**
 * @brief Função auxiliar para o cálculo do carregamento dos transformadores da subestação baseado no resultado do fluxo de potência trifásico e na configuração da rede.
 * recebe como parâmetros a estrutura @p alimentador_tf do tipo TF_ALIMENTADOR com as informações trifásicas dos alimentadores, @p numeroalimetadores_tf um inteiro para inteiro com o número de alimentadores da rede
 * @p numerotrafosSE com o número de transformadores de subestação @p configuracoesParam uma estrutura do tipo CONFIGURACAO com as informações sumarizadas de cada um dos alimentadores 
 * @p idNovaConfiguracaoParam  inteiro com a identificação da configuracao atual @p idAntigaConfiguracaoParam inteiro com a identificação da configuracao antiga @p todosAlimentadores
 * boleano que indica se deve ser realizado em um alimentador ou em todos @p idRNPOrigem indice da RNP de origem, @p idRNPDestino indice da RNP de destino
  
 * @param alimentador_tf alimentadores da rede elétrica
 * @param numeroalimetadores_tf numero de alimentadores da rede trifasica
 * @param numerotrafosSE numero de transformadores de subestação da rede trifásica
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param idNovaConfiguracaoParam inteiro com o indice da configuracao atual 
 * @param idAntigaConfiguracaoParam inteiro com o indice da configuracao anterior
 * @param todosAlimentadores boleano que indica se será feito em todos os alimentadores
 * @param idRNPOrigem indice da RNP de origem
 * @param idRNPDestino indice da RNP de destino
 *  @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * 
 * */

void carregamentoTrafo_tf(TF_ALIMENTADOR *alimentador_tf, long int numeroalimetadores_tf, long int numerotrafosSE,
        CONFIGURACAO *configuracoesParam, int idNovaConfiguracaoParam, 
        int idAntigaConfiguracaoParam, BOOL todosAlimentadores, int idRNPOrigem, int idRNPDestino)
{
  int indiceI,indiceJ;
  int idTrafo;
  double potencia;
  double carregamentoTrafo;
  carregamentoTrafo=0;
//realiza o cálculo para todos os alimentadores
    if (todosAlimentadores) {
        for(indiceI = 1; indiceI <= numerotrafosSE; indiceI++)
        {
            configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI] = 0+I*0;
        }
        for(indiceJ = 1; indiceJ<=numeroalimetadores_tf; indiceJ++) // calcula a potencia de cada trafo
        {
            idTrafo = alimentador_tf[indiceJ-1].DTRFSE->idTrafoSE; // conferir esse id e fazer testes de validação
            //a sequencia de rnps obedece a mesma sequencia de alimentadores com a diferença de uma posição 
            configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1] += configuracoesParam[idNovaConfiguracaoParam].rnp[indiceJ-1].fitnessRNP.potenciaAlimentador;
        }
        for (indiceI = 1; indiceI <= numeroalimetadores_tf; indiceI++) //verifica se o trafo está sobrecarregado
         {     
           idTrafo = alimentador_tf[indiceJ-1].DTRFSE->idTrafoSE;  
           potencia = cabs(configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1]) / (alimentador_tf[indiceI-1].DTRFSE->Snom* 10000); //(10000=1MVA/100) em porcentagem
            if (carregamentoTrafo < potencia)
                carregamentoTrafo = potencia;
          //
        // printf("capacidade %.2lf potencia do trafo  %lf carregamento %.2lf \n", dadosTrafoParam[indiceI].capacidade, cabs(configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI])/1000000, potencia);
        } 
        
        configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = carregamentoTrafo;
        
    }
    else //atualiza os valores considerando apenas os alimentadores que tiveram modificação
    {
        configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = configuracoesParam[idAntigaConfiguracaoParam].objetivo.maiorCarregamentoTrafo;
            
        // faz para a RNPORIGEM
        idTrafo = alimentador_tf[idRNPOrigem].DTRFSE->idTrafoSE;
        configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1] = configuracoesParam[idAntigaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1];
        //verifica se o alimentador da RNP do alimentador de origem pertence a esse trafo. O indice do alimentador é o indice do vetor da rnp acrescido de 1. 

        configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1] -= configuracoesParam[idAntigaConfiguracaoParam].rnp[idRNPOrigem].fitnessRNP.potenciaAlimentador;
        configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1] += configuracoesParam[idNovaConfiguracaoParam].rnp[idRNPOrigem].fitnessRNP.potenciaAlimentador;
        
        potencia = cabs(configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1]) / (alimentador_tf[idRNPOrigem].DTRFSE[idTrafo].Snom * 10000);
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo < potencia)
        configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = potencia;

        //verifica se o alimentador da RNP do alimentador de destino pertence a esse trafo. O indice do alimentador é o indice do vetor da rnp acrescido de 1.
        idTrafo = alimentador_tf[idRNPDestino].DTRFSE->idTrafoSE;
        configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1] = configuracoesParam[idAntigaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo];

        configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1] -= configuracoesParam[idAntigaConfiguracaoParam].rnp[idRNPDestino].fitnessRNP.potenciaAlimentador;
        configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1] += configuracoesParam[idNovaConfiguracaoParam].rnp[idRNPDestino].fitnessRNP.potenciaAlimentador;
        
        //calcula a potencia que está sendo utilizada em porcentagem da capacidade

        potencia = cabs(configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo+1]) / (alimentador_tf[idRNPDestino].DTRFSE[idTrafo].Snom * 10000);
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo < potencia)
        configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = potencia;
            
    }
}



//
/**
 * @brief Função para integrar as informações dos dados dos alimentadores e da subestação em cada alimentador 
 *
 * Essa função recebe como parâmetro o @p *DTRFSE do tipo TF_DTRFSE, com as informações dos transformadores da subestação, @p DALIM do tipo TF_DALIM, com as informações do arquivo DALIM.csv
 * o @p numeroTFSES um inteiro com o numero de trafos de subestação, @p numeroDALIM um inteiro com número de alimentadores no arquivo DALIM.csv, @p **alimentador_tf estrutura com os dados dos
 * alimentadores trifásicos e que será preenchida
 * @param DTRFSE Estrutura do tipo TF_DTRFSE com as informações dos transformadores das subestações
 * @param  DALIM Estrutura do tipo TF_DALIM com as informações sobre os alimentadores disponivilizadas no arquivo DALIM.csv
 * @param numeroTFSES Inteiro com o número de transformadores na subestação
 * @param numeroDALIM Inteiro com o número de alimentadores no arquivo DALIM
 * @param alimentadores alimentadores da rede elétrica
 * @return void.
 * @note
 * @warning .
 */

void trafoSB_info(TF_DTRFSE *DTRFSE,TF_DALIM *DALIM, long int numeroTFSES,long int numeroDALIM, long int numeroAlim_tf ,TF_ALIMENTADOR **alimentador_tf)
{

    long int i,j;

    for (i=0;i<numeroDALIM;i++)
    {
        
        for (j=0;j<numeroAlim_tf;j++)
        {
            if(DALIM[i].noRaiz==(*alimentador_tf)[j].noRaiz)
            {
                (*alimentador_tf)[j].dalim=&DALIM[i];
                break;
            }
        }
    }

    for (i=0;i<numeroTFSES;i++)
    {
        
        for (j=0;j<numeroAlim_tf;j++)
        {
            if(DTRFSE[i].idTrafoSE==(*alimentador_tf)[j].dalim->ID_TR)
            {
                (*alimentador_tf)[j].DTRFSE=&DTRFSE[i];
                break;
            }
        }
    }

}