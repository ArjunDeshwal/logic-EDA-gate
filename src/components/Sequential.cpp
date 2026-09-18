#include "Sequential.h"

// DLatch expects input[0] = D, input[1] = EN (Enable)
DLatch::DLatch(std::string name, uint64_t delay) : Component(std::move(name), delay) {}

void DLatch::evaluate(Simulator& sim) {
    if (inputs.size() < 2 || outputs.empty()) return;
    
    LogicState d = inputs[0]->getState();
    LogicState en = inputs[1]->getState();

    if (en == LogicState::HIGH) {
        if (d != internalState) {
            internalState = d;
            sim.scheduleEvent(intrinsicDelay, outputs[0]->getId(), internalState);
        }
    }
}

// DFlipFlop expects input[0] = D, input[1] = CLK
DFlipFlop::DFlipFlop(std::string name, uint64_t delay) : Component(std::move(name), delay) {}

void DFlipFlop::evaluate(Simulator& sim) {
    if (inputs.size() < 2 || outputs.empty()) return;

    LogicState d = inputs[0]->getState();
    LogicState clk = inputs[1]->getState();

    // Check for rising edge on CLK
    if (lastClock == LogicState::LOW && clk == LogicState::HIGH) {
        if (d != internalState) {
            internalState = d;
            sim.scheduleEvent(intrinsicDelay, outputs[0]->getId(), internalState);
        }
    }
    
    lastClock = clk;
}

