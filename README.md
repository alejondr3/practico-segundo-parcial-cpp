# Práctico del segundo parcial — C++

Soluciones de los ejercicios **17, 23, 31, 26, 11, 9 y 33**. Cada carpeta contiene dos programas independientes:

- `struct.cpp`: nodos y estado con `struct`, operaciones mediante funciones externas y liberación explícita.
- `class.cpp`: nodos y estado privados, métodos públicos, constructor y destructor. Se impide copiar objetos propietarios de memoria para evitar dobles liberaciones.

Los 14 programas usan C++17 y **únicamente `#include <iostream>`**. No usan string, sstream, cctype, cassert, stdexcept ni contenedores STL. La salida usa `cout`; el reconocimiento de caracteres, las comparaciones y las pruebas se implementan manualmente. El ejercicio 33 usa un pool de nodos y un arreglo de topes reservados al iniciar; push/pop no reservan memoria.

## Compilar y ejecutar

Cada archivo tiene su propio `main`: **compilar un archivo a la vez**.

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic ejercicio_17/struct.cpp -o ejercicio17
./ejercicio17
g++ -std=c++17 -Wall -Wextra -Wpedantic ejercicio_17/class.cpp -o ejercicio17_class
./ejercicio17_class
```

En Windows, ejecutar `./ejercicio17.exe`. También pueden abrirse los archivos individualmente en un IDE con soporte C++17.

Para compilar y comprobar los 14 programas con Python y g++ instalados:

```sh
python probar.py
```

Los `main` de los ejercicios 9, 11, 17, 26, 31 y 33 contienen los datos del práctico y pruebas adicionales. **El ejercicio 23 es interactivo:** pide cantidad y valores iniciales, luego permite intercambiar las claves elegidas mediante un menú (1 intercambiar, 0 salir). La función propia `comprobar` cuenta y muestra los fallos; no usa `assert`. Cada programa devuelve 0 si las pruebas pasan y 1 si fallan. Para ensayar otros datos se modifican esos casos en `main`. Los arreglos de los casos de prueba no son memoria auxiliar de las operaciones sobre listas/pilas.

## Ejercicios

| Inciso | Operación | Tiempo | Memoria auxiliar de la operación |
|---|---|---|---|
| 09 | Partición estable en menores, iguales y mayores | O(n) | O(1) |
| 11 | Palíndromo con restauración exacta de enlaces | O(n) | O(1) |
| 17 | Fusión estable de listas dobles | O(n+m) | O(1) |
| 23 | Intercambio físico de dos nodos | O(n) búsqueda, O(1) reenlace | O(1) |
| 26 | Conversión infija a postfija | O(L) caracteres de entrada | O(L) |
| 31 | Ordenamiento con una pila auxiliar | O(n²) peor caso | O(n) |
| 33 | Push, pop y consulta de tope | O(1) | O(1) por operación; pool O(M+N) |

La creación y destrucción de estructuras se contabilizan aparte. En el 33, iniciar y liberar cuestan O(M+N) y O(1), respectivamente, sin contar el costo interno del asignador. El verificador diagnóstico `validar` del pool cuesta O((M+1)·(M+N)) en estados válidos y no forma parte de push/pop. Las funciones `mostrar`, `coincide`, `coincideReversa`, `nodoEn` y `enlaceEn` se usan para mostrar/verificar resultados; las operaciones principales mantienen las cotas indicadas.

Consultar [GUIA.md](GUIA.md) para estrategia, invariantes, memoria y diagramas de nodos.

## Decisiones explícitas

- 17: devuelve `false` si el destino no está vacío o las tres listas no son distintas; igualdad elige primero un nodo de A. Las entradas deben estar ordenadas y poseer nodos distintos.
- 23: se usa la primera aparición de cada clave. Claves iguales o inexistentes devuelven `false` sin alterar la lista.
- 26: números enteros sin signo, identificadores que empiezan por letra o `_`, operadores binarios y paréntesis. `^` es asociativo a la derecha. El signo unario es una variante adicional del enunciado y no se incluye. No se evalúan operaciones: `10/(5-5)` se convierte correctamente.
- 31: los datos se apilan en el orden escrito; el último queda inicialmente arriba. Al terminar, el menor está arriba.
- 33: índices de pila de `0` a `N-1`; overflow, underflow e índices inválidos devuelven `false`. `iniciar` también devuelve `false` si las dimensiones no son válidas, el objeto ya está iniciado o falla la reserva. `N>0`, `M>=0`. Una pila puede consumir todo el pool.

## Memoria

Las listas son propietarias de sus nodos. La partición y el intercambio solo reenlazan; la fusión transfiere la propiedad al destino. Cada reserva se libera con `delete`, o `delete[]` para el pool y los topes. En las versiones con class el destructor libera automáticamente. En las versiones struct se llama a `limpiar` explícitamente. Las funciones de limpieza admiten llamadas repetidas.

La conversión libera su pila también al detectar errores. La pila auxiliar del 31 termina vacía. Su algoritmo usa únicamente push/pop/top y consulta de vacío: no accede a nodos internos.

El PDF original no se incluye en el repositorio.

## Interfaces corregidas

- 26: `convertir` recibe `const char*` y devuelve `bool`. Entrega la postfija por `char*& salida`, y un diagnóstico por `const char*& error`. El llamador debe liberar con `delete[]` cada salida exitosa antes de volver a usar esa variable. En fallo, salida queda en `nullptr`. Se reserva un buffer de hasta `2*L+1` caracteres para la salida; no reemplaza la pila enlazada de operadores.
- 31: `pop` y `top` devuelven `bool` y entregan el dato mediante una referencia. Devuelven `false` si la pila está vacía; no alteran el parámetro de salida en ese caso.
- 33: `indiceValido` es una comprobación propia; no se usan excepciones de biblioteca. El `catch (...)` de inicialización libera reservas parciales si falla `new[]`.

Las versiones class conservan constructor, destructor y estado privado. Las operaciones centrales mantienen sus restricciones de punteros y complejidad.

## Ejercicio 23: datos por teclado

Ejecuta cualquiera de sus versiones, ingresa una cantidad no negativa y luego los valores de los nodos. Selecciona 1, escribe las dos claves a intercambiar y revisa el recorrido hacia adelante y hacia atrás. Puedes repetir el intercambio o seleccionar 0 para salir y liberar memoria. Se mantiene la primera aparición de cada clave cuando existen repetidos.

Ejemplo de entradas: `5`, luego `10 20 30 40 50`, luego `1`, `20`, `40`, `0`. El resultado es `10 40 30 20 50`. `probar.py` suministra automáticamente entradas para comprobar ambas versiones interactivas.

Todos los programas declaran `using namespace std;` después de incluir iostream y usan cout/cin sin prefijos.
