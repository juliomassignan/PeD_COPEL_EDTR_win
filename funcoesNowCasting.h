/**
 * @file funcoesNowCasting.h
 * @author Vitor Henrique Pereira de Melo
 * @date 08 set 2022
 * @brief Cabeçalho da biblioteca para leitura de dados trifásico para análise estática de redes de distribuição
 * As funções definidas na biblioteca estão implemetadas em funcoesLeitura.c
 *
 */
#ifndef funcoesNowCasting_TF_H
#define	funcoesNowCasting_TF_H


void constroi_dmed_prev(TF_DPREV *prev_tf, int nmed,TF_DMED **medidas,TF_GRAFO*grafo,double Sbase);

void atualiza_AM(TF_GRAFO * grafo, long int numeroBarras, TF_DMED*medidas, long int numeroMedidas, TF_AREA_MEDICAO *areasAM);

void atualiza_dmed(int instante_atual,TF_DPREV *prev_tf, int nmed, TF_DMED *medidas);

TF_NCRESULT NowCastingDemanda(TF_GRAFO *grafo_tf, long int numeroBarras_tf, TF_ALIMENTADOR *alimentador_tf, long int numeroAlimentadores, TF_DRAM *ramo_tf,double Sbase, long int **interfaceNiveis_tf,long int numeroInterfaces_tf, TF_AREA_MEDICAO *areasMedicao_tf, TF_DPREV *prev_tf, TF_DMED *medidaPrev_tf ,long int **numeroMedidasTabela, int numeroAmostras);

void preenche_result_NC(TF_GRAFO *grafo_tf, int numeroBarras_tf, int instante_atual,TF_NCRESULT * result);


void imprimeDBAR_cargas(TF_GRAFO *grafo_tf, int numerobarras_tf, const char *ts);


#endif