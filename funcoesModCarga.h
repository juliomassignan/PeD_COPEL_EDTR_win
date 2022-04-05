/* 
 * File:   funcoesModCarga.h
 * Author: Julio
 *
 * Created on 9 de Janeiro de 2015, 16:03
 */

#ifndef FUNCOESMODCARGA_H
#define	FUNCOESMODCARGA_H
#include "data_structures_modcarga.h"
#include "data_structures.h"

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

//------------------------------------------------------------------------------
// Leitura do arquivo de Curvas de Carga, Tabela CNAE e Tabelas
void leituraTabelaConsumidores(TF_DADOS_CONSUMIDOR **consumidores);

//------------------------------------------------------------------------------
// Leitura do arquivo de Curvas de Carga
void leituraCurvasCarga(FILE *arquivo, TF_CURVA_CARGA **curvas);

//------------------------------------------------------------------------------
// Leitura da Tabela dos dados CNAE
void leituraDadosCNAE(FILE *arquivo, TF_DADOS_CNAE **cnae);

//------------------------------------------------------------------------------
// Leitura da Curvas Agregadas quando não for executar funçoes de agregação
void leituraCurvasAgregadas(char *folder, TF_CURVA_TRAFO **curvasTrafos, TF_DBAR *barras, long int numeroBarrasParam);

//------------------------------------------------------------------------------
// Leitura da Tabela de Consumidores
void leituraConsumidores(FILE *arquivo, TF_DADOS_CONSUMIDOR **consumidores, TF_CURVA_CARGA *curvas, TF_DADOS_CNAE *cnae);

//------------------------------------------------------------------------------
// Executa a agregação de cargas
void agregacaoCargas(TF_CURVA_TRAFO **curvasTrafos, long int numeroBarrasParam, TF_DADOS_CONSUMIDOR *consumidores);

//------------------------------------------------------------------------------
// Atualiza os valores de P e Q para o valor agregado na estampa de tempo
void inicializaPQcurvas(TF_DBAR *barras, TF_CURVA_TRAFO *curvasTrafos, long int numeroBarrasParam,  long int estampa_tempo, TF_TIPO_AGREGACAO_CARGAS tipo, double Sbase);

//------------------------------------------------------------------------------
//FUNÇÕES PARA IMPRESSÃO DE DADOS
void imprimeConsumidores(TF_DADOS_CONSUMIDOR *consumidores);
void imprimeCurvasAgregadas(TF_CURVA_TRAFO *curvasTrafos, long int numeroBarrasParam);
void imprimeAMs(TF_AREA_MEDICAO *areasAM);
void imprimePQ_AMs(TF_AREA_MEDICAO *areasAM,TF_GRAFO *grafoSDRParam,long int numeroBarras, long int estampa_tempo,const char *modo);

#endif	/* FUNCOESMODCARGA_H */

