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
	//uCondition full, empty;
	int front, back, count;
	bool estadoLectura=false;
	string elements[20];

	public:

		BoundedBuffer() : front(0), back(0), count(0) {}

		void cambiaEstado(bool estadoNuevo){
			estadoLectura=estadoNuevo;
		}

		//_Nomutex bool estado(){ return estadoLectura;}

		bool estado(){ return estadoLectura;}
		int query() { return count; }

		bool estadoTermino(){
			if (estadoLectura==true && count ==0){
				return true;
			}

			else{
				return false;
			}
		}
		
		//_Nomutex int query() { return count; }

		void insert (string elem);
		string remove() ;

};

//Metodos BoundedBuffer 
void BoundedBuffer::insert(string elem) { 
			if (count == 20) _Accept( remove );//empty.wait();
			elements[back] = elem;
			back = (back+1)% 20;
			count += 1;
			//full.signal();
}

string BoundedBuffer::remove() {
			if (count == 0) _Accept( insert );//full.wait();
			string elem = elements[front];
			front = (front+1)%20;
			count -= 1;
			//empty.signal();
			return elem;
};

/*inline bool existeArchivo(const string& nombreArchivo) {
  struct stat buffer;   
  return (stat (nombreArchivo.c_str(), &buffer) == 0); 
}
*/

_Task Producer {

	BoundedBuffer &Buffer;
	
	public:

		Producer( BoundedBuffer &buf ) : Buffer( buf ) {}


	private:

		void main() {
			const int NoOfItems = rand() % 20;
			string item;
			for (int i = 1; i <= NoOfItems; i += 1) {
				yield( rand() % 20 ); // duerma un rato
				item = to_string(rand() % 100 + 1);
				cout << "insertando:" << item << endl; 
				Buffer.insert( item );
			}
		}
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

	BoundedBuffer &BufferArchivoEntrada; // sched. interno o externo
	BoundedBuffer &BufferArchivoSalida; // sched. interno o externo

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

			while (BufferArchivoEntrada.estadoTermino()!=true){//( BufferArchivoEntrada.estado()==false && BufferArchivoEntrada.query()!=0){

				item = BufferArchivoEntrada.remove();

				item+=" "+check(item);
				cout << "L: '" <<item <<"' reconocida de bufferEntrada"<< endl;
				BufferArchivoSalida.insert( item );
    			cout <<"Se ha insertado el item: '" << item <<"' en bufferSalida"<<endl;

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
				cout << "L: '" <<item << "'' Escrita en " << nombreArchivoSalida << endl;
				if ( item == "-1" ) break;
				//yield( rand() % 20 );
			}
		}

		/*void main() {
			string item = BufferArchivoSalida.remove();
			if ( item != "-1" ){
				cout << "L: '" <<item << "'' Escrita en " << nombreArchivoSalida << endl;
			}
		}*/
};

_Task Consumer {

	BoundedBuffer &Buffer; // sched. interno o externo

	public:

		Consumer( BoundedBuffer &buf ) : Buffer( buf ) {}

	private:

		void main() {
			string item;
			for ( ;; ) {
				item = Buffer.remove();
				cout << "consumiendo: " <<item << endl;
				if ( item == "-1" ) break;
				yield( rand() % 20 );
			}
		}
};

///////////////////////////////////////////

/*string reconocedor(string entrada){
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

*/

void uMain::main(){

	string nombreArchivoEntrada="in.txt";
	string nombreArchivoSalida="out.txt";

	//const int NoOfCons = 2, NoOfProds = 3;

	const int cantidadProductoras = 1, cantidadReconocedoras = 9, cantidadEscritoras = 1;

	//string linea;

	/*ifstream archivoEntrada;

	if (existeArchivo (nombreArchivo)){

		archivoEntrada.open(nombreArchivo.c_str());

		cout << "abierto correctamente" << endl;

		while(!archivoEntrada.eof()){
    		archivoEntrada >> linea;
    		cout << linea << " " <<reconocedor(linea)<< endl;
		}
	}

	else{

		cout << "ta malo" << endl;

	}

	archivoEntrada.close();

	*/

	//BoundedBuffer buf; // Monitor///////////

	BoundedBuffer bufferLector,bufferEscritor;

	//Consumer *cons[NoOfCons]; // Tareas Consumidoras

	//Producer *prods[NoOfProds]; // Tareas Productoras

	//Productora *tareasProductoras[cantidadProductoras];
	Reconocedora *tareasReconocedoras[cantidadReconocedoras];
	Escritora *tareasEscritoras[cantidadEscritoras];

	/*for ( int i = 0; i < NoOfCons; i += 1 )
		cons[i] = new Consumer( buf );

	for ( int i = 0; i < NoOfProds; i += 1 )
		prods[i] = new Producer( buf );*/

	for ( int i = 0; i < cantidadReconocedoras; i += 1 )
		tareasReconocedoras[i] = new Reconocedora( bufferLector, bufferEscritor );

	//for ( int i = 0; i < cantidadProductoras; i += 1 )
	Productora *tareaProductora = new Productora( bufferLector, nombreArchivoEntrada );

	for ( int i = 0; i < cantidadEscritoras; i += 1 )
		tareasEscritoras[i] = new Escritora( bufferEscritor, nombreArchivoSalida );



	/*for ( int i = 0; i < NoOfProds; i += 1 )
		delete prods[i];

	for ( int i = 0; i < NoOfCons; i += 1 )
		buf.insert( "-1" );

	for ( int i = 0; i < NoOfCons; i += 1 )
		delete cons[i];*/

	delete tareaProductora;

	for ( int i = 0; i < cantidadReconocedoras; i += 1 )
		bufferLector.insert( "-1" );

	for ( int i = 0; i < cantidadReconocedoras; i += 1 )
		delete tareasReconocedoras[i];

	for ( int i = 0; i < cantidadEscritoras; i += 1 )
		bufferEscritor.insert( "-1" );

	for ( int i = 0; i < cantidadEscritoras; i += 1 )
		delete tareasEscritoras[i];

}

//Forma de compilar: u++ main.cc -o main
//A pesar de que la funcion main no tiene como entrada argc y argv, en realidad si existen!