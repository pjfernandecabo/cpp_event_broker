// include/EventBuffer.hpp
#pragma once
#include <string>
#include "CircularBuffer.hpp"

class EventBuffer {
private:
    CircularBuffer<std::string, 10> buffer;

public:
    void add(const std::string& e) { buffer.push(e); }

    auto begin() { return buffer.begin(); }
    auto end() { return buffer.end(); }
};
