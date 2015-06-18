#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Entrada: String a analizar
//Retorno:
//		"si" en caso que satisfaga la expresion regular (A+C+G+T)^(*)GT^(+)CT^(*)(A+C+G+T)^(*)
//		"no" en caso contrario

_Monitor BoundedBuffer {
	
	int front, back, count;
	bool estadoLectura=false;
	string elements[20];

	public:

		BoundedBuffer() : front(0), back(0), count(0) {}

		void cambiaEstado(bool estadoNuevo){
			estadoLectura=estadoNuevo;
		}

		bool estadoTermino(){
			if (estadoLectura==true && count <=0){
				return true;
			}

			else{
				return false;
			}
		}

		void insert (string elem);
		string remove() ;

};

void BoundedBuffer::insert(string elem) { 
			if (count == 20) _Accept( remove );
			elements[back] = elem;
			back = (back+1)% 20;
			count += 1;
}

string BoundedBuffer::remove() {
			if (count == 0) _Accept( insert );
			string elem = elements[front];
			front = (front+1)%20;
			count -= 1;
			return elem;
};

_Task Productora {

	BoundedBuffer &BufferArchivoEntrada;
	string &nombreArchivoEntrada;
	int cantidadLineasArchivo=0;

	public:

		Productora( BoundedBuffer &buf, string &nombreArchivo ) : BufferArchivoEntrada( buf ), nombreArchivoEntrada ( nombreArchivo )  {}
		_Nomutex int cantidadLineasArchivoEntrada() { return cantidadLineasArchivo; }

	private:

		inline bool existeArchivo(const string& nombreArchivo) {
			struct stat buffer;   
			return (stat (nombreArchivo.c_str(), &buffer) == 0); 
		}

		void main() {

			if (existeArchivo (nombreArchivoEntrada)){

				string linea;

				ifstream archivoEntrada;

				archivoEntrada.open(nombreArchivoEntrada.c_str());

				cout << "El archivo: '" <<nombreArchivoEntrada<< "' ha sido abierto correctamente."<< endl;

				while(!archivoEntrada.eof()){
    		
    				archivoEntrada >> linea;
    				BufferArchivoEntrada.insert( linea );
    				cantidadLineasArchivo+=1;
    				cout <<"Se ha insertado la linea: '" << linea <<"' en bufferEntrada"<<endl;

				}

				BufferArchivoEntrada.cambiaEstado(true);

				archivoEntrada.close();
			}

			else{

				cout << "El archivo: '" <<nombreArchivoEntrada<< "' No existe."<< endl;
				exit(0);
			}

		}
};

_Task Reconocedora {

	BoundedBuffer &BufferArchivoEntrada;
	BoundedBuffer &BufferArchivoSalida;

	public:

		Reconocedora( BoundedBuffer &bufferEntrada, BoundedBuffer &bufferSalida ) : BufferArchivoEntrada( bufferEntrada ), BufferArchivoSalida(bufferSalida)  {}

	private:

		string check(string entrada){
			int estado = 0;
			string pertenece="no";

			for (int i=0;i<entrada.size();i++){

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

		void main() {

			string item;

			while (BufferArchivoEntrada.estadoTermino()==false){

				item = BufferArchivoEntrada.remove();
				if (item!="-1"){

				item+=" "+check(item);
				cout << "L: '" <<item <<"' reconocida de bufferEntrada"<< endl;
				BufferArchivoSalida.insert( item );
    			cout <<"Se ha insertado el item: '" << item <<"' en bufferSalida"<<endl;
    			}

    			if ( BufferArchivoEntrada.estadoTermino()==true ) break;

			}
		}
};

_Task Escritora {

	BoundedBuffer &BufferArchivoSalida;
	string &nombreArchivoSalida;
	
	public:

		Escritora( BoundedBuffer &buf, string &nombreArchivo ) : BufferArchivoSalida( buf ), nombreArchivoSalida ( nombreArchivo )  {}

	private:

		void main() {

			string item;
			for ( ;; ) {

				item = BufferArchivoSalida.remove();

				if ( item!= "-1"){

				cout << "L: '" <<item << "'' Escrita en " << nombreArchivoSalida << endl;

				}

				else{
					break;
				}
			}
		}

};

void uMain::main(){

	string nombreArchivoEntrada="in.txt";
	string nombreArchivoSalida="out.txt";

	const int cantidadProductoras = 1, cantidadReconocedoras = 3, cantidadEscritoras = 1;

	BoundedBuffer bufferLector,bufferEscritor;

	Reconocedora *tareasReconocedoras[cantidadReconocedoras];
	Escritora *tareasEscritoras[cantidadEscritoras];

	for ( int i = 0; i < cantidadReconocedoras; i += 1 )
		tareasReconocedoras[i] = new Reconocedora( bufferLector, bufferEscritor );

	Productora *tareaProductora = new Productora( bufferLector, nombreArchivoEntrada );

	Escritora *tareaEscritora = new Escritora( bufferEscritor, nombreArchivoSalida );

	delete tareaProductora;
	bufferLector.insert( "-1" );

	for ( int i = 0; i < cantidadReconocedoras; i += 1 )
		delete tareasReconocedoras[i];

	bufferEscritor.insert( "-1" );

	delete tareaEscritora;

}

//Forma de compilar: u++ main.cc -o main
//A pesar de que la funcion main no tiene como entrada argc y argv, en realidad si existen!