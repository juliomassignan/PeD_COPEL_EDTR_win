/**
 * @file data_structures.h
 * @author Julio Augusto Druzina Massignan
 * @date 31 jul 2020
 * @brief Contém as estruturas de dados utilizadas para representação de redes elétricas trifásicas.
 *
 */
#ifndef DATA_STRUCTURES_H
#define	DATA_STRUCTURES_H

//------------------------------------------------------------------------------
//
//DefiniçÕes de constantes
//
//------------------------------------------------------------------------------
#define PI 3.14159265359    //Valor de PI utilizado na biblioteca - pode utilizar o disponível na math.h
#define tolerance 1E-6      //Tolerância numérica de cálculo
#define EPS 1E-9            //Tolerância numérica de cálculo
#define MAXIT 100           //Numero maximo de iterções nos calculos elétricos



//------------------------------------------------------------------------------
//
//Tipos de dados especiais
//
//------------------------------------------------------------------------------
/*Variável Booleana*/

/**
 * @brief Variável Booleana.
 *
 */
enum BOOLEAN {
	true = 1, /**< Valor verdadeiro para os testes booleanos. */
	false = 0 /**< Valor falso para os testes booleanos. */
};
typedef enum BOOLEAN BOOL;

/*Tipo de ramo*/
/**
 * @brief Tipo de ramo da rede elétrica
 *  
 * Define o tipo de equipamento que compõe os ramos do grafo da rede elétrica
 */
typedef enum  {
	ramal, // trecho de circuito
	trafo, // transformador de potência
    regulador, //regulador de tensão
    chave //chave
} TF_RAMO;

/*Estado do ramo*/
/**
 * @brief Estado do ramo da rede elétrica
 *  
 * Define a situação topológica do ramo no grafo (aberto ou fechado)
 */
typedef enum  {
	aberto, 
    fechado
} TF_ESTADO;

/*Tipo de ligação trifásica*/
/**
 * @brief Tipo de ligação trifásica dos equipamentos da rede elétrica
 *  
 * Define os tipos de ligações trifásicas possíveis da rede elétrica
 */
typedef enum  {
        O,
        YN, // Estrela Aterrado
        D, //Delta
        Y, // Estrela Aberta
        OY, //Estrela aberto
        OD, //Delta aberto
        MTC //Monofásico com tap central no secundário
} TF_LIGACAO;

/*Tipo de conexão das fases ligação trifásica*/
/**
 * @brief Tipo de conexão das fases ligação trifásica
 *  
 * Define as fases conectadas de acordo com a respectiva ligação
 */
typedef enum  {
        N,
        A, // Fase a
	B, // Fase b
        C, //Fase c
        AB, //Fase ab
        CA, //Fase ca
        BC, //Fase bc
        ABC //Fase abc
                
} TF_FASES;

//------------------------------------------------------------------------------
//
//Estruturas de Dados para dados de equipamentos e da rede elétrica
//
//------------------------------------------------------------------------------
/*Dados das cargas da rede elétrica*/
/**
 * @brief Dados das cargas elétricas a serem consideradas no cálculo de fluxo de potência
 *  
 * Podem ser representadas individualmente com modelos dependentes de tensão.
 */
typedef struct {
    long int ID;    //Identificação numérica da barra
    TF_FASES fases;    //Número de fase
    TF_LIGACAO lig;    //Tipo de ligação da carga
    double Vbase;   //Tensão nominal da carga
    
    double Pnom[3]; // valor de potência ativa trifásica
    double Qnom[3]; // valor de potência reativa trifásica
    double ZIP;     // modelo dependente de tensão
    
} TF_DLOAD;

// 
/*Dados das shunts de barra*/
/**
 * @brief Define dados de shunts de barra (bancos de capacitores fixos e automáticos)
 *  
 * Podem ser representados individualmente com modelos de controle dependentes de tensão.
 */
typedef struct {
    long int ID;        //Identificação numérica da barra
    TF_FASES fases;        //Número de fase
    TF_LIGACAO lig;        //Tipo de ligação do banco shunt
    double Vbase;       //Tensão nominal do banco shunt
    
    double Qnom[3];     //Potência reativa do shunt trifásico em kVAr (Negativo para capacitor)
    long int controle;  //Tipo de controle (0= sem controle / 1=Controle de tensão da barra)
    long int num;       //número de bancos shunt (para shunts chaveados automaticamente)
    double DV;          //Intevralo de tensão de controle (em pu)
    double Vset[3];     //setpoint de tensão (em pu)
} TF_DSHNT;

// 
/*Dados de geradores distribuídos*/
/**
 * @brief Define dados de geradores distribuídos
 *  
 * Podem ser representados individualmente com modelos de controle de Potência Ativa/Reativa (modo PQ) e também com controle de Tensão (modo PV)
 */
typedef struct {
    long int ID;        //Identificação numérica da barra
    TF_FASES fases;        //Número de fase
    TF_LIGACAO lig;        //Tipo de ligação do GD
    double Vbase;       //Tensão nominal do gerador distribuido
    double Snominal;    //Potência nominal
    
    double Pnom[3];     //Potência ativa do gerador trifásico em kW (Negativo para máquina fornecendo potência ativa)
    double Qnom[3];     //Potência reativa do reativa trifásico em kVAr (Negativo para máquina fornecendo potência reativa)
    long int controle;  //Controle da excitatriz do gerador (0 - Máquina como PQ constante / 1 = Máquina como PV constante / 2 = Corrente constante)
    
    double Qmin;        //Limites mínimo de geração de potência reativa
    double Qmax;        //Limites máximo de geração de potência reativa
    double Vset[3];     //Setpoint de tensão para máquina operando como PV constante
    long int controlePV;//0 = Sequência positiva / 2 = por fase
} TF_DGD;

// 
/*Dados de barras da rede elétrica*/
/**
 * @brief Define dados de barras trifásicas da rede elétrica
 *  
 * Representação individual das barras da rede elétrica na qual são associadas as cargas, GDs e shunts
 */
typedef struct {
    long int ID;        //Identificação numérica da barra
    long int i;         //Identificação sequencial da barra
    TF_FASES fases;        //Número de fase
    TF_LIGACAO ligacao;    //Tipo de ligação do GD
    double Vbase;       //Tensão nominal da barra
    long int tipo;      // Barra  0 - PQ; 1 - PV; 2 - Ref
    
    __complex__ double Vref[3];         //Tensão de referência para controles
    __complex__ double Vinicial[3];     //recebe de arquivo de entrada a tensão inicial
    
    TF_DLOAD loads[10];    //cargas conectadas à barra - limite de 10 cargas por barra para não ter que trabalhar na alocação dinâmica dentro do array de struct
    long int nloads;    //contador do número total de cargas
    TF_DSHNT *shunts;      //shunts conectadas à barra
    long int nshunts;   //contador do número total de shunts
    TF_DGD *gds;           //geradores distribuídos conectadas à barra
    long int ngds;      //contador do número total de gds
    
} TF_DBAR;

// 
/*Dados de circuitos elétricos de distribuição (linhas ou ramais)*/
/**
 * @brief Define dados de circuitos elétricos de distribuição (linhas ou ramais)
 *  
 * Representação individual de circuitos com representação trifásica das matrizes de impedância série e de susceptância shunt
 * Podem representar ramais mono, bi ou trifásicos de acordo com o definido nos elementos das matrizes trifásicas do cirucuito
 * 
 */
typedef struct {
    TF_FASES fases;                    //Número de fase
    double comprimento;             //Comprimento do circuito
    __complex__ double Zaa, Zab, Zac, Zbb, Zbc, Zcc;    //Impedância série do circuito de distribuição (parâmetros prórprios e mútuos)
    double Baa, Bab, Bac, Bbb, Bbc, Bcc;                //Suscpetância shunt do circuito de distribuição (parâmetros prórprios e mútuos)
double ampacidade;                                      //Ampacidade do cabo
  
} TF_DLIN;

// 
/*Dados de transformadores de potência*/
/**
 * @brief Define dados de trasnformadores de potência dos circuitos de distribuição
 *  
 * Representação individual de transformadores abaixadores ou elevadores nos circuitos de distribuição
 * Podem represnetar conexões mono, bi ou trifásicas, com diferentes tipos de ligação e são considerados como elementos do grafo para o cálculo de fluxo de potência
 * 
 */
typedef struct {
  TF_FASES fases;      //Número de fases
  double Vpri;      //Tensão nominal do enrolamento primário
  double Vsec;      //Tensão nominal do enrolamento secundário
  double Snominal;  //Potência nominal do transformador
  double R, X;      //Parâmetros de resistência do enrolamento e reatância de dispersão do transformador
  TF_LIGACAO lig_pri, lig_sec; //Tipo de ligação do primário e secundário (1 = YN / 2 = Delta / 3 = Y / 4 - Estrela aberto / 5 - Delta aberto / 6 - Monofásico com tap central no secundário)
  long int defasamento;     //Defasamento angular construtivo (Ex: Valor numérico na notação ANSI DYn0 ou DYn1) 
  double tap_pri, tap_sec;  //Posição e tap fora da nominal no primário e secundário (tap fixo sem comutação)


  double p_imag;            //Corrente de magnetização
  double noLoadLosses;      //Perdas sem cargas
  double r_mutua, x_mutua;           //Impedância mútua entre enrolamentos para modelo de trafo monofásico com tap central no secundário  
} TF_DTRF;

// 
/*Dados de reguladores de tensão*/
/**
 * @brief Define dados de reguladores de tensão dos circuitos de distribuição com comutação automática de taps
 *  
 * Representação individual de reguladores de tensão com representação dos controladores de comutação automática
 * Podem representar diferentes modos de controle de reguladores para 
 * 
 */
typedef struct {
  TF_FASES fases;      //Número de fases
  double Vnom;      //Tensão nominal do regulador de tensão
  double regulacao; //Capacidade total de regulação de tensão do regulador
  double Snominal;  //Capacidade nominal do regulador
  double R, X;      //Parâmetros de resistência do enrolamento e reatância de dispersão do regulador
  long int ntaps;   //Número total de taps
  TF_LIGACAO lig;      //Tipo de ligação trifásica do regulador
  
  //Parametros do controlador de comutação automática
  double TP;        //Relação de transformação do TP do regulador para mensurar a tensão de controle
  double TC;        //Relação de transformação do TP do regulador para mensurar a tensão de controle 
  double deltaV;    //Banda de tensão para determinar o intervalo de atuação ao redor do setpoint de tensão
  double R1,X1,R2,X2,R3,X3, V1,V2,V3;   //Parâmetros do Line Drop COmpensator trifaśicos para controle remoto pelo regulador
  long int controle;                    //Tipo de controle automático do regulador (0 = Somente Forward sem Restrição / 1 = Locked Forward / 2 = Locked Reverse / 3 = Bidirectional / 4 = Idle / 5 = Neutral Reverse / 6 = Cogenartion / Ver manual Siemens MJ4A)
  double tap[3];            //Valores atuais do tap calculados pelo fluxo de potência
  double tap_ini[3];        //Valores iniciais do tap (ou valores bloqueados de taps)
    //Parametros de controle reverso
  double deltaVr;           //Banda de tensão para determinar o intervalo de atuação ao redor do setpoint de tensão  no sentido reverso
  double R1r,X1r,R2r,X2r,R3r,X3r, V1r,V2r,V3r; //Parâmetros do Line Drop COmpensator trifaśicos para controle remoto pelo regulador no sentido reverso
  
} TF_DREG;

// 
/*Dados dos ramos da topologia da rede*/
/**
 * @brief Define dados dos ramos da topologia da rede (armazena as informações toploógicas e parâmetros de cada componente)
 *  
 * Representação individual dos ramos da rede elétrica, associado aos dados do respectivo componente (linha, trafo, regulador ou chave)
 * São armazenadas as informações trifásicas do modelo da rede elétrica para o cálculo de fluxo de potência (matrizes de impedância e qudripólos de admitância)
 * 
 */
typedef struct {
    long int DE;        //Identificação numérica da barra no terminal inicial do ramo
    long int PARA;      //Identificação numérica da barra no terminal final do ramo
    long int k;         //Identificação sequencial da barra no terminal inicial do ramo
    long int m;         //Identificação sequencial da barra no terminal final do ramo
    TF_FASES fases;        //Número de fases
    TF_RAMO tipo;          //Tipo de componente do ramo: 0= linha  1= trafo  2= regulador  3= chave
    TF_ESTADO estado;      //Condição topológica do ramo
    double ampacidade;  // ampacidade do ramo
    double Snominal;    // capacidade de potência nominal do ramo
    double comprimento;    // comprimento do ramo em metros
    
    //Dados detalhados do componente associado ao ramo
    TF_DLIN linha;
    TF_DTRF trafo;
    TF_DREG regulador;
   
    //Matrizes para modelo de linha
    __complex__ double **Z;     //Matriz de Impedância série para representar ramais
    __complex__ double **B;     //Matriz de Susceptância shunt para representar ramais
    
    //Matrizes de Admitância no modelo de quadripólos
    __complex__ double **Ypp;   //Submatriz de quadripólo trifásico para represnetar trafos e reguladores
    __complex__ double **Yps;   //Submatriz de quadripólo trifásico para represnetar trafos e reguladores
    __complex__ double **Ysp;   //Submatriz de quadripólo trifásico para represnetar trafos e reguladores
    __complex__ double **Yss;   //Submatriz de quadripólo trifásico para represnetar trafos e reguladores
    double tap_pri[3];          //Valores de tap do ramo
    double tap_sec[3];          //Valores de tap do ramo    
} TF_DRAM;

// 
/*Dados dos ramos dos medidores da rede elétrica*/
/**
 * @brief Define dados dos medidores da rede elétrica (provenientes de grandezas analógicas mensuradas nos sistemas supervisórios)
 *  
 * Representação individual de medidas (quantidades elétricas mensuradas na rede elétrica)
 * Utilizada para representar grandezas elétricas mensuradas individualmente por fase, para sereme utilizadas nos processos de estimação de estado e estimação de demanda
 *  
 */
typedef struct {
    long int DE;        //Identificação numérica da barra no terminal inicial do medidor (barra onde o medidor está instalado)
    long int PARA;      //Identificação numérica da barra no terminal final do medidor (barra que define o sentido de para medidas de fluxo em ramos)
    long int id;
    long int k;         //Identificação sequencial da barra no terminal inicial do medidor (barra onde o medidor está instalado)
    long int m;         //Identificação sequencial da barra no terminal final do medidor (barra que define o sentido de para medidas de fluxo em ramos)
    long int ramo;      //Identificador sequencial do ramo onde o medidor esá instalado
    TF_FASES fases;        //Identificador da fase sendo mensurada
    long int tipo;      /*Tipo de medidor:
                                        0: Fluxo de Potência Ativa - kW
                                        1: Fluxo de Potência Reativa - kVAr
                                        2: Injeção de Potência Ativa - kW
                                        3: Injeção de Potência Reativa - kVAr
                                        4: Magnitude de Tensão - kV
                                        5: Ângulo de Tensão - graus
                                        6: Fluxo Magnitude de Corrente - A
                                        7: Fluxo Ângulo de Corrente) - graus
                                        8: Injeção Magnitude de Corrente - A
                                        9: Injeção Ângulo de Corrente) - graus
                                        */
    TF_ESTADO ligado;      //Condição do canal de comunicação da medida
    
    double zmed;        //Valor medido
    double sigma;       //Desvio padrão da medida
    double prec;        //Precisão percentual do medidor
    long int idAlim;    //Identificador do alimentador na qual a medida está instalada

    //Para estimador Multiárea
    long int idArea;    //Identificador da área que a medida pertence no processo de decomposição
    long int idFront;   //Identificador da fronteira que a medida pertence no processo de decomposição
    
    long int par;       //Para indicar o par PQ, par VTeta, e par IdeltaI (somente para estimadores de estado baseados em corrente)
    
    double h;           //Valor da grandeza elétrica respectiva à medida calculado pelo modelo de medição
    long int nvar;      //Numéro de variáveis de estado relacionados com a medida
    double *reguaH;     //Índice das variáveis de estado relacionadas com a medida
    double *H;          //Gradiente calculado do modelo de medição em relação às variáveis de estado
    
} TF_DMED;



//------------------------------------------------------------------------------
//
//Estruturas de Dados para dados topológicos e RNP
//
//------------------------------------------------------------------------------
/**
 * @brief Estrutura de dados auxiliar para representar filas (listas encadeadas)
 *  
 */
typedef struct Fila {
    long int idNo;  //Identificador do nó
    long int idAdj; //Identificador sequencial de adjacenência
    long int profundidade;  //Profundidade na representação por RNP
    struct Fila * prox;     //Ponteiro para o próximo nó na lista
} TF_FILABARRAS; 


/**
 * @brief Estrutura de dados para representar circuitos alimentadores e respectiva topologia armazenada em lista encadeada através de RNP
 *  
 */
typedef struct {
    long int noRaiz;    //Identificação numérica do nó raiz ou início do alimentador
    long int idRaiz;    //Identificação sequencial do nó raiz ou início do alimentador
    long int idAlim;    //Identificação sequencial do alimentador
    long int numeroNos; //Quantidade de nós do alimentador
    TF_FILABARRAS rnp[1];  //Ponteiro para o início da lista encadeada (RNP) da topologia do alimentador
} TF_ALIMENTADOR;

/**
 * @brief Estrutura de dados para representar nós adjacentes e características elétrica em determinada topologia da rede
 *  
 */
typedef struct {
  long int idNo;    //Inteiro que identifica cada nó da rede elétrica.
  TF_ESTADO estado;    //Estado do ramo na topologia atual 0 = desligado e 1 = ligado
  TF_RAMO tipo;        //Tipo de ramo (linha, trafo, regulador ou chave)
  double relacao;   //Relacao de transformação no caso de transformadores
  TF_DRAM *ramo;       //Ponteiro para as informações do ramo
  long int idram;   //Identificador sequencial do ramo
  
  //Medidores
  long int nmed;    //Número de medidores associados ao ramo na topologia da rede
  TF_DMED**medidores; //Poneteiro para os medidores associados ao ramo
  
  //Grandezas elétricas que caracterizam o ramo
  __complex__ double S[3];      //Fluxo de potência elétrica complexa trifásica para o ramo adjacente
  __complex__ double Cur[3];    //Fasor de corrente elétrica trifásica para o ramo adjacente

  double losses;                //Perdas Trifásicas totais
  double loading;               //Maior carregamento em percentual entre as fases
  
  // Para o estimador de demanda trifásico
  long int idAM;     // Indica o índica da área de medição que a barra pertence
   
} TF_NOADJACENTE;

/**
 * @brief Estrutura de dados para representar nós de um grafo e adjacências (representa o grafo completo da rede elétrica)
 *  
 */
typedef struct {
  long int idNo;            //Inteiro que identifica cada nó da rede elétrica.
  long int profundidade;    //Profundidade do nó na representação RNP
  long int idAlim;          //Identificador do Alimentador
  long int idSubestacao;    //Identificador da Subestação Associada

  TF_FASES fases;          //Número de fases da barra
  double Vbase;         //Tensão nominal da barra
  long int tipo;        //Tipo de Barra no cálculo de fluxo de potência  0 - PQ; 1 - PV; 2 - Ref
  TF_DBAR *barra;          //Ponteiro para dados completos de barras
  
  double distanciaSE_acc;         //Distância acumulada até a barra

  long int numeroAdjacentes; /**< Indica o tamanho da lista de adjacentes do nó. */
  TF_NOADJACENTE adjacentes[15]; /**< Lista dos nós adjacentes. Limite de 15 para evitar alocação dinamica*/
  
  //Medidores
  long int nmed;        //Número de medidores associados ao nó na topologia da rede
  long int nmedPQ;        //Número de medidores de potência ativa e reativa associados ao nó na topologia da rede (para medidores Classe A)
  TF_DMED **medidores;     //Poneteiro para os medidores associados ao nó
  
  //Impedância Shunt Totoal
  __complex__ Ysh[3][3];    //Matriz de admitância trifásica complexa para agregar todos os elementos shunt conectados à barra
  
   
  //Grandezas elétricas que caracterizam a barra
  __complex__ double S[3];      //Injeção de potência elétrica complexa trifásica total no nó
  __complex__ double V[3];      //Fasor de tensão complexa trifásico no nó
  __complex__ double Cur[3];    //Injeção de potência elétrica complexa trifásica total no nó
  __complex__ double V_aux[3];  //Variavél auxiliar para cálculos de tensão complexa nodal (pode ser utilizado para representar a tensão em delta)
  
  // Para o estimador de demanda trifásico
  long int tipoNo;   // Indica se é barra sem carga (0), com carga (1), com capacitor (2) ou raiz de alimentador (3)
  long int idAM;     // Indica o índica da área de medição que a barra pertence
  
  
  // Para o estimador de estado multiárea somente  
//   long int idArea;
//   long int idFront;
//   double V_aux_Sig[3];
//   __complex__ double V_est[3];
//   __complex__ double V_coord[3];
//   int estimado;
} TF_GRAFO;

//-----------------------------------------------------
//ESTRUTURAS DE DADOS PARA ARMAZENAR RESULTADOS DE INTERESSE E INTERFACES COM OUTRAS FUNCIONALIDADES
//
//
//
//-----------------------------------------------------
// 
/*Dados de resultados condensados do cálculo de fluxo de potência*/
/**
 * @brief Dados de resultados condensados do cálculo de fluxo de potência para caracterizar as principais características de alimentadores e do sistema
 *   
 * Estrutura de dados para interface entre os resultados de cálculo de fluxo de potência e demais funcionalidades
 *   Pode ser utilizada para armazenar as principais características em alimentadores individuais ou em subestações e sistema completo
 */
typedef struct {
    int convergencia;       //Status de convergência do cálculo de fluxo de potência
    int iteracoes;          //Número de iterações para obter convergência
    BOOL tap_change_flag;       // Flag de mudança de tap no alimentador
    double maiorCarregamentoCorrente;   //Maior valor de carregamento em percentual 
    double perdasResistivas;            //Valor total de perdas na rede elétrica
    double maiorCarregamentoTrafo;      //Carregamento em percentual das subestações
    double carregamentoRede;            //Carregamento total dos alimentadores
    double menorTensao;                 //Valor de menor tensão na rede elétrica
    double quedaMaxima;                 //Queda de tensão em percentual em relação à subestação (nó raiz)
    double desbalancoTensaoMaximo;      //Valor máximo de desbalanço de tensão na rede elétrica
    double desbalancoCorrenteMaximo;     //Valor máximo de desbalanço de corrente nos ramos da rede elétrica
    double desbalancoCorrenteAlim;      //Valor máximo de desbalanço de corrente na saída dos alimentadores    
    
    double menorTensaoABC[3];                 //Valor de menor tensão na rede elétrica por fase
    double quedaMaximaABC[3];                 //Queda de tensão em percentual em relação à subestação (nó raiz) por fase
    double carregamentoRedeABC[3];            //Carregamento total dos alimentadores por fase
    
    
    
} TF_PFSOLUTION;






// ----------------------------------------------
//
// Elementos agregadores
//
// -------------------------------------------------

 
/*Dados estáticos de alimentadores*/
/**
 * @brief Define dados 
 *  
 * Representação individual das informações de subestações (um elemento TF_DSUBESTACAO para cada transformador individual da subestação)
 * 
 *  
 */
typedef struct {
    long int ID; //Identificador da subestação
    TF_ESTADO estado;  // Estado do alimentador (energizada ou não)
    int tipo;       // livre
    double Snominal; // Potência nominal do trafo do alimentador
    double Vpri;     // Tensão no nível primário do alimentador
    char COD_CH[15]; //Código COPEL da Chave início Alimentador
    char nome[40];   //nome do alimentador    
    char sigla_SE[7];   //sigla da subestação

    TF_ALIMENTADOR *circuito;  //ponteiro para o circuito alimentador associado
    int ID_SE;       // identificador da subestação
    int ID_TR;       // identificador do trafo da Se
    
    // Sumário de cálculo
    TF_PFSOLUTION *powerflow_summary;
} TF_DALIM;

// 
/*Dados estáticos de subestações do sistema de distribuição*/
/**
 * @brief Define dados subestações do sistema de distribuição como agregador de alimentadores em determinado nível de tensão da rede elétrica
 *  
 * Representação individual das informações de subestações (um elemento TF_DSUBESTACAO para cada transformador individual da subestação)
 * 
 *  
 */
typedef struct {
    long int ID; //Identificador da subestação
    TF_ESTADO estado;  // Estado da subestação (energizada ou não)
    int tipo;       // livre
    double Snominal; // Potência nominal do trafo da subestação
    double Vpri;     // Tensão no nível primário da subestação   
    BOOL rede_basica; // Identificado de conexão com a rede básica
    char nome[40];   //nome da subestação
    char sigla[7];   //sigla da subestação

    TF_DALIM alimentadores[50];  //Lista de identificadores sequenciais de alimentadores atribuídos a cada subestação (Agregador de alimentadores) limitado em 50
    int numeroAlimentadores;    //Contador do número de alimentadores
    TF_DTRF trafo[10];         // dados de transformador de subestação
    int numeroTrafos;       // número total de trafos
    
    // Sumário de cálculo
    TF_PFSOLUTION *powerflow_summary;

    //Dados para equivalentes de curto circuito (futuro)
    double Pcc3F;
    
} TF_DSUBESTACAO;

#endif	/* DATA_STRUCTURES_H */