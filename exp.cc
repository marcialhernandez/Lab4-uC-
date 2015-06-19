#include <entradaconsola.h>
#include <escritora.h>
#include <productora.h>
#include <reconocedora.h>

using namespace std;

void uMain::main(){

	string nombreArchivoEntrada, nombreArchivoSalida;

	int cantidadReconocedoras,largoBufferEntrada=10,largoBufferSalida=10;

	if (recibeArgumentosConsola(argc, argv, &nombreArchivoEntrada, &nombreArchivoSalida,&cantidadReconocedoras, &largoBufferEntrada, &largoBufferSalida) ==false){
		exit(1);
	}

	BufferComun bufferLector(largoBufferEntrada),bufferEscritor(largoBufferSalida);

	Reconocedora *tareasReconocedoras[cantidadReconocedoras];

	for ( int i = 0; i < cantidadReconocedoras; i += 1 ){

		tareasReconocedoras[i] = new Reconocedora( bufferLector, bufferEscritor );
	}

	Escritora *tareaEscritora = new Escritora( bufferEscritor, nombreArchivoSalida );

	Productora *tareaProductora = new Productora( bufferLector, nombreArchivoEntrada );

	delete tareaProductora;

	for ( int i = 0; i < cantidadReconocedoras; i += 1 ){

		bufferLector.insert( "-1" );
	}

	for ( int i = 0; i < cantidadReconocedoras; i += 1 ){

		delete tareasReconocedoras[i];
	}

	bufferEscritor.insert( "-1" );

	delete tareaEscritora;

	bufferLector.free();

	bufferEscritor.free();

}