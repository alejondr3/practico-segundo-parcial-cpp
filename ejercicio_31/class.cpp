// Ordenar pila con una unica pila auxiliar
// C++17. Ejecutar sin argumentos para correr las pruebas.
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cctype>

class Estructura {
private:
struct Nodo { int dato; Nodo* next; };
    Nodo* tope = nullptr;
public:
    Estructura() = default;
    Estructura(const Estructura&) = delete;
    Estructura& operator=(const Estructura&) = delete;
    ~Estructura() { limpiar(); }

    void push(int x) {
        auto& s = *this;
    s.tope=new Nodo{x,s.tope};
    }

    bool vacia() {
        auto& s = *this;
    return s.tope==nullptr;
    }

    int top() {
        auto& s = *this;
    if(!s.tope) throw std::underflow_error("Pila vacia");
    return s.tope->dato;
    }

    int pop() {
        auto& s = *this;
    if(!s.tope) throw std::underflow_error("Pila vacia");
    Nodo* n=s.tope; int x=n->dato; s.tope=n->next; delete n; return x;
    }

    void limpiar() {
        auto& s = *this;
    while(s.tope) { Nodo* n=s.tope; s.tope=n->next; delete n; }
    }

    void ordenar() {
        auto& s = *this;
    Estructura auxiliar;
    // La auxiliar queda con el mayor en el tope.
    while(!s.vacia()) {
        int actual=s.pop();
        while(!auxiliar.vacia() && auxiliar.top()>actual) s.push(auxiliar.pop());
        auxiliar.push(actual);
    }
    while(!auxiliar.vacia()) s.push(auxiliar.pop());
    }
};

int main() {

    Estructura a;
    for(int x : {4,1,7,3,3,9,2}) a.push(x);
    a.ordenar();
    for(int esperado : {1,2,3,3,4,7,9}) { int obtenido=a.pop(); assert(obtenido==esperado); std::cout << obtenido << ' '; }
    std::cout << '\n'; assert(a.vacia()); a.ordenar();
    a.push(8); a.ordenar(); assert(a.pop()==8);
    for(int x : {-2,-2,-5,0}) a.push(x);
    a.ordenar(); for(int x : {-5,-2,-2,0}) assert(a.pop()==x);
    for(int x : {1,2,3}) a.push(x);
    a.ordenar(); for(int x : {1,2,3}) assert(a.pop()==x);
    for(int x : {3,2,1}) a.push(x);
    a.ordenar(); for(int x : {1,2,3}) assert(a.pop()==x);
    bool error=false; try { a.pop(); } catch(const std::underflow_error&) { error=true; } assert(error);
    a.limpiar();

    std::cout << "Ejercicio 31 - pruebas correctas\n";
}
