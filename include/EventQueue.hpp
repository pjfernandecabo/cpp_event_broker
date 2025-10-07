#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

// ======================================================
// Estructura que representa un evento genérico.
// Más adelante podremos añadir campos como tipo, timestamp, prioridad, etc.
// ======================================================
struct Event {
    std::string data;   // Contenido del evento
};

// ======================================================
// Clase EventQueue
// Cola segura para múltiples hilos (thread-safe queue).
// Se usa para comunicar eventos entre el publisher, el broker y los clientes.
// ======================================================
class EventQueue {
public:
    // Inserta un nuevo evento en la cola
    void push(const Event& event) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(event);
        cond_.notify_one();
    }

    // Extrae un evento de la cola (bloqueante si está vacía)
    Event pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [&]{ return !queue_.empty(); });
        Event event = queue_.front();
        queue_.pop();
        return event;
    }

private:
    std::queue<Event> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};



