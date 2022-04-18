#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesSetor.h"
#include "funcoesInicializacao.h"

long int idSetor = 0; /*< Variável global que calcula o id dos setores **/
int numeroSetoresAdj; /*< Variável global para cálculo do número de adjacentes **/
int numeroNosSetor;/*< Variável global para cálculo do número de nos do setor **/
long int numeroConsumidores; /*< Variável global para cálculo do número de consumidores do setor **/
long int numeroConsumidoresEspeciais; /*< Variável global para cálculo do número de consumidores especiais do setor **/
int numeroSetoresAlimentador; /*< Variável global para cálculo do número de setores do alimentador **/
//int numeroSetores; //removido duplicado 
extern long int numeroAlimentadores;

void adicionaNo(BARRASETOR **setor, long int idNo)
{
    BARRASETOR *novoSetor;
    
    novoSetor = (BARRASETOR *) malloc(sizeof(BARRASETOR));
    
    if(novoSetor == NULL) exit(EXIT_FAILURE);
    
    novoSetor->idNo = idNo;
    novoSetor->prox = *setor;
    
    *setor = novoSetor;
}

void imprimeSetor(BARRASETOR * setor)
{
    BARRASETOR * umSetor = setor;
    printf("ids:\t");
    while(umSetor != NULL)
    {
        printf("%ld\t", (umSetor->idNo));
        umSetor = umSetor->prox;
    }
    printf("\n");
}

void adicionaSetorNaLista(LISTASETORES **listaSetores, BARRASETOR *setor, BARRASETOR *noAdj){
    LISTASETORES *novaLista = NULL;
    LISTASETORES *aux = NULL;
    
    novaLista = (LISTASETORES *) malloc(sizeof(LISTASETORES));
    
    if(novaLista == NULL) exit(EXIT_FAILURE);
    
    novaLista->barraSetor = setor;
    novaLista->barraSetorAdj = noAdj;
    novaLista->idSetor = idSetor;
    novaLista->numeroAdjacentes = numeroSetoresAdj;
    novaLista->numeroNos = numeroNosSetor;
    novaLista->consumidoresEspeciais = numeroConsumidoresEspeciais;
    novaLista->consumidoresNormais = numeroConsumidores;
    novaLista->prox = NULL;
    
    numeroSetoresAlimentador++;
    aux = *listaSetores;
    
    if(aux == NULL) {
        *listaSetores = novaLista;
    }
    else {
        while(aux->prox != NULL)
            aux = aux->prox;

        aux->prox = novaLista;
    }
}

void imprimeSetores(LISTASETORES * listaSetores){
    LISTASETORES * setores = listaSetores;

    while(setores != NULL)
    {
        printf("----------------------------------------------\n");
        printf("setor: %ld numero nos %d \n", setores->idSetor, setores->numeroNos);
        imprimeSetor(setores->barraSetor);
        printf("barras adjacentes: %d \n", setores->numeroAdjacentes);
        imprimeSetor(setores->barraSetorAdj);
        setores = setores->prox;
    }
    printf("----------------------------------------------");
}

void adicionaNoNaFila(FILABARRAS ** fila, long int idNo) {
    FILABARRAS *novoVertice = NULL;
    FILABARRAS *aux = NULL;
 
    novoVertice = (FILABARRAS *)malloc(sizeof(FILABARRAS));
 
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

int retiraNoDaFila(FILABARRAS ** fila) {
    FILABARRAS *aux = NULL;
    
    int idNo = -1;
    
    aux = *fila;
    
    if(aux != NULL) *fila = aux->prox;
    
    idNo = aux->idNo;
    
    free(aux);
 
    return idNo;
}

BOOL filaNaoVazia(FILABARRAS * fila) {
     if(fila == NULL)
         return false;
     return true;
}

BOOL ligadoPorTrecho(NOADJACENTE adjacente) {
    TIPOARESTA tipoAresta = adjacente.tipoAresta;
    if(tipoAresta == trecho || tipoAresta == reguladorTensao) return true;
    return false;
}

BOOL ligadoPorChaveManual(NOADJACENTE adjacente) {
    TIPOARESTA tipoAresta = adjacente.tipoAresta;
    if(tipoAresta == chaveManual) return true;
    return false;
}

BOOL chaveFechada(NOADJACENTE adjcente) {
    ESTADOCHAVE estado = adjcente.estadoChave;
    if(estado == normalmenteFechada) return true;
    return false;
}

BOOL estaLista(BARRASETOR *setor, int idNo) {
    BARRASETOR *p;
    p = setor;
    while(p != NULL && p->idNo != idNo)
        p = p->prox;
    if(p != NULL && p->idNo == idNo)
        return true;
    else
        return false;

}

BOOL possuiInterseccao(BARRASETOR *setor, BARRASETOR *adj)
{
    BARRASETOR *p;
    p = adj;
    BOOL resultado = false;
    while(p!=NULL && !resultado)
    {
        resultado = estaLista(setor, p->idNo);
        p = p->prox; 
    }
    return resultado;
}


void buscaSetor(GRAFO * grafo, long int idNoRaiz, LISTASETORES ** listaSetores, BOOL * visitado) {
    long int i, barraAdj, idNo;    
    
    BARRASETOR * setorAtual = NULL;
    BARRASETOR * barrasAdj = NULL;
    
    FILABARRAS * filaTrecho = NULL;
    FILABARRAS * filaChaveFechada = NULL;
    
    adicionaNo(&setorAtual, idNoRaiz);
    adicionaNoNaFila(&filaTrecho, idNoRaiz);
    idSetor++;
    //printf("%d \t", idSetor);
    numeroConsumidores=grafo[idNoRaiz].priorizacoes.qtdConsumidores;
    numeroConsumidoresEspeciais=grafo[idNoRaiz].priorizacoes.qtdConsumidoresEspeciais;
    numeroSetoresAdj = 0;
    numeroNosSetor = 1;
    while(filaNaoVazia(filaTrecho))
    {
        idNo = retiraNoDaFila(&filaTrecho);
        GRAFO * no = &grafo[idNo];
        grafo[idNo].idSetor = idSetor;
        for(i = 0; i<no->numeroAdjacentes; i++)
        {
            barraAdj = no->adjacentes[i].idNo;
            if(ligadoPorTrecho(no->adjacentes[i]))
            {
                if(!visitado[barraAdj])
                {
                    if(estaLista(setorAtual, barraAdj))
                        printf("CICLO barra %ld idSetor %ld\n", barraAdj, idSetor);
                    else
                    {
                       adicionaNoNaFila(&filaTrecho, barraAdj);
                       adicionaNo(&setorAtual, barraAdj);
                       numeroNosSetor++;
                       numeroConsumidores+=grafo[barraAdj].priorizacoes.qtdConsumidores;
                       numeroConsumidoresEspeciais+=grafo[barraAdj].priorizacoes.qtdConsumidoresEspeciais;
                    }
                }
            }
            else
            {
                numeroSetoresAdj++;
                if(!visitado[barraAdj] && chaveFechada(no->adjacentes[i]))
                {
                    adicionaNoNaFila(&filaChaveFechada, barraAdj);
                }
                adicionaNo(&barrasAdj, barraAdj);
            }
        }
        visitado[no->idNo] = true;
    }
    if (possuiInterseccao(setorAtual, barrasAdj))
    {
        printf("Possui chave ligando mesmo setor %ld \n", idSetor);
    }
    adicionaSetorNaLista(listaSetores, setorAtual, barrasAdj);
    
    while(filaNaoVazia(filaChaveFechada))
    {
        idNo = retiraNoDaFila(&filaChaveFechada);
        buscaSetor(grafo, idNo, listaSetores, visitado);
    }
    
    
}

void buscaSetorAutomatica(GRAFO * grafo, long int idNoRaiz, LISTASETORES ** listaSetores, BOOL * visitado) {
    long int i, barraAdj, idNo;    
    
    BARRASETOR * setorAtual = NULL;
    BARRASETOR * barrasAdj = NULL;
    
    FILABARRAS * filaTrecho = NULL;
    FILABARRAS * filaChaveFechada = NULL;
    
    adicionaNo(&setorAtual, idNoRaiz);
    adicionaNoNaFila(&filaTrecho, idNoRaiz);
    idSetor++;
    numeroSetoresAdj = 0;
    numeroNosSetor = 1;
    while(filaNaoVazia(filaTrecho))
    {
        idNo = retiraNoDaFila(&filaTrecho);
        GRAFO * no = &grafo[idNo];
        grafo[idNo].idSetor = idSetor;
        for(i = 0; i<no->numeroAdjacentes; i++)
        {
            barraAdj = no->adjacentes[i].idNo;
            //barras ligadas por chave manual ou por trecho ou por regulador de tensão, devem compor o mesmo setor
            if(ligadoPorTrecho(no->adjacentes[i]) || ligadoPorChaveManual(no->adjacentes[i]))
            {
                if(!visitado[barraAdj])
                {
                    adicionaNoNaFila(&filaTrecho, barraAdj);
                    adicionaNo(&setorAtual, barraAdj);
                    numeroNosSetor++;
                }
            }
            else//barra ligada por chave automática, pertence a um novo setor adjacente 
            {
                numeroSetoresAdj++;
                if(!visitado[barraAdj] && chaveFechada(no->adjacentes[i]))
                {
                    adicionaNoNaFila(&filaChaveFechada, barraAdj);
                }
                adicionaNo(&barrasAdj, barraAdj);
            }
        }
        visitado[no->idNo] = true;
    }
    adicionaSetorNaLista(listaSetores, setorAtual, barrasAdj);
    
    while(filaNaoVazia(filaChaveFechada))
    {
        idNo = retiraNoDaFila(&filaChaveFechada);
        buscaSetor(grafo, idNo, listaSetores, visitado);
    }
    
    
}

LISTASETORES * buscaSetores(GRAFO *grafo, DADOSALIMENTADOR *alimentadores, long int numeroBarras) {
    int i, idAlim;
    LISTASETORES * lista_setores = NULL;
    
    BOOL visitado[(numeroBarras+1)];
    
    for(i=1; i<=numeroBarras; i++) 
        visitado[i] = false;
    
    for(i=1; i<=numeroAlimentadores; i++)
    {
      // printf("\n alimentador %d :",i);
        numeroSetoresAlimentador = 0;
        idAlim = alimentadores[i].barraAlimentador;
        buscaSetor(grafo, idAlim, &lista_setores, visitado);
        alimentadores[i].numeroSetores = numeroSetoresAlimentador;
    }
    numeroSetores = idSetor;
    return lista_setores;
}

LISTASETORES * buscaSetoresAutomatica(GRAFO *grafo, DADOSALIMENTADOR *alimentadores, long int numeroBarras) {
    int i, idAlim;
    LISTASETORES * lista_setores = NULL;
    
    BOOL visitado[(numeroBarras+1)];
    
    for(i=1; i<=numeroBarras; i++) 
        visitado[i] = false;
    
    for(i=1; i<=numeroAlimentadores; i++)
    {
        numeroSetoresAlimentador = 0;
        idAlim = alimentadores[i].barraAlimentador;
        buscaSetorAutomatica(grafo, idAlim, &lista_setores, visitado);
        alimentadores[i].numeroSetores = numeroSetoresAlimentador;
    }
    numeroSetores = idSetor;
    return lista_setores;
}

BOOL estaListaAdjacencias(GRAFO *grafoSdrParam, long int idNoRaiz, long int idNoAdj)
{
    int contador;
    for(contador =0; contador < grafoSdrParam[idNoRaiz].numeroAdjacentes; contador++)
    {
        if(grafoSdrParam[idNoRaiz].adjacentes[contador].idNo == idNoAdj)
            return true;
    }
    return false;
}

int posicaoBarraAdjacente(GRAFO *grafoSdrParam, long int idNoRaiz, long int idNoAdj)
{
    int contador;
    for(contador =0; contador < grafoSdrParam[idNoRaiz].numeroAdjacentes; contador++)
    {
        if(grafoSdrParam[idNoRaiz].adjacentes[contador].idNo == idNoAdj)
            return contador;
    }
    return -1;
}

BOOL estaListaAdjacenciasSetores(GRAFOSETORES *grafoSetoresSdrParam, long int idNoRaiz, long int idNoAdj, int adjacentes)
{
    int contador;
    for(contador =0; contador < adjacentes; contador++)
    {
       // printf("idNoRaiz %ld \n", idNoRaiz);
        if(grafoSetoresSdrParam[idNoRaiz].setoresAdjacentes[contador] == idNoAdj)
            return true;
    }
    return false;
}

void constroiRNP(BARRASETOR *inicioListaSetor, long int idNo, BOOL *visitado, RNPSETOR *rnpParam, int *indice, int profundidade, GRAFO *grafoSDRParam)
{
    //Atribui o no e a sua profundidade
        int idNoAdj;
        rnpParam->nos[indice[0]].idNo = idNo;
        rnpParam->nos[indice[0]].profundidade = profundidade;
        visitado[idNo] = true;
        indice[0]++;
        profundidade++;
        BARRASETOR *noSetor = inicioListaSetor;
        while((noSetor!=NULL))
        {   
                if (estaListaAdjacencias(grafoSDRParam, idNo,  noSetor->idNo) && (visitado[noSetor->idNo]== false))
                {
                    idNoAdj= noSetor->idNo;
                    constroiRNP(inicioListaSetor, idNoAdj, visitado, rnpParam, indice, profundidade,grafoSDRParam);            
                } 
            
            noSetor = noSetor->prox;
        }
}

void percorreAdjacentesSetor(BARRASETOR *setorParam, BARRASETOR *setorAdjParam, GRAFO *grafoSDRParam, RNPSETOR *rnps,  int numeroNos, long int numeroBarras)
{
    BARRASETOR *noAdjacenteSetor = setorAdjParam;
    BARRASETOR *noSetor = setorParam;
    BOOL visitado[numeroBarras+1];
    int contador;
    int contadorRNPs=0;
    //RNPSETOR *rnpSetor;
    BOOL encontrado = false;
    int idNo;
    int indiceRNP;
    int profundidade;
    while (noAdjacenteSetor != NULL)
    {
        for (contador =1; contador <=numeroBarras; contador++)
        {
            visitado[contador] = false;
        }
        
        //rnpSetor = Malloc(RNPSETOR,1);
        rnps[contadorRNPs].nos = Malloc(NORNP, (numeroNos+1));
        //Atribui o no e a sua profundidade
        rnps[contadorRNPs].nos[0].idNo = noAdjacenteSetor->idNo;
        rnps[contadorRNPs].nos[0].profundidade = 0;
        rnps[contadorRNPs].numeroNos = numeroNos+1;
        visitado[noAdjacenteSetor->idNo] = true;
        rnps[contadorRNPs].idSetorOrigem = grafoSDRParam[noAdjacenteSetor->idNo].idSetor;
        noSetor = setorParam;
        encontrado = false;
        while((noSetor!=NULL)&&(!encontrado))
        {
            int indice;
            for(indice =0; indice <grafoSDRParam[noAdjacenteSetor->idNo].numeroAdjacentes; indice++)
                if (grafoSDRParam[noAdjacenteSetor->idNo].adjacentes[indice].idNo == noSetor->idNo)
                {
                    encontrado = true;
                    idNo= noSetor->idNo;
                }
            noSetor = noSetor->prox;
        }
        //chamada da busca profundidade...
        indiceRNP = 1;
        profundidade = 1;
        constroiRNP(setorParam, idNo, visitado, &rnps[contadorRNPs], &indiceRNP, profundidade,grafoSDRParam);
        contadorRNPs++;
        noAdjacenteSetor = noAdjacenteSetor->prox;    
    }
}
void constroiRNPSetores(LISTASETORES *listaSetores, GRAFO *grafoSDRParam, RNPSETORES **rnpSetores, long int numeroBarras)
{
    if (((* rnpSetores) = (RNPSETORES *)malloc( (numeroSetores+1) * sizeof(RNPSETORES)))==NULL)
    {
        printf("Não foi possível alocar a lista de RNP de setores");
        exit(1);
    }
    LISTASETORES * setores = listaSetores;
    int idNo;
    
    while(setores != NULL)
    {
        idNo = setores->barraSetor->idNo;
        //O setor corresponde a um setor de alimentador
/*        if(grafoSDRParam[idNo].tipoNo == raizAlimentador)
        {
             (*rnpSetores)[setores->idSetor].rnps = Malloc(RNPSETOR, 1);
             (*rnpSetores)[setores->idSetor].idSetor = setores->idSetor;
             (*rnpSetores)[setores->idSetor].numeroRNPs = 1;
             (*rnpSetores)[setores->idSetor].rnps[0].nos = Malloc(NORNP, 1);
             //Atribui o no e a sua profundidade
             (*rnpSetores)[setores->idSetor].rnps[0].nos[0].idNo = idNo;
             (*rnpSetores)[setores->idSetor].rnps[0].nos[0].profundidade = 0;
             (*rnpSetores)[setores->idSetor].rnps[0].numeroNos = 1;
             (*rnpSetores)[setores->idSetor].rnps[0].idSetorOrigem = -1; //o setor alimentador não possui adjacente para origem de fluxo       
        }
        else //Não representa setor de alimentador
        {*/
           (*rnpSetores)[setores->idSetor].rnps = Malloc(RNPSETOR, (setores->numeroAdjacentes+1)); 
           (*rnpSetores)[setores->idSetor].idSetor = setores->idSetor;
           (*rnpSetores)[setores->idSetor].numeroRNPs = setores->numeroAdjacentes;
           percorreAdjacentesSetor(setores->barraSetor, setores->barraSetorAdj, grafoSDRParam, (*rnpSetores)[setores->idSetor].rnps, setores->numeroNos, numeroBarras);
        //}
   
        setores = setores->prox;
    }
}

void imprimeRNPSetores (RNPSETORES *rnpSetoresParam)
{
    int contadorRNP;
    int contadorSetor;
    int contadorNos;
    
    for(contadorSetor = 1; contadorSetor<=numeroSetores; contadorSetor++)
    {
        printf("Identificador Setor: %ld \t ", rnpSetoresParam[contadorSetor].idSetor);
        printf("Numero de RNPs %d \n", rnpSetoresParam[contadorSetor].numeroRNPs);
        for(contadorRNP=0; contadorRNP < rnpSetoresParam[contadorSetor].numeroRNPs; contadorRNP++ )
        {
            printf("\t Identificador setor origem %ld \n", rnpSetoresParam[contadorSetor].rnps[contadorRNP].idSetorOrigem);
            printf(" \t \t Nos: ");
            for(contadorNos=0; contadorNos<rnpSetoresParam[contadorSetor].rnps[contadorRNP].numeroNos; contadorNos++)
            {
                printf("\t %ld", rnpSetoresParam[contadorSetor].rnps[contadorRNP].nos[contadorNos].idNo);
            }
            printf("\n \t \t Prof: ");
            for(contadorNos=0; contadorNos<rnpSetoresParam[contadorSetor].rnps[contadorRNP].numeroNos; contadorNos++)
            {
                printf("\t %d", rnpSetoresParam[contadorSetor].rnps[contadorRNP].nos[contadorNos].profundidade);
            }
            printf("\n");
        }
    }
}

void gravaRNPSetores (RNPSETORES *rnpSetoresParam, long int numeroSetores)
{
    int contadorRNP;
    int contadorSetor;
    int contadorNos;
    FILE *arquivo = fopen("rnpsetores.dad", "w");
    for(contadorSetor = 1; contadorSetor<=numeroSetores; contadorSetor++)
    {
        fprintf(arquivo,"Identificador\tSetor:\t%ld\tRNPs\t%d\n", rnpSetoresParam[contadorSetor].idSetor, rnpSetoresParam[contadorSetor].numeroRNPs);
        for(contadorRNP=0; contadorRNP < rnpSetoresParam[contadorSetor].numeroRNPs; contadorRNP++ )
        {
            fprintf(arquivo,"Origem\t%ld\tnumero\tnos\t%d\n", rnpSetoresParam[contadorSetor].rnps[contadorRNP].idSetorOrigem, rnpSetoresParam[contadorSetor].rnps[contadorRNP].numeroNos);
            for(contadorNos=0; contadorNos<rnpSetoresParam[contadorSetor].rnps[contadorRNP].numeroNos; contadorNos++)
            {
                fprintf(arquivo,"%ld\t", rnpSetoresParam[contadorSetor].rnps[contadorRNP].nos[contadorNos].idNo);
            }
            fprintf(arquivo,"\n");
            for(contadorNos=0; contadorNos<rnpSetoresParam[contadorSetor].rnps[contadorRNP].numeroNos; contadorNos++)
            {
                fprintf(arquivo,"%d\t", rnpSetoresParam[contadorSetor].rnps[contadorRNP].nos[contadorNos].profundidade);
            }
            fprintf(arquivo,"\n");
        }
    }
    
    fclose(arquivo);
}


void imprimeRNPSetor (RNPSETOR rnpSetorParam)
{
    int contadorNos;
    
        
            printf("\t Identificador setor origem %ld \n", rnpSetorParam.idSetorOrigem);
            printf(" \t \t Nos: ");
            for(contadorNos=0; contadorNos<rnpSetorParam.numeroNos; contadorNos++)
            {
                printf("\t %ld", rnpSetorParam.nos[contadorNos].idNo);
            }
            printf("\n \t \t Prof: ");
            for(contadorNos=0; contadorNos<rnpSetorParam.numeroNos; contadorNos++)
            {
                printf("\t %d", rnpSetorParam.nos[contadorNos].profundidade);
            }
            printf("\n");
}

void constroiListaAdjacenciaSetores(GRAFO **grafoSDRParam, LISTASETORES *listaSetores, LISTACHAVES **listaChavesParam, GRAFOSETORES **grafoSetoresParam, long int *numeroChaves, long int *numeroBarras)
{
    
    //aloca a memória do grafo de setores.
    if (((*grafoSetoresParam) = (GRAFOSETORES *)malloc((numeroSetores+1)*sizeof (GRAFOSETORES)))==NULL)
    {
        printf("ERRO: alocacacao grafo da lista de adjacencias de setores!!!!");
        exit(1);
    }        
    
    BARRASETOR *barraSetorAdj;
    BARRASETOR *barrasSetor;
    LISTASETORES * setores = listaSetores;
    long int idBarraSetor;
    int contador;
    BOOL encontrado;
    //percorre a lista encadeada dos setores identificados na buscaSetores
    while(setores != NULL)
    { 
        (*grafoSetoresParam)[setores->idSetor].idSetor = setores->idSetor;
        (*grafoSetoresParam)[setores->idSetor].setorFalta = false;
        (*grafoSetoresParam)[setores->idSetor].numeroAdjacentes = setores->numeroAdjacentes;
        (*grafoSetoresParam)[setores->idSetor].numeroConsumidores = setores->consumidoresNormais;
        (*grafoSetoresParam)[setores->idSetor].numeroConsumidoresEspeciais = setores->consumidoresEspeciais;
        (*grafoSetoresParam)[setores->idSetor].setoresAdjacentes = Malloc(long int, setores->numeroAdjacentes);
        (*grafoSetoresParam)[setores->idSetor].idChavesAdjacentes = Malloc(long int, setores->numeroAdjacentes);
        barraSetorAdj = setores->barraSetorAdj;
        contador = 0;
        while (barraSetorAdj != NULL) {
            /* Explicação do if: Se a barra adjacente ao setor não for alimentada 
             * por nenhum alimentador/trafo ela não deve ser considerada como 
             * adjacente*/
            if ((*grafoSDRParam)[barraSetorAdj->idNo].idSetor != 0) {


                //localiza qual das barras do setor corresponde a adjacência
                barrasSetor = setores->barraSetor;
                encontrado = false;
              
                while (barrasSetor != NULL && !encontrado) {
                    if (estaListaAdjacencias((*grafoSDRParam), barraSetorAdj->idNo, barrasSetor->idNo)) {
                        encontrado = true;
                        idBarraSetor = barrasSetor->idNo;
                    }
                    barrasSetor = barrasSetor->prox;
                }

                if (contador > 0) {
                    //verificar se o setor pertence a lista de adjacências
                    if (estaListaAdjacenciasSetores((*grafoSetoresParam), setores->idSetor, (*grafoSDRParam)[barraSetorAdj->idNo].idSetor, contador)) {
                       // printf("setor %ld setoradj %ld \n", setores->idSetor, (*grafoSDRParam)[barraSetorAdj->idNo].idSetor);
                        criaSetorChave(grafoSetoresParam, grafoSDRParam, listaChavesParam, barraSetorAdj, idBarraSetor, listaSetores, numeroChaves, numeroBarras, buscaChave((*listaChavesParam), barraSetorAdj->idNo, idBarraSetor, numeroChaves[0]));
                    } //else {
                        //insere na lista de adjacências o id do setor adjacente da barra adjacente recuperada na busca de setores
                        (*grafoSetoresParam)[setores->idSetor].setoresAdjacentes[contador] = (*grafoSDRParam)[barraSetorAdj->idNo].idSetor;
                        //localiza a chave da adjacência
                        (*grafoSetoresParam)[setores->idSetor].idChavesAdjacentes[contador] = buscaChave((*listaChavesParam), barraSetorAdj->idNo, idBarraSetor, numeroChaves[0]);
                        //printf("1 setores %ld chave %ld \n", setores->idSetor,(*grafoSetoresParam)[setores->idSetor].idChavesAdjacentes[contador]);

                    //}

                } else {
                    //insere na lista de adjacências o id do setor adjacente da barra adjacente recuperada na busca de setores
                    (*grafoSetoresParam)[setores->idSetor].setoresAdjacentes[contador] = (*grafoSDRParam)[barraSetorAdj->idNo].idSetor;
                    //localiza a chave da adjacência
                   // printf("barras %ld %ld \n", barraSetorAdj->idNo, idBarraSetor);
                    (*grafoSetoresParam)[setores->idSetor].idChavesAdjacentes[contador] = buscaChave((*listaChavesParam), barraSetorAdj->idNo, idBarraSetor, numeroChaves[0]);
                   // printf("setores %ld chave %ld \n", setores->idSetor,(*grafoSetoresParam)[setores->idSetor].idChavesAdjacentes[contador]);
                }
                contador++;

            }
            else{
                //o adjacente identificado não pertence a um setor energizado
                (*grafoSetoresParam)[setores->idSetor].numeroAdjacentes --;
            }
            
            barraSetorAdj = barraSetorAdj->prox;
        }
        setores = setores->prox;
    }
}

void criaSetorChave(GRAFOSETORES **grafoSetoresParam, GRAFO **grafoSDRParam, LISTACHAVES **listaChavesParam, BARRASETOR *barraSetorAdj, long int barraSetor, LISTASETORES *listaSetores, long int *numeroChaves, long int *numeroBarras, long int idChave) {

    //realloca as estruturas de grafoSetores, grafoSDR e listaChaves para poder adicionar os novos elementos
    GRAFOSETORES *temporario;
    GRAFO *tempGrafo;
    LISTACHAVES *tempListaChaves;
    long int idNovoSetor, idNovaBarra, idNovaChave1, idNovaChave2, setorBarraAdj;
    char codOperacionalChave1[15], codOperacionalChave2[15];
    BOOL encontrado = false;
    LISTASETORES *lSetores = listaSetores;
    BARRASETOR *barraTemp;
    BARRASETOR *novaBarra = NULL; //(BARRASETOR *) malloc(sizeof (BARRASETOR));
    BARRASETOR *barraAdjNova =NULL;
    int posAdj;
  //  BARRASETOR *barraAdj2 = (BARRASETOR *) malloc(sizeof (BARRASETOR));
    numeroSetoresAdj = 2; 
    numeroNosSetor = 1;
    numeroConsumidores = 0; 
    numeroConsumidoresEspeciais = 0;

    temporario = (GRAFOSETORES *) realloc((*grafoSetoresParam), (numeroSetores + 2) * sizeof (GRAFOSETORES));
    // printf("usei realloc \n");
    if (temporario == NULL) {
        printf("Nao foi possível criar o setor ficticio\n");
        exit(1);
    } else {
        (*grafoSetoresParam) = temporario;
        idNovoSetor = numeroSetores + 1;
        numeroSetores++;

        tempGrafo = (GRAFO *) realloc((*grafoSDRParam), (numeroBarras[0] + 2) * sizeof (GRAFO));
        (*grafoSDRParam) = tempGrafo;
        idNovaBarra = numeroBarras[0]+1;

        //cria as barras do novo setor
        //novaBarra->idNo = idNovaBarra;
        //novaBarra->prox = NULL;
        adicionaNo(&novaBarra, idNovaBarra);
        adicionaNo(&barraAdjNova, barraSetor);
       // barraAdj1->idNo = barraSetor;
        adicionaNo(&barraAdjNova, barraSetorAdj->idNo);
        idSetor++;
      //  barraAdj2->idNo = barraSetorAdj->idNo;
      //  barraAdj1->prox = barraAdj2;
     
        numeroBarras[0]++;
        //cria a nova barra
        (*grafoSDRParam)[idNovaBarra].idNo = idNovaBarra;
        (*grafoSDRParam)[idNovaBarra].idSetor = idSetor;
        (*grafoSDRParam)[idNovaBarra].tipoNo = (*grafoSDRParam)[barraSetorAdj->idNo].tipoNo;
        (*grafoSDRParam)[idNovaBarra].valorPQ.p = (*grafoSDRParam)[barraSetorAdj->idNo].valorPQ.p;
        (*grafoSDRParam)[idNovaBarra].valorPQ.q = (*grafoSDRParam)[barraSetorAdj->idNo].valorPQ.q;
        (*grafoSDRParam)[idNovaBarra].numeroAdjacentes = 2;
        (*grafoSDRParam)[idNovaBarra].medicao = semMedidor;
        (*grafoSDRParam)[idNovaBarra].adjacentes = Malloc(NOADJACENTE, 2);


        tempListaChaves = (LISTACHAVES *) realloc((*listaChavesParam), (numeroChaves[0] + 3) * sizeof (LISTACHAVES));
        if (tempListaChaves == NULL) {
            printf("Nao foi possível alocar as novas chaves\n");
            exit(1);
        } else {
            (*listaChavesParam) = tempListaChaves;
            idNovaChave1 = numeroChaves[0] + 1;
            idNovaChave2 = numeroChaves[0] + 2;
            numeroChaves[0] += 2;
            //cria os novos codigos operacionais das duas chaves
            sprintf(codOperacionalChave1, "%sA", (*listaChavesParam)[idChave].codOperacional);
            sprintf(codOperacionalChave2, "%sB", (*listaChavesParam)[idChave].codOperacional);
            //copia os dados da chave real para as duas chaves novas
            (*listaChavesParam)[idNovaChave1].condicao = (*listaChavesParam)[idChave].condicao;
            (*listaChavesParam)[idNovaChave1].estadoChave = (*listaChavesParam)[idChave].estadoChave;
            (*listaChavesParam)[idNovaChave1].idNoDe = barraSetor;
            (*listaChavesParam)[idNovaChave1].idNoPara = idNovaBarra;
            (*listaChavesParam)[idNovaChave1].identificador = idNovaChave1;
            (*listaChavesParam)[idNovaChave1].subTipoChave = (*listaChavesParam)[idChave].subTipoChave;
            (*listaChavesParam)[idNovaChave1].tipoChave = (*listaChavesParam)[idChave].tipoChave;
            strcpy((*listaChavesParam)[idNovaChave1].codOperacional, codOperacionalChave1);

            (*listaChavesParam)[idNovaChave2].condicao = (*listaChavesParam)[idChave].condicao;
            (*listaChavesParam)[idNovaChave2].estadoChave = normalmenteFechada;
            (*listaChavesParam)[idNovaChave2].idNoDe = idNovaBarra;
            (*listaChavesParam)[idNovaChave2].idNoPara = barraSetorAdj->idNo;
            (*listaChavesParam)[idNovaChave2].identificador = idNovaChave2;
            (*listaChavesParam)[idNovaChave2].subTipoChave = (*listaChavesParam)[idChave].subTipoChave;
            (*listaChavesParam)[idNovaChave2].tipoChave = (*listaChavesParam)[idChave].tipoChave;
            strcpy((*listaChavesParam)[idNovaChave2].codOperacional, codOperacionalChave2);

            //primeiro adjacente
            (*grafoSDRParam)[idNovaBarra].adjacentes[0].ampacidade = 999;
            (*grafoSDRParam)[idNovaBarra].adjacentes[0].condicao = (*listaChavesParam)[idChave].condicao;
            (*grafoSDRParam)[idNovaBarra].adjacentes[0].estadoChave = (*listaChavesParam)[idChave].estadoChave;
            strcpy((*grafoSDRParam)[idNovaBarra].adjacentes[0].idAresta, (*listaChavesParam)[idNovaChave1].codOperacional);
            (*grafoSDRParam)[idNovaBarra].adjacentes[0].idNo = barraSetor;
            (*grafoSDRParam)[idNovaBarra].adjacentes[0].reatancia = 0;
            (*grafoSDRParam)[idNovaBarra].adjacentes[0].resistencia = 0;
            (*grafoSDRParam)[idNovaBarra].adjacentes[0].medicao = semMedidor;
            (*grafoSDRParam)[idNovaBarra].adjacentes[0].subTipoAresta = (*listaChavesParam)[idChave].subTipoChave;
            (*grafoSDRParam)[idNovaBarra].adjacentes[0].tipoAresta = (*listaChavesParam)[idChave].tipoChave;

            //segunda barra adjacente
            (*grafoSDRParam)[idNovaBarra].adjacentes[1].ampacidade = 999;
            (*grafoSDRParam)[idNovaBarra].adjacentes[1].condicao = (*listaChavesParam)[idChave].condicao;
            (*grafoSDRParam)[idNovaBarra].adjacentes[1].estadoChave = (*listaChavesParam)[idChave].estadoChave;
            strcpy((*grafoSDRParam)[idNovaBarra].adjacentes[1].idAresta, (*listaChavesParam)[idNovaChave2].codOperacional);
            (*grafoSDRParam)[idNovaBarra].adjacentes[1].idNo = barraSetorAdj->idNo;
            (*grafoSDRParam)[idNovaBarra].adjacentes[1].reatancia = 0;
            (*grafoSDRParam)[idNovaBarra].adjacentes[1].resistencia = 0;
            (*grafoSDRParam)[idNovaBarra].adjacentes[1].medicao = semMedidor;
            (*grafoSDRParam)[idNovaBarra].adjacentes[1].subTipoAresta = (*listaChavesParam)[idChave].subTipoChave;
            (*grafoSDRParam)[idNovaBarra].adjacentes[1].tipoAresta = (*listaChavesParam)[idChave].tipoChave;
          //atualiza a adjacencia das barras existentes
            posAdj =  posicaoBarraAdjacente((*grafoSDRParam), barraSetor, barraSetorAdj->idNo);
           (*grafoSDRParam)[barraSetor].adjacentes[posAdj].idNo = idNovaBarra;
           strcpy((*grafoSDRParam)[barraSetor].adjacentes[posAdj].idAresta, (*listaChavesParam)[idNovaChave1].codOperacional);
           posAdj =  posicaoBarraAdjacente((*grafoSDRParam), barraSetorAdj->idNo, barraSetor);
           (*grafoSDRParam)[barraSetorAdj->idNo].adjacentes[posAdj].idNo = idNovaBarra;
           strcpy((*grafoSDRParam)[barraSetorAdj->idNo].adjacentes[posAdj].idAresta, (*listaChavesParam)[idNovaChave2].codOperacional); 
            
            setorBarraAdj = (*grafoSDRParam)[barraSetorAdj->idNo].idSetor;

            

            //criar o novo setor na lista de setores e atualiza as barras nos setores atuais
            barraSetorAdj->idNo = idNovaBarra;

            //localiza na lista de setores o setor adjacente para trocar a barra adjacente dele pela nova Barra
            while (lSetores != NULL && !encontrado) {
                if (lSetores->idSetor == setorBarraAdj) {
                    encontrado = true;
                } else {
                    lSetores = lSetores->prox;
                }
            }

            if (encontrado) {
                //percorre a lista de chaves adjacentes do setor para localizar a chave entre os dois setores reais
                barraTemp = lSetores->barraSetorAdj;
                encontrado = false;
                while (barraTemp != NULL && !encontrado) {
                    if (barraTemp->idNo == barraSetor) {
                        barraTemp->idNo = idNovaBarra;
                        encontrado = true;
                    }
                    barraTemp = barraTemp->prox;
                }
            }
            
            adicionaSetorNaLista(&listaSetores, novaBarra, barraAdjNova);
        }
    }
}


void imprimeGrafoSetores(GRAFOSETORES *grafoSetoresParam)
{
    int contador;
    int contadorAdj;
    for(contador = 1; contador<=numeroSetores; contador++)
    {
        printf("Setor %ld \n", grafoSetoresParam[contador].idSetor);
        printf("\t Adjacentes");
        for(contadorAdj = 0; contadorAdj<grafoSetoresParam[contador].numeroAdjacentes; contadorAdj++)
        {
            printf("\t %ld", grafoSetoresParam[contador].setoresAdjacentes[contadorAdj]);
        }
        printf("\n \t Chaves");
        for(contadorAdj = 0; contadorAdj<grafoSetoresParam[contador].numeroAdjacentes; contadorAdj++)
        {
            printf("\t %ld", grafoSetoresParam[contador].idChavesAdjacentes[contadorAdj]);
        }
        printf("\n");    
    }
    
}

void gravaGrafoSetores(GRAFOSETORES *grafoSetoresParam, long int numeroSetores, LISTACHAVES *listaChavesParam)
{
    int contador;
    int contadorAdj;
    FILE *arquivo = fopen("grafosetores.dad", "w");
    fprintf(arquivo,"numero\tsetores\t%ld\n",numeroSetores);
    for(contador = 1; contador<=numeroSetores; contador++)
    {
        fprintf(arquivo,"Setor\t%ld\tnumero\tadjacentes\t%d\nConsumidores:\t%ld\tConsumidores\tEspeciais:\t%ld\n", grafoSetoresParam[contador].idSetor,grafoSetoresParam[contador].numeroAdjacentes, grafoSetoresParam[contador].numeroConsumidores, grafoSetoresParam[contador].numeroConsumidoresEspeciais);
        fprintf(arquivo,"Adjacentes");
        for(contadorAdj = 0; contadorAdj<grafoSetoresParam[contador].numeroAdjacentes; contadorAdj++)
        {
            fprintf(arquivo,"\t %ld", grafoSetoresParam[contador].setoresAdjacentes[contadorAdj]);
        }
        fprintf(arquivo,"\nChaves");
        for(contadorAdj = 0; contadorAdj<grafoSetoresParam[contador].numeroAdjacentes; contadorAdj++)
        {
            fprintf(arquivo,"\t %ld", grafoSetoresParam[contador].idChavesAdjacentes[contadorAdj]);
        }
        fprintf(arquivo,"\nCodOp. Chaves");
        for(contadorAdj = 0; contadorAdj<grafoSetoresParam[contador].numeroAdjacentes; contadorAdj++)
        {
            fprintf(arquivo,"\t %s", listaChavesParam[grafoSetoresParam[contador].idChavesAdjacentes[contadorAdj]].codOperacional);
        }
        fprintf(arquivo,"\n");
    }
    fclose(arquivo);
}

//realiza a busca a rnp do idsetor1 sendo fornecida pelo idSetor2 e retorna essa rnp
RNPSETOR buscaRNPSetor(RNPSETORES *rnpSetoresParam, long int idSetor1, long int idSetor2)
{
    int indice = 0;
    //realiza a busca pela rnp
    //printf("setor1 %ld setor2 %ld numeroRNPS %d\n", idSetor1, idSetor2,rnpSetoresParam[idSetor1].numeroRNPs);
    while (indice < rnpSetoresParam[idSetor1].numeroRNPs && rnpSetoresParam[idSetor1].rnps[indice].idSetorOrigem != idSetor2)
        indice++;
    if (indice < rnpSetoresParam[idSetor1].numeroRNPs)
        return rnpSetoresParam[idSetor1].rnps[indice];
}

//void desalocaListaSetores()
