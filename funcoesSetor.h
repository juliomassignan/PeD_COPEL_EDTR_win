/**
 * @file funcoesSetor.h
 * @author Marcos Proenca de Almeida
 * @date 23 Oct 2013
 * @brief Contém as estruturas e a descrição das funções utilizadas para a identificação dos setores.
 *
 */

#ifndef FUNCOESSETOR_H
#define	FUNCOESSETOR_H
#include "data_structures.h"


void adicionaNo(BARRASETOR ** listaSetores, long int idNo);

void imprimeSetor(BARRASETOR * setor);



void adicionaSetorNaLista(LISTASETORES ** listaSetores, BARRASETOR * setor, BARRASETOR *verticesAdj);

void imprimeSetores(LISTASETORES * listaSetores);



void adicionaNoNaFila(FILABARRAS ** fila, long int idNo);

int retiraNoDaFila(FILABARRAS ** fila);


BOOL filaNaoVazia(FILABARRAS * fila);


BOOL ligadoPorTrecho(NOADJACENTE aresta);

BOOL chaveFechada(NOADJACENTE aresta);

void buscaSetor(GRAFO *grafo, long int idNo, LISTASETORES **listaSetores, BOOL *visitado);

LISTASETORES * buscaSetores(GRAFO *grafo, DADOSALIMENTADOR *alimentadores, long int numeroBarras);


void constroiRNP(BARRASETOR *inicioListaSetor, long int idNo, BOOL *visitado, RNPSETOR *rnpParam, int *indice, int profundidade, GRAFO *grafoSDRParam);

void percorreAdjacentesSetor(BARRASETOR *setorParam, BARRASETOR *setorAdjParam, GRAFO *grafoSDRParam, RNPSETOR *rnps,  int numeroNos, long int numeroBarras);

void constroiRNPSetores(LISTASETORES *listaSetores, GRAFO *grafoSDRParam, RNPSETORES **rnpSetores, long int numeroBarras);

void imprimeRNPSetores (RNPSETORES *rnpSetoresParam);

void constroiListaAdjacenciaSetores(GRAFO **grafoSDRParam, LISTASETORES *listaSetores, LISTACHAVES **listaChavesParam, GRAFOSETORES **grafoSetoresParam, long int *numeroChaves, long int *numeroBarras);

void imprimeGrafoSetores(GRAFOSETORES *grafoSetoresParam);


BOOL ligadoPorTrecho(NOADJACENTE aresta);

BOOL chaveFechada(NOADJACENTE aresta);

//retorna se um setor já pertence a lista de adjacencias de outro setor.
BOOL estaListaAdjacenciasSetores(GRAFOSETORES *grafoSetoresSdrParam, long int idSetorRaiz, long int idSetorAdj, int adjacentes);

//retorna se um barra é adjacente a outra;
BOOL estaListaAdjacencias(GRAFO *grafoSdrParam, long int idNoRaiz, long int idNoAdj);

//realiza a busca a rnp do idsetor1 sendo fornecida pelo idSetor2 e retorna essa rnp
RNPSETOR buscaRNPSetor(RNPSETORES *rnpSetoresParam, long int idSetor1, long int idSetor2);

//cria os setores, chaves e barras ficticios para permitir o funcionamento de duas chaves entre o mesmo par de setores
void criaSetorChave(GRAFOSETORES **grafoSetoresParam, GRAFO **grafoSDRParam, LISTACHAVES **listaChavesParam, BARRASETOR *barraSetorAdj,long int barraSetor, LISTASETORES *listaSetores, long int *numeroChaves, long int *numeroBarras, long int idChave);

//localiza uma barra na lista de barras adjacentes
int posicaoBarraAdjacente(GRAFO *grafoSdrParam, long int idNoRaiz, long int idNoAdj);

void imprimeRNPSetor (RNPSETOR rnpSetorParam);

void gravaRNPSetores (RNPSETORES *rnpSetoresParam, long int numeroSetores);

void gravaGrafoSetores(GRAFOSETORES *grafoSetoresParam, long int numeroSetores, LISTACHAVES *listaChavesParam);

#endif	/* FUNCOESLEITURADADOS_H */

