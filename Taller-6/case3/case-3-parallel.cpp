# include <stdio.h>
# include <stdlib.h>
# include <omp.h>
# include <cmath>
# define N 10000

void narciParallel(int n){

    int i;
    #pragma omp parallel for private(i) shared(n)  num_threads(8)
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
        printf(" %d ", i);
    }

}

int main()
{
    int n=N;
    narciParallel(n);
  

}