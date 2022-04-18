/* 
 * File:   funcoesInicializacao.h
 * Author: sony
 *
 * Created on 10 de Novembro de 2013, 11:30
 */

#ifndef FUNCOESINICIALIZACAO_H
#define	FUNCOESINICIALIZACAO_H
#include "funcoesRNP.h"

//int numeroSetores; /**< Variável global para armazenar o número de setores do SDR. */ duplicada
extern long int numeroAlimentadores; /**< Variável global para armazenar o número de alimentadores do SDR. */

/**
 *  Este método é responsável por atualizar o estado de todas as chaves do Sistema de DistribuiÃ§Ã£o a partir do arquivo de AtuaÃ§Ã£o de Chaves.
 *  O método faz a leitura do arquivo ESTADOS_CHS_"data e hora".dad a atribui o estado de chaves atualizado de acordo com o FEATNUM.
 *  Tem como parâmetros o grafo que representa o sistema
 * @param grafoSDRParam
 * 
 */
void atualizaEstadoChaves(GRAFO *grafoSDRParam, long int numeroBarras);

/**
 *  Este método é responsável por identificar todas as chaves do Sistema de Distribuição e armazená-las em uma estrutura.
 *  O método também atribui a cada chave um identificador único, que será utilizado para identificar as chaves manobradas.
 *  Tem como parâmetros o grafo que representa o sistema e a estrutura onde serão armazenadas as chaves
 * @param grafoSDRParam
 * @param listaChavesParam
 */
void constroiListaChaves(GRAFO *grafoSDRParam, LISTACHAVES **listaChavesParam, long int numeroChaves, long int numeroBarras);
/**
 * Função que retorna o identificador de uma chave dados os valores da barras correspondentes
 * Recebe como parâmetros a lista de chaves (LISTACHAVES) e o valor das duas barras
 * @param listaChavesParam
 * @param idNo1
 * @param idNo2
 * @return 
 */
int buscaChave(LISTACHAVES *listaChavesParam, int idNo1, int idNo2, long int numeroChaves);

/**
 * Função que retorna o identificador de uma chave dados os valores da barras correspondentes
 * Recebe como parâmetros a lista de chaves (LISTACHAVES) e o valor das duas barras
 * @param listaChavesParam
 * @param idNo1
 * @param idNo2
 * @return 
 */
void buscaProfundidadeSetores(int idSetorParam, BOOL *visitadoParam,RNP *rnpParam, int profundidade, int *indice, GRAFOSETORES *grafoSetoresParam, LISTACHAVES *listaChavesParam);

/**
 * Percorre a lista de alimentadores do sistema com os seus repectivos setores, construindo as RNPs da configuração inicial.
 * Esse processo considera o estado das chaves. 
 * Tem como parâmentros o grafo de setores, o grafo do sistema, a lista de chaves, a lista de alimentadores, a matriz PI ra RNP e lista de configurações 
 * @param grafoSetoresParam
 * @param grafoSDRParam
 * @param listaChavesParam
 * @param alimentadoresParam
 * @param matrizPIParam
 * @param populacaoParam
 */
void constroiIndividuoInicial(GRAFOSETORES *grafoSetoresParam, GRAFO *grafoSDRParam, LISTACHAVES *listaChavesParam, DADOSALIMENTADOR *alimentadoresParam, CONFIGURACAO *populacaoParam);
void imprimeDadosEletricos(CONFIGURACAO *configuracoesParam, long int indiceConfiguracaoParam, long int numeroBarrasParam) ;
#ifdef	__cplusplus
extern "C" {
#endif

void imprimeListaChaves(LISTACHAVES *listaChavesParam, long int numeroChaves);
void inicializaDadosEletricosPorAlimentador(GRAFO *grafoSDRParam, CONFIGURACAO *configuracoesParam, long int indiceConfiguracaoParam, long int numeroBarrasParam, int sBaseParam, DADOSTRAFO *dadosTrafoParam, DADOSALIMENTADOR *dadosAlimentadorParam, RNPSETORES *matrizB);
void imprimeBarrasIsoladas(long int numeroBarrasParam, GRAFO *grafoSDRParam);
void salvaChavesCompleto(long int numeroChavesParam, LISTACHAVES *listaChavesParam);
void salvaChaves(long int numeroChavesParam, LISTACHAVES *listaChavesParam);
void gravaDadosBarras(long int numeroBarrasParam, GRAFO *grafoSDRParam);
void salvaLinhas(long int numeroBarrasParam, GRAFO *grafoSDRParam);
void gravaDadosTrafo(long int numeroTrafosParam, DADOSTRAFO *trafoSDRParam);
void salvaChavesAMontante(LISTACHAVES *listaChavesParam, CONFIGURACAO individuo, GRAFOSETORES *grafoSetores);
void gravaMatrizZCorrente(MATRIZCOMPLEXA *ZParam, MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam);
void gravaMatrizCorrente(MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam);
void constroiMatrizZ(GRAFO *grafoSDRParam, MATRIZCOMPLEXA **ZParam, long int numeroBarrasParam);
void constroiMatrizMaximoCorrente(GRAFO *grafoSDRParam, MATRIZMAXCORRENTE **maximoCorrenteParam, long int numeroBarrasParam);
void contaSetoresAlimentador(int idSetorParam, BOOL *visitadoParam, int *numeroSetores, GRAFOSETORES *grafoSetoresParam, LISTACHAVES *listaChavesParam);

void gravaBarrasRT(DADOSREGULADOR *dadosReguladorParam, long int numeroReguladoresParam, GRAFO *grafoSDRParam, long int numeroBarrasParam);
#ifdef	__cplusplus
}
#endif

#endif	/* FUNCOESINICIALIZACAO_H */

