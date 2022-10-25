#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <complex.h>

#include "data_structures_tf.h"
#include "funcoesMatematicas_tf.h"

long int maxIT = 10000;


//------------------------------------------------------------------------------
//
// Funções com Matrizes complexas
//
//------------------------------------------------------------------------------
//Aloca Vetor
__complex__ double *c_vetAloca(int n){
    int j;
    __complex__ double *m;
    
    m = (__complex__ double*)malloc(n * sizeof(__complex__ double));
    
    for (j = 0; j < n; j++){ 
         m[j] = 0; //Inicializa com 0.
    }

    return m; 
}
//Aloca Matriz
complex double **c_matAloca(int n){
    int i,j;
    complex double **m=NULL;
    
    if((m= (complex double**)malloc(n * sizeof(complex double*)))==NULL)
    {
        printf("Erro -- Nao foi possivel alocar espaco de memoria para uma matriz !!!!");
        exit(1); 
    }

    for (i = 0; i < n; i++){
        if((m[i] = (complex double*) malloc(n * sizeof(complex double)))==NULL)
        {
            printf("Erro -- Nao foi possivel alocar espaco de memoria para uma matriz !!!!");
            exit(1); 
        }
       for (j = 0; j < n; j++){ 
            m[i][j] = 0; //Inicializa com 0.
       }
    }
    return m; 
}
//Conjugado de matrizes
void c_matConj(__complex__ double **A, int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            A[i][j] = conj(A[i][j]);
        }
    }   
}

//Igualdade de matrizes
void c_matIgual(__complex__ double **A, __complex__ double **B, int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            A[i][j] = B[i][j];
        }
    }   
}
//Inversa da matriz método Decomposição LU (Para calcular a matriz admitância de linhas)
void c_matInversaZ(__complex__ double **A, int n){
    int i,j,k;
    __complex__ double **Zaux,piv;
        
    //Aloca Matriz
    Zaux = c_matAloca(3);
    Zaux[0][0] = 1;
    Zaux[1][1] = 1;
    Zaux[2][2] = 1;
    
    //Etapa Forward
    for (k=0; k<3-1; k++)
    {
        for (i=(k+1); i<3; i++)
        {
            if (A[k][k] != (0+I*0) && A[i][k] != (0+I*0)){
                piv=-((A[i][k])/(A[k][k]));
                //printf("%lf %lf\n", real(piv), imag(piv));
                //getchar();
                for (j=0; j<3; j++)
                {                      
                    A[i][j]=(A[i][j]+(piv*A[k][j]));
                    Zaux[i][j]=(Zaux[i][j]+(piv*Zaux[k][j]));
                }
            }
        }
    }
    //Etapa Diagonalização
    for (i=0; i<3; i++)
    {
        if (A[i][i]!= (0+I*0)){
            piv=(1/(A[i][i]));
            // printf("%lf %lf\n", real(piv), imag(piv));
            for (j=0; j<3; j++)
            {
                A[i][j]=(piv*A[i][j]);
                Zaux[i][j]=(piv*Zaux[i][j]);
            }
        }
        else{
            Zaux[i][i]=(0+I*0);
        }
    }
    //Etapa Backward
    for (k=0; k<3-1; k++)
    {
        for (i=(k+1); i<3; i++)
        {
            piv=-(A[k][i]);
            //printf("%lf %lf\n", real(piv), imag(piv));
            for (j=0; j<3; j++)
            {
                    A[k][j]=(A[k][j]+(piv*A[i][j]));
                    Zaux[k][j]=(Zaux[k][j]+(piv*Zaux[i][j]));
            }

        }
    }
    c_matIgual(A,Zaux,3);

    for(i = 0;i<3;i++) free(Zaux[i]);
    free(Zaux);
}

//Transposta
void c_matTransp(__complex__ double **A, int n){
    int i,j;
    __complex__ double Aux[n][n];
    
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            Aux[i][j] = A[i][j];
        }
    }
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            A[i][j] = Aux[j][i];
        }
    }
}
//multiplica por escalar
void c_matMultEsc(__complex__ double **A, __complex__ double b, int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            A[i][j] = b*A[i][j];
        }
    }   
}

////multiplica matrizes quadradas e retorna em A
void c_multMatMat(__complex__ double **A, __complex__ double **B, int n){
    int i,j,k;
    __complex__ double aux, laux[n];
    
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            aux = 0;
            for(k=0;k<n;k++){
                aux += A[i][k]*B[k][j];
            }
            laux[j] = aux;
        }
        for(j=0;j<n;j++){
            A[i][j] = laux[j];
        }
    }
}
////multiplica matrizes quadradas e retorna em A
void c_multMatVet(__complex__ double **A, __complex__ double *B, int n){
    int i,j,k;
    __complex__ double aux, laux[n];
    
    for(i=0;i<n;i++){
        aux = 0;
        for(j=0;j<n;j++){
            aux += A[i][j]*B[j];
        }
        A[i][0] = aux;
    }
}

//Imprime Matriz na Tela
void c_matImprime(__complex__ double **A, int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%.3lf +i %.3lf \t\t", __real__ A[i][j],__imag__ A[i][j]);
        }
        printf("\n");
    }   
}

//Efetua a backward substituion em matriz triangular superior e retorna o vetor x no lugar de b
void c_backwardSubs (complex double **A, long int m, long int n, complex double *b){
    int i, j;
    complex double sum;
    
    //Substituição Backward
    for (i=(n-1);i>=0;i--)
    {
        sum = 0;
        for (j=(n-1);j>i;j--)
        {
            sum = sum + A[i][j]*b[j];
        }
        b[i] = (b[i] - sum)/A[i][i];
    }
}

//Efetua a forward substituion em matriz triangular inferior e retorna o vetor x no lugar de b
void c_forwardSubs (complex double **A, long int m, long int n, complex double *b){
    int i, j;
    complex double sum;
    
    //Substituição Forward
    for (i=0;i<m;i++)
    {
        sum = 0;
        for (j=0;j<i;j++)
        {
            sum = sum + A[i][j]*b[j];
        }
        b[i] = (b[i] - sum)/A[i][i];
    }
}


//------------------------------------------------------------------------------
//
// Funções com Matrizes Reais
//
//------------------------------------------------------------------------------
//ALoca matriz de double
double **aloca_matriz(int m, int n){
    int i,j;
    double **A;

    A=(double**)malloc(m*sizeof(double*));
    for (i=0;i<m;i++)
       A[i]=(double*)malloc(n*sizeof(double));

    for (i=0;i<m;i++){
        for (j=0;j<n;j++){
            A[i][j] = 0;
        }
    }
    return(A);
}

//Aloca vetor de inteiro
long int *aloca_vetor_int(int m){
    int i;
    long int *A;

    A=(long int*)malloc(m*sizeof(long int));

    for (i=0;i<m;i++){
        A[i] = 0;
    }
    return(A);
}

//Aloca vetor de double
double *aloca_vetor(int m){
    int i;
    double *A;

    A=(double*)malloc(m*sizeof(double));

    for (i=0;i<m;i++){
        A[i] = 0;
    }
    return(A);
}
//Função que retorna o produto escalar (interno) de dois vetores c = <a,b>
double prod_escalar(double *a,double *b,int n)
{
     int i;
     double c;

     c=0;
     for (i=0;i<n;i++)
     {
         c += a[i]*b[i];
     }
     return(c);
}
//Iguala Matrizes B = A
void mat_ig(double ***A,int m,int n, double **B){
     int i,j;

     for (i=0;i<m;i++)
     {
         for (j=0;j<n;j++)
         {
             B[i][j]=*A[i][j];
         }

     }
}
//Função que retorna a norma infinita de um vetor
//c = max( abs(a))
double norma_inf(double *a,int n)
{
     int i;
     double c;

     c = cabs(a[0]);
     for (i=0;i<n;i++)
     {
         if (cabs(a[i]) > c) c = cabs(a[i]);
     }

     return(c);
}
//Função que retorna a norma euclidiana de um vetor
//c = sum( abs(a))
double norma_euc(double *a,int n)
{
     int i;
     double c = 0;

     for (i=0;i<n;i++)
     {
         c = c + pow(a[i],2);
     }
     c = sqrt(c);

     return(c);
}
void matTransp( double **A, int m, int n, double **At){
    int i,j;
    
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            At[j][i] = A[i][j];
        }
    }
}

//Tira coluna de matriz
void tira_refs(double ***A,int m,int n,int col1, int col2,double **temp, double *regua, double *x, long int it){
    int i,j,n_cols;
    
    n_cols = col2 - col1 + 1;
    for (i=0;i<n;i++)
    {
        for (j=0;j<m;j++)
        {
            if (j<col1)
               temp[i][j] = *A[i][j];
            else if (j>col2)
                temp[i][(j-n_cols)] = *A[i][j];
        }
    }
    if (it == 0){
        for (j=0;j<m;j++)
        {
            if (j<col1){
                regua[j] = regua[j];
                x[j] = x[j];
            }
            else if (j>col2){
                regua[(j-n_cols)] = regua[j];
                x[(j-n_cols)] = x[j];
            }
        }
    }
}
//Concatena horizontalmente duas matrizes
void cat_hor(double **A,int m1,int n1,double **B,int m2,int n2, double **temp){
    int i,j;

    for (j=0;j<m1;j++)
        {
        for (i=0;i<n1;i++)
        {
            temp[j][i] = A[j][i];
        }
        for (i=0;i<n2;i++)
        {
            temp[j][i+n1] = B[j][i];
        }
    }
}
//Fun��o que multiplica Matrizes
//C = A*B  sizeA[m1 x n1]   sizeB[m2 x n2]
void mult_mat(double **A,int m1,int n1,double **B,int m2, int n2, double **C){
     int i,j,k;
     double aux_sum;


    for( i = 0; i < m1; i++ )
	{
        for( j = 0; j<n2; j++ )
	    {
	        aux_sum=0;
	        for( k=0; k<m2; k++ )
	                 aux_sum = aux_sum + A[i][k] * B[k][j];

            //if (fabs(aux_sum) <precs) C[i][j] = 0;
//            else
                C[i][j] = aux_sum;
	    }
	}
}
void multMatVet(double **A, double *b,long int m, long int n, double *c){
    int i,j;
    for(i=0;i<m;i++){
        c[i] = 0;
        for(j=0;j<n;j++){
            if(A[i][j] != 0) c[i] += A[i][j]*b[j];
        }
    }
}
void somaMatMat(double **A, double **B, long int m, long int n){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            A[i][j] = A[i][j] + B[i][j];
        }
    }
}
//Concatena horizontalmente duas matrizes
void cat_vert(double **A,int m1,int n1,double **B,int m2,int n2, double **temp){
    int i,j;

    for (j=0;j<m1;j++)
        {
        for (i=0;i<n1;i++)
        {
            temp[j][i] = A[j][i];
        }
    }
    for (j=0;j<m2;j++)
        {
        for (i=0;i<n2;i++)
        {
            temp[j+m1][i] = B[j][i];
        }
    }
}
//Concatena horizontalmente dois vetores
void cat_vert_vet(double *A,int m1,double *B,int m2, double *temp){
    int j;

    for (j=0;j<m1;j++){
        temp[j] = A[j];
    }
    for (j=0;j<m2;j++){
        temp[j+m1] = B[j];
    }
}

//------------------------------------------------------------------------------
//
// Funções de Algebra Linear
//
//------------------------------------------------------------------------------
//Retorna o maior autovalor da matriz A caculado pelo método das potências
double eigenvalue_largest(double **A, long int m, long int n){ 
    int i,j;
    double eigen = 0, nDx = 1, tol =0.00001;
    double *x,*x0,*Dx;
    
    x = aloca_vetor(m);
    x0 = aloca_vetor(m);
    Dx = aloca_vetor(m);
    for (i=0;i<m;i++) x0[i] = 1;
    while (nDx>tol){
        double nx0 = norma_euc(x0,m);
        
        for (i=0;i<m;i++){
            x[i] = 0;
            for (j=0;j<n;j++){
                if (cabs(A[i][j]) >= 0.00000001)                
                    x[i] += A[i][j]*x0[i]/nx0;
            }
        }
        for (i=0;i<m;i++){
            Dx[i] = x[i] - x0[i];
            x0[i] = x[i];
        }
        nDx = norma_inf(Dx,m);        
    }
    
    
    for (i=0;i<m;i++){
        x0[i] = 0;
        for (j=0;j<n;j++){
            if (cabs(A[i][j]) >= 0.00000001)                
                x0[i] += A[i][j]*x[i];
        }
    }
    eigen = prod_escalar(x0,x,m)/prod_escalar(x,x,m);       
    
    return(eigen);    
}
//Realiza a fatoração QR e retorna a matriz R 
void QRfactorization(double **A, int m, int n, double **R){
    int i, j,k;
    double *x,*v,*col,**HA;
    double d,w,f,sum;

    x = (double *)calloc(n,sizeof(double));
    v = (double *)calloc(m,sizeof(double));
    col = (double *)calloc(m,sizeof(double));
    HA = (double **)calloc(m,sizeof(double*));
    for (i=0;i<m;i++){
        HA[i] = (double *)calloc(n,sizeof(double));
    }

    //Inicialização de variáveis
    for (i=0;i<m;i++)
    {
        for (j=0;j<n;j++)
        {
            HA[i][j] = A[i][j];
        }
    }
    //Aplicação das transformações de Householder
    for (j=0;j<n;j++){
        for (i=j;i<m;i++){
            col[i-j] = HA[i][j];
        }
        if (HA[j][j] > 0) d = - sqrt(prod_escalar(col,col,m-j));
        else d = sqrt(prod_escalar(col,col,m-j));

        w = HA[j][j] - d;
        f = sqrt(-2*w*d);
        for (i=j;i<m;i++){
            v[i-j] = HA[i][j]/f;
        }
        v[0] = w/f;
        for (i=j+1;i<m;i++){
            HA[i][j] = 0;
        }
        HA[j][j] = d;

        for (k=j+1;k<n;k++){
            for (i=j;i<m;i++){
                col[i-j] = HA[i][k];
            }
            f = 2*prod_escalar(v,col,m-j);
            for (i=j;i<m;i++){
                HA[i][k] = HA[i][k] - f*v[i-j];
            }
        }
    }
    for (i=0;i<m;i++)
    {
        for (j=0;j<n;j++)
        {
            R[i][j] = HA[i][j];
        }
    }
}

//Efetua a backward substituion em matriz triangular superior e retorna o vetor x no lugar de b
void backwardSubs (double **A, long int m, long int n, double *b){
    int i, j;
    double sum;
    
    //Substituição Backward
    for (i=(n-1);i>=0;i--)
    {
        sum = 0;
        for (j=(n-1);j>i;j--)
        {
            sum = sum + A[i][j]*b[j];
        }
        b[i] = (b[i] - sum)/A[i][i];
    }
}
//Efetua a forward substituion em matriz triangular inferior e retorna o vetor x no lugar de b
void forwardSubs (double **A, long int m, long int n, double *b){
    int i, j;
    double sum;
    
    //Substituição Forward
    for (i=0;i<m;i++)
    {
        sum = 0;
        for (j=0;j<i;j++)
        {
            sum = sum + A[i][j]*b[j];
        }
        b[i] = (b[i] - sum)/A[i][i];
    }
}

//**************************************************************************
//REFLEXÕES DE HOUSEHOLDER
/*double *solve_Householder(double **A,int m,int n, double *b)
{
    int i, j,k;
    double *x,*v,*col,**HA, *Hb;
    double d,w,f,sum;

    x = (double *)calloc(m,sizeof(double));
    v = (double *)calloc(m,sizeof(double));
    col = (double *)calloc(m,sizeof(double));
    Hb = (double *)calloc(m,sizeof(double));
    HA = (double **)calloc(m,sizeof(double*));
    for (i=0;i<m;i++)
    {
        HA[i] = (double *)calloc(n,sizeof(double));
    }

    //Inicialização de variáveis
    for (i=0;i<m;i++)
    {
        Hb[i] = b[i];
        for (j=0;j<n;j++)
        {
            HA[i][j] = A[i][j];
        }
    }

    //Aplicação das transformações de Householder
    for (j=0;j<n-1;j++){
        for (i=j;i<m;i++){
            col[i-j] = HA[i][j];
        }
        if (HA[j][j] > 0) d = - sqrt(prod_escalar(col,col,m-j));
        else d = sqrt(prod_escalar(col,col,m-j));

        w = HA[j][j] - d;
        f = sqrt(-2*w*d);
        for (i=j;i<m;i++){
            v[i-j] = HA[i][j]/f;
        }
        v[0] = w/f;
        for (i=j+1;i<m;i++){
            HA[i][j] = 0;
        }
        HA[j][j] = d;

        for (k=j+1;k<n;k++){
            for (i=j;i<m;i++){
                col[i-j] = HA[i][k];
            }
            f = 2*prod_escalar(v,col,m-j);
            for (i=j;i<m;i++){
                HA[i][k] = HA[i][k] - f*v[i-j];
            }
        }
        //Aplica as tranformações de Householder no vetor b
        for (i=j;i<m;i++){
            col[i-j] = Hb[i];
        }
        f = 2*prod_escalar(v,col,m-j);
        for (i=j;i<m;i++){
            Hb[i] = Hb[i] - f*v[i-j];
        }
    }
    //Substituição Backward
    for (i=(n-1);i>=0;i--)
    {
        sum = 0;
        for (j=(n-1);j>i;j--)
        {
            sum = sum + HA[i][j]*x[j];
        }
        x[i] = (Hb[i] - sum)/HA[i][i];
    }

    return(x);
}
*/
//Householder refelctions com precondicionador Jacobi
double *solve_PreconHouseholder(double **A,int m,int n, double *b)
{
    int i, j,k;
    double *x,*v,*col,**HA, *Hb;
    double d,w,f,sum;
    
    double **Precon;
    Precon = aloca_matriz(m,n);
    for(i=0;i<m;i++) Precon[i][i] = 1/sqrt(A[i][i]);
    
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            A[i][j] = A[i][j]*Precon[i][i];
        }
    }
    
    x = (double *)calloc(m,sizeof(double));
    v = (double *)calloc(m,sizeof(double));
    col = (double *)calloc(m,sizeof(double));
    Hb = (double *)calloc(m,sizeof(double));
    HA = (double **)calloc(m,sizeof(double*));
    for (i=0;i<m;i++)
    {
        HA[i] = (double *)calloc(n,sizeof(double));
    }

    //Inicialização de variáveis
    for (i=0;i<m;i++)
    {
        Hb[i] = b[i];
        for (j=0;j<n;j++)
        {
            HA[i][j] = A[i][j];
        }
    }
    //Aplicação das transformações de Householder
    for (j=0;j<n-1;j++){
        for (i=j;i<m;i++){
            col[i-j] = HA[i][j];
        }
        if (HA[j][j] > 0) d = - sqrt(prod_escalar(col,col,m-j));
        else d = sqrt(prod_escalar(col,col,m-j));

        w = HA[j][j] - d;
        f = sqrt(-2*w*d);
        for (i=j;i<m;i++){
            v[i-j] = HA[i][j]/f;
        }
        v[0] = w/f;
        for (i=j+1;i<m;i++){
            HA[i][j] = 0;
        }
        HA[j][j] = d;

        for (k=j+1;k<n;k++){
            for (i=j;i<m;i++){
                col[i-j] = HA[i][k];
            }
            f = 2*prod_escalar(v,col,m-j);
            for (i=j;i<m;i++){
                HA[i][k] = HA[i][k] - f*v[i-j];
            }
        }
        //Aplica as tranformações de Householder no vetor b
        for (i=j;i<m;i++){
            col[i-j] = Hb[i];
        }
        f = 2*prod_escalar(v,col,m-j);
        for (i=j;i<m;i++){
            Hb[i] = Hb[i] - f*v[i-j];
        }
    }
    //Substituição Backward
    for (i=(n-1);i>=0;i--)
    {
        sum = 0;
        for (j=(n-1);j>i;j--)
        {
            sum = sum + HA[i][j]*x[j];
        }
        x[i] = (Hb[i] - sum)/HA[i][i];
    }
/*for (i=0;i<m;i++)
    {
        for (j=0;j<n;j++)
        {
            printf("%lf\t", HA[i][j]);
        }
        printf("\n");
    }*/
    
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            A[i][j] = A[i][j]/Precon[i][i];
        }
    }
    for(j=0;j<n;j++){
        x[j] = x[j]/Precon[j][j];
    }
    
    return(x);
}

double *solve_Householder_LS(double **A,int m,int n, double *b){
    int i, j,k;
    double *x,*v,*col,**HA, *Hb;
    double d,w,f,sum;

    x = (double *)calloc(n,sizeof(double));
    v = (double *)calloc(m,sizeof(double));
    col = (double *)calloc(m,sizeof(double));
    Hb = (double *)calloc(m,sizeof(double));
    HA = (double **)calloc(m,sizeof(double*));
    for (i=0;i<m;i++){
        HA[i] = (double *)calloc(n,sizeof(double));
    }

    //Inicialização de variáveis
    for (i=0;i<m;i++)
    {
        Hb[i] = b[i];
        for (j=0;j<n;j++)
        {
            HA[i][j] = A[i][j];
        }
    }
    //Aplicação das transformações de Householder
    for (j=0;j<n;j++){
        for (i=j;i<m;i++){
            col[i-j] = HA[i][j];
        }
        if (HA[j][j] > 0) d = - sqrt(prod_escalar(col,col,m-j));
        else d = sqrt(prod_escalar(col,col,m-j));

        w = HA[j][j] - d;
        f = sqrt(-2*w*d);
        for (i=j;i<m;i++){
            v[i-j] = HA[i][j]/f;
        }
        v[0] = w/f;
        for (i=j+1;i<m;i++){
            HA[i][j] = 0;
        }
        HA[j][j] = d;

        for (k=j+1;k<n;k++){
            for (i=j;i<m;i++){
                col[i-j] = HA[i][k];
            }
            f = 2*prod_escalar(v,col,m-j);
            for (i=j;i<m;i++){
                HA[i][k] = HA[i][k] - f*v[i-j];
            }
        }
        //Aplica as tranformações de Householder no vetor b
        for (i=j;i<m;i++){
            col[i-j] = Hb[i];
        }
        f = 2*prod_escalar(v,col,m-j);
        for (i=j;i<m;i++){
            Hb[i] = Hb[i] - f*v[i-j];
        }
    }
    //Substituição Backward
    for (i=(n-1);i>=0;i--)
    {
        sum = 0;
        for (j=(n-1);j>i;j--)
        {
            sum = sum + HA[i][j]*x[j];
        }
        x[i] = (Hb[i] - sum)/HA[i][i];
    }
/*for (i=0;i<m;i++){
        for (j=0;j<n;j++){
            printf("%lf\t", HA[i][j]);
        }
        printf("\n");
    }*/
    free(v); free(col);free(Hb);
    for (i=0;i<m;i++) free(HA[i]);
    free(HA);
    return(x);
}

double *solve_Householder(double **A,int m,int n, double *b){
    int i, j,k;
    double *x,*v,*col,**HA, *Hb;
    double d,w,f,sum;

    x = (double *)calloc(m,sizeof(double));
    v = (double *)calloc(m,sizeof(double));
    col = (double *)calloc(m,sizeof(double));
    Hb = (double *)calloc(m,sizeof(double));
    HA = (double **)calloc(m,sizeof(double*));
    for (i=0;i<m;i++){
        HA[i] = (double *)calloc(n,sizeof(double));
    }

    //Inicialização de variáveis
    for (i=0;i<m;i++)
    {
        Hb[i] = b[i];
        for (j=0;j<n;j++)
        {
            HA[i][j] = A[i][j];
        }
    }
    //Aplicação das transformações de Householder
    for (j=0;j<n-1;j++){
        for (i=j;i<m;i++){
            col[i-j] = HA[i][j];
        }
        if (HA[j][j] > 0) d = - sqrt(prod_escalar(col,col,m-j));
        else d = sqrt(prod_escalar(col,col,m-j));

        w = HA[j][j] - d;
        f = sqrt(-2*w*d);
        for (i=j;i<m;i++){
            v[i-j] = HA[i][j]/f;
        }
        v[0] = w/f;
        for (i=j+1;i<m;i++){
            HA[i][j] = 0;
        }
        HA[j][j] = d;

        for (k=j+1;k<n;k++){
            for (i=j;i<m;i++){
                col[i-j] = HA[i][k];
            }
            f = 2*prod_escalar(v,col,m-j);
            for (i=j;i<m;i++){
                HA[i][k] = HA[i][k] - f*v[i-j];
            }
        }
        //Aplica as tranformações de Householder no vetor b
        for (i=j;i<m;i++){
            col[i-j] = Hb[i];
        }
        f = 2*prod_escalar(v,col,m-j);
        for (i=j;i<m;i++){
            Hb[i] = Hb[i] - f*v[i-j];
        }
    }
    //Substituição Backward
    for (i=(n-1);i>=0;i--)
    {
        sum = 0;
        for (j=(n-1);j>i;j--)
        {
            sum = sum + HA[i][j]*x[j];
        }
        x[i] = (Hb[i] - sum)/HA[i][i];
    }
    return(x);
}

//Algoritmo de Crout
double *solve_Crout(double **A,int m,int n, double *b)
{
    int i,j,k;
    double **F,**L,**U,*x,*y;
    double sum;

    x = (double *)calloc(m,sizeof(double));
    y = (double *)calloc(m,sizeof(double));
    F = (double **)calloc(m,sizeof(double*));
    L = (double **)calloc(m,sizeof(double*));
    U = (double **)calloc(m,sizeof(double*));
    for (i=0;i<m;i++)
    {
        F[i] = (double *)calloc(n,sizeof(double));
        L[i] = (double *)calloc(n,sizeof(double));
        U[i] = (double *)calloc(n,sizeof(double));
    }

    //----------------------------------------------------------
    //Fatoração pelo algoritmo de Crout
    for(j=0;j<n;j++)
    {
        F[j][0] = A[j][0];
    }
    for(j=0;j<n;j++)
    {
        F[0][j] = A[0][j]/A[0][0];
    }

    for (j=0;j<n;j++)
    {
        for(k=j;k<n;k++)
        {
            sum = 0;
            for(i=0;i<j;i++)
            {
                sum = sum + F[k][i]*F[i][j];
            }
            F[k][j] = A[k][j] - sum;
            if (fabs(F[k][j])<EPS) F[k][j] = 0;
        }
        if(j!=n)
        {
        for(k=(j+1);k<n;k++)
        {
            sum = 0;
            for(i=0;i<j;i++)
            {
                sum = sum + F[j][i]*F[i][k];
            }
            F[j][k] = (A[j][k] - sum)/F[j][j];
            if (fabs(F[j][k])<EPS) F[j][k] = 0;
        }
        }
    }
    //Fim da Fatoração
    //----------------------------------------------------------

    for (i=0;i<m;i++)
    {
        for (j=0;j<n;j++)
        {
            if (j<=i)
                L[i][j] = F[i][j];
            else
                U[i][j] = F[i][j];
        }
        U[i][i] = 1;
    }


    //---------------------------------------------------
    //Resolve Ly = b
    for (i=0;i<n;i++)
    {
        sum = 0;
        for (j=0;j<i;j++)
        {
            sum = sum + L[i][j]*y[j];
        }
        if (L[i][i] !=0) y[i] = (b[i] - sum)/L[i][i];
    }
    //---------------------------------------------------
    //Resolve Ux = y
    for (i=(n-1);i>=0;i--)
    {
        sum = 0;
        for (j=(n-1);j>i;j--)
        {
            sum = sum + U[i][j]*x[j];
        }
        x[i] = (y[i] - sum);
    }

    return(x);
}

//------------------------------------------------------------------------------
//
// Funções de Matrizes Esparsas
//
//------------------------------------------------------------------------------





