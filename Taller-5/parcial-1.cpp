#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <omp.h>

#define N 5000

void imprimirMatriz(double M[], int f, int c)
{
	for (int i=0;i<f;i++)
	{
		for(int j=0;j<c;j++)
			printf(" %.2f \t",M[i*c+j]);
		printf("\n");
	}
		
}

//------------------------------ VERSION SERIAL -------------------------- 


double generateMatrixSerial(double M[], int n){
    double t1 = omp_get_wtime(); // tiempo iniciO
    int c,d;
    for(c = 0; c < n; c++){
        for(d = 0; d < n; d++){
            M[c*n+d] = 0;
        }
    }
    double t2 = omp_get_wtime(); // tiempo finzaliza
    return (t2-t1);  // diferencia
}


double transposeSerial(double M[], double MT[], int n){
   	double t1 = omp_get_wtime(); // tiempo iniciO
    int c,d;
    for(c = 0; c < n; c++){ 
        for(d = 0; d < n; d++){ 
            MT[d*n+c] = M[c*n+d];
        }
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	return (t2-t1);  // diferencia
}



void checkSymmetrySerial(double M[],double MT[], int n, double difS){
    double t1 = omp_get_wtime(); // tiempo inicio
    int c,d;
    int x = 1;
    for(c = 0; c < n; c++){
        if(x == 0) continue;
        for(d = 0; d < n; d++){
            if(x == 0) continue;
             if(M[c*n+d] != MT[c*n+d] ){
                 x=0;
             }
         }
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
    difS= (t2-t1);
    printf ("\n - **_**Suma de elementos secuencial = %ld - tiempo = %f \n",x,difS);
}

//------------------------------ VERSION PARALELIZADA -------------------------- 

double generateMatrixParallel(double M[], int n, int k){
    double t1 = omp_get_wtime(); // tiempo inicio
    int c,d;
   	#pragma omp parallel for private(c,d) shared(M)  num_threads(k)
    for(c = 0; c < n; c++){
        for(d = 0; d < n; d++){
            M[c*n+d]= 0;
        }
    }
    int v1=rand()%n;
    int v2=rand()%n;
    M[v1*n+v2] = 1;
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	return (t2-t1);  // diferencia
 }

double transposeParallel(double M[], double MT[], int n, int k){
    double t1 = omp_get_wtime(); // tiempo inicio
    int c,d;
    #pragma omp parallel for private(c,d) shared(M, MT)  num_threads(k)
    for(c = 0; c < n; c++){ 
        for(d = 0; d < n; d++){ 
            MT[d*n+c] = M[c*n+d];
        }
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	return (t2-t1);  // diferencia
}


void checkSymmetryParallel(double M[],double MT[], int n, int k, double difP){
    double t1 = omp_get_wtime(); // tiempo inicio
    int c,d;
    int x = 1;
    #pragma omp parallel for private(c,d,x) shared(M, MT)  num_threads(k)    
     for(c = 0; c < n; c++){
        if(x == 0) continue;
        for(d = 0; d < n; d++){
            if(x == 0) continue;
             if(M[c*n+d] != MT[c*n+d] ){
                x=0;
             }
         }
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	difP= t2-t1;  // diferencia
    printf (" - Suma de elementos pararell = %ld - tiempo = %f \n",x,difP);
}

int main(){

    double *M, *MT;
    int n;
    double difP;
    double difS;

    n=N;

 	M = ( double * ) malloc ( n * n * sizeof ( double ) );
    MT = ( double * ) malloc ( n * n * sizeof ( double ) );
	srand(time(NULL));

    int v1=rand()%(n);
    int v2=rand()%n;
    

    int k=0;
    int x;
	for (k = 1; k < 13; k++){
        printf("\n\n**__** # HILOS = %d \n", k);
        for (x = 1; x < 4; x++){
        difP=0;
        difS=0;
        difS+=generateMatrixSerial(M,n);
        M[v1*n+v2] = 1;
        difS+=transposeSerial(M,MT,n);
        checkSymmetrySerial(M, MT, n,difS);

        difP+=generateMatrixParallel(M,n,k);
        M[v1*n+v2] = 1;
        difP+=transposeParallel(M,MT,n,k);
        checkSymmetryParallel(M, MT, n,k,difP);
        }
	}
}



