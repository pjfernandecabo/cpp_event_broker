#pragma once
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

/*
 * Clase thread-safe que representa la cola de eventos.
 * Los hilos pueden push() y pop() eventos de manera concurrente
 * sin riesgo de race conditions.
 */
class EventQueue {
public:
    // Inserta un evento en la cola
    void push(const std::string& event) {
        std::lock_guard<std::mutex> lock(mutex_); // Bloquea mutex durante el push
        queue_.push(event);
        cv_.notify_one(); // Despierta un hilo que pueda estar esperando en pop()
    }

    // Extrae un evento de la cola
    std::string pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        // Espera hasta que haya un evento disponible
        cv_.wait(lock, [this] { return !queue_.empty(); });
        auto event = queue_.front();
        queue_.pop();
        return event;
    }

private:
    std::queue<std::string> queue_;           // Cola interna de eventos
    std::mutex mutex_;                        // Mutex para proteger acceso concurrente
    std::condition_variable cv_;              // Condición para notificar disponibilidad
};


