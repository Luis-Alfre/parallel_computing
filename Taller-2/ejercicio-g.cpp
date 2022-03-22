#include <omp.h>
#include <stdio.h>
#include<stdlib.h> 
#include <time.h>

#define DIM1 500
#define DIM2 500

// Permite imprimir un vector de tama�o n, dado como par�metro en V
void imprimirVector(double V[], int n)
{
	for (int i=0;i<n;i++)
		printf(" %.2f \t",V[i]);
	printf("\n");
}
// Permite imprimir una matriz de tama�o n, dado como par�metro en M
void imprimirMatriz(double M[], int f, int c)
{
	for (int i=0;i<f;i++)
	{
		for(int j=0;j<c;j++)
			printf(" %.2f \t",M[i*c+j]);
		printf("\n");
	}
		
}
// Genera un vector aleatorio de tama�o n
void generarVector(double V[], int n, int max)
{
	for (int i=0;i<n;i++)
		V[i]= rand() % max;  // max = 4
		
}
// Genera una matriz aleatoria de tama�o f x c 
void generarMatriz(double M[], int f, int c, int max)
{
	int k=1;
	for (int i=0;i<f;i++)
		{
			for (int j=0;j<c;j++)
				{
					M[i*c+j]= k++;  // M[i][j] = k++
					//M[i*c+j]= rand();
				}
		
		}
 }

 void productoMatrizSerial(double M[],  double M2[],double M3[], int f, int c, int k){

    double t1 = omp_get_wtime(); // tiempo iniciO 
    double sum;
	for (int h=0;h<c;h++){
		for (int i=0;i<f;i++){
        sum=0;
			for (int j=0;j<c;j++){
                    sum+=M[i*c+j]*M2[h*c+j];
				}
        M3[i*f+h]=sum;
		}
	}
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double dif1 = t2-t1;  // diferencia
    printf ("\n - **_**Suma de elementos secuencial = %ld - tiempo = %f \n",0,dif1);	

 }


 void productoMatrizParallel(double M[], double M2[],double M3[], int f, int c, int k){

    double t1 = omp_get_wtime(); // tiempo iniciO 
    double sum;
	int h,i,j;
	#pragma omp parallel for private(i,j,h) shared(M, M2, M3) reduction(+:sum) num_threads(k)
	for ( h=0;h<c;h++){
		for ( i=0;i<f;i++){
        sum=0;
			for ( j=0;j<c;j++){
                    sum+=M[i*c+j]*M2[h*c+j];
				}
        M3[i*f+h]=sum;
		}
	}
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double dif2 = t2-t1;  // diferencia
    printf (" - Suma de elementos pararell = %ld - tiempo = %f \n",0,dif2);

 }
 int main()
 {
 	// declaramos punteros a la matriz y arreglos que vamos a utilizar
 	double *M, *V, *VR, *M2, *M3;
 	double t1, t2, difP, difS;
 	// declaramos el tama�o en filas y columnas
 	int f = DIM1, c = DIM2;
 	// Reservamos dinamicamente la memoria.
 	M = ( double * ) malloc ( f * c * sizeof ( double ) );
	M2 = ( double * ) malloc ( f * c * sizeof ( double ) );
	M3 = ( double * ) malloc ( c * f * sizeof ( double ) );
	V = ( double * ) malloc ( c * sizeof ( double ) );
	VR = ( double * ) malloc ( f * sizeof ( double ) );

	// semilla para generaci�n de n�meros aleatorios
	srand(time(NULL));
	// se genera y muestra el vector a multiplicar
	//generarVector(V,c,10);
	//printf("\n El vector: \n");
	//imprimirVector(V,c);
	// se genera y muestra la matriz a multiplicar
	generarMatriz(M,f,c,10);
	generarMatriz(M2,f,c,10);

	//printf("\n La Matriz \n");
	//imprimirMatriz(M,f,c);
	//imprimirMatriz(M2,f,c);
	// Se realiza el producto serial

	int j=0;
    int x=0;
	for (j = 1; j < 11; j++){
        printf("\n\n**__** # PROCESADORES = %d \n", j);
        for (x = 1; x < 4; x++){
        productoMatrizSerial(M,M2,M3,f,c,j);
		productoMatrizParallel(M,M2,M3,f,c,j); 
    }
	}
	//printf("\n El vector resultado: \n");
	//imprimirVector(V,f);

 }
