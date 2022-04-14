#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesRNP.h"
#include "funcoesSetor.h"
#include "funcoesAuxiliares.h"

int maximoTentativas = 5; //define o número máximo de tentativas na escolha dos nós para os operadores PAO e CAO

/**
 * Realiza a inserçõa dos nós utilizados para a obtenção da nova configuração a partir dos operadores
 * PAO e CAO. 
 * @param conjuntoNosParam
 * @param p
 * @param a
 * @param iadj
 * @param pos
 */
void insereNos(NOSPAOCAO *conjuntoNosParam, long int p, long int r, long int a, int iadj, int pos) {

    conjuntoNosParam[pos].p = p;
    conjuntoNosParam[pos].a = a;
    conjuntoNosParam[pos].r = r;
    conjuntoNosParam[pos].iadj = iadj;
}
/**
 * Faz a inserção no vetor pi da RNP da configuração que deu origem a nova configuração e dos nós utilizados
 * @param piParam
 * @param idIndividuoParam
 * @param idAncestralParam
 * @param novosNosParam
 * @param numeroAplicacoes
 */
void atualizaVetorPi(VETORPI *piParam, long int idIndividuoParam, long int idAncestralParam, long int *idChaveAbertaParam, long int *idChaveFechadaParam, int numeroManobras) {
    int indice;
    piParam[idIndividuoParam].idAncestral = idAncestralParam;
    piParam[idIndividuoParam].numeroManobras = numeroManobras;
    piParam[idIndividuoParam].idChaveAberta = Malloc(long int, numeroManobras + 1);
    piParam[idIndividuoParam].idChaveFechada = Malloc(long int, numeroManobras + 1);
    if (numeroManobras > 0)
        for (indice = 0; indice < numeroManobras; indice++) {
            piParam[idIndividuoParam].idChaveAberta[indice] = idChaveAbertaParam[indice];
            piParam[idIndividuoParam].idChaveFechada[indice] = idChaveFechadaParam[indice];
        }
}

/**
 * @brief Encontra o nó adjacente à p ou r passado como parâmetro.
 * A função retorna a coluna da matriz PI relativa ao nó a, por meio da coluna obtém-se a RNP e o índice dela correspondente ao nó
 * Por meio da RNP identificamos os demais dados relativos ao nó.
 * 
 * @param tentativasParam: número de tentativas que serão realizadas na busca pelo nó a
 * @param idNoParam
 * @param florestaParam
 * @param idRNPParam
 * @param grafoSetoresParam
 * @param matrizPiParam
 * @param vetorPiParam
 * @return 
 */
COLUNAPI * determinaNoAdjacente(int tentativasParam, long int idNoParam, CONFIGURACAO florestaParam, 
        int idRNPParam, GRAFOSETORES *grafoSetoresParam, MATRIZPI *matrizPiParam, VETORPI *vetorPiParam) 
{
    long int idNoA;
    COLUNAPI *colunaNoAdjacente = NULL;
    //seleciona um número aleatório da lista de adjacências do nó passado como parâmetro
    int indice = inteiroAleatorio(0, (grafoSetoresParam[idNoParam].numeroAdjacentes-1));
    BOOL encontrado = false;
    int tentativas = 0;
    //executa um loop até alcançar o número de tentativas ou encontrar um nó adjacente válido
    while (tentativas < tentativasParam && !encontrado) {
        //recupera o identificador do nó contido na posição aleatória selecionada
       // printf("indice %d idNoParam %ld \n", indice, idNoParam);
        idNoA = grafoSetoresParam[idNoParam].setoresAdjacentes[indice];
        if (grafoSetoresParam[idNoA].setorFalta == false) {
            //busca na matriz PI a posição e RNP do nó selecionado
            colunaNoAdjacente = retornaColunaPi(matrizPiParam, vetorPiParam, idNoA, florestaParam.idConfiguracao);
            //verifica se o nó selecionado está em árvore diferente da passada como parâmetro
            if (colunaNoAdjacente[0].idRNP != idRNPParam) {
                encontrado = true;
            } else //o nó está na mesma árvore deve-se selecionar outro 
            {
                indice = inteiroAleatorio(0, (grafoSetoresParam[idNoParam].numeroAdjacentes-1));
            }
        }
        tentativas++;
    }
  //  printf("tentativas %d tentativasParam %d encontrado %d \n", tentativas, tentativasParam, encontrado);
    //retorna a coluna da matriz pi do nó selecionado
    if(encontrado)
    {
        return colunaNoAdjacente;
    }
    else
        return NULL;
}


/**
 * Realiza a busca binária nas colunas da matriz PI de um nó para localizar a posição deste na configuração
 * Recebe como parâmetros a matrizPI do nó, o índice do nó, o valor da busca e o número de colunas preenchidas.
 * Retorna o índice da coluna.
 *  
 * @param matrizPIParam
 * @param indiceNoParam
 * @param valorBusca
 * @param numeroColunasParam
 * @return 
 */
int buscaBinariaColunaPI(MATRIZPI *matrizPIParam, int indiceNoParam, int valorBusca, int numeroColunasParam)
{
    int indicePrimeiro = 0;
    int indiceMeio = (indicePrimeiro + numeroColunasParam) / 2;
//percorre as colunas até encontrar o vértice ou pesquisar toda a matriz
    while ((indicePrimeiro <= numeroColunasParam) && (matrizPIParam[indiceNoParam].colunas[indiceMeio]->idConfiguracao!= valorBusca)) {
        //verifica qual o próxima faixa de índices que deve ser procurada com base no valor do meio da matriz
        if (matrizPIParam[indiceNoParam].colunas[indiceMeio]->idConfiguracao > valorBusca) {
            numeroColunasParam = indiceMeio - 1;
        }//Busca na parte superior
        else
            if (matrizPIParam[indiceNoParam].colunas[indiceMeio]->idConfiguracao < valorBusca) {
            indicePrimeiro = indiceMeio + 1;
        }//Busca na parte inferior
        indiceMeio = (indicePrimeiro + numeroColunasParam) / 2;

    }
    //retorna o índice do meio
    if (matrizPIParam[indiceNoParam].colunas[indiceMeio]->idConfiguracao == valorBusca)
        return indiceMeio; //encontrou, retorna a posicao
    else
        return -1; //nao encontrou, retorna um aviso
}

/**
 * Realiza a busca binária na lista de matrizes PI pela matriz de um nó especifico.
 * Retorna o índice da posição.
 * 
 * @param matrizPIParam
 * @param parametroBusca
 * @param ultimaPosicaoParam
 * @return 
 */
int buscaBinariaMatrizPI(MATRIZPI *matrizPIParam, int parametroBusca, int ultimaPosicaoParam)
{
    int indicePrimeiro = 0;
    int indiceMeio = (indicePrimeiro + ultimaPosicaoParam) / 2;

    //percorre a lista de forma binária começando pelo indice central
    while ((indicePrimeiro <= ultimaPosicaoParam) && (matrizPIParam[indiceMeio].idNo != parametroBusca)) {
        //atualiza os extremos para direcionar a busca de acordo com o valor do índice do meio
        if (matrizPIParam[indiceMeio].idNo > parametroBusca) {
            ultimaPosicaoParam = indiceMeio - 1;
        }//Busca na parte superior
        else
            if (matrizPIParam[indiceMeio].idNo < parametroBusca) {
            indicePrimeiro = indiceMeio + 1;
        }//Busca na parte inferior
        indiceMeio = (indicePrimeiro + ultimaPosicaoParam) / 2;

    }
    if (matrizPIParam[indiceMeio].idNo == parametroBusca)
        return indiceMeio; //encontrou, retorna a posicao
    else
        return -1; //nao encontrou, retorna um aviso

}

/**
 * Recebe um nó como parâmetro e retorna a coluna da PI correspondente ao no em sua última alteração relativa a uma 
 * configuração esperada.
 * Tem como parâmetros a matrizPI do nó, o vetorPi de configurações, o nó buscado e a configuração desejada.
 * @param matrizPIParam
 * @param vetorPiParam
 * @param idNoParam
 * @param idIndividuoParam
 * @return 
 */
COLUNAPI *retornaColunaPi(MATRIZPI *matrizPIParam, VETORPI *vetorPiParam, int idNoParam, int idConfiguracaoParam)
{
    int indice;
    int indiceColuna;
    int idConfiguracao;
    idConfiguracao = idConfiguracaoParam;

    indice = idNoParam;
    //verifica se o identificador passado como parâmetro é válido
    if (indice >= 0) {
        //busca nas colunas da matriz se existe uma coluna relativa a configuração informada
        indiceColuna = buscaBinariaColunaPI(matrizPIParam, indice, idConfiguracao, matrizPIParam[indice].numeroColunas - 1); //busca a coluna para a floresta desejada
        
        //enquanto nao encontra a coluna para a configuração desejada repete a busca com o valor do ancestral
        //pois, se a coluna não existe para uma configuração isso indica que nó não mudou de posição em relação ao ancestral
        while (indiceColuna < 0) 
        {

            //recupera o ancestral
            idConfiguracao = vetorPiParam[idConfiguracao].idAncestral; // inFirstForest possui o nome da primeira floresta e pode ser diferente de zero

            if (idConfiguracao >= 0)
                //refaz a busca pelas colunas
                indiceColuna = buscaBinariaColunaPI(matrizPIParam, indice, idConfiguracao, matrizPIParam[indice].numeroColunas - 1);
            else
                exit(1);

        }

        return matrizPIParam[indice].colunas[indiceColuna];
    } else
        return NULL;
}

/**
 * Procedimento para adicionar uma coluna na matriz PI de um nó ou, se existir, a coluna para a configuração atualiza os valores
 * Recebe como parâmetros a matriz P, o identificador do nó, o indentificador da configuração, o identificador da RNP, e a posição do nó na RNP
 * @param matrizPIParam
 * @param idNoParam
 * @param idConfiguracaoParam
 * @param idRNPParam
 * @param indiceParam
 */
void adicionaColuna(MATRIZPI *matrizPIParam, int idNoParam, int idConfiguracaoParam, int idRNPParam, int indiceParam) 
{

    int indiceColuna;
    //verifica se já existe uma coluna na configuracao referente ao identficador de configuração passado como parâmetro
    if(matrizPIParam[idNoParam].numeroColunas >0)
    {
        indiceColuna = buscaBinariaColunaPI(matrizPIParam, idNoParam, idConfiguracaoParam, matrizPIParam[idNoParam].numeroColunas - 1);
    }
    else
        indiceColuna = -1;
 
    //se a coluna não existe faz a inserção dos dados e incrementa o contador de colunas
    if (indiceColuna == -1) 
    {
        indiceColuna = matrizPIParam[idNoParam].numeroColunas;
        if(matrizPIParam[idNoParam].numeroColunas+2 == matrizPIParam[idNoParam].maximoColunas)
        {
            int tamanho = matrizPIParam[idNoParam].maximoColunas + 1000;
            //printf("numero linhas %d tamanho %d\n",matrizPIParam[idNoParam].numeroColunas, tamanho);
            COLUNAPI ** temporario;
            temporario = (COLUNAPI **) realloc(matrizPIParam[idNoParam].colunas, (tamanho) * sizeof(COLUNAPI *));
            if(temporario == NULL)
            {
                printf("Nao foi possível expandir colunas da matrizPi do setor %ld\n", matrizPIParam[idNoParam].idNo);
                exit(1);
            }
            else {
                matrizPIParam[idNoParam].colunas = temporario;
                matrizPIParam[idNoParam].maximoColunas += 1000;
                int contador;
                for (contador = matrizPIParam[idNoParam].numeroColunas + 1; contador < matrizPIParam[idNoParam].maximoColunas; contador++)
                    if ((matrizPIParam[idNoParam].colunas[contador] = Malloc(COLUNAPI, 1)) == NULL) {
                            printf("Nao foi possivel alocar coluna %d \n", contador);
                            exit(1);
                        }
            }
        }    
        matrizPIParam[idNoParam].colunas[indiceColuna]->idConfiguracao = idConfiguracaoParam;
        matrizPIParam[idNoParam].colunas[indiceColuna]->idRNP = idRNPParam;
        matrizPIParam[idNoParam].colunas[indiceColuna]->posicao = indiceParam;
        matrizPIParam[idNoParam].numeroColunas++;
    } else { //se existe atualiza a RNP e a posição
        matrizPIParam[idNoParam].colunas[indiceColuna]->idRNP = idRNPParam;
        matrizPIParam[idNoParam].colunas[indiceColuna]->posicao = indiceParam;
    }
}

/**
 * Realiza a busca na RNP pela posição na RNP que determina o fim da subárvore enraizada no nó informado.
 * Recebe como parâmetros o ponteiro da RNP e o índice do nó raiz da subárvore
 * 
 * @param rnpParam
 * @param indiceNoPParam
 * @return 
 */
int limiteSubArvore(RNP rnpParam, int indiceNoPParam)
{
    int indice;
    int tamanho;
    indice = indiceNoPParam + 1;

       tamanho = rnpParam.numeroNos;
       //percorre a RNP até encontrar um nó de profundidade igual ou inferior ao nó raiz da subárvore ou o fim da RNP
        while (indice < tamanho && rnpParam.nos[indiceNoPParam].profundidade < rnpParam.nos[indice].profundidade) {

            indice++;
        }
   //retorna o índice na rnp do fim da subárvore.
    return --indice;
}

/**
 * Esta função retorna um ponteiro para a lista de configurações. Recebe como parâmetros o número de RNPs de cada configuração
 * o identificador do individuo inicial e o número de configurações que compoem a lista.
 * 
 * @param numeroRNPParam
 * @param idIndividuoInicialParam
 * @param numeroConfiguracoesParam
 * @return 
 */
CONFIGURACAO* alocaIndividuo(int numeroRNPParam, long int idIndividuoInicialParam, long int numeroConfiguracoesParam)
{
    //aloca a lista de configurações o identificador do individuo inicial é somado ao numero de configurações para permitir o uso 
    //do identificador como índice da lista.
    CONFIGURACAO *individuo = Malloc(CONFIGURACAO, (numeroConfiguracoesParam+idIndividuoInicialParam));
    long int indice;
    //percorre a lista de configurações fazendo a alocação do vetor de RNPs da configuração
    for (indice = idIndividuoInicialParam; indice <(numeroConfiguracoesParam+idIndividuoInicialParam); indice++ )
    {
        individuo[indice].numeroRNP = numeroRNPParam;
        individuo[indice].rnp = Malloc(RNP, individuo[indice].numeroRNP);
        individuo[indice].idConfiguracao = indice;

    }
    return individuo;
}

/**
 * Realiza a alocação do vetor de nós da RNP passada como parâmetro. Para isso recebe como parâmetro o número de nós que compõem a árvore
 * representada pela RNP.
 * 
 * @param numeroNosParam
 * @param rnpParam
 */
void alocaRNP(int numeroNosParam, RNP *rnpParam) 
{
    //preenche os campos da RNP
    rnpParam[0].numeroNos = numeroNosParam;
    //aloca o vetor de nós
    rnpParam[0].nos = Malloc(NORNP,numeroNosParam);
}

/**
 * Realiza a impressão na tela da configuração passada como parâmetro. Para isso faz uso da função que
 * imprime uma RNP.
 * 
 * @param individuoParam
 */
void imprimeIndividuo(CONFIGURACAO  individuoParam)
{
    int indice;
   //imprime na tela o identificador do indivíduo
    printf("\nIndividuo %ld", individuoParam.idConfiguracao);
    //percorre o vetor de RNPs utilizando a função imprimeRNP para imprimir a RNP

    for (indice = 0; indice < individuoParam.numeroRNP; indice++) 
    {
        
        imprimeRNP(individuoParam.rnp[indice]);
     }
}
/**
 * Realiza a impressão na tela da configuração passada como parâmetro. Para isso faz uso da função que
 * imprime uma RNP.
 * 
 * @param individuoParam
 */
void gravaIndividuo(char *nomeFinal, CONFIGURACAO individuoParam) {
    int indice, indice2;
    FILE *arquivo;
    char nomeArquivo[200];
    sprintf(nomeArquivo, "individuo%ld%s", individuoParam.idConfiguracao,nomeFinal);
    arquivo = fopen(nomeArquivo, "w");
    //imprime na tela o identificador do indivíduo
    fprintf(arquivo, "Individuo\t%ld\n", individuoParam.idConfiguracao);
    //percorre o vetor de RNPs utilizando a função imprimeRNP para imprimir a RNP
    for (indice = 0; indice < individuoParam.numeroRNP; indice++) {
        //imprime o campo nó do vetor de nós da RNP
        for (indice2 = 0; indice2 < individuoParam.rnp[indice].numeroNos; indice2++) {
            fprintf(arquivo, "%ld\t", individuoParam.rnp[indice].nos[indice2].idNo);
        }
        fprintf(arquivo, "\n");

        //imprime o campo profundidade do vetor de nós da RNP
        for (indice2 = 0; indice2 < individuoParam.rnp[indice].numeroNos; indice2++) {
            fprintf(arquivo,"%d\t", individuoParam.rnp[indice].nos[indice2].profundidade);
        }
        fprintf(arquivo, "\n");
        fprintf(arquivo, "\n");
        
    }
    fclose(arquivo);
}


/**
 * Imprime na tela a RNP passada como parâmetro. Imprime o vetor de nós, com o identificador do nó e a profundidade
 * @param rnpParam
 */
void imprimeRNP(RNP rnpParam) 
{
    int indice;
    //imprime o número de nós da RNP
    printf("\n");
    printf("\t-- RNP -- Qtd Nos %d -- ", rnpParam.numeroNos);
    printf("\n");

    //imprime o campo nó do vetor de nós da RNP
    printf("\t\tNos:         ");
    for (indice = 0; indice < rnpParam.numeroNos; indice++) 
    {
        printf("%6ld ", rnpParam.nos[indice].idNo);
    }
    printf("\n");
    
    //imprime o campo profundidade do vetor de nós da RNP
    printf("\t\tProfundidade:");
    for (indice = 0; indice < rnpParam.numeroNos; indice++) 
    {
        printf("%6i ", rnpParam.nos[indice].profundidade);
    }
    printf("\n");

}

/**
 * Libera a memória utilizada para armazenar uma configuração. Recebe como parâmetro a configuração para ser desalocada
 * @param configuracaoParam
 */
void desalocaConfiguracao(CONFIGURACAO configuracaoParam) {
    long int indice = 0;
   
    //percorre o vetor de RNPs liberando a memória utilizada
    while (indice < configuracaoParam.numeroRNP) {
        free(configuracaoParam.rnp[indice].nos);
        configuracaoParam.rnp[indice].nos = NULL;

        indice++;
    }
    //libera a memória utilizada pelo vetor de RNPs
    free(configuracaoParam.rnp);
    configuracaoParam.rnp = NULL;

}

/**
 * Método responsável por fazer a alocação do vetor de MatrizPi para cada nó da configuração, e da matrizPi de cada nó.
 * Além disso, faz a inicialização dos valores de identificação da matriz
 * Recebe como parâmetros o grafo de setores, o vetor de Matriz Pi que será alocado, e o número de colunas da matriz de cada nó. 
 * @param grafoSetoresParam
 * @param matrizPIParam
 * @param numeroMaximoColunasParam
 */
void inicializaMatrizPI(GRAFOSETORES *grafoSetoresParam, MATRIZPI ** matrizPIParam, int numeroMaximoColunasParam) 
{
    long int indiceColuna;
    long int indice;
    
    //aloca o vetor de de matriz Pi    
    if(((*matrizPIParam) = (MATRIZPI *) malloc((numeroSetores+1) * sizeof(MATRIZPI)))==NULL)  
    {
        printf("Erro: Nao foi possivel alocar matriz PI");
        exit(1);
    }
    
    //percorre o vetor de matriz PI fazendo a alocação das matrizes e inicializando os dados
    for(indice=1; indice<=numeroSetores; indice++) {
        //insere o identificador do nó
        (*matrizPIParam)[indice].idNo = grafoSetoresParam[indice].idSetor;
        //inicializa o número de colunas preenchidas com zero
        (*matrizPIParam)[indice].numeroColunas = 0;
        (*matrizPIParam)[indice].maximoColunas = 0.2*numeroMaximoColunasParam;
        //aloca a matriz PI
      if(  ((*matrizPIParam)[indice].colunas = (COLUNAPI **) malloc((0.2*numeroMaximoColunasParam) * sizeof(COLUNAPI *))) ==  NULL) 
      {
          printf("Nao foi possivel alocar matriz PI setor %ld \n", indice);
          exit(1);
      }
        for (indiceColuna = 0; indiceColuna < (*matrizPIParam)[indice].maximoColunas; indiceColuna++) {
             if (((*matrizPIParam)[indice].colunas[indiceColuna] = Malloc(COLUNAPI, 1)) == NULL)
             {   
                 printf("Nao foi possivel alocar coluna %ld \n", indiceColuna);
                  exit(1);
             }
        }
    }
}

/**
 * Este método realiza a alocação e inicialização do vetor pi da RNP.
 * Tem como parâmetros o numero maximo de configurações e o vetor pi.
 * 
 * @param numeroMaximoConfiguracoesParam
 * @param vetorPiParam
 */
void inicializaVetorPi(int numeroMaximoConfiguracoesParam, VETORPI **vetorPiParam)
{
    int indice;
    //aloca o vetor pi 
    (*vetorPiParam) = (VETORPI *) malloc((numeroMaximoConfiguracoesParam) * sizeof(VETORPI));
    //inicializa os campos de cada posição do vetor pi
    for (indice = 0; indice < numeroMaximoConfiguracoesParam; indice++) 
    {
        (*vetorPiParam)[indice].idAncestral = -1;
        (*vetorPiParam)[indice].numeroManobras = -1;
    }
}