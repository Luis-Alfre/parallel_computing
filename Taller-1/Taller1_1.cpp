#include <omp.h>
#include <stdio.h>
#include<stdlib.h> 
#include <time.h>

#define DIM1 20000
#define DIM2 20000

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
 // Calcula el producto de una matriz M de tama�o f x c por un vector V de tama�o c.
 // Cada elemento de uuna fila de la matriz debe ser multiplicada por cada
 // elemento del vector y luego realizar la suma de los productos. El resultado debe
 // ser almacenado en un vector VR de tama�o f.
 void productoMatrizVectorSerial(double M[], double V[], double VR[], int f, int c)
 {
	double t1 = omp_get_wtime(); // tiempo iniciO
 	int i,j;
 	double suma;
 	for (i=0;i<f;i++)
 		{
 			suma = 0;
			for (j=0;j<c;j++)	
				suma=suma + M[i*c+j]*V[j];
			VR[i]=suma;	
		 }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double dif1 = t2-t1;  // diferencia
    printf ("Suma de elementos secuencial = tiempo %f \n",dif1);	

 }

  void productoMatrizVectorPararell(double M[], double V[], double VR[], int f, int c,int k)
 {
	double t1 = omp_get_wtime(); // tiempo iniciO
 	int i,j;
 	double suma;
	#pragma omp parallel for private(i,j) shared(VR, M) reduction(+:suma) num_threads(k)
 	for (i=0;i<f;i++)
 		{
 			suma = 0;
			for (j=0;j<c;j++)	
				suma=suma + M[i*c+j]*V[j];
			VR[i]=suma;	
		 }
	double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double dif1 = t2-t1;  // diferencia
    printf ("Suma de elementos pararell = tiempo %f \n",dif1);
 }
 
 int main()
 {
 	// declaramos punteros a la matriz y arreglos que vamos a utilizar
 	double *M, *V, *VR;
 	double t1, t2, difP, difS;
 	// declaramos el tama�o en filas y columnas
 	int f = DIM1, c = DIM2;
 	// Reservamos dinamicamente la memoria.
 	M = ( double * ) malloc ( f * c * sizeof ( double ) );
	V = ( double * ) malloc ( c * sizeof ( double ) );
	VR = ( double * ) malloc ( f * sizeof ( double ) );

	// semilla para generaci�n de n�meros aleatorios
	srand(time(NULL));
	// se genera y muestra el vector a multiplicar
	generarVector(V,c,10);
	//printf("\n El vector: \n");
	//imprimirVector(V,c);
	// se genera y muestra la matriz a multiplicar
	generarMatriz(M,f,c,10);
	//printf("\n La Matriz \n");
	//imprimirMatriz(M,f,c);
	// Se realiza el producto serial

	int j=0;
	for (j = 1; j < 13; j++)
	{
		printf("\n\nNUMERO DE PROCESADORES = %d \n", j);
		productoMatrizVectorSerial(M,V,VR,f,c);
		productoMatrizVectorPararell(M,V,VR,f,c,j);
	}

	
	//printf("\n El vector resultado: \n");
	//imprimirVector(VR,f);

 }
