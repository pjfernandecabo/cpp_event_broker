//#define BOOST_ASIO_NO_DEPRECATED
//#define ASIO_STANDALONE
//#define _WEBSOCKETPP_CPP11_STL_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <set>
#include <thread>
#include <chrono>
#include "EventQueue.hpp"

// ======================================================
// Alias para simplificar el uso del servidor WebSocket++
// ======================================================
typedef websocketpp::server<websocketpp::config::asio> server;

EventQueue queue;                 // Cola global de eventos
std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients;
std::mutex clients_mutex;

// ======================================================
// Función que publica eventos de forma periódica
// ======================================================
void publisher() {
    for (int i = 1; i <= 15; ++i) {
        std::string msg = "Evento " + std::to_string(i);
        Event event{msg};                  // ✅ creamos un objeto Event
        queue.push(event);                 // lo enviamos a la cola
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "[Publisher] Finalizado.\n";
}

// ======================================================
// Broker: extrae eventos de la cola y los envía a todos los clientes conectados
// ======================================================
void broker(server* s) {
    while (true) {
        Event event = queue.pop();
        std::string payload = event.data;

        std::lock_guard<std::mutex> lock(clients_mutex);
        for (auto& hdl : clients) {
            s->send(hdl, payload, websocketpp::frame::opcode::text);
        }

        std::cout << "[Broker] Enviado: " << payload << std::endl;
    }
}

// ======================================================
// Manejadores del servidor WebSocket
// ======================================================
void on_open(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.insert(hdl);
    std::cout << "[Server] Cliente conectado.\n";
}

void on_close(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.erase(hdl);
    std::cout << "[Server] Cliente desconectado.\n";
}

// ======================================================
// Función principal
// ======================================================
int main() {
    server print_server;

    try {
        print_server.set_access_channels(websocketpp::log::alevel::none);
        print_server.init_asio();

        print_server.set_open_handler(&on_open);
        print_server.set_close_handler(&on_close);

        print_server.listen(9002);
        print_server.start_accept();

        std::thread broker_thread(broker, &print_server);
        std::thread publisher_thread(publisher);

        std::cout << "[Server] Esperando conexiones en ws://localhost:9002 ...\n";
        print_server.run();

        broker_thread.join();
        publisher_thread.join();

    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << std::endl;
    }

    return 0;
}



