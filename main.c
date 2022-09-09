/**
 * @file main.c
 * @author Julio Augusto Druzina Massignan
 * @date 31 jul 2020
 * @brief Programa principal para o cálculo de fluxo de potência trifásico
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#include "data_structures.h"
#include "data_structures_tf.h"
#include "funcoesLeitura_tf.h"
#include "funcoesTopologia_tf.h"
#include "funcoesFluxoVarredura_tf.h"
#include "funcoesIntegracao_tf.h"

#include "data_structures_modcarga_tf.h"
#include "funcoesModCarga_tf.h"

// bibliotecas originais GRNP
#include "funcoesSetor.h"
#include "funcoesInicializacao.h"
#include "funcoesRNP.h"
#include "funcoesAuxiliares.h"

// bibliotecas nowcasting

#include "funcoesNowCasting.h"




// Variáveis globais de modelo trifásico
BOOL symetric_OPT = 0; //Variável global para assumir transposição de linhas

// Variáveis globais de controladores locais
BOOL control_REG_OPT = 1; //Variável global para ativar controle automático de taps em reguladores de tensão
BOOL control_CAP_OPT = 0; //Variável global para ativar controle automático de bancos de capacitores chaveados

// Variáveis globais de múltiplo níveis 
BOOL iteration_multiple_level_OPT = 1; //Variável global para ativar iterações subsequentes entre mútiplos níveis de tensão 
BOOL includeVR_INTERSE_OPT = 1; //Variável global para incluir modelo de regulador ideal nas barras com INTERSE

double Sbase = 1000000; //Variável global com a potência base da rede elétrica em VA

/*
 * 
 */
int main(int argc, char** argv) {
    //lista de variáveis do modelo trifásico
    long int numeroBarras_tf = 0;               // numero total de barras do sistemaa
    long int numeroAlimentadores_tf = 0;        // numero total de alimentadores
    long int numeroRamos_tf = 0;                // numero total de ramos                   
    long int **interfaceNiveis_tf = NULL;       // matriz com informações de interface 34,5/13.8
    long int numeroTRFSES_tf=0;                 // numero de transformadores de subestação
    long int numeroDALIM_tf=0;                  // numero de alimentadores agregados em subestações
    long int numeroInterfaces_tf = 0;           // numero de interfaces 34,5/13.8
    char *folder = NULL;                        // ponteiro para local pasta dos arquivos de dados de entrada
 


    // Estruturas de dados da rede elétrica do modelo trifásico
    TF_DBAR *barra_tf = NULL;                   // Estrutura de dados com dados de barras
    TF_DRAM *ramo_tf = NULL;                    // Estrutura de dados com dados de ramos
    TF_GRAFO *grafo_tf = NULL;                  // Estrutura de dados com o grafo da rede elétrica 
    TF_ALIMENTADOR *alimentador_tf = NULL;      // A estrutura alimentador guarda as conectividade de alimentadores (circuito completo)
    TF_DTRFSE *DTRFSES_tf=NULL;                 // Estrutura de dados com dados dos transformadores das subestações
    TF_DALIM *DALIM_tf=NULL;                    // Estrutura de dados com dados de alimentadores
    TF_PFSOLUTION powerflow_result_rede;        // Estrutura de dados com dados de resultados dos fluxos de potência por alimentador
    TF_PFSOLUTION *powerflow_result_alim=NULL;  // Estrutura de dados com dados de resultados gerais do fluxo de potência
    
    //lista de variáveis do modelo MRAN
    long int numeroNos=0;                       
    long int idConfiguracaoAntiga=0;
    long int numeroChaves=0;
    extern int numeroSetores; 
    long int idConfiguracaoSDR=0;

    // compatibilizacao com a MRAM
    GRAFO *grafoSDRParam=NULL;
    GRAFOSETORES *grafoSetoresSDR=NULL;
    RNPSETORES *rnpSetoresSDR=NULL;
    DADOSREGULADOR *dadosReguladorSDRParam=NULL;
    DADOSALIMENTADOR *dadosAlimentadorSDRParam=NULL;
    LISTACHAVES *listaChavesSDR=NULL;
    CONFIGURACAO *configuracaoInicialSDR=NULL;
    
    // variaveis estimador de demadna

    int nmed;
    

    // --------------------------------------------------------------------------------
    //Os arquivos lidos são:
    // config.txt               : Pasta com arquivos da rede elétrica e curvas agregadas
    //              - DBAR.csv  : Dados de barras da rede elétrica
    //              - DSHNT.csv : Dados de bancos de capacitor da rede elétrica
    //              - DLIN.csv  : Dados de ramais trifásicos da rede elétrica
    //              - DTRF.csv  : Dados de transformadores da rede elétrica
    //              - DREG.csv  : Dados de reguladores de tensão da rede elétrica
    //              - DSWTCH.csv: Dados de chaves da rede elétrica
    //              - DALIM.csv : Dados dos alimentadores da rede elétrica
    //              - DTRFSE.csv: Dados dos transformadores das subestações
    //
    //              Arquivos opcionais:
    //              - DINTERSE.csv : Dados de Interfaces entre dois níveis (ex. 34.5 p/ 13.8kV  e estações de chaves - 34.5/34.5)
    //              - Vinicial.csv : Dados das tensões iniciais do fluxo de potência
    
    //Saída
    // summary_powerflow.dad    : resultados sumarizados de fluxo de potência convergido
    // log_edtr.txt             : arquivo de log do estimador de demanda
    // DBAR.csv                 : resultados de estimação de demanda - Potência ativa e reativa trifásica nas barras de carga
    // stateVT.csv              : tensões nodais nas barras dos sistema
    // loadingI.csv             : Carregamento de corrente nas barras do sistema
    // lista_desconexas.dad     : Lista de barras que não estão associadas a nenhum alimentador
    // --------------------------------------------------------------------------------
    
    // Leitura dos dados da rede elétrica - Assume que já identação de barras está pré-mapeado e ordenado de 0 até NB corretamente
    folder = leituraDados(&barra_tf,&ramo_tf,&numeroBarras_tf,&numeroRamos_tf,&numeroAlimentadores_tf);
    
    // Leitura de Interface entre Níveis de Tensão - Subestações de 34.5 / 13.8 kV
    interfaceNiveis_tf = leituraDINTERSE(folder,"DINTERSE.csv", &numeroInterfaces_tf, &barra_tf,&numeroBarras_tf,&ramo_tf,&numeroRamos_tf);
   
    // Leitura dos arquivos opcionais DALIM.csv e DSES.csv
    leituraQualiSE(folder,&numeroTRFSES_tf,&DTRFSES_tf,&numeroDALIM_tf,&DALIM_tf);

    // Cria estrutura de dados da rede elétrica
    geraGrafo(&grafo_tf, barra_tf, numeroBarras_tf,ramo_tf,numeroRamos_tf); 
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //
    //                        Compatibilização com MRAN
    //
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    
    // insere as informacoes do grafo trifásico no grafo da MRAN
    converteGrafo_TFtoSDR(grafo_tf,numeroBarras_tf,ramo_tf,numeroRamos_tf,&grafoSDRParam,&dadosReguladorSDRParam,&numeroNos, &numeroChaves);
    // insere as informacoes da estrutura alimentadores trifásico na alimentadores da MRAN
    converteDadosAlimentadores_TFtoSDR(grafo_tf,numeroBarras_tf,numeroAlimentadores_tf,&dadosAlimentadorSDRParam);
    
    
    LISTASETORES * lista_setores = buscaSetores(grafoSDRParam, dadosAlimentadorSDRParam, numeroBarras_tf);
    constroiListaChaves(grafoSDRParam, &listaChavesSDR, numeroChaves, numeroBarras_tf);
    constroiListaAdjacenciaSetores(&grafoSDRParam, lista_setores, &listaChavesSDR, &grafoSetoresSDR, &numeroChaves, &numeroNos);
    
    // compatibiliza as chaves ficticias criadas na MRAN 
    compatibiliza_chaveSetoresFicticia_tf(&grafo_tf,&barra_tf,&numeroBarras_tf,&ramo_tf,&numeroRamos_tf,grafoSDRParam,numeroNos);

    constroiRNPSetores(lista_setores, grafoSDRParam, &rnpSetoresSDR, numeroBarras_tf); 
    configuracaoInicialSDR = alocaIndividuo(numeroAlimentadores_tf, idConfiguracaoSDR, 1);
    constroiIndividuoInicial(grafoSetoresSDR, grafoSDRParam, listaChavesSDR, dadosAlimentadorSDRParam, configuracaoInicialSDR);
   
    gravaGrafoSetores(grafoSetoresSDR, numeroSetores, listaChavesSDR);
    gravaRNPSetores (rnpSetoresSDR, numeroSetores);
    gravaIndividuo(".dad",configuracaoInicialSDR[idConfiguracaoSDR]);
    imprimeBarrasIsoladas(numeroBarras_tf, grafoSDRParam);
    salvaChaves(numeroChaves,listaChavesSDR);
    salvaChavesCompleto(numeroChaves, listaChavesSDR);
    gravaDadosBarras(numeroBarras_tf, grafoSDRParam);
    salvaLinhas(numeroBarras_tf, grafoSDRParam);
    salvaChavesAMontante(listaChavesSDR, configuracaoInicialSDR[idConfiguracaoSDR],grafoSetoresSDR);

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //
    //                         Termino Compatibilizacao                      
    //
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // Cria as listas encadeadas radiais dos alimentadores
    buscaProfundidadeAlimentadores(grafo_tf, numeroBarras_tf, &alimentador_tf, numeroAlimentadores_tf); 

    // Insere os dados da subestação na estrutura de alimentadores trifásicos
    trafoSB_info(DTRFSES_tf,DALIM_tf,numeroTRFSES_tf,numeroDALIM_tf,numeroAlimentadores_tf,&alimentador_tf);

    // Transforma em pu e cria matrizes de admitância iniciais
    calculaPU(grafo_tf, numeroBarras_tf, ramo_tf, numeroRamos_tf, Sbase);
    
    //Atualiza a posição de taps
    atualizaTaps(ramo_tf, numeroRamos_tf); //terminar o tap fixo de trafos de potência

    
    //--------------------------------------------------------------------------
    // Fluxo de Potência via Varredura Direta/Inversa Trifásica por Níveis de Tensão
    //           O fluxo de potência está como cargas em corrente constante devido à implementação da COPEL
    
    // aloca a estrutura de powerflow result 
    aloca_pfresult(&powerflow_result_alim);

    // função avalia configuracao
    avaliaConfiguracaoSDR_tf(true,&powerflow_result_rede,&powerflow_result_alim,configuracaoInicialSDR,idConfiguracaoSDR,dadosAlimentadorSDRParam,idConfiguracaoAntiga,rnpSetoresSDR,-1,-1,grafo_tf,numeroBarras_tf,alimentador_tf,numeroAlimentadores_tf,ramo_tf,Sbase,interfaceNiveis_tf,numeroInterfaces_tf,true,numeroTRFSES_tf);
        
   // Tempo Real ---------------- Estimação   
    
    // Leitura do arquivo DSTAT  e atualiza a topologia da rede elétrica
    // leitura do arquvio e atualizaçao
    
    //Leitura do Arquivo DTAPS
    // leitura do arquvio e atualizaçao  de taps
    
    // Leitura de Medidas do sistema SCADA (SASE processado)
    TF_DMED *medida_tf=NULL;
    TF_DMED **medidaPrev_tf=NULL;
    TF_DPREV *prev_tf=NULL;
    TF_AREA_MEDICAO *areasMedicao_tf=NULL;

    int numeroAmostras;
    int instante_atual=0;

    // int **numeroMedidas = leituraMedidas(folder, "DMED.csv", &medida_tf, ramo_tf, numeroRamos_tf, barra_tf, numeroBarras_tf, grafo_tf, Sbase); 
    //   Criação de Áreas de Medição para o Estimador de Demandas Trifásicas
    // buscaAMs(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores, medida_tf, numeroMedidas, &areasMedicao_tf);
   // 
    
    int** numeroMedidas=leituraMedidasPrev(folder, "DPREV.csv", &prev_tf,&numeroAmostras, &nmed,ramo_tf, numeroRamos_tf, barra_tf, numeroBarras_tf, grafo_tf, Sbase); 
    // 
    atualiza_dmed(instante_atual,prev_tf,nmed); 
    constroi_dmed_prev(prev_tf,nmed,&medidaPrev_tf);
      
    buscaAMs_NowCasting(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores, medidaPrev_tf, numeroMedidas, &areasMedicao_tf);
        
    //    //Estimador de Demandas Trifásicas
    estimadorDemandaTrifasico(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores, ramo_tf, Sbase, interfaceNiveis_tf, numeroInterfaces_tf, areasMedicao_tf);
    //    salva (curva de carga de demanda estimada, resultados condensados do fluxo de potencia )
    
    //Liberação de Memória
    free_BARRA(barra_tf,numeroBarras_tf);
    free_DRAM(ramo_tf,numeroRamos_tf);
    free_GRAFO(grafo_tf,numeroBarras_tf);
    free_ALIMENTADOR(alimentador_tf,numeroAlimentadores_tf);
    free(dadosReguladorSDRParam);
    free(dadosAlimentadorSDRParam);
    free(DTRFSES_tf);
    free(DALIM_tf);
    desalocaGrafoSDR(grafoSDRParam, numeroBarras_tf);
    free(powerflow_result_alim);
    return (EXIT_SUCCESS);
}

