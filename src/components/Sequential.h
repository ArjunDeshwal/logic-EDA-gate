#pragma once
#include "Component.h"

// D-Latch (Level sensitive)
class DLatch : public Component {
public:
    DLatch(std::string name, uint64_t delay = 12);
    void evaluate(Simulator& sim) override;

private:
    LogicState internalState = LogicState::UNKNOWN;
};

// D-FlipFlop (Edge triggered)
class DFlipFlop : public Component {
public:
    DFlipFlop(std::string name, uint64_t delay = 15);
    void evaluate(Simulator& sim) override;

private:
    LogicState internalState = LogicState::UNKNOWN;
    LogicState lastClock = LogicState::UNKNOWN;
};

