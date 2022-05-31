# include <stdio.h>
# include <stdlib.h>
# include <omp.h>
# define N 1000


void pronicNumberSerial(){
   double t1 = omp_get_wtime(); // tiempo inicio
   int prno;
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
            printf("%d ",prno);
        }
       
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double difP= t2-t1;  // diferencia
    printf ("\n - Suma de elementos serial = %ld - tiempo = %f \n",0,difP);
}

int main()
{
	printf("\n\n Find the Pronic Numbers between 1 to 1000: \n");
	printf(" -----------------------------------------------\n");
	printf(" The Pronic numbers are: \n");
    pronicNumberSerial();
    printf("\n");
}
