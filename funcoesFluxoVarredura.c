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

#include "data_structures.h"
#include "funcoesTopologia.h"
#include "funcoesFluxoVarredura.h"
#include "funcoesCalculoEletrico.h"
#include "funcoesMatematicas.h"

//------------------------------------------------------------------------------
//
// FUNÇÕES DE IMPRESSÃO DE DADOS
//
//------------------------------------------------------------------------------

// Função para impressão de tensões nodais em arquivos
void salvaTensoesNodais(const char *filename, const char *modo, TF_ALIMENTADOR alimentador, TF_GRAFO *grafo){
    int i, k;
    TF_FILABARRAS *barraAtual;
    
    FILE *arquivo;
    arquivo = fopen(filename,modo);
    
//    fprintf(arquivo,"\n\nAlimentador %ld \n",alimentador.idAlim);
//    fprintf(arquivo,"ID\tPROF\tVa \t Teta_a \tVb \t Teta_b \tVc \t Teta_c \n");
    i=0;
    barraAtual = &alimentador.rnp[0];
    while((barraAtual != NULL)){
        k = barraAtual->idNo;
        switch (grafo[k].fases){
            case 1:
                fprintf(arquivo,"%ld\t%ld\t%ld\t %.7lf \t %.5lf \t-\t-\t-\t-\n",alimentador.idAlim,grafo[k].barra->ID,grafo[k].profundidade,cabs(grafo[k].V[0]),carg(grafo[k].V[0])*180/PI);
                break;
            case 2:
                fprintf(arquivo,"%ld\t%ld\t%ld\t-\t-\t%.7lf \t %.5lf \t-\t-\n",alimentador.idAlim,grafo[k].barra->ID,grafo[k].profundidade,cabs(grafo[k].V[1]),carg(grafo[k].V[1])*180/PI);
                break;
            case 3:
                fprintf(arquivo,"%ld\t%ld\t%ld\t-\t-\t-\t-\t %.7lf \t %.5lf\n",alimentador.idAlim,grafo[k].barra->ID,grafo[k].profundidade,cabs(grafo[k].V[2]),carg(grafo[k].V[2])*180/PI);
                break;
            case 4:
                fprintf(arquivo,"%ld\t%ld\t%ld\t %.7lf \t %.5lf \t %.7lf \t %.5lf\t-\t-\n",alimentador.idAlim,grafo[k].barra->ID,grafo[k].profundidade,cabs(grafo[k].V[0]),carg(grafo[k].V[0])*180/PI,cabs(grafo[k].V[1]),carg(grafo[k].V[1])*180/PI);
                break;
            case 5:
                fprintf(arquivo,"%ld\t%ld\t%ld\t %.7lf \t %.5lf \t-\t-\t %.7lf \t %.5lf\n",alimentador.idAlim,grafo[k].barra->ID,grafo[k].profundidade,cabs(grafo[k].V[0]),carg(grafo[k].V[0])*180/PI,cabs(grafo[k].V[2]),carg(grafo[k].V[2])*180/PI);
                break;
            case 6:
                fprintf(arquivo,"%ld\t%ld\t%ld\t-\t-\t %.7lf \t %.5lf\t %.7lf \t %.5lf\n",alimentador.idAlim,grafo[k].barra->ID,grafo[k].profundidade,cabs(grafo[k].V[1]),carg(grafo[k].V[1])*180/PI,cabs(grafo[k].V[2]),carg(grafo[k].V[2])*180/PI);
                break;
            case 7:
                fprintf(arquivo,"%ld\t%ld\t%ld\t %.7lf \t %.5lf \t %.7lf \t %.5lf\t %.7lf \t %.5lf\n",alimentador.idAlim,grafo[k].barra->ID,grafo[k].profundidade,cabs(grafo[k].V[0]),carg(grafo[k].V[0])*180/PI,cabs(grafo[k].V[1]),carg(grafo[k].V[1])*180/PI,cabs(grafo[k].V[2]),carg(grafo[k].V[2])*180/PI);
                break;    
        }
        barraAtual = barraAtual->prox;
        i++;
    }
    fclose(arquivo);
}

// Função para impressão de correntes nos ramos, ampacidade e carregamento em arquivos
void salvaCorrentesRamos(const char *filename, const char *modo, TF_ALIMENTADOR alimentador, TF_GRAFO *grafo, long int numeroBarras, double Sbase){
    int i, k;
    long int adj;
    TF_FILABARRAS *barraAtual;
    BOOL visitado[numeroBarras];
    double Ibase;
    
    for(i=0; i<numeroBarras; i++){ 
        visitado[i] = false;
    }
    FILE *arquivo;
    arquivo = fopen(filename,modo);
    
//    fprintf(arquivo,"\n\nAlimentador %ld \n",alimentador.idAlim);
//    fprintf(arquivo,"DE\tPARA\tIa \t Ib \tIc \t Ampacidade \tCarregamento \tPa\tPb\tPc\tQa\tQb\tQc \n");
    i=0;
    barraAtual = &alimentador.rnp[0];
    while((barraAtual != NULL)){
        k = barraAtual->idNo;
        Ibase =  Sbase *1000 / (sqrt(3) * grafo[k].Vbase);
        
        for (int m =0; m < grafo[k].numeroAdjacentes;m++){
            adj = grafo[k].adjacentes[m].idNo;
            if (!visitado[adj]){
                grafo[k].adjacentes[m].S[0] = grafo[k].V[0] * conj(grafo[k].adjacentes[m].Cur[0]);
                grafo[k].adjacentes[m].S[1] = grafo[k].V[1] * conj(grafo[k].adjacentes[m].Cur[1]);
                grafo[k].adjacentes[m].S[2] = grafo[k].V[2] * conj(grafo[k].adjacentes[m].Cur[2]);
                
                switch (grafo[adj].fases){
                    case 1:
                        fprintf(arquivo,"%ld\t%ld\t%ld\t%.7lf\t-\t-\t%.7lf\t%.7lf  \t %.7lf \t-\t-\t %.7lf \t-\t-\n",alimentador.idAlim,grafo[k].adjacentes[m].ramo->DE,grafo[k].adjacentes[m].ramo->PARA,cabs(grafo[k].adjacentes[m].Cur[0])*Ibase,grafo[k].adjacentes[m].ramo->ampacidade,grafo[k].adjacentes[m].loading, creal(grafo[k].adjacentes[m].S[0]), cimag(grafo[k].adjacentes[m].S[0]));
                        break;
                    case 2:
                        fprintf(arquivo,"%ld\t%ld\t%ld\t-\t %.7lf  \t-\t   %.7lf  \t    %.7lf  \t-\t %.7lf  \t-\t-\t %.7lf  \t-\n",alimentador.idAlim,grafo[k].adjacentes[m].ramo->DE,grafo[k].adjacentes[m].ramo->PARA,cabs(grafo[k].adjacentes[adj].Cur[1])*Ibase,grafo[k].adjacentes[m].ramo->ampacidade,grafo[k].adjacentes[m].loading , creal(grafo[k].adjacentes[m].S[1]), cimag(grafo[k].adjacentes[m].S[1]) );
                        break;
                    case 3:
                        fprintf(arquivo,"%ld\t%ld\t%ld\t-\t-\t  %.7lf   \t   %.7lf  \t    %.7lf  \t-\t-\t %.7lf  \t-\t-\t %.7lf   \n",alimentador.idAlim,grafo[k].adjacentes[m].ramo->DE,grafo[k].adjacentes[m].ramo->PARA,cabs(grafo[k].adjacentes[m].Cur[2])*Ibase,grafo[k].adjacentes[m].ramo->ampacidade,grafo[k].adjacentes[m].loading , creal(grafo[k].adjacentes[m].S[2]), cimag(grafo[k].adjacentes[m].S[2]));
                        break;
                    case 4:
                        fprintf(arquivo,"%ld\t%ld\t%ld\t %.7lf \t %.7lf  \t-\t   %.7lf  \t    %.7lf \t %.7lf \t %.7lf \t-\t %.7lf \t %.7lf \t- \n",alimentador.idAlim,grafo[k].adjacentes[m].ramo->DE,grafo[k].adjacentes[m].ramo->PARA,cabs(grafo[k].adjacentes[m].Cur[0])*Ibase,cabs(grafo[k].adjacentes[m].Cur[1])*Ibase,grafo[k].adjacentes[m].ramo->ampacidade,grafo[k].adjacentes[m].loading, creal(grafo[k].adjacentes[m].S[0]), creal(grafo[k].adjacentes[m].S[1]), cimag(grafo[k].adjacentes[m].S[0]), cimag(grafo[k].adjacentes[m].S[1]));
                        break;
                    case 5:
                        fprintf(arquivo,"%ld\t%ld\t%ld\t-\t %.7lf  \t  %.7lf   \t   %.7lf  \t    %.7lf \t-\t %.7lf  \t %.7lf  \t-\t %.7lf  \t %.7lf   \n",alimentador.idAlim,grafo[k].adjacentes[m].ramo->DE,grafo[k].adjacentes[m].ramo->PARA,cabs(grafo[k].adjacentes[m].Cur[1])*Ibase,cabs(grafo[k].adjacentes[m].Cur[2])*Ibase,grafo[k].adjacentes[m].ramo->ampacidade,grafo[k].adjacentes[m].loading, creal(grafo[k].adjacentes[m].S[1]), creal(grafo[k].adjacentes[m].S[2]), cimag(grafo[k].adjacentes[m].S[1]), cimag(grafo[k].adjacentes[m].S[2]));
                        break;
                    case 6:
                        fprintf(arquivo,"%ld\t%ld\t%ld\t %.7lf \t-\t  %.7lf   \t   %.7lf  \t    %.7lf  \t %.7lf  \t-\t %.7lf  \t %.7lf  \t-\t %.7lf  \n",alimentador.idAlim,grafo[k].adjacentes[m].ramo->DE,grafo[k].adjacentes[m].ramo->PARA,cabs(grafo[k].adjacentes[m].Cur[0])*Ibase,cabs(grafo[k].adjacentes[m].Cur[2])*Ibase,grafo[k].adjacentes[m].ramo->ampacidade,grafo[k].adjacentes[m].loading, creal(grafo[k].adjacentes[m].S[0]), creal(grafo[k].adjacentes[m].S[2]), cimag(grafo[k].adjacentes[m].S[0]), cimag(grafo[k].adjacentes[m].S[2]));
                        break;
                    case 7:
                        fprintf(arquivo,"%ld\t%ld\t%ld\t %.7lf \t %.7lf  \t  %.7lf   \t   %.7lf  \t    %.7lf  \t %.7lf  \t %.7lf  \t %.7lf  \t %.7lf  \t %.7lf  \t %.7lf  \n",alimentador.idAlim,grafo[k].adjacentes[m].ramo->DE,grafo[k].adjacentes[m].ramo->PARA,cabs(grafo[k].adjacentes[m].Cur[0])*Ibase,cabs(grafo[k].adjacentes[m].Cur[1])*Ibase,cabs(grafo[k].adjacentes[m].Cur[2])*Ibase,grafo[k].adjacentes[m].ramo->ampacidade,grafo[k].adjacentes[m].loading, creal(grafo[k].adjacentes[m].S[0]), creal(grafo[k].adjacentes[m].S[1]), creal(grafo[k].adjacentes[m].S[2]), cimag(grafo[k].adjacentes[m].S[0]), cimag(grafo[k].adjacentes[m].S[1]), cimag(grafo[k].adjacentes[m].S[2]));
                        break;    
                }
                
            }
        }
        
        visitado[k] = true;
        barraAtual = barraAtual->prox;
        i++;
    }
    fclose(arquivo);
}






//Imprime na tela as tensões nodais trifásicas
/**
 * @brief Função auxiliar para imprimir na tela as tensões complexas nodais trifásicas
 * 
 * Essa função imprime na tela as tensões complexas nodais por fase (fase-neutro) de determinada barra da rede elétrica especificada no parâmetro de
 * entrada @p grafo , imprimindo as tensões de um nó específico da rede elétrica.
 * A função retorna @c void.
 * 
 * @param *grafo ponteiro para o nó específico do grafo cujas tensões nodais trifásicas serão impressas na tela
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeTensoesNodais(TF_GRAFO *grafo){
    int i;
    
//    printf("Vbase:%.3lf\t",grafo->Vbase);
    switch (grafo->fases){
        case 1:
            printf("%ld\tVa: %.5lf | %.3lf \tVb:- |-\tVc:- |-\n",grafo->barra->ID,cabs(grafo->V[0]),carg(grafo->V[0])*180/PI);
            break;
        case 2:
            printf("%ld\tVa:- |- \tVb: %.5lf | %.3lf\tVc:- |-\n",grafo->barra->ID,cabs(grafo->V[1]),carg(grafo->V[1])*180/PI);
            break;
        case 3:
            printf("%ld\tVa:- |- \tVb:- |-\tVc: %.5lf | %.3lf\n",grafo->barra->ID,cabs(grafo->V[2]),carg(grafo->V[2])*180/PI);
            break;
        case 4:
            printf("%ld\tVa: %.5lf | %.3lf \tVb: %.5lf | %.3lf\tVc:- |-\n",grafo->barra->ID,cabs(grafo->V[0]),carg(grafo->V[0])*180/PI,cabs(grafo->V[1]),carg(grafo->V[1])*180/PI);
            break;
        case 5:
            printf("%ld\tVa: %.5lf | %.3lf \tVb:  - |  -\tVc: %.5lf | %.3lf\n",grafo->barra->ID,cabs(grafo->V[0]),carg(grafo->V[0])*180/PI,cabs(grafo->V[2]),carg(grafo->V[2])*180/PI);
            break;
        case 6:
            printf("%ld\tVa:  - |  - \tVb: %.5lf | %.3lf\tVc: %.5lf | %.3lf\n",grafo->barra->ID,cabs(grafo->V[1]),carg(grafo->V[1])*180/PI,cabs(grafo->V[2]),carg(grafo->V[2])*180/PI);
            break;
        case 7:
            printf("%ld\tVa: %.5lf | %.3lf \tVb: %.5lf | %.3lf\tVc: %.5lf | %.3lf\n",grafo->barra->ID,cabs(grafo->V[0]),carg(grafo->V[0])*180/PI,cabs(grafo->V[1]),carg(grafo->V[1])*180/PI,cabs(grafo->V[2]),carg(grafo->V[2])*180/PI);
            break;    
    }
    
}

//Imprime na tela os taps dos reguladores
/**
 * @brief Função auxiliar para imprimir na tela os taps dos reguladores de tensão
 * 
 * Essa função imprime na tela os taps dos reguladores de tensão (em cada fase) de determinada regulador de tensão da rede elétrica
 * adjacente à barra especificada no parâmetro de entrada @p grafo , imprimindo os taps do reguladores adjacentes
 * A função retorna @c void.
 * 
 * @param *grafo ponteiro para o nó específico do grafo cujas taps de reguladores adjacentes serão impressos
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeTaps(TF_GRAFO *grafo){
    int i;
    for (i=0;i < grafo->numeroAdjacentes;i++){
        if((grafo->adjacentes[i].tipo  == regulador) && (grafo->adjacentes[i].ramo->k == grafo->idNo)){
            int idram = grafo->adjacentes[i].idram;
            printf("Regulador Barra %ld:    ",grafo->adjacentes[i].ramo->DE);
            printf("%.3lf kVA\t%.2lf kV\tControl {%ld}\t%.0lf\t%.0lf\t%.0lf\n",grafo->adjacentes[i].ramo->regulador.Snominal,grafo->adjacentes[i].ramo->regulador.Vnom/1000,grafo->adjacentes[i].ramo->regulador.controle,grafo->adjacentes[i].ramo->regulador.tap[0],grafo->adjacentes[i].ramo->regulador.tap[1],grafo->adjacentes[i].ramo->regulador.tap[2]);
        }
    }
}

//Imprime na tela as injeções de corrente nodais trifásicas
/**
 * @brief Função auxiliar para imprimir na tela as injecṍes de corrente complexas nodais trifásicas
 * 
 * Essa função imprime na tela as injeções de corrente complexas nodais por fase (fase-neutro) de determinada barra da rede elétrica especificada 
 * no parâmetro de entrada @p grafo , imprimindo as injeções de corrente de um nó específico da rede elétrica.
 * A função retorna @c void.
 * 
 * @param *grafo ponteiro para o nó específico do grafo cujas injeções de corrente nodais trifásicas serão impressas na tela
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeiInjecoesCorrentes(TF_GRAFO *grafo){
    int i;
    
//    printf("Vbase:%.3lf\t",grafo->Vbase);
    switch (grafo->fases){
        case 1:
            printf("%ld\tIa: %.5lf | %.3lf \tIb:  - |  -\tIc:  - |  -\n",grafo->barra->ID,cabs(grafo->Cur[0]),carg(grafo->Cur[0])*180/PI);
            break;
        case 2:
            printf("%ld\tIa:  - |  - \tIb: %.5lf | %.3lf\tIc:  - |  -\n",grafo->barra->ID,cabs(grafo->Cur[1]),carg(grafo->Cur[1])*180/PI);
            break;
        case 3:
            printf("%ld\tIa:  - |  - \tIb:  - |  -\tIc: %.5lf | %.3lf\n",grafo->barra->ID,cabs(grafo->Cur[2]),carg(grafo->Cur[2])*180/PI);
            break;
        case 4:
            printf("%ld\tIa: %.5lf | %.3lf \tIb: %.5lf | %.3lf\tIc:  - |  -\n",grafo->barra->ID,cabs(grafo->Cur[0]),carg(grafo->Cur[0])*180/PI,cabs(grafo->Cur[1]),carg(grafo->Cur[1])*180/PI);
            break;
        case 5:
            printf("%ld\tIa: %.5lf | %.3lf \tIb:  - |  -\tIc: %.5lf | %.3lf\n",grafo->barra->ID,cabs(grafo->Cur[0]),carg(grafo->Cur[0])*180/PI,cabs(grafo->Cur[2]),carg(grafo->Cur[2])*180/PI);
            break;
        case 6:
            printf("%ld\tIa:  - |  - \tIb: %.5lf | %.3lf\tIc: %.5lf | %.3lf\n",grafo->barra->ID,cabs(grafo->Cur[1]),carg(grafo->Cur[1])*180/PI,cabs(grafo->Cur[2]),carg(grafo->Cur[2])*180/PI);
            break;
        case 7:
            printf("%ld\tIa: %.5lf | %.3lf \tIb: %.5lf | %.3lf\tIc: %.5lf | %.3lf\n",grafo->barra->ID,cabs(grafo->Cur[0]),carg(grafo->Cur[0])*180/PI,cabs(grafo->Cur[1]),carg(grafo->Cur[1])*180/PI,cabs(grafo->Cur[2]),carg(grafo->Cur[2])*180/PI);
            break;    
    }
    
}

//Imprime na tela as tensões nodais trifásicas
/**
 * @brief Função auxiliar para imprimir na tela as correntes complexas trifásicas em determinado ramo da rede elétrica
 * 
 * Essa função imprime na tela as injeções de correntes complexas trifásicas por fase de determinada ramo da rede elétrica especificada 
 * no parâmetro de entrada @p noAdj , proveniente da estrutura do tipo NOADJCENTE, imprimindo as correntes que fluem para um nó específico de seu nó a montante.
 * A função retorna @c void.
 * 
 * @param *noAdj ponteiro para o o nó adjacente a jusante cuja corrente complexa trifásica do ramo será impressa
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeCorrentes(TF_NOADJACENTE *noAdj){
   int i;
   
//    printf("Vbase:%.3lf\t",grafo->Vbase);
   switch (noAdj->ramo->fases){
       case 1:
           printf("%ld - %ld\tIa: %.5lf | %.3lf \tIb:  - |  -\tIc:  - |  -\n",noAdj->ramo->DE,noAdj->ramo->PARA,cabs(noAdj->Cur[0]),carg(noAdj->Cur[0])*180/PI);
           break;
       case 2:
           printf("%ld - %ld\tIa:  - |  - \tIb: %.5lf | %.3lf\tIc:  - |  -\n",noAdj->ramo->DE,noAdj->ramo->PARA,cabs(noAdj->Cur[1]),carg(noAdj->Cur[1])*180/PI);
           break;
       case 3:
           printf("%ld - %ld\tIa:  - |  - \tIb:  - |  -\tIc: %.5lf | %.3lf\n",noAdj->ramo->DE,noAdj->ramo->PARA,cabs(noAdj->Cur[2]),carg(noAdj->Cur[2])*180/PI);
           break;
       case 4:
           printf("%ld - %ld\tIa: %.5lf | %.3lf \tIb: %.5lf | %.3lf\tIc:  - |  -\n",noAdj->ramo->DE,noAdj->ramo->PARA,cabs(noAdj->Cur[0]),carg(noAdj->Cur[0])*180/PI,cabs(noAdj->Cur[1]),carg(noAdj->Cur[1])*180/PI);
           break;
       case 5:
           printf("%ld - %ld\tIa: %.5lf | %.3lf \tIb:  - |  -\tIc: %.5lf | %.3lf\n",noAdj->ramo->DE,noAdj->ramo->PARA,cabs(noAdj->Cur[0]),carg(noAdj->Cur[0])*180/PI,cabs(noAdj->Cur[2]),carg(noAdj->Cur[2])*180/PI);
           break;
       case 6:
           printf("%ld - %ld\tIa:  - |  - \tIb: %.5lf | %.3lf\tIc: %.5lf | %.3lf\n",noAdj->ramo->DE,noAdj->ramo->PARA,cabs(noAdj->Cur[1]),carg(noAdj->Cur[1])*180/PI,cabs(noAdj->Cur[2]),carg(noAdj->Cur[2])*180/PI);
           break;
       case 7:
           printf("%ld - %ld\tIa: %.5lf | %.3lf \tIb: %.5lf | %.3lf\tIc: %.5lf | %.3lf\n",noAdj->ramo->DE,noAdj->ramo->PARA,cabs(noAdj->Cur[0]),carg(noAdj->Cur[0])*180/PI,cabs(noAdj->Cur[1]),carg(noAdj->Cur[1])*180/PI,cabs(noAdj->Cur[2]),carg(noAdj->Cur[2])*180/PI);
           break;    
   }
   
}

//------------------------------------------------------------------------------
//
// FUNÇÕES PARA O CÁLCULO DE FLUXO DE POTÊNCIA
//
//------------------------------------------------------------------------------

//Função de cálculo de desbalanço fasorial - relação entre tensão de sequeência negativa e positiva do fasor
/**
 * @brief Função auxiliar para inicializar as tensões complexas nodais trifásicas de todos os alimentadores igual à tensão do nó raiz
 * 
 * Essa função ....
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
double desbalancoFasorialSeq(__complex__ double *Fasor){ 
    int i, j;
    double desbalanco;
   
    __complex__ double FasorSeq[3], **T, a, a2;
    
    a = 1.0*(cos(120*PI/180) + I*sin(120*PI/180));
    a2 = 1.0*(cos(-120*PI/180) + I*sin(-120*PI/180));
    T = c_matAloca(3);
    
    FasorSeq[0] = 0; FasorSeq[1] = 0; FasorSeq[2] = 0;
    
    T[0][0] = 1;  T[0][1] = 1;    T[0][2] = 1;  
    T[1][0] = 1;  T[1][1] = a;    T[1][2] = a2;  
    T[2][0] = 1;  T[2][1] = a2;   T[2][2] = a;  
    
    //Flat start trifásico (Va = Vb = Vc = 1p.u.  Ta = 0  Tb = -120  Tc = 120) - com busca em profundidade para atualizar taps iniciais
    for(i=0; i<3; i++){ 
        for(j=0; j<3; j++){ 
            FasorSeq[i] +=  T[i][j] * Fasor[j];
        }
        FasorSeq[i] = FasorSeq[i]/3;
    }
    desbalanco = cabs(FasorSeq[2])/cabs(FasorSeq[1]);
    return(desbalanco);
}

//Função inicialização do vetor x para sistemas radiais (sem o arquivo Vinicial)
/**
 * @brief Função auxiliar para inicializar as tensões complexas nodais trifásicas de todos os alimentadores igual à tensão do nó raiz
 * 
 * Essa função inicializa as tensões complexas nodais por fase (fase-neutro) de todas as barras de todos os alimentadores da rede elértrica. 
 * O valor de inicilização é igual à tensão complexa nodal do nó raiz de cada alimentador. Recebe como parâmetros de entrada e saída o grafo da
 * rede elétrica @p grafo com as tensṍes complexas inicializadas e como parâmertros de entrada os alimentadores @p alimentadores e quantidades
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
void incializaTensoesRaiz(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores){ 
    int i, k, fase;
    BOOL visitado[numeroBarras];
    __complex__ double V0[3], **Yaux;
    
    Yaux = c_matAloca(3);
    
    //Flat start trifásico (Va = Vb = Vc = 1p.u.  Ta = 0  Tb = -120  Tc = 120) - com busca em profundidade para atualizar taps iniciais
    for(i=0; i<numeroBarras; i++){ 
        visitado[i] = false;
    }
    for(i=0; i<numeroAlimentadores; i++)
    {
        //Tensão Inicial da subestação
        V0[0] = grafo[alimentadores[i].noRaiz].barra->Vinicial[0];//1.0*(cos(0) + I*sin(0));
        V0[1] = grafo[alimentadores[i].noRaiz].barra->Vinicial[1];//1.0*(cos(-120*PI/180) + I*sin(-120*PI/180));
        V0[2] = grafo[alimentadores[i].noRaiz].barra->Vinicial[2];//1.0*(cos(120*PI/180) + I*sin(120*PI/180));
        
        TF_FILABARRAS *barraAtual = &alimentadores[i].rnp[0];
        while(barraAtual != NULL)
        {
            grafo[barraAtual->idNo].V[0] = V0[0];
            grafo[barraAtual->idNo].V[1] = V0[1];
            grafo[barraAtual->idNo].V[2] = V0[2];
            
            
            visitado[barraAtual->idNo] = true;
            barraAtual = barraAtual->prox;
        }        
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
void incializaTensoesVarredura(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores){ 
    int i, k, fase;
    int de,para,n_adj;
    BOOL visitado[numeroBarras];
    __complex__ double V0[3], **Yaux;
    extern BOOL control_REG_OPT;
    TF_FILABARRAS *barraAtual;

    Yaux = c_matAloca(3);

    //Flat start trifásico (Va = Vb = Vc = 1p.u.  Ta = 0  Tb = -120  Tc = 120) - com busca em profundidade para atualizar taps iniciais
    for(i=0; i<numeroBarras; i++){ 
        visitado[i] = false;
    }
    for(i=0; i<numeroAlimentadores; i++)
    {
        //Tensão Inicial da subestação
        V0[0] = grafo[alimentadores[i].noRaiz].barra->Vinicial[0];//1.0*(cos(0) + I*sin(0));
        V0[1] = grafo[alimentadores[i].noRaiz].barra->Vinicial[1];//1.0*(cos(-120*PI/180) + I*sin(-120*PI/180));
        V0[2] = grafo[alimentadores[i].noRaiz].barra->Vinicial[2];//1.0*(cos(120*PI/180) + I*sin(120*PI/180));
        
        barraAtual = &alimentadores[i].rnp[0];
        
        de = barraAtual->idNo;
        grafo[de].V[0] = V0[0];
        grafo[de].V[1] = V0[1];
        grafo[de].V[2] = V0[2];
        visitado[de] = true;
        while(barraAtual != NULL)
        {
            de = barraAtual->idNo;
            n_adj = grafo[de].numeroAdjacentes;
            for(k=0;k< n_adj;k++){
                para = grafo[de].adjacentes[k].idNo;
                if (visitado[para] == false){ 
                    if ((grafo[de].adjacentes[k].tipo == 1)){ //Atualiza o V0 para trafo visto a ligação e tap
                        grafo[para].V[0] = grafo[de].V[0];
                        grafo[para].V[1] = grafo[de].V[1];
                        grafo[para].V[2] = grafo[de].V[2];
                        
                        if ((grafo[de].adjacentes[k].ramo->trafo.lig_pri == 1) && (grafo[de].adjacentes[k].ramo->trafo.lig_sec == 2)){                                
                            grafo[para].V[0] = cabs(grafo[de].V[0])*(cos(-30*PI/180) + I*sin(-30*PI/180));
                            grafo[para].V[1] = cabs(grafo[de].V[1])*(cos(-150*PI/180) + I*sin(-150*PI/180));
                            grafo[para].V[2] = cabs(grafo[de].V[2])*(cos(90*PI/180) + I*sin(90*PI/180));                                    
                        }
                        else if ((grafo[de].adjacentes[k].ramo->trafo.lig_pri == 3) && (grafo[de].adjacentes[k].ramo->trafo.lig_sec == 2)){                                
                            grafo[para].V[0] = cabs(grafo[de].V[0])*(cos(-30*PI/180) + I*sin(-30*PI/180));
                            grafo[para].V[1] = cabs(grafo[de].V[1])*(cos(-150*PI/180) + I*sin(-150*PI/180));
                            grafo[para].V[2] = cabs(grafo[de].V[2])*(cos(90*PI/180) + I*sin(90*PI/180));                                    
                        }
                        else if ((grafo[de].adjacentes[k].ramo->trafo.lig_pri == 2) && (grafo[de].adjacentes[k].ramo->trafo.lig_sec == 1)){
                            grafo[para].V[0] = cabs(grafo[de].V[0])*(cos(-30*PI/180) + I*sin(-30*PI/180));
                            grafo[para].V[1] = cabs(grafo[de].V[1])*(cos(-150*PI/180) + I*sin(-150*PI/180));
                            grafo[para].V[2] = cabs(grafo[de].V[2])*(cos(90*PI/180) + I*sin(90*PI/180));                                    
                        }            
                                    
                
                    }
                    else if (grafo[de].adjacentes[k].tipo == 2){ //Para o caso de regulador de tensão
                        if (control_REG_OPT == 1){
                            grafo[de].adjacentes[k].ramo->regulador.tap[0] = 0;
                            grafo[de].adjacentes[k].ramo->regulador.tap[1] = 0;
                            grafo[de].adjacentes[k].ramo->regulador.tap[2] = 0;

                            atualizaTapRegulador(grafo[de].adjacentes[k].ramo);
                        }
                        grafo[para].V[0] = grafo[de].V[0]*grafo[de].adjacentes[k].ramo->tap_pri[0]*grafo[de].adjacentes[k].ramo->tap_sec[0];
                        grafo[para].V[1] = grafo[de].V[1]*grafo[de].adjacentes[k].ramo->tap_pri[1]*grafo[de].adjacentes[k].ramo->tap_sec[1];
                        grafo[para].V[2] = grafo[de].V[2]*grafo[de].adjacentes[k].ramo->tap_pri[2]*grafo[de].adjacentes[k].ramo->tap_sec[2];
                    }
                    else{
                        grafo[para].V[0] = grafo[de].V[0];
                        grafo[para].V[1] = grafo[de].V[1];
                        grafo[para].V[2] = grafo[de].V[2];
                    }
                }
            }
            visitado[de] = true;
            barraAtual = barraAtual->prox;
        }    
        //----------------------------------------------------------------------
        //Atualiza Correntes Nodais - Modelo de Corrente Constante nas Tensões Inicializadas
        //
        barraAtual = &alimentadores[i].rnp[0];
        k=0;
        while(barraAtual != NULL){
            grafo[barraAtual->idNo].V_aux[0] = grafo[barraAtual->idNo].V[0];
            grafo[barraAtual->idNo].V_aux[1] = grafo[barraAtual->idNo].V[1];
            grafo[barraAtual->idNo].V_aux[2] = grafo[barraAtual->idNo].V[2];
            atualizaInjecoes(&grafo[barraAtual->idNo]);
            barraAtual = barraAtual->prox;
        }
    }
    
    
    
    
    for (i=0;i<3;i++) free(Yaux[i]);
    free(Yaux);
    free(barraAtual);
    
}

//Atualiza tap de regulador de tensão especicio no respectivo quadripólo de admitância
/**
 * @brief Função auxiliar que realiza a atualização dos valores de taps nas matrizes do quadripólo de admitâncias trifásico de regulador de tensão específico
 * 
 * Esta função atualiza as matrizes do quadripólo de admitâncias de um ramo específico @p ramos de acordo com os valores dos taps indicados 
 * dentro da estrutura de dados de reguladores de tensão deste ramo. Pode ser utilizada para atualizar os taps conforme
 * novas informações discretas são obtidas pelo sistema supervisório e conforme atualizações do controladores são calculadas automaticamente.
 * A função retorna @c void .
 * 
 *   
 * @param ramo ramo específico da rede elétrica cuja valor de tap do regulador foi atualizado
 * @return void
 * @see controleReguladorTensao_LDC
 * @see montaQuadripoloRegulador
 * @note 
 * @warning
 */
void atualizaTapRegulador(TF_DRAM *ramo){
    if (ramo->tipo == 2){
        // Tira efeito dos antigos valores do tap nas matrizes de quadripolo
            ramo->Ypp[0][0] = 1/pow(ramo->tap_pri[0],2)*ramo->Ypp[0][0];
            ramo->Ypp[1][1] = 1/pow(ramo->tap_pri[1],2)*ramo->Ypp[1][1];
            ramo->Ypp[2][2] = 1/pow(ramo->tap_pri[2],2)*ramo->Ypp[2][2];
            
            ramo->Yps[0][0] = 1/ramo->tap_sec[0]*1/ramo->tap_pri[0]*ramo->Yps[0][0];
            ramo->Yps[1][1] = 1/ramo->tap_sec[1]*1/ramo->tap_pri[1]*ramo->Yps[1][1];
            ramo->Yps[2][2] = 1/ramo->tap_sec[2]*1/ramo->tap_pri[2]*ramo->Yps[2][2];
            
            ramo->Ysp[0][0] = 1/ramo->tap_sec[0]*1/ramo->tap_pri[0]*ramo->Ysp[0][0];
            ramo->Ysp[1][1] = 1/ramo->tap_sec[1]*1/ramo->tap_pri[1]*ramo->Ysp[1][1];
            ramo->Ysp[2][2] = 1/ramo->tap_sec[2]*1/ramo->tap_pri[2]*ramo->Ysp[2][2];
            
            ramo->Yss[0][0] = 1/pow(ramo->tap_sec[0],2)*ramo->Yss[0][0];
            ramo->Yss[1][1] = 1/pow(ramo->tap_sec[1],2)*ramo->Yss[1][1];
            ramo->Yss[2][2] = 1/pow(ramo->tap_sec[2],2)*ramo->Yss[2][2];
    
    // Atualiaza os taps
            ramo->tap_pri[0] = (1 + ramo->regulador.tap[0]*ramo->regulador.regulacao/ramo->regulador.ntaps);
            ramo->tap_pri[1] = (1 + ramo->regulador.tap[1]*ramo->regulador.regulacao/ramo->regulador.ntaps);
            ramo->tap_pri[2] = (1 + ramo->regulador.tap[2]*ramo->regulador.regulacao/ramo->regulador.ntaps);
            
            ramo->tap_sec[0] = 1;
            ramo->tap_sec[1] = 1;
            ramo->tap_sec[2] = 1;
            
            ramo->Ypp[0][0] = pow(ramo->tap_pri[0],2)*ramo->Ypp[0][0];
            ramo->Ypp[1][1] = pow(ramo->tap_pri[1],2)*ramo->Ypp[1][1];
            ramo->Ypp[2][2] = pow(ramo->tap_pri[2],2)*ramo->Ypp[2][2];
            
            ramo->Yps[0][0] = ramo->tap_sec[0]*ramo->tap_pri[0]*ramo->Yps[0][0];
            ramo->Yps[1][1] = ramo->tap_sec[1]*ramo->tap_pri[1]*ramo->Yps[1][1];
            ramo->Yps[2][2] = ramo->tap_sec[2]*ramo->tap_pri[2]*ramo->Yps[2][2];
            
            ramo->Ysp[0][0] = ramo->tap_sec[0]*ramo->tap_pri[0]*ramo->Ysp[0][0];
            ramo->Ysp[1][1] = ramo->tap_sec[1]*ramo->tap_pri[1]*ramo->Ysp[1][1];
            ramo->Ysp[2][2] = ramo->tap_sec[2]*ramo->tap_pri[2]*ramo->Ysp[2][2];
            
            ramo->Yss[0][0] = pow(ramo->tap_sec[0],2)*ramo->Yss[0][0];
            ramo->Yss[1][1] = pow(ramo->tap_sec[1],2)*ramo->Yss[1][1];
            ramo->Yss[2][2] = pow(ramo->tap_sec[2],2)*ramo->Yss[2][2];
        }
}

//Função que atualiza as injeções de corrente nas barras da rede conforme as cargas e e respectivos modelos de carga, shunts e geradores distribuidos
/**
 * @brief Função auxiliar para atualizar as injecṍes de corrente complexas nodais trifásicas em determinado nó conforme as cargas, shunts e geradores distribuídos
 * 
 * Essa função atualiza as injeções de corrente complexas nodais por fase (fase-neutro) de determinada barra da rede elétrica especificada 
 * no parâmetro de entrada @p no , através da tensão complexa nodal atualizada e dos valores de potência e modelos dependentes de tensão de 
 * cargas, bancos de capacitores (shunts) e geradores distribuídos.
 * A função retorna @c void.
 * 
 * @param *no ponteiro para o nó específico do grafo cujas injeções de corrente nodais trifásicas serão atualizadas
 * @return void
 * @see
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void atualizaInjecoes(TF_GRAFO *no){
    int i, j;
    __complex__ double Saux[3], Iaux[3], IauxDelta[3], Vl[3], V0;

    no->Cur[0] = 0;
    no->Cur[1] = 0;
    no->Cur[2] = 0;
    no->S[0] = 0;
    no->S[1] = 0;
    no->S[2] = 0;
    
    // Atualiza Cargas
    for (i = 0; i<no->barra->nloads;i++){
//        no->barra->loads[i].ZIP = 2;
        switch (no->barra->loads[i].lig){
            case YN:
                Saux[0] = (no->barra->loads[i].Pnom[0] + I * no->barra->loads[i].Qnom[0]) * pow(cabs(no->V[0]) , no->barra->loads[i].ZIP);
                Saux[1] = (no->barra->loads[i].Pnom[1] + I * no->barra->loads[i].Qnom[1]) * pow(cabs(no->V[1]) , no->barra->loads[i].ZIP);
                Saux[2] = (no->barra->loads[i].Pnom[2] + I * no->barra->loads[i].Qnom[2]) * pow(cabs(no->V[2]) , no->barra->loads[i].ZIP);
                
                Iaux[0] = conj(Saux[0]/no->V[0]);
                Iaux[1] = conj(Saux[1]/no->V[1]);
                Iaux[2] = conj(Saux[2]/no->V[2]);
                
                break;
            case Y:
                V0 = no->V[0] + no->V[1] + no->V[2]; //tensão de neutro
                Saux[0] = (no->barra->loads[i].Pnom[0] + I * no->barra->loads[i].Qnom[0]) * pow(cabs(no->V[0] - V0) , no->barra->loads[i].ZIP);
                Saux[1] = (no->barra->loads[i].Pnom[1] + I * no->barra->loads[i].Qnom[1]) * pow(cabs(no->V[1] - V0) , no->barra->loads[i].ZIP);
                Saux[2] = (no->barra->loads[i].Pnom[2] + I * no->barra->loads[i].Qnom[2]) * pow(cabs(no->V[2] - V0) , no->barra->loads[i].ZIP);
                
                Iaux[0] = conj(Saux[0]/(no->V[0] - V0));
                Iaux[1] = conj(Saux[1]/(no->V[1] - V0));
                Iaux[2] = conj(Saux[2]/(no->V[2] - V0));
                
                break;
            case D:
                tensaoDelta(no->V, Vl);
                Saux[0] = (no->barra->loads[i].Pnom[0] + I * no->barra->loads[i].Qnom[0]) * pow(cabs(no->V[0]) , no->barra->loads[i].ZIP);
                Saux[1] = (no->barra->loads[i].Pnom[1] + I * no->barra->loads[i].Qnom[1]) * pow(cabs(no->V[1]) , no->barra->loads[i].ZIP);
                Saux[2] = (no->barra->loads[i].Pnom[2] + I * no->barra->loads[i].Qnom[2]) * pow(cabs(no->V[2]) , no->barra->loads[i].ZIP);
                
                IauxDelta[0] = conj(Saux[0]/Vl[0]);
                IauxDelta[1] = conj(Saux[1]/Vl[1]);
                IauxDelta[2] = conj(Saux[2]/Vl[2]);
                
                Iaux[0] = IauxDelta[0] - IauxDelta[2];
                Iaux[1] = IauxDelta[1] - IauxDelta[0];
                Iaux[2] = IauxDelta[2] - IauxDelta[1];
                
                break;
        }
        no->Cur[0] += Iaux[0];
        no->Cur[1] += Iaux[1];
        no->Cur[2] += Iaux[2];
        
        no->S[0] += no->V[0] * conj(Iaux[0]);
        no->S[1] += no->V[1] * conj(Iaux[1]);
        no->S[2] += no->V[2] * conj(Iaux[2]);
    } 
    
    
    // Atualiza Shunts
    for (i = 0; i<no->barra->nshunts;i++){
        switch (no->barra->shunts[i].lig){
            case YN:
                Saux[0] = ( I * no->barra->shunts[i].Qnom[0]) * pow(cabs(no->V[0]) , 2);
                Saux[1] = ( I * no->barra->shunts[i].Qnom[1]) * pow(cabs(no->V[1]) , 2);
                Saux[2] = ( I * no->barra->shunts[i].Qnom[2]) * pow(cabs(no->V[2]) , 2);
                
                Iaux[0] = conj(Saux[0]/no->V[0]);
                Iaux[1] = conj(Saux[1]/no->V[1]);
                Iaux[2] = conj(Saux[2]/no->V[2]);
                
                break;
            case Y:
                V0 = no->V[0] + no->V[1] + no->V[2]; //tensão de neutro
                Saux[0] = ( I * no->barra->shunts[i].Qnom[0]) * pow(cabs(no->V[0] - V0) , 2);
                Saux[1] = ( I * no->barra->shunts[i].Qnom[1]) * pow(cabs(no->V[1] - V0) , 2);
                Saux[2] = ( I * no->barra->shunts[i].Qnom[2]) * pow(cabs(no->V[2] - V0) , 2);
                
                Iaux[0] = conj(Saux[0]/(no->V[0] - V0));
                Iaux[1] = conj(Saux[1]/(no->V[1] - V0));
                Iaux[2] = conj(Saux[2]/(no->V[2] - V0));
                
                break;
            case D:
                tensaoDelta(no->V, Vl);
                Saux[0] = ( I * no->barra->shunts[i].Qnom[0]) * pow(cabs(no->V[0]) , 2);
                Saux[1] = ( I * no->barra->shunts[i].Qnom[1]) * pow(cabs(no->V[1]) , 2);
                Saux[2] = ( I * no->barra->shunts[i].Qnom[2]) * pow(cabs(no->V[2]) , 2);
                
                IauxDelta[0] = conj(Saux[0]/Vl[0]);
                IauxDelta[1] = conj(Saux[1]/Vl[1]);
                IauxDelta[2] = conj(Saux[2]/Vl[2]);
                
                Iaux[0] = IauxDelta[0] - IauxDelta[2];
                Iaux[1] = IauxDelta[1] - IauxDelta[0];
                Iaux[2] = IauxDelta[2] - IauxDelta[1];
                
                break;
        }
        no->Cur[0] += Iaux[0];
        no->Cur[1] += Iaux[1];
        no->Cur[2] += Iaux[2];
        
        no->S[0] += no->V[0] * conj(Iaux[0]);
        no->S[1] += no->V[1] * conj(Iaux[1]);
        no->S[2] += no->V[2] * conj(Iaux[2]);
    } 
    
    // Atualiza Geradores Distribuídos
    for (i = 0; i<no->barra->ngds;i++){
        switch (no->barra->gds[i].lig){
            case YN:
                Saux[0] = (no->barra->gds[i].Pnom[0] + I * no->barra->gds[i].Qnom[0]);
                Saux[1] = (no->barra->gds[i].Pnom[1] + I * no->barra->gds[i].Qnom[1]);
                Saux[2] = (no->barra->gds[i].Pnom[2] + I * no->barra->gds[i].Qnom[2]);
                
                Iaux[0] = conj(Saux[0]/no->V[0]);
                Iaux[1] = conj(Saux[1]/no->V[1]);
                Iaux[2] = conj(Saux[2]/no->V[2]);
                
                break;
            case Y:
                V0 = no->V[0] + no->V[1] + no->V[2]; //tensão de neutro
                Saux[0] = (no->barra->gds[i].Pnom[0] + I * no->barra->gds[i].Qnom[0]);
                Saux[1] = (no->barra->gds[i].Pnom[1] + I * no->barra->gds[i].Qnom[1]);
                Saux[2] = (no->barra->gds[i].Pnom[2] + I * no->barra->gds[i].Qnom[2]);
                
                Iaux[0] = conj(Saux[0]/(no->V[0] - V0));
                Iaux[1] = conj(Saux[1]/(no->V[1] - V0));
                Iaux[2] = conj(Saux[2]/(no->V[2] - V0));
                
                break;
            case D:
                tensaoDelta(no->V, Vl);
                Saux[0] = (no->barra->gds[i].Pnom[0] + I * no->barra->gds[i].Qnom[0]);
                Saux[1] = (no->barra->gds[i].Pnom[1] + I * no->barra->gds[i].Qnom[1]);
                Saux[2] = (no->barra->gds[i].Pnom[2] + I * no->barra->gds[i].Qnom[2]);
                
                IauxDelta[0] = conj(Saux[0]/Vl[0]);
                IauxDelta[1] = conj(Saux[1]/Vl[1]);
                IauxDelta[2] = conj(Saux[2]/Vl[2]);
                
                Iaux[0] = IauxDelta[0] - IauxDelta[2];
                Iaux[1] = IauxDelta[1] - IauxDelta[0];
                Iaux[2] = IauxDelta[2] - IauxDelta[1];
                
                break;
        }
        no->Cur[0] += Iaux[0];
        no->Cur[1] += Iaux[1];
        no->Cur[2] += Iaux[2];
        
        no->S[0] += no->V[0] * conj(Iaux[0]);
        no->S[1] += no->V[1] * conj(Iaux[1]);
        no->S[2] += no->V[2] * conj(Iaux[2]);
    }
}

//cálculo de tensão a jusante no quadripolo
/**
 * @brief Função auxiliar para atualizar as tensões nodais complexas na etapa Forward do cálculo de fluxo de potência por VDI
 * 
 * Essa função atualiza as tensões complexas nodais por fase (fase-neutro) partindo da tensão complexa trfiásica da barra a montante @p Vp 
 * para atualizar a tensão da barra a jusante @p Vs . O cálculo é realizado de acordo com a etapa Forward do cálculo de Fluxo de Potência 
 * por Varredura Direta/Inversa através da corrente complexa que flui da barra "p" para a barra "s" @p Ips e do modelo de quadripólo de admitância
 * trifásico do respectivo ramo ps @p ramo .
 * A função retorna @c void.
 * 
 * @param Vp tensões complexas nodais trifásica na barra a montante
 * @param Vs tensões complexas nodais trifásica na barra a jusante a ser atualizada
 * @param Ips correntes complexas trifásicas no ramo entre a barra a montante e barra a jusante
 * @param ramo estrutura de dados com informações do ramo que conecta as duas barras e respectivo modelo elétrico
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void calculaTensaoJusante(complex double *Vp, complex double *Vs, complex double *Ips, TF_DRAM *ramo){
    int i,j;
    complex double Vaux[3], Aux[3], **Y, **Yp, **Ys, Iaux[3], V0;
    BOOL singular1 = false, singular2 = false;
    
    Y = c_matAloca(3);
    Yp = c_matAloca(3);
    Ys = c_matAloca(3);
    
    Vaux[0]=0;
    Vaux[1]=0;
    Vaux[2]=0;
    
    //Verifica posto das submatrizes do quadripolo de acordo com a ligacao
    if(ramo->tipo == 1){
        switch (ramo->trafo.lig_pri){
            case 3:
            case 2:
                singular1 = true;
                break;
        }
        switch (ramo->trafo.lig_sec){
            case 2:
                singular2 = true;
                break;
        }

    }


    switch (ramo->tipo){
        case 0: //Linha
            //Calcula queda de tensão na linha através da impedância
            for(i=0;i<3;i++){
                for(j=0;j<3;j++){
                    Vaux[i] += ramo->B[i][j] * Vp[j];
                }
                Aux[i] = Ips[i] - Vaux[i];
            }
            for(i=0;i<3;i++){
                Vaux[i] = 0;
                for(j=0;j<3;j++){
                    Vaux[i] += ramo->Z[i][j] * Aux[j];
                }
                Vs[i] = Vp[i] - Vaux[i];
            }


            break;
        //----------------------------------------------------------------------
        case 1: //Trafo - está para o YN-YN
            V0 = 0;
            Iaux[0] = Ips[0];
            Iaux[1] = Ips[1];
            Iaux[2] = Ips[2];

            c_matIgual(Y, ramo->Yps, 3);
            c_matIgual(Yp, ramo->Ypp, 3);
            
            //tratamento de excessões das ligações de trafos com matriz de admitância singulares
            //--------------------------------------
            //Caso a matriz de quadripolo é singular 
            if (singular1) {
                //Calcula a tensão de neutro do secundário
                c_matIgual(Ys, ramo->Yss, 3);
                c_matInversaZ(Ys, 3);
                for(i=0;i<3;i++){
                    Vaux[i] = 0;
                    for(j=0;j<3;j++){
                        Vaux[i] += ramo->Ysp[i][j] * Vp[j];
                    }
                    Aux[i] = - Vaux[i];
                    //  Aux[i] = Isp[i] - Vaux[i]; // Falta pensar na corrente do secundário!
                }
                for(i=0;i<3;i++){
                    Vs[i] = 0;
                    for(j=0;j<3;j++){
                        Vs[i] += Ys[i][j] * Aux[j];
                    }
                    V0 += Vs[i]/3;
                }
                                
                for (i=0;i<3;i++){
                    Y[2][i] = 1;
                    Yp[2][i] = 0;
                }
                Iaux[2] = 0;
            }
            else if (singular2){
                V0 = 0; //Aproximação para caso de ligação em Delta no secundário - ver referência (Tratativa específica para calcular V0 no secundario - sistemas multiaterrados)
                for (i=0;i<3;i++){
                    Y[2][i] = 1;
                    Yp[2][i] = 0;
                }
                Iaux[2] = 0;
            }
                
            //---------------------------------------
            
            //Calcula Vs = Yps ^-1 * (Ips - Ypp * Vp)
            //Calcula a tensão nas três fases
            for(i=0;i<3;i++){
                for(j=0;j<3;j++){
                    Vaux[i] += Yp[i][j]*Vp[j];
                }
                Aux[i] = Iaux[i] - Vaux[i];
            }
            
            c_matInversaZ(Y, 3);
            for(i=0;i<3;i++){
                Vs[i] = 0;
                for(j=0;j<3;j++){
                    Vs[i] += Y[i][j] * Aux[j];
                }
                Vs[i] += V0;
                if (cabs(Vs[i]) == 0) Vs[i] = 1; //Caso em que não tem a fase resulta em V = 0 - coloca 1 para evitar NaN na hora de atualizar a corrente
            }

            break;
        case 2: //Regulador de Tensão
            //Calcula Vs = Yps ^-1 * (Ips - Ypp * Vp)
            //Calcula a tensão nas três fases
            for(i=0;i<3;i++){
                for(j=0;j<3;j++){
                    Vaux[i] += ramo->Ypp[i][j]*Vp[j];
                }
                Aux[i] = Ips[i] - Vaux[i];
            }
            //Inverte matriz Yps - dá para melhorar este ponto computacionalmente
            c_matIgual(Y, ramo->Yps, 3);

            //tratamento de excessões das ligações de trafos com matriz de admitância singulares

            c_matInversaZ(Y, 3);
            for(i=0;i<3;i++){
                Vs[i] = 0;
                for(j=0;j<3;j++){
                    Vs[i] += Y[i][j] * Aux[j];
                }
                if (cabs(Vs[i]) == 0) Vs[i] = 1; //Caso em que não tem a fase resulta em V = 0 - coloca 1 para evitar NaN na hora de atualizar a corrente
            }
            Vs[0] = ramo->tap_pri[0] * Vp[0];
            Vs[1] = ramo->tap_pri[1] * Vp[1];
            Vs[2] = ramo->tap_pri[2] * Vp[2]; 
            
            // printf("\n%lf  %lf\n", ramo->tap_pri[0], cabs(Vs[0]));
            break;
        case 3: //Chave
            Vs[0] = Vp[0];
            Vs[1] = Vp[1];
            Vs[2] = Vp[2];
            break; 
    }
    for(i = 0;i<3;i++) free(Y[i]);
    free(Y);

    for(i = 0;i<3;i++) free(Yp[i]);
    free(Yp);

    for(i = 0;i<3;i++) free(Ys[i]);
    free(Ys);
}

//Função que calcula as perdas nos ramos da rede elerica através de varredura backward
/**
 * @brief Função auxiliar para calcular as perdas elétrica em deteminado ramo da rede elétria através de varredura backward
 * 
 * Essa função calcula as perdas elétricas trifásicas no ramo a montante de um determinado nó da rede elétrica @p noP . É utilizada em conjunto com
 * uma varredura Backward para calcular as perdas elétricas em um alimentador completo utilizando informações da barra a montante obtida pelo grafo
 * @p grafo da rede elétrica. Retorna a soma das perdas de potência ativa nas três fases (perdas totais do ramo e não por fase).
 * A função retorna @c double.
 * 
 * @param noP barra especificada para as perdas a montante serem calculadas
 * @param grafo grafo da rede elétrica para busca do nó a montante
 * @return double perdas trifásicas totais (soma das três fases) no ramo a montante da barra indicada em noP
 * @see fluxoPotencia_BFS_Alimentador
 * @note Esta função é específica para ser utilizada em conjunto com uma varredura inversa (backward) para calcular as perdas em alimentadores específicos
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
double calculaPerdas(TF_GRAFO *noP, TF_GRAFO *grafo){
    long int i,j,noAdj,noMont = -1, auxNoMont = -1;
    complex double Smk[3], Perdas[3], Skm[3];
    double losses;

    for (j=0;j<3;j++) Perdas[j] = 0;
    for (j=0;j<3;j++) Skm[j] = 0;
    for (j=0;j<3;j++) Smk[j] = 0;
    
    for (i = 0; i < noP->numeroAdjacentes;i ++){
        noAdj = noP->adjacentes[i].idNo;
        
        if (noP->profundidade > grafo[noAdj].profundidade){
            noMont = noAdj;
            auxNoMont = i;
        }
    }
    
    //--------------------------------------------------------------------------
    //Calcula as perdas elétricas trifásicas no ramo em pu
    if (noMont != -1){
        Smk[0] = noP->V[0]*conj(noP->adjacentes[auxNoMont].Cur[0]);
        Smk[1] = noP->V[1]*conj(noP->adjacentes[auxNoMont].Cur[1]);
        Smk[2] = noP->V[2]*conj(noP->adjacentes[auxNoMont].Cur[2]); 
    
        for (i = 0; i < grafo[noMont].numeroAdjacentes;i ++){
            if (grafo[noMont].adjacentes[i].idNo == noP->idNo){
                Skm[0] = grafo[noMont].V[0]*conj(grafo[noMont].adjacentes[i].Cur[0]);
                Skm[1] = grafo[noMont].V[1]*conj(grafo[noMont].adjacentes[i].Cur[1]);
                Skm[2] = grafo[noMont].V[2]*conj(grafo[noMont].adjacentes[i].Cur[2]); 
            }
        }
        Perdas[0] = Skm[0] + Smk[0];
        Perdas[1] = Skm[1] + Smk[1];
        Perdas[2] = Skm[2] + Smk[2];
        losses = creal(Perdas[0] + Perdas[1] + Perdas[2]);

        noP->adjacentes[auxNoMont].losses = losses;
        grafo[noMont].adjacentes[i].losses = losses;
    }
    else
    {
        losses = 0;
    }
    
    
    return(cabs(losses));
}

//Função que calcula o carregamento em percentual nos ramos da rede elerica através de varredura backward
/**
 * @brief Função auxiliar para calcular o carregamento em deteminado ramo da rede elétria através de varredura backward
 * 
 * Essa função calcula o carregamento percentual trifásico no ramo a montante de um determinado nó da rede elétrica @p noP . É utilizada em 
 * conjunto com uma varredura Backward para calcular o carregamento dos ramos em um alimentador completo utilizando informações da barra a montante 
 * obtida pelo grafo @p grafo da rede elétrica. Retorna o maior carregamento entre as três fases no caso de cabos através do valor de ampacidade,
 * e no caso de transformadores e reguladores o carregamento nominal trfásico através da potência nominal.
 * A função retorna @c double.
 * 
 * @param noP barra especificada para o carregamento a montante ser calculado
 * @param grafo grafo da rede elétrica para busca do nó a montante
 * @param Sbase potência base da rede elétrica
 * @return double carregamento percentual no ramo a montante da barra indicada em noP
 * @see fluxoPotencia_BFS_Alimentador
 * @note Esta função é específica para ser utilizada em conjunto com uma varredura inversa (backward) para calcular o carregamento da rede em alimentadores específicos
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
double calculaCarregamento(TF_GRAFO *noP, TF_GRAFO *grafo, double Sbase){
    long int i,j,noAdj,noMont = -1, auxNoMont = -1;
    complex double Smk[3], Perdas[3], Skm[3], Ikm[3];
    double loading;
    
    for (i = 0; i < noP->numeroAdjacentes;i ++){
        noAdj = noP->adjacentes[i].idNo;
        
        if (noP->profundidade > grafo[noAdj].profundidade){
            noMont = noAdj;
            auxNoMont = i;
        }
    }
    
    //--------------------------------------------------------------------------
    //Atualiza o carregamento do ramo
    if (noMont != -1){
        for (i = 0; i < grafo[noMont].numeroAdjacentes;i ++){
            if (grafo[noMont].adjacentes[i].idNo == noP->idNo){
                Skm[0] = grafo[noMont].V[0]*conj(grafo[noMont].adjacentes[i].Cur[0]);
                Skm[1] = grafo[noMont].V[1]*conj(grafo[noMont].adjacentes[i].Cur[1]);
                Skm[2] = grafo[noMont].V[2]*conj(grafo[noMont].adjacentes[i].Cur[2]); 

                Ikm[0] = grafo[noMont].adjacentes[i].Cur[0] * Sbase / (sqrt(3) * grafo[noMont].Vbase);
                Ikm[1] = grafo[noMont].adjacentes[i].Cur[1] * Sbase / (sqrt(3) * grafo[noMont].Vbase);
                Ikm[2] = grafo[noMont].adjacentes[i].Cur[2] * Sbase / (sqrt(3) * grafo[noMont].Vbase);
                break;
            }
        }
        // printf("\n%lf   %d",noP->adjacentes[auxNoMont].ramo->ampacidade,noP->adjacentes[auxNoMont].tipo);
        switch (noP->adjacentes[auxNoMont].tipo){
            case 0:
                if (cabs(Ikm[0]) >= cabs(Ikm[1]) && cabs(Ikm[0]) >= cabs(Ikm[2]))
                    loading = cabs(Ikm[0])/noP->adjacentes[auxNoMont].ramo->ampacidade * 100;
                if (cabs(Ikm[1]) >= cabs(Ikm[0]) && cabs(Ikm[1]) >= cabs(Ikm[2]))
                    loading = cabs(Ikm[1])/noP->adjacentes[auxNoMont].ramo->ampacidade * 100;
                if (cabs(Ikm[2]) >= cabs(Ikm[1]) && cabs(Ikm[2]) >= cabs(Ikm[0]))
                    loading = cabs(Ikm[2])/noP->adjacentes[auxNoMont].ramo->ampacidade * 100;
//                printf("\n%d %d: %lf  %lf\n",noP->adjacentes[auxNoMont].ramo->DE, noP->adjacentes[auxNoMont].ramo->PARA,noP->adjacentes[auxNoMont].ramo->ampacidade, loading);
                break;
            case 1:
                if (cabs(Skm[0]) >= cabs(Skm[1]) && cabs(Skm[0]) >= cabs(Skm[2]))
                    loading = cabs(Skm[0])/noP->adjacentes[auxNoMont].ramo->Snominal * 100;
                if (cabs(Skm[1]) >= cabs(Skm[0]) && cabs(Skm[1]) >= cabs(Skm[2]))
                    loading = cabs(Skm[1])/noP->adjacentes[auxNoMont].ramo->Snominal * 100;
                if (cabs(Skm[2]) >= cabs(Skm[1]) && cabs(Skm[2]) >= cabs(Skm[0]))
                    loading = cabs(Skm[2])/noP->adjacentes[auxNoMont].ramo->Snominal * 100;
                break;
            case 2:
                if (cabs(Skm[0]) >= cabs(Skm[1]) && cabs(Skm[0]) >= cabs(Skm[2]))
                    loading = cabs(Skm[0])/noP->adjacentes[auxNoMont].ramo->Snominal * 100;
                if (cabs(Skm[1]) >= cabs(Skm[0]) && cabs(Skm[1]) >= cabs(Skm[2]))
                    loading = cabs(Skm[1])/noP->adjacentes[auxNoMont].ramo->Snominal * 100;
                if (cabs(Skm[2]) >= cabs(Skm[1]) && cabs(Skm[2]) >= cabs(Skm[0]))
                    loading = cabs(Skm[2])/noP->adjacentes[auxNoMont].ramo->Snominal * 100;
                
//                printf("\n%d %d: %lf  %lf\n",noP->adjacentes[auxNoMont].ramo->DE, noP->adjacentes[auxNoMont].ramo->PARA,noP->adjacentes[auxNoMont].ramo->Snominal, loading);
                break;
            case 3:
                if (cabs(Ikm[0]) >= cabs(Ikm[1]) && cabs(Ikm[0]) >= cabs(Ikm[2]))
                    loading = cabs(Ikm[0])/noP->adjacentes[auxNoMont].ramo->ampacidade * 100;
                if (cabs(Ikm[1]) >= cabs(Ikm[0]) && cabs(Ikm[1]) >= cabs(Ikm[2]))
                    loading = cabs(Ikm[1])/noP->adjacentes[auxNoMont].ramo->ampacidade * 100;
                if (cabs(Ikm[2]) >= cabs(Ikm[1]) && cabs(Ikm[2]) >= cabs(Ikm[0]))
                    loading = cabs(Ikm[2])/noP->adjacentes[auxNoMont].ramo->ampacidade * 100;
                break;
        }
        noP->adjacentes[auxNoMont].loading = loading;
        grafo[noMont].adjacentes[i].loading = loading;
    }
    else
    {
        loading = 0;
    }
    
    
    return(loading);
}


//Função que efetua a etapa backward
/**
 * @brief Função auxiliar para efetuar os cálculos individuais em cada ramo da rede elétrica da Varredura Inversa (Etapa Backward) do cálculo de Fluxo de Potência
 * 
 * Essa função efetua os cálculos elétricos da etapa Backward (varredura inversa) no cálculo do fluxo de potência. Atualiza a corrente no ramo
 * a montante de um determinado nó da rede elétrica @p noP . É utilizada em conjunto com uma varredura Backward para calcular a corrente complexa trifásica
 * dos ramos em um alimentador completo utilizando informações da barra a montante obtida pelo grafo @p grafo da rede elétrica. 
 * Atualiza dentro da estrutrua de dados NOADJCAENTE do grafo os valores de corrente do ramo a montante do noP.
 * A função retorna @c void.
 * 
 * @param noP barra especificada para a corrente a montante ser calculada
 * @param grafo grafo da rede elétrica para busca do nó a montante e atualizar a corrente do ramo
 * @return void
 * @see fluxoPotencia_BFS_Alimentador
 * @note Esta função é específica para ser utilizada em conjunto com uma varredura inversa (backward) 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void backward(TF_GRAFO *noP, TF_GRAFO *grafo){
    long int i,j,noAdj,noMont = -1, auxNoMont = -1;
    complex double Iacc[3], Iaux[3], Vaux[3], **Yaux;
    BOOL singular = false;
    complex double V0;
    
    Yaux = c_matAloca(3);
    
    //Soma correntes à jusante e identifica o nó a montante
    Iacc[0] = 0;
    Iacc[1] = 0;
    Iacc[2] = 0;
    
    int radial = 0;
    for (i = 0; i < noP->numeroAdjacentes;i ++){
        noAdj = noP->adjacentes[i].idNo;
        
        if ((noP->profundidade < grafo[noAdj].profundidade)&& (grafo[noAdj].idAlim == noP->idAlim)){
            Iacc[0] += noP->adjacentes[i].Cur[0];
            Iacc[1] += noP->adjacentes[i].Cur[1];
            Iacc[2] += noP->adjacentes[i].Cur[2];
        }
        else if ((noP->profundidade > grafo[noAdj].profundidade) && (grafo[noAdj].idAlim == noP->idAlim)){
            noMont = noAdj;
            auxNoMont = i;
            radial++;
        }
    }
    if (noMont == -1){ //Nó raiz - corrente é a soma dos jusantes somente
        noP->Cur[0] = -Iacc[0];
        noP->Cur[1] = -Iacc[1];
        noP->Cur[2] = -Iacc[2];
    }
    //Soma injeção de corrente na barra
    Iacc[0] += noP->Cur[0];
    Iacc[1] += noP->Cur[1];
    Iacc[2] += noP->Cur[2];
    
    //--------------------------------------------------------------------------
    //Atualiza a corrente no nó a montante
    if (noMont != -1){
        noP->adjacentes[auxNoMont].Cur[0] = -Iacc[0];
        noP->adjacentes[auxNoMont].Cur[1] = -Iacc[1];
        noP->adjacentes[auxNoMont].Cur[2] = -Iacc[2]; 
        
        //Verifica posto das submatrizes do quadripolo de acordo com a ligacao
        if(noP->adjacentes[auxNoMont].tipo == 1){
            switch (noP->adjacentes[auxNoMont].ramo->trafo.lig_sec){
                case 2:
                case 3:
                    singular = true;
                    break;
            }

        }
        //Calcula a corrente montante de acordo com o tipo de ramo
        switch (noP->adjacentes[auxNoMont].tipo){
            //------------------------------------------------------------------
            case 0: //Linha

                for (i=0;i<3;i++){
                    Iaux[i] = 0;
                    for (j=0;j<3;j++){
                        Iaux[i] += noP->adjacentes[auxNoMont].ramo->B[i][j] * noP->V[j] + noP->adjacentes[auxNoMont].ramo->B[i][j] * grafo[noMont].V[j];
                    }
                }
                
                Iacc[0] = Iacc[0] + Iaux[0];
                Iacc[1] = Iacc[1] + Iaux[1];
                Iacc[2] = Iacc[2] + Iaux[2];
                break;
            //------------------------------------------------------------------
            case 1: //Trafo 
                V0 = 0;
                Iacc[0] = Iacc[0];
                Iacc[1] = Iacc[1];
                Iacc[2] = Iacc[2];
                
                //Injeção de corrente no secundário
                //I = (-Iacc - traf.Yss*Vsec); 
                for (i=0;i<3;i++){
                    Iaux[i] = 0;
                    for (j=0;j<3;j++){
                        Iaux[i] += noP->adjacentes[auxNoMont].ramo->Yss[i][j] * noP->V[j];
                    }
                    Iaux[i] = -Iacc[i] - Iaux[i];
                }
                c_matIgual(Yaux, noP->adjacentes[auxNoMont].ramo->Ysp, 3);
                
                //--------------------------------------
                //Caso a matriz de quadripolo é singular 
                if (singular) {
                    for (i=0;i<3;i++){
                        V0 += grafo[noMont].V[i]/3;
                        Yaux[2][i] = 1;
                    }
                    Iaux[2] = 0;
                }
                //---------------------------------------
                
                //Vpm = traf.Ysp\(-Iacc - traf.Yss*Vsec);
                c_matInversaZ(Yaux, 3);
                for (i=0;i<3;i++){
                    Vaux[i] = 0;
                    for (j=0;j<3;j++){
                        Vaux[i] += Yaux[i][j] * Iaux[j];
                    }
                    Vaux[i] += V0; //quando é Ysp singular soma a tensão de sequência zero - se não for singular V0 = 0
                }
                
                // traf.Ikm = (traf.Ypp*Vpm + traf.Yps*Vsec);
                for (i=0;i<3;i++){
                    Iacc[i] = 0;
                    for (j=0;j<3;j++){
                        Iacc[i] += noP->adjacentes[auxNoMont].ramo->Ypp[i][j] * Vaux[j] + noP->adjacentes[auxNoMont].ramo->Yps[i][j] * noP->V[j];
                    }
                }
                break;
            //------------------------------------------------------------------
            case 2: //Regulador de Tensão
                Iacc[0] = noP->adjacentes[auxNoMont].ramo->tap_pri[0]*Iacc[0];
                Iacc[1] = noP->adjacentes[auxNoMont].ramo->tap_pri[1]*Iacc[1];
                Iacc[2] = noP->adjacentes[auxNoMont].ramo->tap_pri[2]*Iacc[2]; 
                break;
            //------------------------------------------------------------------
            case 3: //Chave
                Iacc[0] = Iacc[0];
                Iacc[1] = Iacc[1];
                Iacc[2] = Iacc[2];                
                break; 
        } 
        //Atualiza a corrente a montante no grafo
        for (i = 0; i < grafo[noMont].numeroAdjacentes;i ++){
            if (grafo[noMont].adjacentes[i].idNo == noP->idNo){
                grafo[noMont].adjacentes[i].Cur[0] = Iacc[0];
                grafo[noMont].adjacentes[i].Cur[1] = Iacc[1];
                grafo[noMont].adjacentes[i].Cur[2] = Iacc[2];
//                if (grafo[noMont].idAlim == 254){
//                    printf("%d    %lf\n",noMont,creal(grafo[noMont].adjacentes[i].Cur[0]));
//                }
            }
            // imprimeCorrentes(&grafo[noMont].adjacentes[i]);
        }
    }

    for(i = 0;i<3;i++) free(Yaux[i]);
    free(Yaux);
}

//Função que atualiza controle de reguladores de tensão de acordo com parametrização do LDC
/**
 * @brief Função auxiliar para o controle automático de taps em reguladores de tensão no cálculo de fluxo de potência
 * 
 * Essa função efetua o cálculo de atualizações de taps em reguladores de tensão com comutação de tap sob carga (OLTC) para controle de tensão em sistemas
 * de distribuição. Atualiza durante a Varredura Direta o valor de tap conforme o modelo detalhado do controlador de comutação com representação do
 * compensador de queda na linha (LDC) em diferentes modos de controle: (0 = Somente Forward sem Restrição / 1 = Locked Forward / 2 = Locked Reverse / 
 * 3 = Bidirectional / 4 = Idle / 5 = Neutral Reverse / 6 = Cogenartion / Ver manual Siemens MJ4A). Recebe como parâmetros de entrada as tensões complexas
 * nodais trifásicas nos terminais do regulador @p Vp e @p Vs , as correntes nos terminais do regulador @p Ips e @p Isp , os valores de tensão
 * base @p Vbase e corrente base @p Ibase para representar a transdução do sensores do controlador, e a estrutura de dados com @p ramos que aramazena
 * internamente em um tipo DREG as informações detalhadas do controlador e respectivo regulador de tensão.
 * Detalhes deste modelo ver referência: 
 * J. A. D. Massignan, B. R. Pereira and J. B. A. London, "Load Flow Calculation with Voltage Regulators Bidirectional Mode and Distributed 
 * Generation," in IEEE Transactions on Power Systems, vol. 32, no. 2, pp. 1576-1577, March 2017, doi: 10.1109/TPWRS.2016.2576679.
 * Retorna um booleano indicando se houve alteração de controladores, no caso de reguladores de tensão com tap automático.
 * A função retorna @c BOOL.
 * 
 * @param Vbase tensão base do regulador de tensão
 * @param Ibase corrente base do regulador de tensão
 * @param Vp tensão complexa nodal trifásica no terminal a montante do regulador
 * @param Vs tensão complexa nodal trifásica no terminal a jusante do regulador
 * @param Ips corrente complexa trifásica no terminal a montante do regulador
 * @param Isp corrente complexa trifásica no terminal a jusante do regulador
 * @param ramo estrutura de dados com informações dos parâmetros e controlador do regulador de tensão
 * @return BOOL indicador de alteração em controladores de tensão
 * @see forward
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL controleReguladorTensao_LDC(double Vbase, double Ibase, complex double *Vp, complex double *Vs, complex double *Ips, complex double *Isp, TF_DRAM *ramo){
    int i;
    complex double   Zcont[3], Zcont_r[3];
    BOOL ctr_act;
    double Vdiff[3], taps_0[3], Vset[3], Vset_r[3];
    int Direction[3];

    // printf("\n Regulador %d - %d \n",ramo->DE, ramo->PARA);

    //Parâmetros do controlador LDC
    double reg = ramo->regulador.regulacao / ramo->regulador.ntaps;

    Zcont[0] = ramo->regulador.R1 + I* ramo->regulador.X1;
    Zcont[1] = ramo->regulador.R2 + I* ramo->regulador.X2;
    Zcont[2] = ramo->regulador.R3 + I* ramo->regulador.X3;

    Zcont_r[0] = ramo->regulador.R1r + I* ramo->regulador.X1r;
    Zcont_r[1] = ramo->regulador.R2r + I* ramo->regulador.X2r;
    Zcont_r[2] = ramo->regulador.R3r + I* ramo->regulador.X3r;

    Vset[0] = ramo->regulador.V1;
    Vset[1] = ramo->regulador.V2;
    Vset[2] = ramo->regulador.V3;
    
    Vset_r[0] = ramo->regulador.V1r;
    Vset_r[1] = ramo->regulador.V2r;
    Vset_r[2] = ramo->regulador.V3r;

    for (i=0;i<3;i++) taps_0[i] = ramo->regulador.tap[i];

    //Calcula direção do fluxo
    for (i=0;i<3;i++) {
        if ((creal(Vs[i] * conj(Isp[i])) && (cimag(Vs[i] * conj(Isp[i])) >= 0)) >= 0) Direction[i] = 1;
        else Direction[i] = 0;
    }
    // printf("\n\n");
    //Malha de Controle
    switch (ramo->regulador.controle){
        //------------------------------------------------------
        // CONTROLE SEM RESTRIÇÃO COPEL - Controle da barra a jusante com valores em p.u. - set point igual a 1 p.u. - específico COPEL
        case 99 :
            for (i=0;i<3;i++) Vdiff[i] = 1 - cabs(Vs[i]);

            for (i=0;i<3;i++){
                // printf("\t Vdiff[%d]= %lf",i, Vdiff[i]);
                // printf("\t tap[%d]= %lf",i, ramo->regulador.tap[i]);
                if (Vdiff[i] >= ramo->regulador.deltaV) {
                    ramo->regulador.tap[i] = ramo->regulador.tap[i] + ceil((Vdiff[i] - ramo->regulador.deltaV)/(ramo->regulador.regulacao/ramo->regulador.ntaps));
//                    printf("\n tap[%d]= %lf\n",i, (Vdiff[i] - ramo->regulador.deltaV)/(ramo->regulador.regulacao/ramo->regulador.ntaps));
                }
                else if (Vdiff[i] <= -ramo->regulador.deltaV) 
                    ramo->regulador.tap[i] = ramo->regulador.tap[i] + floor((Vdiff[i] + ramo->regulador.deltaV)/(ramo->regulador.regulacao/ramo->regulador.ntaps));
                // printf("\t tap[%d]= %lf",i, ramo->regulador.tap[i]);
            }

            break;
        
        //------------------------------------------------------
        // CONTROLE SEM RESTRIÇÃO
        case 0 :
            for (i=0;i<3;i++) Vdiff[i] = Vset[i] - cabs(Vbase*Vs[i]/ramo->regulador.TP - Zcont[i] * Ibase / sqrt(3)* ( - Isp[i])/ramo->regulador.TC);

            for (i=0;i<3;i++){
                // printf("\t Vdiff[%d]= %lf",i, Vdiff[i]);
                // printf("\t tap[%d]= %lf",i, ramo->regulador.tap[i]);
                if (Vdiff[i] >= ramo->regulador.deltaV) 
                    ramo->regulador.tap[i] = ramo->regulador.tap[i] + ceil((Vdiff[i] - ramo->regulador.deltaV)/(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                else if (Vdiff[i] <= -ramo->regulador.deltaV) 
                    ramo->regulador.tap[i] = ramo->regulador.tap[i] + floor((Vdiff[i] + ramo->regulador.deltaV)/(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                // printf("\t tap[%d]= %lf",i, ramo->regulador.tap[i]);
            }

            break;
        //------------------------------------------------------
        // CONTROLE LOCKED FORWARD
        case 1 :
            for (i=0;i<3;i++) Vdiff[i] = Vset[i] - cabs(Vbase*Vs[i]/ramo->regulador.TP - Zcont[i] * Ibase * ( - Isp[i])/ramo->regulador.TC);

            for (i=0;i<3;i++){
                if (Direction[i] ==1){
                    if (Vdiff[i] >= ramo->regulador.deltaV) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] + ceil((Vdiff[i] - ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                    else if (Vdiff[i] <= -ramo->regulador.deltaV) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] + floor((Vdiff[i] + ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                }
                else{
                    ramo->regulador.tap[i] = ramo->regulador.tap_ini[i];
                }
            }
            break;
        //------------------------------------------------------
        // CONTROLE LOCKED REVERSE
        case 2 :
            for (i=0;i<3;i++) Vdiff[i] = Vset_r[i] - cabs(Vbase*Vp[i]/ramo->regulador.TP - Zcont_r[i] * Ibase * ( -Ips[i])/ramo->regulador.TC);

            for (i=0;i<3;i++){
                if (Direction[i] ==1){
                    ramo->regulador.tap[i] = ramo->regulador.tap_ini[i];
                }
                else{
                    if (Vdiff[i] >= ramo->regulador.deltaVr) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] - ceil((Vdiff[i] - ramo->regulador.deltaVr)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                    else if (Vdiff[i] <= -ramo->regulador.deltaVr) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] - floor((Vdiff[i] + ramo->regulador.deltaVr)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                }
            }
            break;
        //------------------------------------------------------
        // CONTROLE BIDIRECTIONAL 
        case 3:
            for (i=0;i<3;i++){
                if (Direction[i] == 1)
                    Vdiff[i] = Vset[i] - cabs(Vs[i]/ramo->regulador.TP - Zcont[i] * Isp[i]/ramo->regulador.TC);
                else
                    Vdiff[i] = Vset_r[i] - cabs(Vp[i]/ramo->regulador.TP - Zcont_r[i] * Ips[i]/ramo->regulador.TC);                    
            }

            for (i=0;i<3;i++){
                if (Direction[i] ==1){
                    if (Vdiff[i] >= ramo->regulador.deltaV) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] + ceil((Vdiff[i] - ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                    else if (Vdiff[i] <= -ramo->regulador.deltaV) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] + floor((Vdiff[i] + ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                }
                else{
                    if (Vdiff[i] >= ramo->regulador.deltaVr) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] - ceil((Vdiff[i] - ramo->regulador.deltaVr)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                    else if (Vdiff[i] <= -ramo->regulador.deltaVr) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] - floor((Vdiff[i] + ramo->regulador.deltaVr)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                
                }
            }
            break;
        //------------------------------------------------------
        // CONTROLE IDLE
        case 4 :
            for (i=0;i<3;i++) Vdiff[i] = Vset[i] - cabs(Vs[i]/ramo->regulador.TP - Zcont[i] * Isp[i]/ramo->regulador.TC);

            for (i=0;i<3;i++){
                if (Direction[i] ==1){
                    if (Vdiff[i] >= ramo->regulador.deltaV) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] + ceil((Vdiff[i] - ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                    else if (Vdiff[i] <= -ramo->regulador.deltaV) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] + floor((Vdiff[i] + ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                }
                else{
                    ramo->regulador.tap[i] = ramo->regulador.tap_ini[i];
                }
            }
            break;
        //------------------------------------------------------
        // CONTROLE NEUTRAL REVERSE  
        case 5:
            for (i=0;i<3;i++) Vdiff[i] = Vset[i] - cabs(Vs[i]/ramo->regulador.TP - Zcont[i] * Isp[i]/ramo->regulador.TC);

            for (i=0;i<3;i++){
                if (Direction[i] ==1){
                    if (Vdiff[i] >= ramo->regulador.deltaV) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] + ceil((Vdiff[i] - ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                    else if (Vdiff[i] <= -ramo->regulador.deltaV) 
                        ramo->regulador.tap[i] = ramo->regulador.tap[i] + floor((Vdiff[i] + ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                }
                else{
                    ramo->regulador.tap[i] = 0;
                }
            }
            break;
        //------------------------------------------------------
        // CONTROLE COGENERATION 
        case 6:
            for (i=0;i<3;i++){
                if (Direction[i] == 1)
                    Vdiff[i] = Vset[i] - cabs(Vs[i]/ramo->regulador.TP - Zcont[i] * Isp[i]/ramo->regulador.TC);
                else
                    Vdiff[i] = Vset[i] - cabs(Vs[i]/ramo->regulador.TP - Zcont_r[i] * Isp[i]/ramo->regulador.TC);                    
            }

            for (i=0;i<3;i++){                
                if (Vdiff[i] >= ramo->regulador.deltaV) 
                    ramo->regulador.tap[i] = ramo->regulador.tap[i] + ceil((Vdiff[i] - ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
                else if (Vdiff[i] <= -ramo->regulador.deltaV) 
                    ramo->regulador.tap[i] = ramo->regulador.tap[i] + floor((Vdiff[i] + ramo->regulador.deltaV)*(reg*ramo->regulador.Vnom/pow(3,0.5)/ramo->regulador.TP));
            }
            break;
    }
    // Verifica limites de corrente
    
    
    // Verifica limites de tap
    for (i=0;i<3;i++){
        if ((ramo->regulador.tap[i] > ramo->regulador.ntaps) && (Direction[i] == 1))
            ramo->regulador.tap[i] = ramo->regulador.ntaps;
        else if ((ramo->regulador.tap[i] < -ramo->regulador.ntaps) && (Direction[i] == 1))
            ramo->regulador.tap[i] = -ramo->regulador.ntaps;
        else if ((ramo->regulador.tap[i] > ramo->regulador.ntaps) && (Direction[i] == -1))
            ramo->regulador.tap[i] = -ramo->regulador.ntaps;
        else if ((ramo->regulador.tap[i] < ramo->regulador.ntaps) && (Direction[i] == -1))
            ramo->regulador.tap[i] = ramo->regulador.ntaps;
    }

    // Indica mudança de tap
    ctr_act = 0;
    for (i=0;i<3;i++){
        if (ramo->regulador.tap[i] != taps_0[i]) ctr_act = 1;
    }
    
    // Atualiza akm
    atualizaTapRegulador(ramo);

    return(ctr_act);
}

//Função que efetua a etapa forward
/**
 * @brief Função auxiliar para efetuar os cálculos individuais em cada ramo da rede elétrica da Varredura Direta (Etapa Forward) do cálculo de Fluxo de Potência
 * 
 * Essa função efetua os cálculos elétricos da etapa Forward (varredura direta) no cálculo do fluxo de potência. Atualiza a tensão complexa nodal nos nós 
 * a jusante de um determinado nó da rede elétrica @p noP . É utilizada em conjunto com uma varredura Forward para calcular as tensões complexas trifásica
 * dos ramos em um alimentador completo utilizando informações das barras a jusante obtida pelo grafo @p grafo da rede elétrica. 
 * Atualiza atraveś da estrutrua de dados NOADJCAENTE do grafo os valores de tensão complexa armazenados no TF_GRAFO @p grafo . Retorna um booleano
 * indicando se houve alteração de controladores, no caso de reguladores de tensão com tap automático.
 * A função retorna @c BOOL.
 * 
 * @param noP barra especificada a montante para atualizar as tensões complexas nodais a jusante 
 * @param grafo grafo da rede elétrica para busca dos nós a jusante para atualizar as tensões
 * @return BOOL indicador de alteração em controladores de tensão
 * @see fluxoPotencia_BFS_Alimentador
 * @note Esta função é específica para ser utilizada em conjunto com uma varredura direta (forward) 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL forward(TF_GRAFO *noP, TF_GRAFO *grafo){
    int i, noAdj, idx;
    complex double Vaux[3];
    BOOL control_action = 0; //variável para indicar se houve transição por parte dos controladores
    BOOL return_control = 0; //variável para indicar se houve transição por parte dos controladores
    
    extern double Sbase;
    double Ibase;

    extern BOOL control_REG_OPT;
    extern BOOL control_CAP_OPT;

    for (i = 0; i < noP->numeroAdjacentes;i ++){
        noAdj = noP->adjacentes[i].idNo;
        Vaux[0] = grafo[noAdj].V[0];
        Vaux[1] = grafo[noAdj].V[1];
        Vaux[2] = grafo[noAdj].V[2];
        if ((noP->profundidade < grafo[noAdj].profundidade) && (grafo[noAdj].idAlim == noP->idAlim)){
            calculaTensaoJusante(noP->V, Vaux, noP->adjacentes[i].Cur, noP->adjacentes[i].ramo);

            grafo[noAdj].V[0] = Vaux[0];
            grafo[noAdj].V[1] = Vaux[1];
            grafo[noAdj].V[2] = Vaux[2]; 

            //------------------------------------------------------------------
            //Atualiza TAPs de acordo com o controlador LDC do regulador
            if (control_REG_OPT == 1){
                if(noP->adjacentes[i].tipo == 2){
                    //Atualiza controle de taps do regulador de tensão
                    for (int j = 0; j < grafo[noAdj].numeroAdjacentes;j ++){
                        if (grafo[noAdj].adjacentes[j].idNo == noP->idNo){
                            idx = j;
                        }
                    }
                    control_action = controleReguladorTensao_LDC(noP->Vbase, Sbase/noP->Vbase, noP->V, Vaux, noP->adjacentes[i].Cur, grafo[noAdj].adjacentes[idx].Cur, noP->adjacentes[i].ramo);
                    if (control_action){
                        return_control = true;
                        calculaTensaoJusante(noP->V, Vaux, noP->adjacentes[i].Cur, noP->adjacentes[i].ramo);
                        grafo[noAdj].V[0] = Vaux[0];
                        grafo[noAdj].V[1] = Vaux[1];
                        grafo[noAdj].V[2] = Vaux[2];
                    }
                    //printf("\nLDC %d\n",control_action);
                }
            }
            //------------------------------------------------------------------
            //Atualiza Bancos de Capacitor Chaveados com os controladores de Bancos de Capacitor
            if (control_CAP_OPT == 1){

            }
            // //------------------------------------------------------------------
            // //Atualiza Geradores Distribuídos com os controladores de Tensão (Futuro)
            // if (control_GD_OPT == 1){

            // }

        }
    }
    return (return_control);
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
TF_PFSOLUTION fluxoPotencia_BFS_Alimentador(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR alimentador, TF_DRAM *ramos,double Sbase){
    int it, nvar, k = 0,i, conv = 0;
    double *DV, nDV = 0, loading = 0, cargaAtivaTotal = 0 ;
    long int *RNP = NULL;
    BOOL control_action = 0;
    double fatorDesbalanco;
    double tempoBFS;

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
//        printf("\n%d    %d\t\t%d",k, barraAtual->idNo,barraAtual->profundidade );
        k++;
        barraAtual = barraAtual->prox;
    }
    nvar = alimentador.numeroNos*6;
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
    powerflow_result.menorTensaoABC[0] = 1.0;
    powerflow_result.menorTensaoABC[1] = 1.0;
    powerflow_result.menorTensaoABC[2] = 1.0;
     
    //Correntes já são inicializadas na função de inicalização de Tensões

       
    ////----------------------------------------------------------------------
    // Fluxo de  Potência por Varredura Direta/Inversa via Soma de Correntes
    for (it = 0;it < MAXIT; it ++){
        //---------------------------------------------------------------------- 
        //Tensões Anteriores para Conevrgência
        barraAtual = &alimentador.rnp[0];
        k=0;
        while(barraAtual != NULL){
        //    atualizaInjecoes(&grafo[barraAtual->idNo]); // Carga como potência constanta, se comentar a carga fica como Corrente constante
            for(i=0;i<3;i++){
                DV[k] = cabs(grafo[barraAtual->idNo].V[i]);
                k++;
                DV[k] = carg(grafo[barraAtual->idNo].V[i]);
                k++;                      
            }
            barraAtual = barraAtual->prox;
        }
        //---------------------------------------------------------------------- 
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
        
        
        //Critério de Convergência
        barraAtual = &alimentador.rnp[0];
        k=0;
        while(barraAtual != NULL){
            for(i=0;i<3;i++){
                DV[k] = DV[k] - cabs(grafo[barraAtual->idNo].V[i]);
                k++;
                DV[k] = DV[k] - carg(grafo[barraAtual->idNo].V[i]);
                k++;                        
            }
            barraAtual = barraAtual->prox;
        }
        
        
        nDV = norma_inf(DV,nvar);
        if ((fabs(nDV) < tolerance) && !control_action){
            conv = 1;
            powerflow_result.convergencia = 1;
            powerflow_result.iteracoes = it + 1;
            break;
        }
    }
    

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
    
    free(DV); free(RNP); free(barraProx); free(barraAtual); free(ramoAdj);
    return(powerflow_result);
}

//Rotina para Cálculo de Fluxo de Potência em múltiplos alimentadores de um sistema de distribuição
/**
 * @brief Função principal para a leitura de uma pasta com os dados da rede elétrica.
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
void fluxoPotencia_BFS_Multiplos(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos,double Sbase){
    long int nmed,nvar,nmedTotal;
    int i,j, idAlim, it, n_threads = 2;
    TF_FILABARRAS *barraAtual = NULL;
    TF_GRAFO *no  = NULL;

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

    // printf("Calculo de Fluxo de Potencia via Varredura Direta/Inversa...\n");
    incializaTensoesVarredura(grafo, numeroBarras, alimentadores, numeroAlimentadores);
//    incializaTensoesRaiz(grafo, numeroBarras, alimentadores, numeroAlimentadores);
    
    //Calculo de fluxo de potência para todos os alimentadores individualmente
    double tol = 0.000001;
        
//    double tIni = omp_get_wtime(); 
    // Para execução em paralelo
//     n_threads =  omp_get_num_procs();
//
//     #pragma omp parallel num_threads(n_threads - 1)
//     {
//         #pragma omp for 
        for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
//            if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 == 34.5){
//                printf("\n alimentador %d Raiz: %ld   Nos: %ld  %.1lf kV\n",idAlim,alimentadores[idAlim].idRaiz,alimentadores[idAlim].numeroNos,grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000);
                powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
//            }
            // printf("...\n");
            
            // if (powerflow_result[idAlim].iteracoes < MAXIT) printf("\nConvergencia em %d iteracoes \n",powerflow_result[idAlim].iteracoes);
            // else printf("\nNumero maximo de iteracoes atingido %d\n", MAXIT);
            
        }//Fim do loop dos alimentadores  
//     }
    
    
    
    FILE *arq_alim;
    arq_alim = fopen("resultadoAlim.csv","w+");
    
    fprintf(arq_alim,"ID\tNUMERO_NOS\tBARRA\tTENSAO_NOM\tNOS\tITERACOES\tPERDAS\tCARREGAMENTO_REDE\tCARREG_CORRENTE\tMENRO_TENSAO\tQUEDA_MAX\tDESBALANCO_TENSAO\tDESBALANCO_CORRENTE\tDESBALANCO_ALIM\n");
    int id_menorV, id_maxCar, id_maxQueda, id_itmax;
    powerflow_result_rede.iteracoes = 0;
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
        fprintf(arq_alim,"%d\t%d\t%ld\t%.3lf\t%ld\t%d",idAlim,alimentadores[idAlim].numeroNos,alimentadores[idAlim].noRaiz,grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000,alimentadores[idAlim].numeroNos,powerflow_result[idAlim].iteracoes);
        fprintf(arq_alim,"\t%lf\t%lf\t%lf\t%lf\t%lf", powerflow_result[idAlim].perdasResistivas* Sbase,powerflow_result[idAlim].carregamentoRede* Sbase,powerflow_result[idAlim].maiorCarregamentoCorrente,powerflow_result[idAlim].menorTensao, powerflow_result[idAlim].quedaMaxima);
        fprintf(arq_alim,"\t%lf\t%lf\t%lf\n", powerflow_result[idAlim].desbalancoTensaoMaximo * 100, powerflow_result[idAlim].desbalancoCorrenteMaximo *100, powerflow_result[idAlim].desbalancoCorrenteAlim*100);
//        if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 == 34.5){
            powerflow_result_rede.perdasResistivas += powerflow_result[idAlim].perdasResistivas;
            powerflow_result_rede.carregamentoRede += powerflow_result[idAlim].carregamentoRede;

            if (powerflow_result[idAlim].iteracoes > powerflow_result_rede.iteracoes){
                id_itmax = idAlim;
                powerflow_result_rede.iteracoes = powerflow_result[idAlim].iteracoes;
            }
            if (powerflow_result[idAlim].maiorCarregamentoCorrente > powerflow_result_rede.maiorCarregamentoCorrente){
                id_maxCar = idAlim;
                powerflow_result_rede.maiorCarregamentoCorrente = powerflow_result[idAlim].maiorCarregamentoCorrente;
            }

            if (powerflow_result[idAlim].menorTensao < powerflow_result_rede.menorTensao){
                id_menorV = idAlim;
                powerflow_result_rede.menorTensao = powerflow_result[idAlim].menorTensao;
            }
            if (powerflow_result[idAlim].quedaMaxima > powerflow_result_rede.quedaMaxima){
                id_maxQueda = idAlim;
                powerflow_result_rede.quedaMaxima = powerflow_result[idAlim].quedaMaxima;
            }
//        }
    }
    fclose(arq_alim);
    printf("Numero maximo de iteracoes: %d  (alim: %d  / %d)\n", powerflow_result_rede.iteracoes, id_itmax,numeroAlimentadores);
    printf("Vmin: %lf p.u. (alim: %d)\n", powerflow_result_rede.menorTensao, id_menorV);
    printf("quedaMax: %lf  %%  (alim: %d)\n", powerflow_result_rede.quedaMaxima,id_maxQueda);
    printf("maior carregamentoCorrente: %lf %%  (alim: %d)\n", powerflow_result_rede.maiorCarregamentoCorrente,id_maxCar);
    printf("carregamentoRede: %lf kW\n", powerflow_result_rede.carregamentoRede * Sbase);
    printf("perdas totais: %lf kW\n", powerflow_result_rede.perdasResistivas * Sbase);
    
    
    //Impressão de resultados em arquivos
    int ppt_aux = 0;
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
        if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 == 34.5){
            if (ppt_aux == 0){
                salvaTensoesNodais("stateVT.txt","w+",alimentadores[idAlim],grafo);
                salvaTensoesNodais("stateVT.txt","w+",alimentadores[idAlim],grafo);
                ppt_aux=1;
            }
            else salvaTensoesNodais("stateVT.txt","a+",alimentadores[idAlim],grafo);
        }
    }
    //Impressão de resultados
    // for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
        // printf("\n\n");
        // printf("Alimentador: %d\n",idAlim +1);
        // printf("...\n");
        
    // printf("\nTensoes Nodais:\n");
    // idAlim = 0;
    // i=0;
    // barraAtual = &alimentadores[idAlim].rnp[0];
    // while((barraAtual != NULL)){
    //     imprimeTensoesNodais(&grafo[barraAtual->idNo]);
    //     barraAtual = barraAtual->prox;
    //     i++;
    // }            
    // imprimeTensoesNodais(&grafo[56789]);
    // imprimeiInjecoesCorrentes(&grafo[56789]);
       
    //     //Imprime as tensões nodais
    //    printf("\nTensoes Nodais:\n");
    //    barraAtual = &alimentadores[idAlim].rnp[0];
    //    while(barraAtual != NULL){
    //        imprimeTensoesNodais(&grafo[barraAtual->idNo]);
    //        barraAtual = barraAtual->prox;
    //    }
        
    //    printf("\nInjecoes de Corrente\n");
    //    barraAtual = &alimentadores[idAlim].rnp[0];
    //    while(barraAtual != NULL){
    //        imprimeiInjecoesCorrentes(&grafo[barraAtual->idNo]);
    //        barraAtual = barraAtual->prox;
    // //    }
    //     printf("\nTaps de Reguladores:\n");
    //     barraAtual = &alimentadores[idAlim].rnp[0];
    //     while(barraAtual != NULL){
    //         imprimeTaps(&grafo[barraAtual->idNo]);
    //         barraAtual = barraAtual->prox;
    //     }
    // }

}


//Rotina para Cálculo de Fluxo de Potência em múltiplos alimentadores de um sistema de distribuição na sequeência hierárquica dos níveis de tensão (Primeiro 13.8kV e depois 34.5kV)
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
void fluxoPotencia_Niveis_BFS_Multiplos(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos,double Sbase, long int **interfaceNiveis,long int numeroInterfaces, BOOL opt_flow){
    long int nmed,nvar,nmedTotal;
    int i,j, idAlim, it, n_threads;
    TF_FILABARRAS *barraAtual = NULL;
    TF_GRAFO *no  = NULL;
    extern BOOL iteration_multiple_level_OPT;
    
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
    
    // runs multiple times if there is a tap change on the upper level (34.5)
    while (tap_modified){
    tap_modified = false;
        
    // Cálculo do Fluxo de Potência no nível de 13.8 kV
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
        if (runPF_alimentador[idAlim]){
            // run power flow for feeder if true, selected on list
            tIni = omp_get_wtime();
            if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 == 13.8){
                if (opt_flow) powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                else powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador_IteracaoUnica(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                
                runPF_alimentador[idAlim] = false;     // finished calculating power flow on the feeder
            }

            tEnd = omp_get_wtime(); 
            tempoAUX = tEnd - tIni;
            fprintf(arquivo,"\n %d \t13.8\t%ld\t%d\t%.12lf\t%lf",i,alimentadores[idAlim].numeroNos,powerflow_result[idAlim].iteracoes,tempoAUX, powerflow_result[idAlim].menorTensao );
            
        }
    }//Fim do loop dos alimentadores  

    //Atualiza as Interfaces das SEs de 34.5 kV com a carga calculada em 13.8 kV
    for (idAlim = 0; idAlim < numeroInterfaces; idAlim++){
        if (interfaceNiveis[idAlim][2] == 0){ //Para mútliplos níveis, até o momento somente dois
            int bar_13_8 = interfaceNiveis[idAlim][0];
            int bar_34_5 = interfaceNiveis[idAlim][1];
            grafo[bar_34_5].barra->nloads = 0;

            grafo[bar_34_5].Cur[0] = 0;
            grafo[bar_34_5].Cur[1] = 0;
            grafo[bar_34_5].Cur[2] = 0;

            grafo[bar_34_5].S[0]=0;
            grafo[bar_34_5].S[1]=0;
            grafo[bar_34_5].S[2]=0;
        }
    }

    for (idAlim = 0; idAlim < numeroInterfaces; idAlim++){
        if (interfaceNiveis[idAlim][2] == 0){ //Para mútliplos níveis, até o momento somente dois
            int bar_13_8 = interfaceNiveis[idAlim][0];
            int bar_34_5 = interfaceNiveis[idAlim][1];

            grafo[bar_34_5].barra->nloads++;
            grafo[bar_34_5].barra->loads[0].lig = YN;


            //Reflete injeções de potência do 13.8 no 34.5 (Revisar e incluir um trfo YnD e Regulador de tensão)
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Pnom[0] = creal(grafo[bar_13_8].V[0]*conj(grafo[bar_13_8].adjacentes[0].Cur[0]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Pnom[1] = creal(grafo[bar_13_8].V[1]*conj(grafo[bar_13_8].adjacentes[0].Cur[1]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Pnom[2] = creal(grafo[bar_13_8].V[2]*conj(grafo[bar_13_8].adjacentes[0].Cur[2]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Qnom[0] = cimag(grafo[bar_13_8].V[0]*conj(grafo[bar_13_8].adjacentes[0].Cur[0]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Qnom[1] = cimag(grafo[bar_13_8].V[1]*conj(grafo[bar_13_8].adjacentes[0].Cur[1]));
            grafo[bar_34_5].barra->loads[grafo[bar_34_5].barra->nloads - 1].Qnom[2] = cimag(grafo[bar_13_8].V[2]*conj(grafo[bar_13_8].adjacentes[0].Cur[2]));

            grafo[bar_34_5].Cur[0] += grafo[bar_13_8].adjacentes[0].Cur[0];
            grafo[bar_34_5].Cur[1] += grafo[bar_13_8].adjacentes[0].Cur[1];
            grafo[bar_34_5].Cur[2] += grafo[bar_13_8].adjacentes[0].Cur[2];                

            grafo[bar_34_5].S[0] += grafo[bar_13_8].V[0]*conj(grafo[bar_13_8].adjacentes[0].Cur[0]);
            grafo[bar_34_5].S[1] += grafo[bar_13_8].V[1]*conj(grafo[bar_13_8].adjacentes[0].Cur[1]);
            grafo[bar_34_5].S[2] += grafo[bar_13_8].V[2]*conj(grafo[bar_13_8].adjacentes[0].Cur[2]);
        }
    }

    // Cálculo do Fluxo de Potência no nível de 34.5 kV
    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
        if (runPF_alimentador[idAlim]){
            // run power flow for feeder if true, selected on list
            tIni = omp_get_wtime();
            if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 == 34.5){
                if (opt_flow) powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                else powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador_IteracaoUnica(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
                
                runPF_alimentador[idAlim] = false;     // finished calculating power flow on the feeder
                
                
            }
            tEnd = omp_get_wtime(); 
            tempoAUX = tEnd - tIni;
            fprintf(arquivo,"\n %d \t34.5\t%ld\t%d\t%.12lf\t%lf",i,alimentadores[idAlim].numeroNos,powerflow_result[idAlim].iteracoes,tempoAUX, powerflow_result[idAlim].menorTensao );

            
            //-----------------------------------------------------------------------
            // for tap changing control at upper level   (check only at upper levels)
            if (powerflow_result[idAlim].tap_change_flag && iteration_multiple_level_OPT){
                tap_modified = true;
                
                printf("\n Mudanca de tap em 34.5 kV (Alimentador %d)- Fluxo de potência será recalculado em 13.8kV\n", idAlim);
                runPF_alimentador[idAlim] = true;  // will run the power flow another time due to updated tap position
                
                // selects also the downstream feeders to run power flow (13.8kV)
                for (int aux = 0; aux < numeroInterfaces; aux++){
                    if (interfaceNiveis[aux][2] == 0){ //Para mútliplos níveis, até o momento somente dois
                        int bar_13_8 = interfaceNiveis[aux][0];
                        int bar_34_5 = interfaceNiveis[aux][1];

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
    
    
    
    // Check if the option for multiple iterations is on
    if (!iteration_multiple_level_OPT){
        tap_modified = false;
    }
    
    } // end of loop of iterations among different levels (34.5kV and 13.8kV)
    
    
    
    fclose(arquivo);

    //Função Atualiza SEs
        
         
    

//    FILE *arq_alim;
//    arq_alim = fopen("resultadoAlim.csv","a+");
//    
//    fprintf(arq_alim,"ID\tNUMERO_NOS\tBARRA\tTENSAO_NOM\tNOS\tITERACOES\tMENOR_TENSAO_A\tMENOR_TENSAO_B\tMENOR_TENSAO_C\tCARGA_TOTAL_A\tCARGA_TOTAL_B\tCARGA_TOTAL_C\tPERDAS\tCARREGAMENTO_REDE\tCARREG_CORRENTE\tMENRO_TENSAO\tQUEDA_MAX\tDESBALANCO_TENSAO\tDESBALANCO_CORRENTE\tDESBALANCO_ALIM\n");
    int id_menorV, id_maxCar, id_maxQueda, id_itmax, id_maxDesbalanco, id_maxDesbalancoCorrente, id_maxDesbalancoCorrenteAlim;
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
    for (idAlim = 0; idAlim < numeroInterfaces; idAlim++){
    if (interfaceNiveis[idAlim][2] == 0){ //Para mútliplos níveis, até o momento somente dois
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
    
//    arquivo = fopen("resultadoAlimentadores_FaseA.csv","wt");
//    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
//        if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 == 13.8){
//        barraAtual = &alimentadores[idAlim].rnp[0];
//        fprintf(arquivo,"Pf\t%d\t", idAlim);
//        while(barraAtual != NULL){
//            i = barraAtual->idNo;
////            fprintf(arquivo,"%d\t", grafo[i].profundidade);
//            fprintf(arquivo,"%.2lf\t", grafo[i].distanciaSE_acc);
//            barraAtual = barraAtual->prox;
//        }
//        fprintf(arquivo,"\nVa\t%d\t", idAlim);
//        barraAtual = &alimentadores[idAlim].rnp[0];
//        while(barraAtual != NULL){
//            i = barraAtual->idNo;
//            fprintf(arquivo,"%.12lf\t", sqrt(3)*grafo[i].Vbase*cabs(grafo[i].V[0]));
//            barraAtual = barraAtual->prox;
//        }
//        fprintf(arquivo,"\n"); 
//        }
//    }
//    fclose(arquivo);
//    arquivo = fopen("resultadoAlimentadores_FaseB.csv","wt");
//    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
//        if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 == 13.8){
//        barraAtual = &alimentadores[idAlim].rnp[0];
//        fprintf(arquivo,"Pf\t%d\t", idAlim);
//        while(barraAtual != NULL){
//            i = barraAtual->idNo;
////            fprintf(arquivo,"%d\t", grafo[i].profundidade);
//            fprintf(arquivo,"%.2lf\t", grafo[i].distanciaSE_acc);
//            barraAtual = barraAtual->prox;
//        }
//        fprintf(arquivo,"\nVb\t%d\t", idAlim);
//        barraAtual = &alimentadores[idAlim].rnp[0];
//        while(barraAtual != NULL){
//            i = barraAtual->idNo;
//            fprintf(arquivo,"%.12lf\t", sqrt(3)*grafo[i].Vbase*cabs(grafo[i].V[1]));
//            barraAtual = barraAtual->prox;
//        }
//        fprintf(arquivo,"\n"); 
//        }
//    }
//    fclose(arquivo);
//    arquivo = fopen("resultadoAlimentadores_FaseC.csv","wt");
//    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
//        if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 == 13.8){
//        barraAtual = &alimentadores[idAlim].rnp[0];
//        fprintf(arquivo,"Pf\t%d\t", idAlim);
//        while(barraAtual != NULL){
//            i = barraAtual->idNo;
////            fprintf(arquivo,"%d\t", grafo[i].profundidade);
//            fprintf(arquivo,"%.2lf\t", grafo[i].distanciaSE_acc);
//            barraAtual = barraAtual->prox;
//        }
//        fprintf(arquivo,"\nVc\t%d\t", idAlim);
//        barraAtual = &alimentadores[idAlim].rnp[0];
//        while(barraAtual != NULL){
//            i = barraAtual->idNo;
//            fprintf(arquivo,"%.12lf\t", sqrt(3)*grafo[i].Vbase*cabs(grafo[i].V[2]));
//            barraAtual = barraAtual->prox;
//        }
//        fprintf(arquivo,"\n"); 
//        }
//    }
//    fclose(arquivo);
}




void fluxoPotencia_Alimentador_BFS_call(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos, int idAlim, double Sbase){
    long int nmed,nvar,nmedTotal;
    int i,j, it, n_threads;
    TF_FILABARRAS *barraAtual = NULL;
    TF_GRAFO *no  = NULL;

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

    // printf("Calculo de Fluxo de Potencia via Varredura Direta/Inversa...\n");
//    incializaTensoesVarredura(grafo, numeroBarras, alimentadores, numeroAlimentadores);
    
    //Calculo de fluxo de potência para todos os alimentadores individualmente
    double tol = 0.000001;
        
    powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
//    powerflow_result[idAlim] = fluxoPotencia_BFS_Alimentador_IteracaoUnica(grafo, numeroBarras, alimentadores[idAlim], ramos, Sbase);
//    printf("\nTaps de Reguladores:\n");
//    barraAtual = &alimentadores[idAlim].rnp[0];
//    while(barraAtual != NULL){
//        imprimeTaps(&grafo[barraAtual->idNo]);
//        barraAtual = barraAtual->prox;
//    }
//    FILE *arq_alim;
//    arq_alim = fopen("resultadoAlim.csv","w+");
//    
//    fprintf(arq_alim,"ID\tBARRA\tTENSAO_NOM\tNOS\tITERACOES\tPERDAS\tCARREGAMENTO_REDE\tCARREG_CORRENTE\tMENRO_TENSAO\tQUEDA_MAX\n");
    int id_menorV, id_maxCar, id_maxQueda, id_itmax,id_maxDesbalanco;
    powerflow_result_rede.iteracoes = 0;
    
//    fprintf(arq_alim,"%d\t%ld\t%.3lf\t%ld\t%d",idAlim,alimentadores[idAlim].noRaiz,grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000,alimentadores[idAlim].numeroNos,powerflow_result[idAlim].iteracoes);
//    fprintf(arq_alim,"\t%lf\t%lf\t%lf\t%lf\t%lf\n", powerflow_result[idAlim].perdasResistivas* Sbase,powerflow_result[idAlim].carregamentoRede* Sbase,powerflow_result[idAlim].maiorCarregamentoCorrente,powerflow_result[idAlim].menorTensao, powerflow_result[idAlim].quedaMaxima);
        powerflow_result_rede.perdasResistivas += powerflow_result[idAlim].perdasResistivas;
        powerflow_result_rede.carregamentoRede += powerflow_result[idAlim].carregamentoRede;

        if (powerflow_result[idAlim].iteracoes > powerflow_result_rede.iteracoes){
            id_itmax = idAlim;
            powerflow_result_rede.iteracoes = powerflow_result[idAlim].iteracoes;
        }
        if (powerflow_result[idAlim].maiorCarregamentoCorrente > powerflow_result_rede.maiorCarregamentoCorrente){
            id_maxCar = idAlim;
            powerflow_result_rede.maiorCarregamentoCorrente = powerflow_result[idAlim].maiorCarregamentoCorrente;
        }

        if (powerflow_result[idAlim].menorTensao < powerflow_result_rede.menorTensao){
            id_menorV = idAlim;
            powerflow_result_rede.menorTensao = powerflow_result[idAlim].menorTensao;
        }
        if (powerflow_result[idAlim].quedaMaxima > powerflow_result_rede.quedaMaxima){
            id_maxQueda = idAlim;
            powerflow_result_rede.quedaMaxima = powerflow_result[idAlim].quedaMaxima;
        }
        
        powerflow_result_rede.carregamentoRedeABC[0] = powerflow_result[idAlim].carregamentoRedeABC[0];
        powerflow_result_rede.carregamentoRedeABC[1] = powerflow_result[idAlim].carregamentoRedeABC[1];
        powerflow_result_rede.carregamentoRedeABC[2] = powerflow_result[idAlim].carregamentoRedeABC[2];
        
        powerflow_result_rede.desbalancoTensaoMaximo = powerflow_result[idAlim].desbalancoTensaoMaximo;

        powerflow_result_rede.desbalancoCorrenteMaximo = powerflow_result[idAlim].desbalancoCorrenteMaximo;

        powerflow_result_rede.desbalancoCorrenteAlim = powerflow_result[idAlim].desbalancoCorrenteAlim;
            
            
    
//    fclose(arq_alim);
    printf("Numero maximo de iteracoes: %d  (alim: %d  / %ld)\n", powerflow_result_rede.iteracoes, id_itmax,numeroAlimentadores);
    printf("Vmin: %lf p.u. (alim: %d)\n", powerflow_result_rede.menorTensao, id_menorV);
    printf("quedaMax: %lf  %%  (alim: %d)\n", powerflow_result_rede.quedaMaxima,id_maxQueda);
    printf("maior carregamentoCorrente: %lf %%  (alim: %d)\n", powerflow_result_rede.maiorCarregamentoCorrente,id_maxCar);
    printf("carregamentoRede: %lf kW\n", powerflow_result_rede.carregamentoRede * Sbase);
    printf("carregamentoRede por fase: %lf kW \t %lf\t %lf\n", powerflow_result_rede.carregamentoRedeABC[0] * Sbase, powerflow_result_rede.carregamentoRedeABC[1] * Sbase, powerflow_result_rede.carregamentoRedeABC[2] * Sbase);
    printf("perdas totais: %lf kW\n", powerflow_result_rede.perdasResistivas * Sbase);
    printf("maximo desbalanco de tensão: %lf %%  \n", powerflow_result_rede.desbalancoTensaoMaximo *100 );
    printf("maximo desbalanco de corrente: %lf %%  \n", powerflow_result_rede.desbalancoCorrenteMaximo *100 );
    printf("maximo desbalanco de corrente de saida de alimentador: %lf %% \n", powerflow_result_rede.desbalancoCorrenteAlim *100);
//    
    
    
    
//    //Impressão de resultados em arquivos
//    int ppt_aux = 0;
//    
//    if (ppt_aux == 0){
//        salvaTensoesNodais("stateVT.txt","w+",alimentadores[idAlim],grafo);
//        ppt_aux=1;
//    }
//    else salvaTensoesNodais("stateVT.txt","a+",alimentadores[idAlim],grafo);
        

}


void amostragemVariacaoCarga(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, double percentual){ 
    int i, k, fase;
    int de,para,n_adj;
    double rnd;
    TF_FILABARRAS *barraAtual;
    __complex__ double Saux[3], Iaux[3], V0[3];


    for(i=0; i<numeroAlimentadores; i++)
    {
        barraAtual = &alimentadores[i].rnp[0];
        
        V0[0] = grafo[alimentadores[i].noRaiz].barra->Vinicial[0];//1.0*(cos(0) + I*sin(0));
        V0[1] = grafo[alimentadores[i].noRaiz].barra->Vinicial[1];//1.0*(cos(-120*PI/180) + I*sin(-120*PI/180));
        V0[2] = grafo[alimentadores[i].noRaiz].barra->Vinicial[2];//1.0*(cos(120*PI/180) + I*sin(120*PI/180));
        
        while(barraAtual != NULL)
        {
            de = barraAtual->idNo;
            grafo[de].Cur[0] = 0;
            grafo[de].Cur[1] = 0;
            grafo[de].Cur[2] = 0;
            
            grafo[de].S[0] = 0;
            grafo[de].S[1] = 0;
            grafo[de].S[2] = 0;
            
            for (k=0;k<grafo[de].barra->nloads;k++){
                rnd = 1; //(float) rand()/RAND_MAX - 0.5;
                Saux[0] = grafo[de].barra->loads[k].Pnom[0] + percentual * cabs(grafo[de].barra->loads[k].Pnom[0]) * rnd;
                Saux[1] = grafo[de].barra->loads[k].Pnom[1] + percentual * cabs(grafo[de].barra->loads[k].Pnom[1]) * rnd;
                Saux[2] = grafo[de].barra->loads[k].Pnom[2] + percentual * cabs(grafo[de].barra->loads[k].Pnom[2]) * rnd;
                
                Saux[0] += I * grafo[de].barra->loads[k].Qnom[0] + percentual * cabs(grafo[de].barra->loads[k].Qnom[0]) * rnd;
                Saux[1] += I * grafo[de].barra->loads[k].Qnom[1] + percentual * cabs(grafo[de].barra->loads[k].Qnom[1]) * rnd;
                Saux[2] += I * grafo[de].barra->loads[k].Qnom[2] + percentual * cabs(grafo[de].barra->loads[k].Qnom[2]) * rnd;
                
                Iaux[0] = conj(Saux[0]/V0[0]);
                Iaux[1] = conj(Saux[1]/V0[1]);
                Iaux[2] = conj(Saux[2]/V0[2]);


                grafo[de].Cur[0] += Iaux[0];
                grafo[de].Cur[1] += Iaux[1];
                grafo[de].Cur[2] += Iaux[2];

                grafo[de].S[0] += V0[0] * conj(Iaux[0]);
                grafo[de].S[1] += V0[1] * conj(Iaux[1]);
                grafo[de].S[2] += V0[2] * conj(Iaux[2]);
                
            }
            
            
            
            
            
            barraAtual = barraAtual->prox;
        }              
    }
    free(barraAtual);
    
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
TF_PFSOLUTION fluxoPotencia_BFS_Alimentador_IteracaoUnica(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR alimentador, TF_DRAM *ramos,double Sbase){
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
     
    //----------------------------------------------------------------------
    //Atualiza Correntes Nodais - Fora do Laço Iterativo - Modelo de Corrente Constante
    //
//    barraAtual = &alimentador.rnp[0];
//    k=0;
//    while(barraAtual != NULL){
//        atualizaInjecoes(&grafo[barraAtual->idNo]);
////        for(i=0;i<3;i++){
////            DV[k] = cabs(grafo[barraAtual->idNo].V[i]);
////            k++;
////            DV[k] = carg(grafo[barraAtual->idNo].V[i]);
////            k++;                      
////        }
//        // imprimeiInjecoesCorrentes(&grafo[barraAtual->idNo]);
//        barraAtual = barraAtual->prox;
//    }

       
    ////----------------------------------------------------------------------
    // Fluxo de  Potência por Varredura Direta/Inversa via Soma de Correntes
//    for (it = 0;it < MAXIT; it ++){
//        //---------------------------------------------------------------------- 
//        //Tensões Anteriores para Conevrgência
//        barraAtual = &alimentador.rnp[0];
//        k=0;
//        while(barraAtual != NULL){
////            atualizaInjecoes(&grafo[barraAtual->idNo]); // Carga como potência constanta, se comentar a carga fica como Corrente constante
//            for(i=0;i<3;i++){
//                DV[k] = cabs(grafo[barraAtual->idNo].V[i]);
//                k++;
//                DV[k] = carg(grafo[barraAtual->idNo].V[i]);
//                k++;                      
//            }
//            barraAtual = barraAtual->prox;
//        }
//        double tIni = omp_get_wtime();
        //---------------------------------------------------------------------- 
        //Backward Sweep
        for(k = alimentador.numeroNos-1; k >= 0; k--){
            backward(&grafo[RNP[k]], grafo);
        }
//        double tEnd = omp_get_wtime();
//        printf("\n%d Back: %lf", alimentador.idAlim, tEnd - tIni);
//        
//        tIni = omp_get_wtime();
        //---------------------------------------------------------------------- 
        //Forward Sweep
        for(k = 0; k < alimentador.numeroNos; k++){
            control_action = forward(&grafo[RNP[k]], grafo);
            if (control_action) powerflow_result.tap_change_flag = true;
        }
        
//        tEnd = omp_get_wtime();
//        printf("For: %lf\n", tEnd - tIni);

//        //Critério de Convergência
//        barraAtual = &alimentador.rnp[0];
//        k=0;
//        while(barraAtual != NULL){
//            for(i=0;i<3;i++){
//                DV[k] = DV[k] - cabs(grafo[barraAtual->idNo].V[i]);
//                k++;
//                DV[k] = DV[k] - carg(grafo[barraAtual->idNo].V[i]);
//                k++;                        
//            }
//            barraAtual = barraAtual->prox;
//        }
//        
//        nDV = norma_inf(DV,nvar);
//        if ((fabs(nDV) < tolerance) && !control_action){
//            conv = 1;
//            powerflow_result.convergencia = 1;
//            powerflow_result.iteracoes = it + 1;
//            break;
//        }
//    }
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


