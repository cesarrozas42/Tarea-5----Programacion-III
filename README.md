# Tarea 5 - Programacion III: Patrones de Diseno

| Ejercicio | Patron | Carpeta | Target CMake |
|---|---|---|---|
| 1 | Singleton - Transaction Manager concurrente | `ej1/` | `ej1_singleton` |
| 2 | Abstract Factory - Motor multiplataforma | `ej2/` | `ej2_abstract_factory` |
| 3 | Composite - Expresiones matematicas | `ej3/` | `ej3_composite` |
| 4 | Iterator - Recorrido de grafo | `ej4/` | `ej4_iterator` |

## Compilar y ejecutar

En CLion: abrir la carpeta, recargar el proyecto CMake y elegir el target en la lista de configuraciones.

Desde terminal:

```bash
cmake -S . -B build
cmake --build build
./build/ej1_singleton
./build/ej4_iterator
```

## Ejercicio 1 - Singleton

- `TransactionManager::getInstance()` usa una variable `static` local (Meyers singleton), cuya inicializacion es thread-safe desde C++11. Constructor privado; copia y movimiento eliminados.
- `deposit` y `withdraw` se ejecutan completos bajo un `std::mutex` (`lock_guard`): validar, modificar el saldo y registrar la `Transaction` es una sola seccion critica, asi que son atomicas.
- `withdraw` rechaza retiros sin fondos suficientes (el saldo nunca queda negativo).
- Solo `TransactionManager` puede modificar el balance de una `Account` (`friend`).

**Nota sobre el resultado esperado:** con la simulacion del enunciado (10 hilos, cada uno deposita 100 y retira 50) el balance final es `1000 + 10*100 - 10*50 = 1500`. El valor `1000 - (10*50) = 500` del enunciado solo seria correcto si no hubiera depositos.

El `main` agrega dos verificaciones: que 20 hilos obtienen la misma direccion de la instancia y una prueba de estres (100 hilos x 1000 retiros de 1 sobre 50000) que debe terminar exactamente en 0.

## Ejercicio 4 - Iterator

- `Graph` guarda sus `Node` en un `unordered_map` privado; el cliente solo usa `addEdge`/`addNode`/`contains`. Los iteradores acceden a la estructura como `friend`, sin exponerla.
- `GraphIterator` es la interfaz (`hasNext`, `next`, `reset`); `BFSIterator` (cola) y `DFSIterator` (pila explicita, sin recursion) la implementan.
- Ambos usan un conjunto de visitados, por lo que los ciclos y auto-lazos no generan bucles infinitos.
- Como el cliente maneja un `std::unique_ptr<GraphIterator>`, el iterador se puede cambiar en tiempo de ejecucion.

Salida para el grafo del enunciado (`1->2, 1->3, 2->4`): BFS `1 2 3 4`, DFS `1 2 4 3`.
