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
    DMED *medida = NULL,*medidaPMU = NULL, *virtuais = NULL;
    ALIMENTADOR *alimentador = NULL, *areas = NULL;

    // Para estimação de demanda e simulações com séries temporais
    CURVA_TRAFO *curvasTrafos = NULL; //Curva de carga dos trafos de distribuição agregadas
    long int flag_mod = 0; //FLAG para rodar a modelagem de cargas

    // --------------------------------------------------------------------------------
    //Os arquivos lidos são:
    //config.txt
    //

    //Saída
    //

    // --------------------------------------------------------------------------------
    // Leitura dos dados da rede elétrica - Assume que já identação de barras está pré-mapeado e ordenado de 0 até NB corretamente
    folder = leituraDados(&barra,&ramo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
    printf("barras: %d   %d\n",numeroBarras,numeroRamos);
    
    // Cria estrutura de dados da rede elétrica
    geraGrafo(&grafo, barra, numeroBarras,ramo,numeroRamos); 
    // printf("gera grafo ok\n");
    // Cria as listas encadeadas radiais dos alimentadores
    buscaProfundidadeAlimentadores(grafo, numeroBarras, &alimentador, numeroAlimentadores); 
    // printf("alimentadores ok\n");
    // Transforma em pu e cria matrizes de admitância iniciais
    calculaPU(grafo, numeroBarras, ramo, numeroRamos, Sbase);
    // printf("calculo pu ok\n");
    //Atualiza a posição de taps
    atualizaTaps(ramo, numeroRamos); //terminar o tap de trafos 
    // printf("taps ok\n");
    
    interfaceNiveis = leituraDINTERSE(folder,"DINTERSE.csv", &numeroInterfaces);
    
    //Simulacoes com Curvas de Carga - Considera o valor de P e Q de arquivo de curva previamente agregada
    if (flag_mod == 3){ 
        leituraCurvasAgregadas(folder, &curvasTrafos,barra,numeroBarras); //Leitura de curvas de cargas agregadas
    }
    
    for (estampa_tempo = 0; estampa_tempo <1;estampa_tempo++){
    if (flag_mod == 3){ 
        inicializaPQcurvas(barra, curvasTrafos, numeroBarras,  estampa_tempo, EQUILIBRADO_MAXIMO, Sbase); 
//        inicializaPQcurvas(barra, curvasTrafos, numeroBarras,  estampa_tempo, DESBALANCEADO_INPUT_DBAR, Sbase);
    }    
      
    // // Fluxo de Potência via Varredura Direta/Inversa Trifásica
//    fluxoPotencia_BFS_Multiplos(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, Sbase/1000);
    
    // // Fluxo de Potência via Varredura Direta/Inversa Trifásica
    incializaTensoesVarredura(grafo, numeroBarras, alimentador, numeroAlimentadores);
    fluxoPotencia_Niveis_BFS_Multiplos(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, Sbase/1000, interfaceNiveis, numeroInterfaces, true);

//    srand(1000);
//    for (int kk = 0; kk<1; kk++){
//        for (int idAlim = 0; idAlim < numeroInterfaces; idAlim++){
//            if (interfaceNiveis[idAlim][1] == 69518){ // para simular troca de SE 
//                interfaceNiveis[idAlim][1] = 55846;
//            }
//        }
//        amostragemVariacaoCarga(grafo, numeroBarras, alimentador, numeroAlimentadores, 0.0);
////        incializaTensoesVarredura(grafo, numeroBarras, alimentador, numeroAlimentadores);       
//        
//        fluxoPotencia_Niveis_BFS_Multiplos(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, Sbase/1000, interfaceNiveis, numeroInterfaces, false);
//    }
    
    }
    
    //Liberação de Memória
    free_BARRA(barra,numeroBarras);
    free_DRAM(ramo,numeroRamos);
    free_GRAFO(grafo,numeroBarras);
    free_ALIMENTADOR(alimentador,numeroAlimentadores);
    return (EXIT_SUCCESS);
}

