// Ordenar pila con una unica pila auxiliar
// Unica biblioteca: iostream. C++17.
#include <iostream>
using namespace std;

void comprobar(bool condicion, const char* caso, int& fallos) {
    if (!condicion) { cout << "FALLO: " << caso << '\n'; ++fallos; }
}
struct Nodo { int dato; Nodo* next; };
struct Estructura {
    Nodo* tope = nullptr;
};
void push(Estructura& s, int x);
bool top(Estructura& s, int& dato);
bool pop(Estructura& s, int& dato);
bool vacia(Estructura& s);
void limpiar(Estructura& s);
void ordenar(Estructura& s);

void push(Estructura& s, int x) {
    s.tope=new Nodo{x,s.tope};
}

bool top(Estructura& s, int& dato) {
    if (!s.tope) return false;
    dato=s.tope->dato; return true;
}

bool pop(Estructura& s, int& dato) {
    if (!s.tope) return false;
    Nodo* n=s.tope; dato=n->dato; s.tope=n->next; delete n;
    return true;
}

bool vacia(Estructura& s) {
    return s.tope==nullptr;
}

void limpiar(Estructura& s) {
    while (s.tope) { Nodo* n=s.tope; s.tope=n->next; delete n; }
}

void ordenar(Estructura& s) {
    Estructura auxiliar;
    int actual=0, superior=0;
    while (pop(s,actual)) {
        while (top(auxiliar,superior) && superior>actual) {
            pop(auxiliar,superior); push(s,superior);
        }
        push(auxiliar,actual);
    }
    while (pop(auxiliar,actual)) push(s,actual);
}

int main() {
    int fallos=0;
    Estructura a;
    int casos[5][7]={{4,1,7,3,3,9,2},{8,0,0,0,0,0,0},
                      {-2,-2,-5,0,0,0,0},{1,2,3,0,0,0,0},{3,2,1,0,0,0,0}};
    int esperado[5][7]={{1,2,3,3,4,7,9},{8,0,0,0,0,0,0},
                         {-5,-2,-2,0,0,0,0},{1,2,3,0,0,0,0},{1,2,3,0,0,0,0}};
    int tamanos[]={7,1,4,3,3};
    ordenar(a); comprobar(vacia(a), "vacia", fallos);
    for (int c=0; c<5; ++c) {
        for (int i=0; i<tamanos[c]; ++i) push(a,casos[c][i]);
        ordenar(a);
        for (int i=0; i<tamanos[c]; ++i) {
            int dato=0; bool ok=pop(a,dato);
            comprobar(ok && dato==esperado[c][i], "orden menor arriba", fallos);
        }
        comprobar(vacia(a), "sin nodos sobrantes", fallos);
    }
    int dato=77;
    comprobar(!pop(a,dato) && dato==77, "underflow", fallos);
    comprobar(!top(a,dato), "tope vacio", fallos); limpiar(a);
    cout << "Ejercicio 31: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
