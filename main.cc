#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


//**********************************Funciones************************//

int isNumber(const string entradaConsola, int * entradaPrograma ){
	int largoEntrada=entradaConsola.size();
	for (int contador=0;contador<largoEntrada;contador++){
		if (!isdigit(entradaConsola.at(contador))){
			return 1;
		}
	}

	*entradaPrograma=stoi(entradaConsola);
	return 0;
}

bool recibeArgumentosConsola(int argc, char **argv, string *nombreEntrada, string *nombreSalida,int *numeroTareasReconocedoras, int *largoBufferEntrada, int *largoBufferSalida){

	/* Declaracion de las banderas */

	//Las banderas _i, _N, _o y _d: para asegurar que solo haya un argumento
	//Por ejemplo; podria escribir por consola -i entrada1 -i entrada2
	//si pasa esto, se retornara un mensaje de error y se terminara la ejecucion

	const char* const opciones = "h:i:o:l:L:";

	int banderaErrorParametros=0, banderaErrorBanderas=0, bandera_i=0, bandera_h=0,bandera_o=0, bandera_l=0, bandera_L=0, argumentoConsola;
	bool estadoValorBufferEntrada =true, estadoValorBufferSalida =true, estadoNumeroTareas =true;

	while (((argumentoConsola = getopt (argc, argv, opciones)) != -1) &&  banderaErrorParametros==0 && banderaErrorBanderas==0){
		//No tiene caso seguir con el while, si se ha detectado una falla en el camino

		switch (argumentoConsola){  

			case 'i': if (bandera_i==0) { //archivo entrada

					  bandera_i++; 

					  *nombreEntrada=optarg;

				  }
				  else{
					  banderaErrorBanderas++;						
				  }
				  break;	  
			case 'h': if (bandera_h==0) {
					  bandera_h++;
					  banderaErrorParametros = banderaErrorParametros + isNumber(optarg, numeroTareasReconocedoras );
					  
					  if (*numeroTareasReconocedoras==0){
						  banderaErrorParametros++;
						  estadoNumeroTareas=false;
					  }					  
				  }              
				  else {
					  banderaErrorBanderas++;
				  }
				  break;

			case 'o': if (bandera_o==0) { //archivo salida

					  bandera_o++; 

					  *nombreSalida=optarg;

				  }
				  else{
					  banderaErrorBanderas++;						
				  }
				  break;

			case 'L': if (bandera_L==0) {

					  bandera_L++; 

					  banderaErrorParametros = banderaErrorParametros + isNumber(optarg, largoBufferEntrada );

					  if (*largoBufferEntrada==0){
						  banderaErrorParametros++;
						  estadoValorBufferEntrada=false;
					  }
				  }
				  else{
					  banderaErrorBanderas++;						
				  }
				  break;
			case 'l': if (bandera_l==0) {

					  bandera_l++; 

					  banderaErrorParametros = banderaErrorParametros + isNumber(optarg, largoBufferSalida);

					  if (*largoBufferSalida==0){
						  banderaErrorParametros++;
						  estadoValorBufferSalida=false;
					  }

				  }
				  else{
					  banderaErrorBanderas++;						
				  }
				  break;


			case ':': banderaErrorParametros++; break;
			case '?': 
				  if ((optopt=='i' || optopt=='o' || optopt=='h' || optopt=='L' || optopt=='l')){
					  banderaErrorParametros++;
				  }
				  else{
					  banderaErrorBanderas++;
				  }
				  break;

			default: banderaErrorBanderas++; break;
		}
	}

	if (banderaErrorBanderas>0){
		cout << "Error 1: Una o más opciones estan duplicadas o no estan disponibles." << endl;
		return false;
	}

	if (bandera_i==0){
		cout << "Error 2: No se ha especificado archivo de entrada." << endl;
		return false;
	}

	if (bandera_h==0){
		cout << "Error 3: No se ha especificado la cantidad de tareas reconocedoras." << endl;
		return false;
	}

	if (banderaErrorParametros>0){

		if (estadoValorBufferEntrada==false){
			cout << "Error 4: El largo del buffer de lectura no puede ser cero." << endl;
			return false;
		}

		else if (estadoValorBufferSalida==false){
			cout << "Error 5: El largo del buffer de escritura no puede ser cero." << endl;
			return false;
		}

		else if (estadoNumeroTareas==false){
			cout << "Error 6: La cantidad de tareas reconocedoras no puede ser cero." << endl;
			return false;
		}

		else{
			cout << "Error 7: Una de las entradas de las banderas -h -L o -l no es valida." << endl;
			return false;
		}
	}

	return true;

}

//**********************************Clases*************************************//

_Monitor BoundedBuffer {

	int front, back, count;
	bool estadoLectura=false;
	string *elements;
	int largoBuffer;

	public:

		BoundedBuffer(int largo){
			front=0;
			back=0;
			count=0;
			largoBuffer=largo;
			elements=new string[largoBuffer];
			cout << "buffer tam '" << largoBuffer <<"' creado!!" << endl;

		} //front(0), back(0), count(0) {}

		~BoundedBuffer(){}

		void free(){
			delete [] elements;
		}

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
			if (count == largoBuffer) _Accept( remove );
			elements[back] = elem;
			back = (back+1)% largoBuffer;
			count += 1;
}

string BoundedBuffer::remove() {
			if (count == 0) _Accept( insert );
			string elem = elements[front];
			front = (front+1)%largoBuffer;
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

	cout << argc << endl;

	const int cantidadReconocedoras = 3;

	BoundedBuffer bufferLector(4),bufferEscritor(1);

	Reconocedora *tareasReconocedoras[cantidadReconocedoras];

	for ( int i = 0; i < cantidadReconocedoras; i += 1 )
		tareasReconocedoras[i] = new Reconocedora( bufferLector, bufferEscritor );

	Escritora *tareaEscritora = new Escritora( bufferEscritor, nombreArchivoSalida );

	Productora *tareaProductora = new Productora( bufferLector, nombreArchivoEntrada );

	delete tareaProductora;
	bufferLector.insert( "-1" );

	for ( int i = 0; i < cantidadReconocedoras; i += 1 )
		delete tareasReconocedoras[i];

	bufferEscritor.insert( "-1" );

	delete tareaEscritora;

	bufferLector.free();

	bufferEscritor.free();

}

//Forma de compilar: u++ main.cc -o main
//A pesar de que la funcion main no tiene como entrada argc y argv, en realidad si existen!