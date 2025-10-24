#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    map<string, double> productos;
    productos["Manzana"] = 1.2;
    productos["Pera"] = 0.9;
    productos["Plátano"] = 1.5;

    // Mostrar
    cout << "Productos disponibles:\n";
    for (auto& [nombre, precio] : productos)
        cout << " - " << nombre << ": " << precio << " €" << endl;

    // Añadir nuevo producto
    string nuevo = "Naranja";
    double precio = 1.1;
    productos[nuevo] = precio;

    cout << "\nTras añadir " << nuevo << ":\n";
    for (auto& [nombre, precio] : productos)
        cout << " - " << nombre << ": " << precio << " €" << endl;

    // Calcular promedio
    double suma = 0.0;
    for (auto& [_, p] : productos)
        suma += p;
    cout << "\nPrecio medio: " << suma / productos.size() << " €" << endl;

    // Eliminar producto
    productos.erase("Pera");

    cout << "\nTras eliminar 'Pera':\n";
    for (auto& [nombre, precio] : productos)
        cout << " - " << nombre << ": " << precio << " €" << endl;

    return 0;
}
