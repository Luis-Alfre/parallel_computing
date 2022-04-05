/*
Universidad de Pamplona
Facultad de Ingenier�as y Arquitectura
Programa de Ingenier�a de Sistemas
Fundamentos de Computaci�n paralela y distribuida

Implementaci�n de una eaquema de soluci�n al problema del TSP mediante un Algoritmo Gen�tico Simple
Version Serial y paralela
*/
#include<stdio.h>
#include <cstdlib>
//#include<windows.h>
#include<time.h>
//#include<utilidades.h>
#include<locale.h>
#include<math.h>

void imprimirVector(int V[], int n)
{
	for (int i=0;i<n;i++)
		printf(" %d \t",V[i]);
	printf("\n");
}

void imprimirMatriz(float M[], int f, int c)
{
	for (int i=0;i<f;i++)
	{
		for(int j=0;j<c;j++)
			printf(" %.2f \t",M[i*c+j]);
		printf("\n");
	}
		
}


/*
Funci�n que genera dos vectores con las coordenadas en X y Y, correspondientes a un caso particular del problema del TSP. 
Las coordenadas corresponden a valores enteros.

Par�metros de entrada:

nCiudades: N�mero de ciudades para el problema del transporte.
xMin, xMax : Valores m�nimo y m�ximo para las coordenadas en X.
yMin, yMax : Valores m�nimo y m�ximo para las coordenadas en Y.

Salida de la funci�n:

coordenadasX: Vector con los valores de las coordenadas en X.
coordenadasY: Vector con los valores de las coordenadas en Y.

*/

void generarCoordenadas(int coordenadasX[], int coordenadasY[], int nCiudades, int xMin, int xMax, int yMin, int yMax)
{
	for (int i=0;i<nCiudades;i++)
		{
			int rango = xMax - xMin + 1;
			coordenadasX[i] = rand()%rango + xMin;
			rango = yMax - yMin +  1;
			coordenadasY[i] = rand()%rango + yMin;
		}
	
}

/*
Funci�n que genera la matriz de adyacencias del grafo determinado por los nodos 
cuyas coordenadas vienen dados en los vectores coordenadasX y coordenadasY. E
sta matriz contiene las distancias entre los nodos del grafo determinado por tales coordenadas.

Par�metros de entrada:

nCiudades: N�mero de ciudades para el problema del transporte.
coordenadasX: Vector con los valores de las coordenadas en X.
coordenadasY: Vector con los valores de las coordenadas en Y.

Salida de la funci�n:

matrizDistancia: Matriz que contiene las distancias entre las ciudades dadas por las coordenada de entradas. Es una matriz cuadrada, sim�trica y de diagonal 0.

*/
void crearMatrizDistancia(float matrizDistancias[], int coordenadasX[], int coordenadasY[], int nCiudades)
{
	int i,j;
	float dist;
	for (i=0;i<nCiudades;i++){
		dist=0;
		for (j=0; j< nCiudades; j++){
			dist= sqrt(pow((coordenadasX[i]-coordenadasX[j]),2)+pow((coordenadasY[i]-coordenadasY[j]),2));
			matrizDistancias[i * nCiudades + j] = dist;
		}
	}
	
}

/*
Funci�n que retorna la longitud del camino dado como par�metro de entrada 
que es igual a la distancia recorrida al iniciar  en la ciudad camino(i) y regresar al mismo lugar. 

Par�metros de entrada:

camino: vector que muestra el orden de las ciudades recorridas en un circuito
matrizDistancia: requerido para obtener la distancia entre ciudades.

Salida de la funci�n

Longitud: valor real que corresponde a la longitud del recorrido correspondiente al circuito dado.

*/
float longitudCircuito(int camino[], float matrizDistancia[], int nCiudades)
{
	double sum = 0;
	for (int i = 0; i < nCiudades - 1; i++)
	{
		double distancia = matrizDistancia[(camino[i] - 1) * nCiudades + camino[i + 1] - 1];
		sum += distancia;
	}
	sum += matrizDistancia[(camino[nCiudades - 1] - 1) * nCiudades + camino[0] - 1];
	return (sum);
		
}

/*

Funci�n que genera una permutaci�n con el recorrido por n ciudades, sin repetir ciudad. 

Par�metros de entrada: 

nCiudades: n�mero de ciudades bajo an�lisis.

Salida de la funci�n:

camino:  vector de enteros consistente en una permutaci�n de n ciudades entre 1 y n.

*/
void generaIndividuo(int camino[], int nCiudades)
{
	int exist, i, j, ciudad;
	for (i = 0; i < nCiudades; i++)
	{
		exist = 1;
		while (exist == 1)
		{
			exist = 0;
			ciudad = rand() % nCiudades + 1;
			j = 0;
			while ((j < i) && (exist == 0))
			{
				if (camino[j] == ciudad){
					exist = 1;
				}
					
				j++;
			}
			if (exist == 0){
				camino[i] = ciudad;
			}
				
		}
	}
	
}

float longitudMaxima(float matrizDistancia[], int nCiudades){
	double mayor = -1;
	int i,j;
	for (i = 0; i < nCiudades; i++)
		for (j = 0; j < nCiudades; j++)
			if (matrizDistancia[i * nCiudades + j] > mayor)
				mayor = matrizDistancia[i * nCiudades + j];
	return (mayor);
}

/*
Funci�n que retorna el valor de la adaptaci�n, que permitir� determinar quien es el individuo mejor adaptado. 
Se calcula como la longitud m�xima del recorrido en la poblaci�n actual 
menos el valor de la longitud del individuo bajo an�lisis: 

adaptaci�n = longMaxima+1 - longitud

Par�metros de entrada:

longutid: Valor de la longitud del individuo
longMaxima: Distancia de mayor recorrido entre todos los individuos de la poblaci�n

Salida de la funci�n 

aptitud: Valor que correponde a la adaptaci�n al problema del transporte. Es un entero positivo, a mayor valor, mejor adaptaci�n
*/

float adaptacion(float longitud, float longitudMaxima)
{
	float aptitud = (longitudMaxima + 1 ) - longitud;
	return aptitud;
}

/*
Funci�n que genera la poblaci�n inicial en un arreglo bidimensional (matriz). 
El n�mero de filas corresponde al tama�o de la poblaci�n. 
Cada fila es una soluci�n posible al problema del TSP,  
siendo una combinaci�n que indica el orden del recorrido sobre n ciudades, 
es decir un vector num�rico con n valores aleatorios entre 1 y n, 
sin que se repita ninguna valor. 
Adem�s de la poblaci�n se deben generar dos vectores adicionales, 
el uno corresponde a la distancia del recorrido de cada individuo 
y el otro a su aptitud, obtenido de la funci�n de adaptaci�n.  

Par�metros de entrada:

tamPob: entero que indica el tama�o de la poblaci�n a generar
nCiudades: entero que indica el n�mero de ciudades correspondiente al problema.
matrizDistancia: matriz de adyacencias correspondiente a las distancias entre las ciudades a modelar.

Salida de la funci�n

poblacion: matriz con los individuos que conforman la poblaci�n.
distancias: arreglo unidimensional (vector) correspondiente a la distancia que corresponde 
            al recorrido de cada individuo en la poblaci�n
aptitud: vector correspondiente a la aptitud o adaptaci�n de cada individuo.
*/

void poblacionInicial(int poblacion[], float distancias[], float aptitud[], int tamPoblacion, int nCiudades, float matrizDistancia[])
{
	int i, j, camino[nCiudades];
	double longitud, longMaxima = -1;
	for (i = 0; i < tamPoblacion; i++){
		generaIndividuo(camino, nCiudades);
		distancias[i] = longitudCircuito(camino, matrizDistancia, nCiudades);
		if (distancias[i] > longMaxima){
			longMaxima = distancias[i];
		}
		for (j = 0; j < nCiudades; j++){
			poblacion[i * nCiudades + j] = camino[j];
		}
	}
	for (i = 0; i < tamPoblacion; i++){
		aptitud[i] = adaptacion(distancias[i], longMaxima);
	}
}


int existe(int camino[],int ciudad,int nCiudades){
	int i;

	for (i=0; i<nCiudades; i++){
		if (camino[i] == ciudad){
			return 1;
		}
	}
	return 0;
}


int buscarPareja(int vector1[], int vector2[],int num ,int tam){
	int i;
	for(i=0; i<num;i++){
		if(vector1[i]==num){
			return vector2[i];
		}
	}
	return -1;
}

/*
El operador de cruce toma dos padres y genera dos cadenas hijas. 
Los parámetros requeridos son: 
padre1, padre2: Vectores que contiene la cadena correspondiente los caminos padre
La función debe retornar:
hijo1, hijo2: Vectores correspondientes a los decendientes de los caminos padre
     
*/		

void subCruce(int inicial, int final, int padre[], int hijo[], int vector1[], int vector2[], int tam, int nCiudades){
	for (int i = inicial; i < final; i++){
		int num = padre[i];
		int e = existe(hijo, num, nCiudades);
		while (e == 1)
		{
			num = buscarPareja(vector1, vector2, num, tam);
			e = existe(hijo, num, nCiudades);
		}
		hijo[i] = num;
	}
}
	 
void cruce(int padre1[], int padre2[], int hijo1[], int hijo2[],int nCiudades)
{
	// se generan aleatoriamente los puntos de cruce
	int pCruce1 = rand() % (nCiudades-2) + 1;
	int pCruce2 = (rand() % (nCiudades-pCruce1-1) )+ pCruce1+1;
	
	
	int *vector1, *vector2;
    int tam = pCruce2-pCruce1;
    vector1 = ( int * ) malloc ( tam * sizeof ( int ) );
    vector2 = ( int * ) malloc ( tam * sizeof ( int ) );
	// iniciamos vectores hijos
	for(int i=0;i<nCiudades;i++)
	{
		hijo1[i]=0;
		hijo2[i]=0;
	}
	
	// Se insertan en los hijos los genes de la parte central de los padres
	// hijo1 <- el centro del padre2
	// hijo2 <- el centro del padre1
	int k=0;
	for (int i=pCruce1;i<pCruce2;i++)
	{
		hijo1[i]=padre2[i];
		hijo2[i]=padre1[i];
		vector2[k] = padre2[i];
		vector1[k] = padre1[i];
		k++;
	}
	//primera parte del hijo1, se toma la primera parte del padre 1, pero si algun
    //valor se repite, es reemplazado según la relacion establecida entre los genes
    //de la parte central.
	subCruce(0,pCruce1,padre1,hijo1,vector2,vector1,tam,nCiudades);

	// Segunda parte del hijo1, se toma la segunda parte del padre 1, pero si algun
    // valor se repite, es reemplazado según la relacion establecida entre los genes
    // de la parte central.
    subCruce(pCruce2,nCiudades,padre1,hijo1,vector2,vector1,tam,nCiudades);

	//Primera parte del hijo2, se toma la primera parte del padre 2, pero si algun
    //valor se repite, es reemplazado según la relacion establecida entre los genes
    //de la parte central.

	subCruce(0,pCruce1,padre2,hijo2,vector1,vector2,tam,nCiudades);
	//Segunda parte del hijo2, se toma la segunda parte del padre 2, pero si algun
    //valor se repite, es reemplazado según la relacion establecida entre los genes
    //de la parte central.

	subCruce(pCruce2,nCiudades,padre2,hijo2,vector1,vector2,tam,nCiudades);

	free(vector1);
	free(vector2);
 }


 int mutacion(int poblacion[], double distancia[], double aptitud[],double probMutacion, double matrizDistancias[], int tamPoblacion, int nCiudades){
	int mutados = 0;
	int i, j, mutado, *individuo;
	individuo = (int *)malloc(nCiudades * sizeof(int));
	double prob;
	for (i = 0; i < tamPoblacion; i++){
		mutado = 0;
		for (j = 0; j < nCiudades; j++)
		{
			prob = (rand() % 1000 + 1) / 1000.0;
			if (prob < probMut){
				int pos2 = rand() % nCiudades;
				int aux = poblacion[i * nCiudades + j];
				poblacion[i * nCiudades + j] = poblacion[i * nCiudades + pos2];
				poblacion[i * nCiudades + pos2] = aux;
				mutado = 1;
			}
		}
		if (mutado == 1){
			mutados++;
			extraerVector(poblacion, individuo, i, tamPoblacion, nCiudades);
			distancias[i] = longitudCircuito(individuo, matrizDistancias, nCiudades);
			double mayorDistancia = maxElementoVector(distancias, tamPoblacion);
			aptitud[i] = adaptacion(distancias[i], mayorDistancia);
		}
	}
	 return mutados;
}

int main()
{
	// Para inlcuir caracteres espa�o�
	//setlocale(LC_ALL, "spanish"); // en Linux
    //SetConsoleCP(1252); 
    //SetConsoleOutputCP(1252);
	// Semilla para n�meros aleatorios

	srand(time(NULL)); 
    
    // Declaraci�n de variables
	int nCiudades = 4;
	int tamPoblacion = 30;
	int xMin = 5, xMax = 20, yMin = 5, yMax = 20;
	
	int *coordenadasX, *coordenadasY, *camino;
	float *MatrizDistancia;
	
	coordenadasX = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	coordenadasY = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	camino = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	MatrizDistancia = ( float * ) malloc ( nCiudades * nCiudades * sizeof ( float ) );

	
	printf (" \n ======Fundamentos de computaci�n paralela y distribuida ======");
	printf (" \n       Algoritmo Gen�tico Simple para el problema del TSP");
	printf (" \n                 Versi�n serial y paralela \n");
	printf (" \n N�mero de ciudades: %d", nCiudades);
	printf (" \n Tama�o de la poblaci�n: %d",tamPoblacion);
		printf (" \n \n");
	
	generarCoordenadas(coordenadasX, coordenadasY, nCiudades, xMin, xMax, yMin, yMax);
	
	printf("Coordenadas en X : \n");
	imprimirVector(coordenadasX,nCiudades);
	printf("Coordenadas en Y : \n");
	imprimirVector(coordenadasY,nCiudades);

	crearMatrizDistancia(MatrizDistancia,coordenadasX,coordenadasY,nCiudades);
	imprimirMatriz(MatrizDistancia,nCiudades,nCiudades);

	int camino2[4] =  {1,2,3,6};
	int camino3[4] =  {3,1,9,4};
	int hijo1[4] =  {8, 5, 4,3};
	int hijo2[4] =  {8, 5, 4,3};
	
	generaIndividuo(camino,nCiudades);
	imprimirVector(camino, nCiudades);

	cruce(camino2,camino3,hijo1,hijo2,nCiudades);

	imprimirVector(hijo1, nCiudades);
	imprimirVector(hijo2, nCiudades);
	
}
