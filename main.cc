#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

//Entrada: String a analizar
//Retorno:
//		true en caso que satisfaga la expresion regular (A+C+G+T)^(*)GT^(+)CT^(*)(A+C+G+T)^(*)
//		false en caso contrario

_Monitor BoundedBuffer {
	//uCondition full, empty;
	int front, back, count;
	string elements[20];

	public:

		BoundedBuffer() : front(0), back(0), count(0) {}
		
		_Nomutex int query() { return count; }

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

bool reconocedor(string entrada){
	int estado = 0;
	bool pertenece=false;

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
			pertenece=true;
			break;
		}
	}
	return pertenece;
}

void uMain::main(){

	string entrada="AGAAAGGCATAAATATATTAGTATTTGTGTACATCTGTTCCTTCCTGTGTGACCCTAAGT";
	
	if (reconocedor(entrada)==true){
		cout << "si"<< endl;
	}

	else{
		cout << "no"<< endl;
	}

	const int NoOfCons = 2, NoOfProds = 3;

	BoundedBuffer buf; // Monitor

	Consumer *cons[NoOfCons]; // Tareas Consumidoras

	Producer *prods[NoOfProds]; // Tareas Productoras

	for ( int i = 0; i < NoOfCons; i += 1 )
		cons[i] = new Consumer( buf );

	for ( int i = 0; i < NoOfProds; i += 1 )
		prods[i] = new Producer( buf );

	for ( int i = 0; i < NoOfProds; i += 1 )
		delete prods[i];

	for ( int i = 0; i < NoOfCons; i += 1 )
		buf.insert( "-1" );

	for ( int i = 0; i < NoOfCons; i += 1 )
		delete cons[i];
	}

//Forma de compilar: u++ main.cc -o main
//A pesar de que la funcion main no tiene como entrada argc y argv, en realidad si existen!