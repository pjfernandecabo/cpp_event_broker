#include <iostream>
#include <memory>   // para std::unique_ptr
#include <string>
using namespace std;

template <typename T>
class Vector {
private:
    unique_ptr<T[]> datos;  // array dinámico seguro
    size_t capacidad;
    size_t longitud;

    void redimensionar(size_t nueva_capacidad) {
        unique_ptr<T[]> nuevo = make_unique<T[]>(nueva_capacidad);
        for (size_t i = 0; i < longitud; ++i)
            nuevo[i] = datos[i];
        datos = move(nuevo);
        capacidad = nueva_capacidad;
    }

public:
    Vector() : capacidad(2), longitud(0), datos(make_unique<T[]>(2)) {}

    void push_back(const T& valor) {
        if (longitud == capacidad)
            redimensionar(capacidad * 2);
        datos[longitud++] = valor;
    }

    T& operator[](size_t indice) {
        if (indice >= longitud)
            throw out_of_range("Índice fuera de rango");
        return datos[indice];
    }

    const T& operator[](size_t indice) const {
        if (indice >= longitud)
            throw out_of_range("Índice fuera de rango");
        return datos[indice];
    }

    size_t size() const { return longitud; }

    void mostrar() const {
        cout << "[ ";
        for (size_t i = 0; i < longitud; ++i)
            cout << datos[i] << (i < longitud - 1 ? ", " : "");
        cout << " ]" << endl;
    }
};

int main() {
    try {
        Vector<int> numeros;
        numeros.push_back(10);
        numeros.push_back(20);
        numeros.push_back(30);
        cout << "Vector<int>: ";
        numeros.mostrar();

        Vector<string> palabras;
        palabras.push_back("Hola");
        palabras.push_back("Templates");
        palabras.push_back("C++");
        cout << "Vector<string>: ";
        palabras.mostrar();

        Vector<double> decimales;
        decimales.push_back(3.14);
        decimales.push_back(2.718);
        decimales.push_back(1.618);
        cout << "Vector<double>: ";
        decimales.mostrar();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
