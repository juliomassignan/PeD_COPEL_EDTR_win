/**
 * @file funcoesTopologia.h
 * @author Julio Augusto Druzina Massignan
 * @date 31 jul 2020
 * @brief Cabeçalho da biblioteca para o tratamento da topologia de redes elétricas trifásicas, para cálculos em pu e representação via RNP.
 * As funções definidas na biblioteca estão implemetadas em funcoesTopologia.c
 *
 */
#ifndef FUNCOESTOPOLOGIA_TF_H
#define	FUNCOESTOPOLOGIA_TF_H

//------------------------------------------------------------------------------
//
// ROTINAS AUXILIARES
//
//------------------------------------------------------------------------------
/**
 * @brief Função auxiliar para alocar novo nó em lista encadeada do tipo FILABARRAS
 *
 * Essa função realiza a alocação de um novo nó na última posíção da lista encadeada do tipo TF_FILABARRAS @p **setor com a respectiva identificação 
 * @p idNo para o novo nó.
 * A função retorna @c void .
 * 
 * @param **setor ponteiro para ponteiro com o último elemento da lista encadeada
 * @param idNo inteiro que presenta a identificação do nó
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void adicionaNo(TF_FILABARRAS **setor, long int idNo);

/**
 * @brief Função auxiliar para alocar novo nó em lista encadeada do tipo FILABARRAS
 *
 * Essa função realiza a alocação de um novo nó na última posíção da lista encadeada do tipo TF_FILABARRAS @p **fila com a respectiva identificação 
 * @p idNo para o novo nó.
 * A função retorna @c void .
 * 
 * @param **fila ponteiro para ponteiro com o último elemento da lista encadeada
 * @param idNo inteiro que presenta a identificação do nó
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void adicionaNoNaFila(TF_FILABARRAS ** fila, long int idNo);

/**
 * @brief Função auxiliar para apontar para o próximo nó em lista encadeada do tipo FILABARRAS
 *
 * Essa função altera a posição do ponteiro TF_FILABARRAS @p **fila para o próximo nó da lista encadeada até que NULL seja encontrado.
 * A função retorna @c void .
 * 
 * @param **fila ponteiro para ponteiro com o próximo elemento da lista encadeada
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void apontaProxNoNaFila(TF_FILABARRAS ** fila);

/**
 * @brief Função auxiliar para retira o primeiro nó em lista encadeada do tipo FILABARRAS
 *
 * Essa função retira a primeira a posição do ponteiro TF_FILABARRAS @p **fila e aponta para o próximo nó da lista encadeada até que NULL seja encontrado.
 * A função retorna @c int .
 * 
 * @param **fila ponteiro para ponteiro com o próximo elemento da lista encadeada
 * @return int inteiro com a identificação do nó retirado
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
int retiraNoDaFila(TF_FILABARRAS ** fila);

/**
 * @brief Função auxiliar para testar se a lista encadeada está vazia
 *
 * Essa função testa se a posição atual do ponteiro TF_FILABARRAS @p **fila é NULL.
 * A função retorna @c BOOL .
 * 
 * @param **fila ponteiro com o nó atual da lista encadeada
 * @return BOOL
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL filaNaoVazia(TF_FILABARRAS * fila);

/**
 * @brief Função auxiliar para testar se a lista encadeada contém nó especificado
 *
 * Essa função testa a listaFILABARRAS @p **setor contém o nó especificado pela identificação @p idNo.
 * A função retorna @c BOOL .
 * 
 * @param **fila ponteiro com o nó atual da lista encadeada
 * @param idNo identificação do nó procurado na lista
 * @return BOOL
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL estaLista(TF_FILABARRAS *setor, int idNo);

/**
 * @brief Função auxiliar para testar se o nó adjacente está conectado ao atual por ramo em estado fechado (ligado ou energizado)
 *
 * Essa função testa se o nó adjacente @p adjancete está concetado por um ramo no grafo cujo estado é fechado (ligado ou energizado).
 * A função retorna @c BOOL .
 * 
 * @param adjacente estrutura do tipo TF_NOADJACENTE para testar se possui ramo fechado
 * @return BOOL
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL ramoLigado(TF_NOADJACENTE adjacente);

/**
 * @brief Função auxiliar para testar se dois nós estão conexos dentro da topologia do grafo
 *
 * Essa função testa se o nó especificados pelos identificadores @p idNoRaiz e o nó @p idNoAdj estão conectados dentro do grafo armazenado em @p grafo.
 * A função retorna @c BOOL .
 * 
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param idNoRaiz primeiro nó a ser buscada conectividade
 * @param idNoAdj segundo nó a ser buscada conectividade
 * @return BOOL
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL estaListaAdjacencias(TF_GRAFO *grafo, long int idNoRaiz, long int idNoAdj);

//------------------------------------------------------------------------------
//
// ROTINAS DE MONTAGEM DAS ESTRUTURAS DE DADOS DA TOPOLOGIA DA REDE
//
//------------------------------------------------------------------------------

/**
 * @brief Função recursiva para realizar busca em profundidade no grafo da rede elétrica
 *
 * Essa função realiza recursivamente uma busca em profundidade no grafo da rede elétrica @p grafo, atualizando a profundidade @p profundidade 
 * de cada nó @p idNo indicado no ponteiro de barras @p barraAtual para percorrer o grafo da rede conforme a conectivivdade da rede elétrica. Utiliza
 * um vetor auxiliar para controlar o nós já visitados na busca @p visitado e também atualiza o identificador de  alimentador @p idAlim de cada nó 
 * e a quantidade total de nós por alimentador @p nos_alim .
 * A função retorna @c void .
 * 
 * @param barraAtual ponteiro para a lista encaddeada com o nó atual da recursão
 * @param idNo identificador do nó atual da recursão
 * @param profundidade profundidade atual da recursão
 * @param visitado vetor auxiliar para controlar os nós já visitados pela busca
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param idAlim identificador do alimentador onde está sendo realizada a busca em profundidade
 * @param nos_alim quantidade total de nós do alimentaodor atualizada a cada recursão
 * @return void
 * @see buscaProfundidadeAlimentadores
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void buscaProfundidade(TF_FILABARRAS *barraAtual, long int idNo, int profundidade,  BOOL *visitado, TF_GRAFO * grafo, long int idAlim, long int *nos_alim);

/**
 * @brief Função para realizar busca em largura no grafo da rede elétrica
 *
 * Essa função realiza uma busca em largura no grafo da rede elétrica @p grafo para determinado alimentador especifico @p idAlim da estrutura 
 * @p alimentador atualizando a profundidade de cada nó para percorrer o grafo  da rede conforme a conectivivdade da rede elétrica. Utiliza 
 * um vetor auxiliar para controlar o nós já visitados na busca @p visitado e parte do nó inicial de cada alimentador defindo por @p idNoRaiz.
 * A função retorna @c void .
 * 
 * 
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param alimentador estrutura de dados dos alimentadores
 * @param idAlim identificador do alimentador onde está sendo realizada a busca em largura
 * @param idNoRaiz nó inicial para iniciar a busca em largura
 * @param visitado vetor auxiliar para controlar os nós já visitados pela busca
 * @return void
 * @see 
 * @note Implementada para testes e validações, mas o código utiliza a busca em profundidade para obter a representação via RNP.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void buscaLargura(TF_GRAFO * grafo, TF_ALIMENTADOR *alimentador, long int idAlim, long int idNoRaiz, BOOL * visitado);


/**
 * @brief Função principal para realizar busca em profundidade nos alimentadores da rede elétrica
 *
 * Essa função realiza uma busca em profundidade em todos os alimentadores @p alimentadores  no grafo da rede elétrica @p grafo, através da função 
 * recursiva buscaProfundidade(). Receb como parâmetros a quantidade total de barras @p numeroBarras e de alimentadores na rede elétrica @p numeroAlimentadores .
 * A função retorna @c void .
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * char *folder = NULL;
 * DBAR *barraExemplo = NULL;
 * TF_DRAM *ramoExemplo = NULL; 
 * TF_GRAFO *grafoExemplo = NULL;
 * TF_ALIMENTADOR *alimentadorExemplo = NULL;
 * 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * geraGrafo(&grafoExemplo, barraExemplo, numeroBarras,ramoExemplo,numeroRamos);
 * buscaProfundidadeAlimentadores(grafoExemplo, numeroBarras, &alimentadorExemplo, numeroAlimentadores); 
 * @endcode
 * 
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param numeroBarras quantidade total de barras
 * @param alimentador estrutura de dados dos alimentadores
 * @param numeroAlimentadores quantidade total de alimentadores
 * @return void
 * @see buscaProfundidade
 * @note 
 * @warning 
 */
void buscaProfundidadeAlimentadores(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR **alimentadores, long int numeroAlimentadores);

/**
 * @brief Função principal que cria estrutura de dados para representar o grafo da rede elétrica e armazenar as condições topológicas e operacionais 
 * da rede elétrica. 
 * 
 * Esta função aloca e cria a estrutura de dados para representar a rede elétrica e respectivas informações estruturais e paramétricas dos componentes
 * da rede elerica. Recebe como parâmetros de entrada as barras @p barras com informações do nós, cargas, bancos de capacitores e geradores 
 * distribuídos que compõem os nós do grafo da rede elétrica, e os ramos @p ramos com informações de circuitos, transformadores, reguladores de 
 * tensão e chaves que compõem os ramos do grafo da rede eletrica, definindo assim a conectividade e adjacência do grafo. Recebe como parâmetros 
 * de entrada e saída a estrutura de dados do tipo TF_GRAFO @p grafo com todas as informações a respeito da rede elétrica a aloca os nós e adjacências 
 * do grafo com estrutura interna do tipo TF_NOADJACENTE que apontam para as barras e ramos inicialmente definidos.
 * A função retorna @c void .
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * char *folder = NULL;
 * DBAR *barraExemplo = NULL;
 * TF_DRAM *ramoExemplo = NULL; 
 * TF_GRAFO *grafoExemplo = NULL;
 * 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * geraGrafo(&grafoExemplo, barraExemplo, numeroBarras,ramoExemplo,numeroRamos); 
 * @endcode
 *  
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param barras estrutura de dados com os dados das barras da rede elétrica
 * @param numeroBarras quantidade total de barras
 * @param ramos estrutura de dados com os dados dos ramos da rede elétrica
 * @param numeroRamos quantidade total de ramos
 * @return void
 * @see 
 * @note 
 * @warning
 */
void geraGrafo(TF_GRAFO ** grafo, TF_DBAR *barras, long int numeroBarras,TF_DRAM *ramos,long int numeroRamos);

/**
 * @brief Função auxiliar para salvar topologia de alimentador obtida pela busca em profundidade em arquivo externo
 *
 * Essa função salva um arquivo de texto indicado em @p arquivo contendo as principais da topologia de um alimentador específico obtida 
 * pela busca em profundidade. Recebe como parâmetro de entrada o o grafo da rede elétrica @p grafo e número total de barras @p numeroBarras 
 * de um determinado alimentador @p idAlim da estrutura de dados de alimentadores @p alimentador.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para arquivo que será impressa a topologia do alimentador
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param numeroBarras quantidade total de barras
 * @param alimentador estrutura de dados dos alimentadores
 * @param idAlim identificador do alimentador cuja topologia será impressa no arquivo
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeAlimentador(FILE *arquivo,TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int idAlim);

//------------------------------------------------------------------------------
//
// ROTINAS DE TRATAMENTO DOS DADOS ELÉTRICOS
//
//------------------------------------------------------------------------------

/**
 * @brief Função auxiliar para montar o quadripólo de admitância trifásico de circuitos (ramais ou linhas de distribuição)
 *
 * Essa função monta o quadripólo de admitâncias trifásicas matricial para ramos da rede elétrica cujos componentes são circuitos trifásicos.
 * Esta função recebe como parâmetro de entrada e saída uma estrutura do tipo TF_DRAM @p ramo que armazena as informações matriciais do quadripólo de
 * admitâncias de um ramo respectivo, e como parâmetro de entrada os dados detalhados do circuito trifásico @p linha com o parâmetros deste componente.
 * A função retorna @c void .
 * 
 * @param *ramo ponteiro o ramo que será montado o quadripólo
 * @param *linha ponteiro para as informações detalhadas do circuito trifásico (ramais ou linhas de distribuição)
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void montaQuadripoloLinha(TF_DRAM *ramo, TF_DLIN *linha);

/**
 * @brief Função auxiliar para montar o quadripólo de admitância trifásico de transformadores de potência
 * 
 * Essa função monta o quadripólo de admitâncias trifásicas matricial para ramos da rede elétrica cujos componentes são transformadores de potência.
 * Esta função recebe como parâmetro de entrada e saída uma estrutura do tipo TF_DRAM @p ramo que armazena as informações matriciais do quadripólo de
 * admitâncias de um ramo respectivo, e como parâmetro de entrada os dados detalhados do transformador @p trafo com o parâmetros deste componente.
 * A função retorna @c void .
 * 
 * @param *ramo ponteiro o ramo que será montado o quadripólo
 * @param *trafo ponteiro para as informações detalhadas do transformador de potência
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void montaQuadripoloTrafo(TF_DRAM *ramo, TF_DTRF *trafo);

/**
 * @brief Função auxiliar para montar o quadripólo de admitância trifásico de reguladores de tensão
 * 
 * Essa função monta o quadripólo de admitâncias trifásicas matricial para ramos da rede elétrica cujos componentes são reguladores de tensão.
 * Esta função recebe como parâmetro de entrada e saída uma estrutura do tipo TF_DRAM @p ramo que armazena as informações matriciais do quadripólo de
 * admitâncias de um ramo respectivo, e como parâmetro de entrada os dados detalhados do reguladores @p reg com o parâmetros deste componente.
 * A função retorna @c void .
 * 
 * @param *ramo ponteiro o ramo que será montado o quadripólo
 * @param *reg ponteiro para as informações detalhadas do regulador de tensão
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void montaQuadripoloRegulador(TF_DRAM *ramo, TF_DREG *reg);

/**
 * @brief Função auxiliar para montar o quadripólo de admitância trifásico de bancos de capacitores
 * 
 * Essa função monta o quadripólo de admitâncias trifásicas matricial para ramos da rede elétrica cujos componentes são transformadores de potência.
 * Esta função recebe como parâmetro de entrada e saída uma estrutura do tipo TF_GRAFO @p no que armazena as informações matriciais do quadripólo de
 * admitâncias de um nó respectivo, e como parâmetro de entrada os dados detalhados do banco de capacitor @p shunt com o parâmetros deste componente.
 * A função retorna @c void .
 * 
 * @param *no ponteiro o para o nó que será montado o quadripólo
 * @param *shunt ponteiro para as informações detalhadas do banco de capacitor
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void montaQuadripoloShunt(TF_GRAFO *no, TF_DSHNT *shunt);

/**
 * @brief Função principal que realiza a transformação das grandezas elétricas em pu (por unidade)
 * 
 * Esta função aloca transformas os dados e informações da rede elétrica das unidades de medida convencionais (S.I.) em pu (por unidade) para 
 * realizar os cálculos elétricos da condição operacional da rede elétrica. Além disto monta as matrizes de quadripólos para representação 
 * das relações elétricas no componentes da re de elétrica. Recebe como parâmetro de entrada e saída o grafo da rede elétrica em @p grafo e como parâmetros 
 * de entrada os ramos @p ramos e quantidades totais de barras e ramos da rede @p numeroBarras e @p numeroRamos . Para realizar a transformação em pu
 * as tensões base são definidas de acordo com os dados lidos nos arquivos de entrada (em especial DBAR.csv) e a potência base da rede elétrica é
 * definida como parâmetro de entrada em @p Sbase.
 * A função retorna @c void .
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * double Sbase = 10000;
 * char *folder = NULL;
 * DBAR *barraExemplo = NULL;
 * TF_DRAM *ramoExemplo = NULL; 
 * TF_GRAFO *grafoExemplo = NULL;
 * 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * geraGrafo(&grafoExemplo, barraExemplo, numeroBarras,ramoExemplo,numeroRamos); 
 * calculaPU(grafo, numeroBarras, ramo, numeroRamos, Sbase);
 * @endcode
 *  
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param numeroBarras quantidade total de barras
 * @param ramos estrutura de dados com os dados dos ramos da rede elétrica
 * @param numeroRamos quantidade total de ramos
 * @param Sbase potência base da rede elétrica
 * @return void
 * @see montaQuadripoloLinha
 * @see montaQuadripoloTrafo
 * @see montaQuadripoloRegulador
 * @see montaQuadripoloShunt
 * @note 
 * @warning
 */
void calculaPU(TF_GRAFO *grafo, long int numeroBarras, TF_DRAM *ramos, long int numeroRamos, double Sbase);

/**
 * @brief Função principal que realiza a atualização dos valores de taps nas matrizes do quadripólo de admitâncias trifásico de reguladores e transformadores
 * 
 * Esta função atualiza as matrizes do quadripólo de admitâncias dos ramos da rede elétrica @p ramos de acordo com os valores dos taps indicados 
 * dentro da estrutura de dados de reguladores de tensão e transformadores de potência. Pode ser utilizada para atualizar os taps conforme
 * novas informações discretas são obtidas pelo sistema supervisório. Recebe como parâmetro de entrada também a quantidade total de ramos.
 * A função retorna @c void .
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * double Sbase = 10000;
 * char *folder = NULL;
 * DBAR *barraExemplo = NULL;
 * TF_DRAM *ramoExemplo = NULL; 
 * TF_GRAFO *grafoExemplo = NULL;
 * 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * geraGrafo(&grafoExemplo, barraExemplo, numeroBarras,ramoExemplo,numeroRamos); 
 * calculaPU(grafo, numeroBarras, ramo, numeroRamos, Sbase);
 * atualizaTaps(ramo, numeroRamos);
 * @endcode
 *  
 * @param ramos estrutura de dados com os dados dos ramos da rede elétrica
 * @param numeroRamos quantidade total de ramos
 * @return void
 * @see montaQuadripoloTrafo
 * @see montaQuadripoloRegulador
 * @note 
 * @warning
 */
void atualizaTaps(TF_DRAM *ramos, long int numeroRamos);




void atualizaEstadoChaves(const char *folder, const char *filename, TF_GRAFO *grafo, long int numeroBarras);

#endif	/* FUNCOESTOPOLOGIA_H */

