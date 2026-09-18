#pragma once
#include "Component.h"

class AndGate : public Component {
public:
    AndGate(std::string name, uint64_t delay = 10) : Component(std::move(name), delay) {}
    void evaluate(Simulator& sim) override;
};

class OrGate : public Component {
public:
    OrGate(std::string name, uint64_t delay = 10) : Component(std::move(name), delay) {}
    void evaluate(Simulator& sim) override;
};

class NotGate : public Component {
public:
    NotGate(std::string name, uint64_t delay = 5) : Component(std::move(name), delay) {}
    void evaluate(Simulator& sim) override;
};

class NandGate : public Component {
public:
    NandGate(std::string name, uint64_t delay = 8) : Component(std::move(name), delay) {}
    void evaluate(Simulator& sim) override;
};

class NorGate : public Component {
public:
    NorGate(std::string name, uint64_t delay = 8) : Component(std::move(name), delay) {}
    void evaluate(Simulator& sim) override;
};

class XorGate : public Component {
public:
    XorGate(std::string name, uint64_t delay = 15) : Component(std::move(name), delay) {}
    void evaluate(Simulator& sim) override;
};

