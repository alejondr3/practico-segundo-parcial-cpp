// Palindromo restaurando la lista
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
    Nodo* head = nullptr;
    Nodo* tail = nullptr;
public:
    Estructura() = default;
    Estructura(const Estructura&) = delete;
    Estructura& operator=(const Estructura&) = delete;
    ~Estructura() { limpiar(); }

    void agregar(int valor) {
        auto& s = *this;
    Nodo* n = new Nodo{valor, nullptr};
    if (s.tail) s.tail->next = n; else s.head = n;
    s.tail = n;
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

    std::string huella() {
        auto& s = *this;
    std::ostringstream out;
    for (Nodo* n=s.head; n; n=n->next) out << n << ':' << n->next << ':' << n->dato << ';';
    out << s.tail; return out.str();
    }

    Nodo* invertir(Nodo* inicio) {
        auto& s = *this;
    (void)s;
    Nodo* anterior=nullptr;
    while(inicio) { Nodo* siguiente=inicio->next; inicio->next=anterior; anterior=inicio; inicio=siguiente; }
    return anterior;
    }

    bool palindromo() {
        auto& s = *this;
    if(!s.head || !s.head->next) return true;
    Nodo *lento=s.head, *rapido=s.head;
    while(rapido->next && rapido->next->next) { lento=lento->next; rapido=rapido->next->next; }
    Nodo* segunda=s.invertir(lento->next);
    bool resultado=true;
    for(Nodo *a=s.head, *b=segunda; b; a=a->next,b=b->next)
        if(a->dato!=b->dato) resultado=false;
    // Nunca salir antes de restaurar, incluso si hubo una diferencia.
    lento->next=s.invertir(segunda);
    return resultado;
    }
};

int main() {

    Estructura a;
    assert(a.palindromo());
    for(int x : {1,2,3,2,1}) a.agregar(x);
    auto h=a.huella(); assert(a.palindromo()); assert(h==a.huella()); a.limpiar();
    for(int x : {1,2,2,1}) a.agregar(x);
    h=a.huella(); assert(a.palindromo()); assert(h==a.huella()); a.limpiar();
    for(int x : {1,2,3,4}) a.agregar(x);
    h=a.huella(); assert(!a.palindromo()); assert(h==a.huella()); a.limpiar();
    a.agregar(7); h=a.huella(); assert(a.palindromo()); assert(h==a.huella()); a.limpiar();
    a.agregar(1); a.agregar(2); h=a.huella(); assert(!a.palindromo()); assert(h==a.huella()); a.limpiar();
    for(int x : {9,9,9}) a.agregar(x);
    h=a.huella(); assert(a.palindromo()); assert(h==a.huella()); a.limpiar();

    std::cout << "Ejercicio 11 - pruebas correctas\n";
}
