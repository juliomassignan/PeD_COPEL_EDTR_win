/**
 * @file data_structures.h
 * @author Telma Woerle de Lima soares
 * @date 10 Sep 2013
 * @brief Este arquivo contém a definição das estruturas de dados utilizadas no projeto smartgrids para
 * reconfiguração/restabelecimento de sistemas de distribuição de energia elétrica radiais (SDR).
 * Os tipos que estão sendo definidos para a aplicação tem como padrão o nome com todas as letras em caixa alta.
 * Para compor o nome das variáveis com nome composto o padrão utizado é o primeiro "nome" em minúsculo e os demais com a primeira letra em maiúsculo e as demais em minúsculo.
 * @see http://lcrserver.icmc.usp.br/projects/smartgrid-copel/wiki
 * @see http://lcrserver.icmc.usp.br/projects/codes/wiki
 */

#ifndef DATA_STRUCTURES_H_INCLUDED
#define DATA_STRUCTURES_H_INCLUDED



/**
 Define versão simplicada para o comando malloc da linguagem c
 */
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
/**
 Define a constante para o valor máximo do tipo inteiro
 */
#define MAXINT    100007
/**
 Define a constante para o valor máximo do tipo float
 */
#define MAXFLOAT  100007.999999

#define fatorPonderacaoManobras  10

#define passoRegulador 0.00625

#ifdef CLOCKS_PER_SEC
 static const double CLOCK_TICKS_PER_SECOND = (double)CLOCKS_PER_SEC;
 #elif defined(CLK_TCK)
 static const double CLOCK_TICKS_PER_SECOND = (double)CLK_TCK;
 #else
 static const double CLOCK_TICKS_PER_SECOND = 1000000.0;
 #endif


/**
 * @brief A enumeração BOOLEAN é utilizada para definir o tipo BOOL.
 *
 */
enum BOOLEAN {
	true = 1, /**< Valor verdadeiro para os testes booleanos. */
	false = 0 /**< Valor falso para os testes booleanos. */
};
/**
 * @brief O tipo bool simula o uso de variáveis booleanas presente na linguagem C++.
 *
 * As variáveis deste tipo serão utilizadas nos testes lógicos.
 * Além disso, este tipo será utilizado para as comparações condicionais.
 */
typedef enum BOOLEAN BOOL;



/**
 * @brief A enumeração TIPONO define a classificação dos nós que representam barras do SDR.
 *
 * Variáveis do TIPONO podem possuir 4 classificações.
 * Essa classificações são sem carga, com carga, capacitor e raiz de alimentador.
 * Os dados de entrada contém um valor inteiro especificado para cada uma das classificações com valores inteiros de 0 a 3
 */
typedef enum  {
	semCarga, /**< Indica as barras que não possuem carga. */
	comCarga, /**< Indica as barras com carga. */
	capacitor, /**< Indica as barras que representam capacitores. */
	raizAlimentador /**< Indica que a barra é raiz de alimentador. */
} TIPONO;

/**
 * @brief A enumeração MEDIDOR define a presença de medidores nos ramos do SD sendo incluida nas informações dos NOSADJACANETES.
 * 
 */
typedef enum  {
        semMedidor, /**< Valor utilizado para as arestas que não possuem medidores. */
        medidorRamo, /**< Indica que o ramo possui um medidor. */
        medidorClasseA /**< Indica que a barra possui um medidor. */
} MEDIDOR;

/**
 * @brief A enumeração TIPOARESTA define a classificação das aresta do SDR.
 *
 * Variáveis do TIPOARESTA podem possuir 4 classificações.
 * Essa classificações são chave manual, chave automática, trecho e regulador de Tensão.
 * Os dados de entrada contém um valor inteiro especificado para cada uma das classificações com valores inteiros de 0 a 3
 */
typedef enum  {
	chaveManual, /**< Indica que o tipo da chave é manual. */
	chaveAutomatica, /**< Indica que o tipo da chave é automática. */
	trecho, /**< Indica que a aresta é um trecho. */
	reguladorTensao /**< Indica que a aresta é um regulador de tensão. */
} TIPOARESTA;

/**
 * @brief A enumeração ESTADOCHAVE define qual o estado da chave no inicio da execução do programa no SDR.
 * Variáveis do tipo ESTADOCHAVE podem possuir 3 valores.
 * Os valores possíveis são normalmente Aberta, normalmente Fechada e outros tipos de aresta. Para as arestas que não representam chaves é utilizado o terceiro valor.
  * Os dados de entrada contém um valor inteiro especificado para cada uma das categorias com valores inteiros de 0 a 2
 */
typedef enum  {
	normalmenteAberta, /**< Indica que o estado da chave é normalmente aberto. */
        normalmenteFechada, /**< Indica que o estado da chave é normalmente fechado. */
        outrasArestas /**< Valor utilizado para as arestas que não representam chaves. */
} ESTADOCHAVE;

/**
 * @brief A enumeração CONDICAO define a condição de operação das arestas do SDR.
 *
 * Variáveis do tipo CONDICAO podem possuir 2 valores.
 * Os valores possíveis são em operação e com restrição. Para as arestas do tipo trecho a condição possui valor em operação em todas as situações.
 * Para os demais tipos de aresta esse valor indica a condição operacional de cada uma e pode variar a cada execução do software.
 * Os dados de entrada contém um valor inteiro especificado para cada uma das categorias com valores inteiros de 0 ou 1
 */
typedef enum  {
	emOperacao, /**< Indica que a aresta está em condição de operação, trechos sempre estão em operação. */
	comRestricao /**< Indica que a aresta possui alguma restrição em sua operação. */
} CONDICAO;


/**
 * @brief A enumeração SUBTIPOARESTA define a subdivisão da classificação das aresta do SDR.
 *
 * Variáveis do SUBTIPOARESTA podem possuir 5 categorias.
 * As categorias são chave fusível, chave seca, chave operável com carga, chave operável com curto circuíto e outros tipos de aresta.
 * Os dados de entrada contém um valor inteiro especificado para cada uma das categorias com valores inteiros de 0 a 4
 */
typedef enum  {
	chaveFusivel, /**< Indica que a chave é do tipo fusível. */
	chaveSeca, /**< Indica que a chave é do tipo seca. */
	chaveCarga, /**< Indica que a chave opera com carga. */
	chaveCurtoCircuito, /**< Indica que a chave opera em curti circuito. */
	outrosSubTipo /**< Utilizada para as arestas que não são chaves. */
} SUBTIPOARESTA;

/**
 * @brief A enumeração TIPOREGULADOR especifica se o regulador de tensão permite ou não fluxo reverso.
 *
 * Variáveis do TIPOREGULADOR podem ter 2 valores.
 * A categoria comFluxoReverso indica que o regulador permite a operação com fluxo reverso e a
 * categoris SemFluxoReverso indica que o regulador não permite a operação com fluxo reverso.
 * Os dados de entrada contém um valor inteiro especificado para cada uma das categorias com valores inteiros de 0 e 1
 */
typedef enum  {
	comFluxoReverso, /**< Indica que o regulador de tensão opera com fluxo reverso. */
	semFluxoReverso /**< Indica que o regulador de tensão não opera com fluxo reverso. */
} TIPOREGULADOR;



/**
 * @brief Armazena as informações de cada uma das arestas do SDR.
 *
 * Esta estrutura é utlizada para armazenar as informações de cada uma das arestas, na representação por grafos
 * do sistema de distribuição. Uma aresta do SDR pode ser uma chave, manual ou automática, um trecho ou um regulador de tensão.
 * Cada posição da lista de adjacentes de um nó do grafo será especificada pelo tipo criado a partir dessa estrutura.
 */
typedef struct {
  long int idNo;/**< Inteiro que identifica cada nó do SDR. */
  char idAresta[15]; /**< Número operacional das chaves e reguladores de tensão. No caso de trechos é um número sequencial */
  ESTADOCHAVE estadoChave; /**< Estado da chave no momento inicial do processo, aberta ou fechada. */ 
  MEDIDOR medicao; /**< Possui medida no ramo. */
  long int idAM; /**< Inteiro que indica o índice da AM caso tenha medidor. */
  TIPOARESTA tipoAresta; /**< Tipo da aresta no SDR. */
  CONDICAO condicao; /**< Condição operacional da aresta. */
  SUBTIPOARESTA subTipoAresta; /**< Subtipo da aresta no SDR. */
  double resistencia; /**< Valor do tipo double indicando a resistência da aresta. */
  double reatancia; /**< Valor do tipo double indicando a reatância da aresta. */
  double ampacidade; /**< Valor do tipo double indicando a ampacidade da aresta. */
} NOADJACENTE;

/**
 * @brief Armazena os valores de P e Q de cada hora do dia.
 *
 * Esta estrutura é utilizada para armazenar os valores de P e Q da barra para cada hora do dia.
 * Nos casos onde a barra for sem carga esses campos possuem valor 0.
 */


typedef struct {
  double p; /**< Valor do tipo double indicando o valor P da barra. */
  double q; /**< Valor do tipo double indicando o valor Q da barra. */
} PQ;


/**
 * @brief Armazena os dados necessários para calcular a priorização.
 *
 * Esta estrutura contém os valores que serão utilizados para calcular a priorização de religação dos setores.
 *
 */


typedef struct {
  double eusdGrupoA; /**< Valor de EUSD do grupo A. */
  double eusdGrupoB; /**< Valor de EUSD do grupo B. */
  int qtdConsumidores; /**< Quantidade de consumidores. */
  int qtdConsumidoresEspeciais; /**< Quantidade de consumidores especiais. */
} PRIORIZACAO;


/**
 * @brief Armazena os dados necessários para calcular as compensações.
 *
 * Esta estrutura contém os valores que serão utilizados para calcular as compensações
 * pagas na forma de multas para a Aneel com base nos indicadores estabelecidos.
 * Valores necessários para o cálculo da compensação esperada no trecho a ser priorizado.
 * Os valores de DIC e DMIC são compensações diretas do trecho desligado e são proporcionais ao tempo esperado para inspeção e recomposição.
 * Já o valor do FIC foi inserido para possibilitar o cálculo das possíveis compensações que ocorrer�o caso se tome a decisão de desligar trechos "sãos" visando o corte de carga para
 * restabelecimento de outros trechos do sistema.
 */


typedef struct {
  double metaDicMensal; /**< Meta de DIC mensal. */
  double metaDicTrimestral; /**< Meta de DIC trimestral. */
  double metaDicAnual; /**< Meta de DIC anual. */
  double metaDmicMensal; /**< Meta de DMIC mensal. */
  double metaFicMensal; /**< Meta de FIC mensal. */
  double metaFicTrimestral; /**< Meta de FIC trimestral. */
  double metaFicAnual; /**< Meta de FIC anual. */
  double resulDicAcumAno; /**< Resultado de DIC acumulado no Ano. */
  double resulDicAcumTrimestre; /**< Resultado de DIC acumulado no Trimestre. */
  double resulDicAcumMes; /**< Resultado de DIC acumulado no mês. */
  double resulFicAcumAno; /**< Resultado de FIC acumulado no Ano. */
  double resulFicAcumTrimestre; /**< Resultado de FIC acumulado no trimestre. */
  double resulFicAcumMes; /**< Resultado de FIC acumulado no mês. */
  double resulDmicMes; /**< Resultado de DMIC acumulado no mês. */

} COMPENSACAO;

/**
 * @brief Armazena as informações de cada um dos nós/barras do SDR.
 *
 * Esta estrutura é utilizada para armazenar as informações de cada um dos nós, na representação por grafos
 * do sistema de distribuição. Cada nó do grafo refere-se a uma barra do SDR. Os nós podem ser barras sem carga ou com carga, capacitor ou raiz de alimentador.
 * Cada posição da lista de adjacentes de um nó do grafo será especificada pelo tipo criado a partir dessa estrutura.
 */

typedef struct {
  long int idNo; /**< Valor inteiro sequencial atribuído a cada nó/barra do SDR. */
  TIPONO tipoNo; /**< Classificação do tipo de cada nó do SDR. */
  MEDIDOR medicao; /**< Possui medida na carga (Consumidor Classse A). */
  long int idAM; /**< Inteiro que indica o índice da AM caso tenha medidor de carga. */
  int numeroAdjacentes; /**< Indica o tamanho da lista de adjacentes do nó. */
  NOADJACENTE *adjacentes; /**< Lista dos nós adjacentes. */
  PQ valorPQ; /**< Array com 24 posições, uma para cada hora do dia contendo os valores de P e Q. */
  PRIORIZACAO priorizacoes; /**< Contém os valores para cálculo das priorizações. */
  COMPENSACAO compensacoes; /**< Contém os valores para verificação de compensações do trecho. */
  long int idSetor; /**< Identificador do setor, será utilizado no próximo de identificação dos setores. */
} GRAFO;


/**
 * @brief Armazena as informações de cada trafo que compõem o SDR.
 *
 * Esta estrutura é utilizada para armazenar as informações dos trafos do SDR.
 * Esses dados são utilizados no cálculo das funções objetivo do problema e para validação de factibilidade da solução
 * com base nas restrições operacionais.
 */

typedef struct {
  int idTrafo; /**< Valor inteiro sequencial atribuído a cada trafo do SDR. */
  int idSubEstacao; /**< Valor inteiro sequencial atribuído a cada subestação do SDR. */
  double capacidade; /**< Indica a capacidade de potência em MVA do trafo. */
  double tensaoReal; /**< Indica a tensão suportada pelo trafo em Kv. */
} DADOSTRAFO;

/**
 * @brief Armazena as informações de cada alimentador que compõem o SDR.
 *
 * Esta estrutura é utilizada para armazenar a relação dos alimentadores com os trafos do SDR.
 * Esses dados são utilizados no cálculo das funções objetivo do problema e para validação de factibilidade da solução
 * com base nas restrições operacionais.
 */

typedef struct {
  char idAlimentador[15]; /**< Código operacional do alimentador no SDR. */
  long int barraAlimentador; /**< Valor inteiro da barra do alimentador, idNo que corresponde a este. */
  int idTrafo; /**< Identificador do trafo ao qual o alimentador estão ligado.*/
  int numeroSetores; /**< Armazena quantos setores compoem o alimentador. Será utilizado para definir o tamanho da RNP*/
} DADOSALIMENTADOR;

/**
 * @brief Armazena as informações dos reguladores de tensão presentes no SDR.
 *
 * Esta estrutura é utilizada para armazenar os dados de operação dos reguladores de tensão presentes no SDR.
 * Esses dados são utilizados no cálculo das funções objetivo do problema e para validação de factibilidade da solução
 * com base nas restrições operacionais.
 */

typedef struct {
  char idRegulador[30]; /**< Numero Operacional do regulador de tensao. */
  TIPOREGULADOR tipoRegulador; /**< Caracteriza se o regulador de tensão permite ou não fluxo reverso. */
  double ampacidade; /**< Ampacidade suportada pelo regulador de tensão.*/
  int numeroTaps; /**< Porcentagem de correção realizada pelo regulador.*/
} DADOSREGULADOR;

//*Estruturas para a identificação de setores*/

typedef struct NoLista {
    long int idNo;
    struct NoLista * prox;
} BARRASETOR;

typedef struct ListaSetores{ 
    BARRASETOR * barraSetor;
    BARRASETOR * barraSetorAdj;
    long int consumidoresEspeciais;
    long int consumidoresNormais;
    long int idSetor;
    int numeroAdjacentes;
    int numeroNos;
    struct ListaSetores * prox;
} LISTASETORES;

typedef struct Fila {
    long int idNo;
    struct Fila * prox;
} FILABARRAS;

/*Estruturas da RNP*/

/**
 * @brief Armazena as informações que compõem as colunas da matriz PI.
 *
 * Esta estrutura é utilizada para armazenar os dados necessários para rapidamente identificar onde
 * um nó se encontra no indivíduo.
 */
typedef struct 
{
  long int idConfiguracao; /**< Inteiro que identifica o índividuo onde o ocorreu modificação no nó. */
  int idRNP; /**< Inteiro que identifica o RNP que contém o nó. */
  int posicao; /**< Inteiro que identifica a posição do nó na RNP. */
}COLUNAPI;


/**
 * @brief Represenra a matriz PI da RNP.
 *
 * Esta estrutura é utilizada para armazenar os dados de cada nó para rapidamente identifica-lo no momento
 * da aplicação dos operadores da RNP.
 */

typedef struct
{
  long int idNo; /**< Identificador do nó. */
  int maximoColunas;
  int numeroColunas; /**< Número de colunas que estão ocupadas da matriz. */
  COLUNAPI **colunas; /**< Colunas de matriz PI. Cada coluna é alocada a medida que precisa ser utilizada. */
} MATRIZPI;


/**
 * @brief Armazena os nós utilizadas para obter um indivíduo.
 *
 * Esta estrutura contém os nós utilizadas para produzir um novo indivíduo à partir 
 * de outro indivíduo por meio dos operadores PAO ou CAO. Essa estrutura é um campo do vetor Pi da RNP. 
 * A informação contida nessa estrutura é utilizada pelo operador de Evolução Diferencial ou pelo operador de 
 * recombinação EHR.
 */

typedef struct 
{ 
	long int p; /**< Valor do nó de poda p utilizado por PAO ou CAO.*/ 
	long int a; /**< Valor do nó de enxerto a utilizado por PAO ou CAO.*/
	long int r; /**< Valor do novo nó raiz r utilizado por CAO.*/
        int iadj;  /**< Indice do nó a na lista de adjacências de p ou r.*/
} NOSPAOCAO;

/**
 * @brief Armazena o ancestral do indivíduo.
 *
 * Esta estrutura armazena o id o indivíduo que foi utilizado para construir a nova solução por meio dos operadores
 * PAO e CAO. Além disso, armazena os nós (p, r, a) que foram escolhidos para obter essa solução.
 */
typedef struct 
{ 
	long int idAncestral; /**< Indice do indivíduo ancestral.*/
        long int *idChaveAberta;
        long int *idChaveFechada;
	int numeroManobras; /**< Número de vezes que PAO e CAO foram aplicados para gerar o indivíduo, normalmente esse valor é 1.*/
	//NOSPAOCAO *nos; /**< Conjuntos de nós utilizados.*/ 
} VETORPI;

/**
 * @brief Armazena os dados de cada nó na RNP.
 *
 * Esta estrutura é utilizada para armazenar nó e a sua profundidade para compor o vetor da RNP
 */
typedef struct
{
 long int idNo; /**< Identificador do nó.*/
 int profundidade; /**< Profundidade do nó em relação a raiz.*/
} NORNP;

/**
 * @brief Estrutura dos objetivos.
 *
 * Estrutura para armazenar o valor de cada objetivo do indivíduo. É um campo do indivíduo.
 */


typedef struct{
  double carregamentoRede; /**<  .*/
  double maiorCarregamentoCorrente; /**<  .*/
  double perdasResistivas; /**< .*/
  __complex__ double potenciaAlimentador; /**< .*/
  double menorTensao; /**< .*/
  long int piorBarra;
}OBJETIVOSRNP;

/**
 * @brief Vetor da RNP.
 *
 * Esta estrutura é utilizada para armazenar uma árvore codificada na RNP.
 */
 typedef struct
 {
  int numeroNos; /**< Quantidade de nós da árvore.*/
  NORNP *nos; /**< Vetor com os nós e as profundidades.*/
  OBJETIVOSRNP fitnessRNP; /**< Opcional para armazenar alguma informação de fitness da árvore.*/
}RNP;


/*Estruturas do AE e do Problema*/

/**
 * @brief Estrutura dos objetivos.
 *
 * Estrutura para armazenar o valor de cada objetivo do indivíduo. É um campo do indivíduo.
 */
typedef struct{
  double maiorCarregamentoCorrente; /**<  .*/
  double perdasResistivas; /**< .*/
  double maiorCarregamentoTrafo; /**< .*/
  double maiorCarregamentoRede;
  double menorTensao; /**< .*/
  int    manobrasManuais; /**< Número de manobras manuais para obter a configuração.*/
  int    manobrasAutomaticas; /**< Número de manobras automaticas para obter a configuração.*/
  double quedaMaxima;
  __complex__ double *potenciaTrafo; /**< Armazena o carregamento de cada um dos trafos no SDR.*/
  double ponderacao;
  int rank;
  int fronteira;
  long int consumidoresSemFornecimento;
  long int consumidoresEspeciaisSemFornecimento;
}OBJETIVOS;

/**
 * @brief Estrutura dos dados elétricos da configuração.
 *
 * Estrutura para armazenar os dados elétricos da configuração que serão utilizados para calcular o fluxo de carga. É um campo do indivíduo.
 */
typedef struct{
  __complex__ double *corrente; /**<Armazena os valores de corrente em cada trecho do SDR.*/
  __complex__ double *iJusante;/**<Armazena os valores de corrente jusante do SDR.*/
  __complex__ double *vBarra;/**<Armazena os valores cálculados de potência das barras.*/
  __complex__ double *potencia;/**<Armazena os valores de potência.*/
}DADOSELETRICOS;

/**
 * @brief Estrutura do indivíduo.
 *
 * Esta estrutura contém os campos que representam um indivíduo no algoritmo evolutivo.
 * É utilizada para compor o vetor de soluções que representam uma população
 */

typedef struct{
 long int idConfiguracao; /**< identificador do indivíduo.*/
 int numeroRNP; /**< Quantidade de RNPs (árvores) do indivíduo.*/
 DADOSELETRICOS dadosEletricos; /**< Armazena os dados elétricos utilizados no cálculo do fluxo de carga.*/
 OBJETIVOS objetivo; /**< Valores do indivíduo para os objetivos.*/
 RNP *rnp; /**< Vetor de ponteiros para RNPs que compõem a solução.*/
}CONFIGURACAO;


/**
 * @brief Adaptação da Estrutura da RNP para armazenar a RNP de setor.
 *
 * Esta estrutura é utilizada para armazenar a RNP de um setor dada a origem do fluxo de energia.
 */
typedef struct
 {
  int numeroNos; /**< Número de nós do setor.*/
  NORNP *nos; /**< Vetor de RNP do setor.*/
  long int idSetorOrigem; /**< Identificador do setor raiz.*/
}RNPSETOR;

/**
 * @brief Armazena a lista de RNPs de cada setor do SDR.
 *
 * Esta estrutura é utilizada para armazenar a lista de RNPs de cada setor identificado do SDR.
 */

typedef struct{
 long int idSetor; /**< Identificador setors.*/
 int numeroRNPs; /**< Número de setores adjacentes.*/
 RNPSETOR *rnps; /**< Vetor de RNPs.*/
}RNPSETORES;


/**
 * @brief Armazena o grafo de setores (lista de adjacências de setores).
 *
 * Esta estrutura é utilizada para armazenar a relação de adjacências dos setores do SDR
 * na forma de grafo. Essa lista é consultada pelos operadores da RNP na escolha dos nós.
 */

typedef struct{
 long int idSetor;/**< Inteiro que identifica o setor. */
 long int numeroConsumidores;
 long int numeroConsumidoresEspeciais;
 int numeroAdjacentes;/**< Número de setores adjacentes. */
 long int *setoresAdjacentes; /**< Lista de setores adjacentes. */
 long int *idChavesAdjacentes; /**< Identificador da chave que liga os setores adjacentes na lista de chaves. */
 BOOL setorFalta;
}GRAFOSETORES;


/**
 * @brief Armazena os valores de ampacidade para cada par de barra.
 *
 * Esta estrutura é utilizada para armazenar a célula de valores de máximo de corrente para cálculo do fluxo de carga.
 */

typedef struct{
 long int idNo; /**< Inteiro que identifica cada nó destino no SDR. */
 double valor; /**< Valor double de máximo de corrente. */
}CELULACORRENTE;

/**
 * @brief Armazena o máximo de corrente das linhas.
 *
 * Esta estrutura é utilizada para armazenar a matriz de valores máximos de corrente para cálculo do fluxo de carga.
 */

typedef struct{
 long int idNo; /**< Inteiro que identifica cada o nó origem. */
 int numeroAdjacentes; /**< Inteiro que identifica a quantidade de nós adjacentes. */
 CELULACORRENTE *noAdjacentes; /**< Armazena os valores complexos para cada nó adjacente. */
}MATRIZMAXCORRENTE;

/**
 * @brief Armazena os valores complexos para cada para de barra.
 *
 * Esta estrutura é utilizada para armazenar a célula de valores complexos para cálculo do fluxo de carga.
 */

typedef struct{
 long int idNo; /**< Inteiro que identifica cada nó destino no SDR. */
 __complex__ double valor; /**< Valor complexo. */
}CELULACOMPLEXA;

/**
 * @brief Armazena a matriz de valores complexos.
 *
 * Esta estrutura é utilizada para armazenar a matriz de de valores complexos para cálculo do fluxo de carga.
 */

typedef struct{
 long int idNo; /**< Inteiro que identifica cada o nó origem. */
 int numeroAdjacentes; /**< Inteiro que identifica a quantidade de nós adjacentes. */
 CELULACOMPLEXA *noAdjacentes; /**< Armazena os valores complexos para cada nó adjacente. */
}MATRIZCOMPLEXA;

/**
 * @brief Armazena as informações de cada chave do SDR.
 *
 * Esta estrutura é utlizada para armazenar as informações de cada chave do SDR com a inserção do identificador interno.
 */
typedef struct {
  long int idNoDe;/**< Inteiro que identifica cada nó origem da chave no SDR. */
  long int idNoPara;/**< Inteiro que identifica cada nó destino da chave no SDR. */
  long int identificador; /**< Identificador interno único para cada chave. */
  char codOperacional[15]; /**< Número operacional das chaves. */
  TIPOARESTA tipoChave; /**< Tipo da chave no SDR. */
  ESTADOCHAVE estadoChave; /**< Estado da chave no momento inicial do processo, aberta ou fechada. */ 
  CONDICAO condicao; /**< Condição operacional da chave. */
  SUBTIPOARESTA subTipoChave; /**< Subtipo da chave no SDR. */
} LISTACHAVES;



/**
 * @brief Armazena as informações de cada chave do SDR.
 *
 * Esta estrutura é utlizada para armazenar as informações de cada chave do SDR com a inserção do identificador interno.
 */
typedef struct {
  long int *noR;/**< Array de Inteiro para armazenar os setores raiz. */
  long int *noA;/**< Array de Inteiro para armazenar os setores destino. */
  long int *noP;/**< Array de Inteiro para armazenar os setores destino. */
  int *rnpA;
  long int numeroNos; /**< Número de pares de nós possíveis para o restabelecimento. */
  
} NOSRESTABELECIMENTO;

/**
 * @brief Estrutura para criação das tabelas do algoritmo AEMT.
 *
 * Esta estrutura é utilizada para armazenar as subpopulações, chamadas de tabelas referentes a cada objetivo. As tabelas armazenam
 * o identificador do indivíduo e o valor do objetivo referente a tabela.
 */
typedef struct {
  long int idConfiguracao;
  double valorObjetivo;
  double distanciaMultidao;
} TABELA;

/**
 * @brief Estrutura para criação das tabelas do algoritmo AEMT.
 *
 * Esta estrutura é utilizada para armazenar as subpopulações, chamadas de tabelas referentes a cada objetivo. As tabelas armazenam
 * o identificador do indivíduo e o valor do objetivo referente a tabela.
 */
typedef struct {
  long int numeroIndividuos;
  long int tamanhoMaximo;
  TABELA *tabela;
} VETORTABELA;

#endif // GERADORRNP_H_INCLUDED
