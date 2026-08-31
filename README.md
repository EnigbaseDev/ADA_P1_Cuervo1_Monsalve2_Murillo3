# Práctica 1 - EDA
## Estructuras Unidimensionales

### Integrantes

- Luisa María Cuervo Prado
- Carlos Mario Monsalve Tangarife
- Sheryl Astrid Murillo Duque

## Objetivo general

Esta práctica implementa dos problemas clásicos de estructuras de datos:

1. Un historial de edición con operaciones de Undo/Redo sobre un documento de texto, usando una **pila** (LIFO).
2. Un búfer de recepción y limitador de tasa por ventana deslizante para un firewall, usando una **cola** (FIFO).

Ambas estructuras se implementan desde cero en dos representaciones (arreglo dinámico y lista enlazada), sin usar `std::stack`, `std::queue`, `std::deque`, `std::list` ni `std::forward_list`. `std::string`/`std::vector` solo se usan como estructuras auxiliares (por ejemplo, para el documento simulado del Problema 1), nunca como sustituto de la pila o la cola evaluadas.

## Requisitos funcionales

### Problema 1: Undo/Redo

- Debe existir un historial de operaciones de edición.
- Cada operación de edición debe registrarse en la pila de Undo.
- La operación `UNDO` debe deshacer la última edición efectiva y moverla a la pila de Redo.
- La operación `REDO` debe rehacer la última operación deshecha y devolverla a la pila de Undo.
- Las operaciones válidas son:
  - `INSERT`: insertar texto en una posición.
  - `DELETE`: borrar un bloque de texto o una cantidad determinada de caracteres.
  - `REPLACE`: reemplazar texto existente por otro texto.
- Si se registra una nueva edición tras un `UNDO`, el historial de `Redo` debe limpiarse.
- El documento final debe respetar el estado real de las operaciones ejecutadas.
- Debe soportar entradas con posiciones fuera de rango, encajándolas al límite válido del documento.

#### Formato esperado de entrada

Cada línea puede ser cualquiera de las siguientes:

- `EDIT INSERT <posicion> <texto>`
- `EDIT DELETE <posicion> <texto_o_longitud>`
- `EDIT REPLACE <posicion> <nuevo_texto>`
- `UNDO`
- `REDO`

#### Ejemplos de comportamiento esperado

- `EDIT INSERT 0 hola` -> inserta `hola` al inicio.
- `EDIT DELETE 0 2` -> borra 2 caracteres desde la posición 0.
- `UNDO` -> revierte la última edición.
- `REDO` -> rehace la edición deshecha.

### Problema 2: Rate limiter

- Debe existir un buffer con capacidad `C`.
- Debe existir una ventana temporal `T` (ms) para limitar la tasa de paquetes.
- Debe existir un límite `L` de paquetes aceptados dentro de la ventana temporal.
- Un paquete debe rechazarse si supera el límite de tasa.
- Un paquete debe rechazarse si el buffer está lleno.
- Debe mantenerse el conteo de paquetes aceptados y rechazados.
- Debe calcularse la ocupación máxima del buffer.

#### Semántica propuesta

- Al llegar cada paquete se purgan primero, desde el frente de la cola de marcas de tiempo, todas las marcas anteriores a `t - T`. El borde `t = t0 + T` se considera **fuera** de la ventana (se purga; condición `current_timestamp >= oldest_ts + time_window_T`).
- El búfer de recepción (`buffer_queue`) y la cola de marcas de tiempo (`timestamp_queue`) crecen siempre en sincronía: cada paquete aceptado se encola en ambas al mismo tiempo. Por eso, cuando una marca de tiempo expira y sale de `timestamp_queue`, el paquete correspondiente también sale de `buffer_queue`, liberando espacio en el búfer. Sin esta purga sincronizada, el búfer se llenaría una única vez con los primeros `C` paquetes y rechazaría todo el tráfico restante para siempre.
- Si, tras purgar, la cantidad de paquetes activos en la ventana es `>= L`, se rechaza con `REJECTED_RATE_LIMIT`.
- Si no hay espacio en el buffer, se rechaza con `REJECTED_BUFFER_FULL`.
- Si se acepta el paquete, se agrega al buffer y a la cola temporal de timestamps.

## Estructura del proyecto

- `src/stack_array.*`: Pila sobre arreglo dinámico (con redimensionamiento por duplicación).
- `src/stack_list.*`: Pila sobre lista enlazada simple.
- `src/undoredo.*`: gestión de edición, undo y redo (Problema 1), parametrizada por representación de pila.
- `src/queue_circular.hpp`: Cola circular sobre arreglo de capacidad fija (plantilla genérica).
- `src/queue_list.hpp`: Cola sobre lista enlazada simple (plantilla genérica).
- `src/ratelimiter.*`: control de tasa y buffer del firewall (Problema 2); usa `QueueCircular` para el búfer de recepción y `QueueList` para la cola de marcas de tiempo.
- `src/main.cpp`: orquestador — corre los casos de prueba, la demostración de TAD (pila y cola) y la experimentación de tiempos de ambos problemas.
- `data/generador.cpp`: genera los archivos sintéticos de entrada con semilla fija.
- `tests/`: casos de prueba de la Sección 11 del enunciado.
- `results/`: salidas generadas por la ejecución (casos de prueba, comparación de TAD, experimentación).
- `report/`: informe técnico (`Informe.pdf`).

## Compilación

```bash
g++ -std=c++17 -O2 -o ada_p1 src/*.cpp
```

> No se debe listar `src/main.cpp` por separado junto con `src/*.cpp` (por ejemplo `g++ ... src/main.cpp src/*.cpp`): el glob `*.cpp` ya incluye `main.cpp`, y listarlo dos veces hace que se compile dos veces, produciendo errores de "multiple definition" al enlazar. El comando de arriba compila cada `.cpp` de `src/` exactamente una vez.

## Ejecución

```bash
./ada_p1
```

Esto corre, en orden: los 7 casos de prueba del Problema 1, la comparación TAD Pila (`StackArray` vs `StackList`), la experimentación de tiempos del Problema 1, los 7 casos de prueba del Problema 2, la comparación TAD Cola (`QueueCircular` vs `QueueList`), y la experimentación de tiempos del Problema 2. Todas las salidas quedan en `results/`.

## Reproducir los experimentos

Los datos sintéticos se generan con semilla fija (`semilla = 42`) para que sean reproducibles:

```bash
g++ -std=c++17 -O2 -o generador data/generador.cpp
./generador
```

Esto regenera los archivos en `data/generados/` (`n1000.txt` ... `n1000000.txt` para el Problema 1, `p2_n1000.txt` ... `p2_n1000000.txt` para el Problema 2, con `C=10, T=1000ms, L=5`). Luego, correr `./ada_p1` regenera automáticamente `results/experimentacion.txt` y `results/experimentacion_p2.txt` (5 repeticiones por tamaño, con media y desviación estándar).

## Resumen de resultados principales

**Problema 1 — tiempo total de procesamiento (media de 5 repeticiones):**

| n (eventos) | tiempo promedio (ms) | desviación estándar (ms) |
|---|---|---|
| 1,000 | 2.00 | 0.44 |
| 10,000 | 15.11 | 0.85 |
| 100,000 | 350.47 | 12.44 |
| 1,000,000 | 47,259.30 | 302.89 |

(En el PC del Tester)
"Ryzen 5 3600, 32gb ram 3200mhz, M.2 nvme"

El crecimiento no es lineal (de 100,000 a 1,000,000, 10× el tamaño, el tiempo se multiplica por ~135×). Esto **no** viene de la pila (`push`/`pop` siguen siendo O(1) amortizado), sino de aplicar cada edición al documento (`std::string::insert`/`erase`), cuyo costo crece con el tamaño del documento — ver el análisis de complejidad global en el informe (Sección 8.3).

**Problema 2 — tiempo total de procesamiento (media de 5 repeticiones):**

| n (paquetes) | tiempo promedio (ms) | desviación estándar (ms) |
|---|---|---|
| 1,000 | 0.0046 | 0.0004 |
| 10,000 | 0.0488 | 0.0021 |
| 100,000 | 0.5254 | 0.0379 |
| 1,000,000 | 5.3153 | 0.3520 |

(En el PC del Tester)
"Ryzen 5 3600, 32gb ram 3200mhz, M.2 nvme"

Este sí crece de forma consistente con O(1) amortizado por paquete (10× el tamaño ≈ 10× el tiempo), como se espera de `enqueue`/`dequeue`/purga sobre las colas.

**Comparación de representaciones (TAD):** tanto para la Pila (`StackArray` vs `StackList`, los 7 casos de prueba) como para la Cola (`QueueCircular` vs `QueueList`, una secuencia sintética de `enqueue`/`dequeue`), ambas representaciones producen resultados idénticos, confirmando que satisfacen el mismo contrato de interfaz independientemente de su representación interna (ver `results/comparacion_pilas.txt` y `results/comparacion_colas.txt`).

## Validación

La solución se considera completa si:

- compila sin errores con el comando de la sección "Compilación",
- interpreta correctamente los comandos de edición,
- respeta la lógica de undo/redo,
- maneja correctamente la ventana de tasa y el buffer (incluyendo su purga),
- sobresale en casos borde y no produce inconsistencias entre historial y documento final,
- las dos representaciones de cada estructura (arreglo y lista) producen resultados idénticos ante la misma entrada.