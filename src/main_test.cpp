#include <iostream>
#include "CircularBuffer.hpp"

int main() {
    CircularBuffer<int, 5> buffer;
    for (int i = 1; i <= 7; ++i)
        buffer.push(i);

    for (auto n : buffer)
        std::cout << n << " ";
}
