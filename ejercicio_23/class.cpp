// Intercambio fisico de nodos de lista doble
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

    bool intercambiar(int x, int y) {
        auto& s = *this;
    if(x==y) return false;
    Nodo *a=s.head, *b=s.head;
    while(a && a->dato!=x) a=a->next;
    while(b && b->dato!=y) b=b->next;
    if(!a || !b) return false;
    if(b->next==a) { Nodo* temporal=a; a=b; b=temporal; }
    Nodo *ap=a->prev, *an=a->next, *bp=b->prev, *bn=b->next;
    if(a->next==b) {
        if(ap) ap->next=b; else s.head=b;
        if(bn) bn->prev=a; else s.tail=a;
        b->prev=ap; b->next=a; a->prev=b; a->next=bn;
    } else {
        if(ap) ap->next=b; else s.head=b;
        if(an) an->prev=b; else s.tail=b;
        if(bp) bp->next=a; else s.head=a;
        if(bn) bn->prev=a; else s.tail=a;
        a->prev=bp; a->next=bn; b->prev=ap; b->next=an;
    }
    return true;
    }
};

int main() {

    Estructura a;
    for(int x : {1,2,3,4,5,6,7,8}) a.agregar(x);
    const void* dos=a.identidad(2); const void* siete=a.identidad(7);
    assert(a.intercambiar(2,7)); assert(a.texto()=="1 7 3 4 5 6 2 8");
    assert(a.identidad(2)==dos && a.identidad(7)==siete && a.validar());
    assert(a.intercambiar(1,8)); assert(a.texto()=="8 7 3 4 5 6 2 1" && a.validar());
    assert(a.intercambiar(4,5)); assert(a.texto()=="8 7 3 5 4 6 2 1" && a.validar());
    assert(a.intercambiar(5,4)); assert(a.validar());
    assert(a.intercambiar(5,4)); assert(a.texto()=="8 7 3 5 4 6 2 1" && a.validar());
    auto antes=a.texto(); assert(!a.intercambiar(99,1)); assert(a.texto()==antes);
    assert(!a.intercambiar(1,1)); std::cout << a.texto() << '\n'; a.limpiar();
    assert(!a.intercambiar(1,2)); a.agregar(1); assert(!a.intercambiar(1,2)); a.limpiar();
    a.agregar(1); a.agregar(2); assert(a.intercambiar(2,1));
    assert(a.texto()=="2 1" && a.reversa()=="1 2" && a.validar()); a.limpiar();

    std::cout << "Ejercicio 23 - pruebas correctas\n";
}
