#include <iostream>
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

    Logger logger1("A");

    {
        Logger logger2("B");
        Logger logger3("C");
        logger2.log("Mensaje B");
        logger3.log("Mensaje C");
        std::cout << "Fin del bloque interno" << std::endl;
    }

    logger1.log("Mensaje A");
    std::cout << "Fin main" << std::endl;

    return 0;
}
