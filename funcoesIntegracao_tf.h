/* 
 * File:   funcoesIntegracao.h
 * Author: Julio Massignan e Vítor Henrique Pereira de Melo
 *
 * Created on 30 de Junho de 2022, 14:03
 */

#ifndef funcoesIntegracaoTF_H
#define	funcoesIntegracaoTF_H
#include "data_structures.h"
#include "data_structures_tf.h"
/**
 * @brief Função para converter o grafo utilizado no fluxo do potência, para preencher o grafo da MRAN e os dados dos reguladores. 
 *
 * Essa função recebe como parâmetro o grafo do tipo TF_GRAFO, do fluxo de potência trifásico e realiza a conversão de suas informações para 
 * para preencher o grafo @p **grafoSDRParam do tipo GRAFO. A partir do grafo trifásico também é preenchida a estrutura @p **grafoSDRParam , do tipo GRAFO. Ambas estruturas são alocadas nessa funcao
 * Além disso, ela retorna número de nós da rede @p numeroBarras , o número de transformadores em @p numeroTrafos , e o número de chaves em @p numeroChaves , que são ponteiros para long int.
 * Todas estas informacoes serao necessarias para utilizar as funcoes da MRAN e o fluxo de potência por varredura de RNP
 * @param grafo_tf grafo trifásico da rede com as informações que serão fornecidas nesta função
 * @param  numeroBarras_tf inteiro com o número de barras da rede, segundo o fluxo trifásico
 * @param ramos_tf vetor do tipo TF_DRAM com as informações dos ramos da rede, ulizada para preencher o grafoSDR e dadosRegulador
 * @param nramos_tf inteiro que conta o número de ramos da rede
 * @param grafoSDRParam ponteiro para um vetor do tipo GRAFO, retorna o grafo da rede a ser utilizado na MRAN 
 * @param dadosReguladorSDRParam ponteiro para um vetor do tipo GRAFO com as informacoes dos reguladores a ser utilizado na MRAN
 * @param numeroBarras retorna o número de nos no grafo grafoSDRParam 
 * @param numeroTrafos retorna o número de trafos
 * @param numeroChaves retorna o numero de chaves na rede
 * @return void.
 * @see leituraBarras
 * @see leituraLinhas
 * @see leituraTrafos
 * @see leituraDados
 * @see geraGrafo
 * @note
 * @warning .
 */
void converteGrafo_TFtoSDR(TF_GRAFO *grafo_tf,long int numeroBarras_tf,TF_DRAM *ramos_tf,long int nramos_tf,GRAFO **grafoSDRParam,DADOSREGULADOR **dadosReguladorSDRParam,long int* numeroBarras, long int *numeroChaves);

/**
 * @brief Função de integracao da MRAN, para preencher a estrutura dadosAlimentadorSDRParam do tipo DADOSALIMENTADOR que será utilizada nas funcoes da MRAN. 
 * 
 * Essa função recebe como parâmetro o vetor @p grafo_tf do tipo TF_GRAFO, com as informacoes dos alimentadores da rede, lidas para o calculo do fluxo de potencia trifasico
 * e o long int @p numerosAlimentadores_tf com o numero de alimentadores de acordo com o fluxo trifásico. Em posse destes dados, esta funcao aloca o espaço necessário para a estrutura @p dadosAlimentadorSDRParam
 * e a preenche, com as informacoes necessarias para utilizar as funcoes da rnp. 
 *
 * 
 * @param grafo_tf vetor do tipo TF_GRAFO com as informacoes trifasicas sobre a rede
 * @param numeroBarras numero inteiro com o numero de barras da rede
 * @param numero_alimentadores numero inteiro que conta o numero de alimentadores presentes no sistema
 * @param dadosAlimentadorSDRParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @return void
 * @see 
 * @note
 * @warning .
 **/

void converteDadosAlimentadores_TFtoSDR(TF_GRAFO *grafo_tf,long int numeroBarras,long int numerosAlimentadores_tf, DADOSALIMENTADOR **dadosAlimentadorSDRParam );



/**
 * @brief Função que compatibiliza a profundidade do grafo trifásico com a da RNP de setores
 * 
 * Essa função compatibiliza a profunidade na estrutura grafo do tipo TF_GRAFO e que contem as informações sobre a rede trifásica.
 * Esta função realiza uma varredura fowared pela RNP de setores e pelos setores e insere no grafo as profundidades totais da rede.
 * Esta função recebe como parâmetros @p grafo_tf que é um vetor do tipo TF_GRAFO com as informações da rede trifásica, do tipo TF_GRAFO com as informações da rede trifásica, 
 * @p configuracaoParam que é um vetor com as configuracoes da rede que é indexada pelo inteiro @p indiceConfiguracao com o id da configuracao atual, com o vetor @p matrizB do tipo RNPSETORES com as RNPS de setores 
 * @p indiceRNP trata-se de um inteiro com o alimentador que está sendo preenchido, @p nvar ponteiro para inteiro que retorna o número de variáveis da rede  
 * 
 * @param grafo ponteiro para o grafo da rede elétrica para inicilizar as tensões nodais
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param nvar ponteiro para inteiro que retorna o número de variáveis da rede 
 * @return void
 * @see
 * @note 
**/

void compatibiliza_profundidadegrafo_tf(TF_GRAFO *grafo_tf,CONFIGURACAO *configuracoesParam,int indiceConfiguracao,RNPSETORES *matrizB,long int indiceRNP, int *nvar);

//Cálculo de Fluxo de Potência para um Alimentador de Distribuição Baseado na Varredura Direta/Inversa Trifasica Utilizando RPN
/**
 * @brief Função auxiliar para o cálculo de fluxo de potência trifásico em um alimentador específico via método de Varredura Direta/Inversa através da RNP de setores,
 * utiliza as informacoes da rede na estrutura @p grafo do tipo TF_GRAFO, com as informacoes elétricas da rede, respeiando a configuacao topologica disponível na 
 * @p configuracoesParam ponteiro do tipo CONFIGURACAO com a configuracao da rede, @p matrizB ponteiro do tipo RNPSETORES com a RNP de setores da rede  
 * 
 * Essa função efetua
 * A função retorna @c TF_PFSOLUTION.
 * 
 * @param grafo grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param numeroBarras quatidade total de barras
 * @param alimentadores alimentadores da rede elétrica
 * @param ramos vetor do tipo TF_DRAM com as informações dos ramos da rede
 * @param Sbase double com a potencia base da rede
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param dadosAlimentadorParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
*/

TF_PFSOLUTION fluxoPotencia_BFS_Alimentador_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR alimentador, TF_DRAM *ramos,double Sbase,
        /*int numeroBarrasParam,*/ CONFIGURACAO *configuracoesParam, 
        /*double VFParam,*/DADOSALIMENTADOR *dadosAlimentadorParam , int indiceConfiguracao, RNPSETORES *matrizB/*,
        MATRIZCOMPLEXA *ZParam,*/ /* MATRIZMAXCORRENTE *maximoCorrenteParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulado*/ );


//Cálculo de Fluxo de Potência em todos os alimentadores realizada de acordo com o nível de tesão do alimentador
/**
 * @brief Função auxiliar para o cálculo de fluxo de potência trifásico nos alimentadores via método de Varredura Direta/Inversa através da RNP de setores,
 * , realiza a verradura inicialmente nos alimentadores de 13.8kV, realiza depois nos alimentadores de 34.5kV com a carga do alimentador anterior refletida nele
 * Verifica se há mudança de tap, caso positivo, reinicia o calculo do fluxo nos alimentadores de 13.8kV utiliza as informacoes da rede na estrutura @p grafo do tipo TF_GRAFO, com as informacoes elétricas da rede, respeiando a configuacao topologica disponível na 
 * @p configuracoesParam ponteiro do tipo CONFIGURACAO com a configuracao da rede, @p matrizB ponteiro do tipo RNPSETORES com a RNP de setores da rede  
 * Essa função efetua
 * A função retorna @c TF_PFSOLUTION.
 * 
 * @param grafo grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param numeroBarras quatidade total de barras
 * @param alimentadores alimentadores da rede elétrica
 * @param interfaceNiveis 
 * @param numeroInterfaces
 * @param opt_flow
 * @param ramos vetor do tipo TF_DRAM com as informações dos ramos da rede
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param dadosAlimentadorParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * 
 * */

void fluxoPotencia_Niveis_BFS_Multiplos_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos,double Sbase,
        long int **interfaceNiveis,long int numeroInterfaces, BOOL opt_flow,/* long int numeroBarrasParam, */
        DADOSALIMENTADOR *dadosAlimentadorParam, /*DADOSTRAFO *dadosTrafoParam,GRAFO *grafoSDRParam,*/
        CONFIGURACAO *configuracoesParam, long int indiceConfiguracao, RNPSETORES *matrizB,TF_PFSOLUTION *powerflow_result_rede, TF_PFSOLUTION **powerflow_result_alim/*,
        MATRIZCOMPLEXA *ZParam*//*, int *indiceRegulador, DADOSREGULADOR *dadosRegulador, MATRIZMAXCORRENTE * maximoCorrenteParam*/);



/**
 * @brief Função que inicializa a tenssão de todos os alimentadores
 * 
 * Essa função percorre todos os alimentadores que tem suas tensões incializadas individualmente a partir da fução inicializaTensaoSDR_alimentador_tf
 * Esta função recebe como parâmetros @p configuracaoParam que é um vetor com as configuracoes da rede que é indexada pelo inteiro @p indiceConfiguracao com o id da configuracao atual, com o vetor @p matrizB do tipo RNPSETORES com as RNPS de setores
 * @p todosAlimentadores do tipo BOLL que informa se a funcao seerá executada para todos alimentadores, @p grafo do tipo TF_GRAFO com as informações da rede trifásica e que recebera as tensoes  
 * @p numeroAlimentadores inteiro com o número de alimentaores da rede, @p numeroBarras com o número de barras da rede, @p alimentadores estrutura do tipo TF_ALIMENTADOR com os alimentadores da rede.
 * 
 * @param grafo ponteiro para o grafo da rede elétrica para inicilizar as tensões nodais
 * @param numeroBarras quatidade total de barras
 * @param alimentadores ponteiro par os alimentadores da rede elétrica
 * @param numeroAlimentadores quantidade total de alimentadores
 * @param todosAlimentadores BOLL que indica se deve ser executado para todos os alimentadores
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param 
 * @return void
 * @see
 * @note 
**/
void inicializaTensaoSDR_alimentadores_tf (TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores,
BOOL todosAlimentadores, CONFIGURACAO* configuracaoParam,RNPSETORES *matrizB,int indiceConfiguracao);


//Função inicialização do vetor x para sistemas radiais através de uma varredura para compensar os taps fora da nominal (sem o arquivo Vinicial)
/**
 * @brief Função auxiliar para inicializar as tensões complexas nodais trifásicas de todos os alimentadores através de um processo de varredura direta por RNP de Setores
 * 
 * Essa função inicializa as tensões complexas nodais por fase (fase-neutro) de todas as barras de um alimentador de acordo com a RNP de setores
 * O valor de inicilização é obtido através de uma varredura direta utilizando a RNP de setores disponível na  @p configuracaoParam que é um vetor com as configuracoes da rede
 * que é indexada pelo inteiro @p indiceConfiguracao com o id da configuracao atual, com o vetor @p matrizB do tipo RNPSETORES com as RNPS de setores, @p indiceRNP um inteiro com o índice da RNP de setores atual
 * @p todosAlimentadores do tipo BOLL que informa se a funcao seerá executada para todos alimentadores, @p root inteiro com o nó raiz do alimentador, @p grafo do tipo TF_GRAFO com as informações da rede trifásica e que recebera as tensoes  
 * @p numeroAlimentadores inteiro com o número de alimentaores da rede, @p numeroBarras com o número de barras da rede, @p alimentadores estrutura do tipo TF_ALIMENTADOR com os alimentadores da rede.
 * A função retorna @c void.
 * 
 * @param grafo ponteiro para o grafo da rede elétrica para inicilizar as tensões nodais
 * @param numeroBarras quatidade total de barras
 * @param alimentadores ponteiro par os alimentadores da rede elétrica
 * @param numeroAlimentadores quantidade total de alimentadores
 * @param todosAlimentadores BOLL que indica se deve ser executado para todos os alimentadores
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @param indiceRNP inteiro com o indice da RNP atual
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param root inteiro com o indice do no raiz
 * @param 
 * @return void
 * @see
 * @note 
*/

void inicializaTensaoSDR_alimentador_tf(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, 
long int numeroAlimentadores, CONFIGURACAO* configuracaoParam, RNPSETORES *matrizB,int indiceRNP,int indiceConfiguracao,int root);


//
/**
 * @brief Função de integracao da MRAN, para compatibilizar as barras ficticias criadas no código da MRAN nos elementos do fluxo trifásico 
 * 
 * Essa função recebe como parâmetro o vetor @p grafoSDR do tipo GRAFO, com as informacoes da rede modificada pela MRAN, @p numeroNos que é um inteiro com o número de nós da rede
 *. de acordo com estas informacoes ela complementa os nós criados nas seguintes variáveis, que são utilizadas no fluxo trifásico:
  @p grafo_tf do tipo TF_GRAFo, @p barras do tipo TF_DBAR, @p ramos do tipo TF_DRAM, @p numeroRamos, @p numeroBarras 
 *
 * 
 * @param grafo_tf vetor do tipo TF_GRAFO com as informacoes trifasicas sobre a rede
 * @param barras vetor do tipo TF_DBAR com as informações trifásicas sobre as barras da rede
 * @param numeroBarras inteiro com o número de barras da rede trifásica
 * @param ramos numero inteiro que conta o numero de alimentadores presentes no sistema
 * @param numeroRamos inteiro que conta o número de ramos da rede
 * @param grafoSDR vetor do tipo GRAFO, o grafo da rede de acordo com a MRAN 
 * @param numeroNos inteiro com o número de nós da rede de acordo com a MRAN
 * @return void
 * @see 
 * @note
 * @warning .
 */

void compatibiliza_chaveSetoresFicticia_tf(TF_GRAFO** grafo_tf,TF_DBAR **barras, long int *numeroBarras, TF_DRAM **ramos, long int *numeroRamos,GRAFO* grafoSDR, long int numeroNos);


/**
 * @brief Função Avalia configuracao, realiza o cálculo do fluxo de potência para uma configuracao específica e compila os resultados, 
 * recebe como parâmetros @p todosAlimentadores, um booleano que indica se o fluxo deve ser resolvido para todos os alimentadores ou apenas para dois
 * @p FirstEXEC um boleano que indica se é a primeira execução do fluxo de potência, @p *powerflow_result_rede uma estrutura do tipo TF_PFSOLUTION que armazena
 * os resultados todais da rede, @p powerflow_result_alim um vetor com os resultados de todos os alimentadores, @p configuracoesParam do tipo CONFIGURACAO
 * com as informações das rnps e os resultados das configuracoes da rede, @p idNovaConfiguracaoParam um inteiro com a identificação da configuração atual,
 * @p grafo_tf uma estrutura do tipo TF_GRAFO com as informações elétricas do grafo da rede, @p numeroBarras_tf com o número de barras totais de acordo com a rede trifásica
 * @p alimentador_tf vetor do tipo TF_ALIMENTADOR, @p numeroAlimentadores_tf inteiro com o número de alimentadores na rede, @p ramos_tf estrutura do tipo TF_DRAM
 * com as informações sobre os ramos da rede trifásica, @p Sbase double com a potência base da rede, @p interfaceNiveis_tf matriz de inteiro utilizada para fazer
 * a interface entre niveis, @p numeroInterfaces_tf inteiro com o númeor de interfaces na rede, @p opt_flow boleano da opção de fluxo de potência @p numeroTrafosSE
 * numero de trafos na subestação 
 *  
 * 
 * 
 * @param todosAlimentadores boleano que indica se o fluxo será realizado em todos os alimentadores
 * @param FirstEXEC booleano que indica se é a primeira execução
 * @param powerflow_result_rede estrutura do tipo TF_PFSOLUTION que guarda os resultados do fluxo de potência da rede
 * @param powerflow_result_alim vetor do tipo TF_PFSOLUTION que guarda os resultados do fluxo de potência dos alimentadores
 * @param configuracoesParam vetor do tipo CONFIGURACAO que guarda as informacoes das configuracoes
 * @param idNovaConfiguracaoParam inteiro com o indice da configuracao atual
 * @param dadosAlimentadorParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @param grafo_tf vetor do tipo TF_GRAFO com as informacoes trifasicas sobre a rede
 * @param numeroBarras_tf inteiro com o número de barras da rede, segundo o fluxo trifásico
 * @param alimentador_tf estrutura do tipo TF_ALIMENTADOR com as informações sobre os alimentadores 
 * @param numeroAlimentadores_tf inteiro com o numero de alimentadores trifásicos
 * @param ramos_tf vetor do tipo TF_DRAM com as informações dos ramos da rede, ulizada para preencher o grafoSDR e dadosRegulador
 * @param Sbase potencia base do sistema
 * @param interfaceNiveis_tf matriz de inteiros com o número de interfaces na rede
 * @param  opt_flow boleano
 * @param numeroTrafosSE numero de transformadores de subestacao
 * @return void
 * @see
 * @note 
 * @warning 
 * Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * */


void avaliaConfiguracaoSDR_tf(BOOL todosAlimentadores, TF_PFSOLUTION *powerflow_result_rede, TF_PFSOLUTION **powerflow_result_alim, CONFIGURACAO *configuracoesParam, long int idNovaConfiguracaoParam, /*DADOSTRAFO *dadosTrafoParam, int numeroTrafosParam,
        int numeroAlimentadoresParam,int *indiceRegulador, DADOSREGULADOR *dadosRegulador,*/ DADOSALIMENTADOR *dadosAlimentadorParam, /*double VFParam,*/ int idAntigaConfiguracaoParam, RNPSETORES *matrizB, int RNP_P,int RNP_A,/*MATRIZCOMPLEXA *ZParam,
        MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam, BOOL copiarDadosEletricos,*/
        TF_GRAFO *grafo_tf, long numeroBarras_tf, TF_ALIMENTADOR *alimentador_tf, int numeroAlimentadores_tf,
        TF_DRAM *ramos_tf,double Sbase, long int **interfaceNiveis_tf,long int numeroInterfaces_tf, BOOL opt_flow,long int numeroTrafosSE);

//Compila os resultados do Fluxo de Potência em todos os alimentadores para gerar os resultados totais da rede
/**
 * @brief Função auxiliar para calcular os resultados gerais da rede elétrica, a partir dos resultados de cada alimentador, recebe como entrada @p powerflow_result vetor de TF_PFSOLUTION
 * do tipo TF_PFSOLUTION com os resultados do fluxo de potencia por alimentador  a estrutura @p alimentadores do tipo TF_ALIMENTADOR com as informações trifásicas dos alimentadores, @p numeroAlimentadores um inteiro para inteiro com o número de alimentadores da rede
 * @p grafo do tipo TF_GRAFO, com as informacoes elétricas da rede, respeiando a configuacao topologica da RNP de setores @p numeroInterfaces inteiro com o número de interfaces entre niveis de tensão
 * @p interfaceNiveis matriz de inteiros para realizar a interface da rede , retorna uma estrutura do tipo TF_PFSOLUTION com as informacoes totais da rede. 
 * @param powerflow_result com os resultados do fluxo de potência
 * @param alimentadores alimentadores da rede elétrica
 * @param numeroAlimentadores numero de alimentadores da rede elétrica
 * @param grafo grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param interfaceNiveis 
 * @param numeroInterfaces
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * 
 **/

TF_PFSOLUTION compilaResultadosRede (TF_PFSOLUTION *powerflow_result,int numeroAlimentadores, TF_ALIMENTADOR *alimentadores ,TF_GRAFO *grafo, int numeroBarras ,double Sbase,
long int numeroInterfaces, long int **interfaceNiveis );
#endif	/* funcoesIntegracao_H */

//Cálculo de Fluxo de Potência em dois alimentadores os alimentadores 
/**
 * @brief Função auxiliar para o cálculo de fluxo de potência trifásico em dois alimentadores específicos via método de Varredura Direta/Inversa através da RNP de setores,
 * , realiza a verradura inicialmente nos alimentadores de 13.8kV, realiza depois nos alimentadores de 34.5kV com a carga do alimentador anterior refletida nele
 * Verifica se há mudança de tap, caso positivo, reinicia o calculo do fluxo nos alimentadores de 13.8kV utiliza as informacoes da rede na estrutura @p grafo do tipo TF_GRAFO, com as informacoes elétricas da rede, respeiando a configuacao topologica disponível na 
 * @p configuracoesParam ponteiro do tipo CONFIGURACAO com a configuracao da rede, @p matrizB ponteiro do tipo RNPSETORES com a RNP de setores da rede  
 * Essa função efetua
 * A função retorna @c TF_PFSOLUTION.
 * 
 * @param grafo_tf grafo da rede elétrica com informações da rede, conectividade e parâmetros, onde são aramzenados os resultados detalhdos do cálculo de fluxo de potência
 * @param numeroBarras_tf quatidade total de barras
 * @param alimentadores alimentadores da rede elétrica
 * @param interfaceNiveis 
 * @param numeroInterfaces
 * @param opt_flow
 * @param ramos vetor do tipo TF_DRAM com as informações dos ramos da rede
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param dadosAlimentadorParam ponteiro pra ponteiro de estrutura do tipo DADOSALIMENTADOR, que retorna o vetor com os alimentadores da rede
 * @param indiceConfiguracao inteiro com o indice da configuracao atual 
 * @param matrizB vetor do tipo RNPSETORES com as RNPS de setores
 * @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * 
 * */

void fluxoPotencia_alimentador_P_A_tf(TF_GRAFO *grafo_tf ,long int numeroBarras_tf, TF_ALIMENTADOR *alimentador_tf, TF_DRAM *ramos_tf,double Sbase,
        /*int numeroBarrasParam,*/ CONFIGURACAO *configuracoesParam, 
        /*double VFParam,*/ DADOSALIMENTADOR *dadosAlimentadorParam ,int indiceConfiguracao, RNPSETORES *matrizB, long int RNP_P, long int RNP_ATF_PFSOLUTION, TF_PFSOLUTION *powerflow_result_rede, TF_PFSOLUTION **powerflow_result_alim/*,
        MATRIZCOMPLEXA *ZParam,*/ /* MATRIZMAXCORRENTE *maximoCorrenteParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulado*/,
long int numeroInterfaces, long int **interfaceNiveis );




//Cálculo de Calculo do Carregamento dos transformadores da subestação
/**
 * @brief Função auxiliar para o cálculo do carregamento dos transformadores da subestação baseado no resultado do fluxo de potência trifásico e na configuração da rede.
 * recebe como parâmetros a estrutura @p alimentador_tf do tipo TF_ALIMENTADOR com as informações trifásicas dos alimentadores, @p numeroalimetadores_tf um inteiro para inteiro com o número de alimentadores da rede
 * @p numerotrafosSE com o número de transformadores de subestação @p configuracoesParam uma estrutura do tipo CONFIGURACAO com as informações sumarizadas de cada um dos alimentadores 
 * @p idNovaConfiguracaoParam  inteiro com a identificação da configuracao atual @p idAntigaConfiguracaoParam inteiro com a identificação da configuracao antiga @p todosAlimentadores
 * boleano que indica se deve ser realizado em um alimentador ou em todos @p idRNPOrigem indice da RNP de origem, @p idRNPDestino indice da RNP de destino
  
 * @param alimentador_tf alimentadores da rede elétrica
 * @param numeroalimetadores_tf numero de alimentadores da rede trifasica
 * @param numerotrafosSE numero de transformadores de subestação da rede trifásica
 * @param configuracaoParam vetor do tipo CONFIGURACAO com as configuracoes atuais dos individuos da RNP de setores
 * @param idNovaConfiguracaoParam inteiro com o indice da configuracao atual 
 * @param idAntigaConfiguracaoParam inteiro com o indice da configuracao anterior
 * @param todosAlimentadores boleano que indica se será feito em todos os alimentadores
 * @param idRNPOrigem indice da RNP de origem
 * @param idRNPDestino indice da RNP de destino
 *  @return TF_PFSOLUTION resultados condensados do cálculo de fluxo de potência trifásico
 * @see fluxoPotencia_BFS_Multiplos
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 * 
 * */

void carregamentoTrafo_tf(TF_ALIMENTADOR *alimentador_tf, long int numeroalimetadores_tf, long int numerotrafosSE,
        CONFIGURACAO *configuracoesParam, int idNovaConfiguracaoParam, 
        int idAntigaConfiguracaoParam, BOOL todosAlimentadores, int idRNPOrigem, int idRNPDestino);

/**
 * @brief Função para integrar as informações dos dados dos alimentadores e da subestação em cada alimentador 
 *
 * Essa função recebe como parâmetro o @p *DTRFSE do tipo TF_DTRFSE, com as informações dos transformadores da subestação, @p DALIM do tipo TF_DALIM, com as informações do arquivo DALIM.csv
 * o @p numeroTFSES um inteiro com o numero de trafos de subestação, @p numeroDALIM um inteiro com número de alimentadores no arquivo DALIM.csv, @p **alimentador_tf estrutura com os dados dos
 * alimentadores trifásicos e que será preenchida
 * @param DTRFSE Estrutura do tipo TF_DTRFSE com as informações dos transformadores das subestações
 * @param  DALIM Estrutura do tipo TF_DALIM com as informações sobre os alimentadores disponivilizadas no arquivo DALIM.csv
 * @param numeroTFSES Inteiro com o número de transformadores na subestação
 * @param numeroDALIM Inteiro com o número de alimentadores no arquivo DALIM
 * @param alimentadores alimentadores da rede elétrica
 * @return void.
 * @note
 * @warning .
 */

void trafoSB_info(TF_DTRFSE *DTRFSE,TF_DALIM *DALIM, long int numeroTFSES,long int numeroDALIM, long int numeroAlim_tf ,TF_ALIMENTADOR **alimentador_tf);

//Alocada na estrutrura de resultados do fluxo de potencia
/**
 * @brief Função auxiliar para alocação memória da estrutura TF_PFSOLUTION
 *
 * Essa função realiza a alocação de memória de forma apropriada para a estrutura DBAR.
 * A função retorna @c void .
 * 
 * @param pfresult ponteiro para TF_PFSOLUTION a ser alocado o espaço de memória
 * @return void
 * @see
 * @note 
 * @warning
 */

void aloca_pfresult(TF_PFSOLUTION **pfresult);