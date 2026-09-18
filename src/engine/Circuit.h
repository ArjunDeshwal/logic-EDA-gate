#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "core/Wire.h"
#include "components/Component.h"
#include "Simulator.h"

class Circuit {
public:
    Circuit() = default;
    ~Circuit();

    void addComponent(Component* comp);
    Wire* createOrGetWire(const std::string& id);
    Wire* getWire(const std::string& id);

    void connectInput(Component* comp, const std::string& wireId);
    void connectOutput(Component* comp, const std::string& wireId);

    // Topological sort for steady-state/initialization
    bool topologicalSort(std::vector<Component*>& sortedComps);

    // Run the event-driven simulation
    void simulate(uint64_t maxTime, const std::string& vcdFilename = "");

    // Apply a stimulus to a wire
    void applyStimulus(uint64_t time, const std::string& wireId, LogicState state);

    void addMonitor(const std::string& wireId);
    void printMonitors(uint64_t time);

private:
    std::unordered_map<std::string, Component*> components;
    std::unordered_map<std::string, Wire*> wires;
    std::vector<std::string> monitoredWires;
    Simulator simulator;
};

