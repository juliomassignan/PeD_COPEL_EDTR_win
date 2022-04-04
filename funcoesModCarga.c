#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <time.h>


#include "data_structures.h"
#include "data_structures_modcarga.h"
#include "funcoesLeitura.h"
#include "funcoesTopologia.h"
#include "funcoesModCarga.h"


BOOL ligadoPorMedidor(NOADJACENTE adjacente) {
    if(adjacente.nmed > 0) return true;
    return false;
}

BOOL barraMedClasseA(GRAFO no) {
    if(no.nmedPQ >0) return true;    
    return false;
}




//------------------------------------------------------------------------------
// Leitura das tabelas de dados
void leituraTabelaConsumidores(DADOS_CONSUMIDOR **consumidores)
{
    FILE *arquivo;
    CURVA_CARGA *curvasCarga;
    DADOS_CNAE *dadosCNAE;
    
    
    // Leitura da tabela de curvas de carga típicas
    arquivo = fopen("CURVA_HORARIA_PU.dad","r");
    if(arquivo == NULL)
    {
            printf("Erro ao abrir arquivo CURVA_HORARIA_PU.dad !!!\n");
            exit(1);
    }
    leituraCurvasCarga(arquivo,&curvasCarga);
    fclose(arquivo);
    
     // Leitura da tabela de dados CNAE
    arquivo = fopen("GRUPO_ATIVIDADE_UC.dad","r");
    if(arquivo == NULL)
    {
            printf("Erro ao abrir arquivo GRUPO_ATIVIDADE_UC.dad !!!\n");
            exit(1);
    }
    leituraDadosCNAE(arquivo,&dadosCNAE);
    fclose(arquivo);
    
    // Leitura da tabela de dados de consumidores
    arquivo = fopen("CONSUMIDORES.dad","r");
    if(arquivo == NULL)
    {
            printf("Erro ao abrir arquivo CONSUMIDORES.dad !!!\n");
            exit(1);
    }
    leituraConsumidores(arquivo,consumidores,curvasCarga,dadosCNAE);
    fclose(arquivo);
    
}

//------------------------------------------------------------------------------
// Leitura da Tabela de Curvas de Carga Típicas
void leituraCurvasCarga(FILE *arquivo, CURVA_CARGA **curvas)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados; /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int contador =0, i; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac,numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */

    
    //Aloca na memória espaço para as curvas de carga tíipicas
    while ((carac = fgetc(arquivo)) != EOF) {
      if (carac == '\n')
        numLinhas++;
    }
    rewind(arquivo);
    if (((*curvas) = (CURVA_CARGA *)malloc( (numLinhas) * sizeof(CURVA_CARGA)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as curvas de carga !!!!");
        exit(1); 
    }
    (*curvas)[0].total = numLinhas-1;
    
    // Le da primeira linha (Cabeçalho)
    fgets(blocoLeitura, 2000, arquivo);
    
    
    // Le o arquivo de curva de cargas até o fim
    while( (fgets(blocoLeitura, 2000, arquivo))!= NULL ){
        dados = blocoLeitura;
        
        // Atribui os dados à struct curvas_carga
        sscanf(getfield(dados,1),"%20c",(*curvas)[contador].id);
        (*curvas)[contador].id[20]=0;
        (*curvas)[contador].tamanho = NPTS_CURVA;
        (*curvas)[contador].minimo = getfield_double(dados,2);
        (*curvas)[contador].maximo = getfield_double(dados,3);
        if ((*curvas)[contador].maximo == 999)
                (*curvas)[contador].maximo = 99999999;
                
        (*curvas)[contador].media = (double *) malloc((*curvas)[contador].tamanho*sizeof(double));
        (*curvas)[contador].std_dev = (double *) malloc((*curvas)[contador].tamanho*sizeof(double));
        for (i = 0; i<(*curvas)[contador].tamanho; i++){
            (*curvas)[contador].media[i] = getfield_double(dados,i+4);
            (*curvas)[contador].std_dev[i] = getfield_double(dados,i+4+NPTS_CURVA);
        }
        contador++;
    }
}

//------------------------------------------------------------------------------
// Leitura da Tabela dos dados CNAE
void leituraDadosCNAE(FILE *arquivo, DADOS_CNAE **cnae)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados; /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int contador =1; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac, numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */
    
    //Aloca na memória espaço para as curvas de carga tíipicas
    while ((carac = fgetc(arquivo)) != EOF) {
      if (carac == '\n')
        numLinhas++;
    }
    rewind(arquivo);
    if (((*cnae) = (DADOS_CNAE *)malloc( (numLinhas) * sizeof(DADOS_CNAE)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para os dados CNAE!!!!");
        exit(1); 
    }
    
    (*cnae)[0].total = numLinhas-1;
    
    // Le da primeira linha (Cabeçalho)
    fgets(blocoLeitura, 2000, arquivo);
    
    // Le o arquivo de curva de cargas até o fim
    while( (fgets(blocoLeitura, 2000, arquivo))!= NULL ){
        dados = blocoLeitura;
        
        // Atribui os dados à struct cnae
        sscanf(getfield(dados,1),"%20c",(*cnae)[contador].curva_carga);
        sscanf(getfield(dados,2),"%6c",(*cnae)[contador].cod_ativ);
        sscanf(getfield(dados,3),"%60c",(*cnae)[contador].desc_ativ);
        sscanf(getfield(dados,4),"%7c",(*cnae)[contador].cod_ativ_cnae);
        (*cnae)[contador].cod_ativ_cnae[7]=0;
       // sscanf(getfield(dados,4),"%ld",(*cnae)[contador].cod_ativ_cnae);
        contador++;
    }
}

//------------------------------------------------------------------------------
// Leitura da Tabela de Consumidores
void leituraConsumidores(FILE *arquivo, DADOS_CONSUMIDOR **consumidores, CURVA_CARGA *curvas, DADOS_CNAE *cnae)
{
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados; /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    int contador =0, i, j, k; /* Variáveis contadores para percorrer o arquivo e a string de leitura. */
    int carac,numLinhas = 0; /* Variável com o número de linhas do arquivo a serem lidas. */
    
    
    //Aloca na memória espaço para as curvas de carga tíipicas
    while ((carac = fgetc(arquivo)) != EOF) {
      if (carac == '\n')
        numLinhas++;
    }
    
    
    rewind(arquivo);
    if (((*consumidores) = (DADOS_CONSUMIDOR *)malloc( (numLinhas+1) * sizeof(DADOS_CONSUMIDOR)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para os dados de consumidores !!!!");
        exit(1); 
    }
    (*consumidores)[0].total = numLinhas-1;
    
    // Le da primeira linha (Cabeçalho)
    fgets(blocoLeitura, 2000, arquivo);
    
    // Le o arquivo de curva de cargas até o fim
    while( (fgets(blocoLeitura, 2000, arquivo))!= NULL ){
        dados = blocoLeitura;
        
        // Atribui os dados à struct consumidores
        if ((strcmp(getfield(dados,12),"LG"))==0 || (strcmp(getfield(dados,12),"CR")==0)) {
            //------------------------------------------------------------------
            // Leitura dos dados de consumidores do arquivo CSV
            (*consumidores)[contador].num_cli_uc = getfield_int(dados,1);
            (*consumidores)[contador].fase_rede_uc = getfield_int(dados,14);
            
            
            if (getfield_double(dados,2) == getfield_double(dados,3)){ //Consumo de ponta = a consumo fora de ponta
                (*consumidores)[contador].cons_kWh_mes = getfield_double(dados,2);
            }
            else{    
                (*consumidores)[contador].cons_kWh_mes = getfield_double(dados,2) + getfield_double(dados,3);
            }
            if (getfield_double(dados,4) == getfield_double(dados,5)){ //Consumo de ponta = a consumo fora de ponta
                (*consumidores)[contador].cons_kVArh_mes = getfield_double(dados,4);
            }
            else{
                (*consumidores)[contador].cons_kVArh_mes = getfield_double(dados,4) + getfield_double(dados,5);
            }
            
            
            sscanf(getfield(dados,13),"%7c",(*consumidores)[contador].cod_ativ_cnae_uc);
            (*consumidores)[contador].cod_ativ_cnae_uc[7]=0;
            //printf("contador %d \n", contador);
            if (strlen((*consumidores)[contador].cod_ativ_cnae_uc)==0)
                strcpy((*consumidores)[contador].cod_ativ_cnae_uc,"       ");
            //(*consumidores)[contador].cod_ativ_cnae_uc = atol(replace(getfield(dados,13)));
            
            sscanf(getfield(dados,15),"%10c",(*consumidores)[contador].numero_posto_UC);
            (*consumidores)[contador].no = getfield_int(dados,19);
            sscanf(getfield(dados,16),"%2c",(*consumidores)[contador].cod_nivel_tensao_uc);
            
            //------------------------------------------------------------------
            //Grandezas calculadas
            (*consumidores)[contador].Pbase = (*consumidores)[contador].cons_kWh_mes/(24*30);
            (*consumidores)[contador].Qbase = (*consumidores)[contador].cons_kVArh_mes/(24*30);
            //Atribui o fator de potência igual a FP_TIPICO caso Qbase = 0, caso contrário assume o fp medido pelos valores de kWh e kVArh
            if ((*consumidores)[contador].Qbase == 0)
                {(*consumidores)[contador].fp = FP_TIPICO;}
            else
                {(*consumidores)[contador].fp = cos(atan((*consumidores)[contador].Qbase/(*consumidores)[contador].Pbase));}
                                    
            //------------------------------------------------------------------
            //Busca a curva de carga do consumidor na tabela CNAE e na tabela de Curvas
            //Caso não encontrar o código CNAE assume a curva padrão
            //Caso não encontrar a curva típica assume a curva padrão
            i=1;
            j=0;
            while (i <= cnae[0].total){
                if (strcmp((*consumidores)[contador].cod_ativ_cnae_uc,cnae[i].cod_ativ_cnae)==0)
            //    if((*consumidores)[contador].cod_ativ_cnae_uc == cnae[i].cod_ativ_cnae)
                {
                    //Busca Curva
                    while (j <= curvas[0].total){
                        if (strncmp(cnae[i].curva_carga,curvas[j].id,20)==0 && (*consumidores)[contador].cons_kWh_mes >= curvas[j].minimo && (*consumidores)[contador].cons_kWh_mes < curvas[j].maximo)
                        {
                            strcpy((*consumidores)[contador].curva_carga.id,curvas[j].id);
                            (*consumidores)[contador].curva_carga.tamanho = curvas[j].tamanho;
                            (*consumidores)[contador].curva_carga.minimo = curvas[j].minimo;
                            (*consumidores)[contador].curva_carga.maximo = curvas[j].maximo;
                            (*consumidores)[contador].curva_carga.media = (double *) malloc(curvas[j].tamanho*sizeof(double));
                            (*consumidores)[contador].curva_carga.std_dev = (double *) malloc(curvas[j].tamanho*sizeof(double));
                            for (k=0;k<curvas[j].tamanho;k++){
                                (*consumidores)[contador].curva_carga.media[k] = curvas[j].media[k];
                                (*consumidores)[contador].curva_carga.std_dev[k] = curvas[j].std_dev[k];
                            }
                            break;
                        }                        
                        j++;
                    }
                    //Busca Curva Padrão caso não encontra a curva do código CNAE
                    if (j == curvas[0].total+1){
                        j=0;
                        while (j <= curvas[0].total){
                            if (strncmp("PADRAO              ",curvas[j].id,20)==0 && (*consumidores)[contador].cons_kWh_mes >= curvas[j].minimo && (*consumidores)[contador].cons_kWh_mes < curvas[j].maximo)
                            {
                                strcpy((*consumidores)[contador].curva_carga.id,curvas[j].id);
                                (*consumidores)[contador].curva_carga.tamanho = curvas[j].tamanho;
                                (*consumidores)[contador].curva_carga.minimo = curvas[j].minimo;
                                (*consumidores)[contador].curva_carga.maximo = curvas[j].maximo;
                                (*consumidores)[contador].curva_carga.media = (double *) malloc(curvas[j].tamanho*sizeof(double));
                                (*consumidores)[contador].curva_carga.std_dev = (double *) malloc(curvas[j].tamanho*sizeof(double));
                                for (k=0;k<curvas[j].tamanho;k++){
                                    (*consumidores)[contador].curva_carga.media[k] = curvas[j].media[k];
                                    (*consumidores)[contador].curva_carga.std_dev[k] = curvas[j].std_dev[k];
                                }
                                break;
                            }                        
                            j++;
                        } 
                    }
                    break;
                }    
                i++;
            }
            
            //Busca Curva Padrão caso não encontra o código CNAE
            if (i == cnae[0].total+1){
                while (j <= curvas[0].total){
                    if (strncmp("PADRAO              ",curvas[j].id,20)==0 && (*consumidores)[contador].cons_kWh_mes >= curvas[j].minimo && (*consumidores)[contador].cons_kWh_mes < curvas[j].maximo)
                    {
                        strcpy((*consumidores)[contador].curva_carga.id,curvas[j].id);
                        (*consumidores)[contador].curva_carga.tamanho = curvas[j].tamanho;
                        (*consumidores)[contador].curva_carga.minimo = curvas[j].minimo;
                        (*consumidores)[contador].curva_carga.maximo = curvas[j].maximo;
                        (*consumidores)[contador].curva_carga.media = (double *) malloc(curvas[j].tamanho*sizeof(double));
                        (*consumidores)[contador].curva_carga.std_dev = (double *) malloc(curvas[j].tamanho*sizeof(double));
                        for (k=0;k<curvas[j].tamanho;k++){
                            (*consumidores)[contador].curva_carga.media[k] = curvas[j].media[k];
                            (*consumidores)[contador].curva_carga.std_dev[k] = curvas[j].std_dev[k];
                        }
                        break;
                    }                        
                    j++;
                } 
            }
            
            //------------------------------------------------------------------
            // Multiplica a curva de carga pela potência base
            i=0;
            while (i < (*consumidores)[contador].curva_carga.tamanho){
                (*consumidores)[contador].curva_carga.media[i] = (*consumidores)[contador].curva_carga.media[i]*((*consumidores)[contador].Pbase);
                (*consumidores)[contador].curva_carga.std_dev[i] = (*consumidores)[contador].curva_carga.std_dev[i]*((*consumidores)[contador].Pbase);        
                i++;
            }
            contador++;
        }
        
    }
    (*consumidores)[0].total = contador;
}

//------------------------------------------------------------------------------
// Leitura da Curvas Agregadas quando não for executar funçoes de agregação
void leituraCurvasAgregadas(char *folder, CURVA_TRAFO **curvasTrafos, DBAR *barras, long int numeroBarrasParam)
{
    int contador = 0,tempo,npts,no,k, i;
    char blocoLeitura[2000]; /* Variável para realizar a leitura do bloco de caracteres do arquivo. */
    char *dados; /* Variável do tipo ponteiro para char, utilizada para alterar o ponteiro da string lida do arquivo de forma a realizar o loop no sscanf. */
    FILE *arquivo;
    char text_aux[500];
    double somaP, somaQ, fat_desb_P[3], fat_desb_Q[3];
    
        
    //--------------------------------------------------------------------------
    // Inicializa as potências as curvas de carga agregadas
    contador = 0;
    
    //Aloca na memória espaço para as curvas de carga
    (*curvasTrafos) = (CURVA_TRAFO *) malloc((numeroBarrasParam+1)*sizeof(CURVA_TRAFO));
    npts = NPTS_CURVA;
    while (contador < numeroBarrasParam+1){
        (*curvasTrafos)[contador].id = -1;
        (*curvasTrafos)[contador].tamanho = 0;
        contador++;
    }

    // Leitura da tabela de curvas de carga típicas
    // Leitura dos dados de medidores
    strcpy(text_aux,folder);
    arquivo = fopen(strcat(text_aux,"CURVAS_AGREGADAS_P.dad"),"r");
    if(arquivo == NULL)
    {
        printf("Erro ao abrir arquivo CURVAS_AGREGADAS_P.dad !!!\n");
    }
    else
    {
        // Le da primeira linha (Cabeçalho)
//        fgets(blocoLeitura, 2000, arquivo);
        
        
        // Le o arquivo de curva de cargas até o fim
        while( (fgets(blocoLeitura, 5000, arquivo))!= NULL ){
            dados = blocoLeitura;
            
            no = getfield_int(dados,1);
            npts = getfield_int(dados,2);
            
            (*curvasTrafos)[no].PA = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].stdPA = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].PB = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].stdPB = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].PC = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].stdPC = (double *) malloc(npts*sizeof(double));

            (*curvasTrafos)[no].QA = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].stdQA = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].QB = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].stdQB = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].QC = (double *) malloc(npts*sizeof(double));
            (*curvasTrafos)[no].stdQC = (double *) malloc(npts*sizeof(double));
            
            k=0;
            somaP = 0;
            for (i=0;i<3;i++) somaP += barras[no].loads[k].Pnom[i];
            if (somaP == 0) somaP = 1;
            somaQ = 0;
            for (i=0;i<3;i++) somaQ += barras[no].loads[k].Qnom[i];
            if (somaQ == 0) somaQ = 1;

            fat_desb_P[0] =  barras[no].loads[k].Pnom[0] / somaP;
            fat_desb_P[1] =  barras[no].loads[k].Pnom[1] / somaP;
            fat_desb_P[2] =  barras[no].loads[k].Pnom[2] / somaP;
            fat_desb_Q[0] =  barras[no].loads[k].Qnom[0] / somaQ;
            fat_desb_Q[1] =  barras[no].loads[k].Qnom[1] / somaQ;
            fat_desb_Q[2] =  barras[no].loads[k].Qnom[2] / somaQ;
                            
            (*curvasTrafos)[no].id = no;      
            for (tempo=0;tempo<npts;tempo++){
//                (*curvasTrafos)[no].PA[tempo] = fat_desb_P[0] * 3*getfield_double(dados,2 + tempo +1) * 1000;
//                (*curvasTrafos)[no].PB[tempo] = fat_desb_P[1] * 3*getfield_double(dados,2 + tempo +1) * 1000;//getfield_double(dados,2 + tempo +1+ npts)* 1000;
//                (*curvasTrafos)[no].PC[tempo] = fat_desb_P[2] * 3*getfield_double(dados,2 + tempo +1) * 1000;//getfield_double(dados,2 + tempo +1+ 2*npts) * 1000;
                (*curvasTrafos)[no].PA[tempo] = getfield_double(dados,2 + tempo +1) * 1000;
                (*curvasTrafos)[no].PB[tempo] = getfield_double(dados,2 + tempo +1+ npts)* 1000;
                (*curvasTrafos)[no].PC[tempo] = getfield_double(dados,2 + tempo +1+ 2*npts) * 1000;
                
                (*curvasTrafos)[no].stdPA[tempo] = getfield_double(dados,2 + tempo +1+ 3*npts)* 1000;
                (*curvasTrafos)[no].stdPB[tempo] = getfield_double(dados,2 + tempo +1+ 4*npts)* 1000;
                (*curvasTrafos)[no].stdPC[tempo] = getfield_double(dados,2 + tempo +1+ 5*npts)* 1000;
                
                //Assume fator de potência típico de 0.92 - mas pode ser alterado caso exista arquivo de curvas Q
                (*curvasTrafos)[no].QA[tempo] = (*curvasTrafos)[no].PA[tempo] * tan(acos(0.92));
                (*curvasTrafos)[no].QB[tempo] = (*curvasTrafos)[no].PB[tempo] * tan(acos(0.92));
                (*curvasTrafos)[no].QC[tempo] = (*curvasTrafos)[no].PC[tempo] * tan(acos(0.92));
                (*curvasTrafos)[no].stdQA[tempo] = (*curvasTrafos)[no].stdPA[tempo] * tan(acos(0.92));
                (*curvasTrafos)[no].stdQB[tempo] = (*curvasTrafos)[no].stdPB[tempo] * tan(acos(0.92));
                (*curvasTrafos)[no].stdQC[tempo] = (*curvasTrafos)[no].stdPC[tempo] * tan(acos(0.92));
            }
            barras[no].nloads = 1;
            k = 0;
            barras[no].loads[k].ID = no;
            barras[no].loads[k].Vbase = barras[no].Vbase;
            barras[no].loads[k].fases = barras[no].fases;
            barras[no].loads[k].lig = YN;
            barras[no].loads[k].ZIP = 0;

//            barras[no].loads[k].Pnom[0] = (*curvasTrafos)[no].PA[0];
//            barras[no].loads[k].Pnom[1] = (*curvasTrafos)[no].PB[0];
//            barras[no].loads[k].Pnom[2] = (*curvasTrafos)[no].PC[0];
//            barras[no].loads[k].Qnom[0] = (*curvasTrafos)[no].QA[0];
//            barras[no].loads[k].Qnom[1] = (*curvasTrafos)[no].QB[0];
//            barras[no].loads[k].Qnom[2] = (*curvasTrafos)[no].QC[0];

        }
        fclose(arquivo);
    }
    
    //Parcela Reativa das Curvas de Carga
    arquivo = fopen("CURVAS_AGREGADAS_Q.dad","r"); //Arquivo com curvas agregadas reativas - opcional
    if(arquivo != NULL)
    {
        //Encontrou arquivo de curvas reativas 
        // Le o arquivo de curva de cargas até o fim
        while( (fgets(blocoLeitura, 5000, arquivo))!= NULL ){
            no = getfield_int(dados,1);
            npts = getfield_int(dados,2);
            
            if ((*curvasTrafos)[no].QA != NULL){
                (*curvasTrafos)[no].tamanho = npts;
                for (tempo=0;tempo<npts;tempo++)
                    (*curvasTrafos)[no].QA[tempo] = getfield_double(dados,2 + tempo+1)* 1000;
                    (*curvasTrafos)[no].QB[tempo] = getfield_double(dados,2 + tempo +1 + npts)* 1000;
                    (*curvasTrafos)[no].QC[tempo] = getfield_double(dados,2 + tempo +1+ 2*npts)* 1000;
                    (*curvasTrafos)[no].stdQA[tempo] = getfield_double(dados,2 + tempo +1+ 3*npts)* 1000;
                    (*curvasTrafos)[no].stdQB[tempo] = getfield_double(dados,2 + tempo +1+ 4*npts)* 1000;
                    (*curvasTrafos)[no].stdQC[tempo] = getfield_double(dados,2 + tempo +1+ 5*npts)* 1000;
                }
            contador++;
        }
        fclose(arquivo);
    }
    
    
}


//------------------------------------------------------------------------------
// Atualiza os valores de P e Q para o valor agregado na estampa de tempo
void inicializaPQcurvas(DBAR *barras, CURVA_TRAFO *curvasTrafos, long int numeroBarrasParam,  long int estampa_tempo, TIPO_AGREGACAO_CARGAS tipo, double Sbase)
{
    int contador, i, k;
    double fat_desb_P[3],fat_desb_Q[3], somaP, somaQ;
    
    //--------------------------------------------------------------------------
    // Inicialização das curvas de cargas - Cargas adcionais ao já definido no DBAR
    contador = 0;
    while (contador < numeroBarrasParam+1){
        //Sem Majorante de Carga
        switch (tipo){
            case VALOR_ESPERADO:
                if (curvasTrafos[contador].id != -1){
                    //   k = barras[contador].nloads;
                    k=0;
                    barras[contador].loads[k].Pnom[0]  = curvasTrafos[contador].PA[estampa_tempo]/Sbase;
                    barras[contador].loads[k].Pnom[1]  = curvasTrafos[contador].PB[estampa_tempo]/Sbase;
                    barras[contador].loads[k].Pnom[2]  = curvasTrafos[contador].PC[estampa_tempo]/Sbase;
                    barras[contador].loads[k].Qnom[0]  = curvasTrafos[contador].QA[estampa_tempo]/Sbase;
                    barras[contador].loads[k].Qnom[1]  = curvasTrafos[contador].QB[estampa_tempo]/Sbase;
                    barras[contador].loads[k].Qnom[2]  = curvasTrafos[contador].QC[estampa_tempo]/Sbase;
                }
                break;
            case MAJORANTE_QUANTIL:
                if (curvasTrafos[contador].id != -1){
                    // k = barras[contador].nloads;
                    k=0;
                    barras[contador].loads[k].Pnom[0]  = (curvasTrafos[contador].PA[estampa_tempo] + FAT_K_AGREG * curvasTrafos[contador].stdPA[estampa_tempo])/Sbase;
                    barras[contador].loads[k].Pnom[1]  = (curvasTrafos[contador].PB[estampa_tempo] + FAT_K_AGREG * curvasTrafos[contador].stdPB[estampa_tempo])/Sbase;
                    barras[contador].loads[k].Pnom[2]  = (curvasTrafos[contador].PC[estampa_tempo] + FAT_K_AGREG * curvasTrafos[contador].stdPB[estampa_tempo])/Sbase;
                    barras[contador].loads[k].Qnom[0]  = (curvasTrafos[contador].QA[estampa_tempo] + FAT_K_AGREG * curvasTrafos[contador].stdQA[estampa_tempo])/Sbase;
                    barras[contador].loads[k].Qnom[1]  = (curvasTrafos[contador].QB[estampa_tempo] + FAT_K_AGREG * curvasTrafos[contador].stdQB[estampa_tempo])/Sbase;
                    barras[contador].loads[k].Qnom[2]  = (curvasTrafos[contador].QC[estampa_tempo] + FAT_K_AGREG * curvasTrafos[contador].stdQC[estampa_tempo])/Sbase;
                }
                break;
            case EQUILIBRADO_MAXIMO:
                if (curvasTrafos[contador].id != -1){
                    // k = barras[contador].nloads;
                    k=0;
                    if ((curvasTrafos[contador].PA[estampa_tempo] >= curvasTrafos[contador].PB[estampa_tempo]) && (curvasTrafos[contador].PA[estampa_tempo] >= curvasTrafos[contador].PC[estampa_tempo])){
                        barras[contador].loads[k].Pnom[0]  = curvasTrafos[contador].PA[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Pnom[1]  = curvasTrafos[contador].PA[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Pnom[2]  = curvasTrafos[contador].PA[estampa_tempo]/Sbase;
                    }
                    if ((curvasTrafos[contador].PB[estampa_tempo] > curvasTrafos[contador].PA[estampa_tempo]) && (curvasTrafos[contador].PB[estampa_tempo] > curvasTrafos[contador].PC[estampa_tempo])){
                        barras[contador].loads[k].Pnom[0]  = curvasTrafos[contador].PB[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Pnom[1]  = curvasTrafos[contador].PB[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Pnom[2]  = curvasTrafos[contador].PB[estampa_tempo]/Sbase;
                    }
                    if ((curvasTrafos[contador].PC[estampa_tempo] > curvasTrafos[contador].PA[estampa_tempo]) && (curvasTrafos[contador].PC[estampa_tempo] > curvasTrafos[contador].PB[estampa_tempo])){
                        barras[contador].loads[k].Pnom[0]  = curvasTrafos[contador].PC[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Pnom[1]  = curvasTrafos[contador].PC[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Pnom[2]  = curvasTrafos[contador].PC[estampa_tempo]/Sbase;
                    }
                    if ((curvasTrafos[contador].QA[estampa_tempo] >= curvasTrafos[contador].QB[estampa_tempo]) && (curvasTrafos[contador].QA[estampa_tempo] >= curvasTrafos[contador].QC[estampa_tempo])){
                        barras[contador].loads[k].Qnom[0]  = curvasTrafos[contador].QA[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Qnom[1]  = curvasTrafos[contador].QA[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Qnom[2]  = curvasTrafos[contador].QA[estampa_tempo]/Sbase;
                    }
                    if ((curvasTrafos[contador].QB[estampa_tempo] > curvasTrafos[contador].QA[estampa_tempo]) && (curvasTrafos[contador].QB[estampa_tempo] > curvasTrafos[contador].QC[estampa_tempo])){
                        barras[contador].loads[k].Qnom[0]  = curvasTrafos[contador].QB[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Qnom[1]  = curvasTrafos[contador].QB[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Qnom[2]  = curvasTrafos[contador].QB[estampa_tempo]/Sbase;
                    }
                    if ((curvasTrafos[contador].QC[estampa_tempo] > curvasTrafos[contador].QA[estampa_tempo]) && (curvasTrafos[contador].QC[estampa_tempo] > curvasTrafos[contador].QB[estampa_tempo])){
                        barras[contador].loads[k].Qnom[0]  = curvasTrafos[contador].QC[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Qnom[1]  = curvasTrafos[contador].QC[estampa_tempo]/Sbase;
                        barras[contador].loads[k].Qnom[2]  = curvasTrafos[contador].QC[estampa_tempo]/Sbase;
                    }

                }
                break;
             case DESBALANCEADO_INPUT_DBAR:
                if (curvasTrafos[contador].id != -1){
                    // k = barras[contador].nloads;
                    k=0;
                    
                    barras[contador].loads[k].Pnom[0]  = (curvasTrafos[contador].PA[estampa_tempo] )/Sbase;
                    barras[contador].loads[k].Pnom[1]  = (curvasTrafos[contador].PB[estampa_tempo] )/Sbase;
                    barras[contador].loads[k].Pnom[2]  = (curvasTrafos[contador].PC[estampa_tempo] )/Sbase;
                    barras[contador].loads[k].Qnom[0]  = (curvasTrafos[contador].QA[estampa_tempo] )/Sbase;
                    barras[contador].loads[k].Qnom[1]  = (curvasTrafos[contador].QB[estampa_tempo] )/Sbase;
                    barras[contador].loads[k].Qnom[2]  = (curvasTrafos[contador].QC[estampa_tempo] )/Sbase;
                }    
                break;
        }
        
        contador++;
    }   
    
}



//------------------------------------------------------------------------------
// Executa a agregação de cargas
void agregacaoCargas(CURVA_TRAFO **curvasTrafos, long int numeroBarrasParam, DADOS_CONSUMIDOR *consumidores)
{
    int contador,tempo,npts,no;
    
    (*curvasTrafos) = (CURVA_TRAFO *) malloc((numeroBarrasParam+1)*sizeof(CURVA_TRAFO));
    
    //--------------------------------------------------------------------------
    // Inicializa as potências as curvas de carga agregadas
    contador = 0;
    npts = consumidores[0].curva_carga.tamanho;
    while (contador < numeroBarrasParam+1){
        (*curvasTrafos)[contador].id = 0;
        (*curvasTrafos)[contador].PA = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].stdPA = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].PB = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].stdPB = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].PC = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].stdPC = (double *) malloc(npts*sizeof(double));
        
        (*curvasTrafos)[contador].QA = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].stdQA = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].QB = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].stdQB = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].QC = (double *) malloc(npts*sizeof(double));
        (*curvasTrafos)[contador].stdQC = (double *) malloc(npts*sizeof(double));
        
        (*curvasTrafos)[contador].tamanho = npts;
        for (tempo=0;tempo<npts;tempo++){
            (*curvasTrafos)[contador].PA[tempo] = 0;
            (*curvasTrafos)[contador].stdPA[tempo] = 0;
            (*curvasTrafos)[contador].PB[tempo] = 0;
            (*curvasTrafos)[contador].stdPB[tempo] = 0; 
            (*curvasTrafos)[contador].PC[tempo] = 0;
            (*curvasTrafos)[contador].stdPC[tempo] = 0; 

            (*curvasTrafos)[contador].QA[tempo] = 0;
            (*curvasTrafos)[contador].stdQA[tempo] = 0; 
            (*curvasTrafos)[contador].QB[tempo] = 0;
            (*curvasTrafos)[contador].stdQB[tempo] = 0; 
            (*curvasTrafos)[contador].QC[tempo] = 0;
            (*curvasTrafos)[contador].stdQC[tempo] = 0;
        }
        contador++;
    }
    
    for (tempo=0;tempo<npts;tempo++){
    
        //--------------------------------------------------------------------------
        //Executa a agregação por fase
        contador = 0;
        while (contador < consumidores[0].total){
            consumidores[contador].P = consumidores[contador].curva_carga.media[tempo];
            consumidores[contador].std_P = consumidores[contador].curva_carga.std_dev[tempo];
            consumidores[contador].Q = consumidores[contador].P*tan(acos(consumidores[contador].fp));
            consumidores[contador].std_Q = consumidores[contador].std_P*tan(acos(consumidores[contador].fp));
            no = consumidores[contador].no;
            
            (*curvasTrafos)[no].id = 1;
            switch(consumidores[contador].fase_rede_uc){
                case 1:
                    (*curvasTrafos)[no].PA[tempo] += consumidores[contador].P;
                    (*curvasTrafos)[no].QA[tempo] += consumidores[contador].Q;
                    (*curvasTrafos)[no].stdPA[tempo] += pow(consumidores[contador].std_P,2);
                    (*curvasTrafos)[no].stdQA[tempo] += pow(consumidores[contador].std_Q,2);        
                    break;
                case 2:
                    (*curvasTrafos)[no].PB[tempo] += consumidores[contador].P;
                    (*curvasTrafos)[no].QB[tempo] += consumidores[contador].Q;
                    (*curvasTrafos)[no].stdPB[tempo] += pow(consumidores[contador].std_P,2);
                    (*curvasTrafos)[no].stdQB[tempo] += pow(consumidores[contador].std_Q,2); 
                    break;
                case 3:
                    (*curvasTrafos)[no].PC[tempo] += consumidores[contador].P;
                    (*curvasTrafos)[no].QC[tempo] += consumidores[contador].Q;
                    (*curvasTrafos)[no].stdPC[tempo] += pow(consumidores[contador].std_P,2);
                    (*curvasTrafos)[no].stdQC[tempo] += pow(consumidores[contador].std_Q,2); 
                    break;
                case 4:
                    (*curvasTrafos)[no].PA[tempo] += consumidores[contador].P/2;
                    (*curvasTrafos)[no].QA[tempo] += consumidores[contador].Q/2;
                    (*curvasTrafos)[no].stdPA[tempo] += pow(consumidores[contador].std_P/2,2);
                    (*curvasTrafos)[no].stdQA[tempo] += pow(consumidores[contador].std_Q/2,2);

                    (*curvasTrafos)[no].PB[tempo] += consumidores[contador].P/2;
                    (*curvasTrafos)[no].QB[tempo] += consumidores[contador].Q/2;
                    (*curvasTrafos)[no].stdPB[tempo] += pow(consumidores[contador].std_P/2,2);
                    (*curvasTrafos)[no].stdQB[tempo] += pow(consumidores[contador].std_Q/2,2); 
                    break;
                case 5:
                    (*curvasTrafos)[no].PA[tempo] += consumidores[contador].P/2;
                    (*curvasTrafos)[no].QA[tempo] += consumidores[contador].Q/2;
                    (*curvasTrafos)[no].stdPA[tempo] += pow(consumidores[contador].std_P/2,2);
                    (*curvasTrafos)[no].stdQA[tempo] += pow(consumidores[contador].std_Q/2,2);

                    (*curvasTrafos)[no].PC[tempo] += consumidores[contador].P/2;
                    (*curvasTrafos)[no].QC[tempo] += consumidores[contador].Q/2;
                    (*curvasTrafos)[no].stdPC[tempo] += pow(consumidores[contador].std_P/2,2);
                    (*curvasTrafos)[no].stdQC[tempo] += pow(consumidores[contador].std_Q/2,2); 
                    break;  
                case 6:
                    (*curvasTrafos)[no].PB[tempo] += consumidores[contador].P/2;
                    (*curvasTrafos)[no].QB[tempo] += consumidores[contador].Q/2;
                    (*curvasTrafos)[no].stdPB[tempo] += pow(consumidores[contador].std_P/2,2);
                    (*curvasTrafos)[no].stdQB[tempo] += pow(consumidores[contador].std_Q/2,2);  

                    (*curvasTrafos)[no].PC[tempo] += consumidores[contador].P/2;
                    (*curvasTrafos)[no].QC[tempo] += consumidores[contador].Q/2;
                    (*curvasTrafos)[no].stdPC[tempo] += pow(consumidores[contador].std_P/2,2);
                    (*curvasTrafos)[no].stdQC[tempo] += pow(consumidores[contador].std_Q/2,2);  
                    break;
                default: //Ligação = 7 ou diferente considera como trifásico
                    (*curvasTrafos)[no].PA[tempo] += consumidores[contador].P/3;
                    (*curvasTrafos)[no].QA[tempo] += consumidores[contador].Q/3;
                    (*curvasTrafos)[no].stdPA[tempo] += pow(consumidores[contador].std_P/3,2);
                    (*curvasTrafos)[no].stdQA[tempo] += pow(consumidores[contador].std_Q/3,2);

                    (*curvasTrafos)[no].PB[tempo] += consumidores[contador].P/3;
                    (*curvasTrafos)[no].QB[tempo] += consumidores[contador].Q/3;
                    (*curvasTrafos)[no].stdPB[tempo] += pow(consumidores[contador].std_P/3,2);
                    (*curvasTrafos)[no].stdQB[tempo] += pow(consumidores[contador].std_Q/3,2);  

                    (*curvasTrafos)[no].PC[tempo] += consumidores[contador].P/3;
                    (*curvasTrafos)[no].QC[tempo] += consumidores[contador].Q/3;
                    (*curvasTrafos)[no].stdPC[tempo] += pow(consumidores[contador].std_P/3,2);
                    (*curvasTrafos)[no].stdQC[tempo] += pow(consumidores[contador].std_Q/3,2);  
                    break;
            }

            contador++;
        }
        //Grava a variância nesta função no lugar do desvio padrão
    }
    
    
}

void imprimeConsumidores(DADOS_CONSUMIDOR *consumidores){
    int i, j;
    
    FILE *saida;
    saida = fopen("consumidores_out.dad","w");
    fprintf(saida,"NUM_CLIENTE;NUM_POSTO;BARRA;FASES;kWh mes;kVArh mes;kWBASE;kVArBASE;CNAE;CURVA;\n");
    for(i=0;i<consumidores[0].total;i++){
        fprintf(saida,"%d;%s;%ld;%d;%lf;%lf;%lf;%lf;%s;%s",consumidores[i].num_cli_uc,consumidores[i].numero_posto_UC,consumidores[i].no,consumidores[i].fase_rede_uc,consumidores[i].cons_kWh_mes,consumidores[i].cons_kVArh_mes,consumidores[i].Pbase,consumidores[i].Qbase,consumidores[i].cod_ativ_cnae_uc,consumidores[i].curva_carga.id);
        for(j=0;j<consumidores[i].curva_carga.tamanho;j++){
            fprintf(saida,";%lf",consumidores[i].curva_carga.media[j]);
        }
        for(j=0;j<consumidores[i].curva_carga.tamanho;j++){
            fprintf(saida,";%lf",consumidores[i].curva_carga.std_dev[j]);
        }
        fprintf(saida,"\n");
    }
    fclose(saida);
}

void imprimeCurvasAgregadas(CURVA_TRAFO *curvasTrafos, long int numeroBarrasParam){
    int i,j;
    
    FILE *saida;
    saida = fopen("cargasagregadas_out.dad","w");
    fprintf(saida,"BARRA;NPTS;P_t_kW;Pstd_t_kW;\n");
    for(i=0;i< numeroBarrasParam+1;i++){
        if (curvasTrafos[i].id==1){
            fprintf(saida,"%d;%ld;",i,curvasTrafos[i].tamanho);
            for(j=0;j<curvasTrafos[i].tamanho;j++){
                fprintf(saida,";%lf",curvasTrafos[i].PA[j]);
            }
            for(j=0;j<curvasTrafos[i].tamanho;j++){
                fprintf(saida,";%lf",curvasTrafos[i].PB[j]);
            }
            for(j=0;j<curvasTrafos[i].tamanho;j++){
                fprintf(saida,";%lf",curvasTrafos[i].PC[j]);
            }
            for(j=0;j<curvasTrafos[i].tamanho;j++){
                fprintf(saida,";%lf",curvasTrafos[i].stdPA[j]);
            }
            for(j=0;j<curvasTrafos[i].tamanho;j++){
                fprintf(saida,";%lf",curvasTrafos[i].stdPB[j]);
            }
            for(j=0;j<curvasTrafos[i].tamanho;j++){
                fprintf(saida,";%lf",curvasTrafos[i].stdPC[j]);
            }
            fprintf(saida,"\n");
        }
    }
    fclose(saida);   
}

//--------------------------------------------------------------------------


// FUNÇÕES PARA TRATAMENTO DAS ÁREAS DE MEDIÇÃO
// Baseada nos modelos Trifásicos




//------------------------------------------------------------------------------
// Busca AMs adjacentes e Elenca os nós da AM
void buscaAM(long int idNo, int idAM,  BOOL *visitado, GRAFO * grafo, AREA_MEDICAO *areasAM, int fase){
    //Depth-Search Algorithm - busca no e a sua profundidade (gera RNP))
    long int barraAdj,i = 0, idAM_jus;
    int nmed;
    
    visitado[idNo] = true;
    GRAFO * no = &grafo[idNo];
    for(i = 0; i < no->numeroAdjacentes; i++)
    {   
        barraAdj = no->adjacentes[i].idNo;
        //Se ramo ligado e ainda não visitado na sequencia
        if ((ramoLigado(no->adjacentes[i]) == true) && (visitado[barraAdj]== false)) {
            //------------------------------------------------------------------------
            // Novo medidor em ramo -> Atualiza a idAM
            if(ligadoPorMedidor(no->adjacentes[i])){ 
                if (idAM != -1){ // Tem um medidor a jusante
                    for (nmed = 0; nmed < no->adjacentes[i].nmed; nmed++){
                        if (((no->adjacentes[i].medidores[nmed]->tipo == 0) || (no->adjacentes[i].medidores[nmed]->tipo == 6)) && (no->adjacentes[i].medidores[nmed]->fases == fase)){ //Testa se é medidor de potência ou magnitude de corrente na fase especificada
                            idAM_jus = no->adjacentes[i].medidores[nmed]->idArea;
                            areasAM[idAM].numeroAdj++;
                            areasAM[idAM].med_adjacentes = (long int *) realloc (areasAM[idAM].med_adjacentes,areasAM[idAM].numeroAdj*sizeof(long int));
                            areasAM[idAM].med_adjacentes[areasAM[idAM].numeroAdj-1] = idAM_jus;   
                            break;
                        }
                    }
                    idAM = idAM_jus; //Atualiza a AM para a recursão
                    areasAM[idAM].descarte = 0;
                }
                else{ //Primeiro medidor encontrado
                    for (nmed = 0; nmed < no->adjacentes[i].nmed; nmed++){
                        if (((no->adjacentes[i].medidores[nmed]->tipo == 0) || (no->adjacentes[i].medidores[nmed]->tipo == 6)) && (no->adjacentes[i].medidores[nmed]->fases == fase)){ //Testa se é medidor de potência ou magnitude de corrente na fase especificada
                            idAM = no->adjacentes[i].medidores[nmed]->idArea; //Atualiza a AM para a recursão
                            areasAM[idAM].descarte = 0;
                            break;
                        }
                    }
                }
                
            }
            //------------------------------------------------------------------------
            // Sem medidor no ramo -> Inlcui nó na idAM
            else{ 
                if ((barraMedClasseA(grafo[barraAdj])==true) && (idAM != -1)){ //Medidor Classe A a jusante, inclui ele na lista de AMs adjacentes para descontar valor medido
                    for (nmed = 0; nmed < grafo[barraAdj].nmed; nmed++){
                        if ((grafo[barraAdj].medidores[nmed]->tipo == 2) && (grafo[barraAdj].medidores[nmed]->fases == fase)){ //Testa se é medidor de potência na fase especificada
                            idAM_jus = grafo[barraAdj].medidores[nmed]->idArea;
                            areasAM[idAM].numeroAdj++;
                            areasAM[idAM].med_adjacentes = (long int *) realloc (areasAM[idAM].med_adjacentes,areasAM[idAM].numeroAdj*sizeof(long int));
                            areasAM[idAM].med_adjacentes[areasAM[idAM].numeroAdj-1] = idAM_jus;   
                            break;
                        }
                    }
                }
                else if((grafo[barraAdj].tipoNo == 1) && (idAM != -1)){ // Barra com carga - incui na lista de barras 
                    areasAM[idAM].numeroNosAM++;
                    areasAM[idAM].nos = (long int*) realloc (areasAM[idAM].nos,areasAM[idAM].numeroNosAM*sizeof(long int));
                    areasAM[idAM].nos[areasAM[idAM].numeroNosAM-1] = barraAdj;
                }
            }
            
            
            //------------------------------------------------------------------------
            // recursão
            idNo= barraAdj;
            buscaAM(idNo, idAM,  visitado, grafo, areasAM, fase);
        } 
    }
}

//------------------------------------------------------------------------------
// Monta AMs
void buscaAMs(GRAFO * grafo, long int numeroBarras, ALIMENTADOR *alimentadores, long int numeroAlimentadores, DMED *medidas, long int **numeroMedidasTabela, AREA_MEDICAO **areasAM) {
    int i, j, idAlim, contador, par;
    long int numeroMedidas;
    BOOL visitado[(numeroBarras+1)];
           
    FILABARRAS *barraAtual;
    //Soma número de medidas na tabela de entrada
    numeroMedidas=0;
    for (i = 0; i < 14; i++){ 
         for (j = 0; j < 20; j++){
              numeroMedidas += numeroMedidasTabela[i][j];
         }
    }
    
    // Aloca Áreas de Medição
    if (((*areasAM) = (AREA_MEDICAO *)malloc( (numeroMedidas+1) * sizeof(AREA_MEDICAO)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para os dados de areas de medição!!!!");
        exit(1); 
    }
    
    // Aloca Áreas de Medição
    contador =0;
    for (i=0; i< numeroMedidas; i++){
        if ((medidas[i].tipo == 0) || (medidas[i].tipo == 6)){  //Medidas de Potência ou medida de Magnitude de Corrente{
            par = medidas[i].par;
            (*areasAM)[contador].medidor = &medidas[i];
            (*areasAM)[contador].fase = medidas[i].fases;
            (*areasAM)[contador].idAlim = medidas[i].idAlim;
            (*areasAM)[contador].numeroAdj = 0;
            (*areasAM)[contador].numeroNosAM = 0;
            (*areasAM)[contador].descarte = 4;
            (*areasAM)[contador].montante = -1;
            (*areasAM)[contador].nos = (long int *)malloc(1*sizeof(long int));
            (*areasAM)[contador].med_adjacentes = (long int *)malloc(1*sizeof(long int));            
            (*areasAM)[contador].med_local = 1;   // Para considerar ou nâo a medida no alimentador - exlcui classe A
            
            
            
            if (medidas[i].tipo == 0){ // Medida de potência ativa e reativa
                (*areasAM)[contador].tipo = 1;
                (*areasAM)[contador].Pmed = medidas[i].zmed;
                (*areasAM)[contador].Qmed = medidas[par].zmed;
                
                //Cálculo do desvio padrão
                (*areasAM)[contador].stdPmed = cabs(medidas[i].zmed)*medidas[i].prec/3 + 0.00001;
                (*areasAM)[contador].stdQmed = cabs(medidas[par].zmed)*medidas[par].prec/3 + 0.00001;
                
                (*areasAM)[contador].fp = cos(atan2((*areasAM)[contador].Qmed,(*areasAM)[contador].Pmed));
                
                medidas[i].idArea = contador;
                medidas[par].idArea = contador;
            }                        
            else if(medidas[i].tipo == 6){ // Medida de magnitude de corrente
                (*areasAM)[contador].tipo = 2;                
                (*areasAM)[contador].Imed = medidas[i].zmed;
                (*areasAM)[contador].fp = 0.92;
                
                //Cálculo do desvio padrão
                (*areasAM)[contador].stdImed = cabs(medidas[i].zmed)*medidas[i].prec/3 + 0.00001;
                
                medidas[i].idArea = contador;
            }
            
            
            if (medidas[i].m != -1){ // Identação do no adjecente que a medida está instalada
                for (j=0;j<grafo[medidas[i].k].numeroAdjacentes;j++){
                    if (grafo[medidas[i].k].adjacentes[j].idNo == medidas[i].m){
                        (*areasAM)[contador].idAdj = j;
                    }
                        
                }
            }
            contador++;
            
        }   
        else if (medidas[i].tipo == 2){  //Medidas de Potência Consumidor Classe A - Injeção de Ativa e Reativa
            par = medidas[i].par;
            (*areasAM)[contador].medidor = &medidas[i];
            (*areasAM)[contador].fase = medidas[i].fases;
            (*areasAM)[contador].idAlim = medidas[i].idAlim;
            (*areasAM)[contador].numeroAdj = 0;
            (*areasAM)[contador].numeroNosAM = 1;
            (*areasAM)[contador].descarte = 0;
            (*areasAM)[contador].montante = -1;
            (*areasAM)[contador].nos = (long int *)malloc(1*sizeof(long int));
            (*areasAM)[contador].med_adjacentes = NULL;            
            (*areasAM)[contador].med_local = 0;   // Para considerar ou nâo a medida no alimentador - exlcui classe A
            
            (*areasAM)[contador].tipo = 1;
            (*areasAM)[contador].Pmed = medidas[i].zmed;
            (*areasAM)[contador].Qmed = medidas[par].zmed;
            
            //Cálculo do desvio padrão
            (*areasAM)[contador].stdPmed = cabs(medidas[i].zmed)*medidas[i].prec/3 + 0.00001;
            (*areasAM)[contador].stdQmed = cabs(medidas[par].zmed)*medidas[par].prec/3 + 0.00001;

            (*areasAM)[contador].fp = cos(atan2((*areasAM)[contador].Qmed,(*areasAM)[contador].Pmed));

            medidas[i].idArea = contador;
            medidas[par].idArea = contador;            
            
            contador++;
            
        }
        // Montar um else para medida de magnitude de tensão
    }
    (*areasAM)[0].total = contador; //número total de AMs
    
    
    // Auxiliar para busca das AMs
    for(i=0; i<numeroBarras; i++){
        visitado[i] = false;
        if (grafo[i].barra->nloads > 0) grafo[i].tipoNo = 1;
    }
    
    //Percorre os Alimentadore através de varredura em profundidade para montar as AMs
    for(idAlim=0; idAlim<numeroAlimentadores; idAlim++){
//        buscaAM(grafo, (*areasAM), alimentadores[idAlim].idRaiz, idAlim, visitado);
        buscaAM(alimentadores[idAlim].idRaiz, -1,  visitado, grafo, (*areasAM), 1); //função recursiva por fase
        
        barraAtual = &alimentadores[idAlim].rnp[0];
        while(barraAtual != NULL)  {
            visitado[barraAtual->idNo] = false;
            barraAtual = barraAtual->prox;          
            
        }        
        buscaAM(alimentadores[idAlim].idRaiz, -1,  visitado, grafo, (*areasAM), 2); //função recursiva por fase
        
        barraAtual = &alimentadores[idAlim].rnp[0];
        while(barraAtual != NULL)  {
            visitado[barraAtual->idNo] = false;
            barraAtual = barraAtual->prox;          
            
        }     
        buscaAM(alimentadores[idAlim].idRaiz, -1,  visitado, grafo, (*areasAM), 3); //função recursiva por fase
        
    }
}



//------------------------------------------------------------------------------
// Percorre AMs em profundidade atualizando o fator de potência de acordo com a AM raiz
void atualizaFP_AMs(AREA_MEDICAO *areasAM) {
    
    long int i, id,idAM;    
    FILABARRAS * filaTrecho = NULL;
    double fp;
    
    for (idAM = 0; idAM < areasAM[0].total; idAM++) {
        if (areasAM[idAM].descarte == 0) {
            areasAM[idAM].fp = areasAM[idAM].Pcalc / sqrt(pow(areasAM[idAM].Pcalc, 2) + pow(areasAM[idAM].Qcalc, 2));
        }
    }
        
    
    for (idAM=0;idAM<areasAM[0].total;idAM++){
        if ((areasAM[idAM].montante == -1) && (areasAM[idAM].numeroAdj > 0)){
            adicionaNoNaFila(&filaTrecho, idAM);
            if (areasAM[idAM].medidor->tipo == 1)
                areasAM[idAM].fp = areasAM[idAM].Pmed/sqrt(pow(areasAM[idAM].Pmed,2)+pow(areasAM[idAM].Qmed,2));
            else
                areasAM[idAM].fp = areasAM[idAM].Pcalc/sqrt(pow(areasAM[idAM].Pcalc,2)+pow(areasAM[idAM].Qcalc,2));
            fp = areasAM[idAM].fp;
            
            while(filaNaoVazia(filaTrecho)){
                id = retiraNoDaFila(&filaTrecho);
                
                if (areasAM[idAM].descarte == 0){
                    for(i = 0; i<areasAM[id].numeroAdj; i++){
                        adicionaNoNaFila(&filaTrecho, areasAM[id].med_adjacentes[i]);
                        areasAM[areasAM[id].med_adjacentes[i]].fp = fp;            
                    }
                }
            }
        }
        else if ((areasAM[idAM].montante == -1) && (areasAM[idAM].numeroAdj == 0) && (areasAM[idAM].descarte == 0)){
            if (areasAM[idAM].medidor->tipo == 1)
                areasAM[idAM].fp = areasAM[idAM].Pmed/sqrt(pow(areasAM[idAM].Pmed,2)+pow(areasAM[idAM].Qmed,2));
            else
                areasAM[idAM].fp = areasAM[idAM].Pcalc/sqrt(pow(areasAM[idAM].Pcalc,2)+pow(areasAM[idAM].Qcalc,2));
        }
    }
       
}



//------------------------------------------------------------------------------
// Filtra as medidas através dos testes de consistência
void filtraAMs(AREA_MEDICAO *areasAM, ALIMENTADOR *alimentadores) {
    int i, j, idMont,aux;
    FILE *saida;
    
    saida = fopen("LOG_consistencia_medidas.txt","w+");
    
    //--------------------------------------------------------------------------
    //
    //Opção Desconsiderar medidas de corrente setada
    if (MED_CUR == 1){
        fprintf(saida,"Medidas de Corrente Desconsideradas\n");
        for (i=0;i<areasAM[0].total;i++)
        {
            if (areasAM[i].idAlim != -1){
                switch (areasAM[i].medidor->tipo){
                    case 6:
                        areasAM[i].descarte = 10;
                        break;
                }
            }
        }
    }
    
    //--------------------------------------------------------------------------
    //
    // Exclui medidas = 0 conectadas a alimenadores ligados (Assumido como falha de comunicação da medida) 
    fprintf(saida,"Medidas excluidas por possível falta de comunicacao. Critério: conectadas a rede mas com valor igual a zero\n");
    for (i=0;i<areasAM[0].total;i++)
    {
        if (areasAM[i].numeroAdj > 0){
            for (j=0;j<areasAM[i].numeroAdj;j++){
                areasAM[areasAM[i].med_adjacentes[j]].montante = i;
            }
        }
        
        
        if ((areasAM[i].idAlim != -1) && (areasAM[i].descarte == 0)){
            switch (areasAM[i].tipo){
                case 1:
                    if ((areasAM[i].Pmed == 0) || (areasAM[i].Qmed == 0)){
                        areasAM[i].descarte = 1;
                        fprintf(saida,"Medidor (%d) %ld - %ld (fase: %d)  tipo PQ:  Alimentador: %ld\n",i,areasAM[i].medidor->k,areasAM[i].medidor->m,areasAM[i].fase,alimentadores[areasAM[i].idAlim].idAlim);
                    }
                    break;
                case 2:
                    if (areasAM[i].Imed == 0){
                        areasAM[i].descarte = 1;
                        fprintf(saida,"Medidor (%d) %ld - %ld (fase: %d)  tipo I:  Alimentador: %ld\n",i,areasAM[i].medidor->k,areasAM[i].medidor->m,areasAM[i].fase,alimentadores[areasAM[i].idAlim].idAlim);
                    }
                    break;
            }
        }
    }
        
    //--------------------------------------------------------------------------
    //
    // Exclui medidas com valor acima de duas vezes o esperado pelas curvas agregadas
    fprintf(saida,"\nMedidas excluidas pelo valor medido ser maior que duas vezes o valor esperado da medida. Criterio: Valor esperado dado pelas curvas agregadas e fluxo de potencia \n");
    for (i=0;i<areasAM[0].total;i++)
    {
        if ((areasAM[i].idAlim != -1) && (areasAM[i].descarte == 0)){
            switch (areasAM[i].medidor->tipo){
                case 1:
                    if ((areasAM[i].Pmed >= 2.5*areasAM[i].Pcalc) || (areasAM[i].Qmed >= 2.5*areasAM[i].Qcalc)){
                        areasAM[i].descarte = 2;
                        fprintf(saida,"Medidor (%d) %ld - %ld (fase: %d)  tipo PQ:  Alimentador: %ld    Pmed = %lf  Qmed = %lf   Pcalc = %lf  Qcalc = %lf\n",i,areasAM[i].medidor->k,areasAM[i].medidor->m,areasAM[i].fase,alimentadores[areasAM[i].idAlim].idAlim,areasAM[i].Pmed,areasAM[i].Qmed,areasAM[i].Pcalc,areasAM[i].Qcalc);
                    }
                    break;
                case 2:
                    if (areasAM[i].Imed >= 2.5*abs(areasAM[i].Icalc)){
                        areasAM[i].descarte = 2;
                        fprintf(saida,"Medidor (%d) %ld - %ld (fase: %d)  tipo I:  Alimentador: %ld   Imed = %lf   Icalc = %lf\n",i,areasAM[i].medidor->k,areasAM[i].medidor->m,areasAM[i].fase,alimentadores[areasAM[i].idAlim].idAlim,areasAM[i].Imed,cabs(areasAM[i].Icalc));
                    }
                    break;
            }
        }
    }
    
    //----------------------------------------------------------------------
    //
    // Exclui medidas inconsistentes quanto a comparação com a área a jusante
    fprintf(saida,"\nMedidas excluidas por inconsistência entre medida a jusante e montante. Critério: Valor mais distante do valor esperado é descartado\n");
    for (i=0;i<areasAM[0].total;i++)
    {
        if ((areasAM[i].idAlim != -1) && (areasAM[i].descarte == 0)){
            idMont = areasAM[i].montante;
            if (idMont != -1){
                switch (areasAM[i].medidor->tipo){
                    case 1:
                        if ((areasAM[idMont].Pmed < areasAM[i].Pmed) || (areasAM[idMont].Qmed < areasAM[i].Qmed)){
                            if (abs(areasAM[idMont].Pmed - areasAM[idMont].Pcalc)/areasAM[idMont].Pmed > abs(areasAM[i].Pmed - areasAM[i].Pcalc)/areasAM[i].Pmed){
                                if (areasAM[idMont].descarte == 0){
                                    areasAM[idMont].descarte = 3;
                                    fprintf(saida,"Descartado Montante - Medidor %d //// Jusante - Medidor %d tipo PQ  Alimentador: %ld\n",idMont,i,alimentadores[areasAM[i].idAlim].idAlim);
                                }
                            }
                            else{
                                areasAM[i].descarte = 3;
                                fprintf(saida,"Montante - Medidor %d ///// Descartado Jusante - Medidor %d tipo PQ  Alimentador: %ld\n",idMont,i,alimentadores[areasAM[i].idAlim].idAlim);
                            }
                        }
                        
                        
                        break;
                    case 2:
                        if (areasAM[idMont].Imed < areasAM[i].Imed){
                            if (abs(areasAM[idMont].Imed - areasAM[idMont].Icalc)/areasAM[idMont].Imed > abs(areasAM[i].Imed - areasAM[i].Icalc)/areasAM[i].Imed){
                                if (areasAM[idMont].descarte == 0){
                                    areasAM[idMont].descarte = 3;
                                    fprintf(saida,"Descartado Montante - Medidor %d //// Junsante - Medidor %d tipo I   Alimentador: %ld\n",idMont,i,alimentadores[areasAM[i].idAlim].idAlim);
                                }
                            }
                            else{
                                areasAM[i].descarte = 3;
                                fprintf(saida,"Montante - Medidor %d //// Descartado Junsante - Medidor %d tipo I   Alimentador: %ld\n",idMont,i,alimentadores[areasAM[i].idAlim].idAlim);
                            }
                        }
                        break;
                }
                
                
            }
        }
    }
    
    //--------------------------------------------------------------------------
    //
    //Varre AMs para tranferir as barras de AMs descartadas a jusante para as respectivas AMs a montante
    //Varre AMs para remover AMs descartadas a jusante para as respectivas AMs a montante
    for (i=0;i<areasAM[0].total;i++)
    {
        if (areasAM[i].descarte >= 1){
            idMont = areasAM[i].montante;
            if (idMont != -1){
                //------------------------------------------------------------------
                //Trasnferi as barras da AM descartada
                areasAM[idMont].nos = (long int*) realloc (areasAM[idMont].nos,(areasAM[idMont].numeroNosAM + areasAM[i].numeroNosAM + 1)*sizeof(long int));

                for (j=0;j<areasAM[i].numeroNosAM;j++){
                    areasAM[idMont].nos[areasAM[idMont].numeroNosAM+j] = areasAM[i].nos[j];
                    areasAM[i].nos[j] = -1;
                }
                areasAM[idMont].numeroNosAM = areasAM[idMont].numeroNosAM + areasAM[i].numeroNosAM;
                
                //--------------------------------------------------------------
                //Trasnferi as AMs jusante da AM descartada
                if (areasAM[i].numeroAdj > 0){
                    areasAM[idMont].med_adjacentes = (long int *) realloc (areasAM[idMont].med_adjacentes,(areasAM[idMont].numeroAdj + areasAM[i].numeroAdj + 1)*sizeof(long int));
                    
                    for (j=0;j<areasAM[i].numeroAdj;j++){
                        areasAM[idMont].med_adjacentes[areasAM[idMont].numeroAdj+j] = areasAM[i].med_adjacentes[j];
                        areasAM[i].med_adjacentes[j] = -1;
                    }
                }
                aux = -1;
                for (j=0;j<(areasAM[idMont].numeroAdj + areasAM[i].numeroAdj);j++){
                    if (areasAM[idMont].med_adjacentes[j] == i){
                        aux = j;
                    }
                    if (aux != -1){
                        areasAM[idMont].med_adjacentes[j] = areasAM[idMont].med_adjacentes[j+1];
                    }
                }
                areasAM[idMont].numeroAdj = areasAM[idMont].numeroAdj - 1 + areasAM[i].numeroAdj;
            
            }
            
        }
    }
    
    //--------------------------------------------------------------------------
    //
    //Se depois da filtragem existem AMs sem barras, indica estes alimentadores e descarta AM
    fprintf(saida,"\nMedidas excluidas por inconsistência de topologia. Critério: a rede a jusante da AM não possui barras do tipo 1 (Barras com Carga)\n");
    for (i=0;i<areasAM[0].total;i++)
    {
        if ((areasAM[i].numeroNosAM == 0) && (areasAM[i].descarte == 0)){
            areasAM[i].descarte = 4;            
        }
    }
        
    atualizaFP_AMs(areasAM);
    
    fclose(saida);
    
}


//------------------------------------------------------------------------------
// Imprime valores das AMs para conferência
void imprimeAMs(AREA_MEDICAO *areasAM){
    int i, j;
    FILE *saida;
    saida = fopen("AMS_out.dad","w");
    
    fprintf(saida,"\n\nAreas de medicao = %ld : \n",areasAM[0].total);
    for (i=0;i<areasAM[0].total;i++){
        if (areasAM[i].descarte == 0){
            fprintf(saida,"\n--------------------------------------------\n");
            fprintf(saida,"\nID:%d Alim: %ld Medida: %ld - %ld Proprietaria: %ld Nos: %ld Adjacentes: %ld;\n",i,areasAM[i].idAlim,areasAM[i].medidor->k,areasAM[i].medidor->m,areasAM[i].medidor->k,areasAM[i].numeroNosAM,areasAM[i].numeroAdj);
            fprintf(saida,"Tipo: %ld - Descarte: %ld convP = %ld  - convQ = %ld   - FP: %lf\n",areasAM[i].tipo,areasAM[i].descarte,areasAM[i].convP,areasAM[i].convQ,areasAM[i].fp);
            fprintf(saida,"Med P = %lf kW - Med Q =  %lf kVAr - Med I = %lf  A - \n",areasAM[i].Pmed/1000,areasAM[i].Qmed/1000,areasAM[i].Imed);
            fprintf(saida,"Calc P = %lf kW - Calc Q =  %lf kVAr - Calc I =  %lf A\n",areasAM[i].Pcalc/1000,areasAM[i].Qcalc/1000,sqrt(pow(__real__ areasAM[i].Icalc,2)+pow(__imag__ areasAM[i].Icalc,2)));
            fprintf(saida,"AMs Jusante: ");
            for(j=0;j<areasAM[i].numeroAdj;j++)
                fprintf(saida,"%ld;",areasAM[i].med_adjacentes[j]);
            fprintf(saida,"\tAM Montante: %ld",areasAM[i].montante);
            fprintf(saida,"\nNos pertencentes a AM: ");
            for(j=0;j<areasAM[i].numeroNosAM;j++)
                fprintf(saida,"%ld;",areasAM[i].nos[j]);
        }
        else{
            fprintf(saida,"\n--------------------------------------------\n");
            fprintf(saida,"\nID:%d Alim: %ld Medida: %ld - %ld \n",i,areasAM[i].idAlim,areasAM[i].medidor->k,areasAM[i].medidor->m);
            fprintf(saida,"Tipo: %ld - Descarte: %ld \n",areasAM[i].tipo,areasAM[i].descarte);
        }
    }
    fclose(saida); 
}

//------------------------------------------------------------------------------
// Atualiza os Valores Calculados nas Medidas e Verifica a Convergência
long int atualizaAM(AREA_MEDICAO *areaAM, GRAFO *grafo, long int numeroNos,BOOL *conv_alim, long int it)
{
    int i, j, de, para, min, fase,idAdj;
    
    __complex__ double auxI;
    //************************************************************
    
    for (i=0;i<areaAM[0].total;i++)
    {
        //------------------------------------
        //Atualiza AMs com os dados calculados
        if ((areaAM[i].med_local == 1) && (areaAM[i].descarte == 0)){  
            de  = areaAM[i].medidor->k;
            para = areaAM[i].medidor->m;
            idAdj = areaAM[i].idAdj;
            fase = areaAM[i].fase - 1;
            areaAM[i].Pcalc = creal(grafo[de].V[fase] * conj(grafo[de].adjacentes[idAdj].Cur[fase]));
            areaAM[i].Qcalc = cimag(grafo[de].V[fase] * conj(grafo[de].adjacentes[idAdj].Cur[fase]));
            areaAM[i].Icalc = grafo[de].adjacentes[idAdj].Cur[fase];
            areaAM[i].Vcalc = grafo[de].V[fase];
            
            // TRATAMENTO PARA MEDIDAS INCOMPLETAS
            switch (areaAM[i].tipo){
                case 2: //Area de medição definida por Medida de corrente
                    auxI = conj(grafo[de].adjacentes[idAdj].Cur[fase]);
                    if (it ==0){
                        __real__ auxI = cos(carg(grafo[de].V[fase]) - acos(areaAM[i].fp));
                        __imag__ auxI = sin(carg(grafo[de].V[fase]) - acos(areaAM[i].fp));
                        

                        auxI = conj(auxI);
                    }
                    
                    if (sqrt(pow(__real__ auxI,2)+pow(__imag__ auxI,2))==0){
                        areaAM[i].Pmed = 0;
                        areaAM[i].Qmed = 0;
                    }
                    else{
                        areaAM[i].Pmed = __real__ (grafo[de].V[fase] * areaAM[i].Imed * auxI/sqrt(pow(__real__ auxI,2)+pow(__imag__ auxI,2)));
                        areaAM[i].Qmed = __imag__ (grafo[de].V[fase] * areaAM[i].Imed * auxI/sqrt(pow(__real__ auxI,2)+pow(__imag__ auxI,2)));
                    }
                    break;
            }
        }
        else {//Consumidores Classe A
            if (areaAM[i].descarte == 0) {
                de = areaAM[i].medidor->k;
                fase = areaAM[i].fase - 1;
                areaAM[i].Pcalc = __real__ grafo[de].S[fase];
                areaAM[i].Qcalc = __imag__ grafo[de].S[fase];

            }
        }
    }
    
    // Cálculo dos mismatches de potência das áreas de medição
    for (i=0;i<areaAM[0].total;i++)
    {
        if (areaAM[i].descarte == 0) {
            //-----------------------
            //Calcula DeltaP e DeltaQ
            areaAM[i].DeltaP = areaAM[i].Pmed - areaAM[i].Pcalc;
            areaAM[i].DeltaQ = areaAM[i].Qmed - areaAM[i].Qcalc;

            if (areaAM[i].numeroAdj > 0){
                for (j=0;j<areaAM[i].numeroAdj;j++){
                    DADOS_MEDIDOR *medAdj = &areaAM[areaAM[i].med_adjacentes[j]].medidor;
                    areaAM[i].DeltaP = areaAM[i].DeltaP - (medAdj->Pmed - medAdj->Pcalc);
                    areaAM[i].DeltaQ = areaAM[i].DeltaQ - (medAdj->Qmed - medAdj->Qcalc);
                }
            }

            //-------------------------------------
            //CRITÉRIO DE CONVERGÊNCIA PARA CADA AM
        
            switch (areaAM[i].tipo) {
                case 1:
                    if (fabs(areaAM[i].Pmed - areaAM[i].Pcalc) < PREC_LMF * areaAM[i].stdPmed) {
                        areaAM[i].convP = 1;
                    } else {
                        areaAM[i].convP = 0;
                    }

                    if (fabs(areaAM[i].Qmed - areaAM[i].Qcalc) < PREC_LMF * areaAM[i].stdQmed) {
                        areaAM[i].convQ = 1;
                    } else {
                        areaAM[i].convQ = 0;
                    }
                    break;
                case 2:
                    if (fabs((areaAM[i].Imed - sqrt(pow(__real__ areaAM[i].Icalc, 2) + pow(__imag__ areaAM[i].Icalc, 2)))) < PREC_LMF * areaAM[i].stdImed) {
                        areaAM[i].convP = 1;
                        areaAM[i].convQ = 1;
                    } else {
                        areaAM[i].convP = 0;
                        areaAM[i].convQ = 0;
                    }
                    break;
            }

            //VETOR dos alimentadores convergidos! - Recebe vetor booleano e indica quais os alimentadores que precisam de novo cálculo do FC
            if ((areaAM[i].idAlim == -1) || (areaAM[i].numeroNosAM == 0)) {
                areaAM[i].convP = 1;
                areaAM[i].convQ = 1;
            }
        }
        if (areaAM[i].descarte >= 1){
            areaAM[i].convP = 1;               
            areaAM[i].convQ = 1;
        }
        
    }
    
    min=1;
    for (i=0;i<areaAM[0].total;i++)
    {
        if ((areaAM[i].convP < min) || (areaAM[i].convQ < min)) min = 0;
    }
    if (min == 1) return (1);
    else return(0);
}

//------------------------------------------------------------------------------
// Ajusta as cargas dentro de uma AM
void ajustaAM(AREA_MEDICAO *areaAM, GRAFO *grafo, long int numeroNos){
    int i, j, noN, fase, de;
        
    // Percorre as barras na estrutura CONFIGURACAO e aplica os fatores de ajuste
    for (i=0;i<areaAM[0].total;i++)
    {
        fase = areaAM[i].fase - 1;
        de = areaAM[i].medidor->k;
        if (areaAM[i].descarte == 0){
            if ((areaAM[i].convP == 0) || (areaAM[i].convQ == 0)){
                areaAM[i].SumPnos = 0;
                areaAM[i].SumQnos = 0;
                for (j=0;j<areaAM[i].numeroNosAM;j++){
                    noN = areaAM[i].nos[j];
                    areaAM[i].SumPnos += __real__ grafo[noN].S[fase];
                    areaAM[i].SumQnos += __imag__ grafo[noN].S[fase];
                }

                for (j=0;j<areaAM[i].numeroNosAM;j++){
                    noN = areaAM[i].nos[j];
                    //Atualiza a potência ativa de todas as barras da AM
                    if (areaAM[i].convP == 0){
                        grafo[noN].S[fase] += __real__ (areaAM[i].DeltaP*grafo[noN].S[fase]/areaAM[i].SumPnos);
                    }
                    //Atualiza a potência reativa de todas as barras da AM
                    if (areaAM[i].convQ == 0){
                        grafo[noN].S[fase] += I * __imag__ (areaAM[i].DeltaQ*grafo[noN].S[fase]/areaAM[i].SumQnos);
                    }
                    
                    grafo[noN].Cur[fase] = conj(grafo[noN].S[fase]/grafo[noN].V_aux[fase]);
                }
            }
        }
                
    }
}



//Atualiza as Injeções de Corrente nos grafos
void atualizaCorrenteNoGrafo(GRAFO *no){
    int i, j;
    __complex__ double Saux[3], Iaux[3], IauxDelta[3], Vl[3], V0;
    
    no->Cur[0] = conj(no->S[0] / no->V[0]);
    no->Cur[1] = conj(no->S[1] / no->V[1]);
    no->Cur[2] = conj(no->S[2] / no->V[2]);
    
    
}


//Rotina para Cálculo de Fluxo de Potência em múltiplos alimentadores de um sistema de distribuição na sequeência hierárquica dos níveis de tensão (Primeiro 13.8kV e depois 34.5kV)
/**
 * @brief Função principal para cálculo de fluxo de potência (Primeiro 13.8kV e depois 34.5kV).
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
void estimadorDemandaTrifasico(GRAFO *grafo, long int numeroBarras, ALIMENTADOR *alimentadores, long int numeroAlimentadores, DRAM *ramos,double Sbase, long int **interfaceNiveis,long int numeroInterfaces, AREA_MEDICAO *areasAM){
    long int nmed,nvar,nmedTotal;
    int i,j, idAlim, it, n_threads;
    FILABARRAS *barraAtual = NULL;
    GRAFO *no  = NULL;
    int conv_ECTR = 0, itECTR = 0;
    
    BOOL convECTR_alim[numeroAlimentadores];
    // Auxiliar para busca das AMs
    for(i=0; i<numeroAlimentadores; i++) convECTR_alim[i] = false;    
    
    FILE *arqout;
    
    arqout = fopen("LOG_EDTR.txt","a+");
    clock_t start = clock();
    
    //---------------------------------------------------------------------
    // Inicializa Tensões e Correntes das Cargas em cada barra
    incializaTensoesVarredura(grafo, numeroBarras, alimentadores, numeroAlimentadores);
    
    //---------------------------------------------------------------------
    // Conversão das Demandas nos Transformadores de Distribuição Delta-Estrela
    
        
    
    while (conv_ECTR==0){
        //---------------------------------------------------------------------
        // Cálculo de Fluxo de Potência
                
        fluxoPotencia_Niveis_BFS_Multiplos(grafo, numeroBarras, alimentadores, numeroAlimentadores, ramos, Sbase/1000, interfaceNiveis, numeroInterfaces, true);
        //fluxoPotencia_BFS_Multiplos(grafo, numeroBarras, alimentador, numeroAlimentadores, ramo, Sbase/1000);
        
        
        //--------------------------------------------------------------------------
        // Função de Atualização das Medidas em Tempo Real
        conv_ECTR = atualizaAM(areasAM, grafo, numeroBarras,convECTR_alim,itECTR);
        
        if (itECTR == 0){
            filtraAMs(areasAM, grafo); //Testa consistência das medidas analógicas
            atualizaAM(areasAM,grafo,numeroBarras,convECTR_alim,itECTR);
        }
        //conv_ECTR = 1; //Rodar sem ajuste de cargas
        
        if (conv_ECTR == 1){
              printf("\nEDTR - Trifásico Convergido em %d iteracoes\n",itECTR);
        }
        else if (itECTR == 15){
              printf("\nEDTR - Trifásico divergiu para os alimentadores:\n");
              conv_ECTR = 2;
        }
        else{
            // Função de Ajuste das Áreas de Medição
            ajustaAM(areasAM,grafo,numeroBarras);
            
            // Atualizar as correntes em função do valor ajustade de potência  - usar o grafo[no].S
            itECTR++;
        }
        
    } //Fim do loop do ECTR
    clock_t end = clock();
    double edtr_time = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(arqout,"\nConvergência: %d - Iteracoes: %d - Tempo: %lf", conv_ECTR, itECTR, edtr_time);
    fclose(arqout);
    
    
    
//    //Impressão de resultados em arquivos
//    int ppt_aux = 0;
//    for (idAlim = 0; idAlim < numeroAlimentadores; idAlim++){
////        if (grafo[alimentadores[idAlim].noRaiz].Vbase*sqrt(3)/1000 == 34.5){
//        if ((idAlim == 18) || (idAlim ==94)){
////        if ((idAlim ==94)){
//            if (ppt_aux == 0){
//                salvaTensoesNodais("stateVT.txt","w+",alimentadores[idAlim],grafo);
//                salvaCorrentesRamos("loadingI.txt", "w+", alimentadores[idAlim],grafo, numeroBarras, Sbase);
//                ppt_aux=1;
//            }
//            else{
//                salvaTensoesNodais("stateVT.txt","a+",alimentadores[idAlim],grafo);
//                salvaCorrentesRamos("loadingI.txt", "a+", alimentadores[idAlim],grafo, numeroBarras, Sbase);
//            }
////            printf("\nTaps de Reguladores:\n");
////            barraAtual = &alimentadores[idAlim].rnp[0];
////            while(barraAtual != NULL){
////                imprimeTaps(&grafo[barraAtual->idNo]);
////                barraAtual = barraAtual->prox;
////            }
//        }
//    }
    
}