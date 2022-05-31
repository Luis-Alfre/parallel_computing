
//Escriba un programa en C para encontrar números primos circulares hasta un límite específico.
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>

void imprimirVector(long int V[], int n)
{
	for (int i=0;i<n;i++)
		printf("%d ->",V[i]);
	printf("\n");
}

//varible de tipo global
int flg;
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

int main()
{
  //Se declaran las variables a utilizar
    long int i=2,ctr;
	
    printf("\n\n Find Circular Prime Numbers upto a specific limit: \n");
    printf(" ---------------------------------------------------\n");
    //Se registra el lmite superior
    printf(" Enter the upper Limit: ");
    scanf("%li",&ctr);
    printf("\n The Circular Prime Numbers less than %li are: \n",ctr);
    
    //ciclo que va iteranto hasta el limite superior y va 
    //llamando dentro de ella la funcion de combinacion
    //e imprime los numero circulares
    while(i<=ctr)
    {
      flg=0;
      AllCombination(i);
      if(flg==0)
      {
          printf("%li ",i);
      }
      i++;
    }
	printf("\n");
}
