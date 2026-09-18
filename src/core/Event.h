#pragma once
#include "Types.h"
#include <cstdint>
#include <string>

struct Event {
    uint64_t timestamp;
    std::string wireId;
    LogicState newState;

    Event(uint64_t t, std::string w, LogicState s) 
        : timestamp(t), wireId(std::move(w)), newState(s) {}

    // For priority queue: min-heap (earliest time first)
    bool operator>(const Event& other) const {
        return timestamp > other.timestamp;
    }
};

