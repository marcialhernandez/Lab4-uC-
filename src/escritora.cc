#include "escritora.h"

void Escritora::main() {

	string item=BufferArchivoSalida.remove();

	ofstream archivoSalida (nombreArchivoSalida.c_str());
	if (archivoSalida.is_open()){


		while ( item!= "-1" ) {

			archivoSalida <<item << endl;

			item = BufferArchivoSalida.remove();
			yield(2); //cambio de contexto ->Depende del planificador cambiarlo o no

		}

		archivoSalida.close();

	}

	else {

		cout << "Error 10: El archivo: '" <<nombreArchivoSalida<< "' no se puede abrir."<< endl;
		exit(0);
	}

}
