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

void constroi_dmed_prev(TF_DPREV *prev_tf, int nmed,TF_DMED ***medidas);
void constroi_dmed_prev2(TF_DPREV *prev_tf, int nmed,TF_DMED **medidas);
void atualiza_dmed(int instante_atual,TF_DPREV *prev_tf, int nmed);

#endif