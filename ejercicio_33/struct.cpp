// N pilas sobre un pool fijo con lista libre
// C++17. Ejecutar sin argumentos para correr las pruebas.
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cctype>

struct Nodo { int dato; int siguiente; };
struct Estructura {
    Nodo* pool = nullptr;
    int* topes = nullptr;
    int cantidad = 0, capacidad = 0, libre = -1;
};
void limpiar(Estructura& s);
void iniciar(Estructura& s, int n, int m);
void validarIndice(Estructura& s, int p);
bool push(Estructura& s, int p, int dato);
bool pop(Estructura& s, int p, int& dato);
bool top(Estructura& s, int p, int& dato);
bool validar(Estructura& s);

void limpiar(Estructura& s) {
    delete[] s.pool; delete[] s.topes;
    s.pool=nullptr; s.topes=nullptr; s.cantidad=0; s.capacidad=0; s.libre=-1;
}

void iniciar(Estructura& s, int n, int m) {
    if(n<=0 || m<0) throw std::invalid_argument("N debe ser positivo y M no negativo");
    if(s.pool || s.topes) throw std::logic_error("Pool ya inicializado");
    // Ambas reservas ocurren una sola vez por inicializacion.
    Nodo* nuevoPool=new Nodo[m];
    int* nuevosTopes=nullptr;
    try { nuevosTopes=new int[n]; } catch(...) { delete[] nuevoPool; throw; }
    s.pool=nuevoPool; s.topes=nuevosTopes; s.cantidad=n; s.capacidad=m;
    for(int i=0;i<n;++i) s.topes[i]=-1;
    for(int i=0;i<m;++i) s.pool[i].siguiente=(i+1<m ? i+1 : -1);
    s.libre=(m>0 ? 0 : -1);
}

void validarIndice(Estructura& s, int p) {
    if(p<0 || p>=s.cantidad) throw std::out_of_range("Indice de pila invalido");
}

bool push(Estructura& s, int p, int dato) {
    validarIndice(s,p);
    if(s.libre==-1) return false;
    int i=s.libre; s.libre=s.pool[i].siguiente;
    s.pool[i].dato=dato; s.pool[i].siguiente=s.topes[p]; s.topes[p]=i;
    return true;
}

bool pop(Estructura& s, int p, int& dato) {
    validarIndice(s,p);
    int i=s.topes[p]; if(i==-1) return false;
    dato=s.pool[i].dato; s.topes[p]=s.pool[i].siguiente;
    s.pool[i].siguiente=s.libre; s.libre=i; return true;
}

bool top(Estructura& s, int p, int& dato) {
    validarIndice(s,p);
    if(s.topes[p]==-1) return false;
    dato=s.pool[s.topes[p]].dato; return true;
}

bool validar(Estructura& s) {
    // Diagnostico sin arreglos auxiliares: cada indice debe aparecer exactamente una vez.
    for(int objetivo=0;objetivo<s.capacidad;++objetivo) {
        int apariciones=0;
        for(int cadena=-1;cadena<s.cantidad;++cadena) {
            int actual=(cadena==-1 ? s.libre : s.topes[cadena]), pasos=0;
            while(actual!=-1) {
                if(actual<0 || actual>=s.capacidad || ++pasos>s.capacidad) return false;
                if(actual==objetivo) ++apariciones;
                actual=s.pool[actual].siguiente;
            }
        }
        if(apariciones!=1) return false;
    }
    return true;
}

int main() {

    Estructura a; iniciar(a,3,8); int dato=0;
    for(int i=0;i<8;++i) assert(push(a,i%3,100+i));
    assert(validar(a)); assert(!push(a,0,999));
    assert(pop(a,1,dato) && dato==107); assert(push(a,2,999));
    assert(top(a,2,dato) && dato==999); assert(validar(a));
    assert(pop(a,2,dato) && dato==999);
    assert(pop(a,2,dato) && dato==105); assert(pop(a,2,dato) && dato==102);
    assert(!pop(a,2,dato));
    assert(pop(a,0,dato) && dato==106); assert(pop(a,0,dato) && dato==103); assert(pop(a,0,dato) && dato==100);
    assert(pop(a,1,dato) && dato==104); assert(pop(a,1,dato) && dato==101);
    assert(!pop(a,1,dato)); assert(validar(a));
    for(int i=0;i<8;++i) assert(push(a,0,i));
    assert(!push(a,1,99));
    for(int i=7;i>=0;--i) assert(pop(a,0,dato) && dato==i);
    bool error=false; try { push(a,3,1); } catch(const std::out_of_range&) { error=true; } assert(error);
    error=false; try { pop(a,-1,dato); } catch(const std::out_of_range&) { error=true; } assert(error);
    assert(validar(a)); limpiar(a);
    iniciar(a,1,0); assert(!push(a,0,1)); assert(!pop(a,0,dato)); limpiar(a);
    error=false; try { iniciar(a,0,8); } catch(const std::invalid_argument&) { error=true; } assert(error);

    std::cout << "Ejercicio 33 - pruebas correctas\n";
}
