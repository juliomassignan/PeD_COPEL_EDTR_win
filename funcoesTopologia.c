/**
 * @file funcoesTopologia.c
 * @author Julio Augusto Druzina Massignan
 * @date 31 jul 2020
 * @brief Contém a implementação das funções utilizadas para o tratamento da topologia de redes elétricas trifásicas, para cálculos em pu e representação via RNP.
 * Este arquivo implementa as funções definidas na biblioteca funcoesTopologia.h
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <complex.h>

#include "data_structures.h"
#include "funcoesTopologia.h"
#include "funcoesMatematicas.h"
//------------------------------------------------------------------------------
//
// ROTINAS AUXILIARES
//
//------------------------------------------------------------------------------
/**
 * @brief Função auxiliar para alocar novo nó em lista encadeada do tipo FILABARRAS
 *
 * Essa função realiza a alocação de um novo nó na última posíção da lista encadeada do tipo TF_FILABARRAS @p **setor com a respectiva identificação 
 * @p idNo para o novo nó.
 * A função retorna @c void .
 * 
 * @param **setor ponteiro para ponteiro com o último elemento da lista encadeada
 * @param idNo inteiro que presenta a identificação do nó
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void adicionaNo(TF_FILABARRAS **setor, long int idNo)
{
    TF_FILABARRAS *novoSetor;
    TF_FILABARRAS *aux = NULL;
    
    novoSetor = (TF_FILABARRAS *) malloc(sizeof(TF_FILABARRAS));
    
    if(novoSetor == NULL) exit(EXIT_FAILURE);
    
    novoSetor->idNo = idNo;
    //novoSetor->prox = *setor;
    novoSetor->prox = NULL;
    aux = *setor;
    aux->prox = novoSetor;
    
    *setor = novoSetor;
}

/**
 * @brief Função auxiliar para alocar novo nó em lista encadeada do tipo FILABARRAS
 *
 * Essa função realiza a alocação de um novo nó na última posíção da lista encadeada do tipo TF_FILABARRAS @p **fila com a respectiva identificação 
 * @p idNo para o novo nó.
 * A função retorna @c void .
 * 
 * @param **fila ponteiro para ponteiro com o último elemento da lista encadeada
 * @param idNo inteiro que presenta a identificação do nó
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void adicionaNoNaFila(TF_FILABARRAS ** fila, long int idNo) {
    TF_FILABARRAS *novoVertice = NULL;
    TF_FILABARRAS *aux = NULL;
 
    novoVertice = (TF_FILABARRAS *)malloc(sizeof(TF_FILABARRAS ));
 
    if(novoVertice == NULL) {
        printf("erro insere_fila\n");
        exit(EXIT_FAILURE);
    }
    
    novoVertice->idNo = idNo;
    novoVertice->prox = NULL;
    
    if(*fila == NULL)
        *fila = novoVertice;
    else {
        aux = *fila;
        while(aux->prox !=NULL) aux = aux->prox;
        aux->prox = novoVertice;
    }
}

/**
 * @brief Função auxiliar para apontar para o próximo nó em lista encadeada do tipo FILABARRAS
 *
 * Essa função altera a posição do ponteiro TF_FILABARRAS @p **fila para o próximo nó da lista encadeada até que NULL seja encontrado.
 * A função retorna @c void .
 * 
 * @param **fila ponteiro para ponteiro com o próximo elemento da lista encadeada
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void apontaProxNoNaFila(TF_FILABARRAS ** fila) {
    TF_FILABARRAS *novoVertice = NULL;
    TF_FILABARRAS *aux = NULL;
    
    aux = *fila;
    while(aux->prox !=NULL) aux = aux->prox;
    *fila = aux;
    
}

/**
 * @brief Função auxiliar para retira o primeiro nó em lista encadeada do tipo FILABARRAS
 *
 * Essa função retira a primeira a posição do ponteiro TF_FILABARRAS @p **fila e aponta para o próximo nó da lista encadeada até que NULL seja encontrado.
 * A função retorna @c int .
 * 
 * @param **fila ponteiro para ponteiro com o próximo elemento da lista encadeada
 * @return int inteiro com a identificação do nó retirado
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
int retiraNoDaFila(TF_FILABARRAS ** fila) {
    TF_FILABARRAS *aux = NULL;
    
    int idNo = -1;
    
    aux = *fila;
    
    if(aux != NULL) *fila = aux->prox;
    
    idNo = aux->idNo;
    
    free(aux);
 
    return idNo;
}

/**
 * @brief Função auxiliar para testar se a lista encadeada está vazia
 *
 * Essa função testa se a posição atual do ponteiro TF_FILABARRAS @p **fila é NULL.
 * A função retorna @c BOOL .
 * 
 * @param **fila ponteiro com o nó atual da lista encadeada
 * @return BOOL
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL filaNaoVazia(TF_FILABARRAS * fila) {
     if(fila == NULL)
         return false;
     return true;
}

/**
 * @brief Função auxiliar para testar se a lista encadeada contém nó especificado
 *
 * Essa função testa a listaFILABARRAS @p **setor contém o nó especificado pela identificação @p idNo.
 * A função retorna @c BOOL .
 * 
 * @param **fila ponteiro com o nó atual da lista encadeada
 * @param idNo identificação do nó procurado na lista
 * @return BOOL
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL estaLista(TF_FILABARRAS *setor, int idNo) {
    TF_FILABARRAS *p;
    p = setor;
    while(p != NULL && p->idNo != idNo)
        p = p->prox;
    if(p != NULL && p->idNo == idNo)
        return true;
    else
        return false;

}

/**
 * @brief Função auxiliar para testar se o nó adjacente está conectado ao atual por ramo em estado fechado (ligado ou energizado)
 *
 * Essa função testa se o nó adjacente @p adjancete está concetado por um ramo no grafo cujo estado é fechado (ligado ou energizado).
 * A função retorna @c BOOL .
 * 
 * @param adjacente estrutura do tipo TF_NOADJACENTE para testar se possui ramo fechado
 * @return BOOL
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL ramoLigado(TF_NOADJACENTE adjacente) {
    if(adjacente.estado == fechado) return true;
    return false;
}


/**
 * @brief Função auxiliar para testar se dois nós estão conexos dentro da topologia do grafo
 *
 * Essa função testa se o nó especificados pelos identificadores @p idNoRaiz e o nó @p idNoAdj estão conectados dentro do grafo armazenado em @p grafo.
 * A função retorna @c BOOL .
 * 
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param idNoRaiz primeiro nó a ser buscada conectividade
 * @param idNoAdj segundo nó a ser buscada conectividade
 * @return BOOL
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
BOOL estaListaAdjacencias(TF_GRAFO *grafo, long int idNoRaiz, long int idNoAdj)
{
    int contador;
    for(contador =0; contador < grafo[idNoRaiz].numeroAdjacentes; contador++)
    {
        if(grafo[idNoRaiz].adjacentes[contador].idNo == idNoAdj)
            return true;
    }
    return false;
}



//------------------------------------------------------------------------------
//
// ROTINAS DE MONTAGEM DAS ESTRUTURAS DE DADOS DA TOPOLOGIA DA REDE
//
//------------------------------------------------------------------------------

/**
 * @brief Função recursiva para realizar busca em profundidade no grafo da rede elétrica
 *
 * Essa função realiza recursivamente uma busca em profundidade no grafo da rede elétrica @p grafo, atualizando a profundidade @p profundidade 
 * de cada nó @p idNo indicado no ponteiro de barras @p barraAtual para percorrer o grafo da rede conforme a conectivivdade da rede elétrica. Utiliza
 * um vetor auxiliar para controlar o nós já visitados na busca @p visitado e também atualiza o identificador de  alimentador @p idAlim de cada nó 
 * e a quantidade total de nós por alimentador @p nos_alim .
 * A função retorna @c void .
 * 
 * @param barraAtual ponteiro para a lista encaddeada com o nó atual da recursão
 * @param idNo identificador do nó atual da recursão
 * @param profundidade profundidade atual da recursão
 * @param visitado vetor auxiliar para controlar os nós já visitados pela busca
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param idAlim identificador do alimentador onde está sendo realizada a busca em profundidade
 * @param nos_alim quantidade total de nós do alimentaodor atualizada a cada recursão
 * @return void
 * @see buscaProfundidadeAlimentadores
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void buscaProfundidade(TF_FILABARRAS *barraAtual, long int idNo, int profundidade,  BOOL *visitado, TF_GRAFO * grafo, long int idAlim, long int *nos_alim)
{
    //Depth-Search Algorithm - busca no e a sua profundidade (gera RNP))
    long int barraAdj,i = 0;
    
    visitado[idNo] = true;
    barraAtual->profundidade = profundidade;
    TF_GRAFO * no = &grafo[idNo];
    grafo[idNo].idAlim = idAlim;
    grafo[idNo].profundidade = profundidade;
    //printf("\nidNo: %d  -  %d", grafo[idNo].barra->ID, profundidade);
    profundidade++;
    for(i = 0; i < no->numeroAdjacentes; i++)
    {   
        barraAdj = no->adjacentes[i].idNo;
        if ((visitado[barraAdj]== false) && no->adjacentes[i].estado == 1)
            {
                barraAtual->idAdj = i;
                nos_alim[0]++; 
                grafo[barraAdj].Vbase = grafo[idNo].Vbase; //Atualiza base de tensão conforme varredura
                grafo[barraAdj].fases = no->adjacentes[i].ramo->fases; //Atualiza base de tensão conforme varredura
                
                grafo[barraAdj].distanciaSE_acc = grafo[idNo].distanciaSE_acc  + no->adjacentes[i].ramo->comprimento;
                idNo= barraAdj;
                //adicionaNo(&barraAtual, idNo);
                adicionaNoNaFila(&barraAtual, idNo);
                apontaProxNoNaFila(&barraAtual);
                buscaProfundidade(barraAtual, idNo, profundidade, visitado, grafo, idAlim,nos_alim);
            } 
    }
}


/**
 * @brief Função para realizar busca em largura no grafo da rede elétrica
 *
 * Essa função realiza uma busca em largura no grafo da rede elétrica @p grafo para determinado alimentador especifico @p idAlim da estrutura 
 * @p alimentador atualizando a profundidade de cada nó para percorrer o grafo  da rede conforme a conectivivdade da rede elétrica. Utiliza 
 * um vetor auxiliar para controlar o nós já visitados na busca @p visitado e parte do nó inicial de cada alimentador defindo por @p idNoRaiz.
 * A função retorna @c void .
 * 
 * 
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param alimentador estrutura de dados dos alimentadores
 * @param idAlim identificador do alimentador onde está sendo realizada a busca em largura
 * @param idNoRaiz nó inicial para iniciar a busca em largura
 * @param visitado vetor auxiliar para controlar os nós já visitados pela busca
 * @return void
 * @see 
 * @note Implementada para testes e validações, mas o código utiliza a busca em profundidade para obter a representação via RNP.
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void buscaLargura(TF_GRAFO * grafo, TF_ALIMENTADOR *alimentador, long int idAlim, long int idNoRaiz, BOOL * visitado) {
    long int i, barraAdj, idNo,profundidade = 0;    
    
    TF_FILABARRAS *barraAtual = NULL;
    TF_FILABARRAS *filaProf = NULL;
    TF_FILABARRAS *filaBarras = NULL;
    
    //adicionaNo(&alimentador[idAlim].rnp, idNoRaiz);
    barraAtual = &alimentador[idAlim].rnp[0];
    adicionaNoNaFila(&filaBarras, idNoRaiz);
    adicionaNoNaFila(&filaProf, profundidade);
    
    while(filaNaoVazia(filaBarras))
    {
        idNo = retiraNoDaFila(&filaBarras);
        if (idNo != idNoRaiz){
            adicionaNo(&barraAtual, idNo);
        }
        profundidade = retiraNoDaFila(&filaProf);
        barraAtual->profundidade = profundidade;
        TF_GRAFO * no = &grafo[idNo];
        grafo[idNo].idAlim = idAlim;
        for(i = 0; i < no->numeroAdjacentes; i++)
        {
            barraAdj = no->adjacentes[i].idNo;
            if(ramoLigado(no->adjacentes[i]))
            {
                if(!visitado[barraAdj])
                {
                    /*if(estaLista(barraAtual, barraAdj))
                        printf("CICLO barra %ld idAlim %ld\n", barraAdj, idAlim);
                    else
                    {*/


                    adicionaNoNaFila(&filaBarras, barraAdj);
                    adicionaNoNaFila(&filaProf, profundidade + 1);
                    alimentador[idAlim].numeroNos++;

                    //}
                }
            }
        }
        visitado[idNo] = true;        
    }   
}

/**
 * @brief Função principal para realizar busca em profundidade nos alimentadores da rede elétrica
 *
 * Essa função realiza uma busca em profundidade em todos os alimentadores @p alimentadores  no grafo da rede elétrica @p grafo, através da função 
 * recursiva buscaProfundidade(). Receb como parâmetros a quantidade total de barras @p numeroBarras e de alimentadores na rede elétrica @p numeroAlimentadores .
 * A função retorna @c void .
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * char *folder = NULL;
 * DBAR *barraExemplo = NULL;
 * TF_DRAM *ramoExemplo = NULL; 
 * TF_GRAFO *grafoExemplo = NULL;
 * TF_ALIMENTADOR *alimentadorExemplo = NULL;
 * 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * geraGrafo(&grafoExemplo, barraExemplo, numeroBarras,ramoExemplo,numeroRamos);
 * buscaProfundidadeAlimentadores(grafoExemplo, numeroBarras, &alimentadorExemplo, numeroAlimentadores); 
 * @endcode
 * 
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param numeroBarras quantidade total de barras
 * @param alimentador estrutura de dados dos alimentadores
 * @param numeroAlimentadores quantidade total de alimentadores
 * @return void
 * @see buscaProfundidade
 * @note 
 * @warning 
 */
void buscaProfundidadeAlimentadores(TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR **alimentadores, long int numeroAlimentadores) {
    int i, idAlim = 0;
    TF_FILABARRAS * lista_barras = NULL;
    TF_FILABARRAS *barraAtual = NULL;
    FILE *arquivo;
    long int numero_nos;
    
    arquivo = fopen("lista_alimentador.dad","w+");
    BOOL visitado[numeroBarras];
    
    if (((*alimentadores)= (TF_ALIMENTADOR *)malloc( numeroAlimentadores * sizeof(TF_ALIMENTADOR)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para alimentadores !!!!");
        exit(1); 
    }
    
    for(i=0; i<numeroBarras; i++){ 
        visitado[i] = false;
        if(grafo[i].tipo == 2){
            (*alimentadores)[idAlim].idAlim = idAlim;
            (*alimentadores)[idAlim].noRaiz = i;
            (*alimentadores)[idAlim].numeroNos = 1;
            (*alimentadores)[idAlim].idRaiz = grafo[i].barra->ID;
            (*alimentadores)[idAlim].rnp[0].idNo = i;
            (*alimentadores)[idAlim].rnp[0].profundidade = 0;
            (*alimentadores)[idAlim].rnp[0].prox = NULL;
            idAlim++;
        }
    }
    
    //Atribui profundidade alimentadores 
    for(i=0; i<numeroAlimentadores; i++)
    {
        //buscaLargura(grafo, (*alimentadores), i, (*alimentadores)[i].noRaiz, visitado);
        barraAtual = &(*alimentadores)[i].rnp[0];
        numero_nos = 1;
        buscaProfundidade(barraAtual,(*alimentadores)[i].noRaiz,0,visitado,grafo,i,&numero_nos);
        (*alimentadores)[i].numeroNos = numero_nos;
//         TF_FILABARRAS *barraAtual = &(*alimentadores)[i].rnp[0];
//         while(barraAtual != NULL){
//             grafo[barraAtual->idNo].Vbase = grafo[(*alimentadores)[i].noRaiz].Vbase;
//                if (barraAtual->idNo == 223406)
//                     printf("\nAlimentador encontrado: %d\n",i);
//             barraAtual = barraAtual->prox;
//        }
//        printf("\n alimentador %d Raiz: %d   Nos: %d  %.1lf kV",i,(*alimentadores)[i].idRaiz,(*alimentadores)[i].numeroNos,grafo[(*alimentadores)[i].noRaiz].Vbase*sqrt(3)/1000);
//        if (grafo[(*alimentadores)[i].noRaiz].Vbase*sqrt(3)/1000 == 34.5)
        //   printf("\n alimentador %d Raiz: %d   Nos: %d  %.1lf kV",i,(*alimentadores)[i].idRaiz,(*alimentadores)[i].numeroNos,grafo[(*alimentadores)[i].noRaiz].Vbase*sqrt(3)/1000);  
           
    }
    
    
//    //    Atualiza a tensão base de acordo com a busca em profundidade
//    arquivo = fopen("lista_barra_alimentador.dad","w+");
//    
//    for(i=0; i<numeroAlimentadores; i++)
//    {
//        TF_FILABARRAS *barraAtual = &(*alimentadores)[i].rnp[0];
//        double Vbase = grafo[barraAtual->idNo].Vbase;
//        while(barraAtual != NULL)
//        {
//            grafo[barraAtual->idNo].Vbase = Vbase;
////            if(barraAtual->idNo == 1864) {
////                printf("\nAlim: %d\n", i);
////                printf("\nAlim: %d\n", i);
////                getchar();
////            }
//            fprintf(arquivo,"\n%ld  \t\t\t%d",grafo[barraAtual->idNo].barra->ID, i);
//            barraAtual = barraAtual->prox;          
//            
//        }
//        
//    }
//    
//    fclose(arquivo);
    
    
    
//     imprimeAlimentador(arquivo,grafo, numeroBarras, (*alimentadores), 254);
    // printf("\n");
    // imprimeAlimentador(arquivo,grafo, numeroBarras, (*alimentadores), 90);
     fclose(arquivo);
    arquivo = fopen("lista_desconexas.dad","w+");
    //Imprime Barras desconexas - que não estão associadas a nenhum alimentador
    for(i=0; i<numeroBarras; i++){ 
        if (visitado[i] == false){
            fprintf(arquivo,"%d\n",grafo[i].idNo);
        }
    }
    fclose(arquivo);
    
    arquivo = fopen("lista_34_5.dad","w+");
    //Imprime Barras do nível de 34.5
    for(i=0; i<numeroAlimentadores; i++)
    {
        if (grafo[(*alimentadores)[i].noRaiz].Vbase*sqrt(3)/1000 == 34.5){
            TF_FILABARRAS *barraAtual = &(*alimentadores)[i].rnp[0];
            while(barraAtual != NULL)
            {
                fprintf(arquivo,"\n%ld",grafo[barraAtual->idNo].barra->ID);
                barraAtual = barraAtual->prox;
            }
        }
        
    }    
    fclose(arquivo);
    
    arquivo = fopen("lista_SEs.dad","w+");
    //Imprime Barras do nível de 34.5
    for(i=0; i<numeroAlimentadores; i++)
    {
        fprintf(arquivo,"\n%ld",grafo[(*alimentadores)[i].noRaiz].barra->ID);
    }    
    fclose(arquivo);
    
    printf("\n");
}

// Gera estrutura de dados do grafo que representa a rede elétricas
/**
 * @brief Função principal que cria estrutura de dados para representar o grafo da rede elétrica e armazenar as condições topológicas e operacionais 
 * da rede elétrica. 
 * 
 * Esta função aloca e cria a estrutura de dados para representar a rede elétrica e respectivas informações estruturais e paramétricas dos componentes
 * da rede elerica. Recebe como parâmetros de entrada as barras @p barras com informações do nós, cargas, bancos de capacitores e geradores 
 * distribuídos que compõem os nós do grafo da rede elétrica, e os ramos @p ramos com informações de circuitos, transformadores, reguladores de 
 * tensão e chaves que compõem os ramos do grafo da rede eletrica, definindo assim a conectividade e adjacência do grafo. Recebe como parâmetros 
 * de entrada e saída a estrutura de dados do tipo TF_GRAFO @p grafo com todas as informações a respeito da rede elétrica a aloca os nós e adjacências 
 * do grafo com estrutura interna do tipo TF_NOADJACENTE que apontam para as barras e ramos inicialmente definidos.
 * A função retorna @c void .
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * char *folder = NULL;
 * DBAR *barraExemplo = NULL;
 * TF_DRAM *ramoExemplo = NULL; 
 * TF_GRAFO *grafoExemplo = NULL;
 * 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * geraGrafo(&grafoExemplo, barraExemplo, numeroBarras,ramoExemplo,numeroRamos); 
 * @endcode
 *  
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param barras estrutura de dados com os dados das barras da rede elétrica
 * @param numeroBarras quantidade total de barras
 * @param ramos estrutura de dados com os dados dos ramos da rede elétrica
 * @param numeroRamos quantidade total de ramos
 * @return void
 * @see 
 * @note 
 * @warning
 */
void geraGrafo(TF_GRAFO ** grafo, TF_DBAR *barras, long int numeroBarras,TF_DRAM *ramos,long int numeroRamos){
    long int i,j,k;
    long int barraDe, barraPara,nadj;
    // TF_NOADJACENTE no_vazio = {.idNO = 0, .estado = 0, .tipo = 3, .relacao = 1, .ramo = NULL, .idram = 0, .nmed = 0, .medidores = NULL, .S[0] = 0, .S[1] = 0, .S[2] = 0, .Cur[0] = 0, .Cur[1] = 0, .Cur[2] = 0, .losses = 0, .loading = 0 };
    
    if (((*grafo) = (TF_GRAFO *)malloc( (numeroBarras) * sizeof(TF_GRAFO)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as barras !!!!");
        exit(1); 
    }
    
    for(i=0;i<numeroBarras;i++){
        (*grafo)[i].idNo = i;//barras[i].ID;
        (*grafo)[i].tipo = barras[i].tipo;
        (*grafo)[i].fases = barras[i].fases;
        (*grafo)[i].Vbase = barras[i].Vbase;
        (*grafo)[i].distanciaSE_acc = 0;
        (*grafo)[i].barra = &barras[i];
        (*grafo)[i].nmed = 0;
        (*grafo)[i].nmedPQ = 0;
        (*grafo)[i].numeroAdjacentes = 0;

        for(j=0;j<3;j++) (*grafo)[i].V[j] = 0;
        for(j=0;j<3;j++) (*grafo)[i].S[j] = 0;
        for(j=0;j<3;j++) (*grafo)[i].Cur[j] = 0;
        for(j=0;j<3;j++) (*grafo)[i].V_aux[j] = 0;
        
        (*grafo)[i].medidores = NULL;
        (*grafo)[i].medidores = (TF_DMED**)malloc(30 * sizeof(TF_DMED));
        for (j=0;j<30;j++)(*grafo)[i].medidores[j] = NULL;
        for(j=0;j<3;j++){
            for(k=0;k<3;k++){
                (*grafo)[i].Ysh[j][k] = 0;
            }
        }
    }
    
    //Cria dados de adjacência de barras
    for(i=0;i<numeroRamos;i++){
        barraDe = ramos[i].k;
        barraPara = ramos[i].m;
        nadj = (*grafo)[barraDe].numeroAdjacentes;
        
        (*grafo)[barraDe].adjacentes[nadj].ramo = &ramos[i];
        (*grafo)[barraDe].adjacentes[nadj].idNo = barraPara;
        (*grafo)[barraDe].adjacentes[nadj].tipo = ramos[i].tipo;
        (*grafo)[barraDe].adjacentes[nadj].estado = ramos[i].estado;
        (*grafo)[barraDe].adjacentes[nadj].nmed = 0;
        (*grafo)[barraDe].adjacentes[nadj].losses = 0;
        (*grafo)[barraDe].adjacentes[nadj].loading = 0;
        (*grafo)[barraDe].adjacentes[nadj].idram = i;
        for(j=0;j<3;j++) (*grafo)[barraDe].adjacentes[nadj].S[j] = 0;
        for(j=0;j<3;j++) (*grafo)[barraDe].adjacentes[nadj].Cur[j] = 0;

        (*grafo)[barraDe].numeroAdjacentes++;
        if(ramos[i].tipo == 1){
            (*grafo)[barraDe].adjacentes[nadj].relacao = ramos[i].trafo.Vsec/ramos[i].trafo.Vpri;
        }
        (*grafo)[barraDe].adjacentes[nadj].medidores = NULL;
        (*grafo)[barraDe].adjacentes[nadj].medidores = (TF_DMED**)malloc(30 * sizeof(TF_DMED*));
        for (j=0;j<30;j++) (*grafo)[barraDe].adjacentes[nadj].medidores[j]  = NULL;
        
        nadj = (*grafo)[barraPara].numeroAdjacentes;
        (*grafo)[barraPara].adjacentes[nadj].ramo = &ramos[i];
        (*grafo)[barraPara].adjacentes[nadj].idNo = barraDe;
        (*grafo)[barraPara].adjacentes[nadj].tipo = ramos[i].tipo;
        (*grafo)[barraPara].adjacentes[nadj].estado = ramos[i].estado;
        (*grafo)[barraPara].adjacentes[nadj].nmed = 0;
        (*grafo)[barraPara].adjacentes[nadj].losses = 0;
        (*grafo)[barraPara].adjacentes[nadj].loading = 0;
        (*grafo)[barraPara].adjacentes[nadj].idram = i;
        for(j=0;j<3;j++) (*grafo)[barraPara].adjacentes[nadj].S[j] = 0;
        for(j=0;j<3;j++) (*grafo)[barraPara].adjacentes[nadj].Cur[j] = 0;

        (*grafo)[barraPara].numeroAdjacentes++;
        if(ramos[i].tipo == 1){
            (*grafo)[barraPara].adjacentes[nadj].relacao = ramos[i].trafo.Vpri/ramos[i].trafo.Vsec;
        }
        (*grafo)[barraPara].adjacentes[nadj].medidores = NULL;
        (*grafo)[barraPara].adjacentes[nadj].medidores = (TF_DMED**)malloc(30 * sizeof(TF_DMED*));
        for (j=0;j<30;j++) (*grafo)[barraPara].adjacentes[nadj].medidores[j]  = NULL;
    }
}

/**
 * @brief Função auxiliar para salvar topologia de alimentador obtida pela busca em profundidade em arquivo externo
 *
 * Essa função salva um arquivo de texto indicado em @p arquivo contendo as principais da topologia de um alimentador específico obtida 
 * pela busca em profundidade. Recebe como parâmetro de entrada o o grafo da rede elétrica @p grafo e número total de barras @p numeroBarras 
 * de um determinado alimentador @p idAlim da estrutura de dados de alimentadores @p alimentador.
 * A função retorna @c void.
 * 
 * @param arquivo ponteiro para arquivo que será impressa a topologia do alimentador
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param numeroBarras quantidade total de barras
 * @param alimentador estrutura de dados dos alimentadores
 * @param idAlim identificador do alimentador cuja topologia será impressa no arquivo
 * @return void
 * @see
 * @note Utilizada somente para validação e conferência das informações
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void imprimeAlimentador(FILE *arquivo,TF_GRAFO *grafo, long int numeroBarras, TF_ALIMENTADOR *alimentadores, long int idAlim){
    int i;
    
    // printf("\n alimentador %d Raiz: %d   Nos: %d  %.1lf kV",idAlim,alimentadores[idAlim].idRaiz,alimentadores[idAlim].numeroNos,grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000);
    
    TF_FILABARRAS *barraAtual = &alimentadores[idAlim].rnp[0];
    while(barraAtual != NULL)
    {
    //    fprintf(arquivo,"\nBarra[%d]: %d  Prof: %d  Vbase = %.1lf",barraAtual->idNo,grafo[barraAtual->idNo].barra->ID,barraAtual->profundidade,grafo[barraAtual->idNo].Vbase*sqrt(3)/1000);
        fprintf(arquivo,"\n%d",grafo[barraAtual->idNo].barra->ID);
        
        barraAtual = barraAtual->prox;
    }
       
    
}



//------------------------------------------------------------------------------
//
// ROTINAS DE TRATAMENTO DOS DADOS ELÉTRICOS
//
//------------------------------------------------------------------------------

//Converte a matriz de impedância em admitância e monta quadripolo dos ramos do grafo
/**
 * @brief Função auxiliar para montar o quadripólo de admitância trifásico de circuitos (ramais ou linhas de distribuição)
 *
 * Essa função monta o quadripólo de admitâncias trifásicas matricial para ramos da rede elétrica cujos componentes são circuitos trifásicos.
 * Esta função recebe como parâmetro de entrada e saída uma estrutura do tipo TF_DRAM @p ramo que armazena as informações matriciais do quadripólo de
 * admitâncias de um ramo respectivo, e como parâmetro de entrada os dados detalhados do circuito trifásico @p linha com o parâmetros deste componente.
 * A função retorna @c void .
 * 
 * @param *ramo ponteiro o ramo que será montado o quadripólo
 * @param *linha ponteiro para as informações detalhadas do circuito trifásico (ramais ou linhas de distribuição)
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void montaQuadripoloLinha(TF_DRAM *ramo, TF_DLIN *linha){
    int aux = 1;
    __complex__ double y, **Zl,**B;
    
    //Aloca Matrizes de Quadripolos
    ramo->Ypp = c_matAloca(3);
    ramo->Yps = c_matAloca(3);
    ramo->Ysp = c_matAloca(3);
    ramo->Yss = c_matAloca(3);
    
    //Aloca Matrizes de Quadripolos
    ramo->Z = c_matAloca(3);
    ramo->B = c_matAloca(3);
    
    //Aloca Matrizes de Impedância e Admitância
    Zl = c_matAloca(3);
    B = c_matAloca(3);
    
    //Matriz Impedância da linha
    Zl[0][0] = linha->Zaa;
    Zl[0][1] = linha->Zab;
    Zl[0][2] = linha->Zac;
    Zl[1][0] = linha->Zab;
    Zl[1][1] = linha->Zbb;
    Zl[1][2] = linha->Zbc;
    Zl[2][0] = linha->Zac;
    Zl[2][1] = linha->Zbc;
    Zl[2][2] = linha->Zcc;
    
    //Matriz Susceptãncia Shunt da linha
    B[0][0] = I * linha->Baa/2;
    B[0][1] = I * linha->Bab/2;
    B[0][2] = I * linha->Bac/2;
    B[1][0] = I * linha->Bab/2;
    B[1][1] = I * linha->Bbb/2;
    B[1][2] = I * linha->Bbc/2;
    B[2][0] = I * linha->Bac/2;
    B[2][1] = I * linha->Bbc/2;
    B[2][2] = I * linha->Bcc/2;
    
    //Matriz de impedância
    c_matIgual(ramo->Z, Zl, 3);
    c_matIgual(ramo->B, B, 3);
    
    //Inversa de Z - salva na variável Zl
    c_matInversaZ(Zl, 3);
    
    c_matIgual(ramo->Ypp, Zl, 3);
    c_matIgual(ramo->Yss, Zl, 3);
    c_matMultEsc(Zl, -1, 3);
    c_matIgual(ramo->Yps, Zl, 3);
    c_matIgual(ramo->Ysp, Zl, 3);
    
    
    ramo->Ypp[0][0] = ramo->Ypp[0][0] + B[0][0];
    ramo->Ypp[0][1] = ramo->Ypp[0][1] + B[0][1];
    ramo->Ypp[0][2] = ramo->Ypp[0][2] + B[0][2];
    ramo->Ypp[1][0] = ramo->Ypp[1][0] + B[1][0];
    ramo->Ypp[1][1] = ramo->Ypp[1][1] + B[1][1];
    ramo->Ypp[1][2] = ramo->Ypp[1][2] + B[1][2];
    ramo->Ypp[2][0] = ramo->Ypp[2][0] + B[2][0];
    ramo->Ypp[2][1] = ramo->Ypp[2][1] + B[2][1];
    ramo->Ypp[2][2] = ramo->Ypp[2][2] + B[2][2];
    
    ramo->Yss[0][0] = ramo->Yss[0][0] + B[0][0];
    ramo->Yss[0][1] = ramo->Yss[0][1] + B[0][1];
    ramo->Yss[0][2] = ramo->Yss[0][2] + B[0][2];
    ramo->Yss[1][0] = ramo->Yss[1][0] + B[1][0];
    ramo->Yss[1][1] = ramo->Yss[1][1] + B[1][1];
    ramo->Yss[1][2] = ramo->Yss[1][2] + B[1][2];
    ramo->Yss[2][0] = ramo->Yss[2][0] + B[2][0];
    ramo->Yss[2][1] = ramo->Yss[2][1] + B[2][1];
    ramo->Yss[2][2] = ramo->Yss[2][2] + B[2][2];
    
//    printf("\n\Quadripolo barra para %d\n\n", noadj->idNo);
//    c_matImprime(ramo->Ypp,3);
//    printf("\n");
//    c_matImprime(ramo->Yps,3);
//    printf("\n");
//    c_matImprime(ramo->Ysp,3);
//    printf("\n");
//    c_matImprime(ramo->Yss,3);
//    printf("\n");
    for (int i=0;i<3;i++){
        free(Zl[i]);
        free(B[i]);
    }
    free(Zl);
    free(B);
    
}

//Converte a matriz de impedância em admitância e monta quadripolo dos ramos do grafo
/**
 * @brief Função auxiliar para montar o quadripólo de admitância trifásico de transformadores de potência
 * 
 * Essa função monta o quadripólo de admitâncias trifásicas matricial para ramos da rede elétrica cujos componentes são transformadores de potência.
 * Esta função recebe como parâmetro de entrada e saída uma estrutura do tipo TF_DRAM @p ramo que armazena as informações matriciais do quadripólo de
 * admitâncias de um ramo respectivo, e como parâmetro de entrada os dados detalhados do transformador @p trafo com o parâmetros deste componente.
 * A função retorna @c void .
 * 
 * @param *ramo ponteiro o ramo que será montado o quadripólo
 * @param *trafo ponteiro para as informações detalhadas do transformador de potência
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void montaQuadripoloTrafo(TF_DRAM *ramo, TF_DTRF *trafo){
    int aux = 1;
    __complex__ double y, **Yi, **Yii,**Yiii, **Yiiii;
    __complex__ double y_mutua, y_shunt;
    
    //Aloca Matrizes de Quadripolos
    ramo->Ypp = c_matAloca(3);
    ramo->Yps = c_matAloca(3);
    ramo->Ysp = c_matAloca(3);
    ramo->Yss = c_matAloca(3);
    
    //Aloca Matrizes de Ligação D-Y-YN
    Yi = c_matAloca(3);
    Yii = c_matAloca(3);
    Yiii = c_matAloca(3);
    Yiiii = c_matAloca(3);
    
    //Admitância do trafo
    y = 1/(trafo->R + I*trafo->X);
    y_shunt = 1/(trafo->noLoadLosses + I*trafo->p_imag);; //impedância de magnetização e perdas em vazio
    
    //Yi
    Yi[0][0] = 1;
    Yi[0][1] = 0;
    Yi[0][2] = 0;
    Yi[1][0] = 0;
    Yi[1][1] = 1;
    Yi[1][2] = 0;
    Yi[2][0] = 0;
    Yi[2][1] = 0;
    Yi[2][2] = 1;
    //Yii
    Yii[0][0] = 2;
    Yii[0][1] = -1;
    Yii[0][2] = -1;
    Yii[1][0] = -1;
    Yii[1][1] = 2;
    Yii[1][2] = -1;
    Yii[2][0] = -1;
    Yii[2][1] = -1;
    Yii[2][2] = 2;
    //Yiii
    Yiii[0][0] = -1/(pow(3,0.5));
    Yiii[0][1] = 1/(pow(3,0.5));
    Yiii[0][2] = 0;
    Yiii[1][0] = 0;
    Yiii[1][1] = -1/(pow(3,0.5));
    Yiii[1][2] = 1/(pow(3,0.5));
    Yiii[2][0] = 1/(pow(3,0.5));
    Yiii[2][1] = 0;
    Yiii[2][2] = -1/(pow(3,0.5));
    
    //Yiiii - Auxilia nas ligações abertas
    Yiiii[0][0] = 1;
    Yiiii[0][1] = -1;
    Yiiii[0][2] = -1;
    Yiiii[1][0] = -1;
    Yiiii[1][1] = 1;
    Yiiii[1][2] = -1;
    Yiiii[2][0] = -1;
    Yiiii[2][1] = -1;
    Yiiii[2][2] = 1;

    c_matMultEsc(Yi, y, 3);
    c_matMultEsc(Yii, y/3, 3);
    c_matMultEsc(Yiii, y, 3);
    c_matMultEsc(Yiiii, y, 3);
    if (trafo->fases == ABC){
        if((trafo->lig_pri == 1)&& (trafo->lig_sec == 1)){ //Ligação YN-YN
            c_matIgual(ramo->Ypp, Yi, 3);
            c_matIgual(ramo->Yps, Yi, 3);
            c_matMultEsc(ramo->Yps, -1, 3);
            c_matIgual(ramo->Ysp, Yi, 3);
            c_matMultEsc(ramo->Ysp, -1, 3);
            c_matIgual(ramo->Yss, Yi, 3);
        }
        else if((trafo->lig_pri == 1)&& (trafo->lig_sec == 3)){ //Ligação YN-Y
            c_matIgual(ramo->Ypp, Yii, 3);
            c_matIgual(ramo->Yps, Yii, 3);
            c_matMultEsc(ramo->Yps, -1, 3);
            c_matIgual(ramo->Ysp, Yii, 3);
            c_matMultEsc(ramo->Ysp, -1, 3);
            c_matIgual(ramo->Yss, Yii, 3);
            ramo->Ypp[2][0] = ramo->Ypp[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            ramo->Ypp[2][1] = ramo->Ypp[2][1] + 100*cabs(y);
            ramo->Ypp[2][2] = ramo->Ypp[2][2] + 100*cabs(y);
        }
        else if((trafo->lig_pri == 1)&& (trafo->lig_sec == 2)){ //Ligação YN-D
            c_matIgual(ramo->Ypp, Yi, 3);
            c_matIgual(ramo->Yps, Yiii, 3);
            c_matIgual(ramo->Ysp, Yiii, 3);
            c_matTransp(ramo->Ysp, 3);
            c_matIgual(ramo->Yss, Yii, 3);
        }
        else if((trafo->lig_pri == 3)&& (trafo->lig_sec == 1)){ //Ligação Y-YN
            c_matIgual(ramo->Ypp, Yii, 3);
            c_matIgual(ramo->Yps, Yii, 3);
            c_matMultEsc(ramo->Yps, -1, 3);
            c_matIgual(ramo->Ysp, Yii, 3);
            c_matMultEsc(ramo->Ysp, -1, 3);
            c_matIgual(ramo->Yss, Yii, 3);
            ramo->Yss[2][0] = ramo->Yss[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            ramo->Yss[2][1] = ramo->Yss[2][1] + 100*cabs(y);
            ramo->Yss[2][2] = ramo->Yss[2][2] + 100*cabs(y);
        }
        else if((trafo->lig_pri == 3)&& (trafo->lig_sec == 3)){ //Ligação Y-Y
            c_matIgual(ramo->Ypp, Yii, 3);
            c_matIgual(ramo->Yps, Yii, 3);
            c_matMultEsc(ramo->Yps, -1, 3);
            c_matIgual(ramo->Ysp, Yii, 3);
            c_matMultEsc(ramo->Ysp, -1, 3);
            c_matIgual(ramo->Yss, Yii, 3);
            ramo->Yss[2][0] = ramo->Yss[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            ramo->Yss[2][1] = ramo->Yss[2][1] + 100*cabs(y); //Pensar se trata-se de simplificação, pois pode existir tensão de neutro neste caso, mas não irá ter corrente de neutro.
            ramo->Yss[2][2] = ramo->Yss[2][2] + 100*cabs(y);
        }
        else if((trafo->lig_pri == 3)&& (trafo->lig_sec == 2)){ //Ligação Y-D
            c_matIgual(ramo->Ypp, Yii, 3);
            c_matIgual(ramo->Yps, Yiii, 3);
            c_matIgual(ramo->Ysp, Yiii, 3);
            c_matTransp(ramo->Ysp, 3);
            c_matIgual(ramo->Yss, Yii, 3);
        }
        else if((trafo->lig_pri == 2)&& (trafo->lig_sec == 1)){ //Ligação D-YN
            c_matIgual(ramo->Ypp, Yii, 3);
            c_matIgual(ramo->Yps, Yiii, 3);
            c_matIgual(ramo->Ysp, Yiii, 3);
            c_matTransp(ramo->Ysp, 3);
            c_matIgual(ramo->Yss, Yi, 3);
            ramo->Ypp[2][0] = ramo->Ypp[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            ramo->Ypp[2][1] = ramo->Ypp[2][1] + 100*cabs(y);
            ramo->Ypp[2][2] = ramo->Ypp[2][2] + 100*cabs(y);
        }
        else if((trafo->lig_pri == 2)&& (trafo->lig_sec == 3)){ //Ligação D-Y
            c_matIgual(ramo->Ypp, Yii, 3);
            c_matIgual(ramo->Yps, Yiii, 3);
            c_matTransp(ramo->Yps, 3);
            c_matIgual(ramo->Ysp, Yiii, 3);
            c_matIgual(ramo->Yss, Yii, 3);
            ramo->Ypp[2][0] = ramo->Ypp[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            ramo->Ypp[2][1] = ramo->Ypp[2][1] + 100*cabs(y);
            ramo->Ypp[2][2] = ramo->Ypp[2][2] + 100*cabs(y);
            
            // Aproximação para o caso do Y não aterrado - tensão de sequência é igual a zero no secundário - fica igual DYn
            ramo->Yps[2][0] = ramo->Yps[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            ramo->Yps[2][1] = ramo->Yps[2][1] + 100*cabs(y);
            ramo->Yps[2][2] = ramo->Yps[2][2] + 100*cabs(y);
        }
        else if((trafo->lig_pri == 2)&& (trafo->lig_sec == 2)){ //Ligação D-D
            c_matIgual(ramo->Ypp, Yii, 3);
            c_matIgual(ramo->Yps, Yii, 3);
            c_matMultEsc(ramo->Yps, -1, 3);
            c_matIgual(ramo->Ysp, Yii, 3);
            c_matMultEsc(ramo->Ysp, -1, 3);
            c_matIgual(ramo->Yss, Yii, 3);
            ramo->Yss[2][0] = ramo->Yss[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            ramo->Yss[2][1] = ramo->Yss[2][1] + 100*cabs(y);
            ramo->Yss[2][2] = ramo->Yss[2][2] + 100*cabs(y);        
        }
    }
    else if (trafo->fases == A){ //Transformador monofásico na fase A
        if((trafo->lig_pri == 1)&& (trafo->lig_sec == 1)){ //Ligação FN - FN
            ramo->Ypp[0][0] = y + y_shunt;
            
            ramo->Yps[0][0] = -y;
            
            ramo->Ysp[0][0] = -y;
            
            ramo->Yss[0][0] = y;        
        }
        else if((trafo->lig_pri == 2)&& (trafo->lig_sec == 1)){ //Ligação FF - FN - de AB para AN
            ramo->Ypp[0][0] = 2/3*(y + y_shunt);
            ramo->Ypp[0][1] = -1/3*(y + y_shunt);
            ramo->Ypp[1][0] = -1/3*(y + y_shunt);
            ramo->Ypp[1][1] = 2/3*(y + y_shunt);
            
            ramo->Yps[0][0] = -y/(pow(3,0.5));
            ramo->Yps[1][0] = -y/(pow(3,0.5));
            
            ramo->Ysp[0][0] = -y/(pow(3,0.5));
            ramo->Ysp[0][1] = -y/(pow(3,0.5));

            ramo->Yss[0][0] = y;
        }
        else if((trafo->lig_pri == 2)&& (trafo->lig_sec == 2)){ //Ligação FF - FF - de AB para AB
            ramo->Ypp[0][0] = 2/3*(y + y_shunt);
            ramo->Ypp[0][1] = -1/3*(y + y_shunt);
            ramo->Ypp[1][0] = -1/3*(y + y_shunt);
            ramo->Ypp[1][1] = 2/3*(y + y_shunt);
            
            ramo->Yps[0][0] = -2/3*(y);
            ramo->Yps[0][1] = 1/3*(y);
            ramo->Yps[1][0] = 1/3*(y);
            ramo->Yps[1][1] = -2/3*(y);
            
            ramo->Ysp[0][0] = -2/3*(y);
            ramo->Ysp[0][1] = 1/3*(y);
            ramo->Ysp[1][0] = 1/3*(y);
            ramo->Ysp[1][1] = -2/3*(y);

            ramo->Yss[0][0] = 2/3*(y);
            ramo->Yss[0][1] = -1/3*(y);
            ramo->Yss[1][0] = -1/3*(y);
            ramo->Yss[1][1] = 2/3*(y);
        }
        else if((trafo->lig_pri == 1)&& (trafo->lig_sec == 6)){ //Ligação com Tap Central no Secundário
            y_mutua = 1/(trafo->r_mutua + I*trafo->x_mutua);
            
            ramo->Ypp[0][0] = 2*y + y_shunt;
            
            ramo->Yps[0][0] = -y;
            ramo->Yps[0][1] = -y;
            
            ramo->Ysp[0][0] = -y;
            ramo->Ysp[1][0] = -y;

            ramo->Yss[0][0] = y + y_mutua;
            ramo->Yss[0][1] = -y_mutua;
            ramo->Yss[1][0] = -y_mutua;
            ramo->Yss[1][1] = y + y_mutua;
        }
    
    }
    else if (trafo->fases == B){ //Transformador monofásico na fase B
        if((trafo->lig_pri == 1)&& (trafo->lig_sec == 1)){ //Ligação Direto ao Terra
            ramo->Ypp[1][1] = y + y_shunt;
            
            ramo->Yps[1][1] = -y;
            
            ramo->Ysp[1][1] = -y;
            
            ramo->Yss[1][1] = y;        
        }
        else if((trafo->lig_pri == 2)&& (trafo->lig_sec == 1)){ //Ligação FF - FN - de BC para BN
            ramo->Ypp[1][1] = 2/3*(y + y_shunt);
            ramo->Ypp[1][2] = -1/3*(y + y_shunt);
            ramo->Ypp[2][1] = -1/3*(y + y_shunt);
            ramo->Ypp[2][2] = 2/3*(y + y_shunt);
            
            ramo->Yps[1][1] = -y/(pow(3,0.5));
            ramo->Yps[2][1] = -y/(pow(3,0.5));
            
            ramo->Ysp[1][1] = -y/(pow(3,0.5));
            ramo->Ysp[1][2] = -y/(pow(3,0.5));

            ramo->Yss[1][1] = y;
        }
        else if((trafo->lig_pri == 2)&& (trafo->lig_sec == 2)){ //Ligação FF - FF - de BC para BC
            ramo->Ypp[1][1] = 2/3*(y + y_shunt);
            ramo->Ypp[1][2] = -1/3*(y + y_shunt);
            ramo->Ypp[2][1] = -1/3*(y + y_shunt);
            ramo->Ypp[2][2] = 2/3*(y + y_shunt);
            
            ramo->Yps[1][1] = -2/3*(y);
            ramo->Yps[1][2] = 1/3*(y);
            ramo->Yps[2][1] = 1/3*(y);
            ramo->Yps[2][2] = -2/3*(y);
            
            ramo->Ysp[1][1] = -2/3*(y);
            ramo->Ysp[1][2] = 1/3*(y);
            ramo->Ysp[2][1] = 1/3*(y);
            ramo->Ysp[2][2] = -2/3*(y);

            ramo->Yss[1][1] = 2/3*(y);
            ramo->Yss[1][2] = -1/3*(y);
            ramo->Yss[2][1] = -1/3*(y);
            ramo->Yss[2][2] = 2/3*(y);
        }
        else if((trafo->lig_pri == 1)&& (trafo->lig_sec == 6)){ //Ligação com Tap Central no Secundário
            y_mutua = 1/(trafo->r_mutua + I*trafo->x_mutua);
            
            ramo->Ypp[1][1] = 2*y + y_shunt;
            
            ramo->Yps[1][0] = -y;
            ramo->Yps[1][1] = -y;
            
            ramo->Ysp[0][1] = -y;
            ramo->Ysp[1][1] = -y;

            ramo->Yss[0][0] = y + y_mutua;
            ramo->Yss[0][1] = -y_mutua;
            ramo->Yss[1][0] = -y_mutua;
            ramo->Yss[1][1] = y + y_mutua;
        }
    }
    else if (trafo->fases == C){ //Transformador monofásico na fase C
        if((trafo->lig_pri == 1)&& (trafo->lig_sec == 1)){ //Ligação Direto ao Terra

            ramo->Ypp[2][2] = y + y_shunt;
            
            ramo->Yps[2][2] = -y;
            
            ramo->Ysp[2][2] = -y;
            
            ramo->Yss[2][2] = y;        
        }
        else if((trafo->lig_pri == 2)&& (trafo->lig_sec == 1)){ //Ligação FF - FN - de CA para CN
            ramo->Ypp[0][0] = 2/3*(y + y_shunt);
            ramo->Ypp[0][2] = -1/3*(y + y_shunt);
            ramo->Ypp[2][0] = -1/3*(y + y_shunt);
            ramo->Ypp[2][2] = 2/3*(y + y_shunt);
            
            ramo->Yps[0][2] = -y/(pow(3,0.5));
            ramo->Yps[2][2] = -y/(pow(3,0.5));
            
            ramo->Ysp[2][0] = -y/(pow(3,0.5));
            ramo->Ysp[2][2] = -y/(pow(3,0.5));

            ramo->Yss[2][2] = y;
        }
        else if((trafo->lig_pri == 2)&& (trafo->lig_sec == 2)){ //Ligação FF - FF - de CA para CA
            ramo->Ypp[0][0] = 2/3*(y + y_shunt);
            ramo->Ypp[0][2] = -1/3*(y + y_shunt);
            ramo->Ypp[2][0] = -1/3*(y + y_shunt);
            ramo->Ypp[2][2] = 2/3*(y + y_shunt);
            
            ramo->Yps[0][0] = -2/3*(y);
            ramo->Yps[0][2] = 1/3*(y);
            ramo->Yps[2][0] = 1/3*(y);
            ramo->Yps[2][2] = -2/3*(y);
            
            ramo->Ysp[0][0] = -2/3*(y);
            ramo->Ysp[0][2] = 1/3*(y);
            ramo->Ysp[2][0] = 1/3*(y);
            ramo->Ysp[2][2] = -2/3*(y);

            ramo->Yss[0][0] = 2/3*(y);
            ramo->Yss[0][2] = -1/3*(y);
            ramo->Yss[2][0] = -1/3*(y);
            ramo->Yss[2][2] = 2/3*(y);
        }
        else if((trafo->lig_pri == 1) && (trafo->lig_sec == 6)){ //Ligação com Tap Central no Secundário
            y_mutua = 1/(trafo->r_mutua + I*trafo->x_mutua);
            
            ramo->Ypp[2][2] = 2*y + y_shunt;
            
            ramo->Yps[2][0] = -y;
            ramo->Yps[2][1] = -y;
            
            ramo->Ysp[0][2] = -y;
            ramo->Ysp[1][2] = -y;

            ramo->Yss[0][0] = y + y_mutua;
            ramo->Yss[0][1] = -y_mutua;
            ramo->Yss[1][0] = -y_mutua;
            ramo->Yss[1][1] = y + y_mutua;
        }
    }
    else if (trafo->fases == AB){ //Transformador Bifásico na fase AB - Ligações Abertas
        if((trafo->lig_pri == 5)&& (trafo->lig_sec == 5)){ //Ligação OD-OD
            Yiiii[0][2] = 0;
            Yiiii[0][2] = 0;
            Yiiii[1][1] = 2*Yiiii[1][1];
            
            c_matIgual(ramo->Ypp, Yiiii, 3);
            c_matIgual(ramo->Yps, Yiiii, 3);
            c_matMultEsc(ramo->Yps, -1, 3);
            c_matIgual(ramo->Ysp, Yiiii, 3);
            c_matMultEsc(ramo->Ysp, -1, 3);
            c_matIgual(ramo->Yss, Yiiii, 3);
            // ramo->Yss[2][0] = ramo->Yss[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            // ramo->Yss[2][1] = ramo->Yss[2][1] + 100*cabs(y);
            // ramo->Yss[2][2] = ramo->Yss[2][2] + 100*cabs(y);                  
        }
        else if((trafo->lig_pri == 5)&& (trafo->lig_sec == 4)){ //Ligação OD-OY
            Yiiii[0][2] = 0;
            Yiiii[0][2] = 0;
            Yiiii[1][1] = 2*Yiiii[1][1];
            c_matMultEsc(Yiiii, 1/3, 3);
            c_matIgual(ramo->Ypp, Yiiii, 3); //Atualizar a corrente de magnetização
            
            // ramo->Yss[2][0] = ramo->Yss[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            // ramo->Yss[2][1] = ramo->Yss[2][1] + 100*cabs(y);
            // ramo->Yss[2][2] = ramo->Yss[2][2] + 100*cabs(y); 
            

            ramo->Ysp[0][0] = -(y)/(pow(3,0.5));
            ramo->Ysp[0][1] = (y)/(pow(3,0.5));
            ramo->Ysp[1][1] = -(y)/(pow(3,0.5));
            ramo->Ysp[1][2] = (y)/(pow(3,0.5));
            
            ramo->Yps[0][0] = -(y)/(pow(3,0.5));
            ramo->Yps[1][0] = (y)/(pow(3,0.5));
            ramo->Yps[1][1] = -(y)/(pow(3,0.5));
            ramo->Yps[2][1] = (y)/(pow(3,0.5));
            
            ramo->Yss[0][0] = (y);
            ramo->Yss[1][1] = (y);
        }
        else if((trafo->lig_pri == 4)&& (trafo->lig_sec == 5)){ //Ligação OY-OD
            ramo->Ypp[0][0] = (y + y_shunt);
            ramo->Ypp[1][1] = (y + y_shunt);
            
            ramo->Yps[0][0] = -(y)/(pow(3,0.5));
            ramo->Yps[0][1] = (y)/(pow(3,0.5));
            ramo->Yps[1][1] = -(y)/(pow(3,0.5));
            ramo->Yps[1][2] = (y)/(pow(3,0.5));
            
            ramo->Ysp[0][0] = -(y)/(pow(3,0.5));
            ramo->Ysp[1][0] = (y)/(pow(3,0.5));
            ramo->Ysp[1][1] = -(y)/(pow(3,0.5));
            ramo->Ysp[2][1] = (y)/(pow(3,0.5));
                        
            Yiiii[0][2] = 0;
            Yiiii[0][2] = 0;
            Yiiii[1][1] = 2*Yiiii[1][1];
            c_matMultEsc(Yiiii, 1/3, 3);
            c_matIgual(ramo->Yss, Yiiii, 3);
            
            // ramo->Yss[2][0] = ramo->Yss[2][0] + 100*cabs(y); //Sequência zero para manter o quadripólo com posto completo
            // ramo->Yss[2][1] = ramo->Yss[2][1] + 100*cabs(y);
            // ramo->Yss[2][2] = ramo->Yss[2][2] + 100*cabs(y); 
        }
    }


    int i;
    for (int i=0;i<3;i++){
        free(Yi[i]);
        free(Yii[i]);
        free(Yiii[i]);
        free(Yiiii[i]);
    }
    free(Yi);
    free(Yii);
    free(Yiii);
    free(Yiiii);
}

//Converte a matriz de impedância em admitância e monta quadripolo dos ramos do grafo
/**
 * @brief Função auxiliar para montar o quadripólo de admitância trifásico de reguladores de tensão
 * 
 * Essa função monta o quadripólo de admitâncias trifásicas matricial para ramos da rede elétrica cujos componentes são reguladores de tensão.
 * Esta função recebe como parâmetro de entrada e saída uma estrutura do tipo TF_DRAM @p ramo que armazena as informações matriciais do quadripólo de
 * admitâncias de um ramo respectivo, e como parâmetro de entrada os dados detalhados do reguladores @p reg com o parâmetros deste componente.
 * A função retorna @c void .
 * 
 * @param *ramo ponteiro o ramo que será montado o quadripólo
 * @param *reg ponteiro para as informações detalhadas do regulador de tensão
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void montaQuadripoloRegulador(TF_DRAM *ramo, TF_DREG *reg){
    int aux = 1;
    __complex__ double y, **Yi, **Yii,**Yiii;
    
    //Aloca Matrizes de Quadripolos
    ramo->Ypp = c_matAloca(3);
    ramo->Yps = c_matAloca(3);
    ramo->Ysp = c_matAloca(3);
    ramo->Yss = c_matAloca(3);
    
    //Aloca Matrizes de Ligação D-Y-YN
    Yi = c_matAloca(3);
    Yii = c_matAloca(3);
    Yiii = c_matAloca(3);
    
    //Admitância do trafo
    y = 1/(reg->R + I*reg->X);
    
    //Yi
    Yi[0][0] = 1;
    Yi[0][1] = 0;
    Yi[0][2] = 0;
    Yi[1][0] = 0;
    Yi[1][1] = 1;
    Yi[1][2] = 0;
    Yi[2][0] = 0;
    Yi[2][1] = 0;
    Yi[2][2] = 1;
    //Yii
    Yii[0][0] = 2;
    Yii[0][1] = -1;
    Yii[0][2] = -1;
    Yii[1][0] = -1;
    Yii[1][1] = 2;
    Yii[1][2] = -1;
    Yii[2][0] = -1;
    Yii[2][1] = -1;
    Yii[2][2] = 2;
    //Yiii
    Yiii[0][0] = -1/(pow(3,0.5));
    Yiii[0][1] = 1/(pow(3,0.5));
    Yiii[0][2] = 0;
    Yiii[1][0] = 0;
    Yiii[1][1] = -1/(pow(3,0.5));
    Yiii[1][2] = 1/(pow(3,0.5));
    Yiii[2][0] = 1/(pow(3,0.5));
    Yiii[2][1] = 0;
    Yiii[2][2] = -1/(pow(3,0.5));
    
    
    //Feito somente para Yn
    switch (ramo->fases){
        case 1:
            Yi[1][1] = 0;
            Yi[2][2] = 0;
            break;    
        case 2:
            Yi[0][0] = 0;
            Yi[2][2] = 0;
            break;
        case 3:
            Yi[1][1] = 0;
            Yi[0][0] = 0;
            break;
        case 4:
            Yi[2][2] = 0;
            break;    
        case 5:
            Yi[1][1] = 0;
            break;
        case 6:
            Yi[0][0] = 0;
            break; 
    }   
       
    c_matMultEsc(Yi, y, 3);
    c_matMultEsc(Yii, y/3, 3);
    c_matMultEsc(Yiii, y, 3);
    
    //Feito Somente para YN!!!!!!!!!!!!!!!!!!!!!!!!!
    if(reg->lig == 1){ //Ligação YN
        c_matIgual(ramo->Ypp, Yi, 3);
        c_matIgual(ramo->Yps, Yi, 3);
        c_matMultEsc(ramo->Yps, -1, 3);
        c_matIgual(ramo->Ysp, Yi, 3);
        c_matMultEsc(ramo->Ysp, -1, 3);
        c_matIgual(ramo->Yss, Yi, 3);
    }
    else if(reg->lig == 2){ //Ligação D
        c_matIgual(ramo->Ypp, Yii, 3);
        c_matIgual(ramo->Yps, Yii, 3);
        c_matMultEsc(ramo->Yps, -1, 3);
        c_matIgual(ramo->Ysp, Yii, 3);
        c_matMultEsc(ramo->Ysp, -1, 3);
        c_matIgual(ramo->Yss, Yii, 3);
    }
    else if(reg->lig == 3){ //Ligação Y
        c_matIgual(ramo->Ypp, Yi, 3);
        c_matIgual(ramo->Yps, Yiii, 3);
        c_matIgual(ramo->Ysp, Yiii, 3);
        c_matTransp(ramo->Ysp, 3);
        c_matIgual(ramo->Yss, Yii, 3);
    }
    else if(reg->lig == 4){ //Ligação OYn
        c_matIgual(ramo->Ypp, Yii, 3);
        c_matIgual(ramo->Yps, Yii, 3);
        c_matMultEsc(ramo->Yps, -1, 3);
        c_matIgual(ramo->Ysp, Yii, 3);
        c_matMultEsc(ramo->Ysp, -1, 3);
        c_matIgual(ramo->Yss, Yii, 3);
    }
    else if(reg->lig == 5){ //Ligação OD
        c_matIgual(ramo->Ypp, Yii, 3);
        c_matIgual(ramo->Yps, Yii, 3);
        c_matMultEsc(ramo->Yps, -1, 3);
        c_matIgual(ramo->Ysp, Yii, 3);
        c_matMultEsc(ramo->Ysp, -1, 3);
        c_matIgual(ramo->Yss, Yii, 3);
    }
    int i;
    for (i=0;i<3;i++) free(Yi[i]);
    free(Yi);
    for (i=0;i<3;i++) free(Yii[i]);
    free(Yii);
    for (i=0;i<3;i++) free(Yiii[i]);
    free(Yiii);
}

//Monta quadripolo dos shunts do grafo
/**
 * @brief Função auxiliar para montar o quadripólo de admitância trifásico de bancos de capacitores
 * 
 * Essa função monta o quadripólo de admitâncias trifásicas matricial para ramos da rede elétrica cujos componentes são transformadores de potência.
 * Esta função recebe como parâmetro de entrada e saída uma estrutura do tipo TF_GRAFO @p no que armazena as informações matriciais do quadripólo de
 * admitâncias de um nó respectivo, e como parâmetro de entrada os dados detalhados do banco de capacitor @p shunt com o parâmetros deste componente.
 * A função retorna @c void .
 * 
 * @param *no ponteiro o para o nó que será montado o quadripólo
 * @param *shunt ponteiro para as informações detalhadas do banco de capacitor
 * @return void
 * @see
 * @note 
 * @warning Como se trata de uma função auxiliar essa não deve ser chamada diretamente por outras partes do programa.
 */
void montaQuadripoloShunt(TF_GRAFO *no, TF_DSHNT *shunt){
    int aux = 1, i, j;
    __complex__ double ya,yb,yc, **Yi, **Yii,**Yiii,**Ysh;
    
    //Aloca Matrizes de Quadripolos
    Ysh = c_matAloca(3);
    
    
    //Aloca Matrizes de Ligação D-Y-YN
    Yi = c_matAloca(3);
    Yii = c_matAloca(3);
    Yiii = c_matAloca(3);
    
    //Admitância do trafo
    
    ya = I*shunt->Qnom[0]/(pow(shunt->Vbase/sqrt(3),2)/pow(no->Vbase,2));
    yb = I*shunt->Qnom[1]/(pow(shunt->Vbase/sqrt(3),2)/pow(no->Vbase,2));
    yc = I*shunt->Qnom[2]/(pow(shunt->Vbase/sqrt(3),2)/pow(no->Vbase,2));
    
    //Yi
    Yi[0][0] = 1*ya;
    Yi[0][1] = 0;
    Yi[0][2] = 0;
    Yi[1][0] = 0;
    Yi[1][1] = 1*yb;
    Yi[1][2] = 0;
    Yi[2][0] = 0;
    Yi[2][1] = 0;
    Yi[2][2] = 1*yc;
    //Yii
    Yii[0][0] = 2*ya;
    Yii[0][1] = -1*yb;
    Yii[0][2] = -1*yc;
    Yii[1][0] = -1*ya;
    Yii[1][1] = 2*yb;
    Yii[1][2] = -1*yc;
    Yii[2][0] = -1*ya;
    Yii[2][1] = -1*yb;
    Yii[2][2] = 2*yc;
    //Yiii
    Yiii[0][0] = -1/(pow(3,0.5))*ya;
    Yiii[0][1] = 1/(pow(3,0.5))*yb;
    Yiii[0][2] = 0;
    Yiii[1][0] = 0;
    Yiii[1][1] = -1/(pow(3,0.5))*yb;
    Yiii[1][2] = 1/(pow(3,0.5))*yc;
    Yiii[2][0] = 1/(pow(3,0.5))*ya;
    Yiii[2][1] = 0;
    Yiii[2][2] = -1/(pow(3,0.5))*yc;
    
    if(shunt->lig == 1){ //Ligação YN
        c_matIgual(Ysh, Yi, 3);
    }
    else if(shunt->lig == 2){ //Ligação D trifásico
        c_matIgual(Ysh, Yiii, 3);
        Ysh[2][0] = Ysh[2][0] + 1; //Sequência zero para manter o quadripólo com posto completo
        Ysh[2][1] = Ysh[2][1] + 1;
        Ysh[2][2] = Ysh[2][2] + 1;
    }
    else if(shunt->lig == 3){ //Ligação Y
        c_matIgual(Ysh, Yii, 3);
    }
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            no->Ysh[i][j] += Ysh[i][j];
        }
    }
    
//    printf("\n\Quadripolo\n\n");
//    c_matImprime(ramo->Ypp,3);
//    printf("\n");
//    c_matImprime(ramo->Yps,3);
//    printf("\n");
//    c_matImprime(ramo->Ysp,3);
//    printf("\n");
//    c_matImprime(ramo->Yss,3);
//    printf("\n");
    for (int i=0;i<3;i++){
        free(Yi[i]);
        free(Yii[i]);
        free(Yiii[i]);
        free(Ysh[i]);
    }
    free(Yi);
    free(Yii);
    free(Yiii);
    free(Ysh);
    
}

//Função que transforma os dados do grafo em pu e monta as matrizes de admitância dos elementos do grafo - 
// Monta Elemento a Elemento de acordo com dados de entrada
/**
 * @brief Função principal que realiza a transformação das grandezas elétricas em pu (por unidade)
 * 
 * Esta função aloca transformas os dados e informações da rede elétrica das unidades de medida convencionais (S.I.) em pu (por unidade) para 
 * realizar os cálculos elétricos da condição operacional da rede elétrica. Além disto monta as matrizes de quadripólos para representação 
 * das relações elétricas no componentes da re de elétrica. Recebe como parâmetro de entrada e saída o grafo da rede elétrica em @p grafo e como parâmetros 
 * de entrada os ramos @p ramos e quantidades totais de barras e ramos da rede @p numeroBarras e @p numeroRamos . Para realizar a transformação em pu
 * as tensões base são definidas de acordo com os dados lidos nos arquivos de entrada (em especial DBAR.csv) e a potência base da rede elétrica é
 * definida como parâmetro de entrada em @p Sbase.
 * A função retorna @c void .
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * double Sbase = 10000;
 * char *folder = NULL;
 * DBAR *barraExemplo = NULL;
 * TF_DRAM *ramoExemplo = NULL; 
 * TF_GRAFO *grafoExemplo = NULL;
 * 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * geraGrafo(&grafoExemplo, barraExemplo, numeroBarras,ramoExemplo,numeroRamos); 
 * calculaPU(grafo, numeroBarras, ramo, numeroRamos, Sbase);
 * @endcode
 *  
 * @param grafo estrutura de dados com as informações de adjacência do grafo
 * @param numeroBarras quantidade total de barras
 * @param ramos estrutura de dados com os dados dos ramos da rede elétrica
 * @param numeroRamos quantidade total de ramos
 * @param Sbase potência base da rede elétrica
 * @return void
 * @see montaQuadripoloLinha
 * @see montaQuadripoloTrafo
 * @see montaQuadripoloRegulador
 * @see montaQuadripoloShunt
 * @note 
 * @warning
 */
void calculaPU(TF_GRAFO *grafo, long int numeroBarras, TF_DRAM *ramos, long int numeroRamos, double Sbase) {
    int i, idNo, idRam;
    TF_FILABARRAS * lista_barras = NULL;
    long int barraAdj = 0;
    double Vbase;
    
    
    //Transforma em PU informações dos nós do grafo
    for (idNo=0;idNo<numeroBarras;idNo++){
        //Transforma em pu os dados de carga
        for(i=0;i<grafo[idNo].barra->nloads;i++){
            grafo[idNo].barra->loads[i].Pnom[0] = 1000*grafo[idNo].barra->loads[i].Pnom[0]/Sbase; 
            grafo[idNo].barra->loads[i].Pnom[1] = 1000*grafo[idNo].barra->loads[i].Pnom[1]/Sbase;
            grafo[idNo].barra->loads[i].Pnom[2] = 1000*grafo[idNo].barra->loads[i].Pnom[2]/Sbase;
            
            grafo[idNo].barra->loads[i].Qnom[0] = 1000*grafo[idNo].barra->loads[i].Qnom[0]/Sbase; 
            grafo[idNo].barra->loads[i].Qnom[1] = 1000*grafo[idNo].barra->loads[i].Qnom[1]/Sbase;
            grafo[idNo].barra->loads[i].Qnom[2] = 1000*grafo[idNo].barra->loads[i].Qnom[2]/Sbase;            
        } 
        
        //Transforma em PU os shunts da rede elétrica
        for(i=0;i<grafo[idNo].barra->nshunts;i++){
            grafo[idNo].barra->shunts[i].Qnom[0] = 1000*grafo[idNo].barra->shunts[i].Qnom[0]/Sbase; 
            grafo[idNo].barra->shunts[i].Qnom[1] = 1000*grafo[idNo].barra->shunts[i].Qnom[1]/Sbase;
            grafo[idNo].barra->shunts[i].Qnom[2] = 1000*grafo[idNo].barra->shunts[i].Qnom[2]/Sbase;
            montaQuadripoloShunt(&grafo[idNo],&grafo[idNo].barra->shunts[i]);
        }
        
        /*//Transforma em pu os dados de GDs - Futuro para fluxo de carga
        for(i=0;i<grafo[idNo].barra->ngds;i++){

        }*/
    }
    
    //Transforma em PU informações dos ramos do grafo
    for (idRam=0;idRam<numeroRamos;idRam++){
        Vbase = grafo[ramos[idRam].m].Vbase;
        //Transforma as impedâncias em pu
        switch(ramos[idRam].tipo){
            case 0:
                ramos[idRam].linha.Zaa = ramos[idRam].linha.Zaa/((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Zab = ramos[idRam].linha.Zab/((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Zac = ramos[idRam].linha.Zac/((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Zbb = ramos[idRam].linha.Zbb/((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Zbc = ramos[idRam].linha.Zbc/((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Zcc = ramos[idRam].linha.Zcc/((pow(Vbase,2))/Sbase);
                
                ramos[idRam].linha.Baa = ramos[idRam].linha.Baa * ((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Bab = ramos[idRam].linha.Bab * ((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Bac = ramos[idRam].linha.Bac * ((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Bbb = ramos[idRam].linha.Bbb * ((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Bbc = ramos[idRam].linha.Bbc * ((pow(Vbase,2))/Sbase);
                ramos[idRam].linha.Bcc = ramos[idRam].linha.Bcc * ((pow(Vbase,2))/Sbase);
                
                ramos[idRam].ampacidade = ramos[idRam].linha.ampacidade; // / (Sbase/(sqrt(3) * Vbase));
                
//                printf("\n%lf",ramos[idRam].ampacidade);
                
                montaQuadripoloLinha(&ramos[idRam], &ramos[idRam].linha);
                break;
            case 1:
                ramos[idRam].trafo.R = 3*ramos[idRam].trafo.R/((pow(Vbase,2))/Sbase);
                ramos[idRam].trafo.X = 3*ramos[idRam].trafo.X/((pow(Vbase,2))/Sbase);
                
                ramos[idRam].Snominal = 1000*ramos[idRam].trafo.Snominal / (Sbase);

                montaQuadripoloTrafo(&ramos[idRam], &ramos[idRam].trafo);
                break;
            case 2:
                ramos[idRam].regulador.R = 3*ramos[idRam].regulador.R/((pow(Vbase,2))/Sbase);
                ramos[idRam].regulador.X = 3*ramos[idRam].regulador.X/((pow(Vbase,2))/Sbase);
                
                ramos[idRam].Snominal = 1000*ramos[idRam].regulador.Snominal / (Sbase);
                ramos[idRam].regulador.Vnom = Vbase;

                montaQuadripoloRegulador(&ramos[idRam], &ramos[idRam].regulador);
                break;  
            case 3:
                ramos[idRam].ampacidade = ramos[idRam].ampacidade; // / (Sbase/(sqrt(3) * Vbase));
                break;
        }
    }   
}


//Atualiza as matrizes Y de acordo com os taps dos transformadores
/**
 * @brief Função principal que realiza a atualização dos valores de taps nas matrizes do quadripólo de admitâncias trifásico de reguladores e transformadores
 * 
 * Esta função atualiza as matrizes do quadripólo de admitâncias dos ramos da rede elétrica @p ramos de acordo com os valores dos taps indicados 
 * dentro da estrutura de dados de reguladores de tensão e transformadores de potência. Pode ser utilizada para atualizar os taps conforme
 * novas informações discretas são obtidas pelo sistema supervisório. Recebe como parâmetro de entrada também a quantidade total de ramos.
 * A função retorna @c void .
 * 
 * Para utilizar a função:
 * @code
 * long int numeroBarras = 0;
 * long int numeroAlimentadores = 0;
 * long int numeroRamos = 0;
 * double Sbase = 10000;
 * char *folder = NULL;
 * DBAR *barraExemplo = NULL;
 * TF_DRAM *ramoExemplo = NULL; 
 * TF_GRAFO *grafoExemplo = NULL;
 * 
 * folder = leituraDados(&barraExemplo,&ramoExemplo,&numeroBarras,&numeroRamos,&numeroAlimentadores);
 * if (folder !=NULL)
 *      printf("leitura concluida\n");
 * geraGrafo(&grafoExemplo, barraExemplo, numeroBarras,ramoExemplo,numeroRamos); 
 * calculaPU(grafo, numeroBarras, ramo, numeroRamos, Sbase);
 * atualizaTaps(ramo, numeroRamos);
 * @endcode
 *  
 * @param ramos estrutura de dados com os dados dos ramos da rede elétrica
 * @param numeroRamos quantidade total de ramos
 * @return void
 * @see montaQuadripoloTrafo
 * @see montaQuadripoloRegulador
 * @note 
 * @warning
 */
void atualizaTaps(TF_DRAM *ramos, long int numeroRamos){
    int i;
    
    for(i=0;i<numeroRamos;i++){
        if (ramos[i].tipo == 2){
            ramos[i].tap_pri[0] = (1 + ramos[i].regulador.tap[0]*ramos[i].regulador.regulacao/ramos[i].regulador.ntaps);
            ramos[i].tap_pri[1] = (1 + ramos[i].regulador.tap[1]*ramos[i].regulador.regulacao/ramos[i].regulador.ntaps);
            ramos[i].tap_pri[2] = (1 + ramos[i].regulador.tap[2]*ramos[i].regulador.regulacao/ramos[i].regulador.ntaps);
            
            ramos[i].tap_sec[0] = 1;
            ramos[i].tap_sec[1] = 1;
            ramos[i].tap_sec[2] = 1;
            
            ramos[i].Ypp[0][0] = pow(ramos[i].tap_pri[0],2)*ramos[i].Ypp[0][0];
            ramos[i].Ypp[1][1] = pow(ramos[i].tap_pri[1],2)*ramos[i].Ypp[1][1];
            ramos[i].Ypp[2][2] = pow(ramos[i].tap_pri[2],2)*ramos[i].Ypp[2][2];
            
            ramos[i].Yps[0][0] = ramos[i].tap_sec[0]*ramos[i].tap_pri[0]*ramos[i].Yps[0][0];
            ramos[i].Yps[1][1] = ramos[i].tap_sec[1]*ramos[i].tap_pri[1]*ramos[i].Yps[1][1];
            ramos[i].Yps[2][2] = ramos[i].tap_sec[2]*ramos[i].tap_pri[2]*ramos[i].Yps[2][2];
            
            ramos[i].Ysp[0][0] = ramos[i].tap_sec[0]*ramos[i].tap_pri[0]*ramos[i].Ysp[0][0];
            ramos[i].Ysp[1][1] = ramos[i].tap_sec[1]*ramos[i].tap_pri[1]*ramos[i].Ysp[1][1];
            ramos[i].Ysp[2][2] = ramos[i].tap_sec[2]*ramos[i].tap_pri[2]*ramos[i].Ysp[2][2];
            
            ramos[i].Yss[0][0] = pow(ramos[i].tap_sec[0],2)*ramos[i].Yss[0][0];
            ramos[i].Yss[1][1] = pow(ramos[i].tap_sec[1],2)*ramos[i].Yss[1][1];
            ramos[i].Yss[2][2] = pow(ramos[i].tap_sec[2],2)*ramos[i].Yss[2][2];
        }
        else if (ramos[i].tipo == 1){ //REVISAR PARA TAP FORA DA NOMINAL EM TRAFOS
            ramos[i].tap_pri[0] = 1; // 
            ramos[i].tap_pri[1] = 1;
            ramos[i].tap_pri[2] = 1;
            
            ramos[i].tap_sec[0] = 1; // ramos[i].trafo.tap_sec
            ramos[i].tap_sec[1] = 1;
            ramos[i].tap_sec[2] = 1;
        }
                
    
    }
    
    
    
}




//Atualiza as matrizes Y de acordo com os taps dos transformadores
/**
 * @brief Função principal que realiza a atualização dos estados lógicos das chaves e elementos seccionadores a partir do FEAT_NUM
 * 
 * Esta função atualiza 
 * 
 * Para utilizar a função:
 * @code
 * @endcode
 *  
 * @param ramos estrutura de dados com os dados dos ramos da rede elétrica
 * @param numeroRamos quantidade total de ramos
 * @return void
 * @note 
 * @warning
 */
void atualizaEstadoChaves(const char *folder, const char *filename, TF_GRAFO *grafo, long int numeroBarras)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados,*dados1; /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    long int contador =0, i, aux; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac,numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */
    long int de, para,estado;
    
    dados1 = strcat(folder,filename);
    
    FILE *arquivo;
    arquivo = fopen(dados1,"r+");
       
    
    // Le o arquivo de curva de cargas até o fim
    while( (fgets(blocoLeitura, 2000, arquivo))!= NULL ){
        dados = blocoLeitura;
        de = (getfield_int(dados,1));
        para = (getfield_int(dados,2));
        estado = (getfield_int(dados,3));
        
        for(i=0;i<grafo[de].numeroAdjacentes;i++){
            if (grafo[de].adjacentes[i].idNo == para)
                grafo[de].adjacentes[i].estado = estado;
        }
        
        for(i=0;i<grafo[para].numeroAdjacentes;i++){
            if (grafo[para].adjacentes[i].idNo == de)
                grafo[para].adjacentes[i].estado = estado;
        }
        
        
    }
    fclose(arquivo);
    
    
    
}



// Supressão de barras de passagem (redução de injeções nulas)
/**
 * @brief Função auxiliar para suprimir barras de injeção nula
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
void supressaoBarrasPassagem(TF_GRAFO *grafo, TF_ALIMENTADOR *alimentadores, long int idAlim, TF_DRAM *ramos)
{
    long int i,j, k, para, prev, post, ramPrev, ramPost,idAdj;
    BOOL aux_teste;
    TF_FILABARRAS *barraAtual, *barraAnterior;
    
    // Percorre o alimentador 
    barraAtual = &alimentadores[idAlim].rnp[0];  
    barraAnterior = NULL;
    while(barraAtual != NULL)
    {
        i = barraAtual->idNo;
        aux_teste = true;
        if ((grafo[i].tipo != 2) && (grafo[i].barra->nloads ==0) && (grafo[i].barra->nshunts ==0) && (grafo[i].barra->ngds==0) && (grafo[i].numeroAdjacentes == 2)){
            // Caso não tenha trafos ou regulador ou chave como adjacente, a barra deve ser suprimida
            aux_teste = false;
            for (j=0;j<grafo[i].numeroAdjacentes;j++){
                para = grafo[i].adjacentes[j].idNo;
                if ((grafo[i].adjacentes[j].tipo == 1) || (grafo[i].adjacentes[j].tipo == 2) || (grafo[i].adjacentes[j].tipo == 3) || (grafo[para].fases != grafo[i].fases) || (grafo[grafo[i].adjacentes[j].idNo].idAlim != idAlim)) aux_teste = true;
                if ((grafo[para].profundidade) < grafo[i].profundidade) prev = para;
                if ((grafo[para].profundidade) > grafo[i].profundidade) post = para;
            }
            if (!aux_teste){
                //Barra deve ser suprimida
                grafo[i].idAlim = -2;
                // suprime do nó a montante
                for (k=0;k<grafo[prev].numeroAdjacentes;k++){
                    if (grafo[prev].adjacentes[k].idNo == i){
                        ramPrev = grafo[prev].adjacentes[k].idram;
                        grafo[prev].adjacentes[k].idNo = post; 
                        grafo[prev].adjacentes[k].tipo = 0;
                        ramos[ramPrev].PARA = post;
                        ramos[ramPrev].tipo = 0;
                        idAdj = k;
                    }
                }
                // suprime do nó a jusante
                for (k=0;k<grafo[post].numeroAdjacentes;k++){
                    if (grafo[post].adjacentes[k].idNo == i){
                        ramPost = grafo[post].adjacentes[k].idram;
                        grafo[post].adjacentes[k].idram = ramPrev;
                        grafo[post].adjacentes[k].ramo = &ramos[ramPrev];
                        grafo[post].adjacentes[k].idNo = prev;  
                        grafo[post].adjacentes[k].tipo = 0;
                        ramos[ramPost].tipo = 0;
                        grafo[post].profundidade--;
                    }
                }
                if ((ramos[prev].tipo == 3) && (ramos[ramPost].tipo == 0)){
                    ramos[ramPrev].tipo = 0;
                    ramos[ramPrev].linha.fases = ramos[ramPost].linha.fases;
                    ramos[ramPrev].linha.comprimento = ramos[ramPost].linha.comprimento;
                    ramos[ramPrev].linha.Zaa = ramos[ramPost].linha.Zaa;
                    ramos[ramPrev].linha.Zab = ramos[ramPost].linha.Zab;
                    ramos[ramPrev].linha.Zac = ramos[ramPost].linha.Zac;
                    ramos[ramPrev].linha.Zbb = ramos[ramPost].linha.Zbb;
                    ramos[ramPrev].linha.Zbc = ramos[ramPost].linha.Zbc;
                    ramos[ramPrev].linha.Zcc = ramos[ramPost].linha.Zcc;
                    ramos[ramPrev].linha.Baa = ramos[ramPost].linha.Baa;
                    ramos[ramPrev].linha.Bab = ramos[ramPost].linha.Bab;
                    ramos[ramPrev].linha.Bac = ramos[ramPost].linha.Bac;
                    ramos[ramPrev].linha.Bbb = ramos[ramPost].linha.Bbb;
                    ramos[ramPrev].linha.Bbc = ramos[ramPost].linha.Bbc;
                    ramos[ramPrev].linha.Bcc = ramos[ramPost].linha.Bcc;
                }
                else if ((ramos[prev].tipo == 3) && (ramos[ramPost].tipo == 3)){
                    ramos[ramPrev].tipo = 0;
                    ramos[ramPrev].linha.fases = ramos[ramPost].fases;
                    ramos[ramPrev].linha.comprimento = 0.02;
                    ramos[ramPrev].linha.Zaa = 0.00001;
                    ramos[ramPrev].linha.Zab = 0;
                    ramos[ramPrev].linha.Zac = 0;
                    ramos[ramPrev].linha.Zbb = 0.00001;
                    ramos[ramPrev].linha.Zbc = 0;
                    ramos[ramPrev].linha.Zcc = 0.00001;
                    ramos[ramPrev].linha.Baa = 0;
                    ramos[ramPrev].linha.Bab = 0;
                    ramos[ramPrev].linha.Bac = 0;
                    ramos[ramPrev].linha.Bbb = 0;
                    ramos[ramPrev].linha.Bbc = 0;
                    ramos[ramPrev].linha.Bcc = 0;
                }
                else if ((ramos[prev].tipo == 0)){
                    // Soma as impedâncias
                    ramos[ramPrev].linha.comprimento +=  ramos[ramPost].linha.comprimento;
                    ramos[ramPrev].linha.Zaa += ramos[ramPost].linha.Zaa;
                    ramos[ramPrev].linha.Zab += ramos[ramPost].linha.Zab;
                    ramos[ramPrev].linha.Zac += ramos[ramPost].linha.Zac;
                    ramos[ramPrev].linha.Zbb += ramos[ramPost].linha.Zbb;
                    ramos[ramPrev].linha.Zbc += ramos[ramPost].linha.Zbc;
                    ramos[ramPrev].linha.Zcc += ramos[ramPost].linha.Zcc;
                    ramos[ramPrev].linha.Baa += ramos[ramPost].linha.Baa;
                    ramos[ramPrev].linha.Bab += ramos[ramPost].linha.Bab;
                    ramos[ramPrev].linha.Bac += ramos[ramPost].linha.Bac;
                    ramos[ramPrev].linha.Bbb += ramos[ramPost].linha.Bbb;
                    ramos[ramPrev].linha.Bbc += ramos[ramPost].linha.Bbc;
                    ramos[ramPrev].linha.Bcc += ramos[ramPost].linha.Bcc;
                }
                //Atualiza RNP
                if ((barraAnterior != NULL)){
                    barraAnterior->prox = barraAtual->prox;
                    barraAnterior->prox->idAdj = idAdj;
                }
                alimentadores[idAlim].numeroNos--;
            }          
        }
        if (aux_teste){
            barraAnterior = barraAtual;
        }
        barraAtual = barraAtual->prox;
    }
    
}