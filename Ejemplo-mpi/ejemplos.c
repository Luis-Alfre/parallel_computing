
#include<mpi.h>
#include <stdio.h>
// Este programa inicia un entorno de comunicación por paso de mensajes
// e imprime el identificador de los procesos que lanza
int main (int arge, char *argvl]){
int rank, size;
// Iniciar el entorno MPI
MPI Init (dargc, &argv) :
// Obtener el numero de procesos lanzados en el comunicador global
MPI Comm size (MPI COMM WORLD, &size) :
/ Obtener el identificador de cada proceso en el comunicador global
MI Comm rank (MPI COMM WORLD,
grank):
printf(" in - Hola desde el proceso &d de d en total in
", rank, size):
/ se finaliza la comunicación entre los procesos
MPI Finalize();
// Aqui ya no pueden enviarse mensajes entre procesos, pero el codigo
sigue
1/
ejecutandose en cada proceso lanzado

}