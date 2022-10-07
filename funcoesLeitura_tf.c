/**
 * @file funcoesLeituraDados.c
 * @author Julio Augusto Druzina Massignan
 * @date 31 jul 2020
 * @brief Contém a implementação das funções utilizadas para a leitura dos dados de redes elétricas trifásicas.
 * Este arquivo implementa as funções definidas na biblioteca funcoesLeitura.h
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <complex.h>
#include <ctype.h>

#include "data_structures_tf.h"
#include "funcoesLeitura_tf.h"

//------------------------------------------------------------------------------
//
// FUNÇÕES DE LEITURA DE DADOS
//
//------------------------------------------------------------------------------
// Troca vírgula por ponto em string
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
char *replace(char *st)
{
    int i = 0;
    while (st[i] != '\0')
    {
        if (st[i] == ',')
            st[i] = '.';
        i++;
    }
    return st;
}

// Lê cada campo de uma string separada por vírgula
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
char *getfield(char *lin, int num)
{
    char *tok = NULL;
    char *line = NULL;
    //    char *def = "0";
    line = strdup(lin);
    // printf("\nteste -  %s",line);
    tok = strtok(line, ";,\t\n\r");
    while (tok != NULL)
    {
        if (!--num)
        {
            return tok;
        }
        tok = strtok(NULL, ";,\t\n\r");
    }
    return NULL;
}

// Lê cada campo de uma string separada por vírgula - para inteiro
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
int getfield_int(char *lin, int num)
{
    char *tok = NULL;
    char *line = NULL;
    int field;
    line = strdup(lin);
    // line = strcpy(line,lin);
    // printf("\nteste -  %s",line);
    tok = strtok(line, " ,;\t\n\r");
    while (tok != NULL)
    {
        if (!--num)
        {
            field = atoi(tok);
            free(line);
            return field;
        }
        tok = strtok(NULL, " ;,\t\n\r");
    }
    free(line);
    return (0);
}

// Lê cada campo de uma string separada por vírgula - para double
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
double getfield_double(char *lin, int num)
{
    char *tok = NULL;
    char *line = NULL;
    double field;
    line = strdup(lin);
    // line = strcpy(line,lin);
    // printf("\nteste -  %s",line);
    tok = strtok(line, " ,;\t\n\r");
    while (tok != NULL)
    {
        if (!--num)
        {
            field = atof(tok);
            free(line);
            return field;
        }
        tok = strtok(NULL, " ,;\t\n\r");
    }
    free(line);
    return NAN;
}

// Retorna string referente ao tipo de ligacao trifásica
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
char *charLigacao(TF_LIGACAO num)
{
    char *tok = (char *)malloc(2 * sizeof(char));
    switch (num)
    {
    case O:
        strcpy(tok, "O");
        return (tok);
        break;
    case YN:
        strcpy(tok, "Yn");
        return (tok);
        break;
    case Y:
        strcpy(tok, "Y");
        return (tok);
        break;
    case D:
        strcpy(tok, "D");
        return (tok);
        break;
    case OY:
        strcpy(tok, "OY");
        return (tok);
        break;
    case OD:
        strcpy(tok, "OD");
        return (tok);
        break;
    }
}

// Retorna string referente ao número de fases
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
char *charFases(TF_FASES num)
{
    char *tok = (char *)malloc(3 * sizeof(char));
    switch (num)
    {
    case N:
        strcpy(tok, "N");
        return (tok);
        break;
    case A:
        strcpy(tok, "A");
        return (tok);
        break;
    case B:
        strcpy(tok, "B");
        return (tok);
        break;
    case C:
        strcpy(tok, "C");
        return (tok);
        break;
    case AB:
        strcpy(tok, "AB");
        return (tok);
        break;
    case BC:
        strcpy(tok, "BC");
        return (tok);
        break;
    case CA:
        strcpy(tok, "CA");
        return (tok);
        break;
    case ABC:
        strcpy(tok, "ABC");
        return (tok);
        break;
    }
}

// Retorna string referente ao tipo de medidor instalado na rede elétrica
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
char *charMedidor(long int num)
{
    char *tok = (char *)malloc(10 * sizeof(char));
    switch (num)
    {
    case 0:
        strcpy(tok, "Pkm");
        return (tok);
        break;
    case 1:
        strcpy(tok, "Qkm");
        return (tok);
        break;
    case 2:
        strcpy(tok, "Pk");
        return (tok);
        break;
    case 3:
        strcpy(tok, "Qk");
        return (tok);
        break;
    case 4:
        strcpy(tok, "Vk");
        return (tok);
        break;
    case 5:
        strcpy(tok, "Tk");
        return (tok);
        break;
    case 6:
        strcpy(tok, "Ikm");
        return (tok);
        break;
    case 7:
        strcpy(tok, "Akm");
        return (tok);
        break;
    case 8:
        strcpy(tok, "Vk_real");
        return (tok);
        break;
    case 9:
        strcpy(tok, "Vk_imag");
        return (tok);
        break;
    case 10:
        strcpy(tok, "Ik_real");
        return (tok);
        break;
    case 11:
        strcpy(tok, "Ik_imag");
        return (tok);
        break;
    case 12:
        strcpy(tok, "Ikm_real");
        return (tok);
        break;
    case 13:
        strcpy(tok, "Ikm_imag");
        return (tok);
        break;
    }
}

//------------------------------------------------------------------------------
//
// FUNÇÕES DE LEITURA DE DADOS DA REDE ELÉTRICA
//
//------------------------------------------------------------------------------
// Leitura de dados da rede elétrica
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
 * TF_DRAM *ramo = NULL;
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
char *leituraDados(TF_DBAR **barra, TF_DRAM **ramo, long int *numeroBarras, long int *numeroRamos, long int *numeroAlimentadores)
{
    FILE *arquivo = NULL;
    char linha[1000], *pasta, *folder, aux[1000], aux2[1000];
    //    folder = (char *)malloc(600*sizeof(char));

    printf("Leitura de dados da rede elétrica...\n");

    // Recebe o nome da pasta com os dados a serem lidos - arquivo config.txt
    FILE *config = NULL;
    config = fopen("config.txt", "r");
    if (config == NULL)
    {
        printf("Erro ao abrir arquivo config.txt !!!\n");

        getchar();
        exit(1);
    }
    fgets(linha, 1000, config);
    folder = getfield(linha, 1);
    printf("Main directory: %s \n", folder);
    fclose(config);
    strcpy(aux, folder);
    strcpy(aux2, folder);

    // Leitura dos dados de barras

    arquivo = fopen(strcat(aux, "DBAR.csv"), "r");
    if (arquivo != NULL)
    {
        leituraDBAR(arquivo, barra, numeroBarras, numeroAlimentadores);
        fclose(arquivo);
    }
    else
    {
        printf("Erro ao abrir arquivo DBAR.csv !!!\n");
        getchar();
        exit(1);
    }
    // printf("\nDBAR ok\n");

    strcpy(aux, aux2);
    arquivo = fopen(strcat(aux, "DSHNT.csv"), "r"); // Le somente se existir o arquivo
    if (arquivo != NULL)
    {
        leituraDSHNT(arquivo, barra, numeroBarras);
        fclose(arquivo);
    }
    // printf("DSHNT ok\n");

    strcpy(aux, aux2);
    arquivo = fopen(strcat(aux, "DGD.csv"), "r"); // Le somente se existir o arquivo
    if (arquivo != NULL)
    {
        leituraDGD(arquivo, barra, numeroBarras);
        fclose(arquivo);
    }
    // printf("DGDS ok\n");
    // Leitura dos dados de ramos

    strcpy(aux, aux2);
    arquivo = fopen(strcat(aux, "DLIN.csv"), "r"); // Le somente se existir o arquivo
    if (arquivo != NULL)
    {
        leituraDLIN(arquivo, ramo, numeroRamos, barra, numeroBarras);
        fclose(arquivo);
    }
    // printf("DLIN ok\n");

    strcpy(aux, aux2);
    arquivo = fopen(strcat(aux, "DTRF.csv"), "r"); // Le somente se existir o arquivo
    if (arquivo != NULL)
    {
        leituraDTRF(arquivo, ramo, numeroRamos, barra, numeroBarras);
        fclose(arquivo);
    }
    // printf("DTRF ok\n");

    strcpy(aux, aux2);
    arquivo = fopen(strcat(aux, "DREG.csv"), "r"); // Le somente se existir o arquivo
    if (arquivo != NULL)
    {
        leituraDREG(arquivo, ramo, numeroRamos, barra, numeroBarras);
        fclose(arquivo);
    }
    // printf("DREG ok\n");

    strcpy(aux, aux2);
    arquivo = fopen(strcat(aux, "DSWTC.csv"), "r"); // Le somente se existir o arquivo
    if (arquivo != NULL)
    {
        leituraDSWTC(arquivo, ramo, numeroRamos, barra, numeroBarras);
        fclose(arquivo);
    }
    // printf("DSWTC ok\n");

    strcpy(aux, aux2);
    arquivo = fopen(strcat(aux, "Vinicial.csv"), "r"); // Le somente se existir o arquivo
    if (arquivo != NULL)
    {
        leituraVinicial(arquivo, barra, numeroBarras);
        fclose(arquivo);
    }

    return (folder);
}

//------------------------------------------------------------------------------
// Leitura de dados de BARRA
/**
 * @brief Função auxiliar para a leitura do arquivo DBAR.csv, referente às informações das barras e cargas.
 *
 * Essa função realiza a leitura do arquivo DBAR.csv que contém os dados relativos as barras e cargas do sistema.
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
void leituraDBAR(FILE *arquivo, TF_DBAR **barras, long int *numeroBarras, long int *numeroAlimentadores)
{
    char blocoLeitura[2000];     /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;                 /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int contador = 0, i, aux, k; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0;    /* Variável com o número de linhas do arquivo a serem lidas. */
    double PA, PB, PC, QA, QB, QC;
    extern BOOL symetric_OPT; /* Variável externa para modelo balanceado e simétrico. */

    // Aloca na memória espaço para as barras
    while ((carac = fgetc(arquivo)) != EOF)
    {
        if (carac == '\n')
            numLinhas++;
    }
    rewind(arquivo);

    if (((*barras) = (TF_DBAR *)malloc((numLinhas + 1) * sizeof(TF_DBAR))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as barras !!!!");
        exit(1);
    }

    // Le o arquivo de curva de cargas até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        // Verifica se a barra já foi criada (Não testa mais isso - assume que o DBAR vem em ordem sequencial, caso já exista a barra vai direto pra ela)
        aux = -1;
        //    for (i=0;i<contador;i++){
        //        if ((*barras)[i].ID == (getfield_int(dados,1))){
        //            aux = i;
        //        }
        //    }

        // Barra já existente - Só adiciona a carga
        if (contador > (getfield_int(dados, 1)))
        {
            aux = (getfield_int(dados, 1));
        }
        if (aux == -1)
        { // Criando novo DBAR
            (*barras)[contador].ID = (getfield_int(dados, 1));
            (*barras)[contador].i = contador;
            (*barras)[contador].ligacao = (getfield_int(dados, 2));
            (*barras)[contador].fases = (getfield_int(dados, 3));
            (*barras)[contador].Vbase = (getfield_double(dados, 4)) / pow(3, 0.5);
            (*barras)[contador].tipo = 0;

            //(*barras)[contador].loads = (DLOAD *)malloc( 1 * sizeof(DLOAD));
            (*barras)[contador].nloads = 0;
            (*barras)[contador].shunts = NULL;
            //(*barras)[contador].shunts = (DSHNT *)malloc( 1 * sizeof(DSHNT));
            (*barras)[contador].nshunts = 0;
            //(*barras)[contador].gds = (DGD *)malloc( 1 * sizeof(DGD));
            (*barras)[contador].gds = NULL;
            (*barras)[contador].ngds = 0;

            (*barras)[contador].Vinicial[0] = 1 * (cos(0) + I * sin(0));
            (*barras)[contador].Vinicial[1] = 1 * (cos(-120 * PI / 180) + I * sin(-120 * PI / 180));
            (*barras)[contador].Vinicial[2] = 1 * (cos(120 * PI / 180) + I * sin(120 * PI / 180));

            // Leitura das Cargas
            PA = (getfield_double(dados, 5));
            PB = (getfield_double(dados, 6));
            PC = (getfield_double(dados, 7));
            QA = (getfield_double(dados, 8));
            QB = (getfield_double(dados, 9));
            QC = (getfield_double(dados, 10));

            //            Modelo Balanceado
            if (symetric_OPT)
            {
                (*barras)[contador].fases = 7;

                double P = (PA + PB + PC) / 3;
                double Q = (PA + PB + PC) / 3;
                PA = P;
                PB = P;
                PC = P;
                QA = Q;
                QB = Q;
                QC = Q;
            }

            if ((PA != 0) || (PB != 0) || (PC != 0) || (QA != 0) || (QB != 0) || (QC != 0))
            {
                // Inseri um valor de load
                (*barras)[contador].nloads++;
                /*if (((*barras)[contador].loads = (DLOAD *)realloc((*barras)[contador].loads, ((*barras)[contador].nloads +1) * sizeof(DLOAD)))==NULL)
                {
                    printf("Erro -- Nao foi possivel alocar espaco de memoria para cargas !!!!");
                    exit(1);
                }*/
                k = (*barras)[contador].nloads - 1;

                (*barras)[contador].loads[k].ID = (*barras)[contador].ID;
                (*barras)[contador].loads[k].Vbase = (*barras)[contador].Vbase;
                (*barras)[contador].loads[k].fases = (*barras)[contador].fases;
                (*barras)[contador].loads[k].lig = (*barras)[contador].ligacao;
                (*barras)[contador].loads[k].ZIP = (getfield_double(dados, 11));

                (*barras)[contador].loads[k].Pnom[0] = PA;
                (*barras)[contador].loads[k].Pnom[1] = PB;
                (*barras)[contador].loads[k].Pnom[2] = PC;
                (*barras)[contador].loads[k].Qnom[0] = QA;
                (*barras)[contador].loads[k].Qnom[1] = QB;
                (*barras)[contador].loads[k].Qnom[2] = QC;
            }

            // Leitura da Barra de Referência
            if (!isnan(getfield_double(dados, 12)))
            {
                (*barras)[contador].tipo = 2;
                numeroAlimentadores[0]++;

                double VA = (getfield_double(dados, 12));
                double VB = (getfield_double(dados, 13));
                double VC = (getfield_double(dados, 14));
                double TA = (getfield_double(dados, 15));
                double TB = (getfield_double(dados, 16));
                double TC = (getfield_double(dados, 17));
                if ((*barras)[contador].Vbase == 0)
                    (*barras)[contador].Vbase = 13800 / pow(3, 0.5);

                __real__(*barras)[contador].Vref[0] = VA * cos(TA * M_PI / 180);
                __imag__(*barras)[contador].Vref[0] = VA * sin(TA * M_PI / 180);
                __real__(*barras)[contador].Vref[1] = VB * cos(TB * M_PI / 180);
                __imag__(*barras)[contador].Vref[1] = VB * sin(TB * M_PI / 180);
                __real__(*barras)[contador].Vref[2] = VC * cos(TC * M_PI / 180);
                __imag__(*barras)[contador].Vref[2] = VC * sin(TC * M_PI / 180);

                (*barras)[contador].Vinicial[0] = VA * (cos(TA * M_PI / 180) + I * sin(TA * M_PI / 180));
                (*barras)[contador].Vinicial[1] = VB * (cos(TB * M_PI / 180) + I * sin(TB * M_PI / 180));
                (*barras)[contador].Vinicial[2] = VC * (cos(TC * M_PI / 180) + I * sin(TC * M_PI / 180));
            }
            contador++;
        }
        else
        { // Inserindo nova carga em DBAR já existente
            // Leitura das Cargas
            PA = (getfield_double(dados, 5));
            PB = (getfield_double(dados, 6));
            PC = (getfield_double(dados, 7));
            QA = (getfield_double(dados, 8));
            QB = (getfield_double(dados, 9));
            QC = (getfield_double(dados, 10));

            if ((PA != 0) || (PB != 0) || (PC != 0) || (QA != 0) || (QB != 0) || (QC != 0))
            {
                // Inseri um valor de load
                (*barras)[aux].nloads++;
                /*if (((*barras)[aux].loads = (DLOAD *)realloc((*barras)[aux].loads, (*barras)[aux].nloads * sizeof(DLOAD)))==NULL)
                {
                    printf("Erro -- Nao foi possivel alocar espaco de memoria para cargas !!!!");
                    exit(1);
                }*/
                k = (*barras)[aux].nloads - 1;
                (*barras)[aux].loads[k].ID = (*barras)[aux].ID;
                (*barras)[aux].loads[k].lig = (getfield_int(dados, 2));
                (*barras)[aux].loads[k].fases = (getfield_int(dados, 3));
                (*barras)[aux].loads[k].Vbase = (getfield_double(dados, 4));
                (*barras)[aux].loads[k].ZIP = (getfield_double(dados, 11));

                (*barras)[aux].loads[k].Pnom[0] = PA;
                (*barras)[aux].loads[k].Pnom[1] = PB;
                (*barras)[aux].loads[k].Pnom[2] = PC;
                (*barras)[aux].loads[k].Qnom[0] = QA;
                (*barras)[aux].loads[k].Qnom[1] = QB;
                (*barras)[aux].loads[k].Qnom[2] = QC;
            }
        }
    }
    numeroBarras[0] = contador;
}

//------------------------------------------------------------------------------
// Leitura de dados de SHUNTS
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
void leituraDSHNT(FILE *arquivo, TF_DBAR **barras, long int *numeroBarras)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;             /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, aux, k;           /* Variáveis contadores para percorrer o arquivo e a string de leitura. */

    // Le o arquivo de curva de cargas até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        // Verifica se a barra já foi criada - Não efetua busca nos IDs de Barras - Assume que já está pré-mapeado corretamente
        aux = -1;
        //        for (i=0;i<numeroBarras[0];i++){
        //            if ((*barras)[i].ID == (getfield_int(dados,1))){
        //                aux = i;
        //            }
        //        }
        aux = (getfield_int(dados, 1));
        if (aux != -1)
        {
            (*barras)[aux].nshunts++;
            if (((*barras)[aux].shunts = (TF_DSHNT *)realloc((*barras)[aux].shunts, (*barras)[aux].nshunts * sizeof(TF_DSHNT))) == NULL)
            {
                printf("Erro -- Nao foi possivel alocar espaco de memoria para shunts !!!!");
                exit(1);
            }
            k = (*barras)[aux].nshunts - 1;

            (*barras)[aux].shunts[k].ID = (*barras)[aux].ID;
            (*barras)[aux].shunts[k].lig = (getfield_int(dados, 2));
            (*barras)[aux].shunts[k].fases = (getfield_int(dados, 3));
            (*barras)[aux].shunts[k].Vbase = (getfield_double(dados, 4));

            (*barras)[aux].shunts[k].Qnom[0] = (getfield_double(dados, 5));
            (*barras)[aux].shunts[k].Qnom[1] = (getfield_double(dados, 6));
            (*barras)[aux].shunts[k].Qnom[2] = (getfield_double(dados, 7));
            (*barras)[aux].shunts[k].controle = (getfield_int(dados, 8));

            if (!isnan(getfield_double(dados, 9)))
            {
                (*barras)[aux].shunts[k].num = (getfield_int(dados, 9));
                (*barras)[aux].shunts[k].DV = (getfield_double(dados, 10));
                (*barras)[aux].shunts[k].Vset[0] = (getfield_double(dados, 11));
                (*barras)[aux].shunts[k].Vset[1] = (getfield_double(dados, 12));
                (*barras)[aux].shunts[k].Vset[2] = (getfield_double(dados, 13));
            }
        }
    }
}

//------------------------------------------------------------------------------
// Leitura de dados de Subestações
/**
 * @brief Função auxiliar para a leitura do arquivo DSUBESTACAO.csv, referente às informações de subestaçoes.
 *
 * Essa função ...
 *
 *
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do TF_DBAR, onde é retornado as informações dos bancos de capacitores.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar um banco de capacitor a ela.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDSUBESTACAO(FILE *arquivo, TF_DBAR **barras, long int *numeroBarras)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;             /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, aux, k;           /* Variáveis contadores para percorrer o arquivo e a string de leitura. */

    // Le o arquivo de curva de cargas até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        // Verifica se a barra já foi criada - Não efetua busca nos IDs de Barras - Assume que já está pré-mapeado corretamente
        aux = -1;

        aux = (getfield_int(dados, 1));
        if (aux != -1)
        {
            (*barras)[aux].nshunts++;
            if (((*barras)[aux].shunts = (TF_DSHNT *)realloc((*barras)[aux].shunts, (*barras)[aux].nshunts * sizeof(TF_DSHNT))) == NULL)
            {
                printf("Erro -- Nao foi possivel alocar espaco de memoria para shunts !!!!");
                exit(1);
            }
            k = (*barras)[aux].nshunts - 1;
        }
    }
}

//------------------------------------------------------------------------------
// Leitura de dados de Interface 34.5 kV com 13.8kV
/**
 * @brief Função auxiliar para a leitura do arquivo DINTERSE.csv, referente às informações de subestaçoes de interface entre 34.5 e 13.8 kV.
 *
 * Essa função ...
 *
 *
 * @param ....
 * @return void.
 * @see leituraDados
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
long int **leituraDINTERSE(char *folder, char *file, long int *numeroInterfaces, TF_DBAR **barras, long int *numeroBarras, TF_DRAM **ramos, long int *numeroRamos)
{
    char blocoLeitura[2000];            /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;                        /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, j, aux, k;                   /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int contador, carac, numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */
    FILE *arquivo;
    char text_aux[500];
    long int **interfaceNiveis = NULL;

    extern BOOL includeVR_INTERSE_OPT;

    // Leitura dos dados de medidores
    // strcpy(text_aux,folder);
    sprintf(text_aux, "%s", folder);
    arquivo = fopen(strcat(text_aux, file), "r");

    if (arquivo != NULL)
    {

        // Aloca na memória espaço para as barras
        while ((carac = fgetc(arquivo)) != EOF)
        {
            if (carac == '\n')
                numLinhas++;
        }
        rewind(arquivo);

        interfaceNiveis = (long int **)malloc((numLinhas + 1) * sizeof(long int *));
        for (i = 0; i < numLinhas; i++)
        {
            interfaceNiveis[i] = (long int *)malloc(4 * sizeof(long int));
            for (j = 0; j < 5; j++)
            {
                interfaceNiveis[i][j] = -1;
            }
        }
        numeroInterfaces[0] = numLinhas;

        // Le o arquivo de interfaces até o fim
        contador = 0;
        while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
        {
            dados = blocoLeitura;

            aux = (getfield_int(dados, 1));
            interfaceNiveis[contador][0] = getfield_int(dados, 1);
            interfaceNiveis[contador][1] = getfield_int(dados, 2);
            interfaceNiveis[contador][2] = getfield_int(dados, 4);
            interfaceNiveis[contador][4] = getfield_int(dados, 6); // Indicando se é subestação 34.5/13.8 ou estação de chaves 34.5/34.5 - sem necessidade de trafo + reg (0 para se e 1 para estação de chaves)

            // para salvar a barra de 34.5 original da interface
            interfaceNiveis[contador][3] = interfaceNiveis[contador][1];
            contador++;
        }
        fclose(arquivo);

        // Incluir um regulador de tensão e um transformador Estrela Aterrado Delta como modelo básico da INTERSE
        int aux_bar = numeroBarras[0];
        if (includeVR_INTERSE_OPT)
        {

            // Percorre interfaces 34.5 / 13.8 para atualizar o modelo de fronteira com barras fictícias
            for (int idInter = 0; idInter < contador; idInter++)
            {
                if ((interfaceNiveis[idInter][1] < aux_bar) && (interfaceNiveis[idInter][4] == 0))
                { // se não é barra ficticia
                    //-------------------------------------------
                    // inclui nova barra de fronteira
                    includeDBAR(barras, numeroBarras, 34500);

                    // inclui regulador entre a barra de fronteira original e a nova barra de fronteira
                    // Considera sempre Forward jusante modelo 99
                    // Controle em p.u. em relação a barra imediatamente após o regulador - caso COPEL
                    includeDREG(ramos, numeroRamos, interfaceNiveis[idInter][1], numeroBarras[0] - 1, 99);

                    // inclui nova barra de fronteira
                    includeDBAR(barras, numeroBarras, 13800);

                    // inclui transformador Estrela Delta entre a barra de fronteira original e a nova barra de fronteira
                    includeDTRF(ramos, numeroRamos, numeroBarras[0] - 2, numeroBarras[0] - 1, 34500, 13800, 1, 1);

                    //-------------------------------------------
                    // define a nova barra de interface da SE
                    interfaceNiveis[idInter][1] = numeroBarras[0] - 1;

                    // Atualiza novo no de fronteira do alimentador de 34.5 (jusante do regulador) para todas as demais interfaces
                    for (int i = 0; i < contador; i++)
                    {
                        if (interfaceNiveis[idInter][3] == interfaceNiveis[i][1])
                            interfaceNiveis[i][1] = interfaceNiveis[idInter][1];
                    }
                }
            }
        }

        return (interfaceNiveis);
    }
    else
    {
        return (NULL);
    }
}

//------------------------------------------------------------------------------
// Leitura de dados de dados qualitativos de dados de subestação e alimentadores
/**

 * @param ....
 * @return void.
 * @see leituraDados
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraQualiSE(char *folder, long int *numeroSE, TF_DTRFSE **DTRFSE, long int *numeroDALIM, TF_DALIM **DALIM)
{
    char blocoLeitura[2000];            /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;                        /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, j, aux, k;                   /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int contador, carac, numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */
    FILE *arquivo;
    char text_aux[500];
    long int **interfaceNiveis = NULL;

    // Leitura dos dados de medidores
    // strcpy(text_aux,folder);
    sprintf(text_aux, "%s", folder);
    arquivo = fopen(strcat(text_aux, "DTRFSE.csv"), "r");
    numeroSE[0] = 0;
    numeroDALIM[0] = 0;
    // default para alimentadores não agrupados
    numeroSE[0] = 1;
    if (((*DTRFSE) = (TF_DTRFSE *)malloc(sizeof(TF_DTRFSE))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as chaves !!!!");
        exit(1);
    }
    (*DTRFSE)[0].ID = -1; // ID - Identificação da subestação
    (*DTRFSE)[0].i = 0;
    (*DTRFSE)[0].idTrafoSE = -1;               // idTrafoSE - identificação do transformador da subestação
    (*DTRFSE)[0].V_pri = 138000;               // V_pri - Tensão do primério da subestação
    (*DTRFSE)[0].V_sec = 13800;                // V_sec - Tensão do secundário da subestação
    (*DTRFSE)[0].Snom = 99999999;              // Snom - Potência Nominal
    strcpy((*DTRFSE)[0].sigla_SE, "Ficticio"); // sigla_SE - Sigla da subestação

    if (arquivo != NULL)
    {
        leituraDTRFSE(arquivo, DTRFSE, numeroSE);
        fclose(arquivo);
    }

    sprintf(text_aux, "%s", folder);
    arquivo = fopen(strcat(text_aux, "DALIM.csv"), "r"); // Le somente se existir o arquivo
    if (arquivo != NULL)
    {
        leituraDALIM(arquivo, DALIM, numeroDALIM);
        fclose(arquivo);
    }
    else
    {
    }
}

//------------------------------------------------------------------------------
// Leitura de dados de GDs
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
void leituraDGD(FILE *arquivo, TF_DBAR **barras, long int *numeroBarras)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;             /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, aux, k;           /* Variáveis contadores para percorrer o arquivo e a string de leitura. */

    // Le o arquivo de curva de cargas até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        // Verifica se a barra já foi criada
        aux = -1;
        for (i = 0; i < numeroBarras[0]; i++)
        {
            if ((*barras)[i].ID == (getfield_int(dados, 1)))
            {
                aux = i;
            }
        }
        if (aux != -1)
        {
            (*barras)[aux].ngds++;
            if (((*barras)[aux].gds = (TF_DGD *)realloc((*barras)[aux].gds, (*barras)[aux].ngds * sizeof(TF_DGD))) == NULL)
            {
                printf("Erro -- Nao foi possivel alocar espaco de memoria para gds !!!!");
                exit(1);
            }
            k = (*barras)[aux].ngds - 1;

            (*barras)[aux].gds[k].ID = (*barras)[aux].ID;
            (*barras)[aux].gds[k].lig = (getfield_int(dados, 2));
            (*barras)[aux].gds[k].fases = (getfield_int(dados, 3));
            (*barras)[aux].gds[k].Vbase = (getfield_double(dados, 4));
            (*barras)[aux].gds[k].Snominal = (getfield_double(dados, 5));

            (*barras)[aux].gds[k].Pnom[0] = (getfield_double(dados, 6));
            (*barras)[aux].gds[k].Pnom[1] = (getfield_double(dados, 7));
            (*barras)[aux].gds[k].Pnom[2] = (getfield_double(dados, 8));
            (*barras)[aux].gds[k].Qnom[0] = (getfield_double(dados, 9));
            (*barras)[aux].gds[k].Qnom[1] = (getfield_double(dados, 10));
            (*barras)[aux].gds[k].Qnom[2] = (getfield_double(dados, 11));
            (*barras)[aux].gds[k].controle = (getfield_int(dados, 12));

            // Valores para limite de reativos em geradores com controle de tensão (barra PV)
            if (!isnan(getfield_double(dados, 13)))
            {
                (*barras)[aux].gds[k].Qmin = (getfield_double(dados, 13));
                (*barras)[aux].gds[k].Qmax = (getfield_double(dados, 14));
                (*barras)[aux].gds[k].Vset[0] = (getfield_double(dados, 15));
                (*barras)[aux].gds[k].Vset[1] = (getfield_double(dados, 16));
                (*barras)[aux].gds[k].Vset[2] = (getfield_double(dados, 17));
                (*barras)[aux].gds[k].controlePV = (getfield_int(dados, 18));
            }
        }
    }
}

//------------------------------------------------------------------------------
// Leitura de dados de LINHAS
/**
 * @brief Função auxiliar para a leitura do arquivo DLIN.csv, referente às informações de circuitos trifásicos (ramais).
 *
 * Essa função realiza a leitura do arquivo DLIN.csv que contém os dados relativos aos ciruitos trifásico do sistema. Armazena estes dados
 * dentro da estrutura de dados do tipo DLIM, como um parâmetro interno de um determinado elemento TF_DRAM (que indica um ramo da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as ramos referentes aos circuitos e a quantidade total de ramos da
 * rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo TF_DRAM, @p **ramos onde são armazenados dados de circuitos trifásicos
 * internamente numa estrutura de dados do tipo DLIM, um parâmetro de entrada e saída @p numeroRamos para indicar a quantidade total de ramos, e
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo TF_DBAR, @p **barras para associar os
 * terminais de determinado ramo com as barras da rede elétrica e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 *
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param ramos é um ponteiro para o ponteiro da estrutura do tipo do TF_DRAM, onde é retornada as informções dos ramos da rede elétrica, nesta função os circuitos são armazenados internamente em um tipo DLIN
 * @param numeroRamos retorna a quantidade total de ramos
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do TF_DBAR para associar a conectividade dos ramos às barras da rede elétrica.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDLIN(FILE *arquivo, TF_DRAM **ramos, long int *numeroRamos, TF_DBAR **barras, long int *numeroBarras)
{
    char blocoLeitura[2000];       /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;                   /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    long int contador = 0, i, aux; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0;      /* Variável com o número de linhas do arquivo a serem lidas. */
    extern BOOL symetric_OPT;      /* Variável externa para modelo balanceado e simétrico. */

    // Aloca na memória espaço para as linhas
    while ((carac = fgetc(arquivo)) != EOF)
    {
        if (carac == '\n')
            numLinhas++;
    }
    rewind(arquivo);
    if (((*ramos) = (TF_DRAM *)malloc((numLinhas + 1) * sizeof(TF_DRAM))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as linhas !!!!");
        exit(1);
    }

    // Le o arquivo de curva de cargas até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        (*ramos)[contador].DE = (getfield_int(dados, 1));
        (*ramos)[contador].PARA = (getfield_int(dados, 2));
        (*ramos)[contador].tipo = 0;
        (*ramos)[contador].estado = 1;
        (*ramos)[contador].fases = (getfield_int(dados, 3));

        // Busca pela ID de barra de cada ramo
        //        for(i=0;i<numeroBarras[0];i++){
        //            if((*ramos)[contador].DE == (*barras)[i].ID ){
        //                (*ramos)[contador].k = (*barras)[i].i;
        //            }
        //            if((*ramos)[contador].PARA == (*barras)[i].ID ){
        //                (*ramos)[contador].m = (*barras)[i].i;
        //            }
        //        }
        (*ramos)[contador].k = (*ramos)[contador].DE;
        (*ramos)[contador].m = (*ramos)[contador].PARA;

        // Preenche o (*ramos)[contador].linha
        (*ramos)[contador].linha.fases = (getfield_int(dados, 3));
        (*ramos)[contador].linha.comprimento = (getfield_double(dados, 4));
        (*ramos)[contador].comprimento = (*ramos)[contador].linha.comprimento;

        if ((*ramos)[contador].comprimento == 1)
        {
            (*ramos)[contador].comprimento = 40;
        }
        double comp = (getfield_double(dados, 4));
        __real__(*ramos)[contador].linha.Zaa = comp * (getfield_double(dados, 5));
        __imag__(*ramos)[contador].linha.Zaa = comp * (getfield_double(dados, 6));
        __real__(*ramos)[contador].linha.Zab = comp * (getfield_double(dados, 7));
        __imag__(*ramos)[contador].linha.Zab = comp * (getfield_double(dados, 8));
        __real__(*ramos)[contador].linha.Zac = comp * (getfield_double(dados, 9));
        __imag__(*ramos)[contador].linha.Zac = comp * (getfield_double(dados, 10));
        __real__(*ramos)[contador].linha.Zbb = comp * (getfield_double(dados, 11));
        __imag__(*ramos)[contador].linha.Zbb = comp * (getfield_double(dados, 12));
        __real__(*ramos)[contador].linha.Zbc = comp * (getfield_double(dados, 13));
        __imag__(*ramos)[contador].linha.Zbc = comp * (getfield_double(dados, 14));
        __real__(*ramos)[contador].linha.Zcc = comp * (getfield_double(dados, 15));
        __imag__(*ramos)[contador].linha.Zcc = comp * (getfield_double(dados, 16));
        (*ramos)[contador].linha.Baa = comp * (getfield_double(dados, 17)) / 1000000; // em microSiemens por unidade de comprimento
        (*ramos)[contador].linha.Bab = comp * (getfield_double(dados, 18)) / 1000000;
        (*ramos)[contador].linha.Bac = comp * (getfield_double(dados, 19)) / 1000000;
        (*ramos)[contador].linha.Bbb = comp * (getfield_double(dados, 20)) / 1000000;
        (*ramos)[contador].linha.Bbc = comp * (getfield_double(dados, 21)) / 1000000;
        (*ramos)[contador].linha.Bcc = comp * (getfield_double(dados, 22)) / 1000000;

        if (symetric_OPT)
        {
            (*ramos)[contador].fases = 7;

            __complex__ double z_aux = ((*ramos)[contador].linha.Zaa + (*ramos)[contador].linha.Zbb + (*ramos)[contador].linha.Zcc) / 3;
            (*ramos)[contador].linha.Zaa = z_aux;
            (*ramos)[contador].linha.Zbb = z_aux;
            (*ramos)[contador].linha.Zcc = z_aux;

            z_aux = ((*ramos)[contador].linha.Zab + (*ramos)[contador].linha.Zac + (*ramos)[contador].linha.Zbc) / 3;
            (*ramos)[contador].linha.Zab = z_aux;
            (*ramos)[contador].linha.Zac = z_aux;
            (*ramos)[contador].linha.Zbc = z_aux;

            double b_aux = ((*ramos)[contador].linha.Baa + (*ramos)[contador].linha.Bbb + (*ramos)[contador].linha.Bcc) / 3;
            (*ramos)[contador].linha.Baa = b_aux;
            (*ramos)[contador].linha.Bbb = b_aux;
            (*ramos)[contador].linha.Bcc = b_aux;

            b_aux = ((*ramos)[contador].linha.Bab + (*ramos)[contador].linha.Bac + (*ramos)[contador].linha.Bbc) / 3;
            (*ramos)[contador].linha.Bab = b_aux;
            (*ramos)[contador].linha.Bac = b_aux;
            (*ramos)[contador].linha.Bbc = b_aux;
        }

        (*ramos)[contador].linha.ampacidade = 9999999.9;
        if (!isnan(getfield_double(dados, 23)))
        {
            (*ramos)[contador].linha.ampacidade = getfield_double(dados, 23);
            if ((*ramos)[contador].linha.ampacidade == 999)
                (*ramos)[contador].linha.ampacidade = 250;
            if ((*ramos)[contador].linha.ampacidade == 0)
                (*ramos)[contador].linha.ampacidade = 250;
        }

        contador++;
    }
    numeroRamos[0] = contador;
}

//------------------------------------------------------------------------------
// Leitura de dados de TRAFOS
/**
 * @brief Função auxiliar para a leitura do arquivo DTRF.csv, referente às informações de transormadores de potência.
 *
 * Essa função realiza a leitura do arquivo DTRF.csv que contém os dados relativos aos trasnformadores de potência do sistema. Armazena estes dados
 * dentro da estrutura de dados do tipo DTRF, como um parâmetro interno de um determinado elemento TF_DRAM (que indica um ramo da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as ramos referentes aos trnasofrmadores e a quantidade total de ramos da
 * rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo TF_DRAM, @p **ramos onde são armazenados dados de transformadores
 * internamente numa estrutura de dados do tipo DTRF, um parâmetro de entrada e saída @p numeroRamos para indicar a quantidade total de ramos, e
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo TF_DBAR, @p **barras para associar os
 * terminais de determinado ramo com as barras da rede elétrica e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 *
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param ramos é um ponteiro para o ponteiro da estrutura do tipo do TF_DRAM, onde é retornada as informções dos ramos da rede elétrica, nesta função os transformadores são armazenados internamente em um tipo DTRF
 * @param numeroRamos retorna a quantidade total de ramos
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do TF_DBAR para associar a conectividade dos ramos às barras da rede elétrica.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDTRF(FILE *arquivo, TF_DRAM **ramos, long int *numeroRamos, TF_DBAR **barras, long int *numeroBarras)
{
    char blocoLeitura[2000];  /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;              /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int contador = 0, i, aux; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */

    // Aloca na memória espaço para os trafos
    while ((carac = fgetc(arquivo)) != EOF)
    {
        if (carac == '\n')
            numLinhas++;
    }
    rewind(arquivo);

    if (((*ramos) = (TF_DRAM *)realloc((*ramos), (numeroRamos[0] + numLinhas + 1) * sizeof(TF_DRAM))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para os trafos !!!!");
        exit(1);
    }

    contador = numeroRamos[0];
    // Le o arquivo até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        (*ramos)[contador].DE = (getfield_int(dados, 1));
        (*ramos)[contador].PARA = (getfield_int(dados, 2));
        (*ramos)[contador].tipo = 1;
        (*ramos)[contador].estado = 1;
        (*ramos)[contador].fases = (getfield_int(dados, 3));
        (*ramos)[contador].comprimento = 1;

        //        for(i=0;i<numeroBarras[0];i++){
        //            if((*ramos)[contador].DE == (*barras)[i].ID ){
        //                (*ramos)[contador].k = (*barras)[i].i;
        //            }
        //            if((*ramos)[contador].PARA == (*barras)[i].ID ){
        //                (*ramos)[contador].m = (*barras)[i].i;
        //            }
        //        }
        (*ramos)[contador].k = (*ramos)[contador].DE;
        (*ramos)[contador].m = (*ramos)[contador].PARA;

        // Preencher o (*ramos)[contador].trafo
        (*ramos)[contador].trafo.fases = (getfield_int(dados, 3));
        (*ramos)[contador].trafo.Vpri = (getfield_double(dados, 4));
        (*ramos)[contador].trafo.Vsec = (getfield_double(dados, 5));
        (*ramos)[contador].trafo.Snominal = (getfield_double(dados, 6));
        (*ramos)[contador].trafo.R = (getfield_double(dados, 7)) * pow((*ramos)[contador].trafo.Vsec, 2) / ((*ramos)[contador].trafo.Snominal * 1000) / 3; // R*(Vpri^2/(S*1000))
        (*ramos)[contador].trafo.X = (getfield_double(dados, 8)) * pow((*ramos)[contador].trafo.Vsec, 2) / ((*ramos)[contador].trafo.Snominal * 1000) / 3;
        (*ramos)[contador].trafo.lig_pri = (getfield_int(dados, 9));
        (*ramos)[contador].trafo.lig_sec = (getfield_int(dados, 10));
        (*ramos)[contador].trafo.defasamento = (getfield_int(dados, 11));
        (*ramos)[contador].trafo.tap_pri = (getfield_double(dados, 12));
        (*ramos)[contador].trafo.tap_sec = (getfield_double(dados, 13));

        (*ramos)[contador].trafo.p_imag = getfield_double(dados, 14);
        if ((*ramos)[contador].trafo.p_imag == NAN)
            (*ramos)[contador].trafo.p_imag = 0;
        (*ramos)[contador].trafo.noLoadLosses = getfield_double(dados, 15);
        if ((*ramos)[contador].trafo.noLoadLosses == NAN)
            (*ramos)[contador].trafo.noLoadLosses = 0;

        if (!isnan(getfield_double(dados, 16)))
        {
            (*ramos)[contador].trafo.r_mutua = getfield_double(dados, 16);
            (*ramos)[contador].trafo.x_mutua = getfield_double(dados, 17);
        }

        contador++;
    }
    numeroRamos[0] = numeroRamos[0] + numLinhas;
}

//------------------------------------------------------------------------------
// Leitura de dados de REGULADORES
/**
 * @brief Função auxiliar para a leitura do arquivo DREG.csv, referente às informações de reguladores de tensão.
 *
 * Essa função realiza a leitura do arquivo DREG.csv que contém os dados relativos aos reguladores de tensão do sistema. Armazena estes dados
 * dentro da estrutura de dados do tipo DREG, como um parâmetro interno de um determinado elemento TF_DRAM (que indica um ramo da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as ramos referentes aos reguladores e a quantidade total de ramos da
 * rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo TF_DRAM, @p **ramos onde são armazenados dados de reguladores de tensão
 * internamente numa estrutura de dados do tipo DTRF, um parâmetro de entrada e saída @p numeroRamos para indicar a quantidade total de ramos, e
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo TF_DBAR, @p **barras para associar os
 * terminais de determinado ramo com as barras da rede elétrica e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 *
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param ramos é um ponteiro para o ponteiro da estrutura do tipo do TF_DRAM, onde é retornada as informações dos ramos da rede elétrica, nesta função os reguladores são armazenados internamente em um tipo DREG
 * @param numeroRamos retorna a quantidade total de ramos
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do TF_DBAR para associar a conectividade dos ramos às barras da rede elétrica.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDREG(FILE *arquivo, TF_DRAM **ramos, long int *numeroRamos, TF_DBAR **barras, long int *numeroBarras)
{
    char blocoLeitura[2000];  /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;              /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int contador = 0, i, aux; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */

    // Aloca na memória espaço para os reguladores
    while ((carac = fgetc(arquivo)) != EOF)
    {
        if (carac == '\n')
            numLinhas++;
    }
    rewind(arquivo);
    if (((*ramos) = (TF_DRAM *)realloc((*ramos), (numeroRamos[0] + numLinhas + 1) * sizeof(TF_DRAM))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para os reguladores de tensão !!!!");
        exit(1);
    }
    contador = numeroRamos[0];
    // Le o arquivo de curva de cargas até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        (*ramos)[contador].DE = (getfield_int(dados, 1));
        (*ramos)[contador].PARA = (getfield_int(dados, 2));
        (*ramos)[contador].tipo = 2;
        (*ramos)[contador].estado = 1;
        (*ramos)[contador].fases = (getfield_int(dados, 3));
        (*ramos)[contador].comprimento = 1;

        //        for(i=0;i<numeroBarras[0];i++){
        //            if((*ramos)[contador].DE == (*barras)[i].ID ){
        //                (*ramos)[contador].k = (*barras)[i].i;
        //            }
        //            if((*ramos)[contador].PARA == (*barras)[i].ID ){
        //                (*ramos)[contador].m = (*barras)[i].i;
        //            }
        //        }
        (*ramos)[contador].k = (*ramos)[contador].DE;
        (*ramos)[contador].m = (*ramos)[contador].PARA;

        // Preencher o (*ramos)[contador].regulador
        (*ramos)[contador].regulador.fases = (getfield_int(dados, 3));
        (*ramos)[contador].regulador.Vnom = (getfield_double(dados, 4));
        (*ramos)[contador].regulador.regulacao = (getfield_double(dados, 5));
        (*ramos)[contador].regulador.ntaps = (getfield_int(dados, 6));
        (*ramos)[contador].regulador.Snominal = (getfield_double(dados, 7));
        (*ramos)[contador].regulador.R = (getfield_double(dados, 8)) * pow((*ramos)[contador].regulador.Vnom, 2) / ((*ramos)[contador].regulador.Snominal * 1000) / 3;
        (*ramos)[contador].regulador.X = (getfield_double(dados, 9)) * pow((*ramos)[contador].regulador.Vnom, 2) / ((*ramos)[contador].regulador.Snominal * 1000) / 3;
        (*ramos)[contador].regulador.lig = (getfield_int(dados, 10));
        (*ramos)[contador].regulador.TP = (getfield_double(dados, 11));
        (*ramos)[contador].regulador.TC = (getfield_double(dados, 12));
        (*ramos)[contador].regulador.deltaV = (getfield_double(dados, 13)) / 2;
        (*ramos)[contador].regulador.R1 = (getfield_double(dados, 14));
        (*ramos)[contador].regulador.X1 = (getfield_double(dados, 15));
        (*ramos)[contador].regulador.R2 = (getfield_double(dados, 16));
        (*ramos)[contador].regulador.X2 = (getfield_double(dados, 17));
        (*ramos)[contador].regulador.R3 = (getfield_double(dados, 18));
        (*ramos)[contador].regulador.X3 = (getfield_double(dados, 19));
        (*ramos)[contador].regulador.V1 = (getfield_double(dados, 20));
        (*ramos)[contador].regulador.V2 = (getfield_double(dados, 21));
        (*ramos)[contador].regulador.V3 = (getfield_double(dados, 22));
        (*ramos)[contador].regulador.controle = (getfield_int(dados, 23));
        (*ramos)[contador].regulador.tap[0] = (getfield_double(dados, 24));
        (*ramos)[contador].regulador.tap[1] = (getfield_double(dados, 25));
        (*ramos)[contador].regulador.tap[2] = (getfield_double(dados, 26));

        (*ramos)[contador].regulador.tap_ini[0] = (getfield_double(dados, 24));
        (*ramos)[contador].regulador.tap_ini[1] = (getfield_double(dados, 25));
        (*ramos)[contador].regulador.tap_ini[2] = (getfield_double(dados, 26));

        if ((*ramos)[contador].regulador.controle == 99)
        {                                                  // Controle em p.u. em relação a barra imediatamente após o regulador - caso COPEL
            (*ramos)[contador].regulador.deltaV = 0.00833; // 1 / 120 (1 volt na base de 120 volts secundários)
        }

        // Se tiver parametros de controle reverso
        if (!isnan(getfield_double(dados, 27)))
        {
            (*ramos)[contador].regulador.deltaVr = (getfield_double(dados, 27)) / 2;
            (*ramos)[contador].regulador.R1r = (getfield_double(dados, 28));
            (*ramos)[contador].regulador.X1r = (getfield_double(dados, 29));
            (*ramos)[contador].regulador.R2r = (getfield_double(dados, 30));
            (*ramos)[contador].regulador.X2r = (getfield_double(dados, 31));
            (*ramos)[contador].regulador.R3r = (getfield_double(dados, 32));
            (*ramos)[contador].regulador.X3r = (getfield_double(dados, 33));
            (*ramos)[contador].regulador.V1r = (getfield_double(dados, 34));
            (*ramos)[contador].regulador.V2r = (getfield_double(dados, 35));
            (*ramos)[contador].regulador.V3r = (getfield_double(dados, 36));
        }
        contador++;
    }
    numeroRamos[0] = numeroRamos[0] + numLinhas;
}

//------------------------------------------------------------------------------
// Leitura de dados de CHAVES
/**
 * @brief Função auxiliar para a leitura do arquivo DREG.csv, referente às informações de chaves e elementos seccionadores.
 *
 * Essa função realiza a leitura do arquivo DSWTC.csv que contém os dados relativos às chaves e elementos seccionadores do sistema.
 * Armazena estes dados dentro da estrutura de dados de um determinado elemento TF_DRAM (que indica um ramo da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as ramos referentes aos reguladores e a quantidade total de ramos da
 * rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo TF_DRAM, @p **ramos onde são armazenados dados de chaves e seccionadores
 * na estrutura de dados do tipo TF_DRAM, um parâmetro de entrada e saída @p numeroRamos para indicar a quantidade total de ramos, e
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo DBAR, @p **barras para associar os
 * terminais de determinado ramo com as barras da rede elétrica e um parâmetro indicando o número total de barras.
 * A função retorna @c void.
 *
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param ramos é um ponteiro para o ponteiro da estrutura do tipo do TF_DRAM, onde é retornada as informações dos ramos da rede elétrica, nesta função as chaves e seccionadoreas são armazenados diretamente na estrutura TF_DRAM
 * @param numeroRamos retorna a quantidade total de ramos
 * @param barras é um ponteiro para o ponteiro da estrutura do tipo do DBAR para associar a conectividade dos ramos às barras da rede elétrica.
 * @param numeroBarras quantidade total de barras
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDSWTC(FILE *arquivo, TF_DRAM **ramos, long int *numeroRamos, TF_DBAR **barras, long int *numeroBarras)
{
    char blocoLeitura[2000];  /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;              /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int contador = 0, i, aux; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */

    // Aloca na memória espaço para os trafos
    while ((carac = fgetc(arquivo)) != EOF)
    {
        if (carac == '\n')
            numLinhas++;
    }
    rewind(arquivo);

    if (((*ramos) = (TF_DRAM *)realloc((*ramos), (numeroRamos[0] + numLinhas + 1) * sizeof(TF_DRAM))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as chaves !!!!");
        exit(1);
    }

    contador = numeroRamos[0];
    // Le o arquivo até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        (*ramos)[contador].DE = (getfield_int(dados, 1));
        (*ramos)[contador].PARA = (getfield_int(dados, 2));
        (*ramos)[contador].tipo = 3;
        (*ramos)[contador].comprimento = 0.002;

        //        for(i=0;i<numeroBarras[0];i++){
        //            if((*ramos)[contador].DE == (*barras)[i].ID ){
        //                (*ramos)[contador].k = (*barras)[i].i;
        //            }
        //            if((*ramos)[contador].PARA == (*barras)[i].ID ){
        //                (*ramos)[contador].m = (*barras)[i].i;
        //            }
        //        }
        (*ramos)[contador].k = (*ramos)[contador].DE;
        (*ramos)[contador].m = (*ramos)[contador].PARA;

        (*ramos)[contador].fases = (getfield_int(dados, 3));
        (*ramos)[contador].estado = (getfield_int(dados, 4));

        (*ramos)[contador].ampacidade = 9999999.9;
        if (!isnan(getfield_double(dados, 6)))
        {
            (*ramos)[contador].ampacidade = getfield_double(dados, 6);
            if ((*ramos)[contador].linha.ampacidade == 0)
                (*ramos)[contador].linha.ampacidade = 9999999.9;
        }

        contador++;
    }
    numeroRamos[0] = numeroRamos[0] + numLinhas;
}

//------------------------------------------------------------------------------
// Leitura de dados de DTRFSE
/**
 * @brief Função auxiliar para a leitura do arquivo DTRFSE.csv, referente às informações das subestações
 *
 * Essa função realiza a leitura do arquivo DTRFSE.csv que contém os dados relativos às subestações do sistema.
 * Armazena estes dados dentro da estrutura de dados TF_DTRFSE (que indica uma subestação da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as informações referentes as subestações da rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo TF_DTRFSE, @p **DSE onde são armazenados dados das subestações
 * um parâmetro de entrada e saída @p numeroSEs para indicar a quantidade total de Subestações, e
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo DTRFSE,
 * A função retorna @c void.
 *
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param DSE é um ponteiro para o ponteiro da estrutura do tipo do TF_DTRFSE, onde são retornadas as informações das subestações da rede elétrica.
 * @param numeroSEs retorna a quantidade total de subestações
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDTRFSE(FILE *arquivo, TF_DTRFSE **DSE, long int *numeroSEs)
{
    char blocoLeitura[2000];  /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;              /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int contador = 1, i, aux; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */

    // Aloca na memória espaço para os trafos
    while ((carac = fgetc(arquivo)) != EOF)
    {
        if (carac == '\n')
            numLinhas++;
    }
    rewind(arquivo);
    (*numeroSEs) = numLinhas + 1; // por conta do default
    if (((*DSE) = (TF_DTRFSE *)realloc((*DSE), (numLinhas + 1 + 1) * sizeof(TF_DTRFSE))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as chaves !!!!");
        exit(1);
    }

    // Le o arquivo até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;
        (*DSE)[contador].ID = (getfield_int(dados, 1)); // ID - Identificação da subestação
        (*DSE)[contador].i = contador;
        (*DSE)[contador].idTrafoSE = (getfield_int(dados, 2)); // idTrafoSE - identificação do transformador da subestação
        (*DSE)[contador].V_pri = (getfield_double(dados, 3));  // V_pri - Tensão do primério da subestação
        (*DSE)[contador].V_sec = (getfield_double(dados, 4));  // V_sec - Tensão do secundário da subestação
        (*DSE)[contador].Snom = (getfield_double(dados, 5));   // Snom - Potência Nominal
        strcpy((*DSE)[contador].sigla_SE, getfield(dados, 6)); // sigla_SE - Sigla da subestação
        contador++;
    }
}

//------------------------------------------------------------------------------
// Leitura de dados de DALIM
/**
 * @brief Função auxiliar para a leitura do arquivo DALIM.csv, referente às informações dos alimentadores
 *
 * Essa função realiza a leitura do arquivo DTRFSE.csv que contém os dados relativos aos alimentadores do sistema.
 * Armazena estes dados dentro da estrutura de dados TF_DALIM (que indica um alimentador da rede elétrica).
 * Também é responsável pela alocação de memória necessária para armazenar as informações referentes aos alimentadores da rede elétrica.
 * Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo TF_DALIM, @p **DALIM onde são armazenados dados dos alimentadores
 * um parâmetro de entrada e saída @p numeroAlim para indicar a quantidade total de alimentadores no circuito
 * como parâmetros de entrada um ponteiro do arquivo @p arquivo a ser lido, um ponteiro para ponteiro do tipo DALIM,
 * A função retorna @c void.
 *
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param DALIM é um ponteiro para o ponteiro da estrutura do tipo do TF_DALIM, onde são retornadas as informações dos alimentadors da rede
 * @param numeroAlim retorna a quantidade total de alimentadores
 * @return void.
 * @see leituraDados
 * @note As barras já devem ter sido alocadas para associar ramos que conectam as barras da rede elétrica.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraDALIM(FILE *arquivo, TF_DALIM **DALIM, long int *numeroAlim)
{
    char blocoLeitura[2000];  /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;              /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int contador = 0, i, aux; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */

    // Aloca na memória espaço para os trafos
    while ((carac = fgetc(arquivo)) != EOF)
    {
        if (carac == '\n')
            numLinhas++;
    }
    rewind(arquivo);
    (*numeroAlim) = numLinhas;
    if (((*DALIM) = (TF_DALIM *)malloc(numLinhas * sizeof(TF_DALIM))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as chaves !!!!");
        exit(1);
    }

    // Le o arquivo até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        (*DALIM)[contador].ID = (getfield_int(dados, 1)); // Identificação do alimentador
        (*DALIM)[contador].i = contador;
        (*DALIM)[contador].ID_TR = (getfield_int(dados, 2));   // identificação do transformador
        (*DALIM)[contador].ID_SE = (getfield_int(dados, 3));   // identificação da subestação
        (*DALIM)[contador].noRaiz = (getfield_int(dados, 4));  // identificação do no raiz
        (*DALIM)[contador].Vpri = (getfield_int(dados, 5));    // tensão nominal do alimentador
        (*DALIM)[contador].tipo = 0;                           // livre
        strcpy((*DALIM)[contador].COD_CH, getfield(dados, 6)); // Código COPEL da Chave início Alimentador
        strcpy((*DALIM)[contador].nome, getfield(dados, 7));   // nome do alimentador

        contador++;
    }
}

//------------------------------------------------------------------------------
// Leitura de valores de tensão de inicialização do estimador
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
void leituraVinicial(FILE *arquivo, TF_DBAR **barras, long int *numeroBarras)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;             /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, aux, k;           /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    double Va, Vb, Vc, Ta, Tb, Tc;

    // Le o arquivo de curva de cargas até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        // Verifica se a barra já foi criada
        aux = -1;
        //        for (i=0;i<numeroBarras[0];i++){
        //            if ((*barras)[i].ID == (getfield_int(dados,1))){
        //                aux = i;
        //            }
        //        }
        aux = getfield_int(dados, 1);
        if (aux != -1)
        {
            Va = (getfield_double(dados, 2));
            Vb = (getfield_double(dados, 3));
            Vc = (getfield_double(dados, 4));
            Ta = (getfield_double(dados, 5)) * PI / 180;
            Tb = (getfield_double(dados, 6)) * PI / 180;
            Tc = (getfield_double(dados, 7)) * PI / 180;

            if (Va != Va)
                Va = 1;
            if (Vb != Vb)
                Vb = 1;
            if (Vc != Vc)
                Vc = 1;
            if (Ta != Ta)
                Ta = 0 * PI / 180;
            if (Tb != Tb)
                Tb = -120 * PI / 180;
            if (Tc != Tc)
                Tc = 120 * PI / 180;

            (*barras)[aux].Vinicial[0] = Va * (cos(Ta) + I * sin(Ta));
            (*barras)[aux].Vinicial[1] = Vb * (cos(Tb) + I * sin(Tb));
            (*barras)[aux].Vinicial[2] = Vc * (cos(Tc) + I * sin(Tc));
        }
    }
}

//------------------------------------------------------------------------------
//
// FUNÇÕES DE INSERÇÃO DE NOVOS ELEMENTOS NA REDE ELÉTRICA
//
//------------------------------------------------------------------------------
/**
 * @brief Função auxiliar para a inserção de nova barra elétrica no final do vetor TF_DBAR.
 * Função auxiliar para criação de barras fictícias no modelo
 *
 *
 * A função retorna @c void.
 *
 * @param **barras: vetor de TF_DBAR a ser incrementado com uma nova barra
 * @param *numeroBarras: número total de barras a ser incrementado
 * @param Vbase: tensão base a ser associada com a barra adicionada
 * @return void.
 * @see leituraDados
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void includeDBAR(TF_DBAR **barras, long int *numeroBarras, double Vbase)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;             /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, aux, k, contador; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */

    contador = numeroBarras[0];

    if (((*barras) = (TF_DBAR *)realloc((*barras), (numeroBarras[0] + 1) * sizeof(TF_DBAR))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para barras fictícias !!!!");
        exit(1);
    }

    (*barras)[contador].ID = contador;
    (*barras)[contador].i = contador;
    (*barras)[contador].ligacao = YN;
    (*barras)[contador].fases = ABC;
    (*barras)[contador].Vbase = Vbase / pow(3, 0.5);
    (*barras)[contador].tipo = 0;

    (*barras)[contador].nloads = 0;
    (*barras)[contador].shunts = NULL;
    (*barras)[contador].nshunts = 0;
    (*barras)[contador].gds = NULL;
    (*barras)[contador].ngds = 0;

    (*barras)[contador].Vinicial[0] = 1 * (cos(0) + I * sin(0));
    (*barras)[contador].Vinicial[1] = 1 * (cos(-120 * PI / 180) + I * sin(-120 * PI / 180));
    (*barras)[contador].Vinicial[2] = 1 * (cos(120 * PI / 180) + I * sin(120 * PI / 180));

    (*barras)[contador].loads[0].Pnom[0] = 0;
    (*barras)[contador].loads[0].Pnom[1] = 0;
    (*barras)[contador].loads[0].Pnom[2] = 0;
    (*barras)[contador].loads[0].Qnom[0] = 0;
    (*barras)[contador].loads[0].Qnom[1] = 0;
    (*barras)[contador].loads[0].Qnom[2] = 0;

    numeroBarras[0]++;
}

/**
 * @brief Função auxiliar para a inserção de novo regulador de tensão elétrica no final do vetor TF_DRAM.
 * Função auxiliar para criação de barras fictícias no modelo
 *
 *
 * A função retorna @c void.
 *
 * @param **barras: vetor de DREG a ser incrementado com uma nova barra
 * @param *numeroBarras: número total de barras a ser incrementado
 * @param Vbase: tensão base a ser associada com a barra adicionada
 * @return void.
 * @see leituraDados
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void includeDREG(TF_DRAM **ramos, long int *numeroRamos, int DE, int PARA, int modo_controle)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;             /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, aux, k, contador; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */

    contador = numeroRamos[0];

    if (((*ramos) = (TF_DRAM *)realloc((*ramos), (numeroRamos[0] + 1) * sizeof(TF_DRAM))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para os reguladores de tensão de SE 34.5 / 13.8 kV !!!!");
        exit(1);
    }

    (*ramos)[contador].DE = DE;
    (*ramos)[contador].PARA = PARA;
    (*ramos)[contador].tipo = 2;
    (*ramos)[contador].estado = 1;
    (*ramos)[contador].fases = ABC;
    (*ramos)[contador].comprimento = 1;

    (*ramos)[contador].k = (*ramos)[contador].DE;
    (*ramos)[contador].m = (*ramos)[contador].PARA;

    // Preencher o (*ramos)[contador].regulador
    // Valores default
    (*ramos)[contador].regulador.Vnom = 34500;
    (*ramos)[contador].regulador.regulacao = 0.10;
    (*ramos)[contador].regulador.ntaps = 16;
    (*ramos)[contador].regulador.Snominal = 10000.00;
    (*ramos)[contador].regulador.R = (0.0063) * pow((*ramos)[contador].regulador.Vnom, 2) / ((*ramos)[contador].regulador.Snominal * 1000) / 3;
    (*ramos)[contador].regulador.X = (0.0063) * pow((*ramos)[contador].regulador.Vnom, 2) / ((*ramos)[contador].regulador.Snominal * 1000) / 3;
    (*ramos)[contador].regulador.lig = 1;
    (*ramos)[contador].regulador.TP = 65;
    (*ramos)[contador].regulador.TC = 100;
    (*ramos)[contador].regulador.deltaV = 0.00833; // 1 / 120 (1 volt na base de 120 volts secundários)
    (*ramos)[contador].regulador.R1 = 0;
    (*ramos)[contador].regulador.X1 = 0;
    (*ramos)[contador].regulador.R2 = 0;
    (*ramos)[contador].regulador.X2 = 0;
    (*ramos)[contador].regulador.R3 = 0;
    (*ramos)[contador].regulador.X3 = 0;
    (*ramos)[contador].regulador.V1 = 122.6;
    (*ramos)[contador].regulador.V2 = 122.6;
    (*ramos)[contador].regulador.V3 = 122.6;
    (*ramos)[contador].regulador.controle = modo_controle;
    (*ramos)[contador].regulador.tap[0] = 0;
    (*ramos)[contador].regulador.tap[1] = 0;
    (*ramos)[contador].regulador.tap[2] = 0;

    (*ramos)[contador].regulador.tap_ini[0] = 0;
    (*ramos)[contador].regulador.tap_ini[1] = 0;
    (*ramos)[contador].regulador.tap_ini[2] = 0;

    numeroRamos[0]++;
}

/**
 * @brief Função auxiliar para a inserção de nova chave no final do vetor TF_DRAM.
 * Função auxiliar para criação de barras fictícias no modelo
 *
 *
 * A função retorna @c void.
 *
 * @param **barras: vetor de DREG a ser incrementado com uma nova barra
 * @param *numeroBarras: número total de barras a ser incrementado
 * @param Vbase: tensão base a ser associada com a barra adicionada
 * @return void.
 * @see leituraDados
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void includeDSWTC(TF_DRAM **ramos, long int *numeroRamos, int DE, int PARA, int estado)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;             /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, aux, k, contador; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */

    contador = numeroRamos[0];

    if (((*ramos) = (TF_DRAM *)realloc((*ramos), (numeroRamos[0] + 1) * sizeof(TF_DRAM))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para os reguladores de tensão de SE 34.5 / 13.8 kV !!!!");
        exit(1);
    }

    (*ramos)[contador].DE = DE;
    (*ramos)[contador].PARA = PARA;
    (*ramos)[contador].tipo = 3;
    (*ramos)[contador].estado = estado;
    (*ramos)[contador].fases = ABC;
    (*ramos)[contador].comprimento = 0.001;

    (*ramos)[contador].k = (*ramos)[contador].DE;
    (*ramos)[contador].m = (*ramos)[contador].PARA;

    (*ramos)[contador].ampacidade = 9999999.9;

    numeroRamos[0]++;
}

/**
 * @brief Função auxiliar para a inserção de novo transformador de potência no final do vetor TF_DRAM.
 * Função auxiliar para criação de barras fictícias no modelo
 *
 *
 * A função retorna @c void.
 *
 * @param **barras: vetor de DTRF a ser incrementado com uma nova barra
 * @param *numeroBarras: número total de barras a ser incrementado
 * @param Vbase: tensão base a ser associada com a barra adicionada
 * @return void.
 * @see leituraDados
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void includeDTRF(TF_DRAM **ramos, long int *numeroRamos, int DE, int PARA, double Vpri, double Vsec, int lig_pri, int lig_sec)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;             /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, aux, k, contador; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */

    contador = numeroRamos[0];

    if (((*ramos) = (TF_DRAM *)realloc((*ramos), (numeroRamos[0] + 1) * sizeof(TF_DRAM))) == NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para os reguladores de tensão de SE 34.5 / 13.8 kV !!!!");
        exit(1);
    }

    (*ramos)[contador].DE = DE;
    (*ramos)[contador].PARA = PARA;
    (*ramos)[contador].tipo = 1;
    (*ramos)[contador].estado = 1;
    (*ramos)[contador].fases = ABC;
    (*ramos)[contador].comprimento = 1;
    (*ramos)[contador].Snominal = 10000.00;

    (*ramos)[contador].k = (*ramos)[contador].DE;
    (*ramos)[contador].m = (*ramos)[contador].PARA;

    // Preencher o (*ramos)[contador].regulador
    // Valores default
    (*ramos)[contador].trafo.Vpri = Vpri;
    (*ramos)[contador].trafo.Vsec = Vsec;
    (*ramos)[contador].trafo.fases = ABC;
    (*ramos)[contador].trafo.Snominal = 10000.00;
    (*ramos)[contador].trafo.R = (0.0133) * pow((*ramos)[contador].trafo.Vsec, 2) / ((*ramos)[contador].trafo.Snominal * 1000) / 3; // R*(Vpri^2/(S*1000))
    (*ramos)[contador].trafo.X = (0.0363) * pow((*ramos)[contador].trafo.Vsec, 2) / ((*ramos)[contador].trafo.Snominal * 1000) / 3;
    (*ramos)[contador].trafo.lig_pri = lig_pri;
    (*ramos)[contador].trafo.lig_sec = lig_sec;
    (*ramos)[contador].trafo.defasamento = 1;
    (*ramos)[contador].trafo.tap_pri = 1;
    (*ramos)[contador].trafo.tap_sec = 1;

    (*ramos)[contador].trafo.p_imag = 0;
    (*ramos)[contador].trafo.noLoadLosses = 0;

    numeroRamos[0]++;
}

//------------------------------------------------------------------------------
//
// FUNÇÕES DE LEITURA DE DADOS ANALÓGICOS E DISCRETOS DO SISTEMA SUPERVISÓRIO
//
//------------------------------------------------------------------------------
/**
 * @brief Função principal para a leitura de arquivo com dados de medidas analógicas do sistema supervisório
 *
 * Essa função realiza a leitura  de arquivo com dados de medidas analógicas do sistema supervisório dentro da pasta com os dados da rede elétrica.
 * O local da pasta é recebido como parâmetro @p folder assim como o nome do arquivo no parâmetro @p file. Além disso o arquivo possui como
 * separação dos dados os marcadores especificados. Realiza alocação de memória para armazenar os dados de medidas trifásicas da rede elétrica.
 * Recebe como parâmetros de entrada e saída um ponteiro para ponteiro do tipo TF_DMED@p **medidas que armazena dados das medidas da rede elétrica, e também
 * uma estrutura de dados TF_GRAFO @p grafo para associar os medidores à topologia da rede elétrica.
 * Recebe como parâmetros de entrada um ponteiro do tipo DBAR @p *barras e um ponterio do tipo TF_DRAM @p ramos que armazenam dados das barras e ramos da
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
 * TF_DRAM *ramoExemplo = NULL;
 * TF_GRAFO *grafoExemplo = NULL;
 * TF_DMED*medidaExemplo = NULL;
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

long int **leituraMedidas(char *folder, char *file, TF_DMED **medidas, TF_DRAM *ramos, long int numeroRamos, TF_DBAR *barras, long int numeroBarras, TF_GRAFO *grafo, double Sbase)
{
    char blocoLeitura[2000];                             /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;                                         /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    long int contador = 0, i, j, k, l, m, ind, aux, adj; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0;                            /* Variável com o número de linhas do arquivo a serem lidas. */
    FILE *arquivo;
    long int **numeroMedidas, nmed;
    double Vbase = 1;
    double regua;
    char text_aux[500];

    // Leitura dos dados de medidores
    strcpy(text_aux, folder);
    arquivo = fopen(strcat(text_aux, file), "r");

    // arquivo = fopen(folder,"r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo %s !!!\n", strcat(text_aux, file));
        exit(1);
    }

    numeroMedidas = (long int **)malloc(14 * sizeof(long int *));
    for (i = 0; i < 14; i++)
    {
        numeroMedidas[i] = (long int *)malloc(20 * sizeof(long int));
        for (j = 0; j < 20; j++)
        {
            numeroMedidas[i][j] = 0;
        }
    }

    // Aloca na memória espaço para as linhas
    while ((carac = fgetc(arquivo)) != EOF)
    {
        if (carac == '\n')
            numLinhas++;
    }
    rewind(arquivo);
    if ((((*medidas) = (TF_DMED *)malloc((numLinhas + 1) * sizeof(TF_DMED))) == NULL))
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as medidas !!!!");
        exit(1);
    }

    // Le o arquivo de curva de cargas até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        (*medidas)[contador].ligado = (getfield_int(dados, 1));
        (*medidas)[contador].tipo = (getfield_int(dados, 2));
        (*medidas)[contador].DE = (getfield_int(dados, 3));
        (*medidas)[contador].PARA = (getfield_int(dados, 4));
        (*medidas)[contador].fases = (getfield_int(dados, 5));
        (*medidas)[contador].id = contador;
        (*medidas)[contador].par = -1;

        (*medidas)[contador].h = 0;
        (*medidas)[contador].zmed = (getfield_double(dados, 6));
        (*medidas)[contador].sigma = (getfield_double(dados, 7));
        (*medidas)[contador].prec = (getfield_double(dados, 7));

        numeroMedidas[(*medidas)[contador].tipo][(*medidas)[contador].fases - 1]++;
        switch ((*medidas)[contador].tipo)
        {
        case 0: // Medidas nos ramos
        case 1:
        case 6:
        case 7:
        case 12: // PMU de corrente retangular real
        case 13: // PMU de corrente retangular imaginário
            for (i = 0; i < numeroRamos; i++)
            {
                if (((*medidas)[contador].DE == ramos[i].DE) && ((*medidas)[contador].PARA == ramos[i].PARA))
                {
                    (*medidas)[contador].k = ramos[i].k;
                    (*medidas)[contador].m = ramos[i].m;
                    (*medidas)[contador].ramo = i;
                }
                if (((*medidas)[contador].DE == ramos[i].PARA) && ((*medidas)[contador].PARA == ramos[i].DE))
                {
                    (*medidas)[contador].k = ramos[i].m;
                    (*medidas)[contador].m = ramos[i].k;
                    (*medidas)[contador].ramo = i;
                }
            }
            break;
        case 2: // Medidas nas barras
        case 3:
        case 4:
        case 5:
        case 8:  // PMU de tensão retangular real
        case 9:  // PMU de tensão retangular imaginário
        case 10: // PMU de injeção de corrente retangular real
        case 11: // PMU de injeção de corrente retangular imaginário
            for (i = 0; i < numeroBarras; i++)
            {
                if ((*medidas)[contador].DE == barras[i].ID)
                {
                    (*medidas)[contador].k = barras[i].i;
                }
                (*medidas)[contador].m = -1;
                (*medidas)[contador].ramo = -1;
            }
            break;
        }
        contador++;
    }
    fclose(arquivo);

    // Associa as medidas ao grafo e transforma em pu os dados medidos
    nmed = 0;
    for (i = 0; i < 14; i++)
    {
        for (j = 0; j < 8; j++)
        {
            nmed = nmed + numeroMedidas[i][j];
        }
    }

    //--------------------------------------------------------------------------
    // Associa os medidores ao grafo e transforma medidas em pu
    for (i = 0; i < nmed; i++)
    {
        k = (*medidas)[i].k;
        m = (*medidas)[i].m;

        (*medidas)[i].idAlim = grafo[k].idAlim;

        // Associa a medida ao grafo e transforma em pu o valor medido e sigma
        switch ((*medidas)[i].tipo)
        {
        case 0: // Medida de Fluxo de Potência Ativa em kW
            (*medidas)[i].zmed = (*medidas)[i].zmed / (Sbase / 1000);
            (*medidas)[i].sigma = (*medidas)[i].sigma / (Sbase / 1000);

            for (j = 0; j < grafo[k].numeroAdjacentes; j++)
            {
                if (grafo[k].adjacentes[j].idNo == m)
                {
                    adj = j;
                }
            }
            ind = grafo[k].adjacentes[adj].nmed;
            grafo[k].adjacentes[adj].medidores[ind] = &(*medidas)[i];
            grafo[k].adjacentes[adj].nmed++;

            (*medidas)[i].nvar = 12;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            regua = (double)m;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j + 6] = regua;
                (*medidas)[i].reguaH[j + 9] = -regua;
                regua += 0.1;
            }
            break;
        case 1: // Medida de Fluxo de Potência Reativa em kVAr
            (*medidas)[i].zmed = (*medidas)[i].zmed / (Sbase / 1000);
            (*medidas)[i].sigma = (*medidas)[i].sigma / (Sbase / 1000);

            for (j = 0; j < grafo[k].numeroAdjacentes; j++)
            {
                if (grafo[k].adjacentes[j].idNo == m)
                {
                    adj = j;
                }
            }
            ind = grafo[k].adjacentes[adj].nmed;
            grafo[k].adjacentes[adj].medidores[ind] = &(*medidas)[i];
            grafo[k].adjacentes[adj].nmed++;

            (*medidas)[i].nvar = 12;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            regua = (double)m;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j + 6] = regua;
                (*medidas)[i].reguaH[j + 9] = -regua;
                regua += 0.1;
            }
            break;
        case 2: // Medida de Injeção de Potência Ativa em kW
            (*medidas)[i].zmed = (*medidas)[i].zmed / (Sbase / 1000);
            (*medidas)[i].sigma = (*medidas)[i].sigma / (Sbase / 1000);

            ind = grafo[k].nmed;
            grafo[k].medidores[ind] = &(*medidas)[i];
            grafo[k].nmed++;
            grafo[k].nmedPQ++;

            (*medidas)[i].nvar = 6 + 6 * grafo[k].numeroAdjacentes;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            for (l = 0; l < grafo[k].numeroAdjacentes; l++)
            {
                regua = grafo[k].adjacentes[l].idNo;
                regua += 0.01;
                for (j = 0; j < 3; j++)
                {
                    (*medidas)[i].reguaH[6 + 6 * l + j] = regua;
                    (*medidas)[i].reguaH[6 + 6 * l + j + 3] = -regua;
                    regua += 0.1;
                }
            }
            break;
        case 3: // Medida de Injeção de Potência Reativa em kVAr
            (*medidas)[i].zmed = (*medidas)[i].zmed / (Sbase / 1000);
            (*medidas)[i].sigma = (*medidas)[i].sigma / (Sbase / 1000);

            ind = grafo[k].nmed;
            grafo[k].medidores[ind] = &(*medidas)[i];
            grafo[k].nmed++;
            grafo[k].nmedPQ++;

            (*medidas)[i].nvar = 6 + 6 * grafo[k].numeroAdjacentes;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            for (l = 0; l < grafo[k].numeroAdjacentes; l++)
            {
                regua = grafo[k].adjacentes[l].idNo;
                regua += 0.01;
                for (j = 0; j < 3; j++)
                {
                    (*medidas)[i].reguaH[6 + 6 * l + j] = regua;
                    (*medidas)[i].reguaH[6 + 6 * l + j + 3] = -regua;
                    regua += 0.1;
                }
            }
            break;
        case 4: // Medida de Magnitude de Tensão - kV
            switch ((*medidas)[i].fases)
            {
            case 1:
            case 2:
            case 3:
                Vbase = grafo[k].Vbase;
                break;
            case 4:
            case 5:
            case 6:
                Vbase = grafo[k].Vbase * (pow(3, 0.5));
                break;
            }
            (*medidas)[i].zmed = (*medidas)[i].zmed / (Vbase / 1000);
            (*medidas)[i].sigma = (*medidas)[i].sigma / (Vbase / 1000);

            ind = grafo[k].nmed;
            grafo[k].medidores[ind] = &(*medidas)[i];
            grafo[k].nmed++;

            (*medidas)[i].nvar = 6;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                regua += 0.1;
            }
            regua = (double)k;
            regua += 0.01;
            for (j = 3; j < 6; j++)
            {
                (*medidas)[i].reguaH[j] = -regua;
                regua += 0.1;
            }
            break;
        case 5: // Medida de Ângulo de tensão - graus
            (*medidas)[i].zmed = (*medidas)[i].zmed * PI / 180;
            (*medidas)[i].sigma = (*medidas)[i].sigma * PI / 180;
            ;

            ind = grafo[k].nmed;
            grafo[k].medidores[ind] = &(*medidas)[i];
            grafo[k].nmed++;

            (*medidas)[i].nvar = 3;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = -regua;
                regua += 0.1;
            }
            break;
        case 6: // Medida de Magnitude de Corrente em A - Fluxo
            Vbase = grafo[k].Vbase;
            (*medidas)[i].zmed = (*medidas)[i].zmed / ((Sbase / 1000) / (pow(3, 0.5) * (Vbase / 1000)));
            (*medidas)[i].sigma = (*medidas)[i].sigma / ((Sbase / 1000) / (pow(3, 0.5) * (Vbase / 1000)));

            for (j = 0; j < grafo[k].numeroAdjacentes; j++)
            {
                if (grafo[k].adjacentes[j].idNo == m)
                {
                    adj = j;
                }
            }
            ind = grafo[k].adjacentes[adj].nmed;
            grafo[k].adjacentes[adj].medidores[ind] = &(*medidas)[i];
            grafo[k].adjacentes[adj].nmed++;

            (*medidas)[i].nvar = 12;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            regua = (double)m;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j + 6] = regua;
                (*medidas)[i].reguaH[j + 9] = -regua;
                regua += 0.1;
            }
            break;
        case 7: // Medida de Ângulo de Corrente em graus
            (*medidas)[i].zmed = (*medidas)[i].zmed * PI / 180;
            (*medidas)[i].sigma = (*medidas)[i].sigma * PI / 180;

            for (j = 0; j < grafo[k].numeroAdjacentes; j++)
            {
                if (grafo[k].adjacentes[j].idNo == m)
                {
                    adj = j;
                }
            }
            ind = grafo[k].adjacentes[adj].nmed;
            grafo[k].adjacentes[adj].medidores[ind] = &(*medidas)[i];
            grafo[k].adjacentes[adj].nmed++;

            (*medidas)[i].nvar = 12;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            regua = (double)m;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j + 6] = regua;
                (*medidas)[i].reguaH[j + 9] = -regua;
                regua += 0.1;
            }
            break;
        case 8: // Medida PMU de tensão retangular Real
            switch ((*medidas)[i].fases)
            {
            case 1:
            case 2:
            case 3:
                Vbase = grafo[k].Vbase;
                break;
            case 4:
            case 5:
            case 6:
                Vbase = grafo[k].Vbase * (pow(3, 0.5));
                break;
            }
            (*medidas)[i].zmed = (*medidas)[i].zmed / (Vbase / 1000);
            (*medidas)[i].sigma = (*medidas)[i].sigma / (Vbase / 1000);

            ind = grafo[k].nmed;
            grafo[k].medidores[ind] = &(*medidas)[i];
            grafo[k].nmed++;

            (*medidas)[i].nvar = 3;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                regua += 0.1;
            }
            break;
        case 9: // Medida PMU de tensão retangular Imaginário
            switch ((*medidas)[i].fases)
            {
            case 1:
            case 2:
            case 3:
                Vbase = grafo[k].Vbase;
                break;
            case 4:
            case 5:
            case 6:
                Vbase = grafo[k].Vbase * (pow(3, 0.5));
                break;
            }
            (*medidas)[i].zmed = (*medidas)[i].zmed / (Vbase / 1000);
            (*medidas)[i].sigma = (*medidas)[i].sigma / (Vbase / 1000);

            ind = grafo[k].nmed;
            grafo[k].medidores[ind] = &(*medidas)[i];
            grafo[k].nmed++;

            (*medidas)[i].nvar = 3;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = -regua;
                regua += 0.1;
            }
            break;
        case 10: // Medida PMU de injeção de corrente retangular real
            Vbase = grafo[k].Vbase;
            (*medidas)[i].zmed = (*medidas)[i].zmed / ((Sbase / 1000) / ((Vbase / 1000)));
            (*medidas)[i].sigma = (*medidas)[i].sigma / ((Sbase / 1000) / ((Vbase / 1000)));

            ind = grafo[k].nmed;
            grafo[k].medidores[ind] = &(*medidas)[i];
            grafo[k].nmed++;

            (*medidas)[i].nvar = 6 + 6 * grafo[k].numeroAdjacentes;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            for (l = 0; l < grafo[k].numeroAdjacentes; l++)
            {
                regua = grafo[k].adjacentes[l].idNo;
                regua += 0.01;
                for (j = 0; j < 3; j++)
                {
                    (*medidas)[i].reguaH[6 + 6 * l + j] = regua;
                    (*medidas)[i].reguaH[6 + 6 * l + j + 3] = -regua;
                    regua += 0.1;
                }
            }
            break;
        case 11: // Medida PMU de injeção de corrente retangular imaginário
            Vbase = grafo[k].Vbase;
            (*medidas)[i].zmed = (*medidas)[i].zmed / ((Sbase / 1000) / ((Vbase / 1000)));
            (*medidas)[i].sigma = (*medidas)[i].sigma / ((Sbase / 1000) / ((Vbase / 1000)));

            ind = grafo[k].nmed;
            grafo[k].medidores[ind] = &(*medidas)[i];
            grafo[k].nmed++;

            (*medidas)[i].nvar = 6 + 6 * grafo[k].numeroAdjacentes;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            for (l = 0; l < grafo[k].numeroAdjacentes; l++)
            {
                regua = grafo[k].adjacentes[l].idNo;
                regua += 0.01;
                for (j = 0; j < 3; j++)
                {
                    (*medidas)[i].reguaH[6 + 6 * l + j] = regua;
                    (*medidas)[i].reguaH[6 + 6 * l + j + 3] = -regua;
                    regua += 0.1;
                }
            }

            break;
        case 12: // Medida PMU de corrente retangular real
            Vbase = grafo[k].Vbase;
            (*medidas)[i].zmed = (*medidas)[i].zmed / ((Sbase / 1000) / ((Vbase / 1000)));
            (*medidas)[i].sigma = (*medidas)[i].sigma / ((Sbase / 1000) / ((Vbase / 1000)));

            for (j = 0; j < grafo[k].numeroAdjacentes; j++)
            {
                if (grafo[k].adjacentes[j].idNo == m)
                {
                    adj = j;
                }
            }
            ind = grafo[k].adjacentes[adj].nmed;
            grafo[k].adjacentes[adj].medidores[ind] = &(*medidas)[i];
            grafo[k].adjacentes[adj].nmed++;

            (*medidas)[i].nvar = 12;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            regua = (double)m;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j + 6] = regua;
                (*medidas)[i].reguaH[j + 9] = -regua;
                regua += 0.1;
            }
            break;
        case 13: // Medida PMU de corrente retangular Imaginário
            Vbase = grafo[k].Vbase;
            (*medidas)[i].zmed = (*medidas)[i].zmed / ((Sbase / 1000) / ((Vbase / 1000)));
            (*medidas)[i].sigma = (*medidas)[i].sigma / ((Sbase / 1000) / ((Vbase / 1000)));

            for (j = 0; j < grafo[k].numeroAdjacentes; j++)
            {
                if (grafo[k].adjacentes[j].idNo == m)
                {
                    adj = j;
                }
            }
            ind = grafo[k].adjacentes[adj].nmed;
            grafo[k].adjacentes[adj].medidores[ind] = &(*medidas)[i];
            grafo[k].adjacentes[adj].nmed++;

            (*medidas)[i].nvar = 12;
            (*medidas)[i].reguaH = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            (*medidas)[i].H = (double *)malloc((*medidas)[i].nvar * sizeof(double));
            for (j = 0; j < (*medidas)[i].nvar; j++)
            {
                (*medidas)[i].H[j] = 0;
            }

            regua = (double)k;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j] = regua;
                (*medidas)[i].reguaH[j + 3] = -regua;
                regua += 0.1;
            }
            regua = (double)m;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medidas)[i].reguaH[j + 6] = regua;
                (*medidas)[i].reguaH[j + 9] = -regua;
                regua += 0.1;
            }
            break;
        }
    }

    //--------------------------------------------------------------------------
    // Associa os medidores ao grafo e transforma medidas em pu (equivalente de corrente para o AMB e Baran precisa do par de medida)
    for (i = 0; i < nmed; i++)
    {
        k = (*medidas)[i].k;
        m = (*medidas)[i].m;

        // Associa a medida ao grafo e transforma em pu o valor medido e sigma
        switch ((*medidas)[i].tipo)
        {
        case 0: // Medida de Fluxo de Potência Ativa em kW
            for (j = 0; j < nmed; j++)
            {
                if (((*medidas)[j].tipo == 1) && ((*medidas)[j].k == k) && ((*medidas)[j].m == m))
                {
                    (*medidas)[i].par = j;
                    (*medidas)[j].par = i;
                    j = nmed;
                }
            }
            break;
        case 2: // Medida de Injeção de Potência Ativa em kW
            for (j = 0; j < nmed; j++)
            {
                if (((*medidas)[j].tipo == 3) && ((*medidas)[j].k == k) && ((*medidas)[j].m == m))
                {
                    (*medidas)[i].par = j;
                    (*medidas)[j].par = i;
                    j = nmed;
                }
            }
            break;
        case 4: // Medida de Magnitude de Tensão - kV
            for (j = 0; j < nmed; j++)
            {
                if (((*medidas)[j].tipo == 5) && ((*medidas)[j].k == k) && ((*medidas)[j].m == m))
                {
                    (*medidas)[i].par = j;
                    (*medidas)[j].par = i;
                    j = nmed;
                }
            }
            break;
        case 6: // Medida de Magnitude de Corrente em A
            for (j = 0; j < nmed; i++)
            {
                if (((*medidas)[j].tipo == 7) && ((*medidas)[j].k == k) && ((*medidas)[j].m == m))
                {
                    (*medidas)[i].par = j;
                    (*medidas)[j].par = i;
                    j = nmed;
                }
            }
            break;
        }
    }

    return (numeroMedidas);
}

//------------------------------------------------------------------------------
//
// FUNÇÕES DE LEITURA DE DADOS ANALÓGICOS E DISCRETOS DO SISTEMA SUPERVISÓRIO
//
//------------------------------------------------------------------------------
/**
 * @brief Função principal para a leitura dos arquivos com dados das medidas analógicas do sistema supervisório
 *
 * Essa função realiza a leitura  de arquivo com dados de medidas analógicas do sistema supervisório dentro da pasta com os dados da rede elétrica.
 * O local da pasta é recebido como parâmetro @p folder assim como o nome do arquivo no parâmetro @p file. Além disso o arquivo possui como
 * separação dos dados os marcadores especificados. Realiza alocação de memória para armazenar os dados de medidas trifásicas da rede elétrica.
 * Recebe como parâmetros de entrada e saída um ponteiro para ponteiro do tipo TF_DMED@p **medidas que armazena dados das medidas da rede elétrica, e também
 * uma estrutura de dados TF_GRAFO @p grafo para associar os medidores à topologia da rede elétrica.
 * Recebe como parâmetros de entrada um ponteiro do tipo DBAR @p *barras e um ponterio do tipo TF_DRAM @p ramos que armazenam dados das barras e ramos da
 * rede elétrica para serem associadas com os medidore, e respectivas quantidades totais nos parâmetros @p numeroBarras e @p numeroRamos. Além disto recebe
 * como parâmetro de entrada a Potência Base da rede elétrica para realizar os cálculos em pu.
 * A função retorna @c char* indicando a pasta selecionada para os arquivos serem lidos.
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
long int **leituraMedidasPrev(char *folder, TF_DPREV **prev, int *numprev, int *nmed, TF_DRAM *ramos, long int numeroRamos, TF_DBAR *barras, long int numeroBarras, TF_GRAFO *grafo, double Sbase)
{
    char blocoLeitura[2000];                             /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;                                         /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    long int contador = 0, i, j, k, l, m, ind, aux, adj; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0, numCol = 1;                /* Variável com o número de linhas do arquivo a serem lidas. */
    FILE *arquivo;
    long int **numeroMedidas, namostras;
    double Vbase = 1;
    long int t;
    int dateaux1, dateaux2;
    double regua;
    char text_aux[500];
    
    int *Vsearch;
    int namostras_total=0;
    int ID;


    // Leitura do arquivo dedados das medidas de previsão
    strcpy(text_aux, folder);
    arquivo = fopen(strcat(text_aux, "DMEDNC.csv"), "r");

    // arquivo = fopen(folder,"r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo %s !!!\n", strcat(text_aux, "DMEDNC.csv"));
        exit(1);
    }

    // Aloca na memória espaço para as linhas
    while ((carac = fgetc(arquivo)) != EOF)
    {
        if (carac == '\n')
            numLinhas++;
    }
    rewind(arquivo);

    (*nmed) = numLinhas; // numero de medidas

    // aloca espaço para o vetor de busca
    if (((Vsearch = (int * )malloc((numLinhas + 5) * sizeof(int))) == NULL))
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para a série temporal !!!!");
        exit(1);
    }


    // aloca memoria para o numero de medidas
    (numeroMedidas) = (long int **)malloc(14 * sizeof(long int *));
    for (i = 0; i < 14; i++)
    {
        numeroMedidas[i] = (long int *)malloc(20 * sizeof(long int));
        for (j = 0; j < 20; j++)
        {
            numeroMedidas[i][j] = 0;
        }
    }

    if ((((*prev ) = (TF_DPREV *)malloc(((*nmed) + 1) * sizeof(TF_DPREV))) == NULL))
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para a série temporal !!!!");
        exit(1);
    }


    contador = 0;
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        (*prev)[contador].id = getfield_int(blocoLeitura, 1); // Leitura do id da medida
        Vsearch[(*prev)[contador].id] = contador; // vetor de busca
        (*prev)[contador].DMED.tipo = getfield_int(blocoLeitura, 2);  // Leitura do tipo da medida
        (*prev)[contador].DMED.DE = getfield_int(blocoLeitura, 3);    // leitura barra DE
        (*prev)[contador].DMED.PARA = getfield_int(blocoLeitura, 4);  // leitura barra PARA
        (*prev)[contador].DMED.fases = getfield_int(blocoLeitura, 5); // leitura numero de fases
        (*prev)[contador].amostras = getfield_int(blocoLeitura, 6);   // leitura numero de amostras
        (*numprev) = (*prev)[contador].amostras;                      // leitura do numero de amostras
        if (((*prev)[contador].time_stamp = (int *)malloc(((*numprev) + 1) * sizeof(int))) == NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para a série temporal !!!!");
            exit(1);
        }
        if (((*prev)[contador].day = (int *)malloc(((*numprev) + 1) * sizeof(int))) == NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para a série temporal !!!!");
            exit(1);
        }
        if (((*prev)[contador].hour = (int *)malloc(((*numprev) + 1) * sizeof(int))) == NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para a série temporal !!!!");
            exit(1);
        }
        if (((*prev)[contador].min = (int *)malloc(((*numprev) + 1) * sizeof(int))) == NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para a série temporal !!!!");
            exit(1);
        }
        if (((*prev)[contador].prev = (double *)malloc(((*numprev) + 1) * sizeof(double))) == NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para a série temporal !!!!");
            exit(1);
        }
        if (((*prev)[contador].prec = (double *)malloc(((*numprev) + 1) * sizeof(double))) == NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para a série temporal !!!!");
            exit(1);
        }
        numeroMedidas[(*prev)[contador].DMED.tipo][(*prev)[contador].DMED.fases - 1]++;
        
        switch ((*prev)[contador].DMED.tipo)
        {
        case 0: // Medidas nos ramos
        case 1:
        case 6:
        case 7:
        case 12: // PMU de corrente retangular real
        case 13: // PMU de corrente retangular imaginário
            for (i = 0; i < numeroRamos; i++)
            {
                if (((*prev)[contador].DMED.DE == ramos[i].DE) && ((*prev)[contador].DMED.PARA == ramos[i].PARA))
                {
                    (*prev)[contador].DMED.k = ramos[i].k;
                    (*prev)[contador].DMED.m = ramos[i].m;
                    (*prev)[contador].DMED.ramo = i;
                }
                if (((*prev)[contador].DMED.DE == ramos[i].PARA) && ((*prev)[contador].DMED.PARA == ramos[i].DE))
                {
                    (*prev)[contador].DMED.k = ramos[i].m;
                    (*prev)[contador].DMED.m = ramos[i].k;
                    (*prev)[contador].DMED.ramo = i;
                }
            }
            break;
        case 2: // Medidas nas barras
        case 3:
        case 4:
        case 5:
        case 8:  // PMU de tensão retangular real
        case 9:  // PMU de tensão retangular imaginário
        case 10: // PMU de injeção de corrente retangular real
        case 11: // PMU de injeção de corrente retangular imaginário
            for (i = 0; i < numeroBarras; i++)
            {
                if ((*prev)[contador].DMED.DE == barras[i].ID)
                {
                    (*prev)[contador].DMED.k = barras[i].i;
                }
                (*prev)[contador].DMED.m = -1;
                (*prev)[contador].DMED.ramo = -1;
            }
            break;
        }
        
        (*prev)[contador].inst_atual = 0;
        (*prev)[contador].fre_amost = 0;
        (*prev)[contador].nsamples = namostras;
        (*prev)[contador].base = 0;
        namostras_total+=namostras;
        contador++;

    }

    fclose(arquivo);

    BaseMedida(grafo, Sbase, (*prev), (*nmed));

    t = 0;

    strcpy(text_aux, folder);
    arquivo = fopen(strcat(text_aux, "DPREV.csv"), "r");

    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;
        ID=getfield_int(dados,1); 
        contador=Vsearch[ID];
        t=getfield_int(dados,5)-1;
        (*prev)[contador].prev[t] = getfield_double(dados, 2);
        (*prev)[contador].prec[t] = getfield_double(dados, 3);
        (*prev)[contador].time_stamp[t] = getfield_int(dados, 4);
        dateaux2 = (*prev)[contador].time_stamp[t];
        dateaux1 = dateaux2 % 10000;
        (*prev)[contador].day[t] = (dateaux2 - dateaux1) / 10000;
        dateaux2 = dateaux1;
        dateaux1 = dateaux2 % 100;
        (*prev)[contador].hour[t] = (dateaux2 - dateaux1) / 100;
        (*prev)[contador].min[t] = dateaux1;

    }
    fclose(arquivo);

    for (size_t contador = 0; contador < (*nmed); contador++)
    {
        (*prev)[contador].DMED.zmed = (*prev)[contador].prev[0];
        (*prev)[contador].DMED.sigma = (*prev)[contador].prec[0];
        (*prev)[contador].DMED.prec = (*prev)[contador].prec[0];
    }

    free(Vsearch);
    return (numeroMedidas);
}

/**
 * @brief Função que associa medidas no grafo da rede trifásica
 *
 * Esta função recebe @p grafo do tipo TF_GRAFO com as informações sobre a rede elétrica e associa a ele as medidas na
 * estrutura @p medida do tipo TF_DMED, @p Sbase double com a potencia base da rede
 *
 * @param grafo vetor do tipo TF_GRAFO com as informações sobre a rede elétrica
 * @param medida vetor do tipo TF_DMED com as informações sobre as medidas disponiveis
 * @param Sbase double com a potencia base da rede
 */

void AssoMedGraf(TF_GRAFO *grafo, TF_DMED *medida, double Sbase)
{
    int j, k, m, i, l;
    int ind;
    int adj;
    double regua;
    k = (*medida).k;
    m = (*medida).m;
    double Vbase;

    (*medida).idAlim = grafo[k].idAlim;

    // Associa a medida ao grafo e transforma em pu o valor medido e sigma
    switch ((*medida).tipo)
    {
    case 0: // Medida de Fluxo de Potência Ativa em kW
        (*medida).zmed = (*medida).zmed / (Sbase / 1000);
        (*medida).sigma = (*medida).sigma / (Sbase / 1000);

        for (j = 0; j < grafo[k].numeroAdjacentes; j++)
        {
            if (grafo[k].adjacentes[j].idNo == m)
            {
                adj = j;
            }
        }
        ind = grafo[k].adjacentes[adj].nmed;
        grafo[k].adjacentes[adj].medidores[ind] = &(*medida);
        grafo[k].adjacentes[adj].nmed++;

        (*medida).nvar = 12;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        regua = (double)m;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j + 6] = regua;
            (*medida).reguaH[j + 9] = -regua;
            regua += 0.1;
        }
        break;
    case 1: // Medida de Fluxo de Potência Reativa em kVAr
        (*medida).zmed = (*medida).zmed / (Sbase / 1000);
        (*medida).sigma = (*medida).sigma / (Sbase / 1000);

        for (j = 0; j < grafo[k].numeroAdjacentes; j++)
        {
            if (grafo[k].adjacentes[j].idNo == m)
            {
                adj = j;
            }
        }
        ind = grafo[k].adjacentes[adj].nmed;
        grafo[k].adjacentes[adj].medidores[ind] = &(*medida);
        grafo[k].adjacentes[adj].nmed++;

        (*medida).nvar = 12;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        regua = (double)m;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j + 6] = regua;
            (*medida).reguaH[j + 9] = -regua;
            regua += 0.1;
        }
        break;
    case 2: // Medida de Injeção de Potência Ativa em kW
        (*medida).zmed = (*medida).zmed / (Sbase / 1000);
        (*medida).sigma = (*medida).sigma / (Sbase / 1000);

        ind = grafo[k].nmed;
        grafo[k].medidores[ind] = &(*medida);
        grafo[k].nmed++;
        grafo[k].nmedPQ++;

        (*medida).nvar = 6 + 6 * grafo[k].numeroAdjacentes;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        for (l = 0; l < grafo[k].numeroAdjacentes; l++)
        {
            regua = grafo[k].adjacentes[l].idNo;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medida).reguaH[6 + 6 * l + j] = regua;
                (*medida).reguaH[6 + 6 * l + j + 3] = -regua;
                regua += 0.1;
            }
        }
        break;
    case 3: // Medida de Injeção de Potência Reativa em kVAr
        (*medida).zmed = (*medida).zmed / (Sbase / 1000);
        (*medida).sigma = (*medida).sigma / (Sbase / 1000);

        ind = grafo[k].nmed;
        grafo[k].medidores[ind] = &(*medida);
        grafo[k].nmed++;
        grafo[k].nmedPQ++;

        (*medida).nvar = 6 + 6 * grafo[k].numeroAdjacentes;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        for (l = 0; l < grafo[k].numeroAdjacentes; l++)
        {
            regua = grafo[k].adjacentes[l].idNo;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medida).reguaH[6 + 6 * l + j] = regua;
                (*medida).reguaH[6 + 6 * l + j + 3] = -regua;
                regua += 0.1;
            }
        }
        break;
    case 4: // Medida de Magnitude de Tensão - kV
        switch ((*medida).fases)
        {
        case 1:
        case 2:
        case 3:
            Vbase = grafo[k].Vbase;
            break;
        case 4:
        case 5:
        case 6:
            Vbase = grafo[k].Vbase * (pow(3, 0.5));
            break;
        }
        (*medida).zmed = (*medida).zmed / (Vbase / 1000);
        (*medida).sigma = (*medida).sigma / (Vbase / 1000);

        ind = grafo[k].nmed;
        grafo[k].medidores[ind] = &(*medida);
        grafo[k].nmed++;

        (*medida).nvar = 6;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            regua += 0.1;
        }
        regua = (double)k;
        regua += 0.01;
        for (j = 3; j < 6; j++)
        {
            (*medida).reguaH[j] = -regua;
            regua += 0.1;
        }
        break;
    case 5: // Medida de Ângulo de tensão - graus
        (*medida).zmed = (*medida).zmed * PI / 180;
        (*medida).sigma = (*medida).sigma * PI / 180;
        ;

        ind = grafo[k].nmed;
        grafo[k].medidores[ind] = &(*medida);
        grafo[k].nmed++;

        (*medida).nvar = 3;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = -regua;
            regua += 0.1;
        }
        break;
    case 6: // Medida de Magnitude de Corrente em A - Fluxo
        Vbase = grafo[k].Vbase;
        (*medida).zmed = (*medida).zmed / ((Sbase / 1000) / (pow(3, 0.5) * (Vbase / 1000)));
        (*medida).sigma = (*medida).sigma / ((Sbase / 1000) / (pow(3, 0.5) * (Vbase / 1000)));

        for (j = 0; j < grafo[k].numeroAdjacentes; j++)
        {
            if (grafo[k].adjacentes[j].idNo == m)
            {
                adj = j;
            }
        }
        ind = grafo[k].adjacentes[adj].nmed;
        grafo[k].adjacentes[adj].medidores[ind] = &(*medida);
        grafo[k].adjacentes[adj].nmed++;

        (*medida).nvar = 12;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        regua = (double)m;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j + 6] = regua;
            (*medida).reguaH[j + 9] = -regua;
            regua += 0.1;
        }
        break;
    case 7: // Medida de Ângulo de Corrente em graus
        (*medida).zmed = (*medida).zmed * PI / 180;
        (*medida).sigma = (*medida).sigma * PI / 180;

        for (j = 0; j < grafo[k].numeroAdjacentes; j++)
        {
            if (grafo[k].adjacentes[j].idNo == m)
            {
                adj = j;
            }
        }
        ind = grafo[k].adjacentes[adj].nmed;
        grafo[k].adjacentes[adj].medidores[ind] = &(*medida);
        grafo[k].adjacentes[adj].nmed++;

        (*medida).nvar = 12;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        regua = (double)m;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j + 6] = regua;
            (*medida).reguaH[j + 9] = -regua;
            regua += 0.1;
        }
        break;
    case 8: // Medida PMU de tensão retangular Real
        switch ((*medida).fases)
        {
        case 1:
        case 2:
        case 3:
            Vbase = grafo[k].Vbase;
            break;
        case 4:
        case 5:
        case 6:
            Vbase = grafo[k].Vbase * (pow(3, 0.5));
            break;
        }
        (*medida).zmed = (*medida).zmed / (Vbase / 1000);
        (*medida).sigma = (*medida).sigma / (Vbase / 1000);

        ind = grafo[k].nmed;
        grafo[k].medidores[ind] = &(*medida);
        grafo[k].nmed++;

        (*medida).nvar = 3;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            regua += 0.1;
        }
        break;
    case 9: // Medida PMU de tensão retangular Imaginário
        switch ((*medida).fases)
        {
        case 1:
        case 2:
        case 3:
            Vbase = grafo[k].Vbase;
            break;
        case 4:
        case 5:
        case 6:
            Vbase = grafo[k].Vbase * (pow(3, 0.5));
            break;
        }
        (*medida).zmed = (*medida).zmed / (Vbase / 1000);
        (*medida).sigma = (*medida).sigma / (Vbase / 1000);

        ind = grafo[k].nmed;
        grafo[k].medidores[ind] = &(*medida);
        grafo[k].nmed++;

        (*medida).nvar = 3;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = -regua;
            regua += 0.1;
        }
        break;
    case 10: // Medida PMU de injeção de corrente retangular real
        Vbase = grafo[k].Vbase;
        (*medida).zmed = (*medida).zmed / ((Sbase / 1000) / ((Vbase / 1000)));
        (*medida).sigma = (*medida).sigma / ((Sbase / 1000) / ((Vbase / 1000)));

        ind = grafo[k].nmed;
        grafo[k].medidores[ind] = &(*medida);
        grafo[k].nmed++;

        (*medida).nvar = 6 + 6 * grafo[k].numeroAdjacentes;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        for (l = 0; l < grafo[k].numeroAdjacentes; l++)
        {
            regua = grafo[k].adjacentes[l].idNo;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medida).reguaH[6 + 6 * l + j] = regua;
                (*medida).reguaH[6 + 6 * l + j + 3] = -regua;
                regua += 0.1;
            }
        }
        break;
    case 11: // Medida PMU de injeção de corrente retangular imaginário
        Vbase = grafo[k].Vbase;
        (*medida).zmed = (*medida).zmed / ((Sbase / 1000) / ((Vbase / 1000)));
        (*medida).sigma = (*medida).sigma / ((Sbase / 1000) / ((Vbase / 1000)));

        ind = grafo[k].nmed;
        grafo[k].medidores[ind] = &(*medida);
        grafo[k].nmed++;

        (*medida).nvar = 6 + 6 * grafo[k].numeroAdjacentes;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        for (l = 0; l < grafo[k].numeroAdjacentes; l++)
        {
            regua = grafo[k].adjacentes[l].idNo;
            regua += 0.01;
            for (j = 0; j < 3; j++)
            {
                (*medida).reguaH[6 + 6 * l + j] = regua;
                (*medida).reguaH[6 + 6 * l + j + 3] = -regua;
                regua += 0.1;
            }
        }

        break;
    case 12: // Medida PMU de corrente retangular real
        Vbase = grafo[k].Vbase;
        (*medida).zmed = (*medida).zmed / ((Sbase / 1000) / ((Vbase / 1000)));
        (*medida).sigma = (*medida).sigma / ((Sbase / 1000) / ((Vbase / 1000)));

        for (j = 0; j < grafo[k].numeroAdjacentes; j++)
        {
            if (grafo[k].adjacentes[j].idNo == m)
            {
                adj = j;
            }
        }
        ind = grafo[k].adjacentes[adj].nmed;
        grafo[k].adjacentes[adj].medidores[ind] = &(*medida);
        grafo[k].adjacentes[adj].nmed++;

        (*medida).nvar = 12;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        regua = (double)m;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j + 6] = regua;
            (*medida).reguaH[j + 9] = -regua;
            regua += 0.1;
        }
        break;
    case 13: // Medida PMU de corrente retangular Imaginário
        Vbase = grafo[k].Vbase;
        (*medida).zmed = (*medida).zmed / ((Sbase / 1000) / ((Vbase / 1000)));
        (*medida).sigma = (*medida).sigma / ((Sbase / 1000) / ((Vbase / 1000)));

        for (j = 0; j < grafo[k].numeroAdjacentes; j++)
        {
            if (grafo[k].adjacentes[j].idNo == m)
            {
                adj = j;
            }
        }
        ind = grafo[k].adjacentes[adj].nmed;
        grafo[k].adjacentes[adj].medidores[ind] = &(*medida);
        grafo[k].adjacentes[adj].nmed++;

        (*medida).nvar = 12;
        (*medida).reguaH = (double *)malloc((*medida).nvar * sizeof(double));
        (*medida).H = (double *)malloc((*medida).nvar * sizeof(double));
        for (j = 0; j < (*medida).nvar; j++)
        {
            (*medida).H[j] = 0;
        }

        regua = (double)k;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j] = regua;
            (*medida).reguaH[j + 3] = -regua;
            regua += 0.1;
        }
        regua = (double)m;
        regua += 0.01;
        for (j = 0; j < 3; j++)
        {
            (*medida).reguaH[j + 6] = regua;
            (*medida).reguaH[j + 9] = -regua;
            regua += 0.1;
        }
        break;
    }
}

/**
 * @brief Associa medidores que existem pares
 *
 *
 * @param medidas vetor do tipo TF_DMED com as informações sobre as medidas disponiveis
 * @param nmed inteiro com o numero de medidas da rede
 */

void AssoMedidoresPares(TF_DMED *medidas, int nmed)
{
    int k, m, i, j;
    for (i = 0; i < nmed; i++)
    {
        k = medidas[i].k;
        m = medidas[i].m;

        // Associa a medida ao grafo e transforma em pu o valor medido e sigma
        switch (medidas[i].tipo)
        {
        case 0: // Medida de Fluxo de Potência Ativa em kW
            for (j = 0; j < nmed; j++)
            {
                if ((medidas[j].tipo == 1) && (medidas[j].k == k) && (medidas[j].m == m))
                {
                    medidas[i].par = j;
                    medidas[j].par = i;
                    j = nmed;
                }
            }
            break;
        case 2: // Medida de Injeção de Potência Ativa em kW
            for (j = 0; j < nmed; j++)
            {
                if ((medidas[j].tipo == 3) && (medidas[j].k == k) && (medidas[j].m == m))
                {
                    medidas[i].par = j;
                    medidas[j].par = i;
                    j = nmed;
                }
            }
            break;
        case 4: // Medida de Magnitude de Tensão - kV
            for (j = 0; j < nmed; j++)
            {
                if ((medidas[j].tipo == 5) && (medidas[j].k == k) && (medidas[j].m == m))
                {
                    medidas[i].par = j;
                    medidas[j].par = i;
                    j = nmed;
                }
            }
            break;
        case 6: // Medida de Magnitude de Corrente em A
            for (j = 0; j < nmed; i++)
            {
                if ((medidas[j].tipo == 7) && (medidas[j].k == k) && (medidas[j].m == m))
                {
                    medidas[i].par = j;
                    medidas[j].par = i;
                    j = nmed;
                }
            }
            break;
        }
    }
}

/**
 * @brief Função auxiliar para determinar a base de uma medida prevista e inseri-la no grafo da rede
 *
 * Recebe como parametro de entrada @p grafo do tipo TF_GRAFO com as informações sobre a rede elétrica trifásica,
 * @p Sbase double com a potência base da rede, @p prev estrutura do tipo TF_DPREV com os dados de previsão, @p nmed
 * inteiro com o numero de medidas da rede
 *
 * @param grafo estrutura do do tipo TF_GRAFO com as informações sobre a rede elétrica trifásica
 * @param Sbase double com a potencia base da rede
 * @param prev estrutura do tipo TF_DPREV com os dados de previsão da rede
 * @param nmed inteiro com o numero de medidas
 */
void BaseMedida(TF_GRAFO *grafo, double Sbase, TF_DPREV *prev, int nmed)
{
    int k;
    double Vbase;
    for (size_t i = 0; i < nmed; i++)
    {
        k = prev[i].DMED.k;
        switch (prev[i].DMED.tipo)
        {
        case 0: // Medida de Fluxo de Potência Ativa em kW
            prev[i].base = (Sbase / 1000);
            break;
        case 1: // Medida de Fluxo de Potência Reativa em kVAr
            prev[i].base = (Sbase / 1000);
            break;
        case 2: // Medida de Injeção de Potência Ativa em kW
            prev[i].base = (Sbase / 1000);
            break;
        case 3: // Medida de Injeção de Potência Reativa em kVAr
            prev[i].base = (Sbase / 1000);
            break;
        case 4: // Medida de Magnitude de Tensão - kV
            switch (prev[i].DMED.fases)
            {
            case 1:
            case 2:
            case 3:
                Vbase = grafo[k].Vbase;
                break;
            case 4:
            case 5:
            case 6:
                Vbase = grafo[k].Vbase * (pow(3, 0.5));
                break;
            }
            prev[i].base = (Vbase / 1000);
            break;
        case 5: // Medida de Ângulo de tensão - graus
            prev[i].base = 1 / (PI / 180);
            break;
        case 6: // Medida de Magnitude de Corrente em A - Fluxo
            Vbase = grafo[k].Vbase;
            prev[i].base = ((Sbase / 1000) / (pow(3, 0.5) * (Vbase / 1000)));
            break;
        case 7: // Medida de Ângulo de Corrente em graus
            prev[i].base = 1 / (PI / 180);
            break;
        case 8: // Medida PMU de tensão retangular Real
            switch (prev[i].DMED.fases)
            {
            case 1:
            case 2:
            case 3:
                Vbase = grafo[k].Vbase;
                break;
            case 4:
            case 5:
            case 6:
                Vbase = grafo[k].Vbase * (pow(3, 0.5));
                break;
            }
            prev[i].base = (Vbase / 1000);

            break;
        case 9: // Medida PMU de tensão retangular Imaginário
            switch (prev[i].DMED.fases)
            {
            case 1:
            case 2:
            case 3:
                Vbase = grafo[k].Vbase;
                break;
            case 4:
            case 5:
            case 6:
                Vbase = grafo[k].Vbase * (pow(3, 0.5));
                break;
            }
            prev[i].base = (Vbase / 1000);
            break;
        case 10: // Medida PMU de injeção de corrente retangular real
            Vbase = grafo[k].Vbase;
            prev[i].base = ((Sbase / 1000) / ((Vbase / 1000)));

            break;
        case 11: // Medida PMU de injeção de corrente retangular imaginário
            Vbase = grafo[k].Vbase;
            prev[i].base = ((Sbase / 1000) / ((Vbase / 1000)));
            break;
        case 12: // Medida PMU de corrente retangular real
            Vbase = grafo[k].Vbase;
            prev[i].base = ((Sbase / 1000) / ((Vbase / 1000)));

            break;
        case 13: // Medida PMU de corrente retangular Imaginário
            Vbase = grafo[k].Vbase;
            prev[i].base = ((Sbase / 1000) / ((Vbase / 1000)));
            break;
        }
    }
}

// // //------------------------------------------------------------------------------
// // //
// // // FUNÇÕES DE IMPRESSÃO DE DADOS
// // //
// // //------------------------------------------------------------------------------
// // // Imprime arquivo com informações da rede elerica para conferências
// // /**
// //  * @brief Função auxiliar para salvar dados da rede elétrica em arquivo externo "dadosRedeEletrica.dad"
// //  *
// //  * Essa função salva um arquivo de texto "dadosRedeEletrica.dad" contendo as principais informações da rede elétrica para conferência
// //  * de informaçÕes e validações do processamento de dados da rede elétrica.
// //  * A função retorna @c void.
// //  *
// //  * @param barras estrutura de dados com informações das barras a serem salvas
// //  * @param numeroBarras quantidade total de barras
// //  * @param ramos estrutura de dados com informações dos ramos a serem salvas
// //  * @param numeroRamos quantidade total de medidas
// //  * @param medidas estrutura de dados com informações das medidas a serem salvas
// //  * @param numeroMedidas matriz com quantidade total de medidas por tipo e fase instaladas
// //  * @return void
// //  * @see
// //  * @note Utilizada somente para validação e conferência das informações
// //  * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
// //  */
// // void salvaDadosRedeEletrica(TF_DBAR *barras, long int numeroBarras, TF_DRAM *ramos, long int numeroRamos, TF_DMED*medidas, long int **numeroMedidas)
// // {
// //     long int i,j,nmed;
// //     FILE *arquivo;

// //     nmed = 0;
// //     for (i = 0; i < 14; i++){
// //         for (j = 0; j < 8; j++){
// //             nmed = nmed + numeroMedidas[i][j];
// //         }
// //     }

// //     // Leitura dos dados de barras
// //     arquivo = fopen("dadosRedeEletrica.dad","w");
// //     if(arquivo == NULL)
// //     {
// //             printf("Erro ao abrir arquivo dadosRedeEletrica.dad !!!\n");
// //             exit(1);
// //     }

// //     fprintf(arquivo,"LISTA DE BARRAS:\n");
// //     for(i=0;i<numeroBarras;i++){
// //         fprintf(arquivo,"Barra: %ld\t%ld\t%s\t%.2lf",barras[i].i,barras[i].ID,charFases(barras[i].fases),barras[i].Vbase);
// //         if(barras[i].nloads>0){
// //             for(j=0;j<barras[i].nloads;j++)
// //                 fprintf(arquivo,"\n\tCarga %ld:\t%s\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.1lf",j,charLigacao(barras[i].loads[j].lig),barras[i].loads[j].Pnom[0],barras[i].loads[j].Pnom[1],barras[i].loads[j].Pnom[2],barras[i].loads[j].Qnom[0],barras[i].loads[j].Qnom[1],barras[i].loads[j].Qnom[2],barras[i].loads[j].ZIP);
// //         }
// //         if(barras[i].nshunts>0){
// //             for(j=0;j<barras[i].nshunts;j++){
// //                 fprintf(arquivo,"\n\tShunt %ld:\t%s\t%.5lf\t%.5lf\t%.5lf\t%ld",j,charLigacao(barras[i].shunts[j].lig),barras[i].shunts[j].Qnom[0],barras[i].shunts[j].Qnom[1],barras[i].shunts[j].Qnom[2],barras[i].shunts[j].controle);
// //                 if(barras[i].shunts[j].controle != 0)
// //                     fprintf(arquivo,"\t%.5lf\t%.5lf\t%.5lf\t%.5lf",barras[i].shunts[j].DV,barras[i].shunts[j].Vset[0],barras[i].shunts[j].Vset[1],barras[i].shunts[j].Vset[2]);
// //             }
// //         }
// //         if(barras[i].ngds>0){
// //             for(j=0;j<barras[i].ngds;j++){
// //                 fprintf(arquivo,"\n\tGD %ld:\t%s\t%.5lf kVA\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%ld",j,charLigacao(barras[i].gds[j].lig),barras[i].gds[j].Snominal,barras[i].gds[j].Pnom[0],barras[i].gds[j].Pnom[1],barras[i].gds[j].Pnom[2],barras[i].gds[j].Qnom[0],barras[i].gds[j].Qnom[1],barras[i].gds[j].Qnom[2],barras[i].gds[j].controle);
// //                 if(barras[i].gds[j].controle != 0)
// //                     fprintf(arquivo,"\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%ld",barras[i].gds[j].Qmin,barras[i].gds[j].Qmax,barras[i].gds[j].Vset[0],barras[i].gds[j].Vset[1],barras[i].gds[j].Vset[2],barras[i].gds[j].controlePV);
// //             }
// //         }
// //         fprintf(arquivo,"\n");
// //     }
// //     fprintf(arquivo,"\nLISTA DE RAMOS:\n");
// //     for(i=0;i<numeroRamos;i++){
// //         switch(ramos[i].tipo){
// //             case ramal:
// //                 fprintf(arquivo,"Linha %ld (%ld)\t%ld (%ld):\t%s\t%d",ramos[i].DE,ramos[i].k,ramos[i].PARA,ramos[i].m,charFases(ramos[i].fases),ramos[i].estado);
// //                 fprintf(arquivo,"\n\t%.4lf  %.4lf\t\t%.4lf  %.4lf\t\t%.4lf  %.4lf",__real__ ramos[i].linha.Zaa,__imag__ ramos[i].linha.Zaa,__real__ ramos[i].linha.Zab,__imag__ ramos[i].linha.Zab,__real__ ramos[i].linha.Zac,__imag__ ramos[i].linha.Zac);
// //                 fprintf(arquivo,"\n\t\t\t\t\t%.4lf  %.4lf\t\t%.4lf  %.4lf",__real__ ramos[i].linha.Zbb,__imag__ ramos[i].linha.Zbb,__real__ ramos[i].linha.Zbc,__imag__ ramos[i].linha.Zbc);
// //                 fprintf(arquivo,"\n\t\t\t\t\t\t\t\t\t%.4lf  %.4lf",__real__ ramos[i].linha.Zcc,__imag__ ramos[i].linha.Zcc);
// //                 break;
// //             case trafo:
// //                 fprintf(arquivo,"Trafo %ld (%ld)\t%ld (%ld):\t%s\t%d",ramos[i].DE,ramos[i].k,ramos[i].PARA,ramos[i].m,charFases(ramos[i].fases),ramos[i].estado);
// //                 fprintf(arquivo,"\n\t%.4lf  %.4lf\t%.3lf\t%.2lf / %.2lf\t%s%s%ld\t%.4lf\t%.4lf",ramos[i].trafo.R,ramos[i].trafo.X,ramos[i].trafo.Snominal,ramos[i].trafo.Vpri,ramos[i].trafo.Vsec,charLigacao(ramos[i].trafo.lig_pri),charLigacao(ramos[i].trafo.lig_sec),ramos[i].trafo.defasamento,ramos[i].trafo.tap_pri,ramos[i].trafo.tap_sec);
// //                 break;
// //             case regulador:
// //                 fprintf(arquivo,"Regulador %ld (%ld)\t%ld (%ld):\t%s\t%d",ramos[i].DE,ramos[i].k,ramos[i].PARA,ramos[i].m,charFases(ramos[i].fases),ramos[i].estado);
// //                 fprintf(arquivo,"\n\t%.4lf  %.4lf\t%.3lf\t%.2lf\t%s\t%ld\t%.4lf\t%.4lf\t%.4lf",ramos[i].regulador.R,ramos[i].regulador.X,ramos[i].regulador.Snominal,ramos[i].regulador.Vnom,charLigacao(ramos[i].regulador.lig),ramos[i].regulador.controle,ramos[i].regulador.tap[0],ramos[i].regulador.tap[1],ramos[i].regulador.tap[2]);
// //                 break;
// //             case chave:
// //                 fprintf(arquivo,"Chave %ld (%ld)\t%ld (%ld):\t%s\t%d",ramos[i].DE,ramos[i].k,ramos[i].PARA,ramos[i].m,charFases(ramos[i].fases),ramos[i].estado);
// //                 break;
// //         }
// //         fprintf(arquivo,"\n");
// //     }

// //     // fprintf(arquivo,"\nLISTA DE MEDIDORES:\n");
// //     // for(i=0;i<nmed;i++){
// //     //     fprintf(arquivo,"Medidor %d %s %d (%d)\t%d (%d):\t%s\t%d",i,charMedidor(medidas[i].tipo),medidas[i].DE,medidas[i].k,medidas[i].PARA,medidas[i].m,charFases(medidas[i].fases),medidas[i].ligado);
// //     //     fprintf(arquivo,"\n\tNumero de variaveis: %d\n",medidas[i].nvar);
// //     //     for(j=0;j<medidas[i].nvar;j++){
// //     //          fprintf(arquivo,"\n\t\t%.1lf",medidas[i].reguaH[j]);
// //     //     }
// //     //     fprintf(arquivo,"\n");
// //     // }

//     fclose(arquivo);
// // }

// Imprime arquivo com informações das medidas analógicas para conferências
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
// void salvaMedidasRedeEletrica(TF_DMED*medidas, long int **numeroMedidas)
// {
//     long int i,j;
//     FILE *arquivo;

//     // Leitura dos dados de barras
//     arquivo = fopen("medidasRedeEletrica.dad","w");
//     if(arquivo == NULL)
//     {
//             printf("Erro ao abrir arquivo dadosRedeEletrica.dad !!!\n");
//             exit(1);
//     }
//     /*
// fprintf(arquivo,"LISTA DE BARRAS:\n");
// for(i=0;i<numeroBarras;i++){
//     fprintf(arquivo,"Barra: %d\t%d\t%s\t%.2lf",barras[i].i,barras[i].ID,charFases(barras[i].fases),barras[i].Vbase);
//     if(barras[i].nloads>0){
//         for(j=0;j<barras[i].nloads;j++)
//             fprintf(arquivo,"\n\tCarga %d:\t%s\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.1lf",j,charLigacao(barras[i].loads[j].lig),barras[i].loads[j].Pnom[0],barras[i].loads[j].Pnom[1],barras[i].loads[j].Pnom[2],barras[i].loads[j].Qnom[0],barras[i].loads[j].Qnom[1],barras[i].loads[j].Qnom[2],barras[i].loads[j].ZIP);
//     }
//     if(barras[i].nshunts>0){
//         for(j=0;j<barras[i].nshunts;j++){
//             fprintf(arquivo,"\n\tShunt %d:\t%s\t%.5lf\t%.5lf\t%.5lf\t%d",j,charLigacao(barras[i].shunts[j].lig),barras[i].shunts[j].Qnom[0],barras[i].shunts[j].Qnom[1],barras[i].shunts[j].Qnom[2],barras[i].shunts[j].controle);
//             if(barras[i].shunts[j].controle != 0)
//                 fprintf(arquivo,"\t%.5lf\t%.5lf\t%.5lf\t%.5lf",barras[i].shunts[j].DV,barras[i].shunts[j].Vset[0],barras[i].shunts[j].Vset[1],barras[i].shunts[j].Vset[2]);
//         }
//     }
//     if(barras[i].ngds>0){
//         for(j=0;j<barras[i].ngds;j++){
//             fprintf(arquivo,"\n\tGD %d:\t%s\t%.5lf kVA\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.1lf",j,charLigacao(barras[i].gds[j].lig),barras[i].gds[j].Snominal,barras[i].gds[j].Pnom[0],barras[i].gds[j].Pnom[1],barras[i].gds[j].Pnom[2],barras[i].gds[j].Qnom[0],barras[i].gds[j].Qnom[1],barras[i].gds[j].Qnom[2],barras[i].gds[j].controle);
//             if(barras[i].gds[j].controle != 0)
//                 fprintf(arquivo,"\t%.5lf\t%.5lf\t%.5lf\t%.5lf\t%.5lf",barras[i].gds[j].Qmin,barras[i].gds[j].Qmax,barras[i].gds[j].Vset[0],barras[i].gds[j].Vset[1],barras[i].gds[j].Vset[2],barras[i].gds[j].controlePV);
//         }
//     }
//     fprintf(arquivo,"\n");
// }   */

// fclose(arquivo);
// }

// Imprime arquivos com informações do sistema completo no padrão do estimador trifásico em pasta separada
/**
 * @brief Função auxiliar para salvar dados da rede elétrica em pasta separada e intercambiar com o estimador de estado
 *
 * Essa função
 *
 * @param medidas estrutura de dados com informações das medidas a serem salvas
 * @param numeroMedidas matriz com quantidade total de medidas por tipo e fase instaladas
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
//  int salvaArquivosAlimentador(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores, TF_DRAM *ramos, int idAlim, double Sbase, BOOL opt_Choice, int id_begin, const char *modo)
// {
//     long int i,j, idRam, auxTipo, para;
//     FILE *arquivo,*arquivo1,*arquivo2,*arquivo3;
//     long int *listaBarrasNew;
//     BOOL visitado[numeroBarras];
//     TF_FILABARRAS *barraAtual;
//     double Vbase, comp, Zbase,prec;

//     for (i=0;i<numeroBarras;i++) visitado[i] = false;

//     // CRia lista sequencial de barras
//     listaBarrasNew = (long int *)malloc( (numeroBarras + 2) * sizeof(long int));
//     for (i=0;i<numeroBarras;i++) listaBarrasNew[i] = -1;
//     long int k = id_begin;
//     barraAtual = &alimentadores[idAlim].rnp[0];
//     while(barraAtual != NULL)
//     {
//         i = barraAtual->idNo;
//         if (opt_Choice){
//             listaBarrasNew[i] = k;
//             k++;
//         }
//         else{
//             listaBarrasNew[i] = grafo[i].barra->ID;
//         }
//         barraAtual = barraAtual->prox;
//     }
//     id_begin = k;

//     //--------------------------------------------------------------------------
//     // Impressão do Arquivo de Barras
//     arquivo = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/DBAR.csv",modo);
//     if(arquivo == NULL){
//             printf("Erro ao abrir arquivo DBAR.dad !!!\n");
//             exit(1);
//     }

//         // Percorre o alimentador
//         barraAtual = &alimentadores[idAlim].rnp[0];
//         Vbase = grafo[barraAtual->idNo].barra->Vbase;
//         while(barraAtual != NULL)
//         {
//             i = barraAtual->idNo;
//             grafo[i].S[0] = grafo[i].V[0] * conj(grafo[i].Cur[0]);
//             grafo[i].S[1] = grafo[i].V[1] * conj(grafo[i].Cur[1]);
//             grafo[i].S[2] = grafo[i].V[2] * conj(grafo[i].Cur[2]);

//             fprintf(arquivo,"%ld;%d;%d;%.2lf;%.2lf;%.2lf;%.2lf;%.2lf;%.2lf;%.2lf;%d",listaBarrasNew[i],grafo[i].barra->ligacao,grafo[i].fases,Vbase*sqrt(3),creal(grafo[i].S[0])*Sbase,creal(grafo[i].S[1])*Sbase,creal(grafo[i].S[2])*Sbase,cimag(grafo[i].S[0])*Sbase,cimag(grafo[i].S[1])*Sbase,cimag(grafo[i].S[2])*Sbase,0);
//             if (grafo[i].tipo == 2){
//                 fprintf(arquivo,";%.5lf;%.5lf;%.5lf;%.5lf;%.5lf;%.5lf",cabs(grafo[i].V[0]),cabs(grafo[i].V[1]),cabs(grafo[i].V[2]),carg(grafo[i].V[0])*180/PI,carg(grafo[i].V[1])*180/PI,carg(grafo[i].V[2])*180/PI);
//             }
//             fprintf(arquivo,"\n");

//             barraAtual = barraAtual->prox;
//         }
//     fclose(arquivo);

//     //--------------------------------------------------------------------------
//     // Impressão do Arquivo de Shunts
//     arquivo = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/DSHNT.csv",modo);
//     if(arquivo == NULL){
//             printf("Erro ao abrir arquivo DSHNT.dad !!!\n");
//             exit(1);
//     }

//         // Percorre o alimentador
//         barraAtual = &alimentadores[idAlim].rnp[0];
//         while(barraAtual != NULL)
//         {
//             i = barraAtual->idNo;
//             for (j=0;j<grafo[i].barra->nshunts;j++){
//                 fprintf(arquivo,"%ld;%d;%d;%.2lf;%.2lf;%.2lf;%.2lf;%ld",listaBarrasNew[i],grafo[i].barra->shunts[j].lig,grafo[i].barra->shunts[j].fases,grafo[i].barra->shunts[j].Vbase,grafo[i].barra->shunts[j].Qnom[0]*Sbase,grafo[i].barra->shunts[j].Qnom[1]*Sbase,grafo[i].barra->shunts[j].Qnom[2]*Sbase,grafo[i].barra->shunts[j].controle);
//                 fprintf(arquivo,"\n");
//             }
//             barraAtual = barraAtual->prox;
//         }
//     fclose(arquivo);

//     //--------------------------------------------------------------------------
//     // Impressão do Arquivo de Linhas, Trafos e Reguladores de Tensão
//     arquivo = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/DLIN.csv",modo);
//     if(arquivo == NULL){
//             printf("Erro ao abrir arquivo DLIN.dad !!!\n");
//             exit(1);
//     }

//     arquivo1 = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/DTRF.csv",modo);
//     if(arquivo1 == NULL){
//             printf("Erro ao abrir arquivo DTRF.dad !!!\n");
//             exit(1);
//     }

//     arquivo2 = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/DREG.csv",modo);
//     if(arquivo2 == NULL){
//             printf("Erro ao abrir arquivo DTRF.dad !!!\n");
//             exit(1);
//     }

//     arquivo3 = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/DSWTC.csv",modo);
//     if(arquivo3 == NULL){
//             printf("Erro ao abrir arquivo DSWTC.dad !!!\n");
//             exit(1);
//     }

//         // Percorre o alimentador
//         barraAtual = &alimentadores[idAlim].rnp[0];
//         while(barraAtual != NULL)
//         {
//             i = barraAtual->idNo;
//             Vbase = grafo[i].Vbase;
//             for (j=0;j<grafo[i].numeroAdjacentes;j++){
//                 idRam = grafo[i].adjacentes[j].idram;
//                 if ((visitado[grafo[i].adjacentes[j].idNo] == false)&& (listaBarrasNew[grafo[i].adjacentes[j].idNo] != -1)){
//                      switch(grafo[i].adjacentes[j].tipo){
//                         case 0: // Linha
//                             comp = ramos[idRam].linha.comprimento;
//                             Zbase = ((pow(Vbase,2))/(Sbase*1000));
//                             fprintf(arquivo,"%ld;%ld;%d;%.3lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.7lf;%.1lf",listaBarrasNew[ramos[idRam].DE],listaBarrasNew[ramos[idRam].PARA],ramos[idRam].fases,comp,creal(ramos[idRam].linha.Zaa)*Zbase/comp,cimag(ramos[idRam].linha.Zaa)*Zbase/comp,creal(ramos[idRam].linha.Zab)*Zbase/comp,cimag(ramos[idRam].linha.Zab)*Zbase/comp,creal(ramos[idRam].linha.Zac)*Zbase/comp,cimag(ramos[idRam].linha.Zac)*Zbase/comp,creal(ramos[idRam].linha.Zbb)*Zbase/comp,cimag(ramos[idRam].linha.Zbb)*Zbase/comp,creal(ramos[idRam].linha.Zbc)*Zbase/comp,cimag(ramos[idRam].linha.Zbc)*Zbase/comp,creal(ramos[idRam].linha.Zcc)*Zbase/comp,cimag(ramos[idRam].linha.Zcc)*Zbase/comp,ramos[idRam].linha.Baa/Zbase/comp,ramos[idRam].linha.Bab/Zbase/comp,ramos[idRam].linha.Bac/Zbase/comp,ramos[idRam].linha.Bbb/Zbase/comp,ramos[idRam].linha.Bbc/Zbase/comp,ramos[idRam].linha.Bcc/Zbase/comp,ramos[idRam].ampacidade);
//                             fprintf(arquivo,"\n");
//                             break;
//                         case 1: // Trafo
//                             Zbase = ((pow(Vbase,2))/(3*Sbase*1000));
//                             fprintf(arquivo1,"%ld;%ld",listaBarrasNew[ramos[idRam].DE],listaBarrasNew[ramos[idRam].PARA]);
//                             fprintf(arquivo1,"\n");
//                             break;
//                         case 2: // Regulador
//                             Zbase = ((pow(Vbase,2))/(3*Sbase*1000));
//                             fprintf(arquivo2,"%ld;%ld;%d;%.2lf;%.2lf;%ld;%.2lf;%.4lf;%.4lf;%d;%.1lf;%.1lf;%.1lf;%.1lf;%.1lf;%.1lf;%.1lf;%.1lf;%.1lf;%.1lf;%.1lf;%.1lf;%ld;%.1lf;%.1lf;%.1lf",listaBarrasNew[ramos[idRam].DE],listaBarrasNew[ramos[idRam].PARA],ramos[idRam].fases,sqrt(3)*ramos[idRam].regulador.Vnom,ramos[idRam].regulador.regulacao,ramos[idRam].regulador.ntaps,ramos[idRam].regulador.Snominal,ramos[idRam].regulador.R*Zbase,ramos[idRam].regulador.X*Zbase,ramos[idRam].regulador.lig,ramos[idRam].regulador.TP,ramos[idRam].regulador.TC,ramos[idRam].regulador.deltaV,ramos[idRam].regulador.R1,ramos[idRam].regulador.X1,ramos[idRam].regulador.R2,ramos[idRam].regulador.X2,ramos[idRam].regulador.R3,ramos[idRam].regulador.X3,ramos[idRam].regulador.V1,ramos[idRam].regulador.V2,ramos[idRam].regulador.V3,ramos[idRam].regulador.controle,ramos[idRam].regulador.tap[0],ramos[idRam].regulador.tap[1],ramos[idRam].regulador.tap[2]);
//                             fprintf(arquivo2,"\n");
//                             break;
//                         case 3: // Chave
//                             fprintf(arquivo3,"%ld;%ld;%d;%d;%d;%.2lf",listaBarrasNew[ramos[idRam].DE],listaBarrasNew[ramos[idRam].PARA],ramos[idRam].fases,ramos[idRam].estado,0,ramos[idRam].ampacidade);
//                             fprintf(arquivo3,"\n");
//                             break;
//                      }
//                 }
//             }
//             visitado[i] = true;
//             barraAtual = barraAtual->prox;
//         }
//     fclose(arquivo);
//     fclose(arquivo1);
//     fclose(arquivo2);
//     fclose(arquivo3);

//     //--------------------------------------------------------------------------
//     // Impressão do Arquivo de DMED_fp.csv
//     arquivo = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/DMED_fp.csv",modo);
//     if(arquivo == NULL){
//             printf("Erro ao abrir arquivo DMED_fp.dad !!!\n");
//             exit(1);
//     }

//         // Percorre o alimentador
//         barraAtual = &alimentadores[idAlim].rnp[0];
//         Vbase = grafo[barraAtual->idNo].barra->Vbase;
//         while(barraAtual != NULL)
//         {
//             i = barraAtual->idNo;
//             if (cabs(grafo[i].S[0]+grafo[i].S[2]+grafo[i].S[3]) < 0.000001) prec = 0.00001; //medida virtual
//             else  prec = 0.30; //pseudo medida

//             if (grafo[i].tipo != 2){
//                 switch (grafo[i].fases){
//                 case 1:
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,1,creal(grafo[i].S[0])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,1,cimag(grafo[i].S[0])*Sbase,prec);

//                     break;
//                 case 2:
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,2,creal(grafo[i].S[1])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,2,cimag(grafo[i].S[1])*Sbase,prec);

//                     break;
//                 case 3:
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,3,creal(grafo[i].S[2])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,3,cimag(grafo[i].S[2])*Sbase,prec);
//                     break;
//                 case 4:
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,1,creal(grafo[i].S[0])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,2,creal(grafo[i].S[1])*Sbase,prec);

//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,1,cimag(grafo[i].S[0])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,2,cimag(grafo[i].S[1])*Sbase,prec);

//                     break;
//                 case 5:
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,1,creal(grafo[i].S[0])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,3,creal(grafo[i].S[2])*Sbase,prec);

//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,1,cimag(grafo[i].S[0])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,3,cimag(grafo[i].S[2])*Sbase,prec);
//                     break;
//                 case 6:
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,2,creal(grafo[i].S[1])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,3,creal(grafo[i].S[2])*Sbase,prec);

//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,2,cimag(grafo[i].S[1])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,3,cimag(grafo[i].S[2])*Sbase,prec);
//                     break;
//                 case 7:
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,1,creal(grafo[i].S[0])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,2,creal(grafo[i].S[1])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,2,listaBarrasNew[i],-1,3,creal(grafo[i].S[2])*Sbase,prec);

//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,1,cimag(grafo[i].S[0])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,2,cimag(grafo[i].S[1])*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,3,listaBarrasNew[i],-1,3,cimag(grafo[i].S[2])*Sbase,prec);
//                     break;
//                 }

//             }
//             if (grafo[i].tipo == 2){
//                 prec = 0.01;
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[i],-1,1,cabs(grafo[i].V[0])*Vbase/1000,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[i],-1,2,cabs(grafo[i].V[1])*Vbase/1000,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[i],-1,3,cabs(grafo[i].V[2])*Vbase/1000,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,5,listaBarrasNew[i],-1,1,carg(grafo[i].V[0])*180/PI,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,5,listaBarrasNew[i],-1,2,carg(grafo[i].V[1])*180/PI,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,5,listaBarrasNew[i],-1,3,carg(grafo[i].V[2])*180/PI,prec);
//             }
// //            fprintf(arquivo,"\n");

//             barraAtual = barraAtual->prox;
//         }

//     fclose(arquivo);

//     // Medidas do Sistema SCADA - Assumida uma tensão na SE e fluxo de potência na SE
//     arquivo = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/DMED_SC.csv",modo);
//     if(arquivo == NULL){
//             printf("Erro ao abrir arquivo DMED_SC.dad !!!\n");
//             exit(1);
//     }

//         // Percorre o alimentador
//         barraAtual = &alimentadores[idAlim].rnp[0];
//         Vbase = grafo[barraAtual->idNo].barra->Vbase;
//         i = barraAtual->idNo;
//         prec = 0.01;
//         fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[i],-1,1,cabs(grafo[i].V[0])*Vbase/1000,prec);
//         fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[i],-1,2,cabs(grafo[i].V[1])*Vbase/1000,prec);
//         fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[i],-1,3,cabs(grafo[i].V[2])*Vbase/1000,prec);
//         fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,5,listaBarrasNew[i],-1,1,carg(grafo[i].V[0])*180/PI,prec);
//         fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,5,listaBarrasNew[i],-1,2,carg(grafo[i].V[1])*180/PI,prec);
//         fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,5,listaBarrasNew[i],-1,3,carg(grafo[i].V[2])*180/PI,prec);

//         // Medida de Fluxo de Potência na Saída do Alimentador
//         for (int adj =0; adj< grafo[i].numeroAdjacentes;adj++ ){
//             if (grafo[i].adjacentes[adj].tipo == 3){
//                 para = grafo[i].adjacentes[adj].idNo;
//                 prec = 0.02;
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,0,listaBarrasNew[i],listaBarrasNew[para],1,creal(grafo[i].V[0]*conj(grafo[i].adjacentes[adj].Cur[0]))*Sbase,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,0,listaBarrasNew[i],listaBarrasNew[para],2,creal(grafo[i].V[1]*conj(grafo[i].adjacentes[adj].Cur[1]))*Sbase,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,0,listaBarrasNew[i],listaBarrasNew[para],3,creal(grafo[i].V[2]*conj(grafo[i].adjacentes[adj].Cur[2]))*Sbase,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,1,listaBarrasNew[i],listaBarrasNew[para],1,cimag(grafo[i].V[0]*conj(grafo[i].adjacentes[adj].Cur[0]))*Sbase,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,1,listaBarrasNew[i],listaBarrasNew[para],2,cimag(grafo[i].V[1]*conj(grafo[i].adjacentes[adj].Cur[1]))*Sbase,prec);
//                 fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,1,listaBarrasNew[i],listaBarrasNew[para],3,cimag(grafo[i].V[2]*conj(grafo[i].adjacentes[adj].Cur[2]))*Sbase,prec);
//             }
//         }

//         // Percorre o alimentador para medidas em reguladores
//         barraAtual = &alimentadores[idAlim].rnp[0];
//         while(barraAtual != NULL)
//         {
//             i = barraAtual->idNo;
//             Vbase = grafo[i].Vbase;
//             for (j=0;j<grafo[i].numeroAdjacentes;j++){
//                 para = grafo[i].adjacentes[j].idNo;
//                 idRam = grafo[i].adjacentes[j].idram;
//                 if ((grafo[i].adjacentes[j].tipo == 2) && (grafo[i].profundidade < grafo[para].profundidade)){
//                     //Tensão montante
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[i],-1,1,cabs(grafo[i].V[0])*Vbase/1000,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[i],-1,2,cabs(grafo[i].V[1])*Vbase/1000,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[i],-1,3,cabs(grafo[i].V[2])*Vbase/1000,prec);

//                     //Tensão Jusante
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[para],-1,1,cabs(grafo[para].V[0])*Vbase/1000,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[para],-1,2,cabs(grafo[para].V[1])*Vbase/1000,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,4,listaBarrasNew[para],-1,3,cabs(grafo[para].V[2])*Vbase/1000,prec);

//                     //Fluxo de potência montante
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,0,listaBarrasNew[i],listaBarrasNew[para],1,creal(grafo[i].V[0]*conj(grafo[i].adjacentes[j].Cur[0]))*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,0,listaBarrasNew[i],listaBarrasNew[para],2,creal(grafo[i].V[1]*conj(grafo[i].adjacentes[j].Cur[1]))*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,0,listaBarrasNew[i],listaBarrasNew[para],3,creal(grafo[i].V[2]*conj(grafo[i].adjacentes[j].Cur[2]))*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,1,listaBarrasNew[i],listaBarrasNew[para],1,cimag(grafo[i].V[0]*conj(grafo[i].adjacentes[j].Cur[0]))*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,1,listaBarrasNew[i],listaBarrasNew[para],2,cimag(grafo[i].V[1]*conj(grafo[i].adjacentes[j].Cur[1]))*Sbase,prec);
//                     fprintf(arquivo,"%d;%d;%ld;%d;%d;%.5lf;%.5lf\n",1,1,listaBarrasNew[i],listaBarrasNew[para],3,cimag(grafo[i].V[2]*conj(grafo[i].adjacentes[j].Cur[2]))*Sbase,prec);
//                 }
//             }
//             barraAtual = barraAtual->prox;
//         }

//     fclose(arquivo);

//     //--------------------------------------------------------------------------
//     // Impressão do Arquivo de Vinicial
//     arquivo = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/Vinicial.csv",modo);
//     if(arquivo == NULL){
//             printf("Erro ao abrir arquivo Vinicial.dad !!!\n");
//             exit(1);
//     }

//         // Percorre o alimentador
//         barraAtual = &alimentadores[idAlim].rnp[0];
//         Vbase = grafo[barraAtual->idNo].barra->Vbase;
//         while(barraAtual != NULL)
//         {
//             i = barraAtual->idNo;
//             switch (grafo[i].fases){
//                 case 1:
//                     fprintf(arquivo,"%ld;%.8lf;NaN;NaN;%.8lf;NaN;NaN;",listaBarrasNew[i],cabs(grafo[i].V[0]),carg(grafo[i].V[0]) *180 / PI);
//                     break;
//                 case 2:
//                     fprintf(arquivo,"%ld;NaN;%.8lf;NaN;NaN;%.8lf;NaN;",listaBarrasNew[i],cabs(grafo[i].V[1]),carg(grafo[i].V[1]) *180 / PI);
//                     break;
//                 case 3:
//                     fprintf(arquivo,"%ld;NaN;NaN;%.8lf;NaN;NaN;%.8lf",listaBarrasNew[i],cabs(grafo[i].V[1]),carg(grafo[i].V[1]) *180 / PI);
//                     break;
//                 case 4:
//                     fprintf(arquivo,"%ld;%.8lf;%.8lf;NaN;%.8lf;%.8lf;NaN;",listaBarrasNew[i],cabs(grafo[i].V[0]),cabs(grafo[i].V[1]),carg(grafo[i].V[0]) *180 / PI, carg(grafo[i].V[1]) *180 / PI);
//                     break;
//                 case 5:
//                     fprintf(arquivo,"%ld;%.8lf;NaN;%.8lf;%.8lf;NaN;%.8lf;",listaBarrasNew[i],cabs(grafo[i].V[0]),cabs(grafo[i].V[2]),carg(grafo[i].V[0]) *180 / PI, carg(grafo[i].V[2]) *180 / PI);
//                     break;
//                 case 6:
//                     fprintf(arquivo,"%ld;NaN;%.8lf;%.8lf;NaN;%.8lf;%.8lf;",listaBarrasNew[i],cabs(grafo[i].V[1]),cabs(grafo[i].V[2]),carg(grafo[i].V[1]) *180 / PI, carg(grafo[i].V[2]) *180 / PI);
//                     break;
//                 case 7:
//                     fprintf(arquivo,"%ld;%.8lf;%.8lf;%.8lf;%.8lf;%.8lf;%.8lf;",listaBarrasNew[i],cabs(grafo[i].V[0]),cabs(grafo[i].V[1]),cabs(grafo[i].V[2]),carg(grafo[i].V[0]) *180 / PI, carg(grafo[i].V[1]) *180 / PI, carg(grafo[i].V[2]) *180 / PI);
//                     break;
//             }
//             fprintf(arquivo,"\n");

//             barraAtual = barraAtual->prox;
//         }
//     fclose(arquivo);

//     //--------------------------------------------------------------------------
//     // Impressão do Arquivo de DARE.csv
//     arquivo = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/DARE.csv",modo);
//     if(arquivo == NULL){
//             printf("Erro ao abrir arquivo DARE.dad !!!\n");
//             exit(1);
//     }

//         // Percorre o alimentador
//         barraAtual = &alimentadores[idAlim].rnp[0];
//         Vbase = grafo[barraAtual->idNo].barra->Vbase;
//         while(barraAtual != NULL)
//         {
//             i = barraAtual->idNo;
//             fprintf(arquivo,"%ld;%d",listaBarrasNew[i],idAlim);
//             if (grafo[i].tipo == 2){ // Barra de Referência do Alimentador
//                 fprintf(arquivo,";%d;%d",1,1);
//             }
//             else{
//                 auxTipo = 0;
//                 for (j=0;j<grafo[i].numeroAdjacentes;j++){   // Testa para ver se possui fronteira (com outro alimentador)
//                     idRam = grafo[i].adjacentes[j].idram;
//                     if (listaBarrasNew[grafo[i].adjacentes[j].idNo] == -1){
//                         auxTipo = 1;
//                     }
//                 }
//                 fprintf(arquivo,";%d;%d",auxTipo,0);
//             }
//             fprintf(arquivo,"\n");

//             barraAtual = barraAtual->prox;
//         }

//     fclose(arquivo);

//     //--------------------------------------------------------------------------
//     // Impressão do Arquivo de Bar_cod.csv
//     arquivo = fopen("D:/Dropbox/PeD COPEL 2018/Codigos/Fluxo_Trifasico/Exportado/Bar_cod.txt",modo);
//     if(arquivo == NULL){
//             printf("Erro ao abrir arquivo Bar_cod.txt !!!\n");
//             exit(1);
//     }

//     // Percorre o alimentador
//         barraAtual = &alimentadores[idAlim].rnp[0];
//         while(barraAtual != NULL)
//         {
//             i = barraAtual->idNo;
//             fprintf(arquivo,"%ld\t%ld",grafo[i].barra->ID,listaBarrasNew[i]);
//             fprintf(arquivo,"\n");
//             barraAtual = barraAtual->prox;
//         }

//     fclose(arquivo);

//     free(listaBarrasNew);
//     return (id_begin);
// }

// Libera memória alocada na estrutrura DBAR
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
void free_BARRA(TF_DBAR *barra, long int numeroBarras)
{
    int i, j;

    for (i = 0; i < numeroBarras; i++)
    {
        free(barra[i].shunts);
        free(barra[i].gds);
    }
    free(barra);
}

// Libera memória alocada na estrutrura TF_DRAM
/**
 * @brief Função auxiliar para liberar memória da estrutura TF_DRAM
 *
 * Essa função realiza a liberação de memória de forma apropriada para a estrutura TF_DRAM.
 * A função retorna @c void .
 *
 * @param ramos ponteiro para TF_DRAM a ser liberado o espaço de memória
 * @param numeroRamos quantidade total de ramos
 * @return void
 * @see
 * @note
 * @warning
 */
void free_DRAM(TF_DRAM *ramos, long int numeroRamos)
{
    int i, j;

    for (i = 0; i < numeroRamos; i++)
    {
        if (ramos[i].tipo != 3)
        {
            for (j = 0; j < 3; j++)
            {
                free(ramos[i].Ypp[j]);
                free(ramos[i].Yps[j]);
                free(ramos[i].Ysp[j]);
                free(ramos[i].Yss[j]);
            }
            free(ramos[i].Ypp);
            free(ramos[i].Yps);
            free(ramos[i].Ysp);
            free(ramos[i].Yss);
        }

        if (ramos[i].tipo == 0)
        {
            for (j = 0; j < 3; j++)
            {
                free(ramos[i].Z[j]);
                free(ramos[i].B[j]);
            }
            free(ramos[i].Z);
            free(ramos[i].B);
        }
    }
    free(ramos);
}

// Libera memória alocada na estrutrura TF_GRAFO
/**
 * @brief Função auxiliar para liberar memória da estrutura TF_GRAFO
 *
 * Essa função realiza a liberação de memória de forma apropriada para a estrutura TF_GRAFO.
 * A função retorna @c void .
 *
 * @param grafo ponteiro para TF_GRAFO a ser liberado o espaço de memória
 * @param numeroBarras quantidade total de barras
 * @return void
 * @see
 * @note
 * @warning
 */
void free_GRAFO(TF_GRAFO *grafo, long int numeroBarras)
{
    int i, j;

    for (i = 0; i < numeroBarras; i++)
    {

        free(grafo[i].medidores);
        for (j = 0; j < grafo[i].numeroAdjacentes; j++)
            free(grafo[i].adjacentes[j].medidores);
    }
    free(grafo);
}

// Libera memória alocada na estrutrura ALIMENTADORES e RNPS
/**
 * @brief Função auxiliar para liberar memória da estrutura TF_ALIMENTADOR e RNPs
 *
 * Essa função realiza a liberação de memória de forma apropriada para a estrutura TF_ALIMENTADOR e RNPs.
 * A função retorna @c void .
 *
 * @param alimentador ponteiro para TF_ALIMENTADOR a ser liberado o espaço de memória
 * @param numeroAlimentadores quantidade total de alimentadores
 * @return void
 * @see
 * @note
 * @warning
 */
void free_ALIMENTADOR(TF_ALIMENTADOR *alimentadores, long int numeroAlimentadores)
{
    int i, j, nmed = 0;

    for (i = 0; i < numeroAlimentadores; i++)
    {
        TF_FILABARRAS *barraAtual = &alimentadores[i].rnp[0];
        TF_FILABARRAS *barraProx;
        barraAtual = barraAtual->prox;
        while (barraAtual != NULL)
        {
            barraProx = barraAtual->prox;
            free(barraAtual);
            barraAtual = barraProx;
        }
    }
    free(alimentadores);
}

// -------------------------- Funçoes Auxiliares de Testes de Validação - Fora da biblioteca base -

void leituraDMED_fp_CARGAS(char *folder, char *file, TF_GRAFO *grafo, long int numeroBarras, double Sbase)
{
    char blocoLeitura[2000];   /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados;               /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int i, aux, k, idNo, fase; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    FILE *arquivo;
    char text_aux[500];
    double zmed;

    // Leitura dos dados de medidores
    strcpy(text_aux, folder);
    arquivo = fopen(strcat(text_aux, file), "r");

    // arquivo = fopen(folder,"r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo %s !!!\n", strcat(text_aux, file));
        exit(1);
    }

    for (i = 0; i < numeroBarras; i++)
    {
        grafo[i].S[0] = 0;
        grafo[i].S[1] = 0;
        grafo[i].S[2] = 0;
        grafo[i].Cur[0] = 0;
        grafo[i].Cur[1] = 0;
        grafo[i].Cur[2] = 0;
    }

    // Le o arquivo de curva de cargas até o fim
    while ((fgets(blocoLeitura, 2000, arquivo)) != NULL)
    {
        dados = blocoLeitura;

        // Verifica se a barra já foi criada - Não efetua busca nos IDs de Barras - Assume que já está pré-mapeado corretamente
        aux = (getfield_int(dados, 2));
        if ((aux == 2) || (aux == 3))
        {
            zmed = getfield_double(dados, 6) / Sbase;

            if (cabs(zmed) != 0)
            {
                idNo = getfield_int(dados, 3);
                fase = getfield_int(dados, 5) - 1;

                if (aux == 3)
                    grafo[idNo].S[fase] += I * zmed;
                else
                    grafo[idNo].S[fase] += zmed;

                if (aux == 3)
                    grafo[idNo].Cur[fase] += conj(I * zmed / grafo[idNo].V[fase]);
                else
                    grafo[idNo].Cur[fase] += conj(zmed / grafo[idNo].V[fase]);

                if (grafo[idNo].Cur[fase] != grafo[idNo].Cur[fase])
                {
                    grafo[idNo].Cur[fase] = 0;
                    grafo[idNo].S[fase] = 0;
                }
            }
        }
    }
    //    zmed = 0;
    //    for (i=0;i<numeroBarras;i++){
    //        zmed += creal(grafo[i].Cur[0]);
    //    }
    //    printf("\nCarga : %lf\n",zmed);
    //    printf("\n");
}
