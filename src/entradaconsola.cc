#include "entradaconsola.h"

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
					  banderaErrorParametros += isNumber(optarg, numeroTareasReconocedoras );
					  
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

					  banderaErrorParametros += isNumber(optarg, largoBufferEntrada );

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

					  banderaErrorParametros += isNumber(optarg, largoBufferSalida);

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

	if (bandera_o==0){
		cout << "Error 3: No se ha especificado archivo de salida." << endl;
		return false;
	}

	if (bandera_h==0){
		cout << "Error 4: No se ha especificado la cantidad de tareas reconocedoras." << endl;
		return false;
	}

	if (banderaErrorParametros>0){

		if (estadoValorBufferEntrada==false){
			cout << "Error 5: El largo del buffer de lectura no puede ser cero." << endl;
			return false;
		}

		else if (estadoValorBufferSalida==false){
			cout << "Error 6: El largo del buffer de escritura no puede ser cero." << endl;
			return false;
		}

		else if (estadoNumeroTareas==false){
			cout << "Error 7: La cantidad de tareas reconocedoras no puede ser cero." << endl;
			return false;
		}

		else{
			cout << "Error 8: Una de las entradas de las banderas -h -L o -l no es valida." << endl;
			return false;
		}
	}

	return true;

}