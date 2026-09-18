#pragma once
#include <string>

enum class LogicState {
    LOW,      // 0
    HIGH,     // 1
    UNKNOWN,  // X
    HIGH_Z    // Z
};

inline std::string stateToString(LogicState state) {
    switch (state) {
        case LogicState::LOW: return "0";
        case LogicState::HIGH: return "1";
        case LogicState::UNKNOWN: return "X";
        case LogicState::HIGH_Z: return "Z";
        default: return "?";
    }
}

