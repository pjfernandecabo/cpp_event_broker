#include <iostream>
#include <string>

class Logger {
public:
    Logger() {
        std::cout << "Logger iniciado" << std::endl;
    }

    ~Logger() {
        std::cout << "Logger terminado" << std::endl;
    }

    void log(const std::string &msg) {
        std::cout << "[LOG] " << msg << std::endl;
    }
};

int main() {
    Logger logger;
    logger.log("Primer mensaje de prueba");
    return 0;
}
