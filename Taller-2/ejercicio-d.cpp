#include <omp.h>
#include <stdio.h>
#include<stdlib.h> 
#include <time.h>

#define DIM1 10000
#define DIM2 10000

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
					//M[i*c+j]= k++;  // M[i][j] = k++
					M[i*c+j]= rand();
				}
		
		}
 }

 void mayorRepetidoMatrizSerial(double M[], double V[], double VR[], int f, int c, int k){

    double t1 = omp_get_wtime(); // tiempo iniciO 
    int may, rep;
	for (int i=0;i<f;i++){
        may=M[i*c];
		rep=0;
			for (int j=0;j<c;j++){
                if(M[i*c+j]>may){
                    may=M[i*c+j];
					rep=0;
                }
				if(may == M[i*c+j]){
					rep++;
				}
				}
        V[i]=may;
		VR[i]=rep;
		}
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double dif1 = t2-t1;  // diferencia
    printf ("\n - **_**Suma de elementos secuencial = %ld - tiempo = %f \n",0,dif1);	

 }


 void mayorRepetidoMatrizParallel(double M[], double V[], double VR[], int f, int c, int k){
    
    double t1 = omp_get_wtime(); // tiempo iniciO 
    int may, rep, i, j;
	#pragma omp parallel for private(i,j) shared(M, V, VR)  num_threads(k)
	for (i=0;i<f;i++){
        may=M[i*c];
		rep=0;
			for (j=0;j<c;j++){
                if(M[i*c+j]>may){
                    may=M[i*c+j];
					rep=0;
                }
				if(may == M[i*c+j]){
					rep++;
				}
				}
        V[i]=may;
		VR[i]=rep;
		}
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	double dif2 = t2-t1;  // diferencia
    printf (" - Suma de elementos pararell = %ld - tiempo = %f \n",0,dif2);

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
	//generarVector(V,c,10);
	//printf("\n El vector: \n");
	//imprimirVector(V,c);
	// se genera y muestra la matriz a multiplicar
	generarMatriz(M,f,c,10);
	//printf("\n La Matriz \n");
	//imprimirMatriz(M,f,c);
	// Se realiza el producto serial

	int j=0;
    int x=0;
	for (j = 1; j < 11; j++){
        printf("\n\n**__** # PROCESADORES = %d \n", j);
        for (x = 1; x < 4; x++){
        mayorRepetidoMatrizSerial(M,V,VR,f,c,j);
		mayorRepetidoMatrizParallel(M,V,VR,f,c,j);
    }
	}
	//printf("\n El vector resultado: \n");


 }
