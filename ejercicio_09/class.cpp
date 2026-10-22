// Particion estable alrededor de un pivote
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
    int datos[] = {3,5,8,5,10,2,1,5,7};
    for (int i=0; i<9; ++i) a.agregar(datos[i]);
    const void* originales[9];
    for (int i=0; i<9; ++i) originales[i]=a.nodoEn(i);
    a.particionar(5);
    int esperado[] = {3,2,1,5,5,5,8,10,7};
    int orden[] = {0,5,6,1,3,7,2,4,8};
    comprobar(a.coincide(esperado,9) && a.validar(), "particion", fallos);
    for (int i=0; i<9; ++i)
        comprobar(a.nodoEn(i)==originales[orden[i]], "estabilidad", fallos);
    a.mostrar(); a.limpiar(); a.particionar(5);
    comprobar(a.validar() && a.coincide(nullptr,0), "vacia", fallos);
    for (int grupo=0; grupo<3; ++grupo) {
        int x = grupo==0 ? 2 : (grupo==1 ? 5 : 8);
        for (int i=0; i<3; ++i) a.agregar(x);
        const void* ids[3];
        for (int i=0; i<3; ++i) ids[i]=a.nodoEn(i);
        a.particionar(5);
        for (int i=0; i<3; ++i)
            comprobar(a.nodoEn(i)==ids[i], "grupo unico", fallos);
        comprobar(a.validar(), "enlaces", fallos); a.limpiar();
    }
    a.agregar(5); a.particionar(5);
    int uno[]={5}; comprobar(a.coincide(uno,1), "un nodo", fallos);
    a.limpiar();
    cout << "Ejercicio 9: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
