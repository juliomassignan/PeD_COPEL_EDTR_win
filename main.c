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

BOOL symetric_OPT = 0; //Variável global para assumir transposição de linhas
BOOL control_REG_OPT = 1; //Variável global para ativar controle automático de taps em reguladores de tensão
BOOL control_CAP_OPT = 0; //Variável global para ativar controle automático de bancos de capacitores chaveados
double Sbase = 1000000; //Variável global com a potência base da rede elétrica em VA

/*
 * 
 */
int main(int argc, char** argv) {
    long int estampa_tempo = 0;
    long int numeroBarras = 0;
    long int numeroAlimentadores = 0, numeroAreas = 0;
    long int numeroRamos = 0;
    long int **numeroMedidas = NULL,**numeroMedidasPMU = NULL,**numeroVirtuais = NULL;
    long int **interfaceNiveis = NULL;
    long int numeroInterfaces = 0;
    long int i = 0;
    char *folder = NULL;
    
    
    // Estruturas de dados da rede elétrica
    DBAR *barra = NULL;
    DRAM *ramo = NULL;
    GRAFO *grafo = NULL;
    DMED *medida = NULL;
    ALIMENTADOR *alimentador = NULL, *areas = NULL;
    
    AREA_MEDICAO *areasMedicao = NULL;
    

    // Para estimação de demanda e simulações com séries temporais
    CURVA_TRAFO *curvasTrafos = NULL; //Curva de carga dos trafos de distribuição agregadas
    long int flag_mod = 3; //FLAG para rodar a modelagem de cargas

    // --------------------------------------------------------------------------------
    //Os arquivos lidos são:
    //config.txt
    //

    //Saída
    //

    // --------------------------------------------------------------------------------
    // Leitura dos dados da rede elétrica - Assume que já identação de barras está pré-mapeado e ordenado de 0 até NB corretamente
    folder = leituraDados(&barra,&ramo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
    
    // Cria estrutura de dados da rede elétrica
    geraGrafo(&grafo, barra, numeroBarras,ramo,numeroRamos); 
    
    // Cria as listas encadeadas radiais dos alimentadores
    buscaProfundidadeAlimentadores(grafo, numeroBarras, &alimentador, numeroAlimentadores); 
//    for(i=0; i<numeroAlimentadores; i++) supressaoBarrasPassagem(grafo, alimentador,i, ramo); // Suprime barras de passagem - Aceleração de cálculo
 
    // Transforma em pu e cria matrizes de admitância iniciais
    calculaPU(grafo, numeroBarras, ramo, numeroRamos, Sbase);
    
    //Atualiza a posição de taps
    atualizaTaps(ramo, numeroRamos); //terminar o tap de trafos 
    
    // Leitura de Interface entre Níveis de Tensão - Subestações de 34.5 / 13.8 kV
    interfaceNiveis = leituraDINTERSE(folder,"DINTERSE.csv", &numeroInterfaces);
    
    //Simulacoes com Curvas de Carga - Considera o valor de P e Q de arquivo de curva previamente agregada
    if (flag_mod == 3){ 
        leituraCurvasAgregadas(folder, &curvasTrafos,barra,numeroBarras); //Leitura de curvas de cargas agregadas
        estampa_tempo  = 10;
        inicializaPQcurvas(barra, curvasTrafos, numeroBarras,  estampa_tempo, MAJORANTE_QUANTIL, Sbase); 
    }    
      
    
    // Fluxo de Potência via Varredura Direta/Inversa Trifásica por Níveis de Tensão
    //           O fluxo de potência está como cargas em corrente constante devido à implementação da COPEL
    incializaTensoesVarredura(grafo, numeroBarras, alimentador, numeroAlimentadores);
//    leituraDMED_fp_CARGAS(folder, "DMED_fp.csv", grafo, numeroBarras, Sbase/1000); // Função Teste no Estimador
    clock_t start2 = clock();
    fluxoPotencia_Niveis_BFS_Multiplos(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, Sbase/1000, interfaceNiveis, numeroInterfaces, true);
    
//    fluxoPotencia_Alimentador_BFS_call(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, 95, Sbase/1000);
    
    clock_t end2 = clock();
    double edtr_time = (double)(end2 - start2) / CLOCKS_PER_SEC;
    printf("\nTempo: %lf\n\n",  edtr_time);


//    salvaArquivosAlimentador(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, 95, Sbase/1000, true, "w+"); // Alimentador 84800LN071
//
//    int idAlim;
//    for (idAlim = 0; idAlim <numeroAlimentadores; idAlim++){
//        if (idAlim == 0) salvaArquivosAlimentador(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, idAlim, Sbase/1000, false, "w+");
//        else salvaArquivosAlimentador(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, idAlim, Sbase/1000, false, "a+");
//    }
    
    // Transformar componentes opcionais de código - 34.5 / 13.8 kV interdependência






    
    // Leitura de Medidas do sistema SCADA (SASE processado)
//    numeroMedidas = leituraMedidas(folder, "DMED.csv", &medida, ramo, numeroRamos, barra, numeroBarras, grafo, Sbase); 
    
    // Criação de Áreas de Medição para o Estimador de Demandas Trifásicas
//    buscaAMs(grafo, numeroBarras, alimentador, numeroAlimentadores, medidas, numeroMedidas, areasMedicao);
    
    //Estimador de Demandas Trifásicas
//    estimadorDemandaTrifasico(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, Sbase/1000, interfaceNiveis, numeroInterfaces, areasMedicao);
    
        
    //Liberação de Memória
    free_BARRA(barra,numeroBarras);
    free_DRAM(ramo,numeroRamos);
    free_GRAFO(grafo,numeroBarras);
    free_ALIMENTADOR(alimentador,numeroAlimentadores);
    return (EXIT_SUCCESS);
}

