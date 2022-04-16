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

//------------------------------ VERSION SERIAL -------------------------- 

//Genera una matriz con datos aleatorios (1 al 50 ) de mañanano n 
double generateMatrixSerial(double M[], int n){
    double t1 = omp_get_wtime(); // tiempo iniciO
    int c,d;
    for(c = 0; c < n; c++){
        for(d = 0; d < n; d++){
            M[c*n+d]= rand()%51;
        }
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
    return (t2-t1);  // diferencia
}

//Se crea la matriz transpuesta apartir de la matriz original intercambiando filas por columnas
double transposeSerial(double M[], double MT[], int n){
   	double t1 = omp_get_wtime(); // tiempo iniciO
    int c,d;
    for(c = 0; c < n; c++){ 
        for(d = 0; d < n; d++){ 
            MT[d*n+c] = M[c*n+d];
        }
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
	return (t2-t1);  // diferencia
}


//Se verifica si la matriz es simetrica o no comparando posición por posicion con la matriz transpuesta 
void checkSymmetrySerial(double M[],double MT[], int n, double difS){
    double t1 = omp_get_wtime(); // tiempo inicio
    int c,d;
    int x = 1;
    for(c = 0; c < n; c++){
        if(x == 0) continue;
        for(d = 0; d < n; d++){
            if(x == 0) continue;
             if(M[c*n+d] != MT[c*n+d] ){
                 x=0;
             }
         }
    }
    double t2 = omp_get_wtime(); // tiempo finzalizaci�n
    difS= (t2-t1);
    printf ("\n - **_**Suma de elementos secuencial = %ld - tiempo = %f \n",x,difS);
}


int main(){

    //Creo los punteros
    double *M, *MT;
    // variable para almacenar la dimensión
    int n;
    //variable para tomar el timepo de ejecución 
    double difS;

    //se lee la dimesión por teclado
    printf("\nEnter the dimension of the matrix: \n\n");
    scanf("%d", &n);

    //se crean las matrices con punteros 
 	M = ( double * ) malloc ( n * n * sizeof ( double ) );
    MT = ( double * ) malloc ( n * n * sizeof ( double ) );

    //se incia la semilla 
	srand(time(NULL));

    //se inicializa el timepo en 0
    difS=0;
    //Se genera la matriz con datos aleatorios
    difS+=generateMatrixSerial(M,n);
    //se crea la matriz transpuesta
    difS+=transposeSerial(M,MT,n);
    //se verifica que la matriz es simetrica o no
    checkSymmetrySerial(M, MT, n,difS);

}