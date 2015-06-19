#ifndef PRODUCTORA_H
#define PRODUCTORA_H
#include <buffercomun.h>

using namespace std;

_Task Productora {

	BufferComun &BufferArchivoEntrada;
	string &nombreArchivoEntrada;

	public:

		Productora( BufferComun &buf, string &nombreArchivo ) : BufferArchivoEntrada( buf ), nombreArchivoEntrada ( nombreArchivo )  {}

	private:

		inline bool existeArchivo(const string& nombreArchivo);

		void main();

};

#endif