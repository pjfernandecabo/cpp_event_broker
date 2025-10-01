#pragma once
#include <string>

struct Event {
    std::string topic;
    std::string payload;

    Event(const std::string& t, const std::string& p)
        : topic(t), payload(p) {}
};
