/*
Universidad de Pamplona
Facultad de Ingenier�as y Arquitectura
Programa de Ingenier�a de Sistemas
Fundamentos de Computaci�n paralela y distribuida

Implementaci�n de una esquema de soluci�n al problema del TSP mediante un Algoritmo Gen�tico Simple
Version Serial y paralela
*/

#include<stdio.h>

#include<time.h>
#include <omp.h>
#include<math.h>
#include "utils.cpp"

//#include<utilidades.h>
// #include<locale.h>  linux

int main() {

	// Semilla para n�meros aleatorios
	srand(time(NULL));

	// Declaracion variables - par�metros del algoritmo
	int nCiudades=70; // nodos del grafo
	int tamPoblacion = 1000; // tama�o de la poblaci�n
	int posMejor_1 = 0,posMejor_2 = 0,posMejor_3 = 0,posMejor_4 = 0; // posici�nes de los individuos mejores adaptados de cada isla
	int numMaxGen = 250;
	int mutados=0;

	// Variables dinamicas para almacenamiento de vectores y matrices
	int *poblacion, *padre1, *padre2, *hijo1, *hijo2,*isla1,*isla2,*isla3,*isla4;
	double  *coordenadasX, *coordenadasY,*matrizDistancias,*distancias,*aptitud;
	
	//Punteros correspondietes a cada isla
	double *distancias_1, *aptitud_1, *puntuacion_1,
	       *puntAcumulada_1, *nuevaAptitud_1, *nuevaDistancias_1;
	int *mejorCamino_1, *pobAuxiliar_1;

	double *distancias_2, *aptitud_2, *puntuacion_2,
	       *puntAcumulada_2, *nuevaAptitud_2, *nuevaDistancias_2;
	int *mejorCamino_2, *pobAuxiliar_2;

	double *distancias_3, *aptitud_3, *puntuacion_3,
	       *puntAcumulada_3, *nuevaAptitud_3, *nuevaDistancias_3;
	int *mejorCamino_3, *pobAuxiliar_3;

	double *distancias_4, *aptitud_4, *puntuacion_4,
	       *puntAcumulada_4, *nuevaAptitud_4, *nuevaDistancias_4;
	int *mejorCamino_4, *pobAuxiliar_4;

	//parametros del algotirmo
	double probCruce = 0.8;
	double probMutacion = 0.1;
	double longitud_1,longitud_2,longitud_3,longitud_4,mejor_long;
	double longitudes[2];
	
	coordenadasX= ( double * ) malloc ( nCiudades * sizeof ( double ) );
	coordenadasY= ( double * ) malloc ( nCiudades * sizeof ( double ) );
	//Se guardan las coordenas extraida de los archivos
	createCoordinateVectors(coordenadasX, COORDINATES_X);
	createCoordinateVectors(coordenadasY, COORDINATES_Y);

	// Se exportan los el vectores x y en unos archivos de texto.
	char cX[]= "coordenadasX.txt";
	imprimirVectorRealFichero(coordenadasX,nCiudades,cX);
	char cY[] = "coordenadasY.txt";
	imprimirVectorRealFichero(coordenadasY,nCiudades,cY);
	

	
	// reserva de memoria para variables din�micas
	padre1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	padre2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	hijo1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	hijo2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	distancias = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	aptitud = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	int tamSubPoblacion = tamPoblacion/4;

	//Punteros de la isla 1
	mejorCamino_1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	distancias_1 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	nuevaDistancias_1 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	aptitud_1 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	nuevaAptitud_1 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	puntuacion_1 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	puntAcumulada_1 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	pobAuxiliar_1 = ( int * ) malloc ( tamSubPoblacion * nCiudades * sizeof ( int ) );
	//Punteros de la isla 2
	mejorCamino_2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	distancias_2 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	nuevaDistancias_2 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	aptitud_2 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	nuevaAptitud_2 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	puntuacion_2 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	puntAcumulada_2 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	pobAuxiliar_2 = ( int * ) malloc ( tamSubPoblacion * nCiudades * sizeof ( int ) );
	//Punteros de la isla 3
	mejorCamino_3 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	distancias_3 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	nuevaDistancias_3 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	aptitud_3 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	nuevaAptitud_3 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	puntuacion_3 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	puntAcumulada_3 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	pobAuxiliar_3 = ( int * ) malloc ( tamSubPoblacion * nCiudades * sizeof ( int ) );
	//Punteros de la isla 4
	mejorCamino_4 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	distancias_4 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	nuevaDistancias_4 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	aptitud_4 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	nuevaAptitud_4 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	puntuacion_4 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	puntAcumulada_4 = ( double * ) malloc ( tamSubPoblacion * sizeof ( double ) );
	pobAuxiliar_4 = ( int * ) malloc ( tamSubPoblacion * nCiudades * sizeof ( int ) );

	//punteros de la poblacion y de las subpoblaciones
	poblacion = ( int * ) malloc ( tamPoblacion * nCiudades * sizeof ( int ) );
	isla1 = ( int * ) malloc ( tamSubPoblacion * nCiudades * sizeof ( int ) );
	isla2 = ( int * ) malloc ( tamSubPoblacion * nCiudades * sizeof ( int ) );
	isla3 = ( int * ) malloc ( tamSubPoblacion * nCiudades * sizeof ( int ) );
	isla4 = ( int * ) malloc ( tamSubPoblacion * nCiudades * sizeof ( int ) );



	printf (" \n ======Fundamentos de computaci�n paralela y distribuida ======");
	printf (" \n       Algoritmo Gen�tico Simple para el problema del TSP");
	printf (" \n                 Versi�n serial y paralela \n");
	printf (" \n N�mero de ciudades: %d", 20);
	printf (" \n Tama�o de la poblaci�n: %d",tamPoblacion);
	printf (" \n Numero de subpoblaciones: %d",4);
	printf (" \n Tama�o de las subpoblaci�nes: %d",tamSubPoblacion);
	printf (" \n Numero de generaciones: %d",numMaxGen);
	printf (" \n Criterio de intercambio de individuos: modelo de anillo, en donde el intercambio se realizara cada 10 generaciones");
	printf (" \n \n");


	// Generacion del problema
	matrizDistancias = ( double * ) malloc ( nCiudades * nCiudades * sizeof ( double ) );
	crearMatrizDistancia(matrizDistancias, coordenadasX, coordenadasY, nCiudades);
	
	// Generaci�n de poblaci�n inicial
	poblacionInicial(poblacion,distancias,aptitud,tamPoblacion,nCiudades,matrizDistancias);
	//Se divide la poblacion en 4 subpoblaciones
	dividirPoblacion(tamPoblacion,poblacion,isla1,isla2,isla3,isla4,nCiudades);
	posMejor_1 = evaluacion(aptitud_1, puntuacion_1, puntAcumulada_1, tamSubPoblacion);
	posMejor_2 = evaluacion(aptitud_2, puntuacion_2, puntAcumulada_2, tamSubPoblacion);
	posMejor_3 = evaluacion(aptitud_3, puntuacion_3, puntAcumulada_3, tamSubPoblacion);
	posMejor_4 = evaluacion(aptitud_4, puntuacion_4, puntAcumulada_4, tamSubPoblacion);

	// Impresi�n en archivo mejor ruta
	extraerVector(isla1,mejorCamino_1,posMejor_1,tamSubPoblacion, nCiudades);
	char nombreCamino[]= "caminoInicial.txt";
	imprimirVectorEnteroFichero(mejorCamino_1,nCiudades,nombreCamino);
	longitud_1 = longitudCircuito(mejorCamino_1, matrizDistancias,nCiudades);
	printf("\n Longitud del camino : %.2f \n",longitud_1 );
	longitudes[0] = longitud_1;
	printf("\n ========= INICIO DEL ALGORITMO GENETICO CON ISLAS==========\n");
	
	//Variables para calcular el tiempo de ejecucion
	double t1,t2,dif1;
	t1 = omp_get_wtime(); // tiempo inicio
	for(int generacion = 1; generacion <= numMaxGen; generacion++) {
		if(generacion%10==0) {
			//Intercambio isla 1 con isla 2
			aplicarIntercambio(isla1,isla2,posMejor_1,posMejor_2,nCiudades,tamSubPoblacion);
			posMejor_1 = evaluacion(aptitud_1, puntuacion_1, puntAcumulada_1, tamSubPoblacion);
			posMejor_2 = evaluacion(aptitud_2, puntuacion_2, puntAcumulada_2, tamSubPoblacion);
			//Intercambio isla 2 con isla 3			
			aplicarIntercambio(isla2,isla3,posMejor_2,posMejor_3,nCiudades,tamSubPoblacion);
			posMejor_2 = evaluacion(aptitud_2, puntuacion_2, puntAcumulada_2, tamSubPoblacion);
			posMejor_3 = evaluacion(aptitud_3, puntuacion_3, puntAcumulada_3, tamSubPoblacion);	
			//Intercambio isla 3 con isla 4
			aplicarIntercambio(isla3,isla4,posMejor_3,posMejor_4,nCiudades,tamSubPoblacion);
			posMejor_3 = evaluacion(aptitud_3, puntuacion_3, puntAcumulada_3, tamSubPoblacion);
			posMejor_4 = evaluacion(aptitud_4, puntuacion_4, puntAcumulada_4, tamSubPoblacion);
			//Intercambio isla 4 con isla 1	
			aplicarIntercambio(isla4,isla1,posMejor_4,posMejor_1,nCiudades,tamSubPoblacion);
			posMejor_1 = evaluacion(aptitud_1, puntuacion_1, puntAcumulada_1, tamSubPoblacion);
			posMejor_4 = evaluacion(aptitud_4, puntuacion_4, puntAcumulada_4, tamSubPoblacion);
		}

		#pragma omp parallel num_threads(8)
		#pragma omp sections  
		{
			# pragma omp section
			{
				//Genetico en la isla 1
		seleccion(isla1, puntAcumulada_1, aptitud_1, distancias_1,// datos entrada
		          pobAuxiliar_1,nuevaAptitud_1,nuevaDistancias_1, // Salida de la funcion
		          tamSubPoblacion,nCiudades ); // datos entrada
		isla1 = pobAuxiliar_1;
		distancias_1 = nuevaDistancias_1;
		aptitud_1 = nuevaAptitud_1;
		reproduccion(isla1,distancias_1,aptitud_1,probCruce,
		             matrizDistancias,tamSubPoblacion,nCiudades);
		mutados += mutacion(isla1,distancias_1,aptitud_1,probMutacion,
		                    matrizDistancias,tamSubPoblacion,nCiudades);

		posMejor_1 = evaluacion(aptitud_1,  // datos entrada
		                      puntuacion_1,puntAcumulada_1, // Salida de la funcion
		                      tamSubPoblacion); // datos entrada

			}
			# pragma omp section
			{
				//Genetico en la isla 2
				seleccion(isla2, puntAcumulada_2, aptitud_2, distancias_2,// datos entrada
						pobAuxiliar_2,nuevaAptitud_2,nuevaDistancias_2, // Salida de la funcion
						tamSubPoblacion,nCiudades ); // datos entrada
				isla2 = pobAuxiliar_2;
				distancias_2 = nuevaDistancias_2;
				aptitud_2 = nuevaAptitud_2;
				reproduccion(isla2,distancias_2,aptitud_2,probCruce,
							matrizDistancias,tamSubPoblacion,nCiudades);
				mutados += mutacion(isla2,distancias_2,aptitud_2,probMutacion,
									matrizDistancias,tamSubPoblacion,nCiudades);

				posMejor_2 = evaluacion(aptitud_2,  // datos entrada
									puntuacion_2,puntAcumulada_2, // Salida de la funcion
									tamSubPoblacion); // datos entrada
			}
			# pragma omp section
			{
				//Genetico en la isla 3
		seleccion(isla3, puntAcumulada_3, aptitud_3, distancias_3,// datos entrada
		          pobAuxiliar_3,nuevaAptitud_3,nuevaDistancias_3, // Salida de la funcion
		          tamSubPoblacion,nCiudades ); // datos entrada
		isla3 = pobAuxiliar_3;
		distancias_3 = nuevaDistancias_3;
		aptitud_3 = nuevaAptitud_3;
		reproduccion(isla3,distancias_3,aptitud_3,probCruce,
		             matrizDistancias,tamSubPoblacion,nCiudades);
		mutados += mutacion(isla3,distancias_3,aptitud_3,probMutacion,
		                    matrizDistancias,tamSubPoblacion,nCiudades);

		posMejor_3 = evaluacion(aptitud_3,  // datos entrada
		                      puntuacion_3,puntAcumulada_3, // Salida de la funcion
		                      tamSubPoblacion); // datos entrada

			}
			# pragma omp section
			{
		//Genetico en la isla 4
		seleccion(isla4, puntAcumulada_4, aptitud_4, distancias_4,// datos entrada
		          pobAuxiliar_4,nuevaAptitud_4,nuevaDistancias_4, // Salida de la funcion
		          tamSubPoblacion,nCiudades ); // datos entrada
		isla4 = pobAuxiliar_4;
		distancias_4 = nuevaDistancias_4;
		aptitud_4= nuevaAptitud_4;
		reproduccion(isla4,distancias_4,aptitud_4,probCruce,
		             matrizDistancias,tamSubPoblacion,nCiudades);
		mutados += mutacion(isla4,distancias_4,aptitud_4,probMutacion,
		                    matrizDistancias,tamSubPoblacion,nCiudades);

		posMejor_4 = evaluacion(aptitud_4,  // datos entrada
		                      puntuacion_4,puntAcumulada_4, // Salida de la funcion
		                      tamSubPoblacion); // datos entrada
			                      
			}
		}
		
		
		
	}
	t2 = omp_get_wtime(); // tiempo finzalizaci�n
	dif1 = t2-t1;  // diferencia 
	printf ("\n ALGORITMO GENETICO en serial- tiempo %f\n",dif1);
	
	//Obtiene el mejor individuo de cada isla
	extraerVector(isla1,mejorCamino_1,posMejor_1,tamSubPoblacion, nCiudades);
	extraerVector(isla2,mejorCamino_2,posMejor_2,tamSubPoblacion, nCiudades);
	extraerVector(isla3,mejorCamino_3,posMejor_3,tamSubPoblacion, nCiudades);
	extraerVector(isla4,mejorCamino_4,posMejor_4,tamSubPoblacion, nCiudades);

	longitud_1 = longitudCircuito(mejorCamino_1, matrizDistancias,nCiudades);
	longitud_2 = longitudCircuito(mejorCamino_2, matrizDistancias,nCiudades);
	longitud_3 = longitudCircuito(mejorCamino_3, matrizDistancias,nCiudades);
	longitud_4 = longitudCircuito(mejorCamino_4, matrizDistancias,nCiudades);

	mejor_long= longitud_1;
	char nombreCaminoFinal [] = "caminoFinal.txt";
	if(mejor_long>longitud_2) {
		mejor_long= longitud_2;
		imprimirVectorEnteroFichero(mejorCamino_2,nCiudades,nombreCaminoFinal);
	}
	if(mejor_long>longitud_3) {
		mejor_long= longitud_3;
		imprimirVectorEnteroFichero(mejorCamino_3,nCiudades,nombreCaminoFinal);
	} 
	if(mejor_long>longitud_4) {
		mejor_long= longitud_4;
		imprimirVectorEnteroFichero(mejorCamino_4,nCiudades,nombreCaminoFinal);
	} 
	if(mejor_long==longitud_1){
		imprimirVectorEnteroFichero(mejorCamino_1,nCiudades,nombreCaminoFinal);
	}
	printf("\n Longitud del camino final isla 1: %.2f \n",longitud_1);
	printf("\n Longitud del camino final isla 2: %.2f \n",longitud_2 );
	printf("\n Longitud del camino final isla 3: %.2f \n",longitud_3 );
	printf("\n Longitud del camino final isla 4: %.2f \n",longitud_4 );
	printf("\n Longitud del camino final (mejor camino): %.2f \n",mejor_long );
	longitudes[1]= mejor_long;
	char texLongitudes[]= "longitudes.txt";
	imprimirVectorRealFichero(longitudes,2,texLongitudes);
	printf("\n Total sujetos mutados : %d \n",mutados);
}
