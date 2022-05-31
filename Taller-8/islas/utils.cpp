/*
Universidad de Pamplona
Facultad de Ingenier�as y Arquitectura
Programa de Ingenier�a de Sistemas
Fundamentos de Computaci�n paralela y distribuida

Implementaci�n de una esquema de soluci�n al problema del TSP mediante un Algoritmo Gen�tico Simple
Version Serial y paralela
*/

/* #include <cstdlib> */
#include <cstdlib>
#include <stdio.h>
/* #include<windows.h> */
#include <time.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
//#include<utilidades.h>
// #include<locale.h>  linux

#include <string.h>
#include <sys/stat.h>

#include <iostream>
#include <cstdio>
#include <fstream>
using namespace std;


#define COORDINATES_X "coordenadasX.txt"
#define COORDINATES_Y "coordenadasY.txt"

#define DIM1 48


void createCoordinateVectors(double Coordinates[], string cordinates){
	ifstream file;
    string linea;
    file.open(cordinates, ios::in);
    int c=0;
    while (!file.eof())
    {
        getline(file,linea);
		if (linea != "")
		{
			Coordinates[c]=std::stod(linea);
        	c++;
		}
    }
}



void imprimirVectorEnteros(int V[], int n) {
	for (int i=0; i<n; i++)
		printf(" %d \t",V[i]);
	printf("\n");
}

void imprimirVectorReales(double V[], int n) {
	for (int i=0; i<n; i++)
		printf(" %.2f \t",V[i]);
	printf("\n");
}

void imprimirMatrizEnteros(int M[], int f, int c) {
	for (int i=0; i<f; i++) {
		for(int j=0; j<c; j++)
			printf(" %d \t",M[i*c+j]);
		printf("\n");
	}

}

void imprimirMatrizReales(double M[], int f, int c) {
	for (int i=0; i<f; i++) {
		for(int j=0; j<c; j++)
			printf(" %.2f \t",M[i*c+j]);
		printf("\n");
	}
}

/*

*/
void imprimirVectorEnteroFichero(int vector[], int n,char nombre[]) {
	FILE *fichero;
	fichero = fopen(nombre,"w");
	for(int i=0; i<n; i++) {
		if (i<n-1)
			fprintf(fichero,"%d,",vector[i]);
		else
			fprintf(fichero,"%d",vector[i]);
	}
	fclose(fichero);
}

/*

*/
void imprimirVectorRealFichero(double vector[], int n,char nombre[]) {
	FILE *fichero;
	fichero = fopen(nombre,"w");
	for(int i=0; i<n; i++) {
		if (i<n-1)
			fprintf(fichero,"%.2f,",vector[i]);
		else
			fprintf(fichero,"%.2f",vector[i]);
	}
	fclose(fichero);
}

void extraerVector(int matriz[],int vector[], int pos, int m, int n) {
	for(int j=0; j<n; j++)
		vector[j]=matriz[n*pos + j];
}
void insertarVector(int matriz[],int vector[],int pos, int m, int n) {
	for(int j=0; j<n; j++)
		matriz[n*pos + j]= vector[j];
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

void generarCoordenadas(double coordenadasX[], double coordenadasY[], int nCiudades, int xMin, int xMax, int yMin, int yMax)
{
	for (int i=0;i<nCiudades;i++)
		{
			int rango = xMax - xMin + 1;
			coordenadasX[i] = rand()%rango + xMin;
			rango = yMax - yMin +  1;
			coordenadasY[i] = rand()%rango + yMin;
		}
		
	// Se imprime el vector en un archivo de texto.
	char cX[]= "coordenadasX.txt";
	imprimirVectorRealFichero(coordenadasX,nCiudades,cX);
	char cY[] = "coordenadasY.txt";
	imprimirVectorRealFichero(coordenadasY,nCiudades,cY);
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
void crearMatrizDistancia(double matrizDistancias[], double coordenadasX[], double coordenadasY[], int nCiudades) {
	for (int i=0; i<nCiudades; i++)
		for(int j=0; j<nCiudades; j++) {
			double distancia = sqrt(pow((coordenadasX[i]-coordenadasX[j]),2)+ pow((coordenadasY[i]-coordenadasY[j]),2));
			matrizDistancias[i*nCiudades+j]=distancia;
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
float longitudCircuito(int camino[], double matrizDistancia[], int nCiudades) {
	double suma = 0;
	for(int i=0; i<nCiudades-1; i++) {
		double distancia = matrizDistancia[(camino[i]-1)*nCiudades + camino[i+1]-1];
		suma += distancia;
	}
	suma += matrizDistancia[(camino[nCiudades-1]-1)*nCiudades + camino[0]-1];
	return (suma);
}

/*

Funci�n que genera una permutaci�n con el recorrido por n ciudades, sin repetir ciudad.

Par�metros de entrada:

nCiudades: n�mero de ciudades bajo an�lisis.

Salida de la funci�n:

camino:  vector de enteros consistente en una permutaci�n de n ciudades entre 1 y n.

*/
void generaIndividuo(int camino[], int nCiudades) {
	int existe,i,j,ciudad;
	for(i=0; i<nCiudades; i++) {

		existe = 1;
		while (existe==1) {
			existe = 0;
			ciudad = rand() % nCiudades + 1;
			j=0;
			while((j<i) && (existe==0)) {
				if (camino[j]==ciudad)
					existe=1;
				j++;
			}
			if (existe == 0)
				camino[i]=ciudad;
		}
	}
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

float adaptacion(float longitud, float longitudMaxima) {
	float aptitud = (longitudMaxima * 1.1 ) - longitud;
	return aptitud;
}

float maxElementoVector(float vector[], int n) {
	float max = -1;
	for(int i=0; i<n; i++) {
		if (vector[i]>max)
			max = vector[i];
	}
	return max;
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

void poblacionInicial(int poblacion[], double distancias[], double aptitud[], int tamPoblacion, int nCiudades, double matrizDistancia[]) {
	int i,j,camino[nCiudades];
	double longitud, longMaxima=-1;
	for(i=0; i<tamPoblacion; i++) {
		//generaIndividuo(camino,nCiudades);
		generaIndividuo(camino, nCiudades);
		distancias[i] = longitudCircuito(camino, matrizDistancia,nCiudades);
		if (distancias[i]>longMaxima)
			longMaxima = distancias[i];
		for(j=0; j<nCiudades; j++) {
			poblacion[i*nCiudades+j] = camino[j];
		}
	}
	for (i=0; i<tamPoblacion; i++) {
		aptitud[i] = adaptacion(distancias[i],longMaxima);
	}
}


/*
Funci�n que permite calcular para cada individuo de la poblaci�n:
la puntuaci�n, que corresponde a la probabilidad de ser seleccionado en la siguiente generaci�n,
la puntuaci�n acumulada, obtenida con los valores anteriores
y la posici�n (indice de la fila) del individuo mejor adaptado.

Par�metros de entrada:

poblacion: array bidimensional (matriz) con los individuos en representaci�n adecuada.
aptitud: array unidimensional, con el valor de la aptitud (adaptaci�n) de cada individuo

Salida de la funci�n:

puntuacion: puntuaci�n de cada individuo que corresponde a la probabilidad de selecci�n
			calculada como la adaptaci�n del individuo sobre la suma total
			de las adaptaciones de toda la poblaci�n.
puntAcumulada: obtenida como la suma acumulada de las adaptaciones individuales. Este valor se
               utilizar� para el proceso de selecci�n de la poblaci�n.
posMejor: Valor entero que representa el �ndice de la fila en
		  la tabla de la poblaci�n con el mejor.

*/


int evaluacion(double aptitud[],  // datos entrada
               double puntuacion[], double puntAcumulada[], // Salida de la funcion
               int tamPoblacion) { // datos entrada
	// Se suma calcula la suma de todas las aptitudes y
	// se busca cual es la mejor aptitud
	float sumaAptitud = 0;
	float aptitudMejor = -1;
	float acumulada = 0;
	int posMejor = 0;
	for(int i=0; i<tamPoblacion; i++) {
		sumaAptitud+=aptitud[i];
		if(aptitud[i]>aptitudMejor) {
			posMejor = i;
			aptitudMejor = aptitud[i];
		}
	}
	// Se calcula la puntuacion y la puntuacion acumulada
	for(int i=0; i<tamPoblacion; i++) {
		puntuacion[i]=aptitud[i]/sumaAptitud;
		puntAcumulada[i] = puntuacion[i]+acumulada;
		acumulada += puntuacion[i];
	}
	return posMejor;
}
/*
Funci�n que  selecciona los individuos que van a trascender a la siguiente generaci�n
o que van a cruzarse para generar nuevos individuos.
La selecci�n se realiza mediante la t�cnica de la ruleta,
teniendo en cuenta la puntuaci�n acumulada.

Par�metros de entrada:

tamPoblaci�n: Tama�o de la poblaci�n ( filas de la matriz poblacion)
nCiudades: N�mero de ciudades de la poblacion ( columnas de la matriz poblacion

poblacion: la tabla (matriz) con los individuos en forma de cadena binaria (genotipos),
           de la generaci�n actual.
puntAcumulada: Vector con la puntuaci�n acumulada de cada individuo que se utilizar�
               para seleccionar los individuos que formar�n la pr�xima generaci�n,
aptitud: Vector de aptitud de la poblaci�n, se usar� para actualizar la nueva poblacion
distancia: vector de distancias de la poblaci�n, se usar� para actualizar la nueva problaci�n

Salida de la funci�n:

pobAuxiliar: Tabla (matriz) con los individuos de la nueva generaci�n.

nuevaAptitud: vector con la aptitud (adaptaci�n) de cada individuo de la nueva generaci�n.

nuevaDistancias: vector con las distancias de cada individuo de la nueva generaci�n.

*/
void seleccion(int poblacion[], double puntAcumulada [],
               double aptitud[], double distancias[],// datos entrada
               int pobAuxiliar[], double nuevaAptitud[], double nuevaDistancias[], // Salida de la funcion
               int tamPoblacion, int nCiudades ) { // datos entrada

	int *selSuper; // vector que guarda los indices de los seleccionados para sobrevivir
	int *camino; // almacena los individuos que se insertaran en la nueva poblacion
	camino = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	selSuper = ( int * ) malloc ( tamPoblacion * sizeof ( int ) );
	double probSel; // Almacenar la probabilidad de seleccion
	int posSuper; // posici�n del superviviente

	// Se seleccionan las posiciones de los supervivientes
	for(int i=0; i<tamPoblacion; i++) {
		probSel = (rand() % 100 + 1)/100.0;
		//printf("\n probabilidad = %.2f ",probSel);
		posSuper = 0;
		while((probSel>puntAcumulada[posSuper])&&(posSuper<tamPoblacion-1))
			posSuper++;
		selSuper[i]=posSuper;
	}
	// Se genera la poblaci�n superviviente
	for(int i=0; i<tamPoblacion; i++) {
		int pos = selSuper[i];
		extraerVector(poblacion,camino,pos,tamPoblacion,nCiudades);
		insertarVector(pobAuxiliar,camino,i,i,nCiudades);
		nuevaAptitud[i]= aptitud[pos];
		nuevaDistancias[i] = distancias[pos];
	}
	free(selSuper);
	free(camino);
}

/*
Esta funci�n verifica si el valor dado en el par�metro ciudad ya existe en el camino
Los parametros de entrada son:
camino: Un individuo que correponde a un camino valido
ciudad: numero entero que cuyo valor se busca dentro del cromosoma
nCiudades: tama�o del vector camino

Valor retornado: 1, si el num existe en el cromosoma, 0 en caso contrario
*/
int existe(int camino[],int ciudad,int nCiudades) {
	for (int i = 0; i < nCiudades ; i++) {
		if (camino[i]==ciudad) {
			return 1;
		}
	}
	return 0;
}

/*
Esta funcion relaciona dos vectores (segmentos de caminos)
para el operador de cruce multipunto

Los parametros de entrada son:
vector1: elementos del segmento del primer camino en la relaci�n
vector2: elementos del segmento del segundo camino en la relaci�n
num: elemento del que quiero buscar su pareja
tam: cantidad de elementos en cada vector

Valor retornado: entero que esta relacionado con el elemento de entrada num

*/
int buscarPareja(int vector1[], int vector2[],int num ,int tam) {
	if(existe(vector1,num,tam)==1) {
		for (int i = 0; i < tam ; i++) {
			if (vector1[i]==num) {
				return vector2[i];
			}
		}
	}
	return(-1);
}
/*
El operador de cruce toma dos padres y genera dos cadenas hijas.
Los par�metros requeridos son:
padre1, padre2: Vectores que contiene la cadena correspondiente los caminos padre
La funci�n debe retornar:
hijo1, hijo2: Vectores correspondientes a los decendientes de los caminos padre

*/

void cruce(int padre1[], int padre2[], int hijo1[], int hijo2[],int nCiudades) {
	// se generan aleatoriamente los puntos de cruce
	int pCruce1 = rand() % (nCiudades-2) + 1;
	int pCruce2 = (rand() % (nCiudades-pCruce1-1) )+ pCruce1+1;


	int *vector1, *vector2;
	int tam = pCruce2-pCruce1;
	vector1 = ( int * ) malloc ( tam * sizeof ( int ) );
	vector2 = ( int * ) malloc ( tam * sizeof ( int ) );
	// iniciamos vectores hijos
	for(int i=0; i<nCiudades; i++) {
		hijo1[i]=0;
		hijo2[i]=0;
	}

	// Se insertan en los hijos los genes de la parte central de los padres
	// hijo1 <- el centro del padre2
	// hijo2 <- el centro del padre1
	int k=0;
	for (int i=pCruce1; i<pCruce2; i++) {
		hijo1[i]=padre2[i];
		hijo2[i]=padre1[i];
		vector2[k] = padre2[i];
		vector1[k] = padre1[i];
		k++;
	}
	//primera parte del hijo1, se toma la primera parte del padre 1, pero si algun
	//valor se repite, es reemplazado seg�n la relacion establecida entre los genes
	//de la parte central.

	for(int i=0; i<pCruce1; i++) {
		int num = padre1[i];
		int e = existe(hijo1,num,nCiudades);
		while(e==1) {
			num = buscarPareja(vector2,vector1,num,tam);
			e = existe(hijo1,num,nCiudades);
		}
		hijo1[i] = num;
	}
	// Segunda parte del hijo1, se toma la segunda parte del padre 1, pero si algun
	// valor se repite, es reemplazado seg�n la relacion establecida entre los genes
	// de la parte central.

	for(int i=pCruce2; i<nCiudades; i++) {
		int num = padre1[i];
		int e = existe(hijo1,num,nCiudades);
		while(e==1) {

			num = buscarPareja(vector2,vector1,num,tam);
			e = existe(hijo1,num,nCiudades);
		}
		hijo1[i] = num;
	}
	//Primera parte del hijo2, se toma la primera parte del padre 2, pero si algun
	//valor se repite, es reemplazado seg�n la relacion establecida entre los genes
	//de la parte central.
	for(int i=0; i<pCruce1; i++) {
		int num = padre2[i];
		int e = existe(hijo2,num,nCiudades);
		while(e==1) {
			num = buscarPareja(vector1,vector2,num,tam);
			e = existe(hijo2,num,nCiudades);
		}
		hijo2[i] = num;
	}
	//Segunda parte del hijo2, se toma la segunda parte del padre 2, pero si algun
	//valor se repite, es reemplazado seg�n la relacion establecida entre los genes
	//de la parte central.
	for(int i=pCruce2; i<nCiudades; i++) {
		int num = padre2[i];
		int e = existe(hijo2,num,nCiudades);
		while(e==1) {
			num = buscarPareja(vector1,vector2,num,tam);
			e = existe(hijo2,num,nCiudades);
		}
		hijo2[i] = num;
	}

	free(vector1);
	free(vector2);
}
/*
Esta funci�n selecciona parejas de individuos de la poblaci�n dada como par�metro
(poblacion seleccionada ) y de acuerdo a la probabilidad de cruce,
realiza el cruce obteniendo dos nuevos individuos que reemplazan a sus padres.
Retorna la poblaci�n con los individuos nuevos insertados.
La funcion tiene los siguientes par�metros:
    pobacion:  tabla de datos con los caminos en forma de combinacion de enteros
    prob_cruce: Valor que determina la probabilidad de que una pareja de individuos se cruce

La funci�n debe retornar:
    nuevaPoblacion:  la nueva poblacion con los individuos obtenidos como resultado del cruce
    aptitud: el vector con el valor de la aptitud de cada camino
    distancia: el vector con el valor de la distancia de cada camino
*/
void reproduccion(int poblacion[], double distancia[], double aptitud[],
                  double probCruce, double matrizDistancias[], int tamPoblacion, int nCiudades) {
	int *selCruce, *hijo1, *hijo2, *padre1, *padre2, i; // vector que guarda los indices de los seleccionados para cruce
	int numSelCruce = 0; // contador de seleccionados para cruce
	hijo1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	hijo2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	padre1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	padre2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	selCruce = ( int * ) malloc ( tamPoblacion * sizeof ( int ) );
	// Este ciclo elije los individuos para cruzar
	for(i=0; i<tamPoblacion; i++) {
		double prob = (rand() % 100 + 1)/100.0;

		// Se eligen los individuos de las posiciones i si prob < proCruce
		if (prob < probCruce) {
			selCruce[numSelCruce] = i;
			numSelCruce += 1;
		}
	}

	// El numero de seleccionados se hace par
	if ((numSelCruce % 2)==1)
		numSelCruce-= 1;
	double mayorDistancia = -1;
	for (i=0; i<tamPoblacion; i++)
		if(distancia[i]>mayorDistancia)
			mayorDistancia = distancia[i];
	i = 0;
	while(i<numSelCruce) {
		extraerVector(poblacion,padre1,selCruce[i],tamPoblacion,nCiudades);
		extraerVector(poblacion,padre2,selCruce[i+1],tamPoblacion,nCiudades);
		cruce(padre1,padre2,hijo1,hijo2,nCiudades);
		insertarVector(poblacion,hijo1,selCruce[i],tamPoblacion,nCiudades);
		distancia[selCruce[i]] = longitudCircuito(hijo1,matrizDistancias,nCiudades);
		if (distancia[selCruce[i]]>mayorDistancia)
			mayorDistancia = distancia[selCruce[i]];
		insertarVector(poblacion,hijo2,selCruce[i+1],tamPoblacion,nCiudades);
		distancia[selCruce[i+1]] = longitudCircuito(hijo2,matrizDistancias,nCiudades);
		if (distancia[selCruce[i+1]]>mayorDistancia)
			mayorDistancia = distancia[selCruce[i+1]];
		aptitud[selCruce[i]] = adaptacion(distancia[selCruce[i]],mayorDistancia);
		aptitud[selCruce[i+1]] = adaptacion(distancia[selCruce[i+1]],mayorDistancia);
		i = i + 2;
	}
	free(hijo1);
	free(hijo2 );
	free(padre1);
	free(padre2 );
	free(selCruce);
}

/*
Esta funci�n recorre cada individuo de la poblaci�n y de acuerdo de acuerdo a la probabilidad de mutacion,
realiza la mutaci�n de alguno de los genes que compone el individuo. Debe actualizar la distancia y aptitud
del individuo mutado

Retorna la poblaci�n con los individuos nuevos mutados y un n�mero entero que indica cuantas
mutaciones se llevaron a cabo.

La funcion tiene los siguientes par�metros:
    poblacion:  tabla de datos con los caminos en forma de combinacion de enteros
    probMutacion: Valor que determina la probabilidad de que gen mute en cada individuo
    distancia: vector de distancia que requiere ser actualizado
    aptitud: vector de aptitudes que requiere ser actualizado
    probMutaci�n: n�mero real entre 0 y 1 que corresponde a la probabilidad de que un inividuo mute. ( valor peque�o)
	matrizDistancias: requerida para actualizad la distancia del recorrido
	tamPoblacion: Cantidad de individuos en la poblaci�n.
	nCiudades: Cantidad de ciudades en el problema.

La funci�n debe retornar:
    poblacion:  la actualizaci�n de la poblacion con los individuos mutados.
    aptitud: el vector con el valor de la aptitud de cada camino actualizado
    distancia: el vector con el valor de la distancia de cada camino actualizado
    numero entero: que corresponde a la cantidad de mutaciones realizadas en la poblaci�n.
*/
int mutacion(int poblacion[], double distancia[],
             double aptitud[],double probMutacion,
             double matrizDistancias[], int tamPoblacion, int nCiudades) {
	// seleccion es el vector que guarda los indices de los seleccionados para la mutacion
	//el sujeto es el que se le hara una mutacion en su gen en cada iteracion
	int *seleccion,*sujeto, i;
	// contador de seleccionados para la mutacion
	int totalMutados = 0;
	//vector que selecciona a los mutados por su posicion
	seleccion = ( int * ) malloc ( tamPoblacion * sizeof ( int ) );
	//vector que guardarda la informacion genetica del sujeto para realizar la mutacion
	sujeto = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	// Este ciclo elije los individuos para mutar
	for(i=0; i<tamPoblacion; i++) {
		double prob = (rand() % 100 + 1)/100.0;
		// Se realiza la seleccion de mutados
		if (prob < probMutacion) {
			seleccion[totalMutados] = i;
			totalMutados += 1;
		}
	}
	//se determina cual es la ruta con mayor distancia
	double mayorDistancia = -1;
	for (i=0; i<tamPoblacion; i++) {
		if(distancia[i]>mayorDistancia) {
			mayorDistancia = distancia[i];
		}
	}

	for(i=0; i<totalMutados; i++) { //se realizara la mutacion a todos los sujetos seleccionados
		//extrae el gen sujeto al que se mutara
		extraerVector(poblacion,sujeto,seleccion[i],tamPoblacion,nCiudades);
		//se intercambian la informacion genetica del sujeto  para realizar la mutacion
		int info = sujeto[0];
		sujeto[0] = sujeto[5];
		sujeto[5] = info;
		//se inserta el individuo mutado en la poblacion
		insertarVector(poblacion,sujeto,seleccion[i],tamPoblacion,nCiudades);
		//se calcula el nuevo circuito del individuo
		distancia[seleccion[i]] = longitudCircuito(sujeto,matrizDistancias,nCiudades);
		//se determina si este nuevo sujeto es el que tiene la mayor distancia con la finalidad de actualizar la aptitud
		if (distancia[seleccion[i]]>mayorDistancia)
			mayorDistancia = distancia[seleccion[i]];
		aptitud[seleccion[i]] = adaptacion(distancia[seleccion[i]],mayorDistancia);
	}
	free(sujeto);
	free(seleccion);
	//retorna el total de sujetos mutados en esta generacion
	return totalMutados;
}

//la poblacion  se dividira en 4
void dividirPoblacion(int tamPoblacion,int poblacion[],int isla1[],
					int isla2[],int isla3[],int isla4[],int nCiudades) {
	int	*sujeto,n,x;
	n=tamPoblacion/4;
	sujeto = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	x=0;
	for(int i=0; i<tamPoblacion; i++) {
		if(i<n) {//rango de datos para la Isla 1
			extraerVector(poblacion,sujeto,i,tamPoblacion,nCiudades);
			insertarVector(isla1,sujeto,x,n,nCiudades);
			x++;
		}
		if(i>=n&&i<n*2) {//rango de datos para la isla 2
			if(i==n) {
				x=0;
			}
			extraerVector(poblacion,sujeto,i,tamPoblacion,nCiudades);
			insertarVector(isla2,sujeto,x,n,nCiudades);
			x++;
		}
		if(i>=n*2&&i<n*3) {//rango de datos para la Isla 3
			if(i==n*2) {
				x=0;
			}
			extraerVector(poblacion,sujeto,i,tamPoblacion,nCiudades);
			insertarVector(isla3,sujeto,x,n,nCiudades);
			x++;
		}
		if(i>=n*3&&i<n*4) {// rango de datos paraIsla la 4
			if(i==n*3) {
				x=0;
			}
			extraerVector(poblacion,sujeto,i,tamPoblacion,nCiudades);
			insertarVector(isla4,sujeto,x,n,nCiudades);
			x++;
		}
	}
}

void aplicarIntercambio(int isla_a[],int isla_b[],int posMejor_a,
                        int posMejor_b, int nCiudades,int tamSubPoblacion) {
	int *sujeto_a,*sujeto_b;
	sujeto_a = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	sujeto_b = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	extraerVector(isla_a,sujeto_a,posMejor_a,tamSubPoblacion,nCiudades);
	extraerVector(isla_b,sujeto_b,posMejor_b,tamSubPoblacion,nCiudades);
	insertarVector(isla_a,sujeto_b,posMejor_a,tamSubPoblacion,nCiudades);
	insertarVector(isla_b,sujeto_a,posMejor_b,tamSubPoblacion,nCiudades);
}



