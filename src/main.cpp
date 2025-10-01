#include "EventQueue.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include <algorithm>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

/*
 * Estructura que representa un cliente conectado
 * Cada cliente tiene su websocket y un mutex propio para proteger write()
 */
struct Client {
    std::shared_ptr<websocket::stream<tcp::socket>> ws;
    std::shared_ptr<std::mutex> write_mutex;
};

// Lista global de clientes conectados
std::vector<Client> clients;
std::mutex clients_mutex; // Mutex para proteger la lista de clientes

// Añade un cliente a la lista
void add_client(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.push_back({ws, std::make_shared<std::mutex>()});
}

// Elimina un cliente de la lista
void remove_client(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.erase(std::remove_if(clients.begin(), clients.end(),
                 [&](const Client& c){ return c.ws == ws; }),
                 clients.end());
}

// Envía un mensaje a todos los clientes conectados (broadcast)
void broadcast(const std::string& msg) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto& client : clients) {
        try {
            // Bloquea el mutex del cliente para evitar conflictos de escritura
            std::lock_guard<std::mutex> lock_ws(*client.write_mutex);
            client.ws->write(net::buffer(msg));
        } catch (...) {
            std::cerr << "[Broker] Error al enviar a un cliente." << std::endl;
        }
    }
}

// Maneja la sesión de un cliente individual
void do_session(tcp::socket socket, EventQueue& queue) {
    try {
        auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket));
        ws->accept();           // Acepta handshake WebSocket
        add_client(ws);         // Añade a la lista de clientes

        std::cout << "[Broker] Cliente conectado." << std::endl;

        // Mantener la conexión activa
        for (;;) {
            beast::flat_buffer buffer;
            ws->read(buffer); // Bloquea esperando posibles mensajes del cliente
        }

    } catch (std::exception const& e) {
        std::cerr << "[Broker] Cliente desconectado: " << e.what() << std::endl;
        // Eliminar de la lista de clientes
        // remove_client(ws);  // Opcional si quieres limpiar inmediatamente
    }
}

int main() {
    try {
        EventQueue queue;

        // Hilo publisher: genera eventos cada 2 segundos y los pone en la cola
        std::thread publisher([&queue]() {
            int counter = 1;
            while (true) {
                std::string event = "Evento " + std::to_string(counter++);
                std::cout << "[Publisher] Generando: " << event << std::endl;
                queue.push(event);
                std::this_thread::sleep_for(std::chrono::seconds(4));
            }
        });

        net::io_context ioc;
        tcp::acceptor acceptor(ioc, {tcp::v4(), 9002}); // Puerto del servidor

        // Hilo consumidor: toma eventos de la cola y los envía a todos los clientes
        std::thread consumer([&queue]() {
            while (true) {
                std::string event = queue.pop();
                std::cout << "[Broker] Enviando a todos: " << event << std::endl;
                broadcast(event);
            }
        });

        std::cout << "Servidor activo en ws://localhost:9002 ..." << std::endl;

        // Loop principal para aceptar nuevas conexiones de clientes
        for (;;) {
            tcp::socket socket(ioc);
            acceptor.accept(socket);
            std::thread(&do_session, std::move(socket), std::ref(queue)).detach();
        }

        publisher.join();
        consumer.join();

    } catch (std::exception const& e) {
        std::cerr << "[Broker] Error: " << e.what() << std::endl;
    }
}
