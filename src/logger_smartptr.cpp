#include <iostream>
#include <memory>
#include <string>

class Logger {
public:
    Logger(const std::string &name) : name(name) {
        std::cout << "Logger " << name << " iniciado" << std::endl;
    }

    ~Logger() {
        std::cout << "Logger " << name << " terminado" << std::endl;
    }

    void log(const std::string &msg) {
        std::cout << "[" << name << "] " << msg << std::endl;
    }

private:
    std::string name;
};

int main() {
    std::cout << "Inicio main" << std::endl;

    // Logger con unique_ptr
    std::unique_ptr<Logger> logger1 = std::make_unique<Logger>("A");
    logger1->log("Mensaje A con unique_ptr");

    {
        // Logger con shared_ptr
        std::shared_ptr<Logger> logger2 = std::make_shared<Logger>("B");
        std::shared_ptr<Logger> logger3 = logger2; // logger3 comparte ownership
        logger2->log("Mensaje B");
        logger3->log("Mensaje compartido B");
        std::cout << "Fin del bloque interno" << std::endl;
    } // logger2 y logger3 se destruyen automáticamente aquí

    std::cout << "Fin main" << std::endl;
    return 0;
}
