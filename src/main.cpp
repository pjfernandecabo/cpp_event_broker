#include "../include/EventQueue.hpp"
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

// Estructura cliente con mutex propio y lista de suscripciones
struct Client {
    std::shared_ptr<websocket::stream<tcp::socket>> ws;
    std::shared_ptr<std::mutex> write_mutex;
    std::vector<std::string> subscriptions; // ej: {"sensor", "log"}
};

std::vector<Client> clients;        // Lista global de clientes
std::mutex clients_mutex;           // Mutex para proteger la lista

// Añadir cliente
void add_client(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.push_back({ws, std::make_shared<std::mutex>(), {}});
}

// Eliminar cliente
void remove_client(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.erase(std::remove_if(clients.begin(), clients.end(),
                 [&](const Client& c){ return c.ws == ws; }),
                 clients.end());
}

// Broadcast solo a clientes suscritos al tipo de evento
void broadcast(const Event& event) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto& client : clients) {
        if (std::find(client.subscriptions.begin(),
                      client.subscriptions.end(),
                      event.type) != client.subscriptions.end()) 
        {
            try {
                std::lock_guard<std::mutex> lock_ws(*client.write_mutex);
                std::string msg = "{ \"type\": \"" + event.type +
                                  "\", \"payload\": \"" + event.payload + "\" }";
                client.ws->write(net::buffer(msg));
            } catch (...) {
                std::cerr << "[Broker] Error al enviar a un cliente." << std::endl;
            }
        }
    }
}

// Manejo de sesión de cliente
void do_session(tcp::socket socket, EventQueue& queue) {
    try {
        auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(socket));
        ws->accept();
        add_client(ws);

        std::cout << "[Broker] Cliente conectado." << std::endl;

        // Recibir mensajes de suscripción del cliente
        for (;;) {
            beast::flat_buffer buffer;
            ws->read(buffer);
            std::string msg = beast::buffers_to_string(buffer.data());

            if (msg.rfind("SUBSCRIBE:", 0) == 0) {
                std::string type = msg.substr(10); // lo que viene después de "SUBSCRIBE:"
                std::lock_guard<std::mutex> lock(clients_mutex);
                for (auto& client : clients) {
                    if (client.ws == ws) {
                        client.subscriptions.push_back(type);
                        std::cout << "[Broker] Cliente suscrito a: " << type << std::endl;
                    }
                }
            }
        }

    } catch (std::exception const& e) {
        std::cerr << "[Broker] Cliente desconectado: " << e.what() << std::endl;
        remove_client(nullptr); // limpiamos cliente
    }
}

int main() {
    try {
        EventQueue queue;

        // Publisher: genera eventos de distintos tipos
        std::thread publisher([&queue]() {
            int counter = 1;
            while (true) {
                Event ev1{"sensor", "Temperatura: " + std::to_string(20 + (counter % 5)) + "°C"};
                Event ev2{"log", "CPU al " + std::to_string(70 + (counter % 10)) + "%"};
                Event ev3{"system", "Evento del sistema #" + std::to_string(counter)};

                queue.push(ev1);
                queue.push(ev2);
                queue.push(ev3);

                std::this_thread::sleep_for(std::chrono::seconds(3));
                counter++;
            }
        });

        net::io_context ioc;
        tcp::acceptor acceptor(ioc, {tcp::v4(), 9002});

        // Consumidor: envía eventos a clientes suscritos
        std::thread consumer([&queue]() {
            while (true) {
                Event event = queue.pop();
                std::cout << "[Broker] Enviando (" << event.type << "): " << event.payload << std::endl;
                broadcast(event);
            }
        });

        std::cout << "Servidor activo en ws://localhost:9002 ..." << std::endl;

        // Acepta clientes en bucle
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

