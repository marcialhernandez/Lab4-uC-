#ifndef ENTRADACONSOLA_H
#define ENTRADACONSOLA_H
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int isNumber(const string entradaConsola, int * entradaPrograma );

bool recibeArgumentosConsola(int argc, char **argv, string *nombreEntrada, string *nombreSalida,int *numeroTareasReconocedoras, int *largoBufferEntrada, int *largoBufferSalida);

#endif