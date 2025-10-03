#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

/*
 * Representa un evento con tipo y contenido.
 * Ejemplo: { "type": "sensor", "payload": "Temperatura: 23°C" }
 */
struct Event {
    std::string type;
    std::string payload;
};

/*
 * Clase thread-safe que gestiona una cola de eventos.
 * Varios hilos pueden producir (push) y consumir (pop) de forma segura.
 */
class EventQueue {
public:
    void push(const Event& event) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(event);
        cv_.notify_one();
    }

    Event pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return !queue_.empty(); });
        auto event = queue_.front();
        queue_.pop();
        return event;
    }

private:
    std::queue<Event> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
};



