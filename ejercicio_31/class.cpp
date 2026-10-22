// Ordenar pila con una unica pila auxiliar
// Unica biblioteca: iostream. C++17.
#include <iostream>
using namespace std;

void comprobar(bool condicion, const char* caso, int& fallos) {
    if (!condicion) { cout << "FALLO: " << caso << '\n'; ++fallos; }
}
class Estructura {
private:
    struct Nodo { int dato; Nodo* next; };
    Nodo* tope = nullptr;
public:
    Estructura() = default;
    Estructura(const Estructura&) = delete;
    Estructura& operator=(const Estructura&) = delete;
    ~Estructura() { limpiar(); }

    void push(int x) {
        auto& s = *this;
        s.tope=new Nodo{x,s.tope};
    }

    bool top(int& dato) {
        auto& s = *this;
        if (!s.tope) return false;
        dato=s.tope->dato; return true;
    }

    bool pop(int& dato) {
        auto& s = *this;
        if (!s.tope) return false;
        Nodo* n=s.tope; dato=n->dato; s.tope=n->next; delete n;
        return true;
    }

    bool vacia() {
        auto& s = *this;
        return s.tope==nullptr;
    }

    void limpiar() {
        auto& s = *this;
        while (s.tope) { Nodo* n=s.tope; s.tope=n->next; delete n; }
    }

    void ordenar() {
        auto& s = *this;
        Estructura auxiliar;
        int actual=0, superior=0;
        while (s.pop(actual)) {
            while (auxiliar.top(superior) && superior>actual) {
                auxiliar.pop(superior); s.push(superior);
            }
            auxiliar.push(actual);
        }
        while (auxiliar.pop(actual)) s.push(actual);
    }
};

int main() {
    int fallos=0;
    Estructura a;
    int casos[5][7]={{4,1,7,3,3,9,2},{8,0,0,0,0,0,0},
                      {-2,-2,-5,0,0,0,0},{1,2,3,0,0,0,0},{3,2,1,0,0,0,0}};
    int esperado[5][7]={{1,2,3,3,4,7,9},{8,0,0,0,0,0,0},
                         {-5,-2,-2,0,0,0,0},{1,2,3,0,0,0,0},{1,2,3,0,0,0,0}};
    int tamanos[]={7,1,4,3,3};
    a.ordenar(); comprobar(a.vacia(), "vacia", fallos);
    for (int c=0; c<5; ++c) {
        for (int i=0; i<tamanos[c]; ++i) a.push(casos[c][i]);
        a.ordenar();
        for (int i=0; i<tamanos[c]; ++i) {
            int dato=0; bool ok=a.pop(dato);
            comprobar(ok && dato==esperado[c][i], "orden menor arriba", fallos);
        }
        comprobar(a.vacia(), "sin nodos sobrantes", fallos);
    }
    int dato=77;
    comprobar(!a.pop(dato) && dato==77, "underflow", fallos);
    comprobar(!a.top(dato), "tope vacio", fallos); a.limpiar();
    cout << "Ejercicio 31: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
