/* 
 * File:   Leitura_Dados.h
 * Author: Julio Massignan
 *
 * Created on 24 de Fevereiro de 2017, 14:03
 */

#ifndef funcoesCalculoEletrico_TF_H
#define	funcoesCalculoEletrico_TF_H

//------------------------------------------------------------------------------
//Funções auxiliares
//
//------------------------------------------------------------------------------
/**
 * @brief Função auxiliar para cálculo de tensão de linha (tensão fase-fase)
 * 
 * Essa função realiza o cálculo de tensão de linha trifásicas (tensão complexa fase-fase) a partir das tensões de fase (tensão complexa fase-neutro).
 * Recebe como parâmetro de entrada a tensão complexa de fase @p Vf e o parâmetro de entrada e saída da tensão de linha @p Vl . Considera a sequência
 * de fase abc. 
 * A função retorna @c void .
 * 
 * @param *Vf ponteiro com as tensões complexas fase-neutro
 * @param *Vl ponteiro com as tensões complexas fase-fase calculadas
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void tensaoDelta(__complex__ double *Vf, __complex__ double *Vl);

//------------------------------------------------------------------------------
//Funções das grandezas elétricas
//
//------------------------------------------------------------------------------
/**
 * @brief Função auxiliar para cálculo do fluxo de potência complexa em determinado ramo da rede elétrica no sentido da barra k para a barra m  (k -> m)
 * 
 * Essa função realiza o cálculo do fluxo de potência complexa em determinado ramo @p ramo da rede elétrica no sentido da barra k @p noP 
 * para a barra m @p noS (k -> m), retornando a potência complexa trifásica (ativa e reativa por fase) no vetor @p S .
 * A função retorna @c void .
 * 
 * @param *noP ponteiro para a barra inicial do sentido de fluxo de potência
 * @param *noS ponteiro para a barra final do sentido de fluxo de potência
 * @param *ramo ponteiro para o ramo sendo calculado o fluxo de potênca
 * @param *s vetor com as potências complexas (ativa e reativa) do fluxo entre a barra k (noP) e barra m (noS)
 * @return void
 * @see Sk
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void Skm(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *S);

/**
 * @brief Função auxiliar para cálculo do fluxo de potência complexa em determinado ramo da rede elétrica no sentido da barra m para a barra k  (m -> k)
 * 
 * Essa função realiza o cálculo do fluxo de potência complexa em determinado ramo @p ramo da rede elétrica no sentido da barra m @p noS
 * para a barra m @p noP (m -> k), retornando a potência complexa trifásica (ativa e reativa por fase) no vetor @p S .
 * A função retorna @c void .
 * 
 * @param *noP ponteiro para a barra final do sentido de fluxo de potência
 * @param *noS ponteiro para a barra inicial do sentido de fluxo de potência
 * @param *ramo ponteiro para o ramo sendo calculado o fluxo de potênca
 * @param *s vetor com as potências complexas (ativa e reativa) do fluxo entre a barra m (noS) e barra k (noP)
 * @return void
 * @see Sk
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void Smk(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *S);

/**
 * @brief Função auxiliar para cálculo da injeção de potência complexa em determinado nó da rede elétrica
 * 
 * Essa função realiza o cálculo da injeção de potência complexa em determinado nó indicado por @p k da rede elétrica @p grafo atraveś do somatório
 * dos fluxos de potência para ramos adjacentes, retornando a potência complexa trifásica (ativa e reativa por fase) no vetor @p S .
 * A função retorna @c void .
 * 
 * @param *grafo estrutura de dados com os dados da rede elétrica
 * @param k identificador do nó a ser calculada a injeção de potência líquida
 * @param *S vetor com as potências complexas (ativa e reativa) líquidas na barra k
 * @return void
 * @see 
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void Sk(TF_GRAFO *grafo, long int k, __complex__ double *S);


void Ikm(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *Ikm);


void Imk(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *Imk);

//------------------------------------------------------------------------------
//Funções das derivadas
void dSkm(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dS, long int opt, long int i);
void dSmk(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dS, long int opt, long int i);
void dSk(TF_GRAFO *grafo, long int k, __complex__ double *dS, long int opt, long int barra, long int fase);

void dSkm_ret(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dS, long int opt, long int i);
void dSmk_ret(TF_GRAFO *noP, TF_GRAFO *noS, TF_DRAM *ramo, __complex__ double *dS, long int opt, long int i);
void dSk_ret(TF_GRAFO *grafo, long int k, __complex__ double *dS, long int opt, long int barra, long int fase);


#endif	/* funcoesCalculoEletrico_H */