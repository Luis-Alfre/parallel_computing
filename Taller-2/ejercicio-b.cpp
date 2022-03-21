#include <omp.h>
#include <stdio.h>
#include<stdlib.h> 
#include <time.h>

#define N 10000



int sumaNprimerosSerial(int n){

    double t1 = omp_get_wtime(); // tiempo iniciO 
    int sum=0;
	for (int i=0;i<=n;i++){
			sum+=i;
		}
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double dif1 = t2-t1;  // diferencia
    printf ("\n - **_**Suma de elementos secuencial = %ld - tiempo = %f \n",sum,dif1);	
	return sum;
 }


 int sumaNprimerosSerialParallel(int n, int k){
    
    double t1 = omp_get_wtime(); // tiempo inicio
    int sum=0;
	int i;
    #pragma omp parallel for private(i)  shared(n) reduction(+:sum) num_threads(k)
	for (i=0;i<=n;i++){
        sum+=i;
	}
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double dif2 = t2-t1;  // diferencia
    printf (" - Suma de elementos pararell = %ld - tiempo = %f \n",sum,dif2);
	return sum;
 }

 int main(){

 	int num=N;

	int j=0;
    int x=0;
	for (j = 1; j < 11; j++){
        printf("\n\n**__** # PROCESADORES = %d \n", j);
        for (x = 1; x < 6; x++){
        sumaNprimerosSerial(num);
		sumaNprimerosSerialParallel(num,j);
    }
	}
	//imprimirVector(V,f);

 }
