
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <ctype.h>

#include "data_structures_tf.h"
#include "funcoesLeitura_tf.h"
#include "data_structures_modcarga_tf.h"



void constroi_dmed_prev(TF_DPREV *prev_tf, int nmed,TF_DMED **medidas,TF_GRAFO*grafo,double Sbase)
{
    // aloca espaço para as medidas 

    int i,j;

    if ((((*medidas) = (TF_DMED *)malloc( nmed*sizeof(TF_DMED)))==NULL))
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as medidas !!!!");
        exit(1); 
    }
    

    for (size_t i = 0; i < nmed; i++)
    {
        (*medidas)[prev_tf[i].id]=prev_tf[i].DMED;
    }
    

    //associa medidores no grafo
    
    for (i=0;i<nmed;i++)
    {
        AssoMedGraf(grafo,&((*medidas)[i]),Sbase);
    }

    AssoMedidoresPares((*medidas),nmed);

    
}


void atualiza_dmed(int instante_atual,TF_DPREV *prev_tf, int nmed, TF_DMED *medidas)
{
    for (size_t i = 0; i < nmed; i++)
    {
        medidas[prev_tf[i].id].zmed = prev_tf[i].prev[instante_atual]/prev_tf[i].base;
        medidas[prev_tf[i].id].sigma = prev_tf[i].prec[instante_atual]/prev_tf[i].base;
        medidas[prev_tf[i].id].prec= prev_tf[i].prec[instante_atual]; 
    }
    

}

void atualiza_AM(TF_GRAFO * grafo, long int numeroBarras, TF_DMED*medidas, long int numeroMedidas, TF_AREA_MEDICAO *areasAM) {
    int i, j, idAlim, contador, par;
           
    TF_FILABARRAS *barraAtual;
    //Soma número de medidas na tabela de entrada


    // Aloca Áreas de Medição
    for (i=0; i< numeroMedidas; i++){
        
        if ((medidas[i].tipo == 0) ||(medidas[i].tipo == 2)){ // Medida de potência ativa e reativa
            contador=medidas[i].idArea;
            par=medidas[i].par;
                areasAM[contador].Pmed = medidas[i].zmed;
                areasAM[contador].Qmed = medidas[par].zmed;
                
                //Cálculo do desvio padrão
                areasAM[contador].stdPmed = cabs(medidas[i].zmed)*medidas[i].prec/3 + 0.00001;
                areasAM[contador].stdQmed = cabs(medidas[par].zmed)*medidas[par].prec/3 + 0.00001;
                
                areasAM[contador].fp = cos(atan2(areasAM[contador].Qmed,areasAM[contador].Pmed));
            }                        
            else if(medidas[i].tipo == 6){ // Medida de magnitude de corrente
                contador=medidas[i].idArea;
                areasAM[contador].tipo = 2;                
                areasAM[contador].Imed = medidas[i].zmed;
                areasAM[contador].fp = 0.92;
                
                //Cálculo do desvio padrão
                areasAM[contador].stdImed = cabs(medidas[i].zmed)*medidas[i].prec/3 + 0.00001;
                
            }
  
        // Montar um else para medida de magnitude de tensão
    }
    
}

TF_NCRESULT NowCastingDemanda(TF_GRAFO *grafo_tf, long int numeroBarras_tf, TF_ALIMENTADOR *alimentador_tf, long int numeroAlimentadores, TF_DRAM *ramo_tf,double Sbase, long int **interfaceNiveis_tf,long int numeroInterfaces_tf, TF_AREA_MEDICAO *areasMedicao_tf, TF_DPREV *prev_tf, TF_DMED *medidaPrev_tf ,long int **numeroMedidasTabela, int numeroAmostras)
{

    int nmed=0;
    TF_NCRESULT result;

    nmed=0;
    for (size_t i = 0; i < 14; i++){ 
         for (size_t j = 0; j < 20; j++){
              nmed += numeroMedidasTabela[i][j];
         }
    }
    
    for (size_t instante_atual = 0; instante_atual < numeroAmostras; instante_atual++)
    {

        // receber a curva trafos
        // fazer rodar toda vez que mudar a hora do instante atual e colocar na inicializa
        // inicializaPQcurvas(barra_tf, curvasTrafos, numeroBarras_tf,  estampa_tempo, VALOR_ESPERADO, Sbase); 

        atualiza_dmed(instante_atual,prev_tf,nmed,medidaPrev_tf); 
        atualiza_AM(grafo_tf,numeroBarras_tf,medidaPrev_tf,nmed,areasMedicao_tf);
        //Estimador de Demandas Trifásicas
        estimadorDemandaTrifasico(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores, ramo_tf, Sbase, interfaceNiveis_tf, numeroInterfaces_tf, areasMedicao_tf);
    }
    
    result.namostras=numeroAmostras;

    return(result);
}