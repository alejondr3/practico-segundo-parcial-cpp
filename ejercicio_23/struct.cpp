// Intercambio fisico de nodos de lista doble
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
bool intercambiar(Estructura& s, int x, int y);

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

bool intercambiar(Estructura& s, int x, int y) {
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

int main() {

    Estructura a;
    for(int x : {1,2,3,4,5,6,7,8}) agregar(a,x);
    const void* dos=identidad(a,2); const void* siete=identidad(a,7);
    assert(intercambiar(a,2,7)); assert(texto(a)=="1 7 3 4 5 6 2 8");
    assert(identidad(a,2)==dos && identidad(a,7)==siete && validar(a));
    assert(intercambiar(a,1,8)); assert(texto(a)=="8 7 3 4 5 6 2 1" && validar(a));
    assert(intercambiar(a,4,5)); assert(texto(a)=="8 7 3 5 4 6 2 1" && validar(a));
    assert(intercambiar(a,5,4)); assert(validar(a));
    assert(intercambiar(a,5,4)); assert(texto(a)=="8 7 3 5 4 6 2 1" && validar(a));
    auto antes=texto(a); assert(!intercambiar(a,99,1)); assert(texto(a)==antes);
    assert(!intercambiar(a,1,1)); std::cout << texto(a) << '\n'; limpiar(a);
    assert(!intercambiar(a,1,2)); agregar(a,1); assert(!intercambiar(a,1,2)); limpiar(a);
    agregar(a,1); agregar(a,2); assert(intercambiar(a,2,1));
    assert(texto(a)=="2 1" && reversa(a)=="1 2" && validar(a)); limpiar(a);

    std::cout << "Ejercicio 23 - pruebas correctas\n";
}
