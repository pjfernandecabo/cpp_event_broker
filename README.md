# C++ Event Broker con WebSocket Tutorial 🚀

Este repositorio contiene un curso práctico de **C++ moderno** enfocado en **IA, integración con Python y arquitecturas de agentes**.  

El proyecto principal es un **Event Broker** que permite publicar y suscribirse a eventos en tiempo real mediante **WebSockets**, combinando C++ y Python.  
 
Los eventos se generan en un hilo **publisher** y se envían a todos los clientes conectados (broadcast).



## 📌 Estructura del curso
Cada **Hora (lección)** del curso está marcada con un **tag** en Git.  
Esto permite navegar fácilmente por las distintas etapas de desarrollo del proyecto.  

- `hora-1`: Servidor C++ básico que imprime "Event Broker starting..."
- `hora-2`: Cola de eventos (`EventQueue`) con hilos productor/consumidor
- `hora-3`: Productor y consumidor con sincronización (`std::mutex`)
- `hora-4`: Primer servidor WebSocket que responde `"Hello"`
- `hora-5`: Integración Publisher → Broker → Cliente Python
- `hora-6`: Soporte para múltiples clientes
- `hora-7`: Refactorización con directorio `include/` y código comentado
- `hora-8`: Opción de **desuscripción** (`UNSUBSCRIBE`) desde clientes
- ... y así sucesivamente

## 🛠️ Requisitos
- **C++17 o superior**
- [CMake](https://cmake.org/) (>= 3.10)
- [WebSocket++](https://github.com/zaphoyd/websocketpp)
- Python 3.9+ con librería `websockets`

## 🚀 Cómo compilar
```bash
mkdir build && cd build
cmake ..
make
./event_broker
```

## 🐍 Ejecución de cliente Python

Ejemplo de cliente que recibe eventos:

```python
python client.py
```

Ejemplo de cliente que se desuscribe tras 3 mensajes:

```python
python client_unsubscribe.py
```

## 🔖 Cómo navegar por las lecciones

Cada lección está marcada con un tag:

```bash
# ver todos los tags disponibles
git tag 

# moverte a la lección de la hora 3
git checkout hora-3
```

&nbsp;
&nbsp;


## Github tags

```bash
# subes la lección 1
git add .
git commit -m "Hora 1: Event broker básico"
git tag hora-1
git push origin main --tags

# después de la lección 2
git add .
git commit -m "Hora 2: Añadido EventQueue"
git tag hora-2
git push origin main --tags

# ver tags
git tag

# volver a cualquier tag
git checkout hora-2
```

# Estructura del proyecto

```css

cpp_event_broker/
├── include/
|  └── EventQueue.hpp
├── src/
│   └── main.cpp
├── CMakeLists.txt
└── README.md
```


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
```