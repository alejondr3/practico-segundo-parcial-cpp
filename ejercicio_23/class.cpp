// Intercambio fisico de nodos de lista doble
// Unica biblioteca: iostream. C++17.
#include <iostream>

void comprobar(bool condicion, const char* caso, int& fallos) {
    if (!condicion) { std::cout << "FALLO: " << caso << '\n'; ++fallos; }
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
            std::cout << actual->dato << ' ';
        std::cout << '\n';
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
};

int main() {
    int fallos=0;
    Estructura a;
    for (int i=1; i<=8; ++i) a.agregar(i);
    const void* dos=a.identidad(2);
    const void* siete=a.identidad(7);
    bool ok=a.intercambiar(2,7);
    int e1[]={1,7,3,4,5,6,2,8};
    comprobar(ok && a.coincide(e1,8) && a.coincideReversa(e1,8), "separados", fallos);
    comprobar(a.identidad(2)==dos && a.identidad(7)==siete, "mismos nodos", fallos);
    comprobar(a.validar(), "enlaces", fallos); a.mostrar();
    ok=a.intercambiar(1,8);
    int e2[]={8,7,3,4,5,6,2,1};
    comprobar(ok && a.coincide(e2,8) && a.validar(), "extremos", fallos);
    ok=a.intercambiar(4,5);
    int e3[]={8,7,3,5,4,6,2,1};
    comprobar(ok && a.coincide(e3,8) && a.validar(), "adyacentes", fallos);
    ok=a.intercambiar(4,5);
    comprobar(ok && a.coincide(e2,8) && a.validar(), "adyacencia inversa", fallos);
    comprobar(!a.intercambiar(99,1) && a.coincide(e2,8), "clave ausente", fallos);
    comprobar(!a.intercambiar(1,1), "claves iguales", fallos); a.limpiar();
    comprobar(!a.intercambiar(1,2), "lista vacia", fallos);
    a.agregar(1); comprobar(!a.intercambiar(1,2), "un nodo", fallos);
    a.agregar(2); ok=a.intercambiar(2,1);
    int e4[]={2,1};
    comprobar(ok && a.coincide(e4,2) && a.coincideReversa(e4,2) && a.validar(),
               "dos nodos", fallos); a.limpiar();
    std::cout << "Ejercicio 23: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
