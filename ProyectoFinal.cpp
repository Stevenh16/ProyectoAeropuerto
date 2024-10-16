#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<conio.h>
#include<limits>
#include<numeric>
using namespace std;
typedef struct r{
	char origen[20];
	char destino[20];
	float distancia;
	float precio;
}ruta;
ruta crear_ruta(const string& o, string& de, float d, float p){
	ruta r;
	strncpy(r.origen,o.c_str(),sizeof(r.origen));
	strncpy(r.destino,de.c_str(),sizeof(r.destino));
	r.distancia=d;
	r.precio=p;
	return r;
}
void escritura(ruta r){
	ofstream f;
	f.open("Rutas.txt",ios::in|ios::out|ios::binary);
	if(f){
		f.seekp(0,ios::end);
		f.write(reinterpret_cast<char*>(&r),sizeof(ruta));
	}
	f.close();
}
void imprimir_ruta(const ruta& r){
	cout<<r.origen<<" -> "<<r.destino<<" ("<<r.distancia<<" km,  $"<<r.precio<<")"<<endl;
}
void lectura(){
	ifstream f;
	ruta r;
	f.open("Rutas.txt",ios::in|ios::out|ios::binary);
	if(f.good()){
		f.seekg(0,ios::end);
		int n=f.tellg()/sizeof(ruta);
		f.seekg(0);
		for(int i=0;i<n;i++){
			f.seekg(sizeof(ruta)*i);
			f.read(reinterpret_cast<char*>(&r),sizeof(ruta));
			imprimir_ruta(r);
		}
	}
	f.close();
}
void lectura_datoscsv(string &nombrea){
	ifstream f;
	int sw=0;
	float di,pr;
	string li,o,de,tilin;
	f.open(nombrea,ios::in|ios::out);
	if(f){
		while(getline(f,li,';')){
			switch(sw){
				case 1:{
					o=tilin;
					de=li;
					break;
				}
				case 2:{
					di=stof(li);
           			break;
				}
				case 3:{
              		pr = stof(li);
                	escritura(crear_ruta(o, de, di, pr));
                	sw=0;
					break;
				}
				default:{
					break;
				}
			}
			if(sw==0){
				size_t inicio=li.find('\n');
				if(inicio!=string::npos){
					tilin=li.substr(inicio+1);
				}else{
					tilin=li;
				}
			}
			sw++;
		}
		cout<<"Archivo leido con exito."<<endl;
	}else{
		cout<<"Error: Archivo no leido, verifique el nombre o el formato."<<endl;
	}
	f.close();
}
void agregar_ruta(){
	int sw, va;
	string o,de;
	float di,p;
	do{
		system("cls");
		cout<<"Ruta solo ida(0) o ida y vuelta(1): ";
		if(cin>>sw){
		}else{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			sw=-1;
		}
	}while(sw!=0 && sw!=1);
	cin.ignore();
	system("cls");
	cout<<"Ruta nueva."<<endl<<"Ingrese origen: ";
	getline(cin,o);
	cout<<"Ingrese destino: ";
	getline(cin,de);
	do{
		system("cls");
		cout<<"Ingrese la distancia: ";
		if(cin>>di){
			va=0;
		}else{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			va=1;
		}
	}while(va==1);
	do{
		system("cls");
		cout<<"Ingrese el precio: ";
		if(cin>>p){
			va=0;
		}else{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			va=1;
		}
	}while(va==1);
	escritura(crear_ruta(o,de,di,p));
	if(sw==1){
		escritura(crear_ruta(de,o,di,p));
	}
}
vector<int> busqueda_recursiva(const string& o, const string& de, int inicio, int &fin, int &sw, const int &tr){
	ruta r;
	vector<int> posiciones;
	ifstream f("Rutas.txt",ios::in|ios::binary);
	if(f.is_open()){	
		if(inicio<fin){
			f.seekg(tr*inicio);
			f.read(reinterpret_cast<char*>(&r),tr);
			if((sw==0&&strcmp(r.origen,o.c_str())==0)||(sw==1&&strcmp(r.destino,de.c_str())==0)||(sw==2&&strcmp(r.origen,o.c_str())==0&&strcmp(r.destino,de.c_str())==0)){
        		posiciones.push_back(inicio);
       		}	
			vector<int> posiciones2=busqueda_recursiva(o,de,inicio+1,fin,sw,tr);		
			posiciones.insert(posiciones.end(),posiciones2.begin(),posiciones2.end());
		}
	}
	f.close();
	return posiciones;
}
void llamar_busqueda(){
	ruta r;
	string o,de="Hola";
	int sw, n=3;
	const int tr=sizeof(ruta);
	do{
		system("cls");
		cout<<"Busqueda por origen(0) o por destino(1) o ambos(2): ";
		if(cin>>sw){
		}else{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			sw=-1;
		}
	}while(sw!=0 && sw!=1 && sw!=2);
	cin.ignore();
	if(sw==0 || sw==2){
		cout<<"Ingresa el origen: ";
		getline(cin,o);
	}
	if(sw==1 || sw==2){
		cout<<"Ingresa el destino: ";
		getline(cin,de);
	}
	ifstream f("Rutas.txt",ios::in|ios::binary);
	if(f.is_open()){
		f.seekg(0,ios::end);
		n=f.tellg()/tr;
		f.seekg(0,ios::beg);
		vector<int>posiciones=busqueda_recursiva(o,de,0,n,sw,tr);
		while(!posiciones.empty()){
			n=posiciones.back();
			posiciones.pop_back();
			f.seekg(tr*n);
			f.read(reinterpret_cast<char*>(&r),sizeof(ruta));
			cout<<r.origen<<" "<<r.destino<<" "<<r.distancia<<" "<<r.precio<<"  Pos:"<<n<<endl;
		}
		f.close();
	}
}
void editar(){
	int pos,va;
	ruta r,tilin;
	string o,de;
	float di,p;
	fstream f;
	llamar_busqueda();
	f.open("Rutas.txt",ios::in|ios::out|ios::binary);
	do{
		cout<<"Ingrese la posicion de la ruta a editar: ";
		if(cin>>pos){
			va=0;
		}else{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			va=1;
		}
		system("cls");
	}while(va==1);
	cin.ignore();
	if(f.is_open()){
		f.seekp(pos*sizeof(ruta));
		f.read(reinterpret_cast<char*>(&r),sizeof(ruta));
		imprimir_ruta(r);
		f.seekg(pos*sizeof(ruta));
		f.seekp(pos*sizeof(ruta));
		cout<<"Origen:";
		getline(cin,o);
		cout<<"Destino:";
		getline(cin,de);
		do{
			system("cls");
			cout<<"Ingrese la distancia: ";
			if(cin>>di){
				va=0;
			}else{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				va=1;
			}
		}while(va==1);
		do{
			system("cls");
			cout<<"Ingrese el precio: ";
			if(cin>>p){
				va=0;
			}else{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				va=1;
			}
		}while(va==1);
		tilin=crear_ruta(o,de,di,p);
		f.write(reinterpret_cast<char*>(&tilin),sizeof(ruta));
	}
	f.close();
}
void eliminar(){
	ruta r;
	int pos,va;
	llamar_busqueda();
	do{
		cout<<"Ingrese la posicion de la ruta a eliminar: ";
		if(cin>>pos){
			va=0;
		}else{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			va=1;
		}
	}while(va==1);
	ifstream original("Rutas.txt",ios::in|ios::binary);
	ofstream temporal("TempRutas.txt",ios::out|ios::binary);
	if(original.is_open() && temporal.is_open()){
		original.seekg(0,ios::end);
		int n=original.tellg()/sizeof(ruta);
		original.seekg(0,ios::beg);
		for(int i=0;i<n;i++){
			if(i!=pos){
				original.seekg(sizeof(ruta)*i);
				original.read(reinterpret_cast<char*>(&r),sizeof(ruta));
				temporal.write(reinterpret_cast<char*>(&r),sizeof(ruta));
			}
		}
		original.close();
		temporal.close();
		remove("Rutas.txt");
		rename("TempRutas.txt","Rutas.txt");
		cout<<"Ruta eliminada con exito."<<endl;
	}
}
void Merge(int inicio,int medio,int fin, int &sw ){
	int k=inicio;
	int elementosizq=medio-inicio+1;
	int elementosder=fin-medio;
	vector<ruta> izquierda(elementosizq);
	vector<ruta> derecha(elementosder);
	fstream f("Rutas.txt",ios::in|ios::out|ios::binary);
    f.seekg(sizeof(ruta) * inicio);
    f.read(reinterpret_cast<char*>(&izquierda[0]), sizeof(ruta) * elementosizq);
    f.seekg(sizeof(ruta) * (medio + 1));
    f.read(reinterpret_cast<char*>(&derecha[0]), sizeof(ruta) * elementosder);
	int i=0,j=0;
	while(i<elementosizq&&j<elementosder){
		if(sw==0){
			if(izquierda[i].distancia<=derecha[j].distancia){
				f.seekp(sizeof(ruta)*k);
				f.write(reinterpret_cast<char*>(&izquierda[i]),sizeof(ruta));
				i++;
			}else{
				f.seekp(sizeof(ruta)*k);
				f.write(reinterpret_cast<char*>(&derecha[j]),sizeof(ruta));
				j ++;
			}
		}else{
			if(izquierda[i].precio<=derecha[j].precio){
				f.seekp(sizeof(ruta)*k);
				f.write(reinterpret_cast<char*>(&izquierda[i]),sizeof(ruta));
				i++;
			}else{
				f.seekp(sizeof(ruta)*k);
				f.write(reinterpret_cast<char*>(&derecha[j]),sizeof(ruta));
				j ++;
			}
		}
		k++;
	}
	while(j<elementosder){
		f.seekp(sizeof(ruta) * k);
        f.write(reinterpret_cast<char*>(&derecha[j]), sizeof(ruta));
        j++;
		k++;
	}
	while(i<elementosizq){
		f.seekp(sizeof(ruta) * k);
        f.write(reinterpret_cast<char*>(&izquierda[i]), sizeof(ruta));
        i++;
		k++;
	}
	f.close();
}
void Mergesort(int &sw,int inicio, int fin){
	if(inicio<fin){
		int mitad=inicio+(fin-inicio)/2;
		Mergesort(sw,inicio,mitad);
		Mergesort(sw,mitad+1,fin);
		Merge(inicio,mitad,fin,sw);
	}
}
void llamar_merge(){
	int sw;
	ifstream f("Rutas.txt",ios::in|ios::binary);
	f.seekg(0,ios::end);
	int n=f.tellg()/sizeof(ruta);
	f.close();
	do{
		system("cls");
		cout<<"Desea ordenar las rutas por distancia(0) o por precio(1): ";
		if(cin>>sw){
		}else{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			sw=-1;
		}
	}while(sw!=0 && sw!=1);
	Mergesort(sw,0,n-1);
}
bool yaVisitada(const vector<ruta>& visitadas, const string& ciudad) {
    return find_if(visitadas.begin(), visitadas.end(), [&ciudad](const ruta& r) {
        return string(r.origen) == ciudad || string(r.destino) == ciudad;
    }) != visitadas.end();
}
void Rutasposibles(const string& o, const string& paso, const string& de, const int& tr,vector<ruta>& visitadas, int &sw,float &dt,float &pt,float& minDistancia, float& minPrecio, vector<ruta>& mejorRuta, vector<vector<ruta>> & todasLasRutas){
    ifstream f("Rutas.txt",ios::in|ios::binary);
    ruta r;
    while (f.read(reinterpret_cast<char*>(&r),tr)) {
        if(string(r.origen)==paso&&!yaVisitada(visitadas,r.destino)){
            visitadas.push_back(r);
            dt+=r.distancia;
            pt+=r.precio;
			if(string(r.destino)==de){
				if((sw==0&&dt<minDistancia)||(sw==1&&pt<minPrecio)){
					minDistancia=dt;
					minPrecio=pt;
					mejorRuta=visitadas;
					todasLasRutas.push_back(visitadas);
				}
            }else{
                Rutasposibles(o, r.destino, de, tr,visitadas,sw,dt,pt,minDistancia,minPrecio,mejorRuta, todasLasRutas);
            }
            dt-=r.distancia;
            pt-=r.precio;
            visitadas.pop_back();
        }
    }
    f.close();
}
void llamar_rutas_posibles(){
    const int tr=sizeof(ruta);
    int sw;
    float dt=0;
    float pt=0;
    string o, de;
    cin.ignore();
    cout << "Rutas posibles." << endl << "Ingrese el origen: ";
    getline(cin, o);
    cout << "Ingrese el destino: ";
    getline(cin, de);
    do{
    	system("cls");
		cout<<"Desea ordenar las rutas por distancia(0) o por precio(1): ";
		if(cin>>sw){
		}else{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			sw=-1;
		}
	}while(sw!=0 && sw!=1);
    vector<ruta> visitadas;
    vector<ruta> mejorRuta;
    vector<vector<ruta>> todasLasRutas;
    float minDistancia=numeric_limits<float>::max();
    float minPrecio=numeric_limits<float>::max();
    Rutasposibles(o, o, de, tr,visitadas,sw,dt,pt,minDistancia,minPrecio,mejorRuta,todasLasRutas);
    if(sw==0){
    	sort(todasLasRutas.begin(), todasLasRutas.end(), [](const auto& a, const auto& b) {
            return accumulate(a.begin(), a.end(), 0.0, [](auto sum, const auto& ruta) {
                return sum + ruta.distancia;
            }) < accumulate(b.begin(), b.end(), 0.0, [](auto sum, const auto& ruta) {
                return sum + ruta.distancia;
            });
        });
	}else{
		sort(todasLasRutas.begin(), todasLasRutas.end(), [](const auto& a, const auto& b) {
            return accumulate(a.begin(), a.end(), 0.0, [](auto sum, const auto& ruta) {
                return sum + ruta.precio;
            }) < accumulate(b.begin(), b.end(), 0.0, [](auto sum, const auto& ruta) {
                return sum + ruta.precio;
            });
        });
	}
	for (const auto& ruta : todasLasRutas) {
        for (const auto& paso : ruta) {
            imprimir_ruta(paso);
        }
        cout << "Suma de distancia total: " << accumulate(ruta.begin(), ruta.end(), 0.0, [](auto sum, const auto& paso) {
            return sum + paso.distancia;
        }) << " km" << endl;
        cout << "Suma de precios total: $" << accumulate(ruta.begin(), ruta.end(), 0.0, [](auto sum, const auto& paso) {
            return sum + paso.precio;
        }) << endl;
        cout << "---------------------------------" << endl;
    }
}
char sel(int o,int s){
	if(o==s)
		return (char)175;
	else
		return ' ';
}
char sel2(int o,int s){
	if(o==s)
		return (char)174;
	else
		return ' ';
}
int main(){
	string nombrea;
	int op;
	int n=0;
	do{
		system("color a");
		system("cls");
		cout<<(char)201;
		for(int i=0;i<33;i++)
			cout<<(char)205;
		cout<<(char)187;
		cout<<endl;
		cout<<(char)186<<sel(n,0)<<"1. Leer Archivo.csv.           "<<sel2(n,0)<<(char)186<<endl;
		cout<<(char)186<<sel(n,1)<<"2. Imprimir rutas.             "<<sel2(n,1)<<(char)186<<endl;
		cout<<(char)186<<sel(n,2)<<"3. Agregar ruta.               "<<sel2(n,2)<<(char)186<<endl;
		cout<<(char)186<<sel(n,3)<<"4. Buscar ruta.                "<<sel2(n,3)<<(char)186<<endl;
		cout<<(char)186<<sel(n,4)<<"5. Editar ruta.                "<<sel2(n,4)<<(char)186<<endl;
		cout<<(char)186<<sel(n,5)<<"6. Eliminar ruta.              "<<sel2(n,5)<<(char)186<<endl;
		cout<<(char)186<<sel(n,6)<<"7. Ordenar ruta.               "<<sel2(n,6)<<(char)186<<endl;
		cout<<(char)186<<sel(n,7)<<"8. Busqueda de rutas posibles. "<<sel2(n,7)<<(char)186<<endl;
		cout<<(char)186<<sel(n,8)<<"0. Salir.                      "<<sel2(n,8)<<(char)186<<endl;
		cout<<(char)200;
		for(int i=0;i<33;i++)
			cout<<(char)205;
		cout<<(char)188;
		cout<<endl;
		op=getch();
		if(op==80){
			if(n==8){
				n=0;
			}else{
				n++;
			}
		}else{
			if(op==72){
				if(n==0){
					n=8;
				}else{
					n--;
				}
			}else{
				if(op==13){
					switch(n){
						case 0:{
							system("cls");
							cout<<"Ingrese el nombre del archivo y el formato (ejemplo: archivo.csv): ";
							getline(cin,nombrea);
							lectura_datoscsv(nombrea);
							getch();
							break;
						}
						case 1:{
							system("cls");
							cout<<"Destino  Origen  Distancia  Precio "<<endl;
							lectura();
							getch();
							break;
						}
						case 2:{
							system("cls");
							agregar_ruta();
							cout<<"Ruta(s) agregada con exito"<<endl;
							getch();
							break;
						}
						case 3:{
							system("cls");
							llamar_busqueda();
							getch();
							break;
						}
						case 4:{
							system("cls");
							editar();
							getch();
							break;
						}
						case 5:{
							system("cls");
							eliminar();
							getch();
							break;
						}
						case 6:{
							system("cls");
							llamar_merge();
							getch();
							break;
						}
						case 7:{
							system("cls");
							llamar_rutas_posibles();
							getch();
							break;
						}
						case 8:{
							system("cls");
							cout<<"Gracias por usar el programa!!"<<endl;
							return 123;
							break;
						}
					}
				}
			}
		}
	}while(op!=0);
	return 0;
}