#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>
#include <dirent.h>
#include "funcoesSetor.h"
#include "funcoesInicializacao.h"
#include "data_structures.h"


//definição da constante ij em número complexo
__complex__ double ij = 1.0fI;

/**
 *  Este método é responsável por atualizar o estado de todas as chaves do Sistema de DistribuiÃ§Ã£o a partir do arquivo de AtuaÃ§Ã£o de Chaves.
 *  O método faz a leitura do arquivo ESTADOS_CHS_"data e hora".dad a atribui o estado de chaves atualizado de acordo com o FEATNUM.
 *  Tem como parâmetros o grafo que representa o sistema
 * @param grafoSDRParam
 * 
 */
void atualizaEstadoChaves(GRAFO *grafoSDRParam, long int numeroBarras)
{
    int contador, contadorAdj;
    char FEATNUM[15], estado;
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados; /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    FILE *arquivo = NULL;
    DIR * d;
    
    //Abre o diretorio atual e procura o arquivo de chaves
    d = opendir (".");
    while (1) {
        struct dirent *entry;
        
        entry = readdir (d);
        if (! entry) {
            break;
        }
        if (strncmp("ESTADOS_CHS",entry->d_name,11)==0){
            arquivo = fopen(entry->d_name,"r");
            break;
        }
    }
    closedir (d);
    
    if(arquivo != NULL){
        //Inicia a leitura do arquivo da primeira a ultima linha
        
        // Le da primeira linha (Cabeçalho)
        fgets(blocoLeitura, 2000, arquivo);
        
        // Le o arquivo de curva de cargas até o fim
        while( (fgets(blocoLeitura, 2000, arquivo))!= NULL ){
            dados = blocoLeitura;
            
            //Le o FEATNUM e estado da chave
            sscanf(dados, "%s %c",FEATNUM,&estado);
                        
            //percorre o grafo do sistema de distribuição e identifica a chave a ser atualizada
            for (contador =1; contador <= numeroBarras; contador++)
            {
                for(contadorAdj = 0; contadorAdj<grafoSDRParam[contador].numeroAdjacentes; contadorAdj++)
                {
                    if (strcmp(grafoSDRParam[contador].adjacentes[contadorAdj].idAresta,FEATNUM)==0)
                    {
                        if (estado == 'A') grafoSDRParam[contador].adjacentes[contadorAdj].estadoChave = 0;
                        else grafoSDRParam[contador].adjacentes[contadorAdj].estadoChave = 1;
                    }
                }
            }
        }
    }
}

/**
 *  Este método é responsável por identificar todas as chaves do Sistema de Distribuição e armazená-las em uma estrutura.
 *  O método também atribui a cada chave um identificador único, que será utilizado para identificar as chaves manobradas.
 *  Tem como parâmetros o grafo que representa o sistema e a estrutura onde serão armazenadas as chaves
 * @param grafoSDRParam
 * @param listaChavesParam
 */
void constroiListaChaves(GRAFO *grafoSDRParam, LISTACHAVES **listaChavesParam, long int numeroChaves, long int numeroBarras)
{
   //faz a alocação da estrutura para armazenar as chaves do sistema
    if (((*listaChavesParam) = (LISTACHAVES *)malloc((numeroChaves+1) * sizeof(LISTACHAVES)))==NULL)
    {
        printf("ERRO ao alocar memoria para lista de chaves!!!");
        exit(1);
    }
    int contador, contadorAdj;
    int identificadorChave = 1; //variável identificador das chaves, começa a sua numeração de 1 e é utilizada para indexar a nova estrutura
    
    //percorre o grafo do sistema de distribuição identificando as chaves e armazenando na nova estrutura atribuindo um identificador
    for (contador =1; contador <= numeroBarras; contador++)
    {
        for(contadorAdj = 0; contadorAdj<grafoSDRParam[contador].numeroAdjacentes; contadorAdj++)
        {
            if((grafoSDRParam[contador].adjacentes[contadorAdj].idNo >= contador)&&(grafoSDRParam[contador].adjacentes[contadorAdj].tipoAresta == chaveManual || grafoSDRParam[contador].adjacentes[contadorAdj].tipoAresta == chaveAutomatica))
            {
                //copia os dados referentes a chave
                (*listaChavesParam)[identificadorChave].idNoDe = grafoSDRParam[contador].idNo;
                (*listaChavesParam)[identificadorChave].idNoPara = grafoSDRParam[contador].adjacentes[contadorAdj].idNo;
                (*listaChavesParam)[identificadorChave].condicao = grafoSDRParam[contador].adjacentes[contadorAdj].condicao;
                (*listaChavesParam)[identificadorChave].estadoChave = grafoSDRParam[contador].adjacentes[contadorAdj].estadoChave;
                (*listaChavesParam)[identificadorChave].identificador = identificadorChave;
                (*listaChavesParam)[identificadorChave].subTipoChave = grafoSDRParam[contador].adjacentes[contadorAdj].subTipoAresta;
                (*listaChavesParam)[identificadorChave].tipoChave = grafoSDRParam[contador].adjacentes[contadorAdj].tipoAresta;
                strcpy((*listaChavesParam)[identificadorChave].codOperacional, grafoSDRParam[contador].adjacentes[contadorAdj].idAresta);       
                
                //incrementa o valor do identificador da chave
                identificadorChave++;
                
            }
        }
    }   
}

/**
 * Realiza a impressão na tela de todas as chaves do sistema com o identificador e as suas barrass.
 * Recebe como parâmetro a estrutura LISTACHAVES
 * @param listaChavesParam
 */
void imprimeListaChaves(LISTACHAVES *listaChavesParam, long int numeroChaves)
{
    int contador;
   //imprime o número de chaves do sistema
    printf("Numero Chaves %ld\n", numeroChaves);
    //percorre a lista realizando a impressão
    for(contador = 1; contador<=numeroChaves; contador++)
    {
        printf("Identificador chave: %ld id no origem: %ld id no destino %ld Estado Chave %d \n", listaChavesParam[contador].identificador, listaChavesParam[contador].idNoDe, listaChavesParam[contador].idNoPara, listaChavesParam[contador].estadoChave);
    }

}

/**
 * Função que retorna o identificador de uma chave dados os valores da barras correspondentes
 * Recebe como parâmetros a lista de chaves (LISTACHAVES) e o valor das duas barras
 * @param listaChavesParam
 * @param idNo1
 * @param idNo2
 * @return 
 */
int buscaChave(LISTACHAVES *listaChavesParam, int idNo1, int idNo2, long int numeroChaves)
{
    long int contador=1;
    BOOL encontrado = false;
    int idChave = -1;
    //percorre a lista de chaves até encontrar a chave ou chegar ao fim da lista
    while(contador<=numeroChaves && !encontrado)
    {
        //verifica a chave refere-se as barras informadas;
       // printf("contador %ld \n", contador);
        if((listaChavesParam[contador].idNoDe == idNo1 && listaChavesParam[contador].idNoPara == idNo2) || (listaChavesParam[contador].idNoDe == idNo2 && listaChavesParam[contador].idNoPara == idNo1))
        {
            encontrado = true;
            idChave = contador;
        }
        contador++;
    }
    
    return idChave;
}

/**
 * Realiza uma busca em profundidade nos setores identificados do sistema de forma a constuir a RNP do alimentador.
 * Tem como parâmetros o identificador do setor, o vetor de flags para indicar os setores visitados, o ponteiro para RNP
 * que está sendo construída, a profundidade atual, o índice atual, o Grafo de setores sistema e a lista de chaves.
 * 
 * @param idSetorParam
 * @param visitadoParam
 * @param rnpParam
 * @param profundidade
 * @param indice
 * @param grafoSetoresParam
 * @param listaChavesParam
 */
void buscaProfundidadeSetores(int idSetorParam, BOOL *visitadoParam,RNP *rnpParam, int profundidade, int *indice, GRAFOSETORES *grafoSetoresParam, LISTACHAVES *listaChavesParam) 
{
    int indiceAdj;
    int idSetorAdj;
    int idChave;
    rnpParam->nos[indice[0]].idNo = idSetorParam;
    rnpParam->nos[indice[0]].profundidade = profundidade;
    visitadoParam[idSetorParam] = true;
    indice[0]++;
    profundidade++;
    //percorre a lista de adjacências do grafo de setores
    for(indiceAdj = 0; indiceAdj < grafoSetoresParam[idSetorParam].numeroAdjacentes; indiceAdj++)
    {
        idSetorAdj = grafoSetoresParam[idSetorParam].setoresAdjacentes[indiceAdj];
        idChave = grafoSetoresParam[idSetorParam].idChavesAdjacentes[indiceAdj];
        //se o setor adjacente não tiver sido visitado e a chave for fechada continua a busca em profundidade recursivamente
        if(!visitadoParam[idSetorAdj] && listaChavesParam[idChave].estadoChave==normalmenteFechada)
        {
            buscaProfundidadeSetores(idSetorAdj, visitadoParam, rnpParam, profundidade, indice, grafoSetoresParam, listaChavesParam);
        }
    }        
}


/**
 * Realiza uma busca em profundidade nos setores identificados do sistema de forma a constuir a RNP do alimentador.
 * Tem como parâmetros o identificador do setor, o vetor de flags para indicar os setores visitados, o ponteiro para RNP
 * que está sendo construída, a profundidade atual, o índice atual, o Grafo de setores sistema e a lista de chaves.
 * 
 * @param idSetorParam
 * @param visitadoParam
 * @param rnpParam
 * @param profundidade
 * @param indice
 * @param grafoSetoresParam
 * @param listaChavesParam
 */

void contaSetoresAlimentador(int idSetorParam, BOOL *visitadoParam, int *numeroSetores, GRAFOSETORES *grafoSetoresParam, LISTACHAVES *listaChavesParam) 
{
    int indiceAdj;
    int idSetorAdj;
    int idChave;
    visitadoParam[idSetorParam] = true;
    numeroSetores[0]++;
    //percorre a lista de adjacências do grafo de setores
    for(indiceAdj = 0; indiceAdj < grafoSetoresParam[idSetorParam].numeroAdjacentes; indiceAdj++)
    {
        idSetorAdj = grafoSetoresParam[idSetorParam].setoresAdjacentes[indiceAdj];
        idChave = grafoSetoresParam[idSetorParam].idChavesAdjacentes[indiceAdj];
        //se o setor adjacente não tiver sido visitado e a chave for fechada continua a busca em profundidade recursivamente
        if(!visitadoParam[idSetorAdj] && listaChavesParam[idChave].estadoChave==normalmenteFechada)
        {
            contaSetoresAlimentador(idSetorAdj, visitadoParam, numeroSetores, grafoSetoresParam, listaChavesParam);
        }
    }        
}


/**
 * Percorre a lista de alimentadores do sistema com os seus repectivos setores, construindo as RNPs da configuração inicial.
 * Esse processo considera o estado das chaves. 
 * Tem como parâmentros o grafo de setores, o grafo do sistema, a lista de chaves, a lista de alimentadores, a matriz PI ra RNP e lista de configurações 
 * @param grafoSetoresParam
 * @param grafoSDRParam
 * @param listaChavesParam
 * @param alimentadoresParam
 * @param matrizPIParam
 * @param populacaoParam
 */
void constroiIndividuoInicial(GRAFOSETORES *grafoSetoresParam, GRAFO *grafoSDRParam, LISTACHAVES *listaChavesParam, DADOSALIMENTADOR *alimentadoresParam, CONFIGURACAO *populacaoParam)
{
    int indice;
    int indiceRNP = 0;
    int profundidade = 0;
    int idIndividuoInicial = 0;
    int numeroSetoresAlimentador;
    
    BOOL *visitado = Malloc(BOOL, (numeroSetores+1));
    int idSetorAlimentador;
    //inicializa o vetor flag de visitado para  busca em profundidade.
    for(indice=0;indice<=numeroSetores; indice++)
    {
        visitado[indice] = false;
    }
    
    
     for(indice=1;indice<=numeroAlimentadores; indice++)
    {
        
        numeroSetoresAlimentador=0;
        idSetorAlimentador = grafoSDRParam[alimentadoresParam[indice].barraAlimentador].idSetor;
        contaSetoresAlimentador(idSetorAlimentador, visitado, &numeroSetoresAlimentador, grafoSetoresParam, listaChavesParam);
        alimentadoresParam[indice].numeroSetores = numeroSetoresAlimentador;
     }
    for(indice=0;indice<=numeroSetores; indice++)
    {
        visitado[indice] = false;
    }
                //percorre a lista de alimentadores do sistema para construir a RNP referente a cada alimentador
    for(indice=1;indice<=numeroAlimentadores; indice++)
    {
        
        indiceRNP=0;
        idSetorAlimentador = grafoSDRParam[alimentadoresParam[indice].barraAlimentador].idSetor;
        //faz a alocação da RNP de cada alimentador
        alocaRNP(alimentadoresParam[indice].numeroSetores,&populacaoParam[idIndividuoInicial].rnp[(indice-1)]);
        //realiza a busca em profundidade para cada alimentador de forma a obter a RNP
        buscaProfundidadeSetores(idSetorAlimentador, visitado,&populacaoParam[idIndividuoInicial].rnp[(indice-1)], profundidade, &indiceRNP, grafoSetoresParam, listaChavesParam);
    }
}

void inicializaDadosEletricos(GRAFO *grafoSDRParam, CONFIGURACAO *configuracoesParam, long int indiceConfiguracaoParam, long int numeroBarrasParam, int sBaseParam, double VFParam, int modeloFluxoCarga) {
    long int contadorBarras;
    int sinal = 1;
    configuracoesParam[indiceConfiguracaoParam].dadosEletricos.iJusante = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
    configuracoesParam[indiceConfiguracaoParam].dadosEletricos.corrente = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
    configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
    configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
    for(contadorBarras = 1; contadorBarras<=numeroBarrasParam; contadorBarras++)
    {
        sinal = 1;
       __real__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[contadorBarras] = grafoSDRParam[contadorBarras].valorPQ.p;
       __imag__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[contadorBarras] = grafoSDRParam[contadorBarras].valorPQ.q;
       //transforma em modelo monofásico...
       configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[contadorBarras] = (configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[contadorBarras]*sBaseParam)/3.0;
       configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra[contadorBarras] = VFParam;
       configuracoesParam[indiceConfiguracaoParam].dadosEletricos.corrente[contadorBarras] = conj(configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[contadorBarras] / configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra[contadorBarras]);
           
    }
}

void inicializaDadosEletricosPorAlimentador(GRAFO *grafoSDRParam, CONFIGURACAO *configuracoesParam, long int indiceConfiguracaoParam, long int numeroBarrasParam, int sBaseParam, DADOSTRAFO *dadosTrafoParam, DADOSALIMENTADOR *dadosAlimentadorParam, RNPSETORES *matrizB) {
    long int noS, noR, noN;
    double VF;
    int indiceRNP, indice, indice1;
    long int noProf[200]; //armazena o ultimo nó presente em uma profundidade, é indexado pela profundidade
    RNPSETOR rnpSetorSR;
    configuracoesParam[indiceConfiguracaoParam].dadosEletricos.iJusante = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
    configuracoesParam[indiceConfiguracaoParam].dadosEletricos.corrente = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
    configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
    configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra = malloc((numeroBarrasParam + 1) * sizeof (__complex__ double));
    for (indiceRNP = 0; indiceRNP < configuracoesParam[indiceConfiguracaoParam].numeroRNP; indiceRNP++) {
       // printf("trafo %d \n",dadosAlimentadorParam[indiceRNP+1].idTrafo);
        VF = 1000*dadosTrafoParam[dadosAlimentadorParam[indiceRNP+1].idTrafo].tensaoReal/ sqrt(3.0);

        noProf[configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].nos[0].profundidade] = configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].nos[0].idNo;
        for (indice = 1; indice < configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].numeroNos; indice++) {
            noS = configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].nos[indice].idNo;
            noR = noProf[configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].nos[indice].profundidade - 1];
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
            for (indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {
                noN = rnpSetorSR.nos[indice1].idNo;
                __real__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] = grafoSDRParam[noN].valorPQ.p;
                __imag__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] = grafoSDRParam[noN].valorPQ.q;
                //transforma em modelo monofásico...
                configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] = (configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] * sBaseParam) / 3.0;
                configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra[noN] = VF;
                configuracoesParam[indiceConfiguracaoParam].dadosEletricos.corrente[noN] = conj(configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] / (configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra[noN]));

            }
             //armazena o nó setor na sua profundidade
            noProf[configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].nos[indice].profundidade] = configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].nos[indice].idNo;
        }
        //Calcula o setor do Alimentador
        if (configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].numeroNos > 1) {
            noS = configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].nos[0].idNo;
            noR = configuracoesParam[indiceConfiguracaoParam].rnp[indiceRNP].nos[1].idNo;
            rnpSetorSR = buscaRNPSetor(matrizB, noS, noR);
            for (indice1 = 1; indice1 < rnpSetorSR.numeroNos; indice1++) {
                noN = rnpSetorSR.nos[indice1].idNo;
                __real__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] = grafoSDRParam[noN].valorPQ.p;
                __imag__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] = grafoSDRParam[noN].valorPQ.q;
                //transforma em modelo monofásico...
                configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] = (configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] * sBaseParam) / 3.0;
                configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra[noN] = VF;
                configuracoesParam[indiceConfiguracaoParam].dadosEletricos.corrente[noN] = conj(configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[noN] / configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra[noN]);

            }
        }
    }


}

void imprimeDadosEletricos(CONFIGURACAO *configuracoesParam, long int indiceConfiguracaoParam, long int numeroBarrasParam) 
{
    long int contadorBarras;
    for(contadorBarras = 1; contadorBarras<=numeroBarrasParam; contadorBarras++)
    {
        printf("Potencia Barra %ld %.2lf j%.2lf\n",contadorBarras,__real__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[contadorBarras],__imag__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.potencia[contadorBarras]);
        printf("Vbarra Barra %ld %.2lf j%.2lf\n",contadorBarras,__real__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra[contadorBarras],__imag__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.vBarra[contadorBarras]);
        printf("Corrente Barra %ld %.2lf j%.2lf\n",contadorBarras,__real__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.corrente[contadorBarras],__imag__ configuracoesParam[indiceConfiguracaoParam].dadosEletricos.corrente[contadorBarras]);
      //  printf("Modulo Corrente Barra %ld %.2lf\n\n",contadorBarras,cabs( configuracoesParam[indiceConfiguracaoParam].dadosEletricos.corrente[contadorBarras]));
     
    }
}

long int buscaChaveNF(LISTACHAVES *listaChavesParam, int idNo1, int idNo2, GRAFOSETORES *grafoSetoresParam)
{
    int contador=0;
    BOOL encontrado = false;
    int idChave = -1;
    //percorre a lista de chaves até encontrar a chave ou chegar ao fim da lista
    while(contador< grafoSetoresParam[idNo1].numeroAdjacentes && !encontrado)
    {
        //verifica a chave refere-se as barras informadas e possui estado fechado;
        if(grafoSetoresParam[idNo1].setoresAdjacentes[contador] == idNo2)
        {
            if(listaChavesParam[grafoSetoresParam[idNo1].idChavesAdjacentes[contador]].estadoChave == normalmenteFechada)
            {
                encontrado = true;
                idChave = grafoSetoresParam[idNo1].idChavesAdjacentes[contador];
            }
        }
        contador++;
    }
    
    return idChave;
}

void imprimeBarrasIsoladas(long int numeroBarrasParam, GRAFO *grafoSDRParam)
{
//    FILE *arquivo;
//    long int contador;
//    
//    arquivo = fopen("barrasIsoladas.dad","w");
//    for(contador = 1; contador <=numeroBarrasParam; contador++)
//    {
//   //     printf("contador %ld\n", contador);
//        if(grafoSDRParam[contador].idSetor == 0)
//            fprintf(arquivo,"BarraId %ld - Adj: %d - Tipo: %d\n", grafoSDRParam[contador].idNo, grafoSDRParam[contador].numeroAdjacentes,grafoSDRParam[contador].tipoNo);
//    }
//    fclose(arquivo);
    
    
    
    FILE *arquivo;
    long int contador,i;
    
    arquivo = fopen("barrasIsoladas.dad","w");
    fprintf(arquivo,"BarraId;Num Adj;Tipo;\n");
    for(contador = 1; contador <=numeroBarrasParam; contador++)
    {
   //     printf("contador %ld\n", contador);
        if(grafoSDRParam[contador].idSetor == 0){
            fprintf(arquivo,"%ld;%d;%d;", grafoSDRParam[contador].idNo, grafoSDRParam[contador].numeroAdjacentes,grafoSDRParam[contador].tipoNo);
            for (i=0; i<grafoSDRParam[contador].numeroAdjacentes;i++){
                fprintf(arquivo,"%ld;%s;",grafoSDRParam[contador].adjacentes[i].idNo, grafoSDRParam[contador].adjacentes[i].idAresta);
            }
            fprintf(arquivo,"\n");
        }
    }
    fclose(arquivo);

}

void gravaDadosBarras(long int numeroBarrasParam, GRAFO *grafoSDRParam)
{
    FILE *arquivo;
    long int contador;
    
    arquivo = fopen("barras.dad","w");
    fprintf(arquivo, "%ld\n", numeroBarrasParam);
    for(contador = 1; contador <=numeroBarrasParam; contador++)
    {
            fprintf(arquivo,"%ld\t%ld\t%d\t%lf\t%lf\n", grafoSDRParam[contador].idNo, grafoSDRParam[contador].idSetor, grafoSDRParam[contador].tipoNo, grafoSDRParam[contador].valorPQ.p,grafoSDRParam[contador].valorPQ.q);
    }
    fclose(arquivo);

}

void salvaLinhas(long int numeroBarrasParam, GRAFO *grafoSDRParam)
{
    FILE *arquivo;
    long int contador, contador2, totalLinhas;
    
    totalLinhas = 0;
    for(contador = 1; contador <=numeroBarrasParam; contador++)
    {
        for(contador2 = 0; contador2 < grafoSDRParam[contador].numeroAdjacentes; contador2++){
            if(grafoSDRParam[contador].idNo < grafoSDRParam[contador].adjacentes[contador2].idNo){
                totalLinhas++;
            }
        }
    }

    arquivo = fopen("linhas.dad","w");
    fprintf(arquivo, "%ld\n", totalLinhas);
    totalLinhas = 0;
    for(contador = 1; contador <=numeroBarrasParam; contador++)
    {
        for(contador2 = 0; contador2 < grafoSDRParam[contador].numeroAdjacentes; contador2++){
            if(grafoSDRParam[contador].idNo < grafoSDRParam[contador].adjacentes[contador2].idNo){
                totalLinhas++;
                fprintf(arquivo,"%ld\t%s\t%d\t%ld\t%ld\t%d\t%d\t%d\n", totalLinhas, grafoSDRParam[contador].adjacentes[contador2].idAresta, grafoSDRParam[contador].adjacentes[contador2].estadoChave, grafoSDRParam[contador].idNo, grafoSDRParam[contador].adjacentes[contador2].idNo, grafoSDRParam[contador].adjacentes[contador2].tipoAresta, grafoSDRParam[contador].adjacentes[contador2].subTipoAresta, grafoSDRParam[contador].adjacentes[contador2].condicao);
            }
        }
    }
    fclose(arquivo);

}


void gravaDadosTrafo(long int numeroTrafosParam, DADOSTRAFO *trafoSDRParam)
{
    FILE *arquivo;
    long int contador;
    
    arquivo = fopen("dadosTrafo.dad","w");
    fprintf(arquivo, "%ld\n", numeroTrafosParam);
    for(contador = 1; contador <=numeroTrafosParam; contador++)
    {
            fprintf(arquivo,"%d\t%d\t%lf\t%lf\n", trafoSDRParam[contador].idTrafo, trafoSDRParam[contador].idSubEstacao, trafoSDRParam[contador].capacidade, trafoSDRParam[contador].tensaoReal);
    }
    fclose(arquivo);

}


void salvaChaves(long int numeroChavesParam, LISTACHAVES *listaChavesParam)
{
    FILE *arquivo;
    long int contador;
    
    arquivo = fopen("chaves.dad","w");
    fprintf(arquivo,"numero\tchaves\t%ld\n", numeroChavesParam);
    for(contador = 1; contador <=numeroChavesParam; contador++)
    {
             fprintf(arquivo, "Cod.\tMRAN\t%ld\tCod.\tOperacional\t%s\tEstado\t%d\n", listaChavesParam[contador].identificador, listaChavesParam[contador].codOperacional, listaChavesParam[contador].estadoChave);
    }
    fclose(arquivo);

}

void salvaChavesCompleto(long int numeroChavesParam, LISTACHAVES *listaChavesParam)
{
    FILE *arquivo;
    long int contador;
    
    arquivo = fopen("chavesCompleto.dad","w");
    fprintf(arquivo,"%ld\n", numeroChavesParam);
    for(contador = 1; contador <=numeroChavesParam; contador++)
    {
        fprintf(arquivo, "%ld\t%s\t%d\t%ld\t%ld\t%d\t%d\t%d\n", listaChavesParam[contador].identificador, listaChavesParam[contador].codOperacional, listaChavesParam[contador].estadoChave, listaChavesParam[contador].idNoDe, listaChavesParam[contador].idNoPara, listaChavesParam[contador].tipoChave, listaChavesParam[contador].subTipoChave, listaChavesParam[contador].condicao);
    }
    fclose(arquivo);

}

void salvaChavesAMontante(LISTACHAVES *listaChavesParam, CONFIGURACAO individuo, GRAFOSETORES *grafoSetores)
{
    FILE *arquivo;
    long int contador, contadorRnp;
    long int ancestral[200];
    long int idChave;
    //RNPSETOR *rnpSetor;
    arquivo = fopen("chaves_amontante.dad","w");
    for (contadorRnp = 0; contadorRnp < individuo.numeroRNP; contadorRnp++){
        ancestral[individuo.rnp[contadorRnp].nos[0].profundidade] = individuo.rnp[contadorRnp].nos[0].idNo;
        for (contador = 1; contador < individuo.rnp[contadorRnp].numeroNos; contador++) {
             idChave = buscaChaveNF(listaChavesParam, individuo.rnp[contadorRnp].nos[contador].idNo,ancestral[individuo.rnp[contadorRnp].nos[contador].profundidade-1], grafoSetores);   
            if(idChave != -1)
            {
                fprintf(arquivo, "Setor\t%ld\tCod.\tMRAN\t%ld\tCod.\tOperacional\t%s\n", individuo.rnp[contadorRnp].nos[contador].idNo, listaChavesParam[idChave].identificador, listaChavesParam[idChave].codOperacional);
            }
            ancestral[individuo.rnp[contadorRnp].nos[contador].profundidade] = individuo.rnp[contadorRnp].nos[contador].idNo;   
        }
    }
    fclose(arquivo);
}

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

}

void gravaMatrizZCorrente(MATRIZCOMPLEXA *ZParam, MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam) {
    long int indiceNos;
    int indiceAdjacentes;
    FILE *arquivo = fopen("matrizImpedanciaCorrente.dad", "w");
    fprintf(arquivo, "%ld\n", numeroBarrasParam);
    //percorre a lista de barras do sistema no grafo
    for (indiceNos = 1; indiceNos <= numeroBarrasParam; indiceNos++) {
        //inclui os valores na lista inicial da matriz Z
         fprintf(arquivo, "%ld\t%d\n", ZParam[indiceNos].idNo,ZParam[indiceNos].numeroAdjacentes); 
       //aloca o vetor de adjacente do tindicePo celulacomplexa para conter os valores necessários
        //percorre a lista de adjacentes calculando a impedância
        for (indiceAdjacentes = 0; indiceAdjacentes < ZParam[indiceNos].numeroAdjacentes; indiceAdjacentes++) {
          fprintf(arquivo, "%ld\t%lf\t%lf\t%lf\n", ZParam[indiceNos].noAdjacentes[indiceAdjacentes].idNo, 
                   __real__ ZParam[indiceNos].noAdjacentes[indiceAdjacentes].valor, 
                  __imag__ ZParam[indiceNos].noAdjacentes[indiceAdjacentes].valor,
                  maximoCorrenteParam[indiceNos].noAdjacentes[indiceAdjacentes].valor);
        }
        // printf("\n\n");
    }
    fclose(arquivo);
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
 */
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
}

void gravaMatrizCorrente(MATRIZMAXCORRENTE *maximoCorrenteParam, long int numeroBarrasParam) {
    long int indiceNos;
    int indiceAdjacentes;
    FILE *arquivo = fopen("matrizCorrente.dad", "w");
    fprintf(arquivo, "%ld\n", numeroBarrasParam);
    //percorre a lista de barras do sistema no grafo
    for (indiceNos = 1; indiceNos <= numeroBarrasParam; indiceNos++) {
        //inclui os valores na lista inicial da matriz Z
         fprintf(arquivo, "%ld\t%d\n", maximoCorrenteParam[indiceNos].idNo,maximoCorrenteParam[indiceNos].numeroAdjacentes); 
       //aloca o vetor de adjacente do tindicePo celulacomplexa para conter os valores necessários
        //percorre a lista de adjacentes calculando a impedância
        for (indiceAdjacentes = 0; indiceAdjacentes < maximoCorrenteParam[indiceNos].numeroAdjacentes; indiceAdjacentes++) {
          fprintf(arquivo, "%ld\t%lf\n", maximoCorrenteParam[indiceNos].noAdjacentes[indiceAdjacentes].idNo, 
                   maximoCorrenteParam[indiceNos].noAdjacentes[indiceAdjacentes].valor);
        }
        // printf("\n\n");
    }
    fclose(arquivo);
}

void gravaBarrasRT(DADOSREGULADOR *dadosReguladorParam, long int numeroReguladoresParam, GRAFO *grafoSDRParam, long int numeroBarrasParam)
{
    long int indiceNo1, indiceNo2, indiceRegulador;
    char idRegulador[30];
    FILE *arquivo = fopen("barrasRegulador.dad", "w");
    fprintf(arquivo, "%ld\n", numeroReguladoresParam);

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
                        fprintf(arquivo, "%s\t%ld\t%ld\n", idRegulador, grafoSDRParam[indiceNo1].idNo, grafoSDRParam[indiceNo1].adjacentes[indiceNo2].idNo);
                    }
                }
            }
        }
    }
    fclose(arquivo);
}
