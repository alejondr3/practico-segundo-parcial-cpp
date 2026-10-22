// Palindromo restaurando la lista
// Unica biblioteca: iostream. C++17.
#include <iostream>
using namespace std;

void comprobar(bool condicion, const char* caso, int& fallos) {
    if (!condicion) { cout << "FALLO: " << caso << '\n'; ++fallos; }
}
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

    bool validar() {
        auto& s = *this;
        Nodo *lento=s.head, *rapido=s.head;
        while (rapido && rapido->next) {
            lento=lento->next; rapido=rapido->next->next;
            if (lento==rapido) return false;
        }
        Nodo* ultimo=nullptr;
        for (Nodo* n=s.head; n; n=n->next) ultimo=n;
        return ultimo==s.tail && ((s.head==nullptr)==(s.tail==nullptr));
    }
};

int main() {
    int fallos=0;
    Estructura a;
    int casos[7][5]={{1,2,3,2,1},{1,2,2,1,0},{1,2,3,4,0},
                     {7,0,0,0,0},{0,0,0,0,0},{1,2,0,0,0},{9,9,9,0,0}};
    int tamanos[]={5,4,4,1,0,2,3};
    bool respuestas[]={true,true,false,true,true,false,true};
    for (int c=0; c<7; ++c) {
        for (int i=0; i<tamanos[c]; ++i) a.agregar(casos[c][i]);
        const void* ids[5]; const void* enlaces[5];
        for (int i=0; i<tamanos[c]; ++i) {
            ids[i]=a.nodoEn(i); enlaces[i]=a.enlaceEn(i);
        }
        bool resultado=a.palindromo();
        comprobar(resultado==respuestas[c], "palindromo", fallos);
        comprobar(a.coincide(casos[c],tamanos[c]) && a.validar(), "restaurar", fallos);
        for (int i=0; i<tamanos[c]; ++i)
            comprobar(a.nodoEn(i)==ids[i] && a.enlaceEn(i)==enlaces[i],
                       "identidad y enlaces", fallos);
        a.limpiar();
    }
    cout << "Ejercicio 11: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
