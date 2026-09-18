#include "Gates.h"

// Helper to check for UNKNOWN states
static bool hasUnknown(const std::vector<Wire*>& inputs) {
    for (auto* w : inputs) {
        if (w->getState() == LogicState::UNKNOWN || w->getState() == LogicState::HIGH_Z) {
            return true;
        }
    }
    return false;
}

void AndGate::evaluate(Simulator& sim) {
    if (outputs.empty()) return;
    LogicState result = LogicState::HIGH;
    if (hasUnknown(inputs)) {
        // Optimization: if any input is LOW, AND is LOW even if others are UNKNOWN
        bool anyLow = false;
        for (auto* w : inputs) {
            if (w->getState() == LogicState::LOW) anyLow = true;
        }
        result = anyLow ? LogicState::LOW : LogicState::UNKNOWN;
    } else {
        for (auto* w : inputs) {
            if (w->getState() == LogicState::LOW) {
                result = LogicState::LOW;
                break;
            }
        }
    }
    sim.scheduleEvent(intrinsicDelay, outputs[0]->getId(), result);
}

void OrGate::evaluate(Simulator& sim) {
    if (outputs.empty()) return;
    LogicState result = LogicState::LOW;
    if (hasUnknown(inputs)) {
        bool anyHigh = false;
        for (auto* w : inputs) {
            if (w->getState() == LogicState::HIGH) anyHigh = true;
        }
        result = anyHigh ? LogicState::HIGH : LogicState::UNKNOWN;
    } else {
        for (auto* w : inputs) {
            if (w->getState() == LogicState::HIGH) {
                result = LogicState::HIGH;
                break;
            }
        }
    }
    sim.scheduleEvent(intrinsicDelay, outputs[0]->getId(), result);
}

void NotGate::evaluate(Simulator& sim) {
    if (outputs.empty() || inputs.empty()) return;
    LogicState result = LogicState::UNKNOWN;
    LogicState inState = inputs[0]->getState();
    
    if (inState == LogicState::HIGH) result = LogicState::LOW;
    else if (inState == LogicState::LOW) result = LogicState::HIGH;
    
    sim.scheduleEvent(intrinsicDelay, outputs[0]->getId(), result);
}

void NandGate::evaluate(Simulator& sim) {
    if (outputs.empty()) return;
    LogicState result = LogicState::LOW;
    if (hasUnknown(inputs)) {
        bool anyLow = false;
        for (auto* w : inputs) {
            if (w->getState() == LogicState::LOW) anyLow = true;
        }
        result = anyLow ? LogicState::HIGH : LogicState::UNKNOWN;
    } else {
        for (auto* w : inputs) {
            if (w->getState() == LogicState::LOW) {
                result = LogicState::HIGH;
                break;
            }
        }
    }
    sim.scheduleEvent(intrinsicDelay, outputs[0]->getId(), result);
}

void NorGate::evaluate(Simulator& sim) {
    if (outputs.empty()) return;
    LogicState result = LogicState::HIGH;
    if (hasUnknown(inputs)) {
        bool anyHigh = false;
        for (auto* w : inputs) {
            if (w->getState() == LogicState::HIGH) anyHigh = true;
        }
        result = anyHigh ? LogicState::LOW : LogicState::UNKNOWN;
    } else {
        for (auto* w : inputs) {
            if (w->getState() == LogicState::HIGH) {
                result = LogicState::LOW;
                break;
            }
        }
    }
    sim.scheduleEvent(intrinsicDelay, outputs[0]->getId(), result);
}

void XorGate::evaluate(Simulator& sim) {
    if (outputs.empty()) return;
    if (hasUnknown(inputs)) {
        sim.scheduleEvent(intrinsicDelay, outputs[0]->getId(), LogicState::UNKNOWN);
        return;
    }
    
    int highCount = 0;
    for (auto* w : inputs) {
        if (w->getState() == LogicState::HIGH) {
            highCount++;
        }
    }
    LogicState result = (highCount % 2 == 1) ? LogicState::HIGH : LogicState::LOW;
    sim.scheduleEvent(intrinsicDelay, outputs[0]->getId(), result);
}

