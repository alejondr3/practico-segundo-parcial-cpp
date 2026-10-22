// Fusion estable de listas dobles ordenadas
// C++17. Ejecutar sin argumentos para correr las pruebas.
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cctype>

class Estructura {
private:
struct Nodo { int dato; Nodo* prev; Nodo* next; };
    Nodo* head = nullptr;
    Nodo* tail = nullptr;
public:
    Estructura() = default;
    Estructura(const Estructura&) = delete;
    Estructura& operator=(const Estructura&) = delete;
    ~Estructura() { limpiar(); }

    void agregar(int valor) {
        auto& s = *this;
    Nodo* n=new Nodo{valor,s.tail,nullptr};
    if(s.tail) s.tail->next=n; else s.head=n;
    s.tail=n;
    }

    void limpiar() {
        auto& s = *this;
    while (s.head) { Nodo* n = s.head; s.head = n->next; delete n; }
    s.tail = nullptr;
    }

    std::string texto() {
        auto& s = *this;
    std::ostringstream out;
    for (Nodo* n=s.head; n; n=n->next) { if(n!=s.head) out << ' '; out << n->dato; }
    return out.str();
    }

    std::string reversa() {
        auto& s = *this;
    std::ostringstream out;
    for(Nodo* n=s.tail;n;n=n->prev) { if(n!=s.tail) out << ' '; out << n->dato; }
    return out.str();
    }

    bool validar() {
        auto& s = *this;
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

    const void* identidad(int clave) {
        auto& s = *this;
    for(Nodo* n=s.head;n;n=n->next) if(n->dato==clave) return n;
    return nullptr;
    }

    void fusionar(Estructura& a, Estructura& b) {
        auto& s = *this;
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
};

int main() {

    Estructura a,b,c;
    for(int x : {1,4,10,12}) a.agregar(x);
    for(int x : {2,3,10,15,20}) b.agregar(x);
    const void* diezA=a.identidad(10);
    c.fusionar(a,b);
    assert(c.texto()=="1 2 3 4 10 10 12 15 20");
    assert(c.reversa()=="20 15 12 10 10 4 3 2 1");
    assert(c.identidad(10)==diezA);
    assert(a.texto().empty() && b.texto().empty());
    assert(a.validar() && b.validar() && c.validar());
    std::cout << c.texto() << '\n' << c.reversa() << '\n'; c.limpiar();
    b.agregar(7); c.fusionar(a,b); assert(c.texto()=="7" && c.validar()); c.limpiar();
    c.fusionar(a,b); assert(c.validar() && c.texto().empty());
    a.agregar(2); b.agregar(2); c.fusionar(a,b); assert(c.texto()=="2 2" && c.validar()); c.limpiar();
    a.agregar(-1); c.fusionar(a,b); assert(c.texto()=="-1" && c.validar()); c.limpiar();

    std::cout << "Ejercicio 17 - pruebas correctas\n";
}
