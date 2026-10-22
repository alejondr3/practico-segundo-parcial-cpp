# Guía de las soluciones

Los diagramas son esquemas de referencia para reproducir a mano. Las letras representan identidad de nodo, no copias de sus datos.

## 09 — Partición estable

Se desconecta cada nodo antes de anexarlo a la cola de uno de tres grupos. Después se concatenan los grupos no vacíos. Anexar al final mantiene el orden de llegada. Cada nodo pertenece exactamente a una cadena.

```text
Entrada: head -> 3a -> 5a -> 8a -> 5b -> 2a -> null
Procesando:
  menores: 3a -> 2a -> null
  iguales: 5a -> 5b -> null
  mayores: 8a -> null
Salida: head -> 3a -> 2a -> 5a -> 5b -> 8a <- tail
Borde, todo igual: head -> 5a -> 5b <- tail; mismos nodos y enlaces
Borde vacío: head = tail = null
```

Ningún nodo se crea o destruye durante la partición. La cabeza y la cola se reconstruyen al concatenar; cada cola parcial termina en null. Pruebas: ejemplo indicado, todos menores, iguales y mayores, vacío y un nodo.

## 11 — Palíndromo y restauración

Los punteros lento/rápido localizan el final de la primera mitad. Se invierte la segunda mitad, se comparan datos y se vuelve a invertir antes de retornar. En longitud impar se omite el centro. Una diferencia marca el resultado, pero nunca provoca una salida prematura que omita restaurar.

```text
Original: head -> 1a -> 2a -> 3 -> 2b -> 1b -> null
                              lento
Segunda mitad invertida: segunda -> 1b -> 2b -> null
Comparación: (1a,1b), (2a,2b)
Restaurada: head -> 1a -> 2a -> 3 -> 2b -> 1b <- tail
Borde: head -> 7 <- tail; retorna true sin cambiar enlaces
Borde vacío: retorna true
```

No se reserva memoria durante la verificación. Las pruebas comparan `huella` antes/después, incluyendo direcciones, enlaces y datos, para verificar restauración exacta. Incluyen casos pares, impares, no palíndromos, vacío, un nodo, dos distintos y repetidos.

## 17 — Fusión doble

Se elige la menor cabeza disponible y se transfiere al final del resultado. En empate se elige A. Se actualiza tanto `prev` como `next`, además de los extremos de origen/destino. Las tres listas deben ser distintas y el destino estar vacío.

```text
A: null <- 1 <-> 4 -> null
B: null <- 2 <-> 3 -> null
Salida: null <- 1 <-> 2 <-> 3 <-> 4 -> null
        head                        tail
A y B al terminar: head = tail = null
Borde A vacía: se transfieren todos los nodos de B
```

Invariantes: `head->prev == null`, `tail->next == null`, y cada enlace hacia adelante tiene su correspondiente enlace hacia atrás. `validar` comprueba ciclos, vecinos y extremos. El resultado pasa a ser propietario de todos los nodos; no se reserva durante la fusión. Pruebas: ejemplo, recorrido inverso, estabilidad del 10 mediante identidad, ambas vacías, una vacía en ambos órdenes y valores iguales.

## 23 — Intercambio físico

Se buscan ambos nodos y se guardan sus cuatro vecinos antes de escribir enlaces. Si B precede inmediatamente a A, se intercambian solo las variables locales para reutilizar el caso adyacente. Se tratan por separado adyacencia y no adyacencia.

```text
Antes: null <- 1 <-> 2a <-> 3 <-> 4b <-> 5 -> null
Después: null <- 1 <-> 4b <-> 3 <-> 2a <-> 5 -> null
Los nodos a y b conservan dato y dirección.
Borde dos nodos:
  null <- 1a <-> 2b -> null
  null <- 2b <-> 1a -> null
          head    tail
```

No hay new/delete ni intercambio de datos durante la operación. Se actualizan también head/tail cuando corresponde. Las pruebas incluyen las claves del práctico, ambos órdenes de adyacencia, extremos, identidad de nodos, clave ausente, claves iguales, vacío, un nodo y lista de dos nodos.

## 26 — Infija a postfija

El tokenizador consume números completos e identificadores completos. El estado `esperaOperando` detecta secuencias incompletas, operandos consecutivos y paréntesis vacíos. Una pila enlazada guarda operadores. Se desapilan operadores de mayor precedencia, y de igual precedencia salvo cuando el entrante es `^`.

```text
Entrada: 12 + x * (7 - 3)^2
Pila en un estado intermedio: tope -> '-' -> '(' -> '*' -> '+' -> null
Salida: 12 x 7 3 - 2 ^ * +
Borde: a^b^c -> a b c ^ ^
Error: (a+b -> apertura sin cierre; liberar toda la pila
```

La pila contiene operadores pendientes; cada apertura actúa como barrera. Cada nodo se libera al desapilar; ante error se limpia y se relanza la excepción. Las pruebas incluyen los cuatro escenarios del práctico, módulo, identificadores, precedencia, asociatividad, paréntesis anidados y doce entradas inválidas. Tras cada error se verifica que la misma estructura puede convertir otra expresión.

## 31 — Ordenar pila

Se extrae un valor de la original; mientras la auxiliar tenga un tope mayor, se devuelven valores a la original. Se inserta el valor en la auxiliar. Al final se vuelca todo a la original, donde el menor queda arriba. La auxiliar permanece ordenada con el mayor arriba.

```text
Original inicial, tope primero: 2 -> 9 -> 3 -> 3 -> 7 -> 1 -> 4
Estado de auxiliar: tope -> 9 -> 7 -> 4 -> 3 -> 3 -> 2 -> 1
Final original: tope -> 1 -> 2 -> 3 -> 3 -> 4 -> 7 -> 9
Borde: tope = null; ambas fases terminan sin operaciones
```

Solo se utilizan las interfaces push/pop/top/vacia. Push crea un nodo y pop libera uno; no se mantienen nodos retirados. El costo cuadrático proviene de las reinserciones. Pruebas: ejemplo con repetidos, vacío, uno, negativos, orden ascendente/descendente y underflow.

## 33 — Pool de N pilas

Los nodos del pool se enlazan por índices. `-1` representa null. Push retira la cabeza de la lista libre y la pone en una pila; pop hace el movimiento inverso. Ningún nodo puede estar a la vez en una pila y en la lista libre.

```text
N=3, M=4, inicio:
  topes: [-1, -1, -1]
  libre -> 0 -> 1 -> 2 -> 3 -> -1
push(1,10), push(0,20):
  pila 1: 0(10) -> -1
  pila 0: 1(20) -> -1
  libre -> 2 -> 3 -> -1
pop(1): libre -> 0 -> 2 -> 3 -> -1
Borde lleno: libre=-1; push devuelve false
Borde pila vacía: tope=-1; pop devuelve false
```

Se reserva una vez el arreglo de M nodos y una vez el arreglo de N topes. Se liberan ambos al finalizar. El verificador recorre las cadenas sin reservar memoria y comprueba pertenencia única de cada índice; es diagnóstico, no parte de push/pop. Pruebas: N=3/M=8, llenado intercalado, overflow, reutilización, LIFO de cada pila, underflow, una pila consume todo el pool, índices inválidos, capacidad cero y N inválido.
