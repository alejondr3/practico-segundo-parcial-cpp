// Intercambio fisico de nodos de lista doble
// Unica biblioteca: iostream. C++17.
#include <iostream>
using namespace std;

void comprobar(bool condicion, const char* caso, int& fallos) {
    if (!condicion) { cout << "FALLO: " << caso << '\n'; ++fallos; }
}
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

    bool coincide(const int datos[], int n) {
        auto& s = *this;
        Nodo* actual = s.head;
        for (int i = 0; i < n; ++i) {
            if (!actual || actual->dato != datos[i]) return false;
            actual = actual->next;
        }
        return actual == nullptr;
    }

    void mostrar() {
        auto& s = *this;
        for (Nodo* actual = s.head; actual; actual = actual->next)
            cout << actual->dato << ' ';
        cout << '\n';
    }

    const void* nodoEn(int posicion) {
        auto& s = *this;
        Nodo* actual = s.head;
        for (int i = 0; actual && i < posicion; ++i) actual = actual->next;
        return posicion < 0 ? nullptr : actual;
    }

    const void* enlaceEn(int posicion) {
        auto& s = *this;
        Nodo* actual = s.head;
        for (int i = 0; actual && i < posicion; ++i) actual = actual->next;
        return posicion < 0 || !actual ? nullptr : actual->next;
    }

    bool coincideReversa(const int datos[], int n) {
        auto& s = *this;
        Nodo* actual=s.tail;
        for (int i=n-1; i>=0; --i) {
            if (!actual || actual->dato!=datos[i]) return false;
            actual=actual->prev;
        }
        return actual==nullptr;
    }
    void mostrarReversa() {
        for (Nodo* n=tail; n; n=n->prev) cout << n->dato << ' ';
        cout << '\n';
    }
};

bool leerEntero(const char* mensaje, int& valor) {
    while (true) {
        cout << mensaje;
        if (cin >> valor) return true;
        if (cin.eof()) return false;
        cin.clear();
        char c;
        while (cin.get(c) && c != '\n') {}
        cout << "Entrada invalida. Escriba un numero entero.\n";
    }
}

int main() {
    Estructura lista;
    int cantidad=0;
    do {
        if (!leerEntero("Cantidad de nodos (0 para lista vacia): ",cantidad)) {
            lista.limpiar(); return 1;
        }
        if (cantidad<0) cout << "La cantidad no puede ser negativa.\n";
    } while (cantidad<0);

    for (int i=0; i<cantidad; ++i) {
        int valor=0;
        cout << "Nodo " << i+1 << " de " << cantidad << '\n';
        if (!leerEntero("Valor: ",valor)) { lista.limpiar(); return 1; }
        lista.agregar(valor);
    }
    cout << "Lista inicial: "; lista.mostrar();

    while (true) {
        int opcion=0;
        cout << "\n1. Intercambiar dos nodos\n0. Salir\n";
        if (!leerEntero("Opcion: ",opcion)) { lista.limpiar(); return 1; }
        if (opcion==0) break;
        if (opcion!=1) { cout << "Opcion invalida.\n"; continue; }
        int x=0,y=0;
        if (!leerEntero("Primera clave: ",x) ||
            !leerEntero("Segunda clave: ",y)) { lista.limpiar(); return 1; }
        if (lista.intercambiar(x,y))
            cout << "Intercambio realizado.\n";
        else
            cout << "No se intercambio: claves iguales o alguna inexistente.\n";
        if (!lista.validar()) {
            cout << "Error interno de enlaces.\n";
            lista.limpiar(); return 1;
        }
        cout << "Lista actual: "; lista.mostrar();
        cout << "Recorrido inverso: "; lista.mostrarReversa();
    }
    lista.limpiar();
    cout << "Memoria liberada.\n";
    return 0;
}
