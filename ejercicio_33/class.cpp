// N pilas sobre un pool fijo con lista libre
// Unica biblioteca: iostream. C++17.
#include <iostream>

void comprobar(bool condicion, const char* caso, int& fallos) {
    if (!condicion) { std::cout << "FALLO: " << caso << '\n'; ++fallos; }
}
class Estructura {
private:
    struct Nodo { int dato; int siguiente; };
    Nodo* pool = nullptr;
    int* topes = nullptr;
    int cantidad = 0, capacidad = 0, libre = -1;
public:
    Estructura() = default;
    Estructura(const Estructura&) = delete;
    Estructura& operator=(const Estructura&) = delete;
    ~Estructura() { limpiar(); }

    void limpiar() {
        auto& s = *this;
        delete[] s.pool; delete[] s.topes;
        s.pool=nullptr; s.topes=nullptr; s.cantidad=0; s.capacidad=0; s.libre=-1;
    }

    bool iniciar(int n, int m) {
        auto& s = *this;
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

    bool indiceValido(int p) {
        auto& s = *this;
        return p>=0 && p<s.cantidad;
    }

    bool push(int p, int dato) {
        auto& s = *this;
        if (!s.indiceValido(p)) return false;
        if(s.libre==-1) return false;
        int i=s.libre; s.libre=s.pool[i].siguiente;
        s.pool[i].dato=dato; s.pool[i].siguiente=s.topes[p]; s.topes[p]=i;
        return true;
    }

    bool pop(int p, int& dato) {
        auto& s = *this;
        if (!s.indiceValido(p)) return false;
        int i=s.topes[p]; if(i==-1) return false;
        dato=s.pool[i].dato; s.topes[p]=s.pool[i].siguiente;
        s.pool[i].siguiente=s.libre; s.libre=i; return true;
    }

    bool top(int p, int& dato) {
        auto& s = *this;
        if (!s.indiceValido(p)) return false;
        if(s.topes[p]==-1) return false;
        dato=s.pool[s.topes[p]].dato; return true;
    }

    bool validar() {
        auto& s = *this;
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
};

int main() {
    int fallos=0;
    Estructura a;
    comprobar(a.iniciar(3,8), "iniciar", fallos);
    for (int i=0; i<8; ++i)
        comprobar(a.push(i%3,100+i), "llenado", fallos);
    comprobar(a.validar() && !a.push(0,999), "overflow", fallos);
    int dato=0;
    bool ok=a.pop(1,dato);
    comprobar(ok && dato==107, "retirar de pila 1", fallos);
    comprobar(a.push(2,999), "reutilizar nodo", fallos);
    ok=a.top(2,dato); comprobar(ok && dato==999, "consultar tope", fallos);
    int p2[]={999,105,102};
    for (int i=0; i<3; ++i) {
        ok=a.pop(2,dato); comprobar(ok && dato==p2[i], "LIFO pila 2", fallos);
    }
    comprobar(!a.pop(2,dato), "underflow", fallos);
    int p0[]={106,103,100}, p1[]={104,101};
    for (int i=0; i<3; ++i) {
        ok=a.pop(0,dato); comprobar(ok && dato==p0[i], "LIFO pila 0", fallos);
    }
    for (int i=0; i<2; ++i) {
        ok=a.pop(1,dato); comprobar(ok && dato==p1[i], "LIFO pila 1", fallos);
    }
    comprobar(a.validar(), "todos los nodos conservados", fallos);
    for (int i=0; i<8; ++i) comprobar(a.push(0,i), "una pila usa todo", fallos);
    comprobar(!a.push(1,99), "pool lleno con pila vacia", fallos);
    for (int i=7; i>=0; --i) {
        ok=a.pop(0,dato); comprobar(ok && dato==i, "vaciar", fallos);
    }
    dato=55;
    comprobar(!a.push(3,1) && !a.pop(-1,dato) && dato==55,
               "indices invalidos", fallos);
    comprobar(!a.iniciar(3,8), "doble inicializacion", fallos);
    a.limpiar(); a.limpiar();
    comprobar(a.iniciar(1,0) && a.validar(), "capacidad cero", fallos);
    comprobar(!a.push(0,1) && !a.pop(0,dato), "pool cero", fallos);
    a.limpiar();
    comprobar(!a.iniciar(0,8) && !a.iniciar(1,-1), "dimensiones invalidas", fallos);
    std::cout << "Ejercicio 33: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
