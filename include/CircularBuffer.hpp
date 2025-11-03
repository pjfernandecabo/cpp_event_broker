// include/CircularBuffer.hpp
#pragma once
#include <array>
#include <cstddef>

template <typename T, size_t N>
class CircularBuffer {
private:
    std::array<T, N> data;
    size_t head = 0;
    size_t count = 0;

public:
    void push(const T& value) {
        data[head] = value;
        head = (head + 1) % N;
        if (count < N) count++;
    }

    size_t size() const { return count; }

    // --- Iterador interno ---
    class iterator {
        CircularBuffer& buf;
        size_t index;
    public:
        iterator(CircularBuffer& b, size_t i) : buf(b), index(i) {}
        bool operator!=(const iterator& other) const { return index != other.index; }
        T& operator*() { return buf.data[index % N]; }
        iterator& operator++() { index++; return *this; }
    };

    iterator begin() { return iterator(*this, 0); }
    iterator end() { return iterator(*this, count); }
};
