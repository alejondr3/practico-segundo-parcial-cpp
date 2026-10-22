// Fusion estable de listas dobles ordenadas
// C++17. Ejecutar sin argumentos para correr las pruebas.
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cctype>

struct Nodo { int dato; Nodo* prev; Nodo* next; };
struct Estructura {
    Nodo* head = nullptr;
    Nodo* tail = nullptr;
};
void agregar(Estructura& s, int valor);
void limpiar(Estructura& s);
std::string texto(Estructura& s);
std::string reversa(Estructura& s);
bool validar(Estructura& s);
const void* identidad(Estructura& s, int clave);
void fusionar(Estructura& s, Estructura& a, Estructura& b);

void agregar(Estructura& s, int valor) {
    Nodo* n=new Nodo{valor,s.tail,nullptr};
    if(s.tail) s.tail->next=n; else s.head=n;
    s.tail=n;
}

void limpiar(Estructura& s) {
    while (s.head) { Nodo* n = s.head; s.head = n->next; delete n; }
    s.tail = nullptr;
}

std::string texto(Estructura& s) {
    std::ostringstream out;
    for (Nodo* n=s.head; n; n=n->next) { if(n!=s.head) out << ' '; out << n->dato; }
    return out.str();
}

std::string reversa(Estructura& s) {
    std::ostringstream out;
    for(Nodo* n=s.tail;n;n=n->prev) { if(n!=s.tail) out << ' '; out << n->dato; }
    return out.str();
}

bool validar(Estructura& s) {
    if((s.head==nullptr)!=(s.tail==nullptr)) return false;
    if(s.head && s.head->prev) return false;
    if(s.tail && s.tail->next) return false;
    // Floyd detecta ciclos antes de los recorridos de verificacion.
    Nodo *lento=s.head, *rapido=s.head;
    while(rapido && rapido->next) { lento=lento->next; rapido=rapido->next->next; if(lento==rapido) return false; }
    Nodo* anterior=nullptr;
    for(Nodo* n=s.head;n;n=n->next) { if(n->prev!=anterior) return false; anterior=n; }
    if(anterior!=s.tail) return false;
    Nodo* siguiente=nullptr;
    for(Nodo* n=s.tail;n;n=n->prev) { if(n->next!=siguiente) return false; siguiente=n; }
    return siguiente==s.head;
}

const void* identidad(Estructura& s, int clave) {
    for(Nodo* n=s.head;n;n=n->next) if(n->dato==clave) return n;
    return nullptr;
}

void fusionar(Estructura& s, Estructura& a, Estructura& b) {
    if(&s==&a || &s==&b || &a==&b) throw std::invalid_argument("Las tres listas deben ser distintas");
    if(s.head) throw std::invalid_argument("El destino debe estar vacio");
    while(a.head || b.head) {
        // En igualdad se elige A: conserva la estabilidad.
        Estructura& origen=(!b.head || (a.head && a.head->dato<=b.head->dato)) ? a : b;
        Nodo* n=origen.head; origen.head=n->next;
        if(origen.head) origen.head->prev=nullptr; else origen.tail=nullptr;
        n->prev=s.tail; n->next=nullptr;
        if(s.tail) s.tail->next=n; else s.head=n;
        s.tail=n;
    }
}

int main() {

    Estructura a,b,c;
    for(int x : {1,4,10,12}) agregar(a,x);
    for(int x : {2,3,10,15,20}) agregar(b,x);
    const void* diezA=identidad(a,10);
    fusionar(c,a,b);
    assert(texto(c)=="1 2 3 4 10 10 12 15 20");
    assert(reversa(c)=="20 15 12 10 10 4 3 2 1");
    assert(identidad(c,10)==diezA);
    assert(texto(a).empty() && texto(b).empty());
    assert(validar(a) && validar(b) && validar(c));
    std::cout << texto(c) << '\n' << reversa(c) << '\n'; limpiar(c);
    agregar(b,7); fusionar(c,a,b); assert(texto(c)=="7" && validar(c)); limpiar(c);
    fusionar(c,a,b); assert(validar(c) && texto(c).empty());
    agregar(a,2); agregar(b,2); fusionar(c,a,b); assert(texto(c)=="2 2" && validar(c)); limpiar(c);
    agregar(a,-1); fusionar(c,a,b); assert(texto(c)=="-1" && validar(c)); limpiar(c);

    std::cout << "Ejercicio 17 - pruebas correctas\n";
}
