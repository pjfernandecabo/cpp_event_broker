# C++ Event Broker con WebSocket

Este proyecto es un **broker de eventos en C++** que soporta múltiples clientes mediante WebSockets.  
Los eventos se generan en un hilo **publisher** y se envían a todos los clientes conectados (broadcast).

## Estructura del proyecto

- `include/EventQueue.hpp` → Clase thread-safe para manejar la cola de eventos.
- `src/main.cpp` → Servidor WebSocket y lógica de broadcast.
- `CMakeLists.txt` → Para compilar el proyecto con CMake.
- `README.md` → Documentación básica.

## Requisitos

- C++20
- Boost (Beast + System)
- CMake >= 3.20

## Compilación

```bash
mkdir build
cd build
cmake ..
make
./event_broker
