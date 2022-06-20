/**
 * @file funcoesTopologia_tf.h
 * @author Julio Augusto Druzina Massignan
 * @date 31 jul 2020
 * @brief Cabeçalho da biblioteca para o cálculo de fluxo de potência trifásico por Varredura Direta/Inversa por soma de correntes.
 * As funções definidas na biblioteca estão implemetadas em funcoesFluxoVarredura.c
 *
 */
#ifndef FUNCOESFLUXOVARREDURA_TF_H
#define	FUNCOESFLUXOVARREDURA_TF_H

//------------------------------------------------------------------------------
//
// FUNÇÕES DE IMPRESSÃO DE DADOS
//
//------------------------------------------------------------------------------
/**
 * @brief Função auxiliar para imprimir na tela as tensões complexas nodais trifásicas
 * 
 * Essa função imprime na tela as tensões complexas nodais por fase (fase-neutro) de determinada barra da rede elétrica especificada no parâmetro de
 * entrada @p grafo , imprimindo as tensões de um nó específico da rede elétrica.
 * A função retorna @c void.
 * 
 * @param *grafo ponteiro para o nó específico do grafo cujas tensões nodais trifásicas serão impressas na tela
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeTensoesNodais(TF_GRAFO *grafo);

/**
 * @brief Função auxiliar para imprimir na tela os taps dos reguladores de tensão
 * 
 * Essa função imprime na tela os taps dos reguladores de tensão (em cada fase) de determinada regulador de tensão da rede elétrica
 * adjacente à barra especificada no parâmetro de entrada @p grafo , imprimindo os taps do reguladores adjacentes
 * A função retorna @c void.
 * 
 * @param *grafo ponteiro para o nó específico do grafo cujas taps de reguladores adjacentes serão impressos
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeTaps(TF_GRAFO *grafo);

/**
 * @brief Função auxiliar para imprimir na tela as injecṍes de corrente complexas nodais trifásicas
 * 
 * Essa função imprime na tela as injeções de corrente complexas nodais por fase (fase-neutro) de determinada barra da rede elétrica especificada 
 * no parâmetro de entrada @p grafo , imprimindo as injeções de corrente de um nó específico da rede elétrica.
 * A função retorna @c void.
 * 
 * @param *grafo ponteiro para o nó específico do grafo cujas injeções de corrente nodais trifásicas serão impressas na tela
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeiInjecoesCorrentes(TF_GRAFO *grafo);

/**
 * @brief Função auxiliar para imprimir na tela as correntes complexas trifásicas em determinado ramo da rede elétrica
 * 
 * Essa função imprime na tela as injeções de correntes complexas trifásicas por fase de determinada ramo da rede elétrica especificada 
 * no parâmetro de entrada @p noAdj , proveniente da estrutura do tipo NOADJCENTE, imprimindo as correntes que fluem para um nó específico de seu nó a montante.
 * A função retorna @c void.
 * 
 * @param *noAdj ponteiro para o o nó adjacente a jusante cuja corrente complexa trifásica do ramo será impressa
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeCorrentes(TF_NOADJACENTE *noAdj);


//------------------------------------------------------------------------------
//
// FUNÇÕES PARA O CÁLCULO DE FLUXO DE POTÊNCIA
//
//------------------------------------------------------------------------------
/**
 * @brief Função auxiliar para inicializar as tensões complexas nodais trifásicas de todos os alimentadores igual à tensão do nó raiz
 * 
 * Essa função inicializa as tensões complexas nodais por fase (fase-neutro) de todas as barras de todos os alimentadores da rede elértrica. 
 * O valor de inicilização é igual à tensão complexa nodal do nó raiz de cada alimentador. Recebe como parâmetros de entrada e saída o grafo da
 * rede elétrica @p grafo com as tensṍes complexas inicializadas e como parâmertros de entrada os alimentadores @p alimentadores e quantidades
 * totais de barras e alimentadores da rede elétrica @p numeroBarras e @p numeroAlimentadores ,respectivamente. 
 * A função retorna @c void.
 * 
 * @param *grafo ponteiro para o grafo da rede elétrica para inicilizar as tensões nodais
 * @param numeroBarras quatidade total de barras
 * @param *alimentadores ponteiro par os alimentadores da rede elétrica
 * @param numeroAlimentadores quantidade total de alimentadores
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void incializaTensoesRaiz(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores);

/**
 * @brief Função auxiliar para inicializar as tensões complexas nodais trifásicas de todos os alimentadores através de um processo de varredura direta
 * 
 * Essa função inicializa as tensões complexas nodais por fase (fase-neutro) de todas as barras de todos os alimentadores da rede elértrica. 
 * O valor de inicilização é obtido através de uma varredura direta partindo do nó raiz de cada alimentador para compensar possíveis valores de tap
 * fora do nominal (no caso de reguladores e transformadores). Recebe como parâmetros de entrada e saída o grafo da
 * rede elétrica @p grafo com as tensões complexas inicializadas e como parâmertros de entrada os alimentadores @p alimentadores e quantidades
 * totais de barras e alimentadores da rede elétrica @p numeroBarras e @p numeroAlimentadores ,respectivamente. 
 * A função retorna @c void.
 * 
 * @param *grafo ponteiro para o grafo da rede elétrica para inicilizar as tensões nodais
 * @param numeroBarras quatidade total de barras
 * @param *alimentadores ponteiro par os alimentadores da rede elétrica
 * @param numeroAlimentadores quantidade total de alimentadores
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void incializaTensoesVarredura(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores);

/**
 * @brief Função auxiliar que realiza a atualização dos valores de taps nas matrizes do quadripólo de admitâncias trifásico de regulador de tensão específico
 * 
 * Esta função atualiza as matrizes do quadripólo de admitâncias de um ramo específico @p ramos de acordo com os valores dos taps indicados 
 * dentro da estrutura de dados de reguladores de tensão deste ramo. Pode ser utilizada para atualizar os taps conforme
 * novas informações discretas são obtidas pelo sistema supervisório e conforme atualizações do controladores são calculadas automaticamente.
 * A função retorna @c void .
 * 
 *   
 * @param ramo ramo específico da rede elétrica cuja valor de tap do regulador foi atualizado
 * @return void
 * @see controleReguladorTensao_LDC
 * @see montaQuadripoloRegulador
 * @note 
 * @warning
 */
void atualizaTapRegulador(TF_DRAM *ramo);

/**
 * @brief Função auxiliar para atualizar as injecṍes de corrente complexas nodais trifásicas em determinado nó conforme as cargas, shunts e geradores distribuídos
 * 
 * Essa função atualiza as injeções de corrente complexas nodais por fase (fase-neutro) de determinada barra da rede elétrica especificada 
 * no parâmetro de entrada @p no , através da tensão complexa nodal atualizada e dos valores de potência e modelos dependentes de tensão de 
 * cargas, bancos de capacitores (shunts) e geradores distribuídos.
 * A função retorna @c void.
 * 
 * @param *no ponteiro para o nó específico do grafo cujas injeções de corrente nodais trifásicas serão atualizadas
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void atualizaInjecoes(TF_GRAFO *no);

/**
 * @brief Função auxiliar para atualizar as tensões nodais complexas na etapa Forward do cálculo de fluxo de potência por VDI
 * 
 * Essa função atualiza as tensões complexas nodais por fase (fase-neutro) partindo da tensão complexa trfiásica da barra a montante @p Vp 
 * para atualizar a tensão da barra a jusante @p Vs . O cálculo é realizado de acordo com a etapa Forward do cálculo de Fluxo de Potência 
 * por Varredura Direta/Inversa através da corrente complexa que flui da barra "p" para a barra "s" @p Ips e do modelo de quadripólo de admitância
 * trifásico do respectivo ramo ps @p ramo .
 * A função retorna @c void.
 * 
 * @param Vp tensões complexas nodais trifásica na barra a montante
 * @param Vs tensões complexas nodais trifásica na barra a jusante a ser atualizada
 * @param Ips correntes complexas trifásicas no ramo entre a barra a montante e barra a jusante
 * @param ramo estrutura de dados com informações do ramo que conecta as duas barras e respectivo modelo elétrico
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void calculaTensaoJusante(complex double *Vp, complex double *Vs, complex double *Ips, TF_DRAM *ramo);

/**
 * @brief Função auxiliar para calcular as perdas elétrica em deteminado ramo da rede elétria através de varredura backward
 * 
 * Essa função calcula as perdas elétricas trifásicas no ramo a montante de um determinado nó da rede elétrica @p noP . É utilizada em conjunto com
 * uma varredura Backward para calcular as perdas elétricas em um alimentador completo utilizando informações da barra a montante obtida pelo grafo
 * @p grafo da rede elétrica. Retorna a soma das perdas de potência ativa nas três fases (perdas totais do ramo e não por fase).
 * A função retorna @c double.
 * 
 * @param noP barra especificada para as perdas a montante serem calculadas
 * @param grafo grafo da rede elétrica para busca do nó a montante
 * @return double perdas trifásicas totais (soma das três fases) no ramo a montante da barra indicada em noP
 * @see fluxoPotencia_BFS_Alimentador
 * @note Esta função é específica para ser utilizada em conjunto com uma varredura inversa (backward) para calcular as perdas em alimentadores específicos
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
double calculaPerdas(TF_GRAFO *noP, TF_GRAFO *grafo);

/**
 * @brief Função auxiliar para calcular o carregamento em deteminado ramo da rede elétria através de varredura backward
 * 
 * Essa função calcula o carregamento percentual trifásico no ramo a montante de um determinado nó da rede elétrica @p noP . É utilizada em 
 * conjunto com uma varredura Backward para calcular o carregamento dos ramos em um alimentador completo utilizando informações da barra a montante 
 * obtida pelo grafo @p grafo da rede elétrica. Retorna o maior carregamento entre as três fases no caso de cabos através do valor de ampacidade,
 * e no caso de transformadores e reguladores o carregamento nominal trfásico através da potência nominal.
 * A função retorna @c double.
 * 
 * @param noP barra especificada para o carregamento a montante ser calculado
 * @param grafo grafo da rede elétrica para busca do nó a montante
 * @param Sbase potência base da rede elétrica
 * @return double carregamento percentual no ramo a montante da barra indicada em noP
 * @see fluxoPotencia_BFS_Alimentador
 * @note Esta função é específica para ser utilizada em conjunto com uma varredura inversa (backward) para calcular o carregamento da rede em alimentadores específicos
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
double calculaCarregamento(TF_GRAFO *noP, TF_GRAFO *grafo, double Sbase);

/**
 * @brief Função auxiliar para efetuar os cálculos individuais em cada ramo da rede elétrica da Varredura Inversa (Etapa Backward) do cálculo de Fluxo de Potência
 * 
 * Essa função efetua os cálculos elétricos da etapa Backward (varredura inversa) no cálculo do fluxo de potência. Atualiza a corrente no ramo
 * a montante de um determinado nó da rede elétrica @p noP . É utilizada em conjunto com uma varredura Backward para calcular a corrente complexa trifásica
 * dos ramos em um alimentador completo utilizando informações da barra a montante obtida pelo grafo @p grafo da rede elétrica. 
 * Atualiza dentro da estrutrua de dados NOADJCAENTE do grafo os valores de corrente do ramo a montante do noP.
 * A função retorna @c void.
 * 
 * @param noP barra especificada para a corrente a montante ser calculada
 * @param grafo grafo da rede elétrica para busca do nó a montante e atualizar a corrente do ramo
 * @return void
 * @see fluxoPotencia_BFS_Alimentador
 * @note Esta função é específica para ser utilizada em conjunto com uma varredura inversa (backward) 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void backward(TF_GRAFO *noP, TF_GRAFO *grafo);

/**
 * @brief Função auxiliar para o controle automático de taps em reguladores de tensão no cálculo de fluxo de potência
 * 
 * Essa função efetua o cálculo de atualizações de taps em reguladores de tensão com comutação de tap sob carga (OLTC) para controle de tensão em sistemas
 * de distribuição. Atualiza durante a Varredura Direta o valor de tap conforme o modelo detalhado do controlador de comutação com representação do
 * compensador de queda na linha (LDC) em diferentes modos de controle: (0 = Somente Forward sem Restrição / 1 = Locked Forward / 2 = Locked Reverse / 
 * 3 = Bidirectional / 4 = Idle / 5 = Neutral Reverse / 6 = Cogenartion / Ver manual Siemens MJ4A). Recebe como parâmetros de entrada as tensões complexas
 * nodais trifásicas nos terminais do regulador @p Vp e @p Vs , as correntes nos terminais do regulador @p Ips e @p Isp , os valores de tensão
 * base @p Vbase e corrente base @p Ibase para representar a transdução do sensores do controlador, e a estrutura de dados com @p ramos que aramazena
 * internamente em um tipo DREG as informações detalhadas do controlador e respectivo regulador de tensão.
 * Detalhes deste modelo ver referência: 
 * J. A. D. Massignan, B. R. Pereira and J. B. A. London, "Load Flow Calculation with Voltage Regulators Bidirectional Mode and Distributed 
 * Generation," in IEEE Transactions on Power Systems, vol. 32, no. 2, pp. 1576-1577, March 2017, doi: 10.1109/TPWRS.2016.2576679.
 * Retorna um booleano indicando se houve alteração de controladores, no caso de reguladores de tensão com tap automático.
 * A função retorna @c BOOL.
 * 
 * @param Vbase tensão base do regulador de tensão
 * @param Ibase corrente base do regulador de tensão
 * @param Vp tensão complexa nodal trifásica no terminal a montante do regulador
 * @param Vs tensão complexa nodal trifásica no terminal a jusante do regulador
 * @param Ips corrente complexa trifásica no terminal a montante do regulador
 * @param Isp corrente complexa trifásica no terminal a jusante do regulador
 * @param ramo estrutura de dados com informações dos parâmetros e controlador do regulador de tensão
 * @return BOOL indicador de alteração em controladores de tensão
 * @see forward
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL controleReguladorTensao_LDC(double Vbase, double Ibase, complex double *Vp, complex double *Vs, complex double *Ips, complex double *Isp, TF_DRAM *ramo);

/**
 * @brief Função auxiliar para efetuar os cálculos individuais em cada ramo da rede elétrica da Varredura Direta (Etapa Forward) do cálculo de Fluxo de Potência
 * 
 * Essa função efetua os cálculos elétricos da etapa Forward (varredura direta) no cálculo do fluxo de potência. Atualiza a tensão complexa nodal nos nós 
 * a jusante de um determinado nó da rede elétrica @p noP . É utilizada em conjunto com uma varredura Forward para calcular as tensões complexas trifásica
 * dos ramos em um alimentador completo utilizando informações das barras a jusante obtida pelo grafo @p grafo da rede elétrica. 
 * Atualiza atraveś da estrutrua de dados NOADJCAENTE do grafo os valores de tensão complexa armazenados no TF_GRAFO @p grafo . Retorna um booleano
 * indicando se houve alteração de controladores, no caso de reguladores de tensão com tap automático.
 * A função retorna @c BOOL.
 * 
 * @param noP barra especificada a montante para atualizar as tensões complexas nodais a jusante 
 * @param grafo grafo da rede elétrica para busca dos nós a jusante para atualizar as tensões
 * @return BOOL indicador de alteração em controladores de tensão
 * @see fluxoPotencia_BFS_Alimentador
 * @note Esta função é específica para ser utilizada em conjunto com uma varredura direta (forward) 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL forward(TF_GRAFO *noP, TF_GRAFO *grafo);


void incializaTensoesVarredura(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores);
    
//------------------------------------------------------------------------------
//
// FUNÇÕES DE LEITURA DE DADOS
//
//------------------------------------------------------------------------------
TF_PFSOLUTION fluxoPotencia_BFS_Alimentador(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR alimentador, TF_DRAM *ramos,double Sbase);
void fluxoPotencia_BFS_Multiplos(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos,double Sbase);


void fluxoPotencia_Niveis_BFS_Multiplos(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos,double Sbase, long int **interfaceNiveis,long int numeroInterfaces, BOOL opt_flow);
TF_PFSOLUTION fluxoPotencia_BFS_Alimentador_IteracaoUnica(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR alimentador, TF_DRAM *ramos,double Sbase);
void amostragemVariacaoCarga(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, double percentual);

double desbalancoFasorialSeq(__complex__ double *Fasor);


#endif	/* FUNCOESFLUXOVARREDURA_H */

