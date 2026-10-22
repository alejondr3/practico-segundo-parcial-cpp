# Práctico del segundo parcial — C++

Soluciones de los ejercicios **17, 23, 31, 26, 11, 9 y 33**. Cada carpeta contiene dos programas independientes:

- `struct.cpp`: nodos y estado con `struct`, operaciones mediante funciones externas y liberación explícita.
- `class.cpp`: nodos y estado privados, métodos públicos, constructor y destructor. Se impide copiar objetos propietarios de memoria para evitar dobles liberaciones.

Los programas usan C++17 y estructuras nativas con punteros. No usan contenedores STL para reemplazar listas o pilas. `std::string` y flujos se usan para tokenización, presentación y comprobaciones. El ejercicio 33 usa un pool de nodos y un arreglo de topes reservados al iniciar; push/pop no reservan memoria.

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

Los `main` contienen los datos del práctico y pruebas adicionales. Ejecutan las operaciones, comprueban resultados con `assert` y muestran un mensaje al terminar. Para ensayar otros datos se modifican esos casos en `main`. No compilar con `-DNDEBUG`: desactiva las comprobaciones y algunas operaciones incluidas dentro de ellas.

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

La creación y destrucción de estructuras se contabilizan aparte. En el 33, iniciar y liberar cuestan O(M+N) y O(1), respectivamente, sin contar el costo interno del asignador. El verificador diagnóstico `validar` del pool cuesta O(M·(M+N)) y no forma parte de push/pop. Los métodos `texto`, `huella` y `reversa` son auxiliares para mostrar/verificar resultados; las operaciones principales mantienen las cotas indicadas.

Consultar [GUIA.md](GUIA.md) para estrategia, invariantes, memoria y diagramas de nodos.

## Decisiones explícitas

- 17: destino vacío y tres listas distintas; igualdad elige primero un nodo de A. Las entradas deben estar ordenadas y poseer nodos distintos.
- 23: se usa la primera aparición de cada clave. Claves iguales o inexistentes devuelven `false` sin alterar la lista.
- 26: números enteros sin signo, identificadores que empiezan por letra o `_`, operadores binarios y paréntesis. `^` es asociativo a la derecha. El signo unario es una variante adicional del enunciado y no se incluye. No se evalúan operaciones: `10/(5-5)` se convierte correctamente.
- 31: los datos se apilan en el orden escrito; el último queda inicialmente arriba. Al terminar, el menor está arriba.
- 33: índices de pila de `0` a `N-1`; overflow y underflow devuelven `false`; índice inválido lanza `std::out_of_range`. `N>0`, `M>=0`. Una pila puede consumir todo el pool.

## Memoria

Las listas son propietarias de sus nodos. La partición y el intercambio solo reenlazan; la fusión transfiere la propiedad al destino. Cada reserva se libera con `delete`, o `delete[]` para el pool y los topes. En las versiones con class el destructor libera automáticamente. En las versiones struct se llama a `limpiar` explícitamente. Las funciones de limpieza admiten llamadas repetidas.

La conversión libera su pila también al detectar errores. La pila auxiliar del 31 termina vacía. Su algoritmo usa únicamente push/pop/top y consulta de vacío: no accede a nodos internos.

El PDF original no se incluye en el repositorio.
