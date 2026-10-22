// Particion estable alrededor de un pivote
// C++17. Ejecutar sin argumentos para correr las pruebas.
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cctype>

struct Nodo { int dato; Nodo* next; };
struct Estructura {
    Nodo* head = nullptr;
    Nodo* tail = nullptr;
};
void agregar(Estructura& s, int valor);
void limpiar(Estructura& s);
std::string texto(Estructura& s);
std::string huella(Estructura& s);
void particionar(Estructura& s, int x);

void agregar(Estructura& s, int valor) {
    Nodo* n = new Nodo{valor, nullptr};
    if (s.tail) s.tail->next = n; else s.head = n;
    s.tail = n;
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

std::string huella(Estructura& s) {
    std::ostringstream out;
    for (Nodo* n=s.head; n; n=n->next) out << n << ':' << n->next << ':' << n->dato << ';';
    out << s.tail; return out.str();
}

void particionar(Estructura& s, int x) {
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

int main() {

    Estructura a;
    for(int x : {3,5,8,5,10,2,1,5,7}) agregar(a,x);
    particionar(a,5); assert(texto(a)=="3 2 1 5 5 5 8 10 7");
    std::cout << texto(a) << '\n'; limpiar(a);
    particionar(a,5); assert(texto(a).empty());
    for(int x : {1,2,3}) agregar(a,x);
    auto antes=huella(a); particionar(a,5); assert(huella(a)==antes); limpiar(a);
    for(int x : {5,5,5}) agregar(a,x);
    antes=huella(a); particionar(a,5); assert(huella(a)==antes); limpiar(a);
    for(int x : {8,9,10}) agregar(a,x);
    antes=huella(a); particionar(a,5); assert(huella(a)==antes); limpiar(a);
    agregar(a,5); particionar(a,5); assert(texto(a)=="5"); limpiar(a);

    std::cout << "Ejercicio 9 - pruebas correctas\n";
}
