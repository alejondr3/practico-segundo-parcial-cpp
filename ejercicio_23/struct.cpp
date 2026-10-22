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

int main() {
    int fallos=0;
    Estructura a;
    for (int i=1; i<=8; ++i) agregar(a,i);
    const void* dos=identidad(a,2);
    const void* siete=identidad(a,7);
    bool ok=intercambiar(a,2,7);
    int e1[]={1,7,3,4,5,6,2,8};
    comprobar(ok && coincide(a,e1,8) && coincideReversa(a,e1,8), "separados", fallos);
    comprobar(identidad(a,2)==dos && identidad(a,7)==siete, "mismos nodos", fallos);
    comprobar(validar(a), "enlaces", fallos); mostrar(a);
    ok=intercambiar(a,1,8);
    int e2[]={8,7,3,4,5,6,2,1};
    comprobar(ok && coincide(a,e2,8) && validar(a), "extremos", fallos);
    ok=intercambiar(a,4,5);
    int e3[]={8,7,3,5,4,6,2,1};
    comprobar(ok && coincide(a,e3,8) && validar(a), "adyacentes", fallos);
    ok=intercambiar(a,4,5);
    comprobar(ok && coincide(a,e2,8) && validar(a), "adyacencia inversa", fallos);
    comprobar(!intercambiar(a,99,1) && coincide(a,e2,8), "clave ausente", fallos);
    comprobar(!intercambiar(a,1,1), "claves iguales", fallos); limpiar(a);
    comprobar(!intercambiar(a,1,2), "lista vacia", fallos);
    agregar(a,1); comprobar(!intercambiar(a,1,2), "un nodo", fallos);
    agregar(a,2); ok=intercambiar(a,2,1);
    int e4[]={2,1};
    comprobar(ok && coincide(a,e4,2) && coincideReversa(a,e4,2) && validar(a),
               "dos nodos", fallos); limpiar(a);
    std::cout << "Ejercicio 23: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
