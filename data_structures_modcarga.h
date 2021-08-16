/* 
 * File:   data_structures_modcarga.h
 * Author: Julio
 *
 * Created on 9 de Janeiro de 2015, 14:07
 * 
 * @brief Este arquivo contém a definição das estruturas de dados utilizadas no módulo de modelagem de carga do projeto smartgrids para
 * reconfiguração/restabelecimento de sistemas de distribuição de energia elétrica radiais (SDR).
 * Os tipos que estão sendo definidos para a aplicação tem como padrão o nome com todas as letras em caixa alta.
 * Para compor o nome das variáveis com nome composto o padrão utizado é o primeiro "nome" em minúsculo e os demais com a primeira letra em maiúsculo e as demais em minúsculo.
 * @see http://lcrserver.icmc.usp.br/projects/smartgrid-copel/wiki
 * @see http://lcrserver.icmc.usp.br/projects/codes/wiki
 */

#ifndef DATA_STRUCTURES_MODCARGA_H
#define	DATA_STRUCTURES_MODCARGA_H

/**
 Define o fator de potência típico para as cargas sem informação de kVAr
 */
#define FP_TIPICO 0.92

/**
 Define o fator k do critério de convergência do ECTR
 */
#define PREC_LMF 2.0 //2.0 corresponde a 98% de confiança na tabela da curva Gaussiana

/**
 Define o numero de pontos da curva de carga
 */
#define NPTS_CURVA 24

/**
 Define o desvio padrão mínimo das medidas
 */
#define STD_MED_MIN 0.01  //kVA

/**
 Define o fator K da agregação JARDINI
 */
#define FAT_K_AGREG 2.0 //2.0 corresponde a 98% de confiança na tabela da curva Gaussiana

/**
 Define se usa ou não as medidas de corrente
 */
#define MED_CUR 0 //0 corresponde a usar as medidas de corrente e 1 a não usar

// Tipo de agregaçao de carga - para curvas de carga com desvio_padrao
typedef enum  {
        VALOR_ESPERADO,
        MAJORANTE_QUANTIL,
        EQUILIBRADO_MAXIMO,
        DESBALANCEADO_INPUT_DBAR
} TIPO_AGREGACAO_CARGAS;

/**
 * @brief Armazena as informações de curvas de carga típicas
 *
 */
typedef struct {
  int total; /**< Salva o tamanho do array no código na posição "0" */ 
  char id[21]; /**< Identificação do Tipo de Curva de Carga */ 
  int tamanho; /**< Valor do número de pontos da curva de carga. */
  double minimo; /**< Valor do tipo double indicando o valor mínimo de consumo. */
  double maximo; /**< Valor do tipo double indicando o valor máximo de consumo. */
  double *media; /**< Valor do tipo double indicando o valor médio da curva de carga. */
  double *std_dev; /**< Valor do tipo double indicando o desvio padrão da curva de carga. */
} CURVA_CARGA;

/**
 * @brief Armazena as informações de curvas de carga agregadas
 *
 */
typedef struct {
  long int id;
  double P; /**< Valor do tipo double indicando o valor mínimo de consumo. */
  double Q; /**< Valor do tipo double indicando o valor máximo de consumo. */
  double stdP; /**< Valor do tipo double indicando o valor mínimo de consumo. */
  double stdQ; /**< Valor do tipo double indicando o valor máximo de consumo. */
  long int tamanho;
  
  double *PA; /**< Valor do tipo double indicando o valor médio da curva de carga agregada. */
  double *stdPA; /**< Valor do tipo double indicando o desvio padrão da curva de carga agregada. */
  double *PB; /**< Valor do tipo double indicando o valor médio da curva de carga agregada. */
  double *stdPB; /**< Valor do tipo double indicando o desvio padrão da curva de carga agregada. */
  double *PC; /**< Valor do tipo double indicando o valor médio da curva de carga agregada. */
  double *stdPC; /**< Valor do tipo double indicando o desvio padrão da curva de carga agregada. */
  
  double *QA; /**< Valor do tipo double indicando o valor médio da curva de carga agregada. */
  double *stdQA; /**< Valor do tipo double indicando o desvio padrão da curva de carga agregada. */
  double *QB; /**< Valor do tipo double indicando o valor médio da curva de carga agregada. */
  double *stdQB; /**< Valor do tipo double indicando o desvio padrão da curva de carga agregada. */
  double *QC; /**< Valor do tipo double indicando o valor médio da curva de carga agregada. */
  double *stdQC; /**< Valor do tipo double indicando o desvio padrão da curva de carga agregada. */

} CURVA_TRAFO;

/**
 * @brief Armazena as informações de classificação de consumidores de acordo com o CNAE e respectiva curva de carga.
 *
 */
typedef struct {
  int total; /**< Salva o tamanho do array no código na posição "0" */ 
  char cod_ativ_cnae[8]; /**< Classificação CNAE. */
  char cod_ativ[7]; /**< Código Atividade. */
  char desc_ativ[61]; /**< Descrição da classificação CNAE. */
  char curva_carga[21]; /**< Curva de carga para a classificação CNAE. */
} DADOS_CNAE;

/**
 * @brief Armazena as informações de consumidores.
 *
 */
typedef struct {
  int total; /**< Salva o tamanho do array no código na posição "0" */ 
  int num_cli_uc; /** <Identificação do consumidor. */
  double cons_kWh_mes; /**< Consumo mensal de potência ativa em kWh. */
  double cons_kVArh_mes; /**< Consumo mensal de potência reativa em kWh. */
  
  int fase_rede_uc; /**< Fase ligada na rede (1 = A, 2 = B, 3 = C, 4 = AB, 5 = AC, 6 = BC e 7 = ABC) */
  char numero_posto_UC[20]; /**< Classificação CNAE do consumidor. */
  char cod_nivel_tensao_uc[3]; /**< Grupo de Tensão. */ 
  char cod_ativ_cnae_uc[8]; /**< Classificação CNAE do consumidor. */  
  
  CURVA_CARGA curva_carga; /**< Curva de carga do consumidor. */
  
  long int no;
  double Pbase;
  double Qbase;
  double fp;
  double P;
  double Q;
  double std_P;
  double std_Q;
} DADOS_CONSUMIDOR;

/**
 * @brief Armazena as informações de consumidores.
 *
 */
typedef struct {
    long int tipo; /**< Tipo de medida: 1 = PQ; 2 = I. */
    long int med_local; /**< 1 = Medida no SD; 2 = Consumidor Classe A. */
    char FEAT_num[11];
    long int de,para,prop,jus; //prop: representa a barra proprietária da medida depende da varredura
    
    double Pmed;
    double Qmed;
    double Imed;
    double Vmed;
    double stdPmed;
    double stdQmed;
    double stdImed;
    double stdVmed;
    double precisaoP;
    double precisaoQ;
    double precisaoI;
    double precisaoV;
    
    double Pcalc;
    double Qcalc;
    __complex__ double Icalc;
    __complex__ double Vcalc;
    
} DADOS_MEDIDOR;

/**
 * @brief Armazena as informações de consumidores.
 *
 */
typedef struct {
    long int total;
    long int tipo; // tipo do medidor da AM (1) medidor de potência ou (2) medidor de corrente
    long int numeroAdj;
    long int idAlim;
    long int idAdj; //índice do adjacente no grafo (para acessar o NOADJACENTE)
    long int descarte;
    
    DMED *medidor; //Ponteiro para o medidor
    FASES fase;
    long int *med_adjacentes; /**< Areas de medição adjacentes. */
    long int montante;
    long int numeroNosAM;
    long int *nos; /**< Nós do tipo "comCarga" a serem ajustados pela AM. */
    long int med_local; /**< 1 = Medida no SD; 2 = Consumidor Classe A. */
    
    double fp;
    double DeltaP;
    double DeltaQ;
    long int convP;
    long int convQ;
    
    
    double SumPnos;
    double SumQnos;
    double ajusteP;
    double ajusteQ;
    
    //Valores medidos da AM
    double Pmed;
    double Qmed;
    double Imed;
    double Vmed;
    double stdPmed;
    double stdQmed;
    double stdImed;
    double stdVmed;
    
    
    //Valores Calculados no Fluxo de POtência para a AM
    double Pcalc;
    double Qcalc;
    __complex__ double Icalc;
    __complex__ double Vcalc;
    
} AREA_MEDICAO;

#endif	/* DATA_STRUCTURES_MODCARGA_H */

