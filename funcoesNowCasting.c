
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <ctype.h>

#include "data_structures_tf.h"
#include "funcoesLeitura_tf.h"
#include "funcoesModCarga_tf.h"
#include "data_structures_modcarga_tf.h"

#include "data_structures_tf.h"
#include "data_structures_modcarga_tf.h"
#include "funcoesTopologia_tf.h"
#include "funcoesLeitura_tf.h"
#include "funcoesFluxoVarredura_tf.h"
#include "funcoesIntegracao_tf.h"




// fazer a documentação

/**
 * @brief Funcao que constroi o DMED de previsão a partir das medidas previstas pelo NowCasting
 * Essa recebe como parâmetro a estrutura @p prev_tf, um vetor do tipo TF_DPREV com os dados de previsão obtidos com o NowCasting,
 * um inteiro com o número de medidas @p nmed, @p grafo um vetor do tipo TF_GRAFO, @p Sbase do tipo double com a potência base da rede
 * para criar a estrutura @p medidas do tipo TF_DMED, para receber os dados de previsão a cada iteracao do estimador de demanda com 
 * Now casting. Esta funcao aloca memória para a estrutura @p medidas
 * 
 * @param prev_tf vetor do tipo TF_DPREV com as medidas previstas pelo NowCasting
 * @param nmed inteiro com o número de medidas da rede
 * @param medidas estrutura do tipo TF_DMED com as medidas previstas
 * @param grafo vetor do tipo TF_GRAFO com as informações elétricas da rede trifasica
 * @param Sbase double com a potencia base da rede
 */
void constroi_dmed_prev(TF_DPREV *prev_tf, int nmed,TF_DMED **medidas,TF_GRAFO *grafo,double Sbase)
{
    // aloca espaço para as medidas 

    int i,j;

    if ((((*medidas) = (TF_DMED *)malloc( nmed*sizeof(TF_DMED)))==NULL))
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para as medidas !!!!");
        exit(1); 
    }
    

    for (size_t i = 0; i < nmed; i++)
    {
        (*medidas)[i]=prev_tf[i].DMED;
    }
    

    //associa medidores no grafo
    
    for (i=0;i<nmed;i++)
    {
        AssoMedGraf(grafo,&((*medidas)[i]),Sbase);
    }

    AssoMedidoresPares((*medidas),nmed);

    
}


/**
 * @brief Função que atualiza o vetor @p medidas do tipo TF_DMED com as medidas do instante atual, estrutura @p prev_tf do tipo
 * TF_DPREV, recebe os parâmetros auxiliares @p instante_atual um inteiro com o instante atual, @p nmed um int com o numero de medidas
 * 
 * @param instante_atual inteiro com o instante atual do estimador de demanda 
 * @param prev_tf vetor do tipo TF_DPREV com as medidas previstas pelo NowCasting 
 * @param nmed inteiro com o número de medidas da rede
 * @param medidas estrutura do tipo TF_DMED com as medidas previstas
 */
void atualiza_dmed(int instante_atual,TF_DPREV *prev_tf, int nmed, TF_DMED *medidas)
{
    for (size_t i = 0; i < nmed; i++)
    {
        medidas[i].zmed = prev_tf[i].prev[instante_atual]/prev_tf[i].base;
        medidas[i].sigma = prev_tf[i].prec[instante_atual]/prev_tf[i].base;
        medidas[i].prec= prev_tf[i].prec[instante_atual]; 
    }
    

}
/**
 * @brief Funcao que atualiza os valores medidos atuais na area de medição
 * 
 * Recebe como parâmetros @p grafo um vetor do tipo TF_GRAFO com as informações elétricas sobre a rede trifásica, @p numeroBarras inteiro com o numero de barras
 * @p medidas vetor do tipo TF_DMED com as medidas atualizadas da rede, @p  numeroMedidas inteiro com o número de medidas
 * @p areasAM vetor do tipo  TF_AREA_MEDICAO com as informacoes sobre as áreas de medicao
 * 
 * @param grafo vetor do tipo TF_GRAFO com as informações elétricas da rede trifasica
 * @param numeroBarras inteiro com o número de barras na rede
 * @param medidas estrutura do tipo TF_DMED com as medidas previstas
 * @param numeroMedidas inteiro com o numero de medidas sobre a rede
 * @param areasAM estrutura do tipo TF_AREA_MEDICAO com as informcaoes sobre as areas de medicao
 */
void atualiza_AM(TF_GRAFO * grafo, long int numeroBarras, TF_DMED *medidas, long int numeroMedidas, TF_AREA_MEDICAO *areasAM) {
    int i, j, idAlim, contador, par;
           
    TF_FILABARRAS *barraAtual;
    //Soma número de medidas na tabela de entrada


    // Aloca Áreas de Medição
    for (i=0; i< numeroMedidas; i++){
        
        if ((medidas[i].tipo == 0) ||(medidas[i].tipo == 2)){ // Medida de potência ativa e reativa
            contador=medidas[i].idArea;
            par=medidas[i].par;
                areasAM[contador].Pmed = medidas[i].zmed;
                areasAM[contador].Qmed = medidas[par].zmed;
                
                //Cálculo do desvio padrão
                areasAM[contador].stdPmed = cabs(medidas[i].zmed)*medidas[i].prec/3 + 0.00001;
                areasAM[contador].stdQmed = cabs(medidas[par].zmed)*medidas[par].prec/3 + 0.00001;
                
                areasAM[contador].fp = cos(atan2(areasAM[contador].Qmed,areasAM[contador].Pmed));
            }                        
            else if(medidas[i].tipo == 6){ // Medida de magnitude de corrente
                contador=medidas[i].idArea;
                areasAM[contador].tipo = 2;                
                areasAM[contador].Imed = medidas[i].zmed;
                areasAM[contador].fp = 0.92;
                
                //Cálculo do desvio padrão
                areasAM[contador].stdImed = cabs(medidas[i].zmed)*medidas[i].prec/3 + 0.00001;
                
            }
  
        // Montar um else para medida de magnitude de tensão
    }
    
}


/**
 * @brief Funcao que realiza o Now Casting de demanda
 * 
 * Esta funcao recebe como parâmetros @p grafo_tf um vetor do tipo TF_GRAFO com as informacoes sobre a rede elétrica @p barras_tf do tipo 
 * TF_DBAR com as informacoes sobre as barras da rede, @p numeroBarras_tf um inteiro com o numero de barras da rede, @p alimentador_tf , uma
 * estrutura do tipo TF_ALIMENTADOR com as informacoes sobre os alimentadores da rede elétrica, @p curvaTrafos uma estrutura do tipo TF_CURVA_TRAFO
 * com as informacoes sobre as curvas de cargas agregadas da rede, @p numeroAlimentadores inteiro com o numero total de alimentadores da rede
 * @p ramo_tf vetor do tipo TF_DRAM com as informacoes dos ramos da rede, @p Sbase double com a potência base da rede, @p interfaceNiveis_tf matriz 
 * de inteiros com as informacoes sobre as interfaces da rede, @p numeroInterfaces_tf inteiro com o numero de interfaces da rede, @p areasMedicao_tf
 * estrutura do tipo TF_AREA_MEDICAO com as informacoes sobre as áreas de medicao, @p prev_tf vetor do tipo TF_DPREV com os dados de previsao 
 * @p medidaPrev_tf do tipo  TF_DMED com as medidas previstas, @p numeroMedidasTabela matriz de inteiros com os tipos de medida, @p numeroAmostras
 * inteiro com o numero de previsoes de medida
 * 
 * Returna os dados compilados em uma estrutura do tipo TF_NCRESULT
 * 
 * @param grafo_tf vetor do tipo TF_GRAFO com as informações elétricas da rede trifasica
 * @param barras_tf estrutura do tipo TF_DBAR com as informacoes sobre as barras da rede
 * @param numeroBarras_tf inteiro com o numero de barras da rede 
 * @param alimentador_tf vetor de estrutura do tipo TF_ALIMENTADOR com as informacoes sobre os alimentadores 
 * @param curvasTrafos vetor de estrutura do tipo TF_CURVA_TRAFO com as informacoes sobre as curvas de cargas agregadas da rede
 * @param numeroAlimentadores inteiro com o numero de alimentadores da rede
 * @param ramo_tf vetor do tipo TF_DRAM com as informacoes sobre os ramos da rede
 * @param Sbase double com a potência base da rede
 * @param interfaceNiveis_tf matriz de inteiros com as interfaces da rede
 * @param numeroInterfaces_tf inteiro com o numero de interfaces
 * @param areasMedicao_tf vetor de estrutura do tipo TF_AREA_MEDICAO com as informacoes sobre as areas de medicao da rede
 * @param prev_tf vetor de estrutura do tipo TF_DPREV com os dados de previsao de medidas
 * @param medidaPrev_tf estrutura do tipo TF_DMED utilizada pra comportar as medidas previstas
 * @param numeroMedidasTabela matriz de inteiros com o numero de medidas e os tipos
 * @param numeroAmostras inteiro com o numero de amostras
 * @return TF_NCRESULT* retorna os dados compilados do nowcasting de demanda
 */
TF_NCRESULT* NowCastingDemanda(TF_GRAFO *grafo_tf, TF_DBAR *barras_tf ,long int numeroBarras_tf, TF_ALIMENTADOR *alimentador_tf, TF_CURVA_TRAFO *curvasTrafos, 
 long int numeroAlimentadores, TF_DRAM *ramo_tf,double Sbase, long int **interfaceNiveis_tf,long int numeroInterfaces_tf, TF_AREA_MEDICAO *areasMedicao_tf, TF_DPREV *prev_tf, 
 TF_DMED *medidaPrev_tf ,long int **numeroMedidasTabela, int numeroAmostras)
{

    FILE *arquivo;
    int nmed=0;
    char ts[15];
    TF_NCRESULT* result;
    (result) = (TF_NCRESULT *) malloc((numeroAmostras+1)*sizeof(TF_NCRESULT));
    
    for (size_t i = 0; i < numeroAmostras; i++)
    {
        result[i].dadosBarras= (TF_NCBAR *) malloc((numeroBarras_tf+1)*sizeof(TF_NCBAR));
    }
    

    nmed=0;
    for (size_t i = 0; i < 14; i++){ 
         for (size_t j = 0; j < 20; j++){
              nmed += numeroMedidasTabela[i][j];
         }
    }
    
    arquivo=fopen("dados_compilados.csv","w");
    fprintf(arquivo,"instante,iteracoes,id_aliment_itxmax,numeroAlimentadores,menor_V,id_menorV,quedaMaxima,id_maxQueda,maiorCarregamentoCorrente,id_maxCar,carregamentoRede,maiorCarregaphA,maiorCarregaphB,maiorCarregaphC,PerdasRes,desba,id_maxDesbalanco,desbalancoCorrenteAlim,id_maxDesbalancoCorrenteAlim\n");
    fclose(arquivo);
    
    for (size_t instante_atual = 0; instante_atual < numeroAmostras; instante_atual++)
    {
        arquivo=fopen("dados_compilados.csv","a");
        fprintf(arquivo,"%d",instante_atual+1);
        fclose(arquivo);

        // receber a curva trafos
        // fazer rodar toda vez que mudar a hora do instante atual e colocar na inicializa
        inicializaPQcurvas(barras_tf, curvasTrafos, numeroBarras_tf,  prev_tf[0].hour[instante_atual], VALOR_ESPERADO, Sbase); // conferir com o Julio se é isso mesmo

        atualiza_dmed(instante_atual,prev_tf,nmed,medidaPrev_tf); 
        atualiza_AM(grafo_tf,numeroBarras_tf,medidaPrev_tf,nmed,areasMedicao_tf);
        //Estimador de Demandas Trifásicas
        estimadorDemandaTrifasico(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores, ramo_tf, Sbase, interfaceNiveis_tf, numeroInterfaces_tf, areasMedicao_tf);
        preenche_result_NC(grafo_tf,numeroBarras_tf,instante_atual,result);

        sprintf(ts,"%d",prev_tf[0].time_stamp[instante_atual]);// pega o time stamp atual
        imprimeDBAR_cargas(grafo_tf,numeroBarras_tf,ts);
    }
    



    return(result);
}


/**
 * @brief Funcao que realiza o Now Casting de demanda
 * 
 * Esta funcao recebe como parâmetros @p grafo_tf um vetor do tipo TF_GRAFO com as informacoes sobre a rede elétrica @p barras_tf do tipo 
 * TF_DBAR com as informacoes sobre as barras da rede, @p numeroBarras_tf um inteiro com o numero de barras da rede, @p alimentador_tf , uma
 * estrutura do tipo TF_ALIMENTADOR com as informacoes sobre os alimentadores da rede elétrica, @p curvaTrafos uma estrutura do tipo TF_CURVA_TRAFO
 * com as informacoes sobre as curvas de cargas agregadas da rede, @p numeroAlimentadores inteiro com o numero total de alimentadores da rede
 * @p ramo_tf vetor do tipo TF_DRAM com as informacoes dos ramos da rede, @p Sbase double com a potência base da rede, @p interfaceNiveis_tf matriz 
 * de inteiros com as informacoes sobre as interfaces da rede, @p numeroInterfaces_tf inteiro com o numero de interfaces da rede, @p areasMedicao_tf
 * estrutura do tipo TF_AREA_MEDICAO com as informacoes sobre as áreas de medicao, @p prev_tf vetor do tipo TF_DPREV com os dados de previsao 
 * @p medidaPrev_tf do tipo  TF_DMED com as medidas previstas, @p numeroMedidasTabela matriz de inteiros com os tipos de medida, @p numeroAmostras
 * inteiro com o numero de previsoes de medida
 * 
 * Returna os dados compilados em uma estrutura do tipo TF_NCRESULT
 * 
 * @param grafo_tf vetor do tipo TF_GRAFO com as informações elétricas da rede trifasica
 * @param barras_tf estrutura do tipo TF_DBAR com as informacoes sobre as barras da rede
 * @param numeroBarras_tf inteiro com o numero de barras da rede 
 * @param alimentador_tf vetor de estrutura do tipo TF_ALIMENTADOR com as informacoes sobre os alimentadores 
 * @param curvasTrafos vetor de estrutura do tipo TF_CURVA_TRAFO com as informacoes sobre as curvas de cargas agregadas da rede
 * @param numeroAlimentadores inteiro com o numero de alimentadores da rede
 * @param ramo_tf vetor do tipo TF_DRAM com as informacoes sobre os ramos da rede
 * @param Sbase double com a potência base da rede
 * @param interfaceNiveis_tf matriz de inteiros com as interfaces da rede
 * @param numeroInterfaces_tf inteiro com o numero de interfaces
 * @param areasMedicao_tf vetor de estrutura do tipo TF_AREA_MEDICAO com as informacoes sobre as areas de medicao da rede
 * @param prev_tf vetor de estrutura do tipo TF_DPREV com os dados de previsao de medidas
 * @param medidaPrev_tf estrutura do tipo TF_DMED utilizada pra comportar as medidas previstas
 * @param numeroMedidasTabela matriz de inteiros com o numero de medidas e os tipos
 * @param numeroAmostras inteiro com o numero de amostras
 * @return TF_NCRESULT* retorna os dados compilados do nowcasting de demanda
 */
TF_NCRESULT* NowCastingDemanda_tf(TF_GRAFO *grafo_tf, TF_DBAR *barras_tf ,long int numeroBarras_tf, TF_ALIMENTADOR *alimentador_tf, TF_CURVA_TRAFO *curvasTrafos, 
 long int numeroAlimentadores, TF_DRAM *ramo_tf,double Sbase, long int **interfaceNiveis_tf,long int numeroInterfaces_tf, TF_AREA_MEDICAO *areasMedicao_tf, TF_DPREV *prev_tf, 
 TF_DMED *medidaPrev_tf ,long int **numeroMedidasTabela, int numeroAmostras,CONFIGURACAO *configuracoesParam, long int idNovaConfiguracaoParam, DADOSALIMENTADOR *dadosAlimentadorParam,RNPSETORES *matrizB,TF_PFSOLUTION *powerflow_result_rede, TF_PFSOLUTION **powerflow_result_alim)
{

    int nmed=0;
    char ts[15];
    TF_NCRESULT* result;
    (result) = (TF_NCRESULT *) malloc((numeroAmostras+1)*sizeof(TF_NCRESULT));
    
    for (size_t i = 0; i < numeroAmostras; i++)
    {
        result[i].dadosBarras= (TF_NCBAR *) malloc((numeroBarras_tf+1)*sizeof(TF_NCBAR));
    }
    

    nmed=0;
    for (size_t i = 0; i < 14; i++){ 
         for (size_t j = 0; j < 20; j++){
              nmed += numeroMedidasTabela[i][j];
         }
    }
    
    for (size_t instante_atual = 0; instante_atual < numeroAmostras; instante_atual++)
    {

        // receber a curva trafos
        // fazer rodar toda vez que mudar a hora do instante atual e colocar na inicializa
        inicializaPQcurvas(barras_tf, curvasTrafos, numeroBarras_tf,  prev_tf[0].hour[instante_atual], VALOR_ESPERADO, Sbase); // conferir com o Julio se é isso mesmo

        atualiza_dmed(instante_atual,prev_tf,nmed,medidaPrev_tf); 
        atualiza_AM(grafo_tf,numeroBarras_tf,medidaPrev_tf,nmed,areasMedicao_tf);
        //Estimador de Demandas Trifásicas
        estimadorDemandaTrifasico_tf(grafo_tf, numeroBarras_tf, alimentador_tf, numeroAlimentadores, ramo_tf, Sbase, interfaceNiveis_tf, numeroInterfaces_tf, areasMedicao_tf,configuracoesParam,idNovaConfiguracaoParam,matrizB,dadosAlimentadorParam,powerflow_result_rede,powerflow_result_alim);
        preenche_result_NC(grafo_tf,numeroBarras_tf,instante_atual,result);

        sprintf(ts,"%d",prev_tf[0].time_stamp[instante_atual]);// pega o time stamp atual
        imprimeDBAR_cargas(grafo_tf,numeroBarras_tf,ts);
    }
    



    return(result);
}



/**
 * @brief Funcao que compila os resultados do Now casting no instante atual 
 * 
 * Recebe como parâmetros @p grafo_tf vetor do tipo TF_GRAFO com as informacoes sobre a rede elétrica trifasica, @p numeroBarras_tf inteiro
 * com o numero de barras na rede, @p instante_atual inteiro com o instante atual, @p result vetor do tipo  TF_NCRESULT com os dados compilados da rede elétrica
 * 
 * 
 * @param grafo_tf 
 * @param numeroBarras_tf 
 * @param instante_atual 
 * @param result 
 */
void preenche_result_NC(TF_GRAFO *grafo_tf, int numeroBarras_tf, int instante_atual,TF_NCRESULT * result)
{
    
    for (size_t i = 0; i < numeroBarras_tf; i++)
    {
        result[instante_atual].dadosBarras[i].P[0]= creal(grafo_tf[i].S[0]);
        result[instante_atual].dadosBarras[i].P[1]= creal(grafo_tf[i].S[1]);
        result[instante_atual].dadosBarras[i].P[2]= creal(grafo_tf[i].S[2]);
        result[instante_atual].dadosBarras[i].Q[0]= cimag(grafo_tf[i].S[0]);
        result[instante_atual].dadosBarras[i].Q[1]= cimag(grafo_tf[i].S[1]);
        result[instante_atual].dadosBarras[i].Q[2]= cimag(grafo_tf[i].S[2]);
    }
    


}

/**
 * @brief Funcao que imprime um arquivo de texto com o o DBAR de acordo com as cargas no grafo da rede trifásica
 * 
 * Recebe com parâmetro @p grafo_tf com as informacoes sobre a rede elétrias, @p numerobarras_tf com o numero de barras totais da rede 
 * trifasica, @p ts uma string com o nome do arquivio que será salvo
 * 
 * @param grafo_tf vetor do tipo TF_GRAFO com as informações elétricas da rede trifasica
 * @param numerobarras_tf inteiro com o numero de barras da rede
 * @param ts string com o sufixo do nome do DBAR
 */
void imprimeDBAR_cargas(TF_GRAFO *grafo_tf, int numerobarras_tf, const char *ts)
{   
    // mudar para entrar com o nome do arquivo
    FILE *arquivo;
    char nome[100]="DBAR_";
    
    strcat(nome,ts);
    strcat(nome,".dad");

    
    arquivo = fopen(nome,"w");
    for (size_t i = 0; i < numerobarras_tf; i++)
    {
        if(grafo_tf[i].tipo==2)
        {
            fprintf(arquivo,"%d;%d;%d;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%d;%.7f;%.7f;%.7f;%.7f;%.7f;%.7f;\n",grafo_tf[i].barra[0].ID,grafo_tf[i].barra[0].ligacao,grafo_tf[i].barra[0].fases,creal(grafo_tf[i].S[0]),creal(grafo_tf[i].S[1]),creal(grafo_tf[i].S[2]),cimag(grafo_tf[i].S[0]),cimag(grafo_tf[i].S[1]),cimag(grafo_tf[i].S[2]),0,cabs(grafo_tf[i].V[0]),cabs(grafo_tf[i].V[1]),cabs(grafo_tf[i].V[2]),carg(grafo_tf[i].V[0])*180/PI,carg(grafo_tf[i].V[1])*180/PI,carg(grafo_tf[i].V[2])*180/PI);
        }
        else if(grafo_tf[i].tipo==1)
        {
            fprintf(arquivo,"%d;%d;%d;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%d;%.7f;%.7f;%.7f;;;;\n",grafo_tf[i].barra[0].ID,grafo_tf[i].barra[0].ligacao,grafo_tf[i].barra[0].fases,creal(grafo_tf[i].S[0]),creal(grafo_tf[i].S[1]),creal(grafo_tf[i].S[2]),cimag(grafo_tf[i].S[0]),cimag(grafo_tf[i].S[1]),cimag(grafo_tf[i].S[2]),0,cabs(grafo_tf[i].V[0]),cabs(grafo_tf[i].V[1]),cabs(grafo_tf[i].V[2]));
        }
        else{
            fprintf(arquivo,"%d;%d;%d;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%d;;;;;;;\n",grafo_tf[i].barra[0].ID,grafo_tf[i].barra[0].ligacao,grafo_tf[i].barra[0].fases,creal(grafo_tf[i].S[0]),creal(grafo_tf[i].S[1]),creal(grafo_tf[i].S[2]),cimag(grafo_tf[i].S[0]),cimag(grafo_tf[i].S[1]),cimag(grafo_tf[i].S[2]),0);
        }
    }
    fclose(arquivo);
}