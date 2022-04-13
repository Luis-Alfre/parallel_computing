#include <omp.h>
#include <stdio.h>
#include<stdlib.h> 
#include <time.h>
#define N 100000000
 
int main (){
  int thread;
  static long int A[N];
  int nthreads;
  long int sum,sumP;
  int i;

  int j=0;
  for (j = 1; j < 13; j++)
  {

    printf("\n\nNUMERO DE PROCESADORES = %d \n", j);
    // Semilla numeros aleatorios
    srand(time(NULL)); 
    for (i = 0; i<N;i++)
    A[i] =  rand() % 21;
	
    // Medir los tiempos en la suma serial	
    double t1 = omp_get_wtime(); // tiempo iniciO
      sum = 0;	
      for (i = 0; i<N; i++)
        sum+= A[i];
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
    double dif1 = t2-t1;  // diferencia
    printf ("Suma de elementos secuencial = %ld - tiempo %f \n",sum,dif1);	
    sumP = 0;


// Medir los tiempos en la suma paralela
 t1 = omp_get_wtime();
#pragma omp parallel for private(i) shared(A) reduction(+:sumP) num_threads(j)
	for (i = 0; i<N; i++)
  		sumP+= A[i];
 t2 = omp_get_wtime();
double dif2 = t2-t1;
printf ("Suma paralela de elementos  = %ld - tiempo %f \n",sumP,dif2);
printf ("Rendimiento = %f",100- (dif2/dif1)*100);



  }



}
