// Fusion estable de listas dobles ordenadas
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

    bool fusionar(Estructura& a, Estructura& b) {
        auto& s = *this;
        if(&s==&a || &s==&b || &a==&b) return false;
        if(s.head) return false;
        while(a.head || b.head) {
            // En igualdad se elige A: conserva la estabilidad.
            Estructura& origen=(!b.head || (a.head && a.head->dato<=b.head->dato)) ? a : b;
            Nodo* n=origen.head; origen.head=n->next;
            if(origen.head) origen.head->prev=nullptr; else origen.tail=nullptr;
            n->prev=s.tail; n->next=nullptr;
            if(s.tail) s.tail->next=n; else s.head=n;
            s.tail=n;
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
};

int main() {
    int fallos=0;
    Estructura a,b,c;
    int va[]={1,4,10,12}, vb[]={2,3,10,15,20};
    for (int i=0; i<4; ++i) a.agregar(va[i]);
    for (int i=0; i<5; ++i) b.agregar(vb[i]);
    const void* diez=a.identidad(10);
    bool ok=c.fusionar(a,b);
    int esperado[]={1,2,3,4,10,10,12,15,20};
    comprobar(ok && c.coincide(esperado,9) && c.coincideReversa(esperado,9),
               "fusion bidireccional", fallos);
    comprobar(c.identidad(10)==diez, "estabilidad", fallos);
    comprobar(a.coincide(nullptr,0) && b.coincide(nullptr,0), "entradas vacias", fallos);
    comprobar(a.validar() && b.validar() && c.validar(), "enlaces", fallos);
    c.mostrar(); c.limpiar();
    comprobar(c.fusionar(a,b) && c.validar(), "ambas vacias", fallos);
    b.agregar(7); comprobar(c.fusionar(a,b), "A vacia", fallos);
    int siete[]={7}; comprobar(c.coincide(siete,1), "un nodo", fallos); c.limpiar();
    a.agregar(-1); comprobar(c.fusionar(a,b), "B vacia", fallos); c.limpiar();
    a.agregar(2); b.agregar(2); comprobar(c.fusionar(a,b), "iguales", fallos);
    int dos[]={2,2}; comprobar(c.coincide(dos,2) && c.validar(), "repetidos", fallos);
    comprobar(!c.fusionar(a,b), "destino ocupado", fallos); c.limpiar();
    comprobar(!a.fusionar(a,b), "listas distintas", fallos);
    a.limpiar(); b.limpiar();
    cout << "Ejercicio 17: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
