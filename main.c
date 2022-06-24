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
    long int estampa_tempo = 0;
    long int numeroBarras_tf = 0;
    long int numeroAlimentadores_tf = 0;
    long int numeroAreas_tf = 0, numeroSubestacoes_tf = 0;
    long int numeroRamos_tf = 0;
    long int numeroNos=0;
    long int **numeroMedidas = NULL,**numeroMedidas_PMU_tf = NULL,**numeroVirtuais_tf = NULL;
    long int **interfaceNiveis_tf = NULL;
    long int numeroInterfaces_tf = 0;
    long int i = 0;
    char *folder = NULL;
    long int numeroTrafos=0;//dados da MRAN
    long int numeroChaves=0;//dados da MRAM
    extern int numeroSetores; //dados da MRAM
    
    // Estruturas de dados da rede elétrica
    TF_DBAR *barra_tf = NULL;
    TF_DRAM *ramo_tf = NULL;
    TF_DSUBESTACAO *subestacoes_tf = NULL;
    TF_DALIM *static_alimentador_tf = NULL;     // salva dados estáticos para relacionar alimentadores com subestações
    TF_GRAFO *grafo_tf = NULL;
    TF_DMED *medida_tf = NULL;
    TF_ALIMENTADOR *alimentador_tf = NULL, *areas_tf = NULL;    // A estrutura alimentador guarda as conectividade de alimentadores (circuito completo)

    TF_AREA_MEDICAO *areasMedicao_tf = NULL;
    
    // compatibilizacao com a MRAM
    GRAFO *grafoSDRParam=NULL;//DEIXAR SÓ SDR
    GRAFOSETORES *grafoSetoresSDR=NULL;
    RNPSETORES *rnpSetoresSDR=NULL;
    DADOSREGULADOR *dadosReguladorSDRParam=NULL;
    DADOSALIMENTADOR *dadosAlimentadorSDRParam=NULL;
    LISTACHAVES *listaChavesSDR=NULL;
    CONFIGURACAO *configuracaoInicialSDR=NULL;
    long int idConfiguracaoSDR=0;
    // Para estimação de demanda e simulações com séries temporais
    TF_CURVA_TRAFO *curvasTrafos = NULL;       //Curva de carga dos trafos de distribuição agregadas
    long int flag_mod = 0;                  //FLAG para rodar a modelagem de cargas

    // --------------------------------------------------------------------------------
    //Os arquivos lidos são:
    // config.txt               : Pasta com arquivos da rede elétrica e curvas agregadas
    //              - 3F_DBAR.csv  : Dados de barras da rede elétrica
    //              - 3F_DSHNT.csv : Dados de bancos de capacitor da rede elétrica
    //              - 3F_DLIN.csv  : Dados de ramais trifásicos da rede elétrica
    //              - 3F_DTRF.csv  : Dados de transformadores da rede elétrica
    //              - 3F_DREG.csv  : Dados de reguladores de tensão da rede elétrica
    //              - 3F_DSWTCH.csv: Dados de chaves da rede elétrica
    //
    //              Arquivos opcionais:
    //              - DINTERSE.csv : Dados de Interfaces entre dois níveis (ex. 34.5 p/ 13.8kV  e estações de chaves - 34.5/34.5)
    //
    // config_sase.txt         : Pasta com arquivos de dados de automação
    //              - 3F_DSTAT.csv : dados atualizados de medidas de estado, status de chaves ou tap de reguladores provenientes do SCADA ou
    //              - 3F_DMED.csv  : dados atualizados de medidas analógicas  provenientes do SCADA
    //
    
    //Saída
    // summary_powerflow.dad    : resultados sumarizados de fluxo de potência convergido
    // log_edtr.txt             : arquivo de log do estimador de demanda
    // DBAR.csv                 : resultados de estimação de demanda - Potência ativa e reativa trifásica nas barras de carga
    //
    // --------------------------------------------------------------------------------
    
    // Leitura dos dados da rede elétrica - Assume que já identação de barras está pré-mapeado e ordenado de 0 até NB corretamente
    clock_t a1 = clock();
    folder = leituraDados(&barra_tf,&ramo_tf,&numeroBarras_tf,&numeroRamos_tf,&numeroAlimentadores_tf);
    
    // Leitura de Interface entre Níveis de Tensão - Subestações de 34.5 / 13.8 kV
    interfaceNiveis_tf = leituraDINTERSE(folder,"DINTERSE.csv", &numeroInterfaces_tf, &barra_tf,&numeroBarras_tf,&ramo_tf,&numeroRamos_tf);
    
    clock_t a2 = clock();
    double tempo_A = (double)(a2 - a1) / CLOCKS_PER_SEC;
    printf("\nTempo Leitura: %lf\n\n",  tempo_A);
    
    
    // Cria estrutura de dados da rede elétrica
    geraGrafo(&grafo_tf, barra_tf, numeroBarras_tf,ramo_tf,numeroRamos_tf); 
    
 // Suprime barras de passagem - Aceleração de cálculo
 

    
    //--------------------------------------------------------------------------
    // Compatibilização com MRAN
    converteGrafo_TFtoSDR(grafo_tf,numeroBarras_tf,ramo_tf,numeroRamos_tf,&grafoSDRParam,&dadosReguladorSDRParam,&numeroNos, &numeroTrafos, &numeroChaves);
    converteDadosAlimentadores_TFtoSDR(grafo_tf,numeroBarras_tf,numeroAlimentadores_tf,&dadosAlimentadorSDRParam);
    // converteDadosAlimentadores_TFtoSDR_alt(alimentador_tf,numeroAlimentadores_tf,&dadosAlimentadorSDRParam);
    //--------------------------------------------------------------------------
    // atualizaEstadoChaves(grafoSDR,numeroBarras); 
    
    //Atualiza os estados das chaves de acordo com ESTADOS_CHS    
    LISTASETORES * lista_setores = buscaSetores(grafoSDRParam, dadosAlimentadorSDRParam, numeroBarras_tf);
    constroiListaChaves(grafoSDRParam, &listaChavesSDR, numeroChaves, numeroBarras_tf);
    constroiListaAdjacenciaSetores(&grafoSDRParam, lista_setores, &listaChavesSDR, &grafoSetoresSDR, &numeroChaves, &numeroNos);
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
    //gravaBarrasRT(dadosReguladorSDRParam, numeroReguladores, grafoSDRParam, numeroBarras);
    //gravaDadosTrafo(numeroTrafos, dadosTrafoSDR);

    //==============Termino Compatibilizacao======================


    // Cria as listas encadeadas radiais dos alimentadores
    buscaProfundidadeAlimentadores(grafo_tf, numeroBarras_tf, &alimentador_tf, numeroAlimentadores_tf); 
    //    for(i=0; i<numeroAlimentadores; i++) supressaoBarrasPassagem(grafo_tf, alimentador,i, ramo_tf);

    // Transforma em pu e cria matrizes de admitância iniciais
    calculaPU(grafo_tf, numeroBarras_tf, ramo_tf, numeroRamos_tf, Sbase);
    
    //Atualiza a posição de taps
    atualizaTaps(ramo_tf, numeroRamos_tf); //terminar o tap fixo de trafos de potência

    //gravaBarrasRT(dadosReguladorSDRParam, numeroReguladores, grafoSDRParam, numeroBarras);
    //gravaDadosTrafo(numeroTrafos, dadosTrafoSDR);

    
    //--------------------------------------------------------------------------
    // Cálculo de Fluxo de Potência Trifásico
    
    clock_t a3 = clock();
    double tempo_B = (double)(a3 - a2) / CLOCKS_PER_SEC;
    printf("\nTempo Processamento Modelo: %lf\n\n",  tempo_B);
    
    //Simulacoes com Curvas de Carga - Considera o valor de P e Q de arquivo de curva previamente agregada
    if (flag_mod == 3){ 
        leituraCurvasAgregadas(folder, &curvasTrafos,barra_tf,numeroBarras_tf); //Leitura de curvas de cargas agregadas
        estampa_tempo  = 10;
        inicializaPQcurvas(barra_tf, curvasTrafos, numeroBarras_tf,  estampa_tempo, VALOR_ESPERADO, Sbase); 
    }         
    
    // Fluxo de Potência via Varredura Direta/Inversa Trifásica por Níveis de Tensão
    //           O fluxo de potência está como cargas em corrente constante devido à implementação da COPEL
    //incializaTensoesVarredura(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores_tf);
    
    inicializaTensaoSDR_alimentadores_tf(grafo_tf,numeroBarras_tf,alimentador_tf,numeroAlimentadores_tf,1,configuracaoInicialSDR,rnpSetoresSDR,idConfiguracaoSDR);

        //Impressão de resultados em arquivos
    int ppt_aux = 0;
    for (int idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
            if (ppt_aux == 0){
                salvaTensoesNodais("stateVT.txt","w+",alimentador_tf[idAlim],grafo_tf);
                salvaCorrentesRamos("loadingI.txt", "w+", alimentador_tf[idAlim],grafo_tf, numeroBarras_tf, Sbase);
                ppt_aux=1;
            }
            else{
                salvaTensoesNodais("stateVT.txt","a+",alimentador_tf[idAlim],grafo_tf);
                salvaCorrentesRamos("loadingI.txt", "a+", alimentador_tf[idAlim],grafo_tf, numeroBarras_tf, Sbase);
            }
    }

    clock_t start2 = clock();
    
    
    fluxoPotencia_Niveis_BFS_Multiplos_tf(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores_tf, ramo_tf, Sbase/1000, interfaceNiveis_tf, numeroInterfaces_tf, true,dadosAlimentadorSDRParam,grafoSDRParam,configuracaoInicialSDR,idConfiguracaoSDR,rnpSetoresSDR);
    // fluxoPotencia_Niveis_BFS_Multiplos(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores_tf, ramo_tf, Sbase/1000, interfaceNiveis_tf, numeroInterfaces_tf, true); 
    // true converge FP | false faz iteraçao única (P&D)
    clock_t end2 = clock();
    double edtr_time = (double)(end2 - start2) / CLOCKS_PER_SEC;
    printf("\nNumero Alimentadores: %d \t Numero Barras: %d \t\t\n\n", numeroAlimentadores_tf, numeroBarras_tf);
    printf("\nTempo: %lf\n\n",  edtr_time);

    
    //--------------------------------------------------------------------------
    // Tempo Real
    //               Estimação de Demanda
    
    
    // Leitura do arquivo DSTAT  e atualiza a topologia da rede elétrica
    // leitura do arquvio e atualizaçao
    
    //Leitura do Arquivo DTAPS
    // leitura do arquvio e atualizaçao  de taps
    
    
    
//    // Leitura de Medidas do sistema SCADA (SASE processado);
//    numeroMedidas_tf = leituraMedidas(folder, "DMED.csv", &medida, ramo_tf, numeroRamos_tf, barra_tf, numeroBarras, grafo, Sbase); 
//    
//    // Criação de Áreas de Medição para o Estimador de Demandas Trifásicas
//    buscaAMs(grafo, numeroBarras, alimentador_tf, numeroAlimentadores, medida, numeroMedidas_tf, &areasMedicao_tf);
//    
//    //Estimador de Demandas Trifásicas
//    estimadorDemandaTrifasico(grafo, numeroBarras, alimentador_tf, numeroAlimentadores, ramo_tf, Sbase/1000, interfaceNiveis, numeroInterfaces, areasMedicao_tf);;;;;
    
        
    //Liberação de Memória
    free_BARRA(barra_tf,numeroBarras_tf);
    free_DRAM(ramo_tf,numeroRamos_tf);
    //free_GRAFO(grafo,numeroBarras);
    free_ALIMENTADOR(alimentador_tf,numeroAlimentadores_tf);
    //free(dadosReguladorSDR);
    //free(dadosAlimentadorSDR);
    //free(dadosTrafoSDR);
    //desalocaGrafoSDR(grafoSDR, numeroBarras);
    //free(listaChaves);
    return (EXIT_SUCCESS);
}

