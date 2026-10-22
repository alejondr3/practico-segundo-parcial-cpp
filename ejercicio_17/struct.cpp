// Fusion estable de listas dobles ordenadas
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
bool fusionar(Estructura& s, Estructura& a, Estructura& b);
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

bool fusionar(Estructura& s, Estructura& a, Estructura& b) {
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

int main() {
    int fallos=0;
    Estructura a,b,c;
    int va[]={1,4,10,12}, vb[]={2,3,10,15,20};
    for (int i=0; i<4; ++i) agregar(a,va[i]);
    for (int i=0; i<5; ++i) agregar(b,vb[i]);
    const void* diez=identidad(a,10);
    bool ok=fusionar(c,a,b);
    int esperado[]={1,2,3,4,10,10,12,15,20};
    comprobar(ok && coincide(c,esperado,9) && coincideReversa(c,esperado,9),
               "fusion bidireccional", fallos);
    comprobar(identidad(c,10)==diez, "estabilidad", fallos);
    comprobar(coincide(a,nullptr,0) && coincide(b,nullptr,0), "entradas vacias", fallos);
    comprobar(validar(a) && validar(b) && validar(c), "enlaces", fallos);
    mostrar(c); limpiar(c);
    comprobar(fusionar(c,a,b) && validar(c), "ambas vacias", fallos);
    agregar(b,7); comprobar(fusionar(c,a,b), "A vacia", fallos);
    int siete[]={7}; comprobar(coincide(c,siete,1), "un nodo", fallos); limpiar(c);
    agregar(a,-1); comprobar(fusionar(c,a,b), "B vacia", fallos); limpiar(c);
    agregar(a,2); agregar(b,2); comprobar(fusionar(c,a,b), "iguales", fallos);
    int dos[]={2,2}; comprobar(coincide(c,dos,2) && validar(c), "repetidos", fallos);
    comprobar(!fusionar(c,a,b), "destino ocupado", fallos); limpiar(c);
    comprobar(!fusionar(a,a,b), "listas distintas", fallos);
    limpiar(a); limpiar(b);
    std::cout << "Ejercicio 17: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
