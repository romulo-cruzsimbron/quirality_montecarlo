#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Lattice.h"

using namespace std;

inline int mod(int a, int b) {
    //assert(b > 0);
    int ret = a%b;
    return (ret>=0)?(ret):(ret+b);
}


random_device rd;
mt19937 g(rd());

Lattice::Lattice(){}

// Iniciamos una matriz aleatoria
void Lattice::init_ising_lattice(int l,int d){
    //Vector approach
    vector<int> indices(n*n);
    int *ptr = &indices[0];
    
    for(int i=0; i< n*n; i++)
        indices[i] = i;	
    shuffle(indices.begin(), indices.end(), g);
    
    int r,c;
    for(int i=0;i<n*n;i++){
        c = indices[i]%n;
        r = indices[i]/n;
        if(i < l) //Coloreamos L
            lattice[r][c]=1;
        else{
            if(i < d+l) //Coloreamos D
                lattice[r][c]=2;
            else //Coloreamos S
                lattice[r][c]=0;
        }
    }
}

element Lattice::selec_random(){
    uniform_int_distribution<int> distributionN(0,n*n-1);
    int num = distributionN(g);
    int i=num / n, j=num % n; 
    element s0;
    s0.val = lattice[i][j];
    s0.x=i;
    s0.y=j;
    return s0; //retorna valor y coordenadas del punto aleatorio
}

// ubicar al vecino
element Lattice::vecino(element s0){
    //[(0,1),(0,-1),(1,0),(-1,0)]
    uniform_int_distribution<int> distribution4(0,4);
    int r = distribution4(g);
    int x = 0, y = 0;
    switch(r){
        case 0: y = 1; break;
        case 1: y = -1; break;
        case 2: x = 1; break;
        case 3: x = -1; break;
    }
    element s1;
    s1.x = mod((s0.x + x),n);
    s1.y = mod((s0.y + y),n);
    s1.val = lattice[s1.x][s1.y];
        
    return s1; //retorna valor y coordenadas del vecino aleatorio al punto aleatorio
}

//Calcula el cambio de energIa al intercmabiar vecinos s0 y s1  viendo la configuracion N0 del paper
int Lattice::difN0(element s0, element s1){
    int u[]={s0.x,s0.y};
    int v[]={s1.x,s1.y};
    int n0=0;
    int n0_f= 0;
    if (lattice[u[0]][u[1]]==0 || lattice[v[0]][v[1]]==0){
        if (lattice[u[0]][u[1]]>0 && lattice[u[0]][mod(u[1]+1,n)]>0)
            n0=n0+1;
        if (lattice[u[0]][u[1]]>0 && lattice[mod(u[0]-1,n)][u[1]]>0 ) 
            n0=n0+1;
        if (lattice[u[0]][u[1]]>0 && lattice[u[0]][mod(u[1]-1,n)]>0 ) 
            n0=n0+1;
        if (lattice[u[0]][u[1]]>0 && lattice[mod(u[0]+1,n)][u[1]]>0 )  
            n0=n0+1;

        if (lattice[v[0]][v[1]]>0 && lattice[v[0]][mod(v[1]+1,n)]>0 )
            n0=n0+1;
        if (lattice[v[0]][v[1]]>0 && lattice[mod(v[0]-1,n)][v[1]]>0 ) 
            n0=n0+1;
        if (lattice[v[0]][v[1]]>0 && lattice[v[0]][mod(v[1]-1,n)]>0 )  
            n0=n0+1;
        if (lattice[v[0]][v[1]]>0 && lattice[mod(v[0]+1,n)][v[1]]>0 )  
            n0=n0+1;


        if (lattice[v[0]][v[1]]>0 && lattice[u[0]][mod(u[1]+1,n)]>0 && (v[0]!= u[0] || v[1] != mod(u[1]+1,n) ))
            n0_f=n0_f+1;
        if (lattice[v[0]][v[1]]>0 && lattice[mod(u[0]-1,n)][u[1]]>0 && (v[0]!= mod(u[0]-1,n) || v[1] != u[1])) 
            n0_f=n0_f+1;
        if (lattice[v[0]][v[1]]>0 && lattice[u[0]][mod(u[1]-1,n)]>0 && (v[0]!= u[0] || v[1] != mod(u[1]-1,n))) 
            n0_f=n0_f+1;
        if (lattice[v[0]][v[1]]>0 && lattice[(u[0]+1)%n][u[1]]>0 && (v[0]!= (u[0]+1)%n || v[1] != u[1]))  
            n0_f=n0_f+1;

        if (lattice[u[0]][u[1]]>0 && lattice[v[0]][(v[1]+1)%n]>0 && (u[0]!= v[0] || u[1] != (v[1]+1)%n))
            n0_f=n0_f+1;
        if (lattice[u[0]][u[1]]>0 && lattice[mod(v[0]-1,n)][v[1]]>0 && (u[0]!= mod(v[0]-1,n) || u[1]!= v[1]))
            n0_f=n0_f+1;
        if (lattice[u[0]][u[1]]>0 && lattice[v[0]][mod(v[1]-1,n)]>0 && (u[0] != v[0] || u[1] != mod(v[1]-1,n) )) 
            n0_f=n0_f+1;
        if (lattice[u[0]][u[1]]>0 && lattice[(v[0]+1)%n][v[1]]>0 && (u[0]!= (v[0]+1)%n || u[1]!= v[1]))
            n0_f=n0_f+1;
    }
    return n0_f - n0;
}

//Calcula el cambio de energIa al intercambiar vecinos s0 y s1  viendo la configuracion N1 del paper
int Lattice::difN1(element s0,element s1){
    int n1=0;
    int n1_f= 0;
    int u[] = {s0.x,s0.y};
    int v[] = {s1.x,s1.y};
    if (lattice[u[0]][u[1]] != lattice[v[0]][v[1]]){

        if (lattice[u[0]][u[1]] != 0 && lattice[u[0]][u[1]]==lattice[u[0]][(u[1]+1)%n]){
            n1=n1+1;
        }
        if (lattice[u[0]][u[1]] != 0 && lattice[u[0]][u[1]]==lattice[mod(u[0]-1,n)][u[1]]){
            n1=n1+1;
        }
        if (lattice[u[0]][u[1]] != 0 && lattice[u[0]][u[1]]==lattice[u[0]][mod(u[1]-1,n)]){
            n1=n1+1;
        }
        if (lattice[u[0]][u[1]] != 0 && lattice[u[0]][u[1]]==lattice[(u[0]+1)%n][u[1]]){
            n1=n1+1;
        }

        if (lattice[v[0]][v[1]] != 0 && lattice[v[0]][v[1]]==lattice[v[0]][(v[1]+1)%n]){
            n1=n1+1;
        }
        if (lattice[v[0]][v[1]] != 0 && lattice[v[0]][v[1]]==lattice[mod(v[0]-1,n)][v[1]])
            n1++;
        if (lattice[v[0]][v[1]] != 0 && lattice[v[0]][v[1]]==lattice[v[0]][mod(v[1]-1,n)]) 
            n1++;
        if (lattice[v[0]][v[1]] != 0 && lattice[v[0]][v[1]]==lattice[(v[0]+1)%n][v[1]]) 
            n1++;


        if (lattice[v[0]][v[1]] != 0 && lattice[v[0]][v[1]]==lattice[u[0]][(u[1]+1)%n] && (v[0]!= u[0] || v[1] != (u[1]+1)%n)) 
            n1_f=n1_f+1;
        if (lattice[v[0]][v[1]] != 0 && lattice[v[0]][v[1]]==lattice[mod(u[0]-1,n)][u[1]] && (v[0]!= mod(u[0]-1,n) || v[1]!=u[1]))
            n1_f=n1_f+1;
        if (lattice[v[0]][v[1]] != 0 && lattice[v[0]][v[1]]==lattice[u[0]][mod(u[1]-1,n)] && (v[0]!= u[0] || v[1]!= mod(u[1]-1,n))) 
            n1_f=n1_f+1;
        if (lattice[v[0]][v[1]] != 0 && lattice[v[0]][v[1]]==lattice[(u[0]+1)%n][u[1]] && (v[0]!= (u[0]+1)%n || v[1]!= u[1])) 
            n1_f=n1_f+1;

        if (lattice[u[0]][u[1]] != 0 && lattice[u[0]][u[1]]==lattice[v[0]][(v[1]+1)%n] && (u[0]!= v[0] || u[1]!=(v[1]+1)%n))
            n1_f=n1_f+1;
        if (lattice[u[0]][u[1]] != 0 && lattice[u[0]][u[1]]==lattice[mod(v[0]-1,n)][v[1]] && (u[0]!= mod(v[0]-1,n) || u[1]!=v[1])) 
            n1_f=n1_f+1;
        if (lattice[u[0]][u[1]] != 0 && lattice[u[0]][u[1]]==lattice[v[0]][mod(v[1]-1,n)] && (u[0]!= v[0] || u[1]!=mod(v[1]-1,n))) 
            n1_f=n1_f+1;
        if (lattice[u[0]][u[1]] != 0 && lattice[u[0]][u[1]]==lattice[(v[0]+1)%n][v[1]] && (u[0]!= (v[0]+1)%n || u[1]!=v[1])) 
            n1_f=n1_f+1;
    }
    return n1_f - n1;
}



//Calcula el cambio de energIa al intercmabiar vecinos s0 y s1  viendo la configuracion N2 del paper
int Lattice::difN2(element s0,element s1){
    int n2=0;
    int n2_f= 0;
    int u[] = {s0.x,s0.y};
    int v[] = {s1.x,s1.y};
    int z[]={mod(v[0]-u[0],n),mod(v[1]-u[1],n)};
    int zr[]={mod(u[1]-v[1],n),mod(v[0]-u[0],n)};

    //Copia de lattice
    int lat[n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            lat[i][j] = lattice[i][j];
        }
    }
    if (lat[u[0]][u[1]] != 0 || lat[v[0]][v[1]] != 0){
        if (lat[u[0]][u[1]] != 0){
            if (lat[u[0]][u[1]]+lat[(u[0]+1)%n][(u[1]+0)%n]+lat[(u[0]+0)%n][mod(u[1]-1,n)]+lat[(u[0]+1)%n][mod(u[1]-1,n)]==6 && lat[(u[0]+1)%n][mod(u[1]-1,n)]==lat[u[0]][u[1]] && lat[(u[0]+1)%n][(u[1]+0)%n]==lat[(u[0]+0)%n][mod(u[1]-1,n)]){
                n2++;
            }
            if (lat[u[0]][u[1]]+lat[(u[0]+1)%n][(u[1]+0)%n]+lat[(u[0]+0)%n][(u[1]+1)%n]+lat[(u[0]+1)%n][(u[1]+1)%n]==6 && lat[(u[0]+1)%n][(u[1]+1)%n]==lat[u[0]][u[1]] && lat[(u[0]+1)%n][(u[1]+0)%n]==lat[(u[0]+0)%n][(u[1]+1)%n]){
                n2++;
            }
            if (lat[u[0]][u[1]]+lat[mod(u[0]-1,n)][(u[1]+0)%n]+lat[(u[0]+0)%n][mod(u[1]-1,n)]+lat[mod(u[0]-1,n)][mod(u[1]-1,n)]==6 && lat[mod(u[0]-1,n)][mod(u[1]-1,n)]==lat[u[0]][u[1]] && lat[mod(u[0]-1,n)][(u[1]+0)%n]==lat[(u[0]+0)%n][mod(u[1]-1,n)]){
                n2=n2+1;
            }
            if (lat[u[0]][u[1]]+lat[mod(u[0]-1,n)][(u[1]+0)%n]+lat[(u[0]+0)%n][(u[1]+1)%n]+lat[mod(u[0]-1,n)][(u[1]+1)%n]==6 && lat[mod(u[0]-1,n)][(u[1]+1)%n]==lat[u[0]][u[1]] && lat[mod(u[0]-1,n)][(u[1]+0)%n]==lat[(u[0]+0)%n][(u[1]+1)%n]){
                n2=n2+1;
            }
        }
        if (lat[v[0]][v[1]] != 0){
            if (lat[v[0]][v[1]]+lat[(v[0]+z[0])%n][(v[1]+z[1])%n]+lat[(v[0]+zr[0])%n][(v[1]+zr[1])%n]+lat[(v[0]+z[0]+zr[0])%n][(v[1]+z[1]+zr[1])%n]==6 && lat[(v[0]+z[0]+zr[0])%n][(v[1]+z[1]+zr[1])%n]==lat[v[0]][v[1]] && lat[(v[0]+z[0])%n][(v[1]+z[1])%n]==lat[(v[0]+zr[0])%n][(v[1]+zr[1])%n]){
                n2=n2+1;
            }
            if (lat[v[0]][v[1]]+lat[(v[0]+z[0])%n][(v[1]+z[1])%n]+lat[mod(v[0]-zr[0],n)][mod(v[1]-zr[1],n)]+lat[mod(v[0]+z[0]-zr[0],n)][mod(v[1]+z[1]-zr[1],n)]==6 && lat[mod(v[0]+z[0]-zr[0],n)][mod(v[1]+z[1]-zr[1],n)]==lat[v[0]][v[1]] && lat[(v[0]+z[0])%n][(v[1]+z[1])%n]==lat[mod(v[0]-zr[0],n)][mod(v[1]-zr[1],n)]){
                n2=n2+1;
            }
        }
        int x = lat[u[0]][u[1]];
        lat[u[0]][u[1]]=lat[v[0]][v[1]];
        lat[v[0]][v[1]]=x;

        if (lat[u[0]][u[1]] != 0){
            if (lat[u[0]][u[1]]+lat[(u[0]+1)%n][(u[1]+0)%n]+lat[(u[0]+0)%n][mod(u[1]-1,n)]+lat[(u[0]+1)%n][mod(u[1]-1,n)]==6 && lat[(u[0]+1)%n][mod(u[1]-1,n)]==lat[u[0]][u[1]] && lat[(u[0]+1)%n][(u[1]+0)%n]==lat[(u[0]+0)%n][mod(u[1]-1,n)]){
                n2_f=n2_f+1;
            }
            if (lat[u[0]][u[1]]+lat[(u[0]+1)%n][(u[1]+0)%n]+lat[(u[0]+0)%n][(u[1]+1)%n]+lat[(u[0]+1)%n][(u[1]+1)%n]==6 && lat[(u[0]+1)%n][(u[1]+1)%n]==lat[u[0]][u[1]] && lat[(u[0]+1)%n][(u[1]+0)%n]==lat[(u[0]+0)%n][(u[1]+1)%n]){
                n2_f=n2_f+1;
            }
            if (lat[u[0]][u[1]]+lat[mod(u[0]-1,n)][(u[1]+0)%n]+lat[(u[0]+0)%n][mod(u[1]-1,n)]+lat[mod(u[0]-1,n)][mod(u[1]-1,n)]==6 && lat[mod(u[0]-1,n)][mod(u[1]-1,n)]==lat[u[0]][u[1]] && lat[mod(u[0]-1,n)][(u[1]+0)%n]==lat[(u[0]+0)%n][mod(u[1]-1,n)]){
                n2_f=n2_f+1;
            }
            if (lat[u[0]][u[1]]+lat[mod(u[0]-1,n)][(u[1]+0)%n]+lat[(u[0]+0)%n][(u[1]+1)%n]+lat[mod(u[0]-1,n)][(u[1]+1)%n]==6 && lat[mod(u[0]-1,n)][(u[1]+1)%n]==lat[u[0]][u[1]] && lat[mod(u[0]-1,n)][(u[1]+0)%n]==lat[(u[0]+0)%n][(u[1]+1)%n]){
                n2_f=n2_f+1;
            }
        }
        if (lat[v[0]][v[1]] != 0){
            if (lat[v[0]][v[1]]+lat[(v[0]+z[0])%n][(v[1]+z[1])%n]+lat[(v[0]+zr[0])%n][(v[1]+zr[1])%n]+lat[(v[0]+z[0]+zr[0])%n][(v[1]+z[1]+zr[1])%n]==6 && lat[(v[0]+z[0]+zr[0])%n][(v[1]+z[1]+zr[1])%n]==lat[v[0]][v[1]] && lat[(v[0]+z[0])%n][(v[1]+z[1])%n]==lat[(v[0]+zr[0])%n][(v[1]+zr[1])%n]){
                n2_f=n2_f+1;
            }
            if (lat[v[0]][v[1]]+lat[(v[0]+z[0])%n][(v[1]+z[1])%n]+lat[mod(v[0]-zr[0],n)][mod(v[1]-zr[1],n)]+lat[mod(v[0]+z[0]-zr[0],n)][mod(v[1]+z[1]-zr[1],n)]==6 && lat[mod(v[0]+z[0]-zr[0],n)][mod(v[1]+z[1]-zr[1],n)]==lat[v[0]][v[1]] && lat[(v[0]+z[0])%n][(v[1]+z[1])%n]==lat[mod(v[0]-zr[0],n)][mod(v[1]-zr[1],n)]){
                n2_f=n2_f+1;
            }
        }
    }
    return n2_f - n2;
}

void Lattice::Actualizar(int etapa,double& energia, double T,double C0,double C1,double C2){
    element s0 = selec_random();
    element s1 = vecino(s0);
    int dN0 = difN0(s0,s1);
    int dN1 = difN1(s0,s1);
    int dN2 = difN2(s0,s1); 
    double dE = C0*dN0 +C1*dN1+C2*dN2;// Damos el valor de c0,c1 y c2

    uniform_real_distribution<double> real(0, 1);
    if (dE < 0 || (real(g) < exp(1.0*(-dE)/T))){
        int x = lattice[s0.x][s0.y];
        lattice[s0.x][s0.y] = lattice[s1.x][s1.y];
        lattice[s1.x][s1.y] = x;
        energia += dE;
    }
    energias[etapa] = energia;
}
//element s0, element s1, 
void Lattice::Probar(element s0, element s1,string str){
    string line;
    ifstream plot(str+".csv");
    if(plot.is_open()){
        int row=0,col;
        getline (plot,line);
        while (getline (plot,line))
        {
            stringstream ss(line);
            int val;
            col = 0;
            while(ss>> val){
                if (ss.peek() == ',')
                    ss.ignore();
                lattice[row][col] = val;
                col++;
            }
            row++;
        }
        plot.close();
    }

    //Mostrar("prueba.csv");
    cout<<"Prueba de coordenadas\n******************"<<endl;
    cout<<s0.x<<","<<s0.y<<": "<<lattice[s0.x][s0.y]<<endl;
    cout<<s1.x<<","<<s1.y<<": "<<lattice[s1.x][s1.y]<<endl;
    int dN0 = difN0(s0,s1);
    int dN1 = difN1(s0,s1);
    int dN2 = difN2(s0,s1);
    cout<< "dN0: " << dN0 << endl;
    cout<< "dN1: " << dN1 << endl;
    cout<< "dN2: " << dN2 << endl;
    cout<<"*****************"<<endl;
}

void Lattice::modelo_cristalizacion(unsigned int netapas,double T,double C0,double C1,double C2){
    double energia=0;
    if(energias == nullptr)
        energias = new double[netapas];
    int count=0;
    this->netapas = netapas;
    for(int etapa=0; etapa < netapas; etapa++){
        Actualizar(etapa,energia,T,C0,C1,C2);
    }
}

void Lattice::Animar(unsigned int netapas,double T,double C0,double C1,double C2) {
    if(ventana.OnInit() == false) {
        cout<<"could not init!"<<endl;
        //return -1;
    }
    if(ventana.LoadContent(n) == false)
    {
        cout<<"could not Load Content!"<<endl;
        //return -1;
    }
    //lat->init_ising_lattice(lat->n,l,d);
    double energia=0;
    energias = new double[netapas];
    int count=0;
    this->netapas = netapas;
    bool closed = false;
    for(int etapa=0;etapa < netapas; etapa++){
        Actualizar(etapa,energia,T,C0,C1,C2);
        if(etapa%100==0){
            if(ventana.Running) {
                //if(SDL_PollEvent(&Event)){
                while(SDL_PollEvent(&ventana.Event)) {
                    ventana.OnEvent(&ventana.Event,n);
                }
                //OnLoop();
                string str = "Etapas: "+to_string(etapa+1);
                ventana.UpdateMsg(str);
                ventana.OnRender(n, lattice,colors);
                SDL_Delay(50);
            }else{
                if(!closed){
                    ventana.Cleanup();
                    closed = true;
                }
            }

        }
    }
}

int Lattice::Mostrar(string filename) {
    
    if(ventana.OnInit() == false) {
        cout<<"could not init!"<<endl;
        return -1;
    }
    if(ventana.LoadContent(n) == false)
    {
        cout<<"could not Load Content!"<<endl;
        return -1;
    }
    string str = "Etapas: "+to_string(netapas);
    ventana.UpdateMsg(str);    
    while(ventana.Running) {
        //if(SDL_PollEvent(&Event)){
        while(SDL_PollEvent(&ventana.Event)) {
            ventana.OnEvent(&ventana.Event,n);
        }
        //OnLoop();
        ventana.OnRender(n, lattice,colors);
        SDL_Delay(500);
    }
    ventana.SaveRender(filename);
    ventana.Cleanup();
    return 0;
}

void Lattice::GuardarCsv(string str){
    //Guardar Lattice a csv
    ofstream plot("L_"+str+".csv");
    if(plot.is_open()){
        plot<<"#Lattice,n="<<n<<endl;
        for(int i=0; i < n; i+=1){
            for(int j=0; j<n;j++){
                plot<<lattice[i][j];
                    if(j<n-1)
                    plot<<",";
            }plot<<endl;
        }
        plot.close();
    }
    else cerr<<"No se pudo abrir el archivo";
    //Guardar el cambio de energia a csv
    ofstream plot2("E_"+str+".csv");
    if(plot2.is_open()){
        plot2<<"#Iter,Energia"<<endl;
        for(int i=0; i<netapas; i+=100){
            plot2<<i<< ","<< energias[i]<<endl;
        }
        plot2.close();
    }
    else cerr<<"No se pudo abrir el archivo";
}
