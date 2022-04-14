/**
 * @file funcoesLeituraDados.c
 * @author Telma Woerle de Lima Soares
 * @date 12 Sep 2013
 * @brief Contém a implementação das funções utilizadas para a leitura dos dados no novo formato especificado.
 * Este arquivo implementa as funções definidas na biblioteca funcoesLeituraDados.h
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesLeituraDados.h"


long int numeroAlimentadores; /**< Variável global para armazenar o número de alimentadores do SDR. */
long int numeroReguladores; /**< Variável global para armazenar o número de reguladores de tensão do SDR. */

/**
 * @brief Função principal para a leitura do arquivo dados_barras_linhas_trafos.dad.
 *
 * Essa função realiza a leitura do arquivo dados_barras_linhas_trafos.dad por meio das 
 * funções leituraBarras, leiturasLinhas e leituraTrafos. Essa função principal é responsável por 
 * localizar os marcadores DBAR, DLIM e DTRAFO. A função assume que o nome do arquivo é padrão da forma 
 * dados_barras_linhas_trafos.dad. Além disso o arquivo possui como separação dos dados os marcadores especificados.
 * Ao localizar o marcador o fluxo de controle é desviado para a função correspondente para a leitura dos dados referentes ao marcador.
 * Recebe como parâmetros de entrada e saída um ponteiro para ponteiro do tipo GRAFO @p **grafoSDRParam e
 * um ponteiro para ponteiro do tipo DADOSTRAFO @p  **dadosTrafoSDRParam.
 * A função retorna @c BOOL indicando se a leitura foi bem sucedida.
 * Para utilizar a função:
 * @code
 * GRAFO *grafoExemplo;
 * DADOSTRAFO *trafoExemplo;
 * BOOL leitura = leituraBarrasLinhasTrafos( &grafoExemplo, &trafoExemplo);
 * if (leitura)
 *      printf("leitura concluida\n");
 * @endcode
 * 
 * @param grafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo do GRAFO, onde é retornado o SDR na forma de um grafo.
 * @param dadosTrafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo DADOSTRAFO, onde é retornado os dados dos trafos do SDR.
 * @return Retorna o tipo booleano definido indicando que a leitura foi bem sucedida.
 * @see leituraBarras
 * @see leituraLinhas
 * @see leituraTrafos
 * @note
 * @warning .
 */
BOOL leituraBarrasLinhasTrafos(GRAFO **grafoSDRParam, DADOSTRAFO **dadosTrafoSDRParam, long int *numeroBarras, long int *numeroTrafos, long int *numeroChaves)
{
    FILE *arquivo; /* Variável ponteiro para a manipulação do arquivo. */
    char blocoLeitura[100]; /* Variável para realizar a leitura de caracteres do arquivo. */
    char marcador[10]; /* Variável para ler o marcador que diferencia cada trecho do arquivo. */
    BOOL continuaLeitura =  true;
    // Abertura do arquivo dados_barras_linhas_trafos.dad para leitura e associação ao ponteiro de arquivo
    arquivo = fopen("dados_barras_linhas_trafos.dad","r");
    
    //Verifica se o arquivo foi aberto com sucesso, caso ocorra um erro retorna aviso ao usuário e encerra o programa.
    if(arquivo == NULL)
    {
            printf("Erro ao abrir arquivo dados_barras_linhas_trafos.dad !!!\n");
            exit(1);
    }
    
    //Realiza a leitura até que seja encontrado o fim do arquivo.
    while(continuaLeitura && !feof(arquivo))
    {
        //Leitura de um bloco de caracteres do arquivo.
        fgets(blocoLeitura,100,arquivo);
        //Extrai da string lida a primeira palavra na variável marcador.
        sscanf(blocoLeitura,"%s",marcador);
        //Converte a string da variável marcador para caracteres em UPPERCASE.
        //strupr(marcador);
        //Realiza a comparação da string armazenada na variável marcador com os marcadores definidos. Caso a string não correponda a nenhum marcador encerra o programa.
        //printf("Marcador %s \n", marcador);
        if (!strcmp(marcador, "DBAR"))
        {
            leituraBarras(arquivo,grafoSDRParam, numeroBarras); //Direciona o fluxo para a função leituraBarras que realiza a leitura no formato do trecho de barras
        }
        else
        if (!strcmp(marcador, "DLIN"))
        {
            leituraLinhas(arquivo,*grafoSDRParam, numeroChaves); //Direciona o fluxo para a função leituraLinhas que realiza a leitura no formato do trecho de linhas
        }
        else
        if (!strcmp(marcador, "DTRAFO"))
        {
            leituraTrafos(arquivo,dadosTrafoSDRParam, numeroTrafos); //Direciona o fluxo para a função leituraTrafos que realiza a leitura no formato do trecho de trafos
            continuaLeitura = false;
        }
        else
        {
            printf("Erro ao ler arquivo dados_barras_linhas_trafos.dad !!! \n Formato Invalido!!!\n");
            exit(1);
        }    
        
    }
    //Fecha o arquivo
    fclose(arquivo);
    return(true);
}

/**
 * @brief Função auxiliar para a leitura do arquivo dados_barras_linhas_trafos.dad, referente ao trecho relativo as informações das barras. 
 *
 * Essa função realiza a leitura do trecho do arquivo dados_barras_linhas_trafos.dad relativo ao marcador DBAR 
 * que contém os dados relativos as barras do sistema. Também é responsável pela alocação de memória necessária para armazenar o SDR
 * na forma de um grafo. Recebe como parâmetro de entrada e saída um ponteiro para ponteiro do tipo GRAFO., @p **grafoSDRParam .
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param grafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo do GRAFO, onde é retornado o SDR na forma de um grafo.
 * @return void.
 * @see leituraBarrasLinhasTrafos
 * @note Para realizar a leitura dos dados de P e Q, que são constituídos de 24 pares, utilizou-se a string de controle %n. 
 * Essa string de controle retorna a quantidade de caracteres lidos. Com isso é possível no sscanf fazer a alteração do ponteiro
 *  e utilizar uma estrutura de repetição para realizar a leitura desses dados. 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraBarras(FILE *arquivo, GRAFO **grafoSDRParam, long int *numeroBarras)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados; /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    
    long int contador; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    long int numeroNos; /* Variável para ler o número de nós que o arquivo contém. */
    PQ valorPQ; /* Variável array do tipo PQ auxiliar para extrair os valores de P e Q da string lida. */
    long int identificadorNo; /* Variável auxiliar para leitura do identificador do nó na string lida do arquivo. */
    int tipoDoNo; /* Variável auxiliar para leitura do tipo do nó na string lida do arquivo. */
    PRIORIZACAO dadosPriorizacao; /* Variável auxiliar para leitura dos dados de priorizações da barra na string lida do arquivo. */
    COMPENSACAO dadosCompensacao; /* Variável auxiliar para leitura dos dados de compensações na string lida do arquivo. */   
    
    //leitura do bloco de caracteres do arquivo
    fgets(blocoLeitura, 2000, arquivo);
    //identificação da variável numero de nós da string lida do arquivo
    sscanf(blocoLeitura, "%ld",&numeroNos);
    //atribui o valor lido da string a variável global que armazena o número de barras do SDR
    numeroBarras[0] = numeroNos;
    
    //realiza a alocação da estrutura em grafo para armazenar o SDR e verifica se o processo foi bem sucedido
    //caso não seja possível realizar a alocação o programa é encerrado
    if (((*grafoSDRParam) = (GRAFO *)malloc( (numeroNos+1) * sizeof(GRAFO)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para o grafo do SDR !!!!");
        exit(1); 
    }
    //realiza a leitura e inserção na estrutura do grafo dos dados relativos as barras do SDR
    for (contador = 1; contador<=numeroNos; contador++)
    {
        fgets(blocoLeitura, 2000, arquivo);
        //atribui a string lida do arquivo a variável auxiliar dados, do tipo ponteiro para permitir a alteração do ponteiro da string
        dados = blocoLeitura;
        //faz a leitura na string dos dados identificador do nó(barra) e do tipo do nó, o terceiro valor retorna a quantidade de caracteres lidos
        sscanf(dados, "%ld %d %lf %lf %lf %lf %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&identificadorNo,&tipoDoNo,&valorPQ.p,&valorPQ.q,&dadosPriorizacao.eusdGrupoA,&dadosPriorizacao.eusdGrupoB,&dadosPriorizacao.qtdConsumidores,&dadosPriorizacao.qtdConsumidoresEspeciais,&dadosCompensacao.metaDicMensal,&dadosCompensacao.metaDicTrimestral,&dadosCompensacao.metaDicAnual,&dadosCompensacao.metaDmicMensal,&dadosCompensacao.metaFicMensal, &dadosCompensacao.metaFicTrimestral, &dadosCompensacao.metaFicAnual, &dadosCompensacao.resulDicAcumAno, &dadosCompensacao.resulDicAcumTrimestre, &dadosCompensacao.resulDicAcumMes, &dadosCompensacao.resulFicAcumAno, &dadosCompensacao.resulFicAcumTrimestre, &dadosCompensacao.resulFicAcumMes, &dadosCompensacao.resulDmicMes);           

        //printf("barra %ld consumidor %d consumidorEspecial %d \t", identificadorNo, dadosPriorizacao.qtdConsumidores, dadosPriorizacao.qtdConsumidoresEspeciais);
        //atribui os dados lidos nas variáveis auxiliares para a estrutura que armazena o grafo que representa do SDR
        //o formato (*grafoSDRParam) é utilizado pois o parâmetro trata-se de um ponteiro para ponteiro
        (*grafoSDRParam)[contador].idNo = identificadorNo;
        (*grafoSDRParam)[contador].tipoNo = tipoDoNo;
        (*grafoSDRParam)[contador].numeroAdjacentes = 0;
        (*grafoSDRParam)[contador].compensacoes.metaDicAnual = dadosCompensacao.metaDicAnual;
        (*grafoSDRParam)[contador].compensacoes.metaDicMensal = dadosCompensacao.metaDicMensal;
        (*grafoSDRParam)[contador].compensacoes.metaDicTrimestral = dadosCompensacao.metaDicTrimestral;
        (*grafoSDRParam)[contador].compensacoes.metaDmicMensal = dadosCompensacao.metaDmicMensal;
        (*grafoSDRParam)[contador].compensacoes.metaFicAnual = dadosCompensacao.metaFicAnual;
        (*grafoSDRParam)[contador].compensacoes.metaFicMensal = dadosCompensacao.metaFicMensal;
        (*grafoSDRParam)[contador].compensacoes.metaFicTrimestral = dadosCompensacao.metaFicTrimestral;
        (*grafoSDRParam)[contador].compensacoes.resulDicAcumAno = dadosCompensacao.resulDicAcumAno;
        (*grafoSDRParam)[contador].compensacoes.resulDicAcumMes = dadosCompensacao.resulDicAcumMes;
        (*grafoSDRParam)[contador].compensacoes.resulDicAcumTrimestre = dadosCompensacao.resulDicAcumTrimestre;
        (*grafoSDRParam)[contador].compensacoes.resulDmicMes = dadosCompensacao.resulDmicMes;
        (*grafoSDRParam)[contador].compensacoes.resulFicAcumAno = dadosCompensacao.resulFicAcumAno;
        (*grafoSDRParam)[contador].compensacoes.resulFicAcumMes = dadosCompensacao.resulFicAcumMes;
        (*grafoSDRParam)[contador].compensacoes.resulFicAcumTrimestre = dadosCompensacao.resulFicAcumTrimestre;
        (*grafoSDRParam)[contador].priorizacoes.eusdGrupoA = dadosPriorizacao.eusdGrupoA;
        (*grafoSDRParam)[contador].priorizacoes.eusdGrupoB = dadosPriorizacao.eusdGrupoB;
        (*grafoSDRParam)[contador].priorizacoes.qtdConsumidores = dadosPriorizacao.qtdConsumidores;
        (*grafoSDRParam)[contador].priorizacoes.qtdConsumidoresEspeciais = dadosPriorizacao.qtdConsumidoresEspeciais;
        (*grafoSDRParam)[contador].valorPQ.p = valorPQ.p;
        (*grafoSDRParam)[contador].valorPQ.q = valorPQ.q;
        (*grafoSDRParam)[contador].idSetor = 0;
        (*grafoSDRParam)[contador].medicao = semMedidor;
        //alocação do espaço para armazenar a lista de adjacentes que será lida na função leituralinhas
        (*grafoSDRParam)[contador].adjacentes = Malloc(NOADJACENTE,11);
    }
 }

/**
 * @brief Função auxiliar para a leitura do arquivo dados_barras_linhas_trafos.dad, referente ao trecho relativo as informações das linhas. 
 *
 * Essa função realiza a leitura do trecho do arquivo dados_barras_linhas_trafos.dad relativo ao marcador DLIN 
 * que contém os dados relativos as linhas do sistema. Nessa função é construída a lista de adjacentes de cada nó da representação do SDR na 
 * forma de grafos. Recebe como parâmetro de entrada e saída um ponteiro do tipo GRAFO., @p *grafoSDRParam .
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param grafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo do GRAFO, onde é retornado o SDR na forma de um grafo.
 * @return void
 * @see leituraBarrasLinhasTrafos
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraLinhas(FILE *arquivo, GRAFO *grafoSDRParam, long int *numeroChaves)
{
    char blocoLeitura[1000]; //Variável para leitura do bloco de caracteres do arquivo
    long int barraDe, barraPara; //Variáveis para ler o par barra de origem e barra de destino
    char identificadorAresta[20]; //Variável para ler o indentificador da aresta
    int dadosCondicao; //Variável para ler a condição operacional da aresta (linha)
    int estadoChave; //Variável para ler o estado das linhas (chave aberta, chave fechada ou outros)
    int dadosTipoAresta; //Variável para ler os dados do tipo da aresta
    int numeroArestas; //Variável para ler o número de arestas do sistema
    int dadosSubTipoAresta; //Variável para ler o subtipo das arestas
    int contador; //Variável contador 
    double resistencia; //Variável para leitura da resistência da linha
    double reatancia; //Variável para leitura da reatância da linha
    double ampacidade; //Variável para leitura da ampacidade da linha
    numeroChaves[0] =0;
    //Leitura do bloco de caracteres do arquivo
    fgets(blocoLeitura, 1000, arquivo);
    //Leitura do número de arestas da string blocoLeitura
    sscanf(blocoLeitura, "%d",&numeroArestas);
    //Leitura de cada linha do arquivo relativo a cada uma das arestas do SDR
    for(contador = 0; contador<numeroArestas; contador++)
    {
         //Leitura do bloco de caracteres do arquivo
        fgets(blocoLeitura, 1000, arquivo);
         //Leitura de cad um dos dados das arestas do SDR a partir da string blocoLeitura
        sscanf(blocoLeitura, "%ld %ld %s %d %d %d %d %lf %lf %lf",&barraDe, &barraPara, identificadorAresta, &dadosTipoAresta, &estadoChave, &dadosCondicao, &dadosSubTipoAresta, &resistencia, &reatancia, &ampacidade);
        //Insere os dados das linhas no formato da lista de adjacências do grafo
        grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].idNo = barraPara;
        grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].condicao = dadosCondicao;
        grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].estadoChave = estadoChave;
        grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].tipoAresta =  dadosTipoAresta;
        grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].subTipoAresta = dadosSubTipoAresta;
        grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].resistencia = resistencia;
        grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].reatancia = reatancia;
        grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].ampacidade = ampacidade;
        grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].medicao = semMedidor;
        // como o parâmetro identificador da Aresta é uma string para inserir na lista de adjacencias utiliza o comando de cópia de strings
        strcpy(grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].idAresta, identificadorAresta);
        //faz a contagem do número de chaves do SDR
        if (grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].tipoAresta == chaveManual || grafoSDRParam[barraDe].adjacentes[grafoSDRParam[barraDe].numeroAdjacentes].tipoAresta == chaveAutomatica)
            numeroChaves[0]++;
        //Atualiza o número de adjacentes da barra
        grafoSDRParam[barraDe].numeroAdjacentes++;
        //Processo similar ao anterior só inverte as barras De e Para
        grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].idNo = barraDe;
        grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].condicao = dadosCondicao;
        grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].estadoChave = estadoChave;
        grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].tipoAresta =  dadosTipoAresta;
        grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].subTipoAresta = dadosSubTipoAresta;
        grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].resistencia = resistencia;
        grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].reatancia = reatancia;
        grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].ampacidade = ampacidade;
        grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].medicao = semMedidor;
        strcpy(grafoSDRParam[barraPara].adjacentes[grafoSDRParam[barraPara].numeroAdjacentes].idAresta, identificadorAresta);
        grafoSDRParam[barraPara].numeroAdjacentes++;
    }
}

/**
 * @brief Função auxiliar para a leitura do arquivo dados_barras_linhas_trafos.dad, referente ao trecho relativo as informações dos trafos. 
 *
 * Essa função realiza a leitura do trecho do arquivo dados_barras_linhas_trafos.dad relativo ao marcador DTRAFO 
 * que contém os dados relativos aos trafos do sistema. Nessa função é alocada a estrutura que armazena os dados dos trafos.
 * Para isso ao função recebe como parâmetro de entrada e saída e um ponteiro para o ponteiro do tipo DADOSTRAFO, @p **dadosTrafoSDRParam .
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para o arquivo onde está sendo realizada a leitura.
 * @param dadosTrafoSDRParam é um ponteiro para o ponteiro da estrutura do tipo do DADOSTRAFO, onde os dados do trafo são retornados.
 * @return void
 * @see leituraBarrasLinhasTrafos
 * @note
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void leituraTrafos(FILE *arquivo, DADOSTRAFO **dadosTrafoSDRParam, long int *numeroTrafos)
{
    char blocoLeitura[300];  //Variável para leitura do bloco de caracteres do arquivo
    long int tamTrafos; //Variável para ler o número de trafos do SDR
    long int contador; //Variável contador para controlar a leitura dos dados de trafos
    int identificadorTrafo; //Variável para o identificador do trafo
    int identificadorSubEstacao; //Variável para o identificador da subestação
    double capacidade; //Variável para leitura da capacidade do trafo
    double tensaoReal; //Variável para leitura da tensão real do trafo
    
     //Leitura do bloco de caracteres do arquivo
    fgets(blocoLeitura, 300, arquivo);
     //Leitura do número de trafos do SDR
    sscanf(blocoLeitura, "%ld",&tamTrafos);
    //Atribui o número de trafos lido do arquivo para a variável global numeroTrafos
    numeroTrafos[0] = tamTrafos;
    //Aloca a memória para armazenar os dados dos trafos do SDR e verifica se o processo foi concluído com sucesso. 
    //Caso não seja possível alocar a memória encerra o programa.
    if (((*dadosTrafoSDRParam) = (DADOSTRAFO *)malloc( (tamTrafos+1) * sizeof(DADOSTRAFO)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para os dados de trafos do SDR !!!!");
        exit(1); 
    }
    
    //Leitura dos dados de cada trafo
    for(contador = 1; contador <= numeroTrafos[0]; contador++)
    {
         //Leitura do bloco de caracteres do arquivo
        fgets(blocoLeitura, 300, arquivo);
         //Leitura de cada informação dos dados dos trafos na string blocoLeitura
        sscanf(blocoLeitura,"%d %d %lf %lf", &identificadorTrafo, &identificadorSubEstacao, &capacidade, &tensaoReal);
        //Inserção na estrutura de dados dos trafos das informações lidas.
        (*dadosTrafoSDRParam)[contador].idTrafo = identificadorTrafo;
        (*dadosTrafoSDRParam)[contador].idSubEstacao = identificadorSubEstacao;
        (*dadosTrafoSDRParam)[contador].capacidade = capacidade;
        (*dadosTrafoSDRParam)[contador].tensaoReal = tensaoReal;
        
    }
}
/**
 * @brief Função para imprimir os dados lidos sobre o SDR representado na forma de um grafo.
 *
 * Essa função recebe como parâmetro uma variavél do tipo GRAFO e realiza a impressão
 * na tela dos principais dados contidos na estrutura informada. É utilizada para validar o processo executado pela função de leitura.
 
 * Exemplo:
 * @code
 * GRAFO *grafoExemplo;
 * DADOSTRAFO *trafoExemplo;
 * BOOL leitura = leituraBarrasLinhasTrafos( &grafoExemplo, &trafoExemplo);
 * imprimeGrafo(grafoExemplo);
 * @endcode
 * 
 * @param grafoSDRParam é um ponteiro da estrutura do tipo do GRAFO.

 */

void imprimeGrafo(GRAFO *grafoSDRParam, long int numeroBarras)
{
    int contador, contadorAdj; //Variáveis contadores para imprimir os dados do grafo do SDR
    
    //Loop para percorrer cada barra do SDR
    for(contador =1; contador <= numeroBarras; contador++)
    {
        //Imprime os dados das barras
        printf ("BARRA ID: %ld \t", grafoSDRParam[contador].idNo);
        printf ("\t TIPO DA BARRA: %d \t",grafoSDRParam[contador].tipoNo);
        printf ("\t SETOR DA BARRA: %ld \n",grafoSDRParam[contador].idSetor);
        printf ("\t P Q \n");
        printf("\t");
        printf("%.2lf  %.2lf", grafoSDRParam[contador].valorPQ.p, grafoSDRParam[contador].valorPQ.q);

        printf("\n");
        printf("\t Dados para Priorizacoes: \n");
        printf("\t \t EUSD Grupo A:                   %.2lf\n", grafoSDRParam[contador].priorizacoes.eusdGrupoA);
        printf("\t \t EUSD Grupo B:                   %.2lf\n", grafoSDRParam[contador].priorizacoes.eusdGrupoB);
        printf("\t \t Numero Consumidores:           %10d\n", grafoSDRParam[contador].priorizacoes.qtdConsumidores);
        printf("\t \t Numero Consumidores Especiais: %10d\n", grafoSDRParam[contador].priorizacoes.qtdConsumidoresEspeciais);
        printf("\n");
       /* printf("\t Dados Compensacoes: \n");
        printf("\t \t Meta DIC Mensal:          %.2lf\n", grafoSDRParam[contador].compensacoes.metaDicMensal);
        printf("\t \t Meta DIC Trimestral:      %.2lf\n", grafoSDRParam[contador].compensacoes.metaDicTrimestral);
        printf("\t \t Meta DIC Anual:           %.2lf\n", grafoSDRParam[contador].compensacoes.metaDicAnual);
        printf("\t \t Meta DMIC Mensal:         %.2lf\n", grafoSDRParam[contador].compensacoes.metaDmicMensal);
        printf("\t \t Meta FIC Mensal:          %.2lf\n", grafoSDRParam[contador].compensacoes.metaFicMensal);
        printf("\t \t Meta FIC Trimestral:      %.2lf\n", grafoSDRParam[contador].compensacoes.metaFicTrimestral);
        printf("\t \t Meta FIC Anual:           %.2lf\n", grafoSDRParam[contador].compensacoes.metaFicAnual);
        printf("\t \t Acumulado DIC Mensal:     %.2lf\n", grafoSDRParam[contador].compensacoes.resulDicAcumMes);
        printf("\t \t Acumulado DIC Trimestral: %.2lf\n", grafoSDRParam[contador].compensacoes.resulDicAcumTrimestre);
        printf("\t \t Acumulado DIC Anual:      %.2lf\n", grafoSDRParam[contador].compensacoes.resulDicAcumAno);
        printf("\t \t Acumulado DMIC Mensal:    %.2lf\n", grafoSDRParam[contador].compensacoes.resulDmicMes);
        printf("\t \t Acumulado FIC Mensal:     %.2lf\n", grafoSDRParam[contador].compensacoes.resulFicAcumMes);
        printf("\t \t Acumulado FIC Trimestral: %.2lf\n", grafoSDRParam[contador].compensacoes.resulFicAcumTrimestre);
        printf("\t \t Acumulado FIC Anual:      %.2lf\n", grafoSDRParam[contador].compensacoes.resulFicAcumAno);
        printf("\t Numero Adjacentes: %d \n", grafoSDRParam[contador].numeroAdjacentes);
        printf("\t");*/
        //Imprime cada barra da lista de adjacentes
        for(contadorAdj = 0; contadorAdj<grafoSDRParam[contador].numeroAdjacentes; contadorAdj++)
        {
            printf("\t Barra Adjacente %ld Estado chave %d", grafoSDRParam[contador].adjacentes[contadorAdj].idNo,grafoSDRParam[contador].adjacentes[contadorAdj].estadoChave);
        }
        printf("\n");
    }
    

}

/**
 * @brief Função para imprimir os dados lidos sobre os trafos do SDR.
 *
 * Essa função recebe como parâmetro uma variavél do tipo DADOSTRAFO e realiza a impressão
 * na tela dos dados contidos na estrutura  informada. É utilizada para validar o processo executado pela função de leitura.
 
 * Exemplo:
 * @code
 * GRAFO *grafoExemplo;
 * DADOSTRAFO *trafoExemplo;
 * BOOL leitura = leituraBarrasLinhasTrafos( &grafoExemplo, &trafoExemplo);
 * imprimeTrafos(trafoExemplo);
 * @endcode
 * 
 * @param dadosTrafoSDRParam é um ponteiro da estrutura do tipo do DADOSTRAFO.

 */
void imprimeTrafos(DADOSTRAFO *dadosTrafoSDRParam, long int numeroTrafos)
{
    int contador;
    //Loop para imprimir os dados de cada um dos trafos
    for(contador = 1; contador<=numeroTrafos; contador++)
    {
        printf("Identificador Trafo %d \n", dadosTrafoSDRParam[contador].idTrafo);
        printf("\t Identificador SubEstacao %d \n", dadosTrafoSDRParam[contador].idSubEstacao);
        printf("\t Capacidade %lf \n",dadosTrafoSDRParam[contador].capacidade);
        printf("\t Tensao Real %lf \n",dadosTrafoSDRParam[contador].tensaoReal);
    }    

}

/**
 * @brief Função para imprimir os dados lidos sobre os trafos do SDR.
 *
 * Essa função recebe como parâmetro uma variavél do tipo DADOSTRAFO e realiza a impressão
 * na tela dos dados contidos na estrutura  informada. É utilizada para validar o processo executado pela função de leitura.
 
 * Exemplo:
 * @code
 * GRAFO *grafoExemplo;
 * DADOSTRAFO *trafoExemplo;
 * BOOL leitura = leituraBarrasLinhasTrafos( &grafoExemplo, &trafoExemplo);
 * imprimeTrafos(trafoExemplo);
 * @endcode
 * 
 * @param dadosTrafoSDRParam é um ponteiro da estrutura do tipo do DADOSTRAFO.

 */
void imprimeReguladores(DADOSREGULADOR *dadosReguladorParam, long int numeroReguladoresParam)
{
    int contador;
    //Loop para imprimir os dados de cada um dos trafos
    for(contador = 0; contador<numeroReguladoresParam; contador++)
    {
        printf("Identificador Regulador %s \n", dadosReguladorParam[contador].idRegulador);
        printf("\t Ampacidade %lf \n", dadosReguladorParam[contador].ampacidade);
        printf("\t Taps %d \n",dadosReguladorParam[contador].numeroTaps);
        printf("\t Tipo %d \n",dadosReguladorParam[contador].tipoRegulador);
    }    

}

/**
 * @brief Função principal para a leitura do arquivo dados_alimentadores.dad.
 *
 * Essa função realiza a leitura do arquivo dados_alimentadores.dad. A função assume que o nome do arquivo é padrão da forma 
 * dados_alimentadores.dad. O arquivo inicia com o marcador DALIM que é verificado pela função.
 * Localizado o marcador a função dá procedimento a leitura dos dados do arquivo armazenando-os na estrutura de dados definida.
 * Tem como parâmentro de entrada e saída um ponteiro para ponteiro da estrutura do tipo DADOS ALIMNETADORES @p **dadosAlimentadoresSDRParam.
 * A função retorna @c BOOL indicando se a leitura foi bem sucedida.
 * Para utilizar a função:
 * @code
 * DADOSALIMENTADORES *alimentadorExemplo;
 * BOOL leitura = leituraDadosAlimentadores( &alimentadorExemplo);
 * if (leitura)
 *      printf("leitura concluida\n");
 * @endcode
 * 
 * @param dadosAlimentadoreSDRParam é um ponteiro para o ponteiro da estrutura do tipo do DADOSALIMENTADORES, onde é retornado os dados dos alimentadores.
 * @return Retorna o tipo booleano definido indicando que a leitura foi bem sucedida.
 */
BOOL leituraDadosAlimentadores(DADOSALIMENTADOR **dadosAlimentadorSDRParam)
{
    FILE *arquivo;  //Variável ponteiro do tipo FILE para ler e manipular o arquivo.
    char blocoLeitura[100]; //Variável para realizar a leitura do bloco de caracteres do arquivo.
    char marcador[10]; //Variável para leitura e verificação do marcador de formato do arquivo.
    long int contador; //Variável contador para realizar a leitura das linhas de dados do arquivo.
    long int numAlim; // Variável para leitura do número de dados de alimentadores contidos no arquivo.
    char idAlimentador[15]; //Variável para leitura do identificador do alimentador.
    long int idBarraAlimentador; //Variável para leitura do identificador da barra do alimentador.
    int idTrafoAlimentador; //Variável para leitura do identificador do trafo onde o alimentador está ligado.
    
    //Abertura e associação do arquivo dados_alimentadores.dad á variáve arquivo
    arquivo = fopen("dados_alimentadores.dad","r");
    //Verifica se foi possível abrir o arquivo e encerra o programa se ocorreu um erro.
    if(arquivo == NULL)
    {
            printf("Erro ao abrir arquivo dados_alimentadores.dad !!!\n");
            exit(1);
    }
    
    //Leitura do bloco de dados do arquivo
    fgets(blocoLeitura,100,arquivo);
    
    //Leitura a partir da string blocoLeitura do marcador do arquivo
    sscanf(blocoLeitura,"%s",marcador);
    
    //Verifica se o arquivo contém o marcador especificado
    if (!strcmp(marcador, "DALIM"))
    {
       //Leitura do bloco de dados do arquivo 
       fgets(blocoLeitura,100,arquivo);
       //Leitura a parti da variável blocoLeitura do número de alimentadores
       sscanf(blocoLeitura,"%ld",&numAlim);
       //Atribui o número de alimentadores a variável global
       numeroAlimentadores = numAlim;
       printf("numeroAlimentadores %ld \n", numAlim);
       //Aloca a memória para a estrutura de dados dos alimentadores e verifica se a alocação ocorreu com sucesso. Caso contrário o programa é encerrado.
       if (((*dadosAlimentadorSDRParam) = (DADOSALIMENTADOR *)malloc( (numAlim+1) * sizeof(DADOSALIMENTADOR)))==NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para os dados dos alimentadores do SDR !!!!");
            exit(1); 
        }
       //Loop para leitura dos dados dos alimentadores
       for(contador=1; contador<=numAlim; contador++)
       {
           //Leitura do bloco de dados do arquivo
           fgets(blocoLeitura, 100, arquivo);
           //Leitura a partir da string blocoLeitura dos dados dos alimentadores
           sscanf(blocoLeitura,"%s %ld %d", idAlimentador,&idBarraAlimentador,&idTrafoAlimentador);
           
           //Insere os dados dos alimentadores na estrutura
           (*dadosAlimentadorSDRParam)[contador].barraAlimentador = idBarraAlimentador;
           (*dadosAlimentadorSDRParam)[contador].idTrafo = idTrafoAlimentador;
           strcpy((*dadosAlimentadorSDRParam)[contador].idAlimentador, idAlimentador);
       }
    }
    else
    {
        printf("Erro ao ler arquivo dados_alimentadores.dad !!! \n Formato Invalido!!!\n");
        exit(1);
    }
    
    //fecha o arquivo lido.
    fclose(arquivo);
    return(true);
}

/**
 * @brief Função principal para a leitura do arquivo dados_reguladores_tensao.dad.
 *
 * Essa função realiza a leitura do arquivo dados_reguladores_tensao.dad. A função assume que o nome do arquivo é padrão da forma 
 * dados_reguladores_tensao.dad. O arquivo inicia com o marcador DBRDT que é verificado pela função.
 * Localizado o marcador a função dá procedimento a leitura dos dados do arquivo armazenando-os na estrutura de dados definida.
 * Tem como parâmentro de entrada e saída um ponteiro para ponteiro da estrutura do tipo DADOSREGULADOR @p **dadosReguladoresSDRParam.
 * A função retorna @c BOOL indicando se a leitura foi bem sucedida.
 * Para utilizar a função:
 * @code
 * DADOSREGULADOR *reguladorExemplo;
 * BOOL leitura = leituraDadosReguladoresTensao( &reguladorExemplo);
 * if (leitura)
 *      printf("leitura concluida\n");
 * @endcode
 * 
 * @param dadosReguladorSDRParam é um ponteiro para o ponteiro da estrutura do tipo do DADOSREGULADOR, onde é retornado os dados dos reguladores de tensão.
 * @return Retorna o tipo booleano definido indicando que a leitura foi bem sucedida.
 */
BOOL leituraDadosReguladoresTensao(DADOSREGULADOR **dadosReguladorSDRParam)
{
    FILE *arquivo; //Variável ponteiro do tipo FILE para ler e manipular o arquivo.
    char blocoLeitura[100]; //Variável para realizar a leitura do bloco de caracteres do arquivo.
    char marcador[10]; //Variável para leitura e verificação do marcador de formato do arquivo.
    int contador; //Variável contador para controlar a leitura do arquivo
    long int numRegulador; //Variável para armazenar o número de reguladores de tensão
    char idRegulador[30]; //Variável para o identificador do regulador de tensão
    int tipoRegulador; //Variável para fazer a leitura do tipo do regulador
    double ampacidade; //Variável para fazer a leitura da ampacidade
    int numeroTaps; //Variável para fazer a leitura da correção do regulador
    
    //Abertura do arquivo dados_reguladores_tensao.dad e associação a variável arquivo
    arquivo = fopen("dados_reguladores_tensao.dad","r");
    //Verifica se a abertura do arquivo ocorreu com sucesso. Caso contrário encerra o programa.
    if(arquivo == NULL)
    {
            printf("Erro ao abrir arquivo dados_reguladores_tensao.dad !!!\n");
            exit(1);
    }
    
    //leitura do bloco de caracteres do arquivo
    fgets(blocoLeitura,100,arquivo);
    //leitura da string blocoLeitura do marcador 
    sscanf(blocoLeitura,"%s",marcador);
    //verifica se o conteúdo da variável marcador corresponde ao marcador do arquivo. Caso contrário o programa é encerrado
    if (!strcmp(marcador, "DBRT"))
    {
       //leitura do bloco de caracteres do arquivo
       fgets(blocoLeitura,100,arquivo);
       //leitura do numero de reguladores da string blocoLeitura
       sscanf(blocoLeitura,"%ld",&numRegulador);
       //Atribui o número de reguladores para a variável global
       numeroReguladores = numRegulador;
       //Aloca a memória para armazenar os dados dos reguladores de tensão e verifica se a alocação foi vem sucedida.
       if (((*dadosReguladorSDRParam) = (DADOSREGULADOR *)malloc( numRegulador * sizeof(DADOSREGULADOR)))==NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para os dados dos alimentadores do SDR !!!!");
            exit(1); 
        }
       //Leitura dos dados dos alimentadores
       for(contador=0; contador<numRegulador; contador++)
       { 
           //leitura do bloco de caracteres do arquivo
           fgets(blocoLeitura, 100, arquivo);
           sscanf(blocoLeitura, "%s %d %lf %d", idRegulador,&tipoRegulador,&ampacidade,&numeroTaps); 
           //Atribui os dados lidas para a estrutura com os dados dos reguladores de tensão
           strcpy((*dadosReguladorSDRParam)[contador].idRegulador, idRegulador);
           (*dadosReguladorSDRParam)[contador].tipoRegulador = tipoRegulador;
           (*dadosReguladorSDRParam)[contador].ampacidade = ampacidade;
           (*dadosReguladorSDRParam)[contador].numeroTaps = numeroTaps;
       }
    }
    else
    {
        printf("Erro ao ler arquivo dados_reguladores_tensao.dad !!! \n Formato Invalido!!!\n");
        exit(1);
    }
    
    //fecha o arquivo
    fclose(arquivo);
    return(true);
}

void leituraParametros(long int *numeroGeracoes, int *tamanhoTabelas, int *SBase, int *tamanhoPopulacaoInicial, int *seed)
{
    
    FILE *arquivo; //Variável ponteiro do tipo FILE para ler e manipular o arquivo.
    char blocoLeitura[100]; //Variável para realizar a leitura do bloco de caracteres do arquivo.
    int contador;
    
    //Abertura do arquivo dados_reguladores_tensao.dad e associação a variável arquivo
    arquivo = fopen("parametros.dad","r");
    //Verifica se a abertura do arquivo ocorreu com sucesso. Caso contrário encerra o programa.
    if(arquivo == NULL)
    {
            printf("Erro ao abrir arquivo parametros.dad !!!\n");
            exit(1);
    }
    
    //leitura do bloco de caracteres do arquivo
    fgets(blocoLeitura,100,arquivo);
    //leitura da string blocoLeitura do numero de gerações 
    sscanf(blocoLeitura,"%d",&seed[0]);
    
    //leitura do bloco de caracteres do arquivo
    fgets(blocoLeitura,100,arquivo);
    //leitura da string blocoLeitura do numero de gerações 
    sscanf(blocoLeitura,"%ld",&numeroGeracoes[0]);
    fgets(blocoLeitura,100,arquivo);
    //leitura da string blocoLeitura do tamanho da população inicial 
    sscanf(blocoLeitura,"%d",&tamanhoPopulacaoInicial[0]);
    //leitura do bloco de caracteres do arquivo
    fgets(blocoLeitura,100,arquivo);
    //leitura da string blocoLeitura do tamanho de cada tabela 
    sscanf(blocoLeitura,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d ",&tamanhoTabelas[0], &tamanhoTabelas[1],&tamanhoTabelas[2],&tamanhoTabelas[3],&tamanhoTabelas[4],&tamanhoTabelas[5], &tamanhoTabelas[6], &tamanhoTabelas[7], &tamanhoTabelas[8], &tamanhoTabelas[9], &tamanhoTabelas[10], &tamanhoTabelas[11], &tamanhoTabelas[12], &tamanhoTabelas[13]);
   //leitura do bloco de caracteres do arquivo
    fgets(blocoLeitura,100,arquivo);
    //leitura da string blocoLeitura do numero de gerações 
    sscanf(blocoLeitura,"%d",&SBase[0]);
    
    
}

void leituraDadosEntrada(int *numeroSetorFalta, long int **setoresFalta)
{
    
    FILE *arquivo; //Variável ponteiro do tipo FILE para ler e manipular o arquivo.
    char blocoLeitura[100]; //Variável para realizar a leitura do bloco de caracteres do arquivo.
    int contador;
    long int setor;
    
    //Abertura do arquivo dados_reguladores_tensao.dad e associação a variável arquivo
    arquivo = fopen("dadosEntrada.dad","r");
    //Verifica se a abertura do arquivo ocorreu com sucesso. Caso contrário encerra o programa.
    if(arquivo == NULL)
    {
            printf("Erro ao abrir arquivo dadoEntrada.dad !!!\n");
            exit(1);
    }

 //leitura do bloco de caracteres do arquivo
    fgets(blocoLeitura,100,arquivo);
    //leitura da string blocoLeitura do numero de setores em falta 
    sscanf(blocoLeitura,"%d",&numeroSetorFalta[0]);
    if (((*setoresFalta) = (long int *)malloc( (numeroSetorFalta[0]+1) * sizeof(long int)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para o grafo do SDR !!!!");
        exit(1); 
    }
    for(contador =0; contador < numeroSetorFalta[0]; contador++)
    {
        //leitura do bloco de caracteres do arquivo
        fgets(blocoLeitura,100,arquivo);
        //leitura da string blocoLeitura do numero de gerações 
        sscanf(blocoLeitura,"%ld",&setor);
        (*setoresFalta)[contador] = setor;
    }
}