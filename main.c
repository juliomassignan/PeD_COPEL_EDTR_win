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
#include "funcoesLeitura.h"
#include "funcoesTopologia.h"
#include "funcoesFluxoVarredura.h"

#include "data_structures_modcarga.h"
#include "funcoesModCarga.h"

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
    long int numeroBarras = 0;
    long int numeroAlimentadores = 0, numeroAreas = 0, numeroSubestacoes = 0;
    long int numeroRamos = 0;
    long int **numeroMedidas = NULL,**numeroMedidasPMU = NULL,**numeroVirtuais = NULL;
    long int **interfaceNiveis = NULL;
    long int numeroInterfaces = 0;
    long int i = 0;
    char *folder = NULL;
    
    
    // Estruturas de dados da rede elétrica
    DBAR *barra = NULL;
    DRAM *ramo = NULL;
    DSUBESTACAO *subestacoes = NULL;
    DALIM *static_alimentador = NULL;     // salva dados estáticos para relacionar alimentadores com subestações
    GRAFO *grafo = NULL;
    DMED *medida = NULL;
    ALIMENTADOR *alimentador = NULL, *areas = NULL;    // A estrutura alimentador guarda as conectividade de alimentadores (circuito completo)
    
    AREA_MEDICAO *areasMedicao = NULL;
    

    // Para estimação de demanda e simulações com séries temporais
    CURVA_TRAFO *curvasTrafos = NULL;       //Curva de carga dos trafos de distribuição agregadas
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
    folder = leituraDados(&barra,&ramo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
    
    // Leitura de Interface entre Níveis de Tensão - Subestações de 34.5 / 13.8 kV
    interfaceNiveis = leituraDINTERSE(folder,"DINTERSE.csv", &numeroInterfaces, &barra,&numeroBarras,&ramo,&numeroRamos);
    
    clock_t a2 = clock();
    double tempo_A = (double)(a2 - a1) / CLOCKS_PER_SEC;
    printf("\nTempo Leitura: %lf\n\n",  tempo_A);
    
    
    
    // Cria estrutura de dados da rede elétrica
    geraGrafo(&grafo, barra, numeroBarras,ramo,numeroRamos); 
    
    // Cria as listas encadeadas radiais dos alimentadores
    buscaProfundidadeAlimentadores(grafo, numeroBarras, &alimentador, numeroAlimentadores); 
//    for(i=0; i<numeroAlimentadores; i++) supressaoBarrasPassagem(grafo, alimentador,i, ramo); // Suprime barras de passagem - Aceleração de cálculo
 
    // Transforma em pu e cria matrizes de admitância iniciais
    calculaPU(grafo, numeroBarras, ramo, numeroRamos, Sbase);
    
    //Atualiza a posição de taps
    atualizaTaps(ramo, numeroRamos); //terminar o tap fixo de trafos de potência
    
    
    
    //--------------------------------------------------------------------------
    // Compatibilização com MRAN
    
    //resultadoLeitura = leituraBarrasLinhasTrafos(&grafoSDR, &dadosTrafoSDR, &numeroBarras, &numeroTrafos, &numeroChaves);
    //resultadoLeitura =  leituraDadosAlimentadores(&dadosAlimentadorSDR);
    //resultadoLeitura =  leituraDadosReguladoresTensao(&dadosReguladorSDR);

    //--------------------------------------------------------------------------
    // atualizaEstadoChaves(grafoSDR,numeroBarras); //Atualiza os estados das chaves de acordo com ESTADOS_CHS
    //LISTASETORES * lista_setores = buscaSetores(grafoSDR, dadosAlimentadorSDR, numeroBarras);

    //constroiListaChaves(grafoSDR, &listaChaves, numeroChaves, numeroBarras);
    //constroiListaAdjacenciaSetores(&grafoSDR, lista_setores, &listaChaves, &grafoSetores, &numeroChaves, &numeroBarras);
    //constroiRNPSetores(lista_setores, grafoSDR, &rnpSetores, numeroBarras);
    //gravaRNPSetores (rnpSetores, numeroSetores);
    //gravaGrafoSetores(grafoSetores, numeroSetores, listaChaves);
    //configuracaoInicial = alocaIndividuo(numeroAlimentadores, idConfiguracao, 1);
    //constroiIndividuoInicial(grafoSetores, grafoSDR, listaChaves, dadosAlimentadorSDR, configuracaoInicial);
    
    
    //gravaIndividuo(".dad",configuracaoInicial[idConfiguracao]);
    //imprimeBarrasIsoladas(numeroBarras, grafoSDR);
    //salvaChaves(numeroChaves,listaChaves);
    //salvaChavesCompleto(numeroChaves, listaChaves);
    //gravaDadosBarras(numeroBarras, grafoSDR);
    //salvaLinhas(numeroBarras, grafoSDR);
    //salvaChavesAMontante(listaChaves, configuracaoInicial[idConfiguracao],grafoSetores);
    //gravaBarrasRT(dadosReguladorSDR, numeroReguladores, grafoSDR, numeroBarras);
    //gravaDadosTrafo(numeroTrafos, dadosTrafoSDR);

    //free(dadosReguladorSDR);
    //free(dadosAlimentadorSDR);
    //free(dadosTrafoSDR);
    //desalocaGrafoSDR(grafoSDR, numeroBarras);
    //free(listaChaves);
    
    
    // Alterado no Fluxo trifãsico (incompatível)
    //  //constroiMatrizZ(grafoSDR, &Z, numeroBarras);
    //  //constroiMatrizMaximoCorrente(grafoSDR, &maximoCorrente, numeroBarras);
    //  //gravaMatrizZCorrente(Z, maximoCorrente,numeroBarras);
    //  //gravaMatrizCorrente(maximoCorrente, numeroBarras);
    
    
    
    
    clock_t a3 = clock();
    double tempo_B = (double)(a3 - a2) / CLOCKS_PER_SEC;
    printf("\nTempo Processamento Modelo: %lf\n\n",  tempo_B);
    
    //Simulacoes com Curvas de Carga - Considera o valor de P e Q de arquivo de curva previamente agregada
    if (flag_mod == 3){ 
        leituraCurvasAgregadas(folder, &curvasTrafos,barra,numeroBarras); //Leitura de curvas de cargas agregadas
        estampa_tempo  = 10;
        inicializaPQcurvas(barra, curvasTrafos, numeroBarras,  estampa_tempo, VALOR_ESPERADO, Sbase); 
    }    
     
    
    // Fluxo de Potência via Varredura Direta/Inversa Trifásica por Níveis de Tensão
    //           O fluxo de potência está como cargas em corrente constante devido à implementação da COPEL
    incializaTensoesVarredura(grafo, numeroBarras, alimentador, numeroAlimentadores);
    
    clock_t start2 = clock();
    
    
    fluxoPotencia_Niveis_BFS_Multiplos(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, Sbase/1000, interfaceNiveis, numeroInterfaces, true); // true converge FP | false faz iteraçao única (P&D)
    clock_t end2 = clock();
    double edtr_time = (double)(end2 - start2) / CLOCKS_PER_SEC;
    printf("\nNumero Alimentadores: %d \t Numero Barras: %d \t\t\n\n", numeroAlimentadores, numeroBarras);
    printf("\nTempo: %lf\n\n",  edtr_time);

    
    
    
    // Tempo Real ---------------- Estimação
    
    
    // Leitura do arquivo DSTAT  e atualiza a topologia da rede elétrica
    // leitura do arquvio e atualizaçao
    
    //Leitura do Arquivo DTAPS
    // leitura do arquvio e atualizaçao  de taps
    
    
    
//    // Leitura de Medidas do sistema SCADA (SASE processado)
//    numeroMedidas = leituraMedidas(folder, "DMED.csv", &medida, ramo, numeroRamos, barra, numeroBarras, grafo, Sbase); 
//    
//    // Criação de Áreas de Medição para o Estimador de Demandas Trifásicas
//    buscaAMs(grafo, numeroBarras, alimentador, numeroAlimentadores, medida, numeroMedidas, &areasMedicao);
//    
//    //Estimador de Demandas Trifásicas
//    estimadorDemandaTrifasico(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, Sbase/1000, interfaceNiveis, numeroInterfaces, areasMedicao);
    
        
    //Liberação de Memória
    free_BARRA(barra,numeroBarras);
    free_DRAM(ramo,numeroRamos);
    free_GRAFO(grafo,numeroBarras);
    free_ALIMENTADOR(alimentador,numeroAlimentadores);
    return (EXIT_SUCCESS);
}

