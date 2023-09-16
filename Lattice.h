#include "Renderer.h"


typedef struct element{
	int val;
	int x,y;
}element;

class Lattice{
	Renderer ventana;
	int n,netapas;
	int **lattice;
	double *energias;
	int colors[3][3];
//	friend class Renderer;
	public:

	Lattice();
	Lattice(int n){
		this->n = n;
		energias = nullptr;
		//Array approach
		lattice =  new int*[n];
		for(int i=0;i<n;i++)
			lattice[i] = new int[n];

		int color0[3] = {66, 28, 82};//morado
		int color1[3] = {44, 103, 0};//verde
		int color2[3] = {255, 207, 121};//amarillo

		for(int i=0;i<3;i++){
			colors[0][i] = color0[i];
			colors[1][i] = color1[i];
			colors[2][i] = color2[i];
		}
	}
	Lattice(const Lattice &lat){
		n = lat.n;
		energias = nullptr;
		lattice = new int*[lat.n];
		for(int i=0;i<n;i++)
			lattice[i] = new int[n];
		for(int i=0;i<3;i++){
			colors[0][i] = lat.colors[0][i];
			colors[1][i] = lat.colors[1][i];
			colors[2][i] = lat.colors[2][i];
		}
	}
	void init_ising_lattice(int l,int d);
	element selec_random();
	element vecino(element s0);

	//Calcula el cambio de energIa al intercmabiar vecinos s0 y s1  viendo la configuracion N0 del paper
	int difN0(element s0, element s1);
	//Calcula el cambio de energIa al intercambiar vecinos s0 y s1  viendo la configuracion N1 del paper
	int difN1(element s0,element s1);
	//Calcula el cambio de energIa al intercmabiar vecinos s0 y s1  viendo la configuracion N2 del paper
	int difN2(element s0,element s1);
	void Probar(element s0, element s1,std::string );
	void modelo_cristalizacion(unsigned int netapas,double T,double C0,double C1,double C2);

	void GuardarCsv(std::string str);
	void Actualizar(int, double&, double, double,double,double);
	void Animar(unsigned int netapas,double T,double C0,double C1,double C2);
	int Mostrar(std::string str);
	~Lattice(){
		//liberacion de memoria
		for(int i=0;i<n;i++){
			delete[] lattice[i];
		}
		delete[] lattice;
		if(energias!=nullptr)
			delete[] energias;
	}
};
