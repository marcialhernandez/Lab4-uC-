#ifndef RECONOCEDORA_H
#define RECONOCEDORA_H
#include <buffercomun.h>

using namespace std;

_Task Reconocedora {

	BufferComun &BufferArchivoEntrada;
	BufferComun &BufferArchivoSalida;

	public:

		Reconocedora( BufferComun &bufferEntrada, BufferComun &bufferSalida ) : BufferArchivoEntrada( bufferEntrada ), BufferArchivoSalida(bufferSalida)  {}

	private:

		string check(string entrada);

		void main();
};

#endif