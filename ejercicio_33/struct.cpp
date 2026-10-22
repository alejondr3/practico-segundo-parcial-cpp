// N pilas sobre un pool fijo con lista libre
// Unica biblioteca: iostream. C++17.
#include <iostream>
using namespace std;

void comprobar(bool condicion, const char* caso, int& fallos) {
    if (!condicion) { cout << "FALLO: " << caso << '\n'; ++fallos; }
}
struct Nodo { int dato; int siguiente; };
struct Estructura {
    Nodo* pool = nullptr;
    int* topes = nullptr;
    int cantidad = 0, capacidad = 0, libre = -1;
};
void limpiar(Estructura& s);
bool iniciar(Estructura& s, int n, int m);
bool indiceValido(Estructura& s, int p);
bool push(Estructura& s, int p, int dato);
bool pop(Estructura& s, int p, int& dato);
bool top(Estructura& s, int p, int& dato);
bool validar(Estructura& s);

void limpiar(Estructura& s) {
    delete[] s.pool; delete[] s.topes;
    s.pool=nullptr; s.topes=nullptr; s.cantidad=0; s.capacidad=0; s.libre=-1;
}

bool iniciar(Estructura& s, int n, int m) {
    if (n<=0 || m<0 || s.pool || s.topes) return false;
    Nodo* nuevoPool=nullptr;
    int* nuevosTopes=nullptr;
    try {
        nuevoPool=new Nodo[m];
        nuevosTopes=new int[n];
    } catch (...) {
        delete[] nuevoPool; delete[] nuevosTopes; return false;
    }
    s.pool=nuevoPool; s.topes=nuevosTopes;
    s.cantidad=n; s.capacidad=m;
    for (int p=0; p<n; ++p) s.topes[p]=-1;
    for (int i=0; i<m; ++i) s.pool[i].siguiente=(i+1<m ? i+1 : -1);
    s.libre=(m>0 ? 0 : -1); return true;
}

bool indiceValido(Estructura& s, int p) {
    return p>=0 && p<s.cantidad;
}

bool push(Estructura& s, int p, int dato) {
    if (!indiceValido(s,p)) return false;
    if(s.libre==-1) return false;
    int i=s.libre; s.libre=s.pool[i].siguiente;
    s.pool[i].dato=dato; s.pool[i].siguiente=s.topes[p]; s.topes[p]=i;
    return true;
}

bool pop(Estructura& s, int p, int& dato) {
    if (!indiceValido(s,p)) return false;
    int i=s.topes[p]; if(i==-1) return false;
    dato=s.pool[i].dato; s.topes[p]=s.pool[i].siguiente;
    s.pool[i].siguiente=s.libre; s.libre=i; return true;
}

bool top(Estructura& s, int p, int& dato) {
    if (!indiceValido(s,p)) return false;
    if(s.topes[p]==-1) return false;
    dato=s.pool[s.topes[p]].dato; return true;
}

bool validar(Estructura& s) {
    if (s.cantidad<0 || s.capacidad<0) return false;
    if (!s.topes) return !s.pool && s.cantidad==0 && s.capacidad==0 && s.libre==-1;
    if (!s.pool || s.cantidad==0) return false;
    // Comprobar incluso las cadenas de un pool de capacidad cero.
    for (int cadena=-1; cadena<s.cantidad; ++cadena) {
        int actual=(cadena==-1 ? s.libre : s.topes[cadena]), pasos=0;
        while (actual!=-1) {
            if (actual<0 || actual>=s.capacidad || ++pasos>s.capacidad) return false;
            actual=s.pool[actual].siguiente;
        }
    }
    for (int objetivo=0; objetivo<s.capacidad; ++objetivo) {
        int apariciones=0;
        for (int cadena=-1; cadena<s.cantidad; ++cadena) {
            int actual=(cadena==-1 ? s.libre : s.topes[cadena]);
            while (actual!=-1) {
                if (actual==objetivo) ++apariciones;
                actual=s.pool[actual].siguiente;
            }
        }
        if (apariciones!=1) return false;
    }
    return true;
}

int main() {
    int fallos=0;
    Estructura a;
    comprobar(iniciar(a,3,8), "iniciar", fallos);
    for (int i=0; i<8; ++i)
        comprobar(push(a,i%3,100+i), "llenado", fallos);
    comprobar(validar(a) && !push(a,0,999), "overflow", fallos);
    int dato=0;
    bool ok=pop(a,1,dato);
    comprobar(ok && dato==107, "retirar de pila 1", fallos);
    comprobar(push(a,2,999), "reutilizar nodo", fallos);
    ok=top(a,2,dato); comprobar(ok && dato==999, "consultar tope", fallos);
    int p2[]={999,105,102};
    for (int i=0; i<3; ++i) {
        ok=pop(a,2,dato); comprobar(ok && dato==p2[i], "LIFO pila 2", fallos);
    }
    comprobar(!pop(a,2,dato), "underflow", fallos);
    int p0[]={106,103,100}, p1[]={104,101};
    for (int i=0; i<3; ++i) {
        ok=pop(a,0,dato); comprobar(ok && dato==p0[i], "LIFO pila 0", fallos);
    }
    for (int i=0; i<2; ++i) {
        ok=pop(a,1,dato); comprobar(ok && dato==p1[i], "LIFO pila 1", fallos);
    }
    comprobar(validar(a), "todos los nodos conservados", fallos);
    for (int i=0; i<8; ++i) comprobar(push(a,0,i), "una pila usa todo", fallos);
    comprobar(!push(a,1,99), "pool lleno con pila vacia", fallos);
    for (int i=7; i>=0; --i) {
        ok=pop(a,0,dato); comprobar(ok && dato==i, "vaciar", fallos);
    }
    dato=55;
    comprobar(!push(a,3,1) && !pop(a,-1,dato) && dato==55,
               "indices invalidos", fallos);
    comprobar(!iniciar(a,3,8), "doble inicializacion", fallos);
    limpiar(a); limpiar(a);
    comprobar(iniciar(a,1,0) && validar(a), "capacidad cero", fallos);
    comprobar(!push(a,0,1) && !pop(a,0,dato), "pool cero", fallos);
    limpiar(a);
    comprobar(!iniciar(a,0,8) && !iniciar(a,1,-1), "dimensiones invalidas", fallos);
    cout << "Ejercicio 33: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
