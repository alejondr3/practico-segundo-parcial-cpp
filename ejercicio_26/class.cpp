// Conversion infija a postfija con tokens
// Unica biblioteca: iostream. C++17.
#include <iostream>

void comprobar(bool condicion, const char* caso, int& fallos) {
    if (!condicion) { std::cout << "FALLO: " << caso << '\n'; ++fallos; }
}
bool esDigito(char c) { return c >= '0' && c <= '9'; }
bool esLetra(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool esEspacio(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}
bool igualTexto(const char* a, const char* b) {
    if (!a || !b) return a == b;
    int i = 0;
    while (a[i] && a[i] == b[i]) ++i;
    return a[i] == b[i];
}
class Estructura {
private:
    struct Nodo { char dato; Nodo* next; };
    Nodo* tope = nullptr;
public:
    Estructura() = default;
    Estructura(const Estructura&) = delete;
    Estructura& operator=(const Estructura&) = delete;
    ~Estructura() { limpiar(); }

    void push(char x) {
        auto& s = *this;
        s.tope=new Nodo{x,s.tope};
    }

    char pop() {
        auto& s = *this;
        if (!s.tope) return '\0';
        Nodo* n=s.tope; char c=n->dato;
        s.tope=n->next; delete n; return c;
    }

    void limpiar() {
        auto& s = *this;
        while(s.tope) { Nodo* n=s.tope; s.tope=n->next; delete n; }
    }

    int prioridad(char c) {
        auto& s = *this;
        (void)s;
        if(c=='^') return 3;
        if(c=='*' || c=='/' || c=='%') return 2;
        if(c=='+' || c=='-') return 1;
        return 0;
    }

    bool convertir(const char* infija, char*& salida, const char*& error) {
        auto& s = *this;
        s.limpiar(); salida=nullptr; error=nullptr;
        if (!infija) { error="Entrada nula"; return false; }
        int largo=0;
        while (infija[largo]) ++largo;
        char* temporal=new char[2*largo+1];
        int usados=0, i=0;
        bool esperaOperando=true, hayToken=false;
        while (i<largo && !error) {
            char c=infija[i];
            if (esEspacio(c)) { ++i; continue; }
            hayToken=true;
            if (esDigito(c) || esLetra(c)) {
                if (!esperaOperando) { error="Falta operador"; break; }
                int inicio=i++;
                if (esDigito(c)) {
                    while (esDigito(infija[i])) ++i;
                } else {
                    while (esLetra(infija[i]) || esDigito(infija[i])) ++i;
                }
                for (int k=inicio; k<i; ++k) temporal[usados++]=infija[k];
                temporal[usados++]=' '; esperaOperando=false;
            } else if (c=='(') {
                if (!esperaOperando) { error="Falta operador antes de ("; break; }
                s.push(c); ++i;
            } else if (c==')') {
                if (esperaOperando) { error="Falta operando antes de )"; break; }
                while (s.tope && s.tope->dato!='(') {
                    temporal[usados++]=s.pop(); temporal[usados++]=' ';
                }
                if (!s.tope) { error="Cierre sin apertura"; break; }
                s.pop(); ++i;
            } else if (s.prioridad(c)>0) {
                if (esperaOperando) { error="Falta operando; sin unarios"; break; }
                while (s.tope && s.tope->dato!='(' &&
                      (s.prioridad(s.tope->dato)>s.prioridad(c) ||
                      (s.prioridad(s.tope->dato)==s.prioridad(c) && c!='^'))) {
                    temporal[usados++]=s.pop(); temporal[usados++]=' ';
                }
                s.push(c); ++i; esperaOperando=true;
            } else error="Caracter no permitido";
        }
        if (!error && (!hayToken || esperaOperando)) error="Expresion incompleta";
        while (!error && s.tope) {
            char c=s.pop();
            if (c=='(') error="Apertura sin cierre";
            else { temporal[usados++]=c; temporal[usados++]=' '; }
        }
        s.limpiar();
        if (error) { delete[] temporal; return false; }
        temporal[usados-1]='\0'; salida=temporal;
        return true;
    }
};

int main() {
    int fallos=0;
    Estructura a;
    const char* entradas[]={"12 + x * (7 - 3)^2", "a^b^c", "10/(5-5)",
                            "foo_2 % 12 + 3*4", "a-b-c", "((42))"};
    const char* esperadas[]={"12 x 7 3 - 2 ^ * +", "a b c ^ ^", "10 5 5 - /",
                             "foo_2 12 % 3 4 * +", "a b - c -", "42"};
    for (int i=0; i<6; ++i) {
        char* salida=nullptr; const char* error=nullptr;
        bool ok=a.convertir(entradas[i],salida,error);
        comprobar(ok && igualTexto(salida,esperadas[i]), "conversion", fallos);
        if (ok) std::cout << salida << '\n';
        delete[] salida;
    }
    const char* invalidas[]={"", " ", "(a+b", "a+b)", "a+", "a b", "()",
                             "2(x)", "a**b", "12x", "a@b", "-3", nullptr};
    for (int i=0; i<13; ++i) {
        char* salida=nullptr; const char* error=nullptr;
        bool ok=a.convertir(invalidas[i],salida,error);
        comprobar(!ok && !salida && error, "expresion invalida", fallos);
        delete[] salida;
        ok=a.convertir("1+2",salida,error);
        comprobar(ok && igualTexto(salida,"1 2 +"), "reusar tras error", fallos);
        delete[] salida;
    }
    a.limpiar();
    std::cout << "Ejercicio 26: " << fallos << " fallos\n";
    return fallos==0 ? 0 : 1;
}
