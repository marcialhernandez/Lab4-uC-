#ifndef BUFFERCOMUN_H
#define BUFFERCOMUN_H
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

_Monitor BufferComun {

	int front, back, count, largoBuffer;
	bool estadoLectura=false;
	string *elements;

	public:

		BufferComun(int largo);

		void free();

		_Nomutex void cambiaEstado(bool estadoNuevo);

		_Nomutex bool estadoTermino();

		void insert (string elem);

		string remove() ;

};

#endif