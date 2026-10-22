// Particion estable alrededor de un pivote
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

    void particionar(int x) {
        auto& s = *this;
    Nodo *menor=nullptr, *finMenor=nullptr, *igual=nullptr, *finIgual=nullptr;
    Nodo *mayor=nullptr, *finMayor=nullptr;
    while (s.head) {
        Nodo* n=s.head; s.head=n->next; n->next=nullptr;
        Nodo **inicio, **fin;
        if(n->dato<x) { inicio=&menor; fin=&finMenor; }
        else if(n->dato==x) { inicio=&igual; fin=&finIgual; }
        else { inicio=&mayor; fin=&finMayor; }
        if(*fin) (*fin)->next=n; else *inicio=n;
        *fin=n;
    }
    s.tail=nullptr;
    // Concatenar solo los grupos no vacios.
    if(menor) { s.head=menor; s.tail=finMenor; }
    if(igual) { if(s.tail) s.tail->next=igual; else s.head=igual; s.tail=finIgual; }
    if(mayor) { if(s.tail) s.tail->next=mayor; else s.head=mayor; s.tail=finMayor; }
    }
};

int main() {

    Estructura a;
    for(int x : {3,5,8,5,10,2,1,5,7}) a.agregar(x);
    a.particionar(5); assert(a.texto()=="3 2 1 5 5 5 8 10 7");
    std::cout << a.texto() << '\n'; a.limpiar();
    a.particionar(5); assert(a.texto().empty());
    for(int x : {1,2,3}) a.agregar(x);
    auto antes=a.huella(); a.particionar(5); assert(a.huella()==antes); a.limpiar();
    for(int x : {5,5,5}) a.agregar(x);
    antes=a.huella(); a.particionar(5); assert(a.huella()==antes); a.limpiar();
    for(int x : {8,9,10}) a.agregar(x);
    antes=a.huella(); a.particionar(5); assert(a.huella()==antes); a.limpiar();
    a.agregar(5); a.particionar(5); assert(a.texto()=="5"); a.limpiar();

    std::cout << "Ejercicio 9 - pruebas correctas\n";
}
