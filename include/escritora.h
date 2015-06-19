#ifndef ESCRITORA_H
#define ESCRITORA_H
#include <buffercomun.h>

using namespace std;

_Task Escritora {

	BufferComun &BufferArchivoSalida;
	string &nombreArchivoSalida;
	
	public:

		Escritora( BufferComun &buf, string &nombreArchivo ) : BufferArchivoSalida( buf ), nombreArchivoSalida ( nombreArchivo )  {}

	private:

		void main();
};

#endif