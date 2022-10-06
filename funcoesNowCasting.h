/**
 * @file funcoesNowCasting.h
 * @author Vitor Henrique Pereira de Melo
 * @date 08 set 2022
 * @brief Cabeçalho da biblioteca para leitura de dados trifásico para análise estática de redes de distribuição
 * As funções definidas na biblioteca estão implemetadas em funcoesLeitura.c
 *
 */
#ifndef funcoesNowCasting_TF_H
#define	funcoesNowCasting_TF_H

/**
 * @brief Funcao que constroi o DMED de previsão a partir das medidas previstas pelo NowCasting
 * Essa recebe como parâmetro a estrutura @p prev_tf, um vetor do tipo TF_DPREV com os dados de previsão obtidos com o NowCasting,
 * um inteiro com o número de medidas @p nmed, @p grafo um vetor do tipo TF_GRAFO, @p Sbase do tipo double com a potência base da rede
 * para criar a estrutura @p medidas do tipo TF_DMED, para receber os dados de previsão a cada iteracao do estimador de demanda com 
 * Now casting. Esta funcao aloca memória para a estrutura @p medidas
 * 
 * @param prev_tf vetor do tipo TF_DPREV com as medidas previstas pelo NowCasting
 * @param nmed inteiro com o número de medidas da rede
 * @param medidas estrutura do tipo TF_DMED com as medidas previstas
 * @param grafo vetor do tipo TF_GRAFO com as informações elétricas da rede trifasica
 * @param Sbase double com a potencia base da rede
 */
void constroi_dmed_prev(TF_DPREV *prev_tf, int nmed,TF_DMED **medidas,TF_GRAFO*grafo,double Sbase);

/**
 * @brief Funcao que atualiza os valores medidos atuais na area de medição
 * 
 * Recebe como parâmetros @p grafo um vetor do tipo TF_GRAFO com as informações elétricas sobre a rede trifásica, @p numeroBarras inteiro com o numero de barras
 * @p medidas vetor do tipo TF_DMED com as medidas atualizadas da rede, @p  numeroMedidas inteiro com o número de medidas
 * @p areasAM vetor do tipo  TF_AREA_MEDICAO com as informacoes sobre as áreas de medicao
 * 
 * @param grafo vetor do tipo TF_GRAFO com as informações elétricas da rede trifasica
 * @param numeroBarras inteiro com o número de barras na rede
 * @param medidas estrutura do tipo TF_DMED com as medidas previstas
 * @param numeroMedidas inteiro com o numero de medidas sobre a rede
 * @param areasAM estrutura do tipo TF_AREA_MEDICAO com as informcaoes sobre as areas de medicao
 */
void atualiza_AM(TF_GRAFO * grafo, long int numeroBarras, TF_DMED*medidas, long int numeroMedidas, TF_AREA_MEDICAO *areasAM);

/**
 * @brief Função que atualiza o vetor @p medidas do tipo TF_DMED com as medidas do instante atual, estrutura @p prev_tf do tipo
 * TF_DPREV, recebe os parâmetros auxiliares @p instante_atual um inteiro com o instante atual, @p nmed um int com o numero de medidas
 * 
 * @param instante_atual inteiro com o instante atual do estimador de demanda 
 * @param prev_tf vetor do tipo TF_DPREV com as medidas previstas pelo NowCasting 
 * @param nmed inteiro com o número de medidas da rede
 * @param medidas estrutura do tipo TF_DMED com as medidas previstas
 */
void atualiza_dmed(int instante_atual,TF_DPREV *prev_tf, int nmed, TF_DMED *medidas);

/**
 * @brief Funcao que realiza o Now Casting de demanda
 * 
 * Esta funcao recebe como parâmetros @p grafo_tf um vetor do tipo TF_GRAFO com as informacoes sobre a rede elétrica @p barras_tf do tipo 
 * TF_DBAR com as informacoes sobre as barras da rede, @p numeroBarras_tf um inteiro com o numero de barras da rede, @p alimentador_tf , uma
 * estrutura do tipo TF_ALIMENTADOR com as informacoes sobre os alimentadores da rede elétrica, @p curvaTrafos uma estrutura do tipo TF_CURVA_TRAFO
 * com as informacoes sobre as curvas de cargas agregadas da rede, @p numeroAlimentadores inteiro com o numero total de alimentadores da rede
 * @p ramo_tf vetor do tipo TF_DRAM com as informacoes dos ramos da rede, @p Sbase double com a potência base da rede, @p interfaceNiveis_tf matriz 
 * de inteiros com as informacoes sobre as interfaces da rede, @p numeroInterfaces_tf inteiro com o numero de interfaces da rede, @p areasMedicao_tf
 * estrutura do tipo TF_AREA_MEDICAO com as informacoes sobre as áreas de medicao, @p prev_tf vetor do tipo TF_DPREV com os dados de previsao 
 * @p medidaPrev_tf do tipo  TF_DMED com as medidas previstas, @p numeroMedidasTabela matriz de inteiros com os tipos de medida, @p numeroAmostras
 * inteiro com o numero de previsoes de medida
 * 
 * Returna os dados compilados em uma estrutura do tipo TF_NCRESULT
 * 
 * @param grafo_tf vetor do tipo TF_GRAFO com as informações elétricas da rede trifasica
 * @param barras_tf estrutura do tipo TF_DBAR com as informacoes sobre as barras da rede
 * @param numeroBarras_tf inteiro com o numero de barras da rede 
 * @param alimentador_tf vetor de estrutura do tipo TF_ALIMENTADOR com as informacoes sobre os alimentadores 
 * @param curvasTrafos vetor de estrutura do tipo TF_CURVA_TRAFO com as informacoes sobre as curvas de cargas agregadas da rede
 * @param numeroAlimentadores inteiro com o numero de alimentadores da rede
 * @param ramo_tf vetor do tipo TF_DRAM com as informacoes sobre os ramos da rede
 * @param Sbase double com a potência base da rede
 * @param interfaceNiveis_tf matriz de inteiros com as interfaces da rede
 * @param numeroInterfaces_tf inteiro com o numero de interfaces
 * @param areasMedicao_tf vetor de estrutura do tipo TF_AREA_MEDICAO com as informacoes sobre as areas de medicao da rede
 * @param prev_tf vetor de estrutura do tipo TF_DPREV com os dados de previsao de medidas
 * @param medidaPrev_tf estrutura do tipo TF_DMED utilizada pra comportar as medidas previstas
 * @param numeroMedidasTabela matriz de inteiros com o numero de medidas e os tipos
 * @param numeroAmostras inteiro com o numero de amostras
 * @return TF_NCRESULT* retorna os dados compilados do nowcasting de demanda
 */
TF_NCRESULT NowCastingDemanda(TF_GRAFO *grafo_tf, TF_DBAR *barras_tf ,long int numeroBarras_tf, TF_ALIMENTADOR *alimentador_tf, TF_CURVA_TRAFO *curvasTrafos, 
long int numeroAlimentadores, TF_DRAM *ramo_tf,double Sbase, long int **interfaceNiveis_tf,long int numeroInterfaces_tf, TF_AREA_MEDICAO *areasMedicao_tf, TF_DPREV *prev_tf, 
TF_DMED *medidaPrev_tf ,long int **numeroMedidasTabela, int numeroAmostras);


/**
 * @brief Funcao que compila os resultados do Now casting no instante atual 
 * 
 * Recebe como parâmetros @p grafo_tf vetor do tipo TF_GRAFO com as informacoes sobre a rede elétrica trifasica, @p numeroBarras_tf inteiro
 * com o numero de barras na rede, @p instante_atual inteiro com o instante atual, @p result vetor do tipo  TF_NCRESULT com os dados compilados da rede elétrica
 * 
 * 
 * @param grafo_tf 
 * @param numeroBarras_tf 
 * @param instante_atual 
 * @param result 
 */
void preenche_result_NC(TF_GRAFO *grafo_tf, int numeroBarras_tf, int instante_atual,TF_NCRESULT * result);

/**
 * @brief Funcao que imprime um arquivo de texto com o o DBAR de acordo com as cargas no grafo da rede trifásica
 * 
 * Recebe com parâmetro @p grafo_tf com as informacoes sobre a rede elétrias, @p numerobarras_tf com o numero de barras totais da rede 
 * trifasica, @p ts uma string com o nome do arquivio que será salvo
 * 
 * @param grafo_tf vetor do tipo TF_GRAFO com as informações elétricas da rede trifasica
 * @param numerobarras_tf inteiro com o numero de barras da rede
 * @param ts string com o sufixo do nome do DBAR
 */
void imprimeDBAR_cargas(TF_GRAFO *grafo_tf, int numerobarras_tf, const char *ts);


#endif