/* 
 * File:   funcoesProblema.h
 * Author: sony
 *
 * Created on 6 de Dezembro de 2013, 15:22
 */

#ifndef FUNCOESPROBLEMA_H
#define	FUNCOESPROBLEMA_H
#include "data_structures.h"

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif
void constroiMatrizZ(GRAFO *grafoSDRParam, MATRIZCOMPLEXA **ZParam, long int numeroBarrasParam);
__complex__ double valorZ(MATRIZCOMPLEXA *ZParam, long int idNo1, long int idNo2);
void imprimeMatrizZ(MATRIZCOMPLEXA *ZParam, long int numeroBarrasParam);
void constroiMatrizMaximoCorrente(GRAFO *grafoSDRParam, MATRIZMAXCORRENTE **maximoCorrenteParam, long int numeroBarrasParam);
double maximoCorrente(MATRIZMAXCORRENTE *maximoCorrenteParam, long int idNo1, long int idNo2);
void numeroManobras(CONFIGURACAO *configuracoesParam, 
        long int idChaveAberta, long int idChaveFechada, LISTACHAVES *listaChavesParam,
        int idConfiguracaoParam, ESTADOCHAVE *estadoInicialChaves, long int *numChaveAberta, 
        long int *numChaveFechada, int idNovaConfiguracaoParam);
void buscaNosRestabelecimento(long int *a, CONFIGURACAO *configuracoesParam, int rnpP, 
        long int indiceP, GRAFOSETORES *grafoSetoresParam, LISTACHAVES *listaChavesParam, 
        int *rnpA, long int *indiceA, int idConfiguracaoParam, long int indiceL, 
        long int *nosFalta, int *numeroFalta, long int *r, long int *indiceR, 
        ESTADOCHAVE *estadoInicial, MATRIZPI *matrizPiParam, VETORPI *vetorPiParam);
void constroiRNPDestinoRestabelecimento(CONFIGURACAO *configuracoesParam, int rnpP, 
        int rnpA, int indiceL, int indiceP, int indiceA, int indiceR, MATRIZPI *matrizPiParam, 
        long int idConfiguracaoParam, RNP *rnpDestino, long int idNovaConfiguracaoParam);
void constroiRNPOrigemRestabelecimento(CONFIGURACAO *configuracoesParam, int rnpP, 
        int indiceL, int indiceP, MATRIZPI *matrizPiParam, long int idConfiguracaoParam, RNP *rnpOrigem, long int idNovaConfiguracaoParam);
void fluxoCargaTodosAlimentadores(long int numeroBarrasParam, 
        DADOSALIMENTADOR *dadosAlimentadorParam, DADOSTRAFO *dadosTrafoParam,
        CONFIGURACAO *configuracoesParam, long int indiceConfiguracao, RNPSETORES *matrizB,
        MATRIZCOMPLEXA *ZParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulador, MATRIZMAXCORRENTE * maximoCorrenteParam);
void fluxoCargaAlimentador(int numeroBarrasParam, CONFIGURACAO *configuracoesParam, 
        double VFParam, int indiceRNP, int indiceConfiguracao, RNPSETORES *matrizB,
        MATRIZCOMPLEXA *ZParam, MATRIZMAXCORRENTE *maximoCorrenteParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulador);
void avaliaConfiguracao(BOOL todosAlimentadores, CONFIGURACAO *configuracoesParam, int rnpA, int rnpP, long int idNovaConfiguracaoParam, DADOSTRAFO *dadosTrafoParam, int numeroTrafosParam,
        int numeroAlimentadoresParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulador, DADOSALIMENTADOR *dadosAlimentadorParam, double VFParam, int idAntigaConfiguracaoParam, RNPSETORES *matrizB, MATRIZCOMPLEXA *ZParam,
        MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam, BOOL copiarDadosEletricos);
void imprimeIndicadoresEletricos(CONFIGURACAO configuracaoParam);
void carregamentoPerdas(CONFIGURACAO *configuracaoParam, int indiceRNP, RNPSETORES *matrizB, 
        MATRIZCOMPLEXA *ZParam, MATRIZMAXCORRENTE *maximoCorrenteParam);
void calculaPonderacao(CONFIGURACAO *configuracoesParam, long int idConfiguracaoParam, double VFParam);
void reconfigurador(GRAFO *grafoSDRParam, long int numeroBarras,  DADOSTRAFO *dadosTrafoSDRParam, long int numeroTrafos, 
        DADOSALIMENTADOR *dadosAlimentadorSDRParam, long int numeroAlimentadores, DADOSREGULADOR *dadosReguladorSDR,
        long int numeroReguladores, RNPSETORES *rnpSetoresParam, long int numeroSetores, GRAFOSETORES *grafoSetoresParam, 
        LISTACHAVES *listaChavesParam, long int numeroChaves, CONFIGURACAO *configuracaoInicial, MATRIZMAXCORRENTE *maximoCorrente,
    MATRIZCOMPLEXA *Z, ESTADOCHAVE *estadoInicial, ESTADOCHAVE *estadoAutomaticas, ESTADOCHAVE *estadoRestabelecimento);
void desalocaMatrizZ(MATRIZCOMPLEXA *ZParam, long int numeroBarrasParam);
void desalocaMatrizCorrente(MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam);
void buscaTodosNosRestabelecimento(CONFIGURACAO *configuracoesParam, int rnpP,
        long int indiceP, GRAFOSETORES *grafoSetoresParam, int idConfiguracaoParam, long int indiceL,
        long int *nosFalta, int numeroFalta, MATRIZPI *matrizPiParam, VETORPI *vetorPiParam, 
        NOSRESTABELECIMENTO *nosParam, long int indiceFalta);
BOOL verificaFactibilidade(CONFIGURACAO configuracaoParam);
//long int melhorSolucao(CONFIGURACAO *configuracoesParam, VETORTABELA *tabelasParam);
void isolaRestabeleceTodasOpcoes(GRAFOSETORES *grafoSetoresParam, long int idConfiguracaoParam,
        CONFIGURACAO *configuracoesParam, MATRIZPI *matrizPiParam,
        VETORPI *vetorPiParam, long int setorFaltaParam, ESTADOCHAVE *estadoInicial, long int * numChaveAberta,
        long int *numChaveFechada, long int *idNovaConfiguracaoParam, LISTACHAVES *listaChavesParam);
void isolaRestabeleceMultiplasFaltas(GRAFOSETORES *grafoSetoresParam, long int idConfiguracaoParam,
        CONFIGURACAO *configuracoesParam, MATRIZPI *matrizPiParam,
        VETORPI *vetorPiParam, long int numeroFaltasParam, long int *setorFaltaParam, ESTADOCHAVE *estadoInicial, long int * numChaveAberta,
        long int *numChaveFechada, long int *idNovaConfiguracaoParam, long int *idConfiguracaoIniParam, LISTACHAVES *listaChavesParam);
void sequenciaChaveamento(FILE *arquivo, VETORPI *vetorPiParam, long int idConfiguracaoParam, LISTACHAVES *listaChavesParam);
void salvaDadosArquivo(char *nomeArquivo, char *nomeFinal, CONFIGURACAO *configuracoesParam, VETORTABELA *tabelas, int numeroTabelas, VETORPI *vetorPiParam, LISTACHAVES *listaChavesParam); 
long int melhorSolucao(CONFIGURACAO *configuracoesParam, VETORTABELA *tabelasParam);
long int melhorSolucaoNaoFactivel(CONFIGURACAO *configuracoesParam, VETORTABELA *tabelasParam) ;
void correnteJusante(int idRNP, int carga, int iteracao,
        CONFIGURACAO configuracaoParam, RNPSETORES *matrizB, int *indiceRegulador, DADOSREGULADOR *dadosRegulador);
void constroiVetorTaps(int *tapsParam, DADOSREGULADOR *dadosReguladorParam, long int numeroReguladoresParam, GRAFO *grafoSDRParam, long int numeroBarrasParam);


#endif	/* FUNCOESPROBLEMA_H */

