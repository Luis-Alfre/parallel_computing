
//Escriba un programa en C para encontrar números primos circulares hasta un límite específico.
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include <omp.h>
# define N 10000

int flg;
#pragma omp threadprivate (flg)

void chkPrime(long int n)
{
    long int i;
    i=n-1;
    while(i>=2)
    {
        if(n%i==0)
        {
            flg=1;
        }
        i--;
    }
}

void AllCombination(long int a)
{
  long int b,c,d,e,i,j,k,s,z,v,x[8],y[8],m;
  b=a;
  i=0;
  while(b>0)
  {
      y[i]=b % 10;
      b=b/10;
      i++;
  }

  c=0;
  for(j=i-1;j>=0;j--)
  {
    x[c]=y[j];
    c++;
  }
  m=i;
  while(m>0)
  {
     c=m-1;
     d=i-1;
     e=0;
     s=0;
     while(e<i)
     {
       z=pow(10,d);
       v=z*x[c%i];
       c++;
       d--;
       e++;
       s=s+v;
     }
     m--;
     chkPrime(s);
  }
}

void circularNumbersSerial(int ctr){
  double t1 = omp_get_wtime(); // tiempo inicio
  int t, sum=0;
    for (t = 2; t <= ctr; t++)
    {
      flg=0;
      AllCombination(t);

      if(flg==0){
          sum+=t;
      }
    }
    
  double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double difP= t2-t1;  // diferencia
  printf (" \n - **_**Suma de elementos serial = %ld - tiempo = %f \n",sum,difP); 
}


void circularNumbersParallel(int ctr, int k){
  double t1 = omp_get_wtime(); // tiempo inicio
  int t, sum=0;
  #pragma omp parallel for private(t), schedule(static,3) shared(ctr), reduction(+:sum)  num_threads(k)
    for (t = 2; t <= ctr; t++)
    {
      flg=0;
      AllCombination(t);

      if(flg==0){
          sum+=t;
      }
    }
  double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double difP= t2-t1;  // diferencia
  printf ("- Suma de elementos pararell = %ld - tiempo = %f \n",sum,difP); 
}

int main()
{
    long int ctr=N;
	  int j=0;
    int x=0;
	  for (j = 1; j < 11; j++){
        printf("\n\n**__** # PROCESADORES = %d \n", j);
        for (x = 1; x < 6; x++){
        circularNumbersSerial(ctr);
        circularNumbersParallel(ctr, j);
	}
}
}


