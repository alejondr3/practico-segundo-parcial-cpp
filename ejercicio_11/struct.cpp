// Palindromo restaurando la lista
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
Nodo* invertir(Estructura& s, Nodo* inicio);
bool palindromo(Estructura& s);

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

Nodo* invertir(Estructura& s, Nodo* inicio) {
    (void)s;
    Nodo* anterior=nullptr;
    while(inicio) { Nodo* siguiente=inicio->next; inicio->next=anterior; anterior=inicio; inicio=siguiente; }
    return anterior;
}

bool palindromo(Estructura& s) {
    if(!s.head || !s.head->next) return true;
    Nodo *lento=s.head, *rapido=s.head;
    while(rapido->next && rapido->next->next) { lento=lento->next; rapido=rapido->next->next; }
    Nodo* segunda=invertir(s,lento->next);
    bool resultado=true;
    for(Nodo *a=s.head, *b=segunda; b; a=a->next,b=b->next)
        if(a->dato!=b->dato) resultado=false;
    // Nunca salir antes de restaurar, incluso si hubo una diferencia.
    lento->next=invertir(s,segunda);
    return resultado;
}

int main() {

    Estructura a;
    assert(palindromo(a));
    for(int x : {1,2,3,2,1}) agregar(a,x);
    auto h=huella(a); assert(palindromo(a)); assert(h==huella(a)); limpiar(a);
    for(int x : {1,2,2,1}) agregar(a,x);
    h=huella(a); assert(palindromo(a)); assert(h==huella(a)); limpiar(a);
    for(int x : {1,2,3,4}) agregar(a,x);
    h=huella(a); assert(!palindromo(a)); assert(h==huella(a)); limpiar(a);
    agregar(a,7); h=huella(a); assert(palindromo(a)); assert(h==huella(a)); limpiar(a);
    agregar(a,1); agregar(a,2); h=huella(a); assert(!palindromo(a)); assert(h==huella(a)); limpiar(a);
    for(int x : {9,9,9}) agregar(a,x);
    h=huella(a); assert(palindromo(a)); assert(h==huella(a)); limpiar(a);

    std::cout << "Ejercicio 11 - pruebas correctas\n";
}
