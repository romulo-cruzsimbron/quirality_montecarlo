#include <vector>
#include<string>
typedef struct element{
	int val;
	int x,y;
}element;

class Lattice{
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
	void read_ising_lattice();
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
	void dfs(int x, int y, std::vector<int> direction,int *ret_yellow,int *ret_green, int *n_misc);
	void preclusters(int &green_small,int &yellow_small, std::vector<float> &clusters, std::vector<float> &quiralidad_green, std::vector<float> &quiralidad_yellow);
	bool valid (int x, int y);
	void distribucion(std::vector<float> &clusters);
	void GuardarCsv(std::string str);
	void GuardarVector(std::vector<float> soluto,std::string file_name);
	void GuardarVectorTwo(std::vector<float> starting, std::vector<float> soluto);
	void Actualizar(int, double&, double, double,double,double);
	void printi();
	void vis_set();
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
