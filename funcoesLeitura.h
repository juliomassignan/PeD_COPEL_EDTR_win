/**
 * @file funcoesTopologia.h
 * @author Julio Augusto Druzina Massignan
 * @date 31 jul 2020
 * @brief Cabeçalho da biblioteca para leitura de dados trifásico para análise estática de redes de distribuição
 * As funções definidas na biblioteca estão implemetadas em funcoesLeitura.c
 *
 */
#ifndef funcoesLeitura_H
#define	funcoesLeitura_H


//------------------------------------------------------------------------------
//
// FUNÇÕES DE AUXILIARES LEITURA DE DADOS
//
//------------------------------------------------------------------------------


/**
 * @brief Função auxiliar trocar vírgula por ponto em strings. 
 *
 * Essa função realiza a substituição de vírgula por ponto para auxiliar a leitura de valores numéricos lidos como string dentro do programa.
 * A função retorna @c *char.
 * 
 * @param *st ponteiro para string com caracteres a serem substituidos
 * @return char* string com vírgula substituida por ponto.
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
char* replace(char *st);

/**
 * @brief Função auxiliar para ler strings separadas por ";,\t\n\r"
 *
 * Essa função realiza a leitura de uma linha de caracteres em @p *lin separadas por ";,\t\n\r" e retorna uma string referente 
 * à coluna especificado no @p num.
 * A função retorna @c *char.
 * 
 * @param *lin ponteiro linha de carcateres separadas de acordo com os separadores ";,\t\n\r" a ser extraída a coluna especificada
 * @param num inteiro que indica a coluna a ser lida na linha de caracteres 
 * @return char* string com o campo da coluna extraída da linha completa.
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
char* getfield(char* line, int num);

/**
 * @brief Função auxiliar para ler strings separadas por ";,\t\n\r" e retornar valores inteiros
 *
 * Essa função realiza a leitura de uma linha de caracteres em @p *lin separadas por ";,\t\n\r" e retorna um inteiro referente 
 * à coluna especificado no @p num.
 * A função retorna @c int.
 * 
 * @param *lin ponteiro linha de carcateres separadas de acordo com os separadores ";,\t\n\r" a ser extraída a coluna especificada
 * @param num inteiro que indica a coluna a ser lida na linha de caracteres 
 * @return int inteiro com o campo da coluna extraída da linha completa.
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
int getfield_int(char* lin, int num);

/**
 * @brief Função auxiliar para ler strings separadas por ";,\t\n\r" e retornar valores double
 *
 * Essa função realiza a leitura de uma linha de caracteres em @p *lin separadas por ";,\t\n\r" e retorna um inteiro referente 
 * à coluna especificado no @p num.
 * A função retorna @c double.
 * 
 * @param *lin ponteiro linha de carcateres separadas de acordo com os separadores ";,\t\n\r" a ser extraída a coluna especificada
 * @param num inteiro que indica a coluna a ser lida na linha de caracteres 
 * @return double número decimal em ponto flutuante com o campo da coluna extraída da linha completa.
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
double getfield_double(char* lin, int num);

/**
 * @brief Função auxiliar retornar string referente ao tipo de ligação trifásica
 *
 * Essa função retorna uma string para ser impressa na tela ou em arquivos referente ao tipo de ligação trifásica de algum elemento 
 * da rede indicado na estrutura de dados LIGACAO @p num (1 = YN / 2 = Delta / 3 = Y / 4 - Estrela aberto / 5 - Delta aberto / 6 - Monofásico com tap central no secundário).
 * A função retorna @c char*.
 * 
 * @param num tipo de dado LIGACAO que representa  o tipo de ligação trifásica
 * @return char* string referente ao tipo de ligação.
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
char* charLigacao(LIGACAO num);

/**
 * @brief Função auxiliar retornar string referente ao número de fases
 *
 * Essa função retorna uma string para ser impressa na tela ou em arquivos referente ao número de fases trifásica de algum elemento 
 * da rede indicado na estrutura de dados FASES @p num (1=A; 2=B; 3=C; 4=AB; 5=CA; 6=BC; 7=ABC).
 * A função retorna @c char*.
 * 
 * @param num tipo de dado FASES que representa as fases conectadas de determinado elemento da rede elétrica
 * @return char* string referente ao número de fases
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
char* charFases(FASES num);

/**
 * @brief Função auxiliar retornar string referente ao tipo de grandeza elétrica mensurada
 *
 * Essa função retorna uma string para ser impressa na tela ou em arquivos referente ao tipo de grandeza elétrica mensurada de algum medidor (dados analógicos) 
 * da rede indicado no inteiro @p num  (0: Fluxo de Potência Ativa - kW
 *                                      1: Fluxo de Potência Reativa - kVAr
 *                                      2: Injeção de Potência Ativa - kW
 *                                      3: Injeção de Potência Reativa - kVAr
 *                                      4: Magnitude de Tensão - kV
 *                                      5: Ângulo de Tensão - graus
 *                                      6: Fluxo Magnitude de Corrente - A
 *                                      7: Fluxo Ângulo de Corrente) - graus
 *                                      8: Injeção Magnitude de Corrente - A
 *                                      9: Injeção Ângulo de Corrente) - graus
 * A função retorna @c char*.
 * 
 * @param num inteiro que representa o tipo de grandeza elétrica mensurada de algum medidor (dados analógicos) 
 * @return char* string referente ao tipo de grandeza elétrica mensurada 
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
char* charMedidor(long int num);

//------------------------------------------------------------------------------
//
// FUNÇÕES DE LEITURA DE DADOS DA REDE ELÉTRICA
//
//------------------------------------------------------------------------------
/**
 * @brief Função principal para a leitura de uma pasta com os dados da rede elétrica.
 *
 * Essa função realiza a leitura da pasta com os dados da rede elétrica em arquivos separados. 
 * A pasta e subpasta são indicadas no arquivo "config.txt". A função assume que o nome do arquivo é padrão da e os arquivos 
 * lidos são: DBAR.csv; DSHNT.csv; DGD.csv; DLIN.csv; DTRF.csv; DREG.csv; DSWTC.csv; DSUBESTACAP.csv; e Viniciail.csv . 
 * Todos os arquivos são opcionais de leitura exceto o DBAR.csv. Além disso o arquivo possui como separação dos dados os marcadores especificados.
 * Realiza alocação de memória para armazenar os dados da rede elétrica.
 * Recebe como parâmetros de entrada e saída um ponteiro para ponteiro do tipo DBAR @p **barras que armazena dados das barras da rede elétrica e
 * um ponteiro para ponteiro do tipo DBAR @p **barras que armazena dados das barras da rede elétrica. Além disto recebe como 
 * parâmetros de entrada e saída inteiros que indicam: @p numeroBarras a quantidade total de barras na rede; @p numeroRamos a 
 * quantidade total de ramos na rede; e @p numeroAlimentadores a quantidade total de alimentadores na rede.
 * A função retorna @c char* indicando a pasta selecionada para os arquivos serem lidos.
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * char *folder = NULL;
 * DBAR *barra = NULL;
 * DRAM *ramo = NULL; 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * @endcode
 * 
 * @param barra parâmetro de entrada e saída que armazena os dados de barras da rede elétrica
 * @param ramo  parâmetro de entrada e saída que armazena os dados de ramos da rede elétrica
 * @param numeroBarras parâmetro de entrada e saída que armazena a quantidade total de barras da rede elétrica
 * @param numeroRamos parâmetro de entrada e saída que armazena a quantidade total de ramos da rede elétrica
 * @param numeroAlimentadores parâmetro de entrada e saída que armazena a quantidade total de alimentadores da rede elétrica
 * @return char* com string referente ao endereço da pasta com os arquivos da rede elétrica lidos
 * @see leituraDBAR
 * @see leituraDSHNT
 * @see leituraDGD
 * @see leituraDLIN
 * @see leituraDTRF
 * @see leituraDREG
 * @see leituraDSWTC
 * @see leituraDSUBESTACAO
 * @see leituraVinicial
 * @note
 * @warning .
 */
char *leituraDados(DBAR **barra, DRAM **ramo, long int *numeroBarras, long int *numeroRamos, long int *numeroAlimentadores);

/**
 * @brief Função auxiliar para a leitura do arquivo DBAR.csv, referente às informações das barras e cargas. 
 *
 * Essa função realiza a leitura do arquivo DBAR.csv relativo que contém os dados relativos as barras e cargas do sistema. 
 * Também é responsável pela alocação de memória necessária para armazenar as barras e a quantidade total de barras e alimentadores.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo DBAR, @p **barras, um ponteiro do arquivo @p arquivo a ser lido e 
 * as quantidades totais de barras e alimentadores da rede elétrica.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do DBAR, onde é retornado as informações de barras e cargas.
 * @param numeroBarras quantidade total de barras
 * @param numeroAlimentadores quantidade total de alimentadores identificados por barras do tipo VTheta
 * @return void.
 * @see leituraDados
 * @note Barras que caracterizam o início de alimentadores (barras de referência dos cicruitos) são lidas neste arquivo e indicam a 
 * quantidade de alimentadores da rede para posteriormente serem associadas a alguma subestação.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDBAR(FILE *arquivo, DBAR **barras, long int *numeroBarras, long int *numeroAlimentadores);

/**
 * @brief Função auxiliar para a leitura do arquivo DSHNT.csv, referente às informações dos bancos de capacitores. 
 *
 * Essa função realiza a leitura do arquivo DSHNT.csv que contém os dados relativos aos bancos de capacitores do sistema. Armazena estes dados
 * dentro da estrutura de dados de barras, como um parâmetro interno de um determinado elemento DBAR já pré-alocado.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo DBAR, @p **barras onde são armazenados os bancos de capacitores,
 * um ponteiro do arquivo @p arquivo a ser lido e e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do DBAR, onde é retornado as informações dos bancos de capacitores.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar um banco de capacitor a ela.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDSHNT(FILE *arquivo, DBAR **barras, long int *numeroBarras);

/**
 * @brief Função auxiliar para a leitura do arquivo DGD.csv, referente às informações dos geradores distribuídos. 
 *
 * Essa função realiza a leitura do arquivo DGD.csv que contém os dados relativos aos geradores distribuídos do sistema. Armazena estes dados
 * dentro da estrutura de dados de barras, como um parâmetro interno de um determinado elemento DBAR já pré-alocado.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo DBAR, @p **barras onde são armazenados os geradores distribuídos,
 * um ponteiro do arquivo @p arquivo a ser lido e e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do DBAR, onde é retornado as informações dos geradores distribuídos.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar um gerador distribuído a ela.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDGD(FILE *arquivo, DBAR **barras, long int *numeroBarras);

/**
 * @brief Função auxiliar para a leitura do arquivo DLIN.csv, referente às informações de circuitos trifásicos (ramais). 
 *
 * Essa função realiza a leitura do arquivo DLIN.csv que contém os dados relativos aos ciruitos trifásico do sistema. Armazena estes dados
 * dentro da estrutura de dados do tipo DLIM, como um parâmetro interno de um determinado elemento DRAM (que indica um ramo da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as ramos referentes aos circuitos e a quantidade total de ramos da 
 * rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo DRAM, @p **ramos onde são armazenados dados de circuitos trifásicos
 * internamente numa estrutura de dados do tipo DLIM, um parâmetro de entrada e saída @p numeroRamos para indicar a quantidade total de ramos, e
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo DBAR, @p **barras para associar os
 * terminais de determinado ramo com as barras da rede elétrica e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param ramos é um ponteiro para o ponteiro da estrutura do tipo do DRAM, onde é retornada as informções dos ramos da rede elétrica, nesta função os circuitos são armazenados internamente em um tipo DLIN
 * @param numeroRamos retorna a quantidade total de ramos
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do DBAR para associar a conectividade dos ramos às barras da rede elétrica.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDLIN(FILE *arquivo, DRAM **ramos, long int *numeroRamos, DBAR **barras, long int *numeroBarras);

/**
 * @brief Função auxiliar para a leitura do arquivo DTRF.csv, referente às informações de transormadores de potência. 
 *
 * Essa função realiza a leitura do arquivo DTRF.csv que contém os dados relativos aos trasnformadores de potência do sistema. Armazena estes dados
 * dentro da estrutura de dados do tipo DTRF, como um parâmetro interno de um determinado elemento DRAM (que indica um ramo da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as ramos referentes aos trnasofrmadores e a quantidade total de ramos da 
 * rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo DRAM, @p **ramos onde são armazenados dados de transformadores
 * internamente numa estrutura de dados do tipo DTRF, um parâmetro de entrada e saída @p numeroRamos para indicar a quantidade total de ramos, e
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo DBAR, @p **barras para associar os
 * terminais de determinado ramo com as barras da rede elétrica e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param ramos é um ponteiro para o ponteiro da estrutura do tipo do DRAM, onde é retornada as informções dos ramos da rede elétrica, nesta função os transformadores são armazenados internamente em um tipo DTRF
 * @param numeroRamos retorna a quantidade total de ramos
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do DBAR para associar a conectividade dos ramos às barras da rede elétrica.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDTRF(FILE *arquivo, DRAM **ramos, long int *numeroRamos, DBAR **barras, long int *numeroBarras);

/**
 * @brief Função auxiliar para a leitura do arquivo DREG.csv, referente às informações de reguladores de tensão. 
 *
 * Essa função realiza a leitura do arquivo DREG.csv que contém os dados relativos aos reguladores de tensão do sistema. Armazena estes dados
 * dentro da estrutura de dados do tipo DREG, como um parâmetro interno de um determinado elemento DRAM (que indica um ramo da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as ramos referentes aos reguladores e a quantidade total de ramos da 
 * rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo DRAM, @p **ramos onde são armazenados dados de reguladores de tensão
 * internamente numa estrutura de dados do tipo DTRF, um parâmetro de entrada e saída @p numeroRamos para indicar a quantidade total de ramos, e
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo DBAR, @p **barras para associar os
 * terminais de determinado ramo com as barras da rede elétrica e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param ramos é um ponteiro para o ponteiro da estrutura do tipo do DRAM, onde é retornada as informações dos ramos da rede elétrica, nesta função os reguladores são armazenados internamente em um tipo DREG
 * @param numeroRamos retorna a quantidade total de ramos
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do DBAR para associar a conectividade dos ramos às barras da rede elétrica.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDREG(FILE *arquivo, DRAM **ramos, long int *numeroRamos, DBAR **barras, long int *numeroBarras);

/**
 * @brief Função auxiliar para a leitura do arquivo DREG.csv, referente às informações de chaves e elementos seccionadores. 
 *
 * Essa função realiza a leitura do arquivo DSWTC.csv que contém os dados relativos às chaves e elementos seccionadores do sistema. 
 * Armazena estes dados dentro da estrutura de dados de um determinado elemento DRAM (que indica um ramo da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as ramos referentes aos reguladores e a quantidade total de ramos da 
 * rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo DRAM, @p **ramos onde são armazenados dados de chaves e seccionadores
 * na estrutura de dados do tipo DRAM, um parâmetro de entrada e saída @p numeroRamos para indicar a quantidade total de ramos, e
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo DBAR, @p **barras para associar os
 * terminais de determinado ramo com as barras da rede elétrica e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param ramos é um ponteiro para o ponteiro da estrutura do tipo do DRAM, onde é retornada as informações dos ramos da rede elétrica, nesta função as chaves e seccionadoreas são armazenados diretamente na estrutura DRAM
 * @param numeroRamos retorna a quantidade total de ramos
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do DBAR para associar a conectividade dos ramos às barras da rede elétrica.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDSWTC(FILE *arquivo, DRAM **ramos, long int *numeroRamos, DBAR **barras, long int *numeroBarras);

/**
 * @brief Função auxiliar para a leitura do arquivo Vinicial.csv, referente às informações de inicialização do cálculo de fluxo de potência.
 *
 * Essa função realiza a leitura do arquivo Vinicial.csv que contém os dados relativos às tensões complexas nodais trifásicas da rede elétrica. 
 * Armazena estes dados dentro da estrutura de dados DBAR como valores de inicialização para o cálculo de fluxo de potência.
 * Recebe como parâmetro de entrada um ponteiro do arquivo @p arquivo a ser lido, e como parâmetros de entrada e saída  um ponteiro 
 * para ponteiro do tipo DBAR, @p **barras para inicializar as tensões complexas nodais trifásicas e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do DBAR para inicializar os valores de tensões complexas nodais trifásicas.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraVinicial(FILE *arquivo, DBAR **barras, long int *numeroBarras);

/**
 * @brief Função principal para a leitura de arquivo com dados de medidas analógicas do sistema supervisório
 *
 * Essa função realiza a leitura  de arquivo com dados de medidas analógicas do sistema supervisório dentro da pasta com os dados da rede elétrica.
 * O local da pasta é recebido como parâmetro @p folder assim como o nome do arquivo no parâmetro @p file. Além disso o arquivo possui como 
 * separação dos dados os marcadores especificados. Realiza alocação de memória para armazenar os dados de medidas trifásicas da rede elétrica.
 * Recebe como parâmetros de entrada e saída um ponteiro para ponteiro do tipo DMED @p **medidas que armazena dados das medidas da rede elétrica, e também
 * uma estrutura de dados GRAFO @p grafo para associar os medidores à topologia da rede elétrica. 
 * Recebe como parâmetros de entrada um ponteiro do tipo DBAR @p *barras e um ponterio do tipo DRAM @p ramos que armazenam dados das barras e ramos da 
 * rede elétrica para serem associadas com os medidore, e respectivas quantidades totais nos parâmetros @p numeroBarras e @p numeroRamos. Além disto recebe 
 * como parâmetro de entrada a Potência Base da rede elétrica para realizar os cálculos em pu.
 * A função retorna @c char* indicando a pasta selecionada para os arquivos serem lidos.
 * 
 * Para utilizar a função:
 * @code
* long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * double Sbase = 10000;
 * char *folder = NULL;
 * long int **numeroMedidas = NULL;
 * DBAR *barraExemplo = NULL;
 * DRAM *ramoExemplo = NULL; 
 * GRAFO *grafoExemplo = NULL;
 * DMED *medidaExemplo = NULL;
 * 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * geraGrafo(&grafoExemplo, barraExemplo, numeroBarras,ramoExemplo,numeroRamos); 
 * numeroMedidas = leituraMedidas(folder, "DMED.csv", &medidaExemplo, ramoExemplo, numeroRamos, barraExemplo, numeroBarras, grafoExemplo, Sbase); 
 * @endcode
 * 
 * @param folder endereço da pasta para o arquivo de medidas ser lido
 * @param file  nome do arquivo de medidas a ser lido (default = DMED.csv)
 * @param medidas estrutura de dados para armazenar os dados de medidas analógicas da rede elétrica
 * @param ramos  parâmetro de entrada e saída que armazena os dados de ramos da rede elétrica
 * @param numeroRamos parâmetro de entrada e saída que armazena a quantidade total de ramos da rede elétrica
 * @param barras parâmetro de entrada e saída que armazena os dados de barras da rede elétrica
 * @param numeroBarras parâmetro de entrada e saída que armazena a quantidade total de barras da rede elétrica
 * @param grafo estrutura topológica da rede elétrica para armazenar a condição da rede a ser analisada
 * @param Sbase potência base da rede elétrica para realizar os cálculos em pu
 * @return long int** matriz com quantidade total de medidas por tipo e fase instaladas
 * @see leituraDados
 * @see funcoesTopologia.h  /  geraGrafo
 * @note Dados da rede elétrica e um grafo da rede elétrica devem ser criados antes de se chamar esta função. 
 * @warning .
 */
long int **leituraMedidas(char *folder,char *file, DMED **medidas, DRAM *ramos, long int numeroRamos, DBAR *barras, long int numeroBarras, GRAFO *grafo, double Sbase);


//------------------------------------------------------------------------------
//
// FUNÇÕES DE IMPRESSÃO DE DADOS DA REDE ELÉTRICA
//
//------------------------------------------------------------------------------
/**
 * @brief Função auxiliar para salvar dados da rede elétrica em arquivo externo "dadosRedeEletrica.dad"
 *
 * Essa função salva um arquivo de texto "dadosRedeEletrica.dad" contendo as principais informações da rede elétrica para conferência 
 * de informaçÕes e validações do processamento de dados da rede elétrica.
 * A função retorna @c void.
 * 
 * @param barras estrutura de dados com informações das barras a serem salvas
 * @param numeroBarras quantidade total de barras
 * @param ramos estrutura de dados com informações dos ramos a serem salvas
 * @param numeroRamos quantidade total de medidas
 * @param medidas estrutura de dados com informações das medidas a serem salvas
 * @param numeroMedidas matriz com quantidade total de medidas por tipo e fase instaladas
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void salvaDadosRedeEletrica(DBAR *barras, long int numeroBarras, DRAM *ramos, long int numeroRamos, DMED *medidas, long int **numeroMedidas);

/**
 * @brief Função auxiliar para salvar dados da rede elétrica em arquivo externo "dadosRedeEletrica.dad"
 *
 * Essa função salva um arquivo de texto "medidasRedeEletrica.dad" contendo as principais informações das medidas analógicas para conferência 
 * de informaçÕes e validações do processamento de dados dos sistemas supervisórios.
 * A função retorna @c void.
 * 
 * @param medidas estrutura de dados com informações das medidas a serem salvas
 * @param numeroMedidas matriz com quantidade total de medidas por tipo e fase instaladas
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void salvaMedidasRedeEletrica(DMED *medidas, long int **numeroMedidas);

/**
 * @brief Função auxiliar para liberar memória da estrutura DBAR
 *
 * Essa função realiza a liberação de memória de forma apropriada para a estrutura DBAR.
 * A função retorna @c void .
 * 
 * @param barra ponteiro para DBAR a ser liberado o espaço de memória
 * @param numeroBarras quantidade total de barras
 * @return void
 * @see
 * @note 
 * @warning
 */
void free_BARRA(DBAR *barra, long int numeroBarras);

/**
 * @brief Função auxiliar para liberar memória da estrutura DRAM
 *
 * Essa função realiza a liberação de memória de forma apropriada para a estrutura DRAM.
 * A função retorna @c void .
 * 
 * @param ramos ponteiro para DRAM a ser liberado o espaço de memória
 * @param numeroRamos quantidade total de ramos
 * @return void
 * @see
 * @note 
 * @warning
 */
void free_DRAM(DRAM *ramos, long int numeroRamos);

/**
 * @brief Função auxiliar para liberar memória da estrutura GRAFO
 *
 * Essa função realiza a liberação de memória de forma apropriada para a estrutura GRAFO.
 * A função retorna @c void .
 * 
 * @param grafo ponteiro para GRAFO a ser liberado o espaço de memória
 * @param numeroBarras quantidade total de barras
 * @return void
 * @see
 * @note 
 * @warning
 */
void free_GRAFO(GRAFO *grafo, long int numeroBarras);

/**
 * @brief Função auxiliar para liberar memória da estrutura ALIMENTADOR e RNPs
 *
 * Essa função realiza a liberação de memória de forma apropriada para a estrutura ALIMENTADOR e RNPs.
 * A função retorna @c void .
 * 
 * @param alimentador ponteiro para ALIMENTADOR a ser liberado o espaço de memória
 * @param numeroAlimentadores quantidade total de alimentadores
 * @return void
 * @see
 * @note 
 * @warning
 */
void free_ALIMENTADOR(ALIMENTADOR *alimentadores, long int numeroAlimentadores);





long int **leituraDINTERSE(char *folder,char *file, long int *numeroInterfaces, DBAR **barras, long int *numeroBarras, DRAM **ramos, long int *numeroRamos);
void includeDREG(DRAM **ramos, long int *numeroRamos, int DE, int PARA);
void includeDBAR(DBAR **barras, long int *numeroBarras,  double Vbase);
void includeDTRF(DRAM **ramos, long int *numeroRamos,  int DE, int PARA, double Vpri, double Vsec, int lig_pri, int lig_sec);

#endif	/* funcoesLeitura_H */

