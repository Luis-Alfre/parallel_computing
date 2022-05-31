
//Escriba un programa en C para encontrar números primos circulares hasta un límite específico.
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include <omp.h>
# define N 100


// varible global
// Mediante threadprivate se otorga a la variable la facultad de ser
// privada durante todas las regiones paralelas subsiguientes.
// De esta forma, las variables threadprivate no desaparecen al
// finalizar la región paralela
int flg;
#pragma omp threadprivate (flg)

//funcion que clacula si un numero es primo o no
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

//funcion que realiza la permutación de el numero dado como parametro
//cada permutacion la va evalucion en funcion que verifica si es primo o no
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

// funcion que calcula los numeros circulares
// recibe como parametro el limite superior
// va iteranto un ciclo for hasta que llegue al limite superior
// va invocando a la funcion combinacion
// si el numero es circular lo imprime
// se toman los tiempos de ejecución
// se paraleliza el ciclo for con directivas de omp
void circularNumbersParallel(int ctr){
  double t1 = omp_get_wtime(); // tiempo inicio
  int t;
  #pragma omp parallel for private(t), shared(ctr)  
    for (t = 2; t <= ctr; t++)
    {
      flg=0;
      AllCombination(t);

      if(flg==0){
          printf("%li ",t);
      }
    }
  double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double difP= t2-t1;  // diferencia
  printf ("\n - Suma de elementos pararell = %ld - tiempo = %f \n",0,difP); 
}

int main()
{
    //Se declaran las variables a utilizar
    long int ctr;
    printf("\n\n Find Circular Prime Numbers upto a specific limit: \n");
    printf(" ---------------------------------------------------\n");
    //Se registra el lmite superior
    printf(" Enter the upper Limit: ");
    scanf("%li",&ctr);
    printf("\n The Circular Prime Numbers less than %li are: \n",ctr);
    // se llamda a la funcion que calcula los numeros circulares
    // recibe como parametro el limite superior
    circularNumbersParallel(ctr);
	printf("\n");
}
