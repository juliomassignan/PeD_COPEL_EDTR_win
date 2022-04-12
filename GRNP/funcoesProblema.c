#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "funcoesProblema.h"
#include "funcoesRNP.h"
#include "funcoesSetor.h"
#include "funcoesLeituraDados.h"
#include "funcoesInicializacao.h"


//definição da constante ij em número complexo
//__complex__ double ij = 1.0fI;
BOOL flagMudancaTap = false;
FILE *arquivog;
/**
 * Recebe como parametro o grafo de setores, o identificador do setor, a lista de chaves
 *  e retorna somente os setores ligados por chaves automáticas.
 * 
 * @param grafoSetoresParam
 * @param idNoSetorParam
 * @param listaChavesParam
 * @return 
 
GRAFOSETORES *listaSetoresAutomatica(GRAFOSETORES *grafoSetoresParam, long int idNoSetorParam, LISTACHAVES *listaChavesParam) {
    int contador;
    int indice;
    long int idChave;
    GRAFOSETORES *listaAutomaticas;
    listaAutomaticas = (GRAFOSETORES *) malloc(sizeof (GRAFOSETORES));

    listaAutomaticas[0].idSetor = grafoSetoresParam[idNoSetorParam].idSetor;
    listaAutomaticas[0].idChavesAdjacentes = Malloc(long int, grafoSetoresParam[idNoSetorParam].numeroAdjacentes);
    listaAutomaticas[0].setoresAdjacentes = Malloc(long int, grafoSetoresParam[idNoSetorParam].numeroAdjacentes);
    listaAutomaticas[0].numeroAdjacentes = 0;
    indice = 0;
    for (contador = 0; contador < grafoSetoresParam[idNoSetorParam].numeroAdjacentes; contador++) {
        idChave = grafoSetoresParam[idNoSetorParam].idChavesAdjacentes[contador];
        if (listaChavesParam[idChave].tipoChave == chaveAutomatica) {
            listaAutomaticas[0].idChavesAdjacentes[indice] = grafoSetoresParam[idNoSetorParam].idChavesAdjacentes[contador];
            listaAutomaticas[0].setoresAdjacentes[indice] = grafoSetoresParam[idNoSetorParam].setoresAdjacentes[contador];
            indice++;
            listaAutomaticas[0].numeroAdjacentes++;
        }
    }
    return listaAutomaticas;
}*/

/**
 * Método que percorre o grafo que representa o sis54tema de distribuição e monta a matriz de impedâncias Z utilizada no 
 * cálculo do fluxo de potência. Para isso faz uso de uma estrutura de forma a armazenar a matriz como matriz esparsa.
 * Além disso faz a alocação de memória da matriz
 * Tem como parâmetros o grafo do sistema, a matriz Z e o número de barras do sistema.
 * 
 * @param grafoSDRParam
 * @param ZParam
 * @param numeroBarrasParam
 
void constroiMatrizZ(GRAFO *grafoSDRParam, MATRIZCOMPLEXA **ZParam, long int numeroBarrasParam) {
    int indiceNos;
    int indiceAdjacentes;

    //aloca a memória para a matriz Z 
    if (((*ZParam) = (MATRIZCOMPLEXA *) malloc((numeroBarrasParam + 1) * sizeof (MATRIZCOMPLEXA))) == NULL) {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para a matriz Z !!!!");
        exit(1);
    }
    //percorre a lista de barras do sistema no grafo
    for (indiceNos = 1; indiceNos <= numeroBarrasParam; indiceNos++) {
        //inclui os valores na lista inicial da matriz Z
        (*ZParam)[indiceNos].idNo = grafoSDRParam[indiceNos].idNo;
        (*ZParam)[indiceNos].numeroAdjacentes = grafoSDRParam[indiceNos].numeroAdjacentes;
        //aloca o vetor de adjacente do tindicePo celulacomplexa para conter os valores necessários
        (*ZParam)[indiceNos].noAdjacentes = (CELULACOMPLEXA *) malloc(grafoSDRParam[indiceNos].numeroAdjacentes * sizeof (CELULACOMPLEXA));
        //percorre a lista de adjacentes calculando a impedância
        for (indiceAdjacentes = 0; indiceAdjacentes < grafoSDRParam[indiceNos].numeroAdjacentes; indiceAdjacentes++) {
            (*ZParam)[indiceNos].noAdjacentes[indiceAdjacentes].idNo = grafoSDRParam[indiceNos].adjacentes[indiceAdjacentes].idNo;
            (*ZParam)[indiceNos].noAdjacentes[indiceAdjacentes].valor = grafoSDRParam[indiceNos].adjacentes[indiceAdjacentes].resistencia + ij * grafoSDRParam[indiceNos].adjacentes[indiceAdjacentes].reatancia;
        }
    }

}*/

void imprimeMatrizZ(MATRIZCOMPLEXA *ZParam, long int numeroBarrasParam) {
    long int indiceNos;
    int indiceAdjacentes;

    //percorre a lista de barras do sistema no grafo
    for (indiceNos = 1; indiceNos <= numeroBarrasParam; indiceNos++) {
        //inclui os valores na lista inicial da matriz Z
         printf("Barra %ld -- adjacentes \n ", ZParam[indiceNos].idNo); 
       //aloca o vetor de adjacente do tindicePo celulacomplexa para conter os valores necessários
        //percorre a lista de adjacentes calculando a impedância
        for (indiceAdjacentes = 0; indiceAdjacentes < ZParam[indiceNos].numeroAdjacentes; indiceAdjacentes++) {
           printf("barra adj %ld: resistencia %.10lf reatancia %.10lf \t", ZParam[indiceNos].noAdjacentes[indiceAdjacentes].idNo, 
                   __real__ ZParam[indiceNos].noAdjacentes[indiceAdjacentes].valor, __imag__ ZParam[indiceNos].noAdjacentes[indiceAdjacentes].valor);
        }
         printf("\n\n");
    }

}

/**
 * Tendo como parâmetro a matriz Z, e as barras da linha, retorna o valor da impedância.
 * @param ZParam
 * @param idNo1
 * @param idNo2
 * @return 
 */
__complex__ double valorZ(MATRIZCOMPLEXA *ZParam, long int idNo1, long int idNo2) {
    int indiceAdjacentes = 0;

    while (indiceAdjacentes < ZParam[idNo1].numeroAdjacentes) {
        if (ZParam[idNo1].noAdjacentes[indiceAdjacentes].idNo == idNo2)
            return ZParam[idNo1].noAdjacentes[indiceAdjacentes].valor;
        indiceAdjacentes++;
    }
}

/**
 * Método para contruir a matriz que contém o máximo de corrente da linha.
 * Para isso faz uso de uma estrutura de forma a armazenar a matriz como matriz esparsa.
 * Além disso faz a alocação da estrutura.
 * Tem como parâmetros o grafo do sistema, a matriz de maximo de corrente e o número de barras
 * 
 * @param grafoSDRParam
 * @param maximoCorrenteParam
 * @param numeroBarrasParam

void constroiMatrizMaximoCorrente(GRAFO *grafoSDRParam, MATRIZMAXCORRENTE **maximoCorrenteParam, long int numeroBarrasParam) {
    int indiceNos;
    int indiceAdjacentes;
    //realiza a alocação de memória. A seguinte construção foi utilizada (*maximoCorrenteParam) pois o método deve alterar o valor do 
    // ponteiro da estrutura.
    if (((*maximoCorrenteParam) = (MATRIZMAXCORRENTE *) malloc((numeroBarrasParam + 1) * sizeof (MATRIZMAXCORRENTE))) == NULL) {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para a matriz de máximo de corrente !!!!");
        exit(1);
    }
    //percorre o grafo e seus adjacentes preenchendo os valores da matriz
    for (indiceNos = 1; indiceNos <= numeroBarrasParam; indiceNos++) {
        (*maximoCorrenteParam)[indiceNos].idNo = grafoSDRParam[indiceNos].idNo;
        (*maximoCorrenteParam)[indiceNos].numeroAdjacentes = grafoSDRParam[indiceNos].numeroAdjacentes;
        //aloca o espaço para os dados dos adjacentes.
        (*maximoCorrenteParam)[indiceNos].noAdjacentes = (CELULACORRENTE *) malloc(grafoSDRParam[indiceNos].numeroAdjacentes * sizeof (CELULACORRENTE));
        //percorre a lista de adjacentes preenchendo os valores de máximo de corrente
        for (indiceAdjacentes = 0; indiceAdjacentes < grafoSDRParam[indiceNos].numeroAdjacentes; indiceAdjacentes++) {
            (*maximoCorrenteParam)[indiceNos].noAdjacentes[indiceAdjacentes].idNo = grafoSDRParam[indiceNos].adjacentes[indiceAdjacentes].idNo;
            (*maximoCorrenteParam)[indiceNos].noAdjacentes[indiceAdjacentes].valor = grafoSDRParam[indiceNos].adjacentes[indiceAdjacentes].ampacidade;
        }
    }
}*/

/**
 * Dados como parâmetro a matriz de correntes e as barras da linha, recupera o valor do máximo de corrente
 * @param maximoCorrenteParam
 * @param idNo1
 * @param idNo2
 * @return 
 */
double maximoCorrente(MATRIZMAXCORRENTE *maximoCorrenteParam, long int idNo1, long int idNo2) {
    int indiceAdjacentes = 0;

    while (indiceAdjacentes < maximoCorrenteParam[idNo1].numeroAdjacentes) {
        if (maximoCorrenteParam[idNo1].noAdjacentes[indiceAdjacentes].idNo == idNo2)
            return maximoCorrenteParam[idNo1].noAdjacentes[indiceAdjacentes].valor;
        indiceAdjacentes++;
    }

}

//verifica se entre as barras informadas existe um regulador de tensão
BOOL eRegulador(long int barra1, long int barra2, GRAFO *grafoSDRParam)
{
    BOOL encontrado = false;
    BOOL resultado = false;
    int contadorAdj = 0;
    
    while(contadorAdj < grafoSDRParam[barra1].numeroAdjacentes && !encontrado)
    {
        if(grafoSDRParam[barra1].adjacentes[contadorAdj].idNo == barra2)
        {
            encontrado = true;
            if(grafoSDRParam[barra1].adjacentes[contadorAdj].tipoAresta == reguladorTensao)
                resultado = true;
        }
    }
    
    return resultado;
}

void correnteJusante(int idRNP, int carga, int iteracao,
        CONFIGURACAO configuracaoParam, RNPSETORES *matrizB, int *indiceRegulador, DADOSREGULADOR *dadosRegulador) {
    long int indice, indice1, noS, noR, noN, idSetorS, idSetorR, idBarra1, idBarra2, indice2, indice3;
    double IMod, IAng;
    long int noProf[200]; //armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
   __complex__ double iAcumulada;
    RNPSETOR rnpSetorSR;
    //calculo das correntes

    //determina o modelo de calculo
    if (iteracao != 0) //se nao é a primeira vez que calcula o módulo e o ângulo 
    {
        indice = 0;
        noProf[configuracaoParam.rnp[idRNP].nos[indice].profundidade] = configuracaoParam.rnp[idRNP].nos[indice].idNo;
        for (indice = 1; indice < configuracaoParam.rnp[idRNP].numeroNos; indice++) {
            noS = configuracaoParam.rnp[idRNP].nos[indice].idNo;
            noR = noProf[configuracaoParam.rnp[idRNP].nos[indice].profundidade - 1];
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
            for (indice1 = 0; indice1 < rnpSetorSR.numeroNos; indice1++) {
                noN = rnpSetorSR.nos[indice1].idNo;
                
                if (carga == 0) //modelo de corrente constante
                {


                    IMod = cabs(configuracaoParam.dadosEletricos.corrente[noN]);
                    IAng = carg(configuracaoParam.dadosEletricos.vBarra[noN]) - carg(configuracaoParam.dadosEletricos.potencia[noN]);
                    configuracaoParam.dadosEletricos.corrente[noN] = IMod * cos(IAng) + I * IMod * sin(IAng);

                } else //modelo de potencia constante
                {
                    configuracaoParam.dadosEletricos.corrente[noN] = conj(configuracaoParam.dadosEletricos.potencia[noN] / configuracaoParam.dadosEletricos.vBarra[noN]);
                }
            }
            noProf[configuracaoParam.rnp[idRNP].nos[indice].profundidade] = configuracaoParam.rnp[idRNP].nos[indice].idNo;
        }
    }
        

    for (indice = configuracaoParam.rnp[idRNP].numeroNos - 1; indice > 0; indice--) {
        indice2 = indice - 1;
        //busca pelo nó raiz
        while (indice2 >= 0 && configuracaoParam.rnp[idRNP].nos[indice2].profundidade != (configuracaoParam.rnp[idRNP].nos[indice].profundidade - 1))
            indice2--;
        idSetorS = configuracaoParam.rnp[idRNP].nos[indice].idNo;
        idSetorR = configuracaoParam.rnp[idRNP].nos[indice2].idNo;
        rnpSetorSR = buscaRNPSetor(matrizB, idSetorS, idSetorR);

        for (indice2 = 0; indice2 < rnpSetorSR.numeroNos; indice2++) {//zera as variáveias iJusante
            idBarra1 = rnpSetorSR.nos[indice2].idNo;
            configuracaoParam.dadosEletricos.iJusante[idBarra1] = 0 + I * 0;
        }
    }
    for (indice = configuracaoParam.rnp[idRNP].numeroNos - 1; indice > 0; indice--) {
        indice2 = indice - 1;
        //busca pelo nó raiz
        while (indice2 >= 0 && configuracaoParam.rnp[idRNP].nos[indice2].profundidade != (configuracaoParam.rnp[idRNP].nos[indice].profundidade - 1))
            indice2--;
        idSetorS = configuracaoParam.rnp[idRNP].nos[indice].idNo;
        idSetorR = configuracaoParam.rnp[idRNP].nos[indice2].idNo;
        rnpSetorSR = buscaRNPSetor(matrizB, idSetorS, idSetorR);
        for (indice2 = rnpSetorSR.numeroNos - 1; indice2 > 0; indice2--) {
            indice3 = indice2 - 1;
            while (indice3 >= 0 && rnpSetorSR.nos[indice3].profundidade != (rnpSetorSR.nos[indice2].profundidade - 1)) {
                indice3--;
            }
            idBarra1 = rnpSetorSR.nos[indice3].idNo;
            idBarra2 = rnpSetorSR.nos[indice2].idNo;
            iAcumulada = configuracaoParam.dadosEletricos.iJusante[idBarra2] + configuracaoParam.dadosEletricos.corrente[idBarra2];
           // printf("corrente barra %ld %.4lf j%.4lf \n", idBarra2+23,__real__ configuracaoParam.dadosEletricos.corrente[idBarra2],__imag__ configuracaoParam.dadosEletricos.corrente[idBarra2]);
            
           // printf("corrente ramo %ld--%ld %.4lf j%.4lf \n",idBarra1+23, idBarra2+23,__real__ iAcumulada,__imag__ iAcumulada);
            configuracaoParam.dadosEletricos.iJusante[idBarra1] = configuracaoParam.dadosEletricos.iJusante[idBarra1] + iAcumulada;         
            
           // if (eRegulador(idBarra1, idBarra2,)) //existe um regulador de tensão entre as barras 1 e 2.
            if((indiceRegulador[idBarra1]!=0) && (indiceRegulador[idBarra2]!=0) && (indiceRegulador[idBarra1]==indiceRegulador[idBarra2])){
                                                                                                     
                configuracaoParam.dadosEletricos.iJusante[idBarra1] = configuracaoParam.dadosEletricos.iJusante[idBarra1]*(1 + 0.00625 * dadosRegulador[indiceRegulador[idBarra1]].numeroTaps);
            }
   
        }

    }
}

void calculaNumeroMaximoTaps(double carregamentoRT, DADOSREGULADOR *dadosReguladorSDR, long int regulador) {
    //*********************************************************************************************
    // Faz as condicoes para a Limitacao do tap em funcao do carregamento do RT (Tabela fornecida pelo Marcel - Copel)
    if (carregamentoRT < 1) { // Se o carregamento for menor que 100% o limite eh 16 (Quantidade maxima de taps em um RT)
        if (dadosReguladorSDR[regulador].numeroTaps > 16)
            dadosReguladorSDR[regulador].numeroTaps = 16;
        if (dadosReguladorSDR[regulador].numeroTaps < -16)
            dadosReguladorSDR[regulador].numeroTaps = -16;
    } else {
        if (carregamentoRT < 1.1) { // Se o carregamento estiver entre 100% e 110% o limite eh 14. (O tap comeca a ser limitado nao pela quantidade do RT mas sim em funcao da corrente que passa pelo RT)
            if (dadosReguladorSDR[regulador].numeroTaps > 14)
                dadosReguladorSDR[regulador].numeroTaps = 14;
            if (dadosReguladorSDR[regulador].numeroTaps < -14)
                dadosReguladorSDR[regulador].numeroTaps = -14;
        } else {
            if (carregamentoRT < 1.2) { // Se o carregamento estiver entre 110% e 120% o limite eh 12.
                if (dadosReguladorSDR[regulador].numeroTaps > 12)
                    dadosReguladorSDR[regulador].numeroTaps = 12;
                if (dadosReguladorSDR[regulador].numeroTaps < -12)
                    dadosReguladorSDR[regulador].numeroTaps = -12;
            } else {

                if (carregamentoRT < 1.35) { // Se o carregamento estiver entre 120% e 135% o limite eh 10.
                    if (dadosReguladorSDR[regulador].numeroTaps > 10)
                        dadosReguladorSDR[regulador].numeroTaps = 10;
                    if (dadosReguladorSDR[regulador].numeroTaps < -10)
                        dadosReguladorSDR[regulador].numeroTaps = -10;
                } else {

                    if (carregamentoRT > 1.35 && carregamentoRT < 1.6) { // Se o carregamento estiver entre 135% e 160% o limite eh 8.
                        if (dadosReguladorSDR[regulador].numeroTaps > 8)
                            dadosReguladorSDR[regulador].numeroTaps = 8;
                        if (dadosReguladorSDR[regulador].numeroTaps < -8)
                            dadosReguladorSDR[regulador].numeroTaps = -8;
                    } else { //o carregamento é maior do que 1.6
                            dadosReguladorSDR[regulador].numeroTaps = 0;
                    }
                }
            }
        }
    }
}
//calcula a tensão em cada barra, a menor tensão das barras e retorna a barra com menor tensão

long int tensaoQuedaTensao(int indiceRNP, CONFIGURACAO configuracaoParam, double VFParam, RNPSETORES *matrizB,
        MATRIZCOMPLEXA *ZParam, int *indiceRegulador, DADOSREGULADOR *dadosReguladorSDR) {
    int indice, indice1;
    long int noS, noR, noM, noN;
    int pior;
    int tapAnterior;
    double limiteInferior = 13491; // Tensao minima (tensao de linha - V) em uma barra regulada pelo RT;
    double limiteSuperior = 13763; // Tensao maxima (tensao de linha - V) em uma barra regulada pelo RT;
    double carregamentoRT;
    double aux;

    long int noProf[200]; //armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
    long int barraProf[100]; //armazena o ultimo nó do setor presente em uma profundidade, é indexado pela profundidade
    __complex__ double deltaV, valorz;
    double temporario;
    int tensao;
    double menorTensao;
    RNPSETOR rnpSetorSR;
    noProf[configuracaoParam.rnp[indiceRNP].nos[0].profundidade] = configuracaoParam.rnp[indiceRNP].nos[0].idNo;
    menorTensao = VFParam;
    flagMudancaTap = false;
    //printf("menorTensao %lf \n", menorTensao);
    for (indice = 1; indice < configuracaoParam.rnp[indiceRNP].numeroNos; indice++) {
        noS = configuracaoParam.rnp[indiceRNP].nos[indice].idNo;
        noR = noProf[configuracaoParam.rnp[indiceRNP].nos[indice].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
        barraProf[rnpSetorSR.nos[0].profundidade] = rnpSetorSR.nos[0].idNo;
        //printf("Setor noS %ld noR %ld \t", noS, noR);
        //  imprimeRNPSetor(rnpSetorSR);
        for (indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {
            noM = barraProf[rnpSetorSR.nos[indice1].profundidade - 1];
            noN = rnpSetorSR.nos[indice1].idNo;
            if (indiceRegulador[noM] != 0 && indiceRegulador[noN] != 0) //ha um regulador de tensao entre as barras m e n.
            {
                configuracaoParam.dadosEletricos.vBarra[noN] = configuracaoParam.dadosEletricos.vBarra[noM]*(1 + passoRegulador * dadosReguladorSDR[indiceRegulador[noM]].numeroTaps);
                tensao = (int) cabs(configuracaoParam.dadosEletricos.vBarra[noN] * sqrt(3));
                tapAnterior = dadosReguladorSDR[indiceRegulador[noM]].numeroTaps; // Grava o estado do tap antes das analises


                if (tensao > limiteSuperior || tensao < limiteInferior) { // Analises para a mudanca ou nao do tap

                    if (tensao > limiteSuperior) {
                        aux = limiteSuperior / (configuracaoParam.dadosEletricos.vBarra[noM] * sqrt(3)) - 1;

                        // Calcula os novos taps:
                        dadosReguladorSDR[indiceRegulador[noM]].numeroTaps = (int) floor(aux / passoRegulador); //arredondamento para baixo

                        // Calcula o carregamento do RT:
                        carregamentoRT = cabs(configuracaoParam.dadosEletricos.iJusante[noM]) / dadosReguladorSDR[indiceRegulador[noM]].ampacidade;

                        calculaNumeroMaximoTaps(carregamentoRT, dadosReguladorSDR, indiceRegulador[noM]);
                        //*********************************************************************************************

                        //    		  printf("tap[%d] = %d\n",indreg[m],regulador[indreg[m]].tap);
                        configuracaoParam.dadosEletricos.vBarra[noN] = configuracaoParam.dadosEletricos.vBarra[noM]*(1 + passoRegulador * dadosReguladorSDR[indiceRegulador[noM]].numeroTaps);
                    }
                } else {

                    if (tensao < limiteInferior) {
                        aux = limiteInferior / (configuracaoParam.dadosEletricos.vBarra[noM] * sqrt(3)) - 1;

                        // Calcula os novos taps:
                        dadosReguladorSDR[indiceRegulador[noM]].numeroTaps = (int) ceil(aux / passoRegulador); //arredondamento para cima

                        // Calcula o carregamento do RT:
                        carregamentoRT = cabs(configuracaoParam.dadosEletricos.iJusante[noM]) / dadosReguladorSDR[indiceRegulador[noM]].ampacidade;

                        calculaNumeroMaximoTaps(carregamentoRT, dadosReguladorSDR, indiceRegulador[noM]);

                        configuracaoParam.dadosEletricos.vBarra[noN] = configuracaoParam.dadosEletricos.vBarra[noM]*(1 + passoRegulador * dadosReguladorSDR[indiceRegulador[noM]].numeroTaps);
                    }
                }

                // Condicao para verificar se o tap foi alterado ou nao
                if (tapAnterior != dadosReguladorSDR[indiceRegulador[noM]].numeroTaps) {
                    flagMudancaTap = true;
                }

            } else {
                deltaV = valorZ(ZParam, noM, noN) * (configuracaoParam.dadosEletricos.iJusante[noN] + configuracaoParam.dadosEletricos.corrente[noN]);
                configuracaoParam.dadosEletricos.vBarra[noN] = configuracaoParam.dadosEletricos.vBarra[noM] - deltaV;
            }
            /*#########################################################
                #################    Queda de Tensão    #################*/
            temporario = sqrt(pow(__real__ configuracaoParam.dadosEletricos.vBarra[noN], 2) + pow(__imag__ configuracaoParam.dadosEletricos.vBarra[noN], 2));
            if (temporario < menorTensao) {
                menorTensao = temporario;
                pior = noN;
            }
            //armazena o nó barra na sua profundidade
            barraProf[rnpSetorSR.nos[indice1].profundidade] = rnpSetorSR.nos[indice1].idNo;
        }
        //armazena o nó setor na sua profundidade
        noProf[configuracaoParam.rnp[indiceRNP].nos[indice].profundidade] = configuracaoParam.rnp[indiceRNP].nos[indice].idNo;
    }
    configuracaoParam.rnp[indiceRNP].fitnessRNP.menorTensao = menorTensao;
    // printf("\n");
    return (pior);
}

void carregamentoPerdas(CONFIGURACAO *configuracaoParam, int indiceRNP, RNPSETORES *matrizB,
        MATRIZCOMPLEXA *ZParam, MATRIZMAXCORRENTE *maximoCorrenteParam) {
    int indice, indice1;
    long int noS, noR, noN, noM;
    double temporario;
    double perda = 0;
    __complex__ double ijus, ibarra;
    RNPSETOR rnpSetorSR;
    double perdas = 0;
    __complex__ double perdasReativas, valorz;
    double carregamentoRede = 0;
    __complex__ double carregamentoAlimentador = 0 + I * 0;
    
    
    //fprintf(arquivog, "Alimentador %d \n", indiceRNP+1);
    temporario = 0;
    ijus = 0;
    ibarra = 0;

    long int noProf[200]; //armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
    long int barraProf[100]; //armazena o ultimo nó do setor presente em uma profundidade, é indexado pela profundidade
    indice = 0;
    noProf[configuracaoParam->rnp[indiceRNP].nos[indice].profundidade] = configuracaoParam->rnp[indiceRNP].nos[indice].idNo;
    for (indice = 1; indice < configuracaoParam->rnp[indiceRNP].numeroNos; indice++) {
        noS = configuracaoParam->rnp[indiceRNP].nos[indice].idNo;
        noR = noProf[configuracaoParam->rnp[indiceRNP].nos[indice].profundidade - 1];
        rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
        indice1 = 0;
        barraProf[rnpSetorSR.nos[indice1].profundidade] = rnpSetorSR.nos[indice1].idNo;
        for (indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {
            noM = barraProf[rnpSetorSR.nos[indice1].profundidade - 1];
            noN = rnpSetorSR.nos[indice1].idNo;

            /*#########################################################
             * ############    Carregamento máximo da rede (em corrente)   ##########*/
            ijus = configuracaoParam->dadosEletricos.iJusante[noN];
            ibarra = configuracaoParam->dadosEletricos.corrente[noN];
            temporario = cabs(ijus + ibarra) / (maximoCorrente(maximoCorrenteParam, noM, noN));
            
          //  fprintf(arquivog,"BarraN %ld corrente %lf Ramo %ld--%ld corrente jusante %lf soma %lf carregamento %lf \n", noN, cabs(ibarra), noM, noN, cabs(ijus), cabs(ijus + ibarra), temporario*100);
            
       
            if (temporario > carregamentoRede) {
                carregamentoRede = temporario;
                //printf("noM %ld noN %ld %lf\n", noM, noN, temporario);
            }
            /*#########################################################
             * 	#####################      Perdas     #### ##############*/
            valorz = valorZ(ZParam, noM, noN);
           // 
            perda = (3 * (pow((cabs(ijus + ibarra)), 2))) * __real__ valorz;
           // printf("ramo %ld--%ld corrente %lf corrente jusante %lf soma %lf valorZ %lf perda %lf \n", noM+23, noN+23, cabs(ibarra), cabs(ijus), cabs(ijus + ibarra), __real__ valorz, perda);
            perdas = perdas + perda;
            //  printf("perda %.2lf perdas %lf \n", perda/1000, perdas/1000);
            perdasReativas = I * 3 * pow((cabs(ijus + ibarra)), 2) * __imag__ valorZ(ZParam, noM, noN);
            /*#########################################################
              ###########    Carregamento dos alimentadores (em potência)   ##########*/
            carregamentoAlimentador = carregamentoAlimentador + (configuracaoParam->dadosEletricos.vBarra[noN] * conj(configuracaoParam->dadosEletricos.corrente[noN]))*3;

            __real__ carregamentoAlimentador += perda;

            carregamentoAlimentador += perdasReativas;
            /*##########################################################*/
            barraProf[rnpSetorSR.nos[indice1].profundidade] = rnpSetorSR.nos[indice1].idNo;

        }
        noProf[configuracaoParam->rnp[indiceRNP].nos[indice].profundidade] = configuracaoParam->rnp[indiceRNP].nos[indice].idNo;
    }
     // printf("carregamento %.5lf \n", carregamentoRede*100);
    configuracaoParam->rnp[indiceRNP].fitnessRNP.potenciaAlimentador = carregamentoAlimentador;
    configuracaoParam->rnp[indiceRNP].fitnessRNP.maiorCarregamentoCorrente = carregamentoRede;
    configuracaoParam->rnp[indiceRNP].fitnessRNP.carregamentoRede = cabs(carregamentoAlimentador);
    configuracaoParam->rnp[indiceRNP].fitnessRNP.perdasResistivas = perdas / 1000;
}



void fluxoCargaTodosAlimentadores(long int numeroBarrasParam, 
        DADOSALIMENTADOR *dadosAlimentadorParam, DADOSTRAFO *dadosTrafoParam,
        CONFIGURACAO *configuracoesParam, long int indiceConfiguracao, RNPSETORES *matrizB,
        MATRIZCOMPLEXA *ZParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulador, MATRIZMAXCORRENTE * maximoCorrenteParam) {

    int indiceRNP;
    double VF;
    char nomeArquivo[200];
    sprintf(nomeArquivo, "dadosCorrente%ld.dad", indiceConfiguracao);
    arquivog = fopen(nomeArquivo, "w");
    for (indiceRNP = 0; indiceRNP < configuracoesParam[indiceConfiguracao].numeroRNP; indiceRNP++) {
        VF = 1000*dadosTrafoParam[dadosAlimentadorParam[indiceRNP+1].idTrafo].tensaoReal/sqrt(3);
        fluxoCargaAlimentador(numeroBarrasParam, configuracoesParam, VF, indiceRNP, indiceConfiguracao, matrizB,
                ZParam, maximoCorrenteParam, indiceRegulador, dadosRegulador);
    }
    fclose(arquivog);
}

void fluxoCargaAlimentador(int numeroBarrasParam, CONFIGURACAO *configuracoesParam, 
        double VFParam, int indiceRNP, int indiceConfiguracao, RNPSETORES *matrizB,
        MATRIZCOMPLEXA *ZParam, MATRIZMAXCORRENTE *maximoCorrenteParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulador) {

    long int indice, noS, noR, noN, indiceBarra;
    int iteracoes = 0, carga;
    
    double erroV, erroA, tol, difV, difA;
    __complex__ float *V_barra_ant;
    long int pior; //armazena a barra com pior perfil de tensão de cada alimentador
    long int noProf[200]; //armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
    RNPSETOR rnpSetorSR;
    V_barra_ant = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
    tol = 0.001;
    

    carga = 0; //0 --> corrente constante, 1--> potencia constante 
   //     printf("\nALIMENTADOR %d \n", indiceRNP+1);
    do // varredura backward/forward
    {
     //   printf("----------------iteracao %d -----------------\n", iteracoes);
        indice = 0;
        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
        for(indice = 1; indice < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
            noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
            noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
            
            for (indiceBarra = 0; indiceBarra < rnpSetorSR.numeroNos; indiceBarra++) {
                noN = rnpSetorSR.nos[indiceBarra].idNo;
                if (iteracoes == 0) {
                    configuracoesParam[indiceConfiguracao].dadosEletricos.vBarra[noN] = VFParam;
                    configuracoesParam[indiceConfiguracao].dadosEletricos.corrente[noN] = conj(configuracoesParam[indiceConfiguracao].dadosEletricos.potencia[noN] / configuracoesParam[indiceConfiguracao].dadosEletricos.vBarra[noN]);
                }
                V_barra_ant[noN] = configuracoesParam[indiceConfiguracao].dadosEletricos.vBarra[noN];
            }
            noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
        }
        correnteJusante(indiceRNP, carga, iteracoes,
                configuracoesParam[indiceConfiguracao], matrizB, indiceRegulador, dadosRegulador);
       // printf("indiceRNP %d\n", indiceRNP);
        pior = tensaoQuedaTensao(indiceRNP, configuracoesParam[indiceConfiguracao], VFParam, matrizB, ZParam, indiceRegulador,dadosRegulador); // etapa forward - tensões
        configuracoesParam[indiceConfiguracao].rnp[indiceRNP].fitnessRNP.piorBarra = pior;
        
      //  printf("quedaMaxima %.2lf pior %d menorTensao %.2lf \n", quedaMaxima, pior,menorTensao);
        erroV = 0;
        erroA = 0;
        indice = 0;
        noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
        
        for (indice = 1; indice < configuracoesParam[indiceConfiguracao].rnp[indiceRNP].numeroNos; indice++) {
            noS = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
            noR = noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade - 1];
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
            for (indiceBarra = 0; indiceBarra < rnpSetorSR.numeroNos; indiceBarra++) {
                noN = rnpSetorSR.nos[indiceBarra].idNo;
                difV = cabs(configuracoesParam[indiceConfiguracao].dadosEletricos.vBarra[noN]) - cabs(V_barra_ant[noN]); //calcula a diferença de módulos das tensões entre duas iterações
                if (difV < 0)
                    difV = -1 * difV; //faz mÃ³dulo
                if (difV > erroV) //encontra o maior desvio
                    erroV = difV;
                difA = carg(configuracoesParam[indiceConfiguracao].dadosEletricos.vBarra[noN]) - carg(V_barra_ant[noN]); //calcula a diferença de ângulos das tensões entre duas iterações
                if (difA < 0)
                    difA = -1 * difA; //faz módulo
                if (difA > erroA) //encontra o maior desvio
                    erroA = difA;
            }
            noProf[configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracao].rnp[indiceRNP].nos[indice].idNo;
        }
        
        iteracoes++;
    } while ((erroV > tol || erroA > tol || flagMudancaTap) && iteracoes < 50);
    
    carregamentoPerdas(&configuracoesParam[indiceConfiguracao], indiceRNP, matrizB, ZParam, maximoCorrenteParam);
    
    free(V_barra_ant);
}



void carregamentoTrafo(DADOSTRAFO *dadosTrafoParam, int numeroTrafosParam, 
        int numeroAlimentadoresParam, DADOSALIMENTADOR *dadosAlimentadorParam, 
        CONFIGURACAO *configuracoesParam, int idNovaConfiguracaoParam, 
        int idAntigaConfiguracaoParam, BOOL todosAlimentadores, int idRNPOrigem, int idRNPDestino)
{
  int indiceI,indiceJ;
  int idTrafo;
  double potencia;
  double carregamentoTrafo;
  carregamentoTrafo=0;
//realiza o cálculo para todos os alimentadores
    if (todosAlimentadores) {
        for(indiceI = 1; indiceI <= numeroTrafosParam; indiceI++)
        {
            configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI] = 0+I*0;
        }
        for(indiceJ = 1; indiceJ<=numeroAlimentadoresParam; indiceJ++)
        {
            idTrafo = dadosAlimentadorParam[indiceJ].idTrafo;
            //a sequencia de rnps obedece a mesma sequencia de alimentadores com a diferença de uma posição 
            configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[idTrafo] += configuracoesParam[idNovaConfiguracaoParam].rnp[indiceJ-1].fitnessRNP.potenciaAlimentador;
        }
        for (indiceI = 1; indiceI <= numeroTrafosParam; indiceI++) {       
           potencia = cabs(configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI]) / (dadosTrafoParam[indiceI].capacidade * 10000); //(10000=1MVA/100) em porcentagem
            if (carregamentoTrafo < potencia)
                carregamentoTrafo = potencia;
          //
        // printf("capacidade %.2lf potencia do trafo  %lf carregamento %.2lf \n", dadosTrafoParam[indiceI].capacidade, cabs(configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI])/1000000, potencia);
        } 
        
        configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = carregamentoTrafo;
        
    }
    else //atualiza os valores considerando apenas os alimentadores que tiveram modificação
    {
        configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = configuracoesParam[idAntigaConfiguracaoParam].objetivo.maiorCarregamentoTrafo;
        for (indiceI = 1; indiceI <= numeroTrafosParam; indiceI++) {       
           
            configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI] = configuracoesParam[idAntigaConfiguracaoParam].objetivo.potenciaTrafo[indiceI];
          //verifica se o alimentador da RNP do alimentador de origem pertence a esse trafo. O indice do alimentador é o indice do vetor da rnp acrescido de 1. 
            if(dadosTrafoParam[indiceI].idTrafo == dadosAlimentadorParam[idRNPOrigem+1].idTrafo)
           {
               configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI] -= configuracoesParam[idAntigaConfiguracaoParam].rnp[idRNPOrigem].fitnessRNP.potenciaAlimentador;
               configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI] += configuracoesParam[idNovaConfiguracaoParam].rnp[idRNPOrigem].fitnessRNP.potenciaAlimentador;
           }
            //verifica se o alimentador da RNP do alimentador de destino pertence a esse trafo. O indice do alimentador é o indice do vetor da rnp acrescido de 1.
           if(dadosTrafoParam[indiceI].idTrafo == dadosAlimentadorParam[idRNPDestino+1].idTrafo)
           {
               configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI] -= configuracoesParam[idAntigaConfiguracaoParam].rnp[idRNPDestino].fitnessRNP.potenciaAlimentador;
               configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI] += configuracoesParam[idNovaConfiguracaoParam].rnp[idRNPDestino].fitnessRNP.potenciaAlimentador;
           } 
           //calcula a potencia que está sendo utilizada em porcentagem da capacidade
           potencia = cabs(configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo[indiceI]) / (dadosTrafoParam[indiceI].capacidade * 10000); //10000=1MVA*100%
           //obtem o maior carregamento de trafo da rede em porcentagem 
           if (configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo < potencia)
                configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = potencia;
        }        
    }
}

void calculaPonderacao(CONFIGURACAO *configuracoesParam, long int idConfiguracaoParam, double VFParam) {
    int a, b, c;
    double Fit = 0.0;
    a = b = c = 100;
    if (configuracoesParam[idConfiguracaoParam].objetivo.maiorCarregamentoCorrente < 100)
        a = 0;
    if (configuracoesParam[idConfiguracaoParam].objetivo.menorTensao > (VFParam*0.9)) /*12420 é 10% de queda de tensao*/
        b = 0;
    if (configuracoesParam[idConfiguracaoParam].objetivo.maiorCarregamentoTrafo <= 100)
        c = 0;
   /* if (idConfiguracaoParam < 2) {
        Fit = configuracoesParam[idConfiguracaoParam].objetivo.perdasResistivas
                + a * configuracoesParam[idConfiguracaoParam].objetivo.maiorCarregamentoCorrente
                + b * (VFParam - configuracoesParam[idConfiguracaoParam].objetivo.menorTensao) / VFParam
                // + (configuracoesParam[idConfiguracaoParam].objetivo.manobrasManuais + configuracoesParam[idConfiguracaoParam].objetivo.manobrasAutomaticas)
                + c * configuracoesParam[idConfiguracaoParam].objetivo.maiorCarregamentoTrafo;
    } else {*/
        Fit = configuracoesParam[idConfiguracaoParam].objetivo.perdasResistivas
                + a * configuracoesParam[idConfiguracaoParam].objetivo.maiorCarregamentoCorrente/100
                + b * (VFParam - configuracoesParam[idConfiguracaoParam].objetivo.menorTensao) / VFParam
                + (configuracoesParam[idConfiguracaoParam].objetivo.manobrasManuais + configuracoesParam[idConfiguracaoParam].objetivo.manobrasAutomaticas)
                + c * configuracoesParam[idConfiguracaoParam].objetivo.maiorCarregamentoTrafo/100;
    //}
    //  printf("fit %lf \n", Fit);
    configuracoesParam[idConfiguracaoParam].objetivo.ponderacao = Fit;
}

void avaliaConfiguracao(BOOL todosAlimentadores, CONFIGURACAO *configuracoesParam, int rnpA, int rnpP, long int idNovaConfiguracaoParam, DADOSTRAFO *dadosTrafoParam, int numeroTrafosParam,
        int numeroAlimentadoresParam, int *indiceRegulador, DADOSREGULADOR *dadosRegulador, DADOSALIMENTADOR *dadosAlimentadorParam, double VFParam, int idAntigaConfiguracaoParam, RNPSETORES *matrizB, MATRIZCOMPLEXA *ZParam,
        MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam, BOOL copiarDadosEletricos) {
    long int contador;
    double quedaMaxima, menorTensao, VF, perdasTotais;
    perdasTotais = 0;
    //verifica se deve manter os dados elétricos para cada configuração gerada individualmente. Para isso é necessário copiar os dados elétricos de uma configuração para outra
    if (idNovaConfiguracaoParam != idAntigaConfiguracaoParam) {
        if (copiarDadosEletricos) {
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.iJusante = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.corrente = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.potencia = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.vBarra = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
            for (contador = 1; contador <= numeroBarrasParam; contador++) {
                configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.iJusante[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.iJusante[contador];
                configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.corrente[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.corrente[contador];
                configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.potencia[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.potencia[contador];
                configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.vBarra[contador] = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.vBarra[contador];
            }
        } else { //não mantém os dados elétricos individualmente, somente copia os ponteiros para os vetores com dados elétricos
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.iJusante = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.iJusante;
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.corrente = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.corrente;
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.potencia = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.potencia;
            configuracoesParam[idNovaConfiguracaoParam].dadosEletricos.vBarra = configuracoesParam[idAntigaConfiguracaoParam].dadosEletricos.vBarra;
        }
    }

    

    configuracoesParam[idNovaConfiguracaoParam].objetivo.potenciaTrafo = malloc((numeroTrafosParam + 1) * sizeof (__complex__ double));
    configuracoesParam[idNovaConfiguracaoParam].objetivo.menorTensao = 100000;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoRede = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoTrafo = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.perdasResistivas = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.ponderacao = 0;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.quedaMaxima = 0;
    if (todosAlimentadores) //calcula os valores de fitness para todos os alimentadores
    {
        fluxoCargaTodosAlimentadores(numeroBarrasParam,dadosAlimentadorParam,dadosTrafoParam, configuracoesParam, idNovaConfiguracaoParam, matrizB, ZParam, indiceRegulador, dadosRegulador, maximoCorrenteParam);
    } else { //calcula o fluxo de carga somente para os alimentadores que foram alterados.
        VF = 1000*dadosTrafoParam[dadosAlimentadorParam[rnpP+1].idTrafo].tensaoReal/sqrt(3);
        fluxoCargaAlimentador(numeroBarrasParam, configuracoesParam, VF, rnpP, idNovaConfiguracaoParam, matrizB, ZParam, maximoCorrenteParam, indiceRegulador, dadosRegulador);
        VF = 1000*dadosTrafoParam[dadosAlimentadorParam[rnpA+1].idTrafo].tensaoReal/sqrt(3);
        fluxoCargaAlimentador(numeroBarrasParam, configuracoesParam, VF, rnpA, idNovaConfiguracaoParam, matrizB, ZParam, maximoCorrenteParam, indiceRegulador, dadosRegulador);
    }
    int indiceRNP;
    for (indiceRNP = 0; indiceRNP < configuracoesParam[idNovaConfiguracaoParam].numeroRNP; indiceRNP++) {
      //  printf("alimentador %d carregamento %lf\n",indiceRNP+1,configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.maiorCarregamentoCorrente*100);
        quedaMaxima = 100000;
        VF = 1000*dadosTrafoParam[dadosAlimentadorParam[indiceRNP+1].idTrafo].tensaoReal/sqrt(3);
        menorTensao = configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.menorTensao;
        if (quedaMaxima > menorTensao) // encontra a menor tensão do sistema
            quedaMaxima = menorTensao;
        //atualiza a menor tensão do individuo
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.menorTensao > menorTensao)
            configuracoesParam[idNovaConfiguracaoParam].objetivo.menorTensao = menorTensao;
        quedaMaxima = ((VF - quedaMaxima) / VF)*100;
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.quedaMaxima < quedaMaxima)
            configuracoesParam[idNovaConfiguracaoParam].objetivo.quedaMaxima = quedaMaxima;
        //potencia
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoRede < configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.carregamentoRede)
            configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoRede = configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.carregamentoRede;
        //máximo de corrente
        if (configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente < configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.maiorCarregamentoCorrente)
            configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente = configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.maiorCarregamentoCorrente;
        //total perdas
        //if (configuracoesParam[idNovaConfiguracaoParam].objetivo.perdasResistivas < configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.perdasResistivas)
          //  configuracoesParam[idNovaConfiguracaoParam].objetivo.perdasResistivas = configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.perdasResistivas;
        perdasTotais+=configuracoesParam[idNovaConfiguracaoParam].rnp[indiceRNP].fitnessRNP.perdasResistivas;

    }
    configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente = configuracoesParam[idNovaConfiguracaoParam].objetivo.maiorCarregamentoCorrente * 100;
    configuracoesParam[idNovaConfiguracaoParam].objetivo.perdasResistivas = perdasTotais;
    carregamentoTrafo(dadosTrafoParam, numeroTrafosParam, numeroAlimentadoresParam, dadosAlimentadorParam, configuracoesParam, idNovaConfiguracaoParam, idAntigaConfiguracaoParam, todosAlimentadores, rnpP, rnpA);
    calculaPonderacao(configuracoesParam, idNovaConfiguracaoParam, VFParam);
}

void constroiVetorTaps(int *tapsParam, DADOSREGULADOR *dadosReguladorParam, long int numeroReguladoresParam, GRAFO *grafoSDRParam, long int numeroBarrasParam)
{
    long int indiceNo1, indiceNo2, indiceRegulador;
    char idRegulador[30];
    for(indiceNo1 = 1; indiceNo1 <= numeroBarrasParam; indiceNo1++)
    {
        for(indiceNo2 = 0; indiceNo2 < grafoSDRParam[indiceNo1].numeroAdjacentes; indiceNo2++)
        {
            if(grafoSDRParam[indiceNo1].adjacentes[indiceNo2].tipoAresta == reguladorTensao)
            {
                //existe regulador de tensão entre as barras
                //localiza o regulador de tensão
                strcpy(idRegulador,grafoSDRParam[indiceNo1].adjacentes[indiceNo2].idAresta);
                for(indiceRegulador =0; indiceRegulador <numeroReguladoresParam; indiceRegulador++)
                {
                    if(strcmp(idRegulador,dadosReguladorParam[indiceRegulador].idRegulador) == 0)
                    {
                        //printf("regulador %s barra1 %ld barra2 %ld\n", idRegulador, grafoSDRParam[indiceNo1].idNo, grafoSDRParam[indiceNo1].adjacentes[indiceNo2].idNo);
                        tapsParam[grafoSDRParam[indiceNo1].idNo] = indiceRegulador;
                        tapsParam[grafoSDRParam[indiceNo1].adjacentes[indiceNo2].idNo] = indiceRegulador;
                    }
                }
            }
            else //não existe regulador de tensão entre as duas barras, insere valor 0 no taps.
            {
                tapsParam[grafoSDRParam[indiceNo1].idNo] = 0;
                tapsParam[grafoSDRParam[indiceNo1].adjacentes[indiceNo2].idNo] = 0;
            }
        }
    }
}

void imprimeIndicadoresEletricos(CONFIGURACAO configuracaoParam)
{
   // printf("Maior Carregamento Alimentador %.2lf \n", configuracaoParam.objetivo.maiorCarregamentoRede/1000);
    printf("Maior Carregamento Rede %.2lf%%\n",configuracaoParam.objetivo.maiorCarregamentoCorrente);
    printf("Maior Carregamento Trafo %.2lf%%\n", configuracaoParam.objetivo.maiorCarregamentoTrafo);
    printf("Perdas Resistivas %.2lfkW \n",configuracaoParam.objetivo.perdasResistivas);
    printf("Menor Tensao %.2lfV \n",configuracaoParam.objetivo.menorTensao*sqrt(3));
    printf("Queda Maxima %.2lf%%\n",configuracaoParam.objetivo.quedaMaxima);
    //printf("Consumidores %ld\n",configuracaoParam.objetivo.consumidoresSemFornecimento);
    //printf("Consumidores Especiais %ld\n",configuracaoParam.objetivo.consumidoresEspeciaisSemFornecimento);
    //printf("Ponderacao %.2lf \n",configuracaoParam.objetivo.ponderacao);   
}


void inicializaObjetivos(CONFIGURACAO *configuracao) {
    configuracao[0].objetivo.maiorCarregamentoRede = 0.0;
    configuracao[0].objetivo.maiorCarregamentoCorrente = 0.0;
    configuracao[0].objetivo.maiorCarregamentoTrafo = 0;
    configuracao[0].objetivo.manobrasManuais = 0;
    configuracao[0].objetivo.manobrasAutomaticas = 0;
    configuracao[0].objetivo.menorTensao = 0;
    //
    configuracao[0].objetivo.ponderacao = 0;
    configuracao[0].objetivo.quedaMaxima = 0;
    configuracao[0].objetivo.rank = 0;
    configuracao[0].objetivo.fronteira = 0;
    configuracao[0].objetivo.perdasResistivas = 0;
    configuracao[0].objetivo.consumidoresSemFornecimento= 0;
    configuracao[0].objetivo.consumidoresEspeciaisSemFornecimento = 0;
}

void desalocaMatrizZ(MATRIZCOMPLEXA *ZParam, long int numeroBarrasParam)
{
    long int contador;
    for (contador =1; contador<=numeroBarrasParam; contador++)
    {
        free(ZParam[contador].noAdjacentes);
    }
    free(ZParam);
}

void desalocaMatrizCorrente(MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam)
{
    long int contador;
    for (contador =1; contador<=numeroBarrasParam; contador++)
    {
        free(maximoCorrenteParam[contador].noAdjacentes);
    }
    free(maximoCorrenteParam);
}



