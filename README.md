# Práctica 1 - EDA
## Estructuras Unidimensionales

### Integrantes

- Luisa María Cuervo Prado
- Carlos Mario Monsalve Tangarife
- Sheryl Astrid Murillo Duque

## Objetivo general

Esta práctica implementa dos problemas clásicos de estructuras de datos:

1. Un historial de edición con operaciones de Undo/Redo sobre un documento de texto.
2. Un rate limiter basado en ventana deslizante para decidir si un paquete debe aceptarse o rechazarse.

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
- Debe existir una ventana temporal `T` para limitar la tasa de paquetes.
- Debe existir un límite `L` de paquetes aceptados dentro de la ventana temporal.
- Un paquete debe rechazarse si supera el límite de tasa.
- Un paquete debe rechazarse si el buffer está lleno.
- Debe mantenerse el conteo de paquetes aceptados y rechazados.
- Debe calcularse la ocupación máxima del buffer.

#### Semántica propuesta

- Se purgan los paquetes más antiguos cuando su antigüedad es mayor o igual a `T`.
- Si la cantidad de paquetes activos en la ventana supera `L`, se rechaza con `REJECTED_RATE_LIMIT`.
- Si no hay espacio en el buffer, se rechaza con `REJECTED_BUFFER_FULL`.
- Si se acepta el paquete, se agrega al buffer y a la cola temporal de timestamps.

## Estructura del proyecto

- `src/stack_array.*`: implementación de la pila dinámica.
- `src/undoredo.*`: gestión de edición, undo y redo.
- `src/ratelimiter.*`: control de tasa y buffer.
- `tests/`: casos de prueba del enunciado.
- `results/`: salidas generadas por la ejecución.

## Validación

El programa debe ejecutarse sin errores, generar salidas para cada caso de prueba y producir un documento final consistente con las operaciones recibidas.

Para verificar el estado real del proyecto, se puede compilar y ejecutar con:

```bash
g++ -std=c++17 src/*.cpp -I src -o app
./app
```

La solución se considera completa si:

- compila sin errores,
- interpreta correctamente los comandos de edición,
- respeta la lógica de undo/redo,
- maneja correctamente la ventana de tasa y el buffer,
- sobresale en casos borde y no produce inconsistencias entre historial y documento final.