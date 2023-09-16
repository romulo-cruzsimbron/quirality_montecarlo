#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "Lattice.h"
#include <chrono>
#include <omp.h>
#include <numeric>

using namespace std;
using namespace std::chrono;

	
int main(int argc, char** argv){
	//Valores iniciales que se pueden modificar con parametros de entrada
	int n =50;
	//double T =1.9;
	double C0 =-1;
	double C1 =-2;
	double C2 =-5;
	Lattice lattice( n );


	//Paralelizamos
	
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	#pragma omp parallel firstprivate(lattice) shared(cout,arrC0,arrC1,arrC2,T)
	{
		#pragma omp for
		vector<string> file_list;
		ifstream input( "list.txt" );
		for( string eachLine; getline( input, eachLine ); )
		{
		  istringstream strm(eachLine);
		  string splitedLines;
		  // loop for each string and add to the vector
		  while ( strm >> splitedLines )
				{
				stringstream geek(splitedLines);
				string num;    // can be int, float or double
				geek >>num;
				//perform action on all num in each line one by one 
				file_list.push_back(num);
				}    
		  }
		
		//cout<<file_list[2];	
		int len = file_list.size();
		vector<int> soluto_list;
		vector<float> soluto_means;
		vector<float> starting;
		vector<float> starting2;
		//vector<int> cluster_list;
		for(int i=0;i<len;i++)
		{
			vector<float> cluster_list(521,0);
			vector<float> quiralidad_green_list(521,0);
			vector<float> quiralidad_yellow_list(521,0);
			cout<<file_list[i]<<"\n";
			cout<<endl;	
			//string file = "Imagenes/L_260_260_120-10-20-50";
			string file = file_list[i];
			lattice.Probar({1,0,0}, {1,0,1},file);
			file = file.erase(0,9);
			//cout << file<<"\n";
			string fileName0 =file+".csv";
			ofstream plot0(fileName0);
			file = file.erase(0,2);
			cout << file<<"\n";
			
			
			//Definir concentracion inicial desde el nombre
			stringstream test(file);
			string segment;
			vector<string> seglist;

			while(getline(test, segment, '_'))
			{
				seglist.push_back(segment);
			}

			cout << seglist[2]<<"\n";
			
			//Extraer el valor de la temperaturas desde el nombre
			stringstream test2(seglist[2]);
			string segment2;
			vector<string> seglist2;

			while(getline(test2, segment2, '-'))
			{
				seglist2.push_back(segment2);
			}
			cout << seglist2[0]<<"\n";			
			
			double T =stod(seglist2[0])/100;
			starting2.push_back(T);				
			

			soluto_list = {};
			//file =  "260_260_120-10-20-50";
			vector< vector<float> > cluster_list_list;
			vector< vector<float> > quiralidad_green_list_list;
			vector< vector<float> > quiralidad_yellow_list_list;
			for (int et = 1; et< 1000; et++){
				//vector<int> cluster_list(520,0);			
				int netapas = 100000;
				string filename = file;
				lattice.modelo_cristalizacion(netapas,T,C0,C1,C2);
				lattice.GuardarCsv(filename);
				lattice.Probar({1,0,0}, {1,0,1},filename);
				int green_small_lat = 0;
				int yellow_small_lat = 0;
				lattice.preclusters(green_small_lat,yellow_small_lat,cluster_list,quiralidad_green_list,quiralidad_yellow_list);			
				soluto_list.push_back(green_small_lat+yellow_small_lat);
				lattice.distribucion(cluster_list);
				cluster_list_list.push_back(cluster_list);
				quiralidad_green_list_list.push_back(quiralidad_green_list);
				quiralidad_yellow_list_list.push_back(quiralidad_yellow_list);
				//lattice.GuardarVector(cluster_list);
				//lattice.printi();
				lattice.vis_set();
				string fileName ="L_"+filename+".csv";
				//cout<<fileName<<"\n";
				ofstream plot(fileName);			
				if(plot.is_open()){
					if (remove(fileName.c_str()) != 0)
						perror("File deletion failed");
					else
						//cout << "File deleted successfully \n";	
					plot.close();
				}
				else cerr<<"No se pudo abrir el archivo \n";

				string fileName2 ="E_"+filename+".csv";
				ofstream plot2(fileName2);
				if(plot2.is_open()){
					if (remove(fileName2.c_str()) != 0)
						perror("File deletion failed");
					else
						//cout << "File deleted successfully \n";	
					plot2.close();
				}
				else cerr<<"No se pudo abrir el archivo \n";
			
			}
			
			//lattice.GuardarVector(cluster_list_list[2]);
			auto l_cluster = cluster_list_list.size(); 
			vector<float> distribucion_final;
			vector<float> green_final;
			vector<float> yellow_final;
			vector<float> quiralidad_promedio_final;
			for (int i = 0; i< 521; i++)
			{
				vector<float> suma_vector;
				vector<float> suma_green;
				vector<float> suma_yellow;
				for (int j = 0; j< l_cluster; j++)
				{
					suma_vector.push_back(cluster_list_list[j][i]);
					suma_green.push_back(quiralidad_green_list_list[j][i]);
					suma_yellow.push_back(quiralidad_yellow_list_list[j][i]);
				}				
				float frecuencia = 0.0f;
				float promedio_green = 0.0f;
				float promedio_yellow = 0.0f;
				frecuencia = accumulate( suma_vector.begin(), suma_vector.end(), 0.0) / l_cluster;
				promedio_green = accumulate(suma_green.begin(),suma_green.end(),0.0)/ l_cluster;
				promedio_yellow = accumulate(suma_yellow.begin(),suma_yellow.end(),0.0)/ l_cluster;
				distribucion_final.push_back(frecuencia);
				green_final.push_back(promedio_green);
				yellow_final.push_back(promedio_yellow);
				quiralidad_promedio_final.push_back((promedio_green-promedio_yellow)/(promedio_green+promedio_yellow));
				
				
				
				



			}
			lattice.GuardarVector(distribucion_final,"Distribucion/"+file+".txt");
			lattice.GuardarVector(green_final,"Quiralidad_green/"+file+".txt");
			lattice.GuardarVector(yellow_final,"Quiralidad_yellow/"+file+".txt");
			lattice.GuardarVector(quiralidad_promedio_final,"Quiralidad_exceso/"+file+".txt");
			
			
			
			auto n = soluto_list.size(); 
			float average = 0.0f;
			if ( n != 0) 
			{
				average = accumulate( soluto_list.begin(), soluto_list.end(), 0.0) / n; 
			}
			soluto_means.push_back(average);
			
			
		}
		lattice.GuardarVectorTwo(starting2,soluto_means);
		//lattice.GuardarVector(cluster_list);			  		
	}
	
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
	cout << "Tiempo de ejecucion: " << 1.0*duration/1000 <<"s"<<endl;

}
