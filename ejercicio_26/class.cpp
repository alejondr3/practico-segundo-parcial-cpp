// Conversion infija a postfija con tokens
// C++17. Ejecutar sin argumentos para correr las pruebas.
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cctype>

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
    if(!s.tope) throw std::underflow_error("Pila vacia");
    Nodo* n=s.tope; char x=n->dato; s.tope=n->next; delete n; return x;
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

    std::string convertir(const std::string& infija) {
        auto& s = *this;
    s.limpiar();
    std::ostringstream salida;
    bool esperaOperando=true, hayToken=false;
    try {
        for(std::size_t i=0;i<infija.size();) {
            unsigned char c=static_cast<unsigned char>(infija[i]);
            if(std::isspace(c)) { ++i; continue; }
            hayToken=true;
            if(std::isdigit(c) || std::isalpha(c) || c=='_') {
                if(!esperaOperando) throw std::invalid_argument("Falta operador entre operandos");
                std::size_t inicio=i++;
                if(std::isdigit(c)) {
                    while(i<infija.size() && std::isdigit(static_cast<unsigned char>(infija[i]))) ++i;
                } else {
                    while(i<infija.size() && (std::isalnum(static_cast<unsigned char>(infija[i])) || infija[i]=='_')) ++i;
                }
                salida << infija.substr(inicio,i-inicio) << ' '; esperaOperando=false;
            } else if(c=='(') {
                if(!esperaOperando) throw std::invalid_argument("Falta operador antes de parentesis");
                s.push('('); ++i;
            } else if(c==')') {
                if(esperaOperando) throw std::invalid_argument("Parentesis vacio o falta operando");
                while(s.tope && s.tope->dato!='(') salida << s.pop() << ' ';
                if(!s.tope) throw std::invalid_argument("Parentesis de cierre sin apertura");
                s.pop(); ++i; esperaOperando=false;
            } else if(s.prioridad(static_cast<char>(c))) {
                if(esperaOperando) throw std::invalid_argument("Falta operando; operadores unarios no incluidos");
                while(s.tope && s.tope->dato!='(' &&
                     (s.prioridad(s.tope->dato)>s.prioridad(static_cast<char>(c)) ||
                     (s.prioridad(s.tope->dato)==s.prioridad(static_cast<char>(c)) && c!='^')))
                    salida << s.pop() << ' ';
                s.push(static_cast<char>(c)); ++i; esperaOperando=true;
            } else throw std::invalid_argument("Caracter no permitido");
        }
        if(!hayToken || esperaOperando) throw std::invalid_argument("Expresion vacia o incompleta");
        while(s.tope) {
            char c=s.pop();
            if(c=='(') throw std::invalid_argument("Parentesis de apertura sin cierre");
            salida << c << ' ';
        }
    } catch(...) { s.limpiar(); throw; }
    std::string resultado=salida.str(); resultado.pop_back(); return resultado;
    }
};

int main() {

    Estructura a;
    assert(a.convertir("12 + x * (7 - 3)^2")=="12 x 7 3 - 2 ^ * +");
    assert(a.convertir("a^b^c")=="a b c ^ ^");
    assert(a.convertir("10/(5-5)")=="10 5 5 - /");
    assert(a.convertir("foo_2 % 12 + 3*4")=="foo_2 12 % 3 4 * +");
    assert(a.convertir("a-b-c")=="a b - c -");
    assert(a.convertir("((42))")=="42");
    for(const char* invalida : {"", " ", "(a+b", "a+b)", "a+", "a b", "()", "2(x)", "a**b", "12x", "a@b", "-3"}) {
        bool error=false;
        try { a.convertir(invalida); } catch(const std::invalid_argument&) { error=true; }
        assert(error);
        assert(a.convertir("1+2")=="1 2 +");
    }
    std::cout << a.convertir("12 + x * (7 - 3)^2") << '\n'; a.limpiar();

    std::cout << "Ejercicio 26 - pruebas correctas\n";
}
