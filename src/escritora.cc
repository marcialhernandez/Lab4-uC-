#include "escritora.h"

void Escritora::main() {

	string item;
	bool avisaInicio=true;

	ofstream archivoSalida (nombreArchivoSalida.c_str());
	if (archivoSalida.is_open()){

		for ( ;; ) {

			item = BufferArchivoSalida.remove();

			if ( item!= "-1"){

				//cout << "L: '" <<item << "'' Escrita en " << nombreArchivoSalida << endl;
		
				if (avisaInicio==true){

					archivoSalida << item;
					avisaInicio=false;

				}

				else{

					archivoSalida <<endl<<item;

				}

			}

			else{

				break;
			}

		}

		archivoSalida.close();

	}

	else {

		cout << "Error 10: El archivo: '" <<nombreArchivoSalida<< "' no se puede abrir."<< endl;
		exit(0);
	}

}
