# include <stdio.h>
# include <stdlib.h>
# include <omp.h>
# include <cmath>
# define N 10000

void narciSerial(int n){
    double t1 = omp_get_wtime(); // tiempo inicio
    int i, rta=0;
    for ( i = 1; i < n; i++){

        int cn = 0, c = 0, ult = 0, j, sum = 0;

        cn = i;
        //cuenta cuantas cifras tiene el numero
        while(cn>0){
            cn/=10;
            c++;
        }
        cn = i;

        for (j = 0; j < c; j++){
            ult = cn %10;
            sum = sum + pow(1.0*ult,c);
            cn /= 10;
        }
    if (sum == i)
        rta+=i;
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double difP= t2-t1;  // diferencia
    printf (" \n - **_**Suma de elementos secuencial = %ld - tiempo = %f \n",rta,difP);
}


void narciParallel(int n, int k){
    double t1 = omp_get_wtime(); // tiempo inicio
    int i, rta=0;
    #pragma omp parallel for private(i) schedule(static,3) shared(n)  reduction(+:rta) num_threads(k)
    for ( i = 1; i < n; i++){

        int cn = 0, c = 0, ult = 0, j, sum = 0;

        cn = i;
        //cuenta cuantas cifras tiene el numero
        while(cn>0){
            cn/=10;
            c++;
        }
        cn = i;

        for (j = 0; j < c; j++){
            ult = cn %10;
            sum = sum + pow(1.0*ult,c);
            cn /= 10;
        }
    if (sum == i)
        rta+=i;
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double difP= t2-t1;  // diferencia
    printf (" - Suma de elementos pararell = %ld - tiempo = %f \n",rta,difP);

}

int main()
{
    int n=N;

    int j=0;
    int x=0;
	for (j = 1; j < 11; j++){
        printf("\n\n**__** # PROCESADORES = %d \n", j);
        for (x = 1; x < 6; x++){
        narciSerial(n);
        narciParallel(n,j);
	    }
   
    }
}