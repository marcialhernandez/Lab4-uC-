#include "productora.h"

inline bool Productora::existeArchivo(const string& nombreArchivo) {
	struct stat buffer;   
	return (stat (nombreArchivo.c_str(), &buffer) == 0); 
}

void Productora::main() {

	if (existeArchivo (nombreArchivoEntrada)){

		string linea;

		ifstream archivoEntrada;

		archivoEntrada.open(nombreArchivoEntrada.c_str());

		//cout << "El archivo: '" <<nombreArchivoEntrada<< "' ha sido abierto correctamente."<< endl;

		while(!archivoEntrada.eof()){
	
			linea="";
			archivoEntrada >> linea;

			if (linea!=""){
				BufferArchivoEntrada.insert( linea );
				//cout <<"Se ha insertado la linea: '" << linea <<"' en bufferEntrada"<<endl;
			}

			yield(2); //cambio de contexto ->Depende del planificador cambiarlo o no

		}

		BufferArchivoEntrada.cambiaEstado(true);

		archivoEntrada.close();
	}

	else{

		cout << "Error 9: El archivo: '" <<nombreArchivoEntrada<< "' no existe."<< endl;
		exit(0);
	}

}