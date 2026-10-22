// Ordenar pila con una unica pila auxiliar
// C++17. Ejecutar sin argumentos para correr las pruebas.
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cctype>

struct Nodo { int dato; Nodo* next; };
struct Estructura {
    Nodo* tope = nullptr;
};
void push(Estructura& s, int x);
bool vacia(Estructura& s);
int top(Estructura& s);
int pop(Estructura& s);
void limpiar(Estructura& s);
void ordenar(Estructura& s);

void push(Estructura& s, int x) {
    s.tope=new Nodo{x,s.tope};
}

bool vacia(Estructura& s) {
    return s.tope==nullptr;
}

int top(Estructura& s) {
    if(!s.tope) throw std::underflow_error("Pila vacia");
    return s.tope->dato;
}

int pop(Estructura& s) {
    if(!s.tope) throw std::underflow_error("Pila vacia");
    Nodo* n=s.tope; int x=n->dato; s.tope=n->next; delete n; return x;
}

void limpiar(Estructura& s) {
    while(s.tope) { Nodo* n=s.tope; s.tope=n->next; delete n; }
}

void ordenar(Estructura& s) {
    Estructura auxiliar;
    // La auxiliar queda con el mayor en el tope.
    while(!vacia(s)) {
        int actual=pop(s);
        while(!vacia(auxiliar) && top(auxiliar)>actual) push(s,pop(auxiliar));
        push(auxiliar,actual);
    }
    while(!vacia(auxiliar)) push(s,pop(auxiliar));
}

int main() {

    Estructura a;
    for(int x : {4,1,7,3,3,9,2}) push(a,x);
    ordenar(a);
    for(int esperado : {1,2,3,3,4,7,9}) { int obtenido=pop(a); assert(obtenido==esperado); std::cout << obtenido << ' '; }
    std::cout << '\n'; assert(vacia(a)); ordenar(a);
    push(a,8); ordenar(a); assert(pop(a)==8);
    for(int x : {-2,-2,-5,0}) push(a,x);
    ordenar(a); for(int x : {-5,-2,-2,0}) assert(pop(a)==x);
    for(int x : {1,2,3}) push(a,x);
    ordenar(a); for(int x : {1,2,3}) assert(pop(a)==x);
    for(int x : {3,2,1}) push(a,x);
    ordenar(a); for(int x : {1,2,3}) assert(pop(a)==x);
    bool error=false; try { pop(a); } catch(const std::underflow_error&) { error=true; } assert(error);
    limpiar(a);

    std::cout << "Ejercicio 31 - pruebas correctas\n";
}
