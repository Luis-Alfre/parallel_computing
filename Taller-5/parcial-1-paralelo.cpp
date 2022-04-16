#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <omp.h>

#define N 1000

//impirime una matriz
void imprimirMatriz(double M[], int f, int c)
{
	for (int i=0;i<f;i++)
	{
		for(int j=0;j<c;j++)
			printf(" %.2f \t",M[i*c+j]);
		printf("\n");
	}
		
}

//------------------------------ VERSION PARALELIZADA -------------------------- 


//Genera una matriz con datos aleatorios (1 al 50 ) de mañanano n 
double generateMatrixParallel(double M[], int n, int k){
    double t1 = omp_get_wtime(); // tiempo inicio
    int c,d;
   	#pragma omp parallel for private(c,d) shared(M)  num_threads(k)
    for(c = 0; c < n; c++){
        for(d = 0; d < n; d++){
            M[c*n+d]= rand()%51;
        }
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	return (t2-t1);  // diferencia
 }



//Se crea la matriz transpuesta apartir de la matriz original intercambiando filas por columnas
double transposeParallel(double M[], double MT[], int n, int k){
    double t1 = omp_get_wtime(); // tiempo inicio
    int c,d;
    #pragma omp parallel for private(c,d) shared(M, MT)  num_threads(k)
    for(c = 0; c < n; c++){ 
        for(d = 0; d < n; d++){ 
            MT[d*n+c] = M[c*n+d];
        }
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	return (t2-t1);  // diferencia
}





//Se verifica si la matriz es simetrica o no comparando posición por posicion con la matriz transpuesta 
void checkSymmetryParallel(double M[],double MT[], int n, int k, double difP){
    double t1 = omp_get_wtime(); // tiempo inicio
    int c,d;
    int x = 1;
    #pragma omp parallel for private(c,d) shared(M, MT,x)  num_threads(k)    
    for(c = 0; c < n ; c++){
        if(x == 0) continue;
        for(d = 0; d < n; d++){
            if(x == 0) continue;
             if(M[c*n+d] != MT[c*n+d] ){
                 x=0;
             }
         }
        
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	difP= t2-t1;  // diferencia
    printf (" - Suma de elementos pararell = %ld - tiempo = %f \n",0,difP);
}

int main(){

    //Creo los punteros
    double *M, *MT;
    // variable para almacenar la dimensión
    int n;
    //variable para tomar el timepo de ejecución
    double difP;


    //se lee la dimesión por teclado
    printf("\nEnter the dimension of the matrix: \n\n");
    scanf("%d", &n);


    //se crean las matrices con punteros 
 	M = ( double * ) malloc ( n * n * sizeof ( double ) );
    MT = ( double * ) malloc ( n * n * sizeof ( double ) );

    //se incia la semilla 
	srand(time(NULL));

    //Se realizan dos bucles for el primero de ellos
    //para ir iterando los hilos en la ejecución 
    //y el segundo hace la repetición 4 veces para cada hilo
    int k=0;
    int x;
	for (k = 1; k < 13; k++){
        printf("\n\n**__** # HILOS = %d \n", k);
        for (x = 1; x < 4; x++){
        //se inicializa el timepo en 0
        difP=0;
        //Se genera la matriz con datos aleatorios
        difP+=generateMatrixParallel(M,n,k);
        //se crea la matriz transpuesta
        difP+=transposeParallel(M,MT,n,k);
        //se verifica que la matriz es simetrica o no
        checkSymmetryParallel(M, MT, n,k,difP);
        }
	}



}