/**
 * @file funcoesLeituraDados.h
 * @author Telma Woerle de Lima Soares
 * @date 12 Sep 2013
 * @brief Contém a descrição das funções utilizadas para a leitura dos dados no novo formato.
 *
 */

#ifndef FUNCOESLEITURADADOS_H
#define	FUNCOESLEITURADADOS_H
#include "data_structures.h"

/**
 * @brief Função principal para a leitura do arquivo dados_barras_linhas_trafos.dad.
 *
 * Essa função realiza a leitura do arquivo dados_barras_linhas_trafos.dad por meio das 
 * funções leituraBarras, leiturasLinhas e leituraTrafos. Essa função principal é responsável por 
 * localizar os marcadores DBAR, DLIM e DTRAFO. A função assume que o nome do arquivo é padrão da forma 
 * dados_barras_linhas_trafos.dad. Além disso o arquivo possui como separação dos dados os marcadores especificados.
 * Ao localizar o marcador o fluxo de controle é desviado para a função correspondente para a leitura dos dados referentes ao marcador.
 * Recebe como parâmetros de entrada e saída um ponteiro para ponteiro do tipo GRAFO @p **grafoSDRParam e
 * um ponteiro para ponteiro do tipo DADOSTRAFO @p  **dadosTrafoSDRParam.
 * A função retorna @c BOOL indicando se a leitura foi bem sucedida.
 * Para utilizar a função:
 * @code
 * GRAFO *grafoExemplo;
 * DADOSTRAFO *trafoExemplo;
 * BOOL leitura = leituraBarrasLinhasTrafos( &grafoExemplo, &trafoExemplo);
 * if (leitura)
 *      printf("leitura concluida\n");
 * @endcode
 * 
 * @param grafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo do GRAFO, onde é retornado o SDR na forma de um grafo.
 * @param dadosTrafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo DADOSTRAFO, onde é retornado os dados dos trafos do SDR.
 * @return Retorna o tipo booleano definido indicando que a leitura foi bem sucedida.
 * @see leituraBarras
 * @see leituraLinhas
 * @see leituraTrafos
 * @note
 * @warning .
 */
BOOL leituraBarrasLinhasTrafos(GRAFO **grafoSDRParam, DADOSTRAFO **dadosTrafoSDRParam, long int *numeroBarras, long int *numeroTrafos, long int *numeroChaves);
/**
 * @brief Função auxiliar para a leitura do arquivo dados_barras_linhas_trafos.dad, referente ao trecho relativo as informações das barras. 
 *
 * Essa função realiza a leitura do trecho do arquivo dados_barras_linhas_trafos.dad relativo ao marcador DBAR 
 * que contém os dados relativos as barras do sistema. Também é responsável pela alocação de memória necessária para armazenar o SDR
 * na forma de um grafo. Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo GRAFO., @p **grafoSDRParam .
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param grafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo do GRAFO, onde é retornado o SDR na forma de um grafo.
 * @return void.
 * @see leituraBarrasLinhasTrafos
 * @note Para realizar a leitura dos dados de P e Q, que são constituídos de 24 pares, utilizou-se a string de controle %n. 
 * Essa string de controle retorna a quantidade de caracteres lidos. Com isso é possível no sscanf fazer a alteração do ponteiro
 *  e utilizar uma estrutura de repetição para realizar a leitura desses dados. 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraBarras(FILE *arquivo, GRAFO **grafoSDRParam, long int *numeroBarras);
/**
 * @brief Função auxiliar para a leitura do arquivo dados_barras_linhas_trafos.dad, referente ao trecho relativo as informações das linhas. 
 *
 * Essa função realiza a leitura do trecho do arquivo dados_barras_linhas_trafos.dad relativo ao marcador DLIN 
 * que contém os dados relativos as linhas do sistema. Nessa função é construída a lista de adjacentes de cada nó da representação do SDR na 
 * forma de grafos. Recebe como parâmetro de entrada e saída um ponteiro do tipo GRAFO., @p *grafoSDRParam .
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param grafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo do GRAFO, onde é retornado o SDR na forma de um grafo.
 * @return void
 * @see leituraBarrasLinhasTrafos
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraLinhas(FILE *arquivo, GRAFO *grafoSDRParam, long int *numeroChaves);

/**
 * @brief Função auxiliar para a leitura do arquivo dados_barras_linhas_trafos.dad, referente ao trecho relativo as informações dos trafos. 
 *
 * Essa função realiza a leitura do trecho do arquivo dados_barras_linhas_trafos.dad relativo ao marcador DTRAFO 
 * que contém os dados relativos aos trafos do sistema. Nessa função é alocada a estrutura que armazena os dados dos trafos.
 * Para isso ao função recebe como parâmetro de entrada e saída e um ponteiro para o ponteiro do tipo DADOSTRAFO, @p **dadosTrafoSDRParam .
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param dadosTrafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo do DADOSTRAFO, onde os dados do trafo são retornados.
 * @return void
 * @see leituraBarrasLinhasTrafos
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraTrafos(FILE *arquivo, DADOSTRAFO **dadosTrafoSDRParam, long int *numeroTrafos);
/**
 * @brief Função principal para a leitura do arquivo dados_alimentadores.dad.
 *
 * Essa função realiza a leitura do arquivo dados_alimentadores.dad. A função assume que o nome do arquivo é padrão da forma 
 * dados_alimentadores.dad. O arquivo inicia com o marcador DALIM que é verificado pela função.
 * Localizado o marcador a função dá procedimento a leitura dos dados do arquivo armazenando-os na estrutura de dados definida.
 * Tem como parâmentro de entrada e saída um ponteiro para ponteiro da estrutura do tipo DADOS ALIMNETADORES @p **dadosAlimentadoresSDRParam.
 * A função retorna @c BOOL indicando se a leitura foi bem sucedida.
 * Para utilizar a função:
 * @code
 * DADOSALIMENTADORES *alimentadorExemplo;
 * BOOL leitura = leituraDadosAlimentadores( &alimentadorExemplo);
 * if (leitura)
 *      printf("leitura concluida\n");
 * @endcode
 * 
 * @param dadosAlimentadoreSDRParam é um ponteiro para o ponteiro da estrutura do tipo do DADOSALIMENTADORES, onde é retornado os dados dos alimentadores.
 * @return Retorna o tipo booleano definido indicando que a leitura foi bem sucedida.
 */
BOOL leituraDadosAlimentadores(DADOSALIMENTADOR **dadosAlimentadorSDRParam);
/**
 * @brief Função principal para a leitura do arquivo dados_reguladores_tensao.dad.
 *
 * Essa função realiza a leitura do arquivo dados_reguladores_tensao.dad. A função assume que o nome do arquivo é padrão da forma 
 * dados_reguladores_tensao.dad. O arquivo inicia com o marcador DBRDT que é verificado pela função.
 * Localizado o marcador a função dá procedimento a leitura dos dados do arquivo armazenando-os na estrutura de dados definida.
 * Tem como parâmentro de entrada e saída um ponteiro para ponteiro da estrutura do tipo DADOSREGULADOR @p **dadosReguladoresSDRParam.
 * A função retorna @c BOOL indicando se a leitura foi bem sucedida.
 * Para utilizar a função:
 * @code
 * DADOSREGULADOR *reguladorExemplo;
 * BOOL leitura = leituraDadosReguladoresTensao( &reguladorExemplo);
 * if (leitura)
 *      printf("leitura concluida\n");
 * @endcode
 * 
 * @param dadosReguladorSDRParam é um ponteiro para o ponteiro da estrutura do tipo do DADOSREGULADOR, onde é retornado os dados dos reguladores de tensão.
 * @return Retorna o tipo booleano definido indicando que a leitura foi bem sucedida.
 */
BOOL leituraDadosReguladoresTensao(DADOSREGULADOR **dadosReguladorSDRParam);

/**
 * @brief Função para imprimir os dados lidos sobre o SDR representado na forma de um grafo.
 *
 * Essa função recebe como parâmetro uma variavél do tipo GRAFO e realiza a impressão
 * na tela dos principais dados contidos na estrutura informada. É utilizada para validar o processo executado pela função de leitura.
 
 * Exemplo:
 * @code
 * GRAFO *grafoExemplo;
 * DADOSTRAFO *trafoExemplo;
 * BOOL leitura = leituraBarrasLinhasTrafos( &grafoExemplo, &trafoExemplo);
 * imprimeGrafo(grafoExemplo);
 * @endcode
 * 
 * @param grafoSDRParam é um ponteiro da estrutura do tipo do GRAFO.

 */
void imprimeGrafo(GRAFO *grafoSDRParam, long int numeroBarras);
/**
 * @brief Função para imprimir os dados lidos sobre os trafos do SDR.
 *
 * Essa função recebe como parâmetro uma variavél do tipo DADOSTRAFO e realiza a impressão
 * na tela dos dados contidos na estrutura  informada. É utilizada para validar o processo executado pela função de leitura.
 
 * Exemplo:
 * @code
 * GRAFO *grafoExemplo;
 * DADOSTRAFO *trafoExemplo;
 * BOOL leitura = leituraBarrasLinhasTrafos( &grafoExemplo, &trafoExemplo);
 * imprimeTrafos(trafoExemplo);
 * @endcode
 * 
 * @param dadosTrafoSDRParam é um ponteiro da estrutura do tipo do DADOSTRAFO.

 */
void imprimeTrafos(DADOSTRAFO *dadosTrafoSDRParam, long int numeroTrafos);

void leituraParametros(long int *numeroGeracoes, int *tamanhoTabelas, int *SBase, int *tamanhoPopulacaoInicial, int *seed);
void leituraDadosEntrada(int *numeroSetorFalta, long int **setoresFalta);

#endif	/* FUNCOESLEITURADADOS_H */

