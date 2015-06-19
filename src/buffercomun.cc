#include "buffercomun.h"

_Nomutex bool BufferComun::estadoTermino(){

			if (estadoLectura==true && count <1){
				return true;
			}

			else{
				return false;
			}
		}

_Nomutex void BufferComun::cambiaEstado(bool estadoNuevo){
			estadoLectura=estadoNuevo;
		}

void BufferComun::free(){
			delete [] elements;
		}

BufferComun::BufferComun(int largo){
			front=0;
			back=0;
			count=0;
			largoBuffer=largo;
			elements=new string[largoBuffer];
			estadoLectura=false;
		} 


void BufferComun::insert(string elem) { 
			if (count == largoBuffer) _Accept( remove );
			elements[back] = elem;
			back = (back+1)% largoBuffer;
			count += 1;
}

string BufferComun::remove() {
			if (count == 0) _Accept( insert );
			string elem = elements[front];
			front = (front+1)%largoBuffer;
			count -= 1;
			return elem;
};

