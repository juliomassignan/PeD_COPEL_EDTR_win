/* 
 * File:   funcoesRNP.h
 * Author: sony
 *
 * Created on 10 de Novembro de 2013, 12:10
 */

#ifndef FUNCOESRNP_H
#define	FUNCOESRNP_H
#include "data_structures.h"

#ifdef	__cplusplus
extern "C" {
#endif



#ifdef	__cplusplus
}
#endif
//int numeroSetores;


void insereNos(NOSPAOCAO *conjuntoNosParam, long int p, long int r, long int a, int iadj, int pos);
/**
 * Faz a inserção no vetor pi da RNP da configuração que deu origem a nova configuração e dos nós utilizados
 *
 * @param piParam
 * @param idIndividuoParam
 * @param idAncestralParam
 * @param novosNosParam
 * @param numeroAplicacoes
 */

void atualizaVetorPi(VETORPI *piParam, long int idIndividuoParam, long int idAncestralParam, long int *idChaveAbertaParam, long int *idChaveFechadaParam, int numeroManobras);

/**
 * @brief Encontra o nó adjacente à p ou r passado como parâmetro.
 * A função retorna a coluna da matriz PI relativa ao nó a, por meio da coluna obtém-se a RNP e o índice dela correspondente ao nó
 * Por meio da RNP identificamos os demais dados relativos ao nó.
 * 
 * @param tentativasParam: número de tentativas que serão realizadas na busca pelo nó a
 * @param idNoParam : id do nó p ou r para o qual deseja-se encontrar o adjacente
 * @param idFlorestaParam: identificador da floresta
 * @param idRNPParam: indice da RNP do nó p
 * @param grafoSetoresParam: grafo da lista de adjacência de setores
 * @param matrizPiParam: ponteiro para a matriz PI
 * @param vetorPiParam: ponteiro para o vetor PI
 * @return coluna da matriz relativa ao nó adjacente
 */
COLUNAPI * determinaNoAdjacente(int tentativasParam, long int idNoParam, CONFIGURACAO florestaParam, int idRNPParam, GRAFOSETORES *grafoSetoresParam, MATRIZPI *matrizPiParam, VETORPI *vetorPiParam); 
/**
 * Realiza a busca binária nas colunas da matriz PI de um nó para localizar a posição deste na configuração
 * Recebe como parâmetros a matrizPI do nó, o índice do nó, o valor da busca e o número de colunas preenchidas.
 * Retorna o índice da coluna.
 *  
 * @param matrizPIParam
 * @param indiceNoParam
 * @param valorBusca
 * @param numeroColunasParam
 * @return 
 */
int buscaBinariaColunaPI(MATRIZPI *matrizPIParam, int indiceNoParam, int valorBusca, int numeroColunasParam);

/**
 * Realiza a busca binária na lista de matrizes PI pela matriz de um nó especifico.
 * Retorna o índice da posição.
 * 
 * @param matrizPIParam
 * @param parametroBusca
 * @param ultimaPosicaoParam
 * @return 
 */
int buscaBinariaMatrizPI(MATRIZPI *matrizPIParam, int parametroBusca, int ultimaPosicaoParam);

/**
 * Recebe um nó como parâmetro e retorna a coluna da PI correspondente ao no em sua última alteração relativa a uma 
 * configuração esperada.
 * Tem como parâmetros a matrizPI do nó, o vetorPi de configurações, o nó buscado e a configuração desejada.
 * @param matrizPIParam
 * @param vetorPiParam
 * @param idNoParam
 * @param idIndividuoParam
 * @return 
 */
COLUNAPI *retornaColunaPi(MATRIZPI *matrizPIParam, VETORPI *vetorPiParam, int idNoParam, int idIndividuoParam);

/**
 * Procedimento para adicionar uma coluna na matriz PI de um nó ou, se existir, a coluna para a configuração atualiza os valores
 * Recebe como parâmetros a matriz P, o identificador do nó, o indentificador da configuração, o identificador da RNP, e a posição do nó na RNP
 * @param matrizPIParam
 * @param idNoParam
 * @param idConfiguracaoParam
 * @param idRNPParam
 * @param indiceParam
 */
void adicionaColuna(MATRIZPI *matrizPIParam, int idNoParam, int idIndividuoParam, int idRNPParam, int indiceParam);

/**
 * Realiza a busca na RNP pela posição na RNP que determina o fim da subárvore enraizada no nó informado.
 * Recebe como parâmetros o ponteiro da RNP e o índice do nó raiz da subárvore
 * 
 * @param rnpParam
 * @param indiceNoPParam
 * @return 
 */
int limiteSubArvore(RNP rnpParam, int indiceNoPParam);

/**
 * Esta função retorna um ponteiro para a lista de configurações. Recebe como parâmetros o número de RNPs de cada configuração
 * o identificador do individuo inicial e o número de configurações que compoem a lista.
 * 
 * @param numeroRNPParam
 * @param idIndividuoInicialParam
 * @param numeroConfiguracoesParam
 * @return 
 */
CONFIGURACAO *alocaIndividuo(int numeroRNPParam, long int idIndividuoParam, long int numeroFlorestasParam);

/**
 * Realiza a alocação do vetor de nós da RNP passada como parâmetro. Para isso recebe como parâmetro o número de nós que compõem a árvore
 * representada pela RNP.
 * 
 * @param numeroNosParam
 * @param rnpParam
 */
void alocaRNP(int numeroNosParam, RNP *rnpParam);

/**
 * Realiza a impressão na tela da configuração passada como parâmetro. Para isso faz uso da função que
 * imprime uma RNP.
 * 
 * @param individuoParam
 */
void imprimeIndividuo(CONFIGURACAO individuoParam);

/**
 * Realiza a impressão em arquivo da configuração passada como parâmetro. Para isso faz uso da função que
 * imprime uma RNP.
 * 
 * @param individuoParam
 */
void gravaIndividuo(char *nomeFinal, CONFIGURACAO individuoParam);

/**
 * Imprime na tela a RNP passada como parâmetro. Imprime o vetor de nós, com o identificador do nó e a profundidade
 * @param rnpParam
 */
void imprimeRNP(RNP rnpParam);

/**
 * Libera a memória utilizada para armazenar uma configuração. Recebe como parâmetro a configuração para ser desalocada
 * @param configuracaoParam
 */
void desalocaConfiguracao(CONFIGURACAO individuoParam);

/**
 * Método responsável por fazer a alocação do vetor de MatrizPi para cada nó da configuração, e da matrizPi de cada nó.
 * Além disso, faz a inicialização dos valores de identificação da matriz
 * Recebe como parâmetros o grafo de setores, o vetor de Matriz Pi que será alocado, e o número de colunas da matriz de cada nó. 
 * @param grafoSetoresParam
 * @param matrizPIParam
 * @param numeroMaximoColunasParam
 */
void inicializaMatrizPI(GRAFOSETORES *grafoSetoresParam, MATRIZPI ** matrizPIParam, int numeroMaximoColunasParam, int numeroSetores);

/**
 * Este método realiza a alocação e inicialização do vetor pi da RNP.
 * Tem como parâmetros o numero maximo de configurações e o vetor pi.
 * 
 * @param numeroMaximoConfiguracoesParam
 * @param vetorPiParam
 */
void inicializaVetorPi(int numeroMaximoIndividuosParam, VETORPI **vetorPiParam);


void copiaIndividuoInicial(CONFIGURACAO *configuracoesParam, CONFIGURACAO *configuracaoInicial, long int idNovoIndividuo, MATRIZPI *matrizPIParam, VETORPI *vetorPiParam);
#endif	/* FUNCOESRNP_H */

