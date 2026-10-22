// Intercambio fisico de nodos de lista doble
// Unica biblioteca: iostream. C++17.
#include <iostream>

void comprobar(bool condicion, const char* caso, int& fallos) {
    if (!condicion) { std::cout << "FALLO: " << caso << '\n'; ++fallos; }
}
struct Nodo { int dato; Nodo* prev; Nodo* next; };
struct Estructura {
    Nodo* head = nullptr;
    Nodo* tail = nullptr;
};
void agregar(Estructura& s, int valor);
void limpiar(Estructura& s);
bool validar(Estructura& s);
const void* identidad(Estructura& s, int clave);
bool intercambiar(Estructura& s, int x, int y);
bool coincide(Estructura& s, const int datos[], int n);
void mostrar(Estructura& s);
const void* nodoEn(Estructura& s, int posicion);
const void* enlaceEn(Estructura& s, int posicion);
bool coincideReversa(Estructura& s, const int datos[], int n);

void agregar(Estructura& s, int valor) {
    Nodo* n=new Nodo{valor,s.tail,nullptr};
    if(s.tail) s.tail->next=n; else s.head=n;
    s.tail=n;
}

void limpiar(Estructura& s) {
    while (s.head) { Nodo* n = s.head; s.head = n->next; delete n; }
    s.tail = nullptr;
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

bool coincide(Estructura& s, const int datos[], int n) {
    Nodo* actual = s.head;
    for (int i = 0; i < n; ++i) {
        if (!actual || actual->dato != datos[i]) return false;
        actual = actual->next;
    }
    return actual == nullptr;
}

void mostrar(Estructura& s) {
    for (Nodo* actual = s.head; actual; actual = actual->next)
        std::cout << actual->dato << ' ';
    std::cout << '\n';
}

const void* nodoEn(Estructura& s, int posicion) {
    Nodo* actual = s.head;
    for (int i = 0; actual && i < posicion; ++i) actual = actual->next;
    return posicion < 0 ? nullptr : actual;
}

const void* enlaceEn(Estructura& s, int posicion) {
    Nodo* actual = s.head;
    for (int i = 0; actual && i < posicion; ++i) actual = actual->next;
    return posicion < 0 || !actual ? nullptr : actual->next;
}

bool coincideReversa(Estructura& s, const int datos[], int n) {
    Nodo* actual=s.tail;
    for (int i=n-1; i>=0; --i) {
        if (!actual || actual->dato!=datos[i]) return false;
        actual=actual->prev;
    }
    return actual==nullptr;
}

void mostrarReversa(Estructura& s) {
    for (Nodo* n=s.tail; n; n=n->prev) std::cout << n->dato << ' ';
    std::cout << '\n';
}

bool leerEntero(const char* mensaje, int& valor) {
    while (true) {
        std::cout << mensaje;
        if (std::cin >> valor) return true;
        if (std::cin.eof()) return false;
        std::cin.clear();
        char c;
        while (std::cin.get(c) && c != '\n') {}
        std::cout << "Entrada invalida. Escriba un numero entero.\n";
    }
}

int main() {
    Estructura lista;
    int cantidad=0;
    do {
        if (!leerEntero("Cantidad de nodos (0 para lista vacia): ",cantidad)) {
            limpiar(lista); return 1;
        }
        if (cantidad<0) std::cout << "La cantidad no puede ser negativa.\n";
    } while (cantidad<0);

    for (int i=0; i<cantidad; ++i) {
        int valor=0;
        std::cout << "Nodo " << i+1 << " de " << cantidad << '\n';
        if (!leerEntero("Valor: ",valor)) { limpiar(lista); return 1; }
        agregar(lista,valor);
    }
    std::cout << "Lista inicial: "; mostrar(lista);

    while (true) {
        int opcion=0;
        std::cout << "\n1. Intercambiar dos nodos\n0. Salir\n";
        if (!leerEntero("Opcion: ",opcion)) { limpiar(lista); return 1; }
        if (opcion==0) break;
        if (opcion!=1) { std::cout << "Opcion invalida.\n"; continue; }
        int x=0,y=0;
        if (!leerEntero("Primera clave: ",x) ||
            !leerEntero("Segunda clave: ",y)) { limpiar(lista); return 1; }
        if (intercambiar(lista,x,y))
            std::cout << "Intercambio realizado.\n";
        else
            std::cout << "No se intercambio: claves iguales o alguna inexistente.\n";
        if (!validar(lista)) {
            std::cout << "Error interno de enlaces.\n";
            limpiar(lista); return 1;
        }
        std::cout << "Lista actual: "; mostrar(lista);
        std::cout << "Recorrido inverso: "; mostrarReversa(lista);
    }
    limpiar(lista);
    std::cout << "Memoria liberada.\n";
    return 0;
}
