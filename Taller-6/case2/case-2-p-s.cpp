# include <stdio.h>
# include <stdlib.h>
# include <omp.h>
# define N 10000


void pronicNumberSerial(){
   double t1 = omp_get_wtime(); // tiempo inicio
   int prno, sum=0;
    for(prno=1;prno<=N;prno++){
        int flg=0;
        int i=1;
        while(i<=prno){
            if(i*(i+1)==prno){
                    flg=1;
                    i=prno;
                }
            i++;
        }
        if(flg==1){
            sum+=prno;
        }
       
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double difP= t2-t1;  // diferencia
    printf (" \n - **_**Suma de elementos secuencial = %ld - tiempo = %f \n",sum,difP);
}

void pronicNumberParallel(int k){
   double t1 = omp_get_wtime(); // tiempo inicio
   int prno, sum=0;
    #pragma omp parallel for schedule(static,3) private(prno) reduction(+:sum) num_threads(k)
    for(prno=1;prno<=N;prno++){
        int flg=0;
        int i=1;
        //#pragma omp parallel private(prno) reduction(+:sum) num_threads(k)
        while(i<=prno){
            if(i*(i+1)==prno){
                    flg=1;
                    i=prno;
                }
            i++;
        }
        if(flg==1){
            sum+=prno;
        }
       
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double difP= t2-t1;  // diferencia
    printf (" - Suma de elementos pararell = %ld - tiempo = %f \n",sum,difP);
}


int main()
{
	int j=0;
    int x=0;
	for (j = 1; j < 11; j++){
        printf("\n\n**__** # PROCESADORES = %d \n", j);
        for (x = 1; x < 6; x++){
        pronicNumberSerial();
        pronicNumberParallel(j);
	}
   
}
}
