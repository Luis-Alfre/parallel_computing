/*
Universidad de Pamplona
Facultad de Ingenier�as y Arquitectura
Programa de Ingenier�a de Sistemas
Fundamentos de Computaci�n paralela y distribuida

*/

#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<math.h>
#include <omp.h>

void imprimirMatrizEnteros(int M[], int f, int c)
{
	for (int i=0;i<f;i++)
	{
		for(int j=0;j<c;j++)
			printf(" %d \t",M[i*c+j]);
		printf("\n");
	}
		
}

void copiarMatrizEnteros(int M[], int N[], int f, int c)
{
	for (int i=0;i<f;i++)
	{
		for(int j=0;j<c;j++)
			M[i*c+j] = N[i*c+j];
	}
		
}

// Varables globales

	

main()
{
	// Para inlcuir caracteres espa�o�
	//setlocale(LC_ALL, "spanish"); // en Linux
    SetConsoleCP(1252); 
    SetConsoleOutputCP(1252);
	// Semilla para n�meros aleatorios
	srand(time(NULL)); 
	
	int *matrizInicial;
	int *matrizCopia1, *matrizCopia2;
	int ciclos= 3, fil= 2, col = 3, thread;
	
	matrizInicial = (int *) malloc (fil*col*sizeof(int));
	matrizCopia1  = (int *) malloc (fil*col*sizeof(int));
	matrizCopia2  = (int *) malloc (fil*col*sizeof(int));
		
	for(int i=0;i<fil;i++)
	{
		for(int j=0;j<col;j++)
		matrizInicial[col*i+j] = 0;
	}
	
	imprimirMatrizEnteros(matrizInicial,fil,col);

	copiarMatrizEnteros(matrizCopia1,matrizInicial,fil,col);
	printf(" \n Copia 1: \n");
	imprimirMatrizEnteros(matrizCopia1,fil,col);
	copiarMatrizEnteros(matrizCopia2,matrizInicial,fil,col);
	printf(" \n Copia 2: \n");
	imprimirMatrizEnteros(matrizCopia2,fil,col);
	for(int i=0;i<ciclos;i++)//migraciones
	{
		#pragma omp parallel num_threads(2)
		#pragma omp sections  
		{
			# pragma omp section
			{
				int thread = omp_get_thread_num();
				printf( "\n desde el hilo %d, iterac�on %d \n",thread, i);
				for(int j=0;j<fil;j++)
				{
					for(int k=0;k<col;k++)
						matrizCopia1[col*j+k] += 1 ;
				}
			}
			# pragma omp section
			{
				int thread = omp_get_thread_num();
				printf( "\n desde el hilo %d, iterac�on %d \n",thread, i);
				for(int j=0;j<fil;j++)
				{
					for(int k=0;k<col;k++)
						matrizCopia2[col*j+k] += 2 ;
				}
			}

		}
		// Aqui deber�a incluir el intercambio de elementos
    

	}
	printf(" \n Copia 1: \n");
	imprimirMatrizEnteros(matrizCopia1,fil,col);
	printf(" \n Copia 2: \n");
	imprimirMatrizEnteros(matrizCopia2,fil,col);
	

	
}
