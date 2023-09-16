#include <iostream>
#include <sstream>
#include "Lattice.h"
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv){
	/*
	//Valores iniciales que se pueden modificar con parametros de entrada
	int n =50;
	unsigned int netapas=10000000;
	int l =250;
	int d =250;
	double T =1.8;
	double C0 =-1;
	double C1 =-2;
	double C2 =-5;
	int tmod = (int)T*100;
	// Valores de entrada al llamar al ejecutable
	if(argc>2){
		char key = argv[1][1];

		istringstream ss(argv[1]);
		int val;
		if (!(ss >> val)) {
  			cerr << "Invalid number: " << argv[1] << endl;
		} else if (!ss.eof()) {
			cerr << "Trailing characters after number: " << argv[1] << '\n';
		}
		switch(key){
			case 'n': n = val; break;
			case 's': netapas = val; break;
			case 'l': l = val; break;
			case 'd': d = (float)val; break;
			case 't': T = (float)val; break;
		}
	}
	*/
	int n = 50;
	unsigned int netapas=100000000;
	Lattice lattice( n );
	int l=250,d=250;
	
	//El tamaño es variable y podemos cambiarlo;
	const int sizec = 1;
	const int sizet = 16;
	double arrC0[] = {-1};
	double arrC1[] = {-2};
	double arrC2[] = {-5};
	double arrT[] = {0.1,0.3,0.5,0.7,0.9,1.1,1.3,1.5,1.7,1.9,2.1,2.3,2.5,2.7,2.9,3.1};
	int arrqui[] = {5,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,245,250};

/*
	//Un solo valor secuencial
	double C0=arrC0[0];
	double C1=arrC1[0];
	double C2=arrC2[0];
	lattice.init_ising_lattice(l,d);
*/
/*
	
	//Probar 1 iteracion
	element u = {1,2,5};// val, x, y
	element v = {1,2,6};
	lattice.Probar(u,v,"L_250_250_180-10-10-30"); //lee la matriz del archivo L_250_...
*/
/*	
	//Modelo de cristalizacion
	high_resolution_clock::time_point t1 = high_resolution_clock::now();// Toma el tiempo inicial
	lattice.modelo_cristalizacion(netapas,T,C0,C1,C2);
	//lattice.Animar(netapas,T,C0,C1,C2); //Crea animacion del modelo en tiempo real...(ejecuta el modelo a la par que lo muestra graficamente)		
	cout << "n: "<<n<< " l: "<<l<< " d: "<< d << " T: " << T <<endl;
	cout << "Num Etapas: "<<netapas<< " C0: "<<C0<< " C1: "<<C1<< " C2: "<<C2 <<endl;
	
	high_resolution_clock::time_point t2 = high_resolution_clock::now(); // Toma el tiempo de fin
	auto duration = duration_cast<milliseconds>( t2 - t1 ).count(); 
	cout << "Tiempo de ejecucion: " << 1.0*duration/1000 <<"s"<<endl; //Imprime el tiempo transcurrido
	
	//Guardamos la data en ficheros que seran leidos por gnuplot
	string filename = to_string(l)+"_"+to_string(d)+"_"+to_string((int)(T*100))+to_string((int)(C0*10))
						+to_string((int)(C1*10))+to_string((int)(C2*10));
	//lattice.Mostrar(filename);
	lattice.GuardarCsv(filename);
	
*/

	//Paralelizamos
	
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	#pragma omp parallel firstprivate(lattice) shared(cout,arrC0,arrC1,arrC2,T)
	{
		#pragma omp for
		for(int k = 0; k < sizec; k++){
			double C0 = arrC0[k];
			for(int j = 0; j< sizec; j++){
				double C1 = arrC1[j];
				for(int i = 0; i< sizec; i++){
					double C2 = arrC2[i];
					for(int g = 0; g< sizet; g++){	
							double T = arrT[g];
							lattice.init_ising_lattice(l,d);
							lattice.modelo_cristalizacion(netapas,T,C0,C1,C2);
							//lattice.Animar(netapas,T,C0,C1,C2); //Crea animacion del modelo en tiempo real...(ejecuta el modelo a la par que lo muestra graficamente)		
							cout << "n: "<<n<< " l: "<<l<< " d: "<< d << " T: " << T <<endl;
							cout << "Num Etapas: "<<netapas<< " C0: "<<C0<< " C1: "<<C1<< " C2: "<<C2 <<endl;
							//Guardamos la data en ficheros que seran leidos por gnuplot
							string filename = to_string(l)+"_"+to_string(d)+"_"+to_string((int)(T*100))+to_string((int)(C0*10))+to_string((int)(C1*10))+to_string((int)(C2*10));
							//graficar la matriz
							//lattice.Mostrar(filename);
							//cout << "imagen" <<endl;
							//Guardamos a csv
							lattice.GuardarCsv(filename);
						
					}
				}
			}
		}
	}
	
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
	cout << "Tiempo de ejecucion: " << 1.0*duration/1000 <<"s"<<endl;

}
