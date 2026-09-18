#pragma once
#include <vector>
#include <string>
#include "core/Types.h"
#include "core/Wire.h"
#include "engine/Simulator.h"

class Component {
public:
    Component(std::string name, uint64_t delay) 
        : name(std::move(name)), intrinsicDelay(delay) {}
    virtual ~Component() = default;

    std::string getName() const { return name; }
    uint64_t getDelay() const { return intrinsicDelay; }

    void addInput(Wire* w) { inputs.push_back(w); }
    void addOutput(Wire* w) { 
        outputs.push_back(w); 
        // Note: Wire needs to know it's driven by this component in a full DAG traversal,
        // but for event simulation, inputs need to know which components to wake up.
        // So the simulator will connect Wires -> Components.
    }

    const std::vector<Wire*>& getInputs() const { return inputs; }
    const std::vector<Wire*>& getOutputs() const { return outputs; }

    // Evaluates logic based on current input wire states and schedules output events
    virtual void evaluate(Simulator& sim) = 0;

protected:
    std::string name;
    uint64_t intrinsicDelay;
    std::vector<Wire*> inputs;
    std::vector<Wire*> outputs;
};

