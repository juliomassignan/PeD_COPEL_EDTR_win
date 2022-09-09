
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <ctype.h>

#include "data_structures_tf.h"



void constroi_dmed_prev(TF_DPREV *prev_tf, int nmed,TF_DMED ***medidas)
{
    // aloca espaço para as medidas 

    if ((((*medidas) = (TF_DMED **)malloc( nmed*sizeof(TF_DMED*)))==NULL))
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as medidas !!!!");
        exit(1); 
    }
    for (size_t i = 0; i < nmed; i++)
    {
        (*medidas)[i]=(TF_DMED *)malloc( sizeof(TF_DMED));
    }
    

    for (size_t i = 0; i < nmed; i++)
    {
        medidas[0][i]=&(prev_tf[i].DMED);
    }
    

    
}

void constroi_dmed_prev2(TF_DPREV *prev_tf, int nmed,TF_DMED **medidas)
{
    // aloca espaço para as medidas 
    if ((((*medidas) = (TF_DMED *)malloc((nmed+1) * sizeof(TF_DMED)))==NULL))
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as medidas !!!!");
        exit(1); 
    }

    for (size_t i = 0; i < nmed; i++)
    {
        (*medidas)[i]=(prev_tf[i].DMED);
    }
    

    
}


void atualiza_dmed(int instante_atual,TF_DPREV *prev_tf, int nmed)
{
    for (size_t i = 0; i < nmed; i++)
    {
        prev_tf[i].DMED.zmed = prev_tf[i].prev[instante_atual]/prev_tf[i].base;
        prev_tf[i].DMED.sigma = prev_tf[i].prec[instante_atual]/prev_tf[i].base;
        prev_tf[i].DMED.prec= prev_tf[i].prec[instante_atual]; 
    }
    

}