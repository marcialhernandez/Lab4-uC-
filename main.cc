#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

bool automata(string entrada){
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
	if (automata(entrada)==true){
		cout << "si"<< endl;
	}
	else{
		cout << "no"<< endl;
	}
}

//Forma de compilar: u++ main.cc -o main
//A pesar de que la funcion main no tiene como entrada argc y argv, en realidad si existen!
