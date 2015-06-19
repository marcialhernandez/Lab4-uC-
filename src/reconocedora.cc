#include "reconocedora.h"

string Reconocedora::check(string entrada){
	int estado = 0;
	string pertenece="no";
	int tamEntrada=int(entrada.size());

	for (int i=0;i<tamEntrada;i++){

		if (entrada[i] == 'G' && estado ==0){
			estado=1;
		}
		else if (estado==0 && entrada[i]!='G'){
			estado=0;
		}

		else if (estado==1 &&  entrada[i]=='C'){
			estado=0;
		}

		else if ((estado==1 || estado==2) && entrada[i] == 'T'){
			estado=2;
		}

		else if ((estado==1 || estado==2) && entrada[i]=='G'){
			estado=1;
		}

		else if ((estado==0 || estado==1 || estado==2) && entrada[i]=='A'){
			estado=0;
		}

		else if (estado==2 && entrada[i]=='C'){
			estado=3;
			pertenece="si";
			break;
		}
	}

	return pertenece;
}

void Reconocedora::main() {

	string item;

	while (BufferArchivoEntrada.estadoTermino()==false){

		item = BufferArchivoEntrada.remove();

		if (item!="-1"){

			item+=" "+check(item);
			//cout << "L: '" <<item <<"' reconocida de bufferEntrada"<< endl;
			BufferArchivoSalida.insert( item );
			//cout <<"Se ha insertado el item: '" << item <<"' en bufferSalida"<<endl;
		}

		else{

			break;
		}

		yield(2); //cambio de contexto ->Depende del planificador cambiarlo o no

	}
}