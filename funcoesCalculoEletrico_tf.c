#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <complex.h>

#include "data_structures_tf.h"
#include "funcoesCalculoEletrico_tf.h"
#include "funcoesMatematicas_tf.h"
//#include "data_structures.h"
//------------------------------------------------------------------------------
//
// FUNÇÕES DE CÁLCULO ELETRICO - FLUXOS DE POTÊNCIA, CORRENTE ETC..
//
//------------------------------------------------------------------------------
//função que calcula a tensão no Delta
/**
 * @brief Função auxiliar para cálculo de tensão de linha (tensão fase-fase)
 * 
 * Essa função realiza o cálculo de tensão de linha trifásicas (tensão complexa fase-fase) a partir das tensões de fase (tensão complexa fase-neutro).
 * Recebe como parâmetro de entrada a tensão complexa de fase @p Vf e o parâmetro de entrada e saída da tensão de linha @p Vl . Considera a sequência
 * de fase abc. 
 * A função retorna @c void .
 * 
 * @param *Vf ponteiro com as tensões complexas fase-neutro
 * @param *Vl ponteiro com as tensões complexas fase-fase calculadas
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void tensaoDelta(__complex__ double *Vf, __complex__ double *Vl){
    Vl[0] = (Vf[0] - Vf[1]);
    Vl[1] = (Vf[1] - Vf[2]);
    Vl[2] = (Vf[2] - Vf[0]);
}

//Função Potência aparente complexa no ramo k->m
/**
 * @brief Função auxiliar para cálculo do fluxo de potência complexa em determinado ramo da rede elétrica no sentido da barra k para a barra m  (k -> m)
 * 
 * Essa função realiza o cálculo do fluxo de potência complexa em determinado ramo @p ramo da rede elétrica no sentido da barra k @p noP 
 * para a barra m @p noS (k -> m), retornando a potência complexa trifásica (ativa e reativa por fase) no vetor @p S .
 * A função retorna @c void .
 * 
 * @param *noP ponteiro para a barra inicial do sentido de fluxo de potência
 * @param *noS ponteiro para a barra final do sentido de fluxo de potência
 * @param *ramo ponteiro para o ramo sendo calculado o fluxo de potênca
 * @param *s vetor com as potências complexas (ativa e reativa) do fluxo entre a barra k (noP) e barra m (noS)
 * @return void
 * @see Sk
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void Skm(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *S){
    int i,k;
    __complex__ double Vp[3], Vs[3];
    
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
    
    for (k=0;k<3;k++){
        S[k]=0;
        for (i=0;i<3;i++){
            S[k] = S[k] + Vp[k]*conj(ramo->Ypp[k][i])*conj(Vp[i]) + Vp[k]*conj(ramo->Yps[k][i])*conj(Vs[i]);
        }
    }
    switch (ramo->fases){
        case 1:
            S[1] = 0;
            S[2] = 0;
            break;    
        case 2:
            S[0] = 0;
            S[2] = 0;
            break;
        case 3:
            S[1] = 0;
            S[0] = 0;
            break;
        case 4:
            S[2] = 0;
            break;    
        case 5:
            S[1] = 0;
            break;
        case 6:
            S[0] = 0;
            break; 
    }
    
    
}

//Função Potência aparente complexa no ramo m->k
/**
 * @brief Função auxiliar para cálculo do fluxo de potência complexa em determinado ramo da rede elétrica no sentido da barra m para a barra k  (m -> k)
 * 
 * Essa função realiza o cálculo do fluxo de potência complexa em determinado ramo @p ramo da rede elétrica no sentido da barra m @p noS
 * para a barra m @p noP (m -> k), retornando a potência complexa trifásica (ativa e reativa por fase) no vetor @p S .
 * A função retorna @c void .
 * 
 * @param *noP ponteiro para a barra final do sentido de fluxo de potência
 * @param *noS ponteiro para a barra inicial do sentido de fluxo de potência
 * @param *ramo ponteiro para o ramo sendo calculado o fluxo de potênca
 * @param *s vetor com as potências complexas (ativa e reativa) do fluxo entre a barra m (noS) e barra k (noP)
 * @return void
 * @see Sk
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void Smk(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *S){
    int i,k;
    __complex__ double Vp[3], Vs[3];
        
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
    
    for (k=0;k<3;k++){
        S[k]=0;
        for (i=0;i<3;i++){
            S[k] = S[k] + Vs[k]*conj(ramo->Ysp[k][i])*conj(Vp[i]) + Vs[k]*conj(ramo->Yss[k][i])*conj(Vs[i]);
        }
    }
    switch (ramo->fases){
        case 1:
            S[1] = 0;
            S[2] = 0;
            break;    
        case 2:
            S[0] = 0;
            S[2] = 0;
            break;
        case 3:
            S[1] = 0;
            S[0] = 0;
            break;
        case 4:
            S[2] = 0;
            break;    
        case 5:
            S[1] = 0;
            break;
        case 6:
            S[0] = 0;
            break; 
    }
}

//Função Potência aparente complexa na barra "k"
/**
 * @brief Função auxiliar para cálculo da injeção de potência complexa em determinado nó da rede elétrica
 * 
 * Essa função realiza o cálculo da injeção de potência complexa em determinado nó indicado por @p k da rede elétrica @p grafo atraveś do somatório
 * dos fluxos de potência para ramos adjacentes, retornando a potência complexa trifásica (ativa e reativa por fase) no vetor @p S .
 * A função retorna @c void .
 * 
 * @param *grafo estrutura de dados com os dados da rede elétrica
 * @param k identificador do nó a ser calculada a injeção de potência líquida
 * @param *S vetor com as potências complexas (ativa e reativa) líquidas na barra k
 * @return void
 * @see 
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void Sk(TF_GRAFO *grafo, long int k, __complex__ double *S){
    int i,j;
    
    __complex__ double *Saux;
    Saux = (__complex__ double*)malloc(3 * sizeof(__complex__ double));
    
    for(j=0;j<3;j++){
        S[j] = 0;
    }
    for (i=0; i<grafo[k].numeroAdjacentes;i++){
        if (k == grafo[k].adjacentes[i].ramo->k)
            Skm(&grafo[k],&grafo[grafo[k].adjacentes[i].idNo],grafo[k].adjacentes[i].ramo, Saux);
        else
            Smk(&grafo[grafo[k].adjacentes[i].idNo],&grafo[k], grafo[k].adjacentes[i].ramo, Saux);
        
        for(j=0;j<3;j++){
            S[j] = S[j] - Saux[j];
        }
    }
    //Potência nos shunts
    for (j=0;j<3;j++){
        for (i=0;i<3;i++){
            S[j] = S[j] + grafo[k].V[j]*conj(grafo[k].Ysh[j][i])*conj(grafo[k].V[i]);
        }
    }
    
    free(Saux);
}

//Função Ikm
void Ikm(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *Ikm){
    int i,k;
    __complex__ double Vp[3], Vs[3];
    
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
    
    for (k=0;k<3;k++){
        Ikm[k]=0;
        for (i=0;i<3;i++){
            Ikm[k] = Ikm[k] + ramo->Ypp[k][i]*Vp[i] + ramo->Yps[k][i]*Vs[i];
        }
    }
}

//Função Ikm
void Imk(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *Imk){
    int i,k;
    __complex__ double Vp[3], Vs[3];
    
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
    
    for (k=0;k<3;k++){
        Imk[k]=0;
        for (i=0;i<3;i++){
            Imk[k] = Imk[k] + ramo->Ysp[k][i]*Vp[i] + ramo->Yss[k][i]*Vs[i];
        }
    }
}


//------------------------------------------------------------------------------
//
// FUNÇÕES DE CÁLCULO ELETRICO - DERIVADAS DOS FLUXOS DE POTÊNCIA, CORRENTE ETC..
//
//------------------------------------------------------------------------------

//Derivada da Potência aparente complexa no ramo k->m 
//opt = 0 tensão noP
//opt = 1 angulo noP
//opt = 2 tensão noS
//opt = 3 angulo noS em relação à fase "i"
void dSkm(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dS, long int opt, long int i){ //fase i
    int j,k;
    __complex__ double *Vp, *Vs, **J = NULL, P1,P2,P3;
    
    J = c_matAloca(3);
    J[i][i] = 1;
    
    Vp = c_vetAloca(3);
    Vs = c_vetAloca(3);
        
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
        
    switch (opt){
        case 0: //Derivada em relação a tensão do noP - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k]=0;
                for (j=0;j<3;j++){
                    dS[k] += J[k][k]*1/cabs(Vp[k])*Vp[k]*conj(ramo->Ypp[k][j])*conj(Vp[j]) + J[k][k]*1/cabs(Vp[k])*Vp[k]*conj(ramo->Yps[k][j])*conj(Vs[j]);
                }
                dS[k] += Vp[k]*conj(ramo->Ypp[k][i])*1/cabs(Vp[i])*conj(Vp[i]);
            }
            break;
        case 1: //Derivada em relação ao ângulo do noP - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k]=0;
                for (j=0;j<3;j++){
                    dS[k] += I*Vp[k]*J[k][k]*conj(ramo->Ypp[k][j])*conj(Vp[j]) + I*Vp[k]*J[k][k]*conj(ramo->Yps[k][j])*conj(Vs[j]);
                }
                dS[k] += Vp[k]*conj(ramo->Ypp[k][i])*(-1)*I*conj(Vp[i]);
            }
            break;
        case 2:  //Derivada em relação a tensão do noS - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k] = Vp[k]*conj(ramo->Yps[k][i])*1/cabs(Vs[i])*conj(Vs[i]);
            }
            break;
        case 3: //Derivada em relação ao ângulo do noS - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k] = Vp[k]*conj(ramo->Yps[k][i])*(-1)*I*conj(Vs[i]);
            }
            break;
    }
    switch (ramo->fases){
        case 1:
            dS[1] = 0;
            dS[2] = 0;
            break;    
        case 2:
            dS[0] = 0;
            dS[2] = 0;
            break;
        case 3:
            dS[1] = 0;
            dS[0] = 0;
            break;
        case 4:
            dS[2] = 0;
            break;    
        case 5:
            dS[1] = 0;
            break;
        case 6:
            dS[0] = 0;
            break; 
    }
    free(Vp);free(Vs);
    for(i=0;i<3;i++) free(J[i]);
    free(J);
}

//void dSmk
void dSmk(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dS, long int opt, long int i){ //fase i
    int j,k;
    __complex__ double *Vp, *Vs, **J = NULL;
    
    J = c_matAloca(3);
    J[i][i] = 1;
    
    Vp = c_vetAloca(3);
    Vs = c_vetAloca(3);
        
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
        
    switch (opt){
        case 0: //Derivada em relação a tensão do noP - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k] = Vs[k]*conj(ramo->Ysp[k][i])*1/cabs(Vp[i])*conj(Vp[i]);
            }
            break;
        case 1: //Derivada em relação ao ângulo do noP - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k] = Vs[k]*conj(ramo->Ysp[k][i])*(-1)*I*conj(Vp[i]);
            }
            break;
        case 2:  //Derivada em relação a tensão do noS - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k]=0;
                for (j=0;j<3;j++){
                    dS[k] += J[k][k]*1/cabs(Vs[k])*Vs[k]*conj(ramo->Ysp[k][j])*conj(Vp[j]) + J[k][k]*1/cabs(Vs[k])*Vs[k]*conj(ramo->Yss[k][j])*conj(Vs[j]);
                }
                dS[k] += Vs[k]*conj(ramo->Yss[k][i])*1/cabs(Vs[i])*conj(Vs[i]);
            }
            break;
        case 3: //Derivada em relação ao ângulo do noS - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k]=0;
                for (j=0;j<3;j++){
                    dS[k] += I*Vs[k]*J[k][k]*conj(ramo->Ysp[k][j])*conj(Vp[j]) + I*Vs[k]*J[k][k]*conj(ramo->Yss[k][j])*conj(Vs[j]);
                }
                dS[k] += Vs[k]*conj(ramo->Yss[k][i])*(-1)*I*conj(Vs[i]);
            }
            break;
    }
    switch (ramo->fases){
        case 1:
            dS[1] = 0;
            dS[2] = 0;
            break;    
        case 2:
            dS[0] = 0;
            dS[2] = 0;
            break;
        case 3:
            dS[1] = 0;
            dS[0] = 0;
            break;
        case 4:
            dS[2] = 0;
            break;    
        case 5:
            dS[1] = 0;
            break;
        case 6:
            dS[0] = 0;
            break; 
    }
    free(Vp);free(Vs);
    for(i=0;i<3;i++) free(J[i]);
    free(J);
}


//opt = 0 tensão
//opt = 1 angulo em relação à fase "i" da barra 
void dSk(TF_GRAFO *grafo, long int k, __complex__ double *dS, long int opt, long int barra, long int fase){
    int i,j, opt1;
    double **J = NULL;
    
    J = c_matAloca(3);
    J[fase][fase] = 1;
    
    __complex__ double *dSaux;
    dSaux = (__complex__ double*)malloc(3 * sizeof(__complex__ double));
    
    for(j=0;j<3;j++){
        dS[j] = 0;
    }
    if (barra == k){
        for (i=0; i<grafo[k].numeroAdjacentes;i++){
            if ((k == grafo[k].adjacentes[i].ramo->k)){
                if (opt == 0) opt1 = 0;
                else opt1 = 1;
                dSkm(&grafo[k],&grafo[grafo[k].adjacentes[i].idNo],grafo[k].adjacentes[i].ramo, dSaux, opt1, fase);
            }
            else{
                if (opt == 0) opt1 = 2;
                else opt1 = 3;
                dSmk(&grafo[grafo[k].adjacentes[i].idNo],&grafo[k], grafo[k].adjacentes[i].ramo, dSaux, opt1, fase);
            }
            for(j=0;j<3;j++){
                dS[j] = dS[j] - dSaux[j];
            }
        }
        
        //Derivada da potência nos shunts
        if (opt==0){//Derivada em relação a tensão do noP - DSp / DVpi
            for (j=0;j<3;j++){
                for (i=0;i<3;i++){
                    dS[j] += J[j][j]*1/cabs(grafo[k].V[j])*grafo[k].V[j]*conj(grafo[k].Ysh[j][i])*conj(grafo[k].V[i]) ;
                }
                dS[j] += grafo[k].V[j]*conj(grafo[k].Ysh[j][fase])*1/cabs(grafo[k].V[fase])*conj(grafo[k].V[fase]);
            }
        }
        else{//opt==1 >> Derivada em relação ao angulo do noP - DSp / DVpi
            for (j=0;j<3;j++){
                for (i=0;i<3;i++){
                    dS[j] += I*grafo[k].V[j]*J[j][j]*conj(grafo[k].Ysh[j][i])*conj(grafo[k].V[i]);
                }
                dS[j] += grafo[k].V[j]*conj(grafo[k].Ysh[j][fase])*(-1)*I*conj(grafo[k].V[fase]);
            }
        }           
    }
    else{
        for (i=0; i<grafo[k].numeroAdjacentes;i++){
            if ((k == grafo[k].adjacentes[i].ramo->k) && (barra == grafo[k].adjacentes[i].ramo->m)){
                if (opt == 0) opt1 = 2;
                else opt1 = 3;  
                dSkm(&grafo[k],&grafo[grafo[k].adjacentes[i].idNo],grafo[k].adjacentes[i].ramo, dSaux, opt1, fase);
                break;
            }
            else if ((barra == grafo[k].adjacentes[i].ramo->k) && (k == grafo[k].adjacentes[i].ramo->m)){
                if (opt == 0) opt1 = 0;
                else opt1 = 1;
                dSmk(&grafo[grafo[k].adjacentes[i].idNo],&grafo[k], grafo[k].adjacentes[i].ramo, dSaux, opt1, fase);
                break;
            }
        }
        for(j=0;j<3;j++){
            dS[j] = dS[j] - dSaux[j];
        }
    }
    free(dSaux);
    for(i=0;i<3;i++) free(J[i]);
    free(J);
}

//void dIkm




//void dImk


//------------------------------------------------------------------------------
// Coordenadas Retangulares
//
//------------------------------------------------------------------------------
void dSkm_ret(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dS, long int opt, long int i){ //fase i
    int j,k;
    __complex__ double Vp[3], Vs[3], **J = NULL;
    
    J = c_matAloca(3);
    J[i][i] = 1;
    
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
        
    switch (opt){
        case 0: //Derivada em relação a tensão real do noP - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k]=0;
                for (j=0;j<3;j++){
                    dS[k] += conj(ramo->Ypp[k][j])*__real__ Vp[j] - I*conj(ramo->Ypp[k][j])*__imag__ Vp[j];
                    dS[k] += conj(ramo->Yps[k][j])*__real__ Vs[j] - I*conj(ramo->Yps[k][j])*__imag__ Vs[j];
                }
                if (k!=i) dS[k] = 0; //Jff x
                dS[k] += __real__ Vp[k] * conj(ramo->Ypp[k][i]) - I*__imag__ Vp[k] * conj(ramo->Ypp[k][i]);
            }
            break;
        case 1: //Derivada em relação a tensão imaginária do noP - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k]=0;
                for (j=0;j<3;j++){
                    dS[k] += I*conj(ramo->Ypp[k][j])*__real__ Vp[j] + conj(ramo->Ypp[k][j])*__imag__ Vp[j];
                    dS[k] += I*conj(ramo->Yps[k][j])*__real__ Vs[j] + conj(ramo->Yps[k][j])*__imag__ Vs[j];
                }
                if (k!=i) dS[k] = 0; //Jff x
                dS[k] += -I * __real__ Vp[k] * conj(ramo->Ypp[k][i]) + __imag__ Vp[k] * conj(ramo->Ypp[k][i]);
            }
            break;
        case 2:  //Derivada em relação a tensão real do noS - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k] = __real__ Vp[k] * conj(ramo->Yps[k][i]) + I*__imag__ Vp[k] * conj(ramo->Yps[k][i]);
            }
            break;
        case 3: //Derivada em relação a tensão imaginária do noS - DSp / DVpi
            for (k=0;k<3;k++){
                dS[k] = -I * __real__ Vp[k] * conj(ramo->Yps[k][i]) + __imag__ Vp[k] * conj(ramo->Yps[k][i]);
            }
            break;
    }
    free(J);
}

//void dSmk
void dSmk_ret(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dS, long int opt, long int i){ //fase i
    int j,k;
    __complex__ double Vp[3], Vs[3], **J = NULL;
    
    J = c_matAloca(3);
    J[i][i] = 1;
        
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
        
    switch (opt){
        case 0: //Derivada em relação a tensão real do noP - DSs / DVpi
            for (k=0;k<3;k++){
                dS[k] = __real__ Vs[k] * conj(ramo->Ysp[k][i]) + I * __imag__ Vp[k] * conj(ramo->Ysp[k][i]);
            }
            break;
        case 1: //Derivada em relação a tensão imaginária do noP - DSs / DVpi
            for (k=0;k<3;k++){
                dS[k] = -I*__real__ Vs[k] * conj(ramo->Ysp[k][i]) + __imag__ Vp[k] * conj(ramo->Ysp[k][i]);
            }
            break;
        case 2:  //Derivada em relação a tensão real do noS - DSs / DVpi
            for (k=0;k<3;k++){
                dS[k]=0;
                for (j=0;j<3;j++){
                    dS[k] += conj(ramo->Ysp[k][j])*__real__ Vp[j] - I * conj(ramo->Ysp[k][j])*__imag__ Vp[j];
                    dS[k] += conj(ramo->Yss[k][j])*__real__ Vs[j] - I * conj(ramo->Yss[k][j])*__imag__ Vs[j];
                }
                if (k!=i) dS[k] = 0; //Jff x
                dS[k] += __real__ Vs[k] * conj(ramo->Yss[k][i]) + I * __imag__ Vs[k] * conj(ramo->Yss[k][i]);
            }
            break;
        case 3: //Derivada em relação ao ângulo do noS - DSs / DVpi
            for (k=0;k<3;k++){
                dS[k]=0;
                for (j=0;j<3;j++){
                    dS[k] += I * conj(ramo->Ysp[k][j])*__real__ Vp[j] + conj(ramo->Ysp[k][j])*__imag__ Vp[j];
                    dS[k] += I * conj(ramo->Yss[k][j])*__real__ Vs[j] + conj(ramo->Yss[k][j])*__imag__ Vs[j];
                }
                if (k!=i) dS[k] = 0; //Jff x
                dS[k] += -I * __real__ Vs[k] * conj(ramo->Yss[k][i]) + __imag__ Vs[k] * conj(ramo->Yss[k][i]);
            }
            break;
    }
    free(J);
}


//opt = 0 tensão
//opt = 1 angulo em relação à fase "i" da barra 
void dSk_ret(TF_GRAFO *grafo, long int k, __complex__ double *dS, long int opt, long int barra, long int fase){
    int i,j, opt1;
    
    __complex__ double *dSaux;
    dSaux = (__complex__ double*)malloc(3 * sizeof(__complex__ double));
    
    for(j=0;j<3;j++){
        dS[j] = 0;
    }
    if (barra == k){
        for (i=0; i<grafo[k].numeroAdjacentes;i++){
            if ((k == grafo[k].adjacentes[i].ramo->k)){
                if (opt == 0) opt1 = 0;
                else opt1 = 1;
                dSkm_ret(&grafo[k],&grafo[grafo[k].adjacentes[i].idNo],grafo[k].adjacentes[i].ramo, dSaux, opt1, fase);
            }
            else{
                if (opt == 0) opt1 = 2;
                else opt1 = 3;
                dSmk_ret(&grafo[grafo[k].adjacentes[i].idNo],&grafo[k], grafo[k].adjacentes[i].ramo, dSaux, opt1, fase);
            }
            for(j=0;j<3;j++){
                dS[j] = dS[j] - dSaux[j];
            }
        }
        
        
        //Derivada da potência nos shunts
        if (opt==0){//Derivada em relação a tensão real do noP - DSp / DVpi
            for (j=0;j<3;j++){
                if (j==fase){
                    for (i=0;i<3;i++){
                        dS[j] += conj(grafo[k].Ysh[j][i])*__real__ grafo[k].V[i] - I*conj(grafo[k].Ysh[j][i])*__imag__ grafo[k].V[i];
                    }
                }// if (j!=fase) dS[j] = 0; //Jff x
                dS[j] += __real__ grafo[k].V[j] * conj(grafo[k].Ysh[j][fase]) - I*__imag__ grafo[k].V[j] * conj(grafo[k].Ysh[j][fase]);
            }
        }
        else{//opt==1 >> Derivada em relação a tensao imaginaria do noP - DSp / DVpi
            for (j=0;j<3;j++){
                if (j==fase){
                    for (i=0;i<3;i++){
                        dS[j] += I*conj(grafo[k].Ysh[j][i])*__real__ grafo[k].V[i] + conj(grafo[k].Ysh[j][i])*__imag__ grafo[k].V[i];
                    }
                }//  if (j!=fase) dS[j] = 0; //Jff x
                dS[j] += -I * __real__ grafo[k].V[j] * conj(grafo[k].Ysh[j][fase]) + __imag__ grafo[k].V[j] * conj(grafo[k].Ysh[j][fase]);
            }
        }       
        
    }
    else{
        for (i=0; i<grafo[k].numeroAdjacentes;i++){
            if ((k == grafo[k].adjacentes[i].ramo->k) && (barra == grafo[k].adjacentes[i].ramo->m)){
                if (opt == 0) opt1 = 2;
                else opt1 = 3;  
                dSkm_ret(&grafo[k],&grafo[grafo[k].adjacentes[i].idNo],grafo[k].adjacentes[i].ramo, dSaux, opt1, fase);
                break;
            }
            else if ((barra == grafo[k].adjacentes[i].ramo->k) && (k == grafo[k].adjacentes[i].ramo->m)){
                if (opt == 0) opt1 = 0;
                else opt1 = 1;
                dSmk_ret(&grafo[grafo[k].adjacentes[i].idNo],&grafo[k], grafo[k].adjacentes[i].ramo, dSaux, opt1, fase);
                break;
            }
        }
        for(j=0;j<3;j++){
            dS[j] = dS[j] - dSaux[j];
        }
    }
    free(dSaux);
}



//Derivada da Magnitude de Corrente no ramo k->m 
//opt = 0 tensão noP
//opt = 1 angulo noP
//opt = 2 tensão noS
//opt = 3 angulo noS em relação à fase "i"
void dIkm(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dI, long int opt, long int i){ //fase i
    int j,k;
    __complex__ double Vp[3], Vs[3], **J = NULL, P1,P2,P3;
    __complex__ double dS[3];
    __complex__ double S[3];
    
    
    J = c_matAloca(3);
    J[i][i] = 1;
        
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
    
    
    //Derivada do Fluxo de Potência
    dSkm(noP,noS,ramo,dS,opt,i);       
    Skm(noP, noS, ramo, S);
        
    switch (opt){
        case 0: //Derivada em relação a tensão do noP - DSp / DVpi
            for (k=0;k<3;k++){
//                dI = 1/(bar[k].V)*(Pkm(km)/S*dPkm(km,3) + Qkm(km)/S*dQkm(km,3)) - S/pow(bar[k].V,2);
                dI[k] = 1/cabs(Vp[i]) * (__real__ S[k]/cabs(S[k]) * __real__ dS[k] +  __imag__ S[k]/cabs(S[k]) * __imag__ dS[k]) - S[k]/pow(cabs(Vp[i]),2);
            }
            break;
        case 1: //Derivada em relação ao ângulo do noP - DSp / DVpi
            for (k=0;k<3;k++){
//                dI = 1/(bar[k].V)*(Pkm(km)/S*dPkm(km,1) + Qkm(km)/S*dQkm(km,1));
                dI[k] = 1/cabs(Vp[i]) * (__real__ S[k]/cabs(S[k]) * __real__ dS[k] +  __imag__ S[k]/cabs(S[k]) * __imag__ dS[k]);
            }
            break;
        case 2:  //Derivada em relação a tensão do noS - DSp / DVpi
            for (k=0;k<3;k++){
//                dI = 1/(bar[k].V)*(Pkm(km)/S*dPkm(km,2) + Qkm(km)/S*dQkm(km,2));
                dI[k] = 1/cabs(Vp[i]) * (__real__ S[k]/cabs(S[k]) * __real__ dS[k] +  __imag__ S[k]/cabs(S[k]) * __imag__ dS[k]);
            }
            break;
        case 3: //Derivada em relação ao ângulo do noS - DSp / DVpi
            for (k=0;k<3;k++){
//                dI = 1/(bar[k].V)*(Pkm(km)/S*dPkm(km,4) + Qkm(km)/S*dQkm(km,4));
                dI[k] = 1/cabs(Vp[i]) * (__real__ S[k]/cabs(S[k]) * __real__ dS[k] +  __imag__ S[k]/cabs(S[k]) * __imag__ dS[k]);
            }
            break;
    }
    
    switch (ramo->fases){
        case 1:
            dI[1] = 0;
            dI[2] = 0;
            break;    
        case 2:
            dI[0] = 0;
            dI[2] = 0;
            break;
        case 3:
            dI[1] = 0;
            dI[0] = 0;
            break;
        case 4:
            dI[2] = 0;
            break;    
        case 5:
            dI[1] = 0;
            break;
        case 6:
            dI[0] = 0;
            break; 
    }
    free(J);
}

//Derivada da Magnitude de Corrente no ramo m->k 
//opt = 0 tensão noP
//opt = 1 angulo noP
//opt = 2 tensão noS
//opt = 3 angulo noS em relação à fase "i"
void dImk(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dI, long int opt, long int i){ //fase i
    int j,k;
    __complex__ double Vp[3], Vs[3], **J = NULL, P1,P2,P3;
    __complex__ double dS[3];
    __complex__ double S[3];
    
    
    J = c_matAloca(3);
    J[i][i] = 1;
        
    Vp[0] = noP->V[0];
    Vp[1] = noP->V[1];
    Vp[2] = noP->V[2];
    Vs[0] = noS->V[0];
    Vs[1] = noS->V[1];
    Vs[2] = noS->V[2];
    
    
    //Derivada do Fluxo de Potência
    dSmk(noP,noS,ramo,dS,opt,i);       
    Smk(noP, noS, ramo, S);
        
    switch (opt){
        case 0: //Derivada em relação a tensão do noP - DSp / DVpi
            for (k=0;k<3;k++){
//                dI = 1/(bar[k].V)*(Pkm(km)/S*dPkm(km,3) + Qkm(km)/S*dQkm(km,3)) - S/pow(bar[k].V,2);
                dI[k] = 1/cabs(Vp[i]) * (__real__ S[k]/cabs(S[k]) * __real__ dS[k] +  __imag__ S[k]/cabs(S[k]) * __imag__ dS[k]) - S[k]/pow(cabs(Vp[i]),2);
            }
            break;
        case 1: //Derivada em relação ao ângulo do noP - DSp / DVpi
            for (k=0;k<3;k++){
//                dI = 1/(bar[k].V)*(Pkm(km)/S*dPkm(km,1) + Qkm(km)/S*dQkm(km,1));
                dI[k] = 1/cabs(Vp[i]) * (__real__ S[k]/cabs(S[k]) * __real__ dS[k] +  __imag__ S[k]/cabs(S[k]) * __imag__ dS[k]);
            }
            break;
        case 2:  //Derivada em relação a tensão do noS - DSp / DVpi
            for (k=0;k<3;k++){
//                dI = 1/(bar[k].V)*(Pkm(km)/S*dPkm(km,2) + Qkm(km)/S*dQkm(km,2));
                dI[k] = 1/cabs(Vp[i]) * (__real__ S[k]/cabs(S[k]) * __real__ dS[k] +  __imag__ S[k]/cabs(S[k]) * __imag__ dS[k]);
            }
            break;
        case 3: //Derivada em relação ao ângulo do noS - DSp / DVpi
            for (k=0;k<3;k++){
//                dI = 1/(bar[k].V)*(Pkm(km)/S*dPkm(km,4) + Qkm(km)/S*dQkm(km,4));
                dI[k] = 1/cabs(Vp[i]) * (__real__ S[k]/cabs(S[k]) * __real__ dS[k] +  __imag__ S[k]/cabs(S[k]) * __imag__ dS[k]);
            }
            break;
    }
    
    switch (ramo->fases){
        case 1:
            dI[1] = 0;
            dI[2] = 0;
            break;    
        case 2:
            dI[0] = 0;
            dI[2] = 0;
            break;
        case 3:
            dI[1] = 0;
            dI[0] = 0;
            break;
        case 4:
            dI[2] = 0;
            break;    
        case 5:
            dI[1] = 0;
            break;
        case 6:
            dI[0] = 0;
            break; 
    }
    free(J);
}