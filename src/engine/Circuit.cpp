#include "Circuit.h"
#include "io/VcdWriter.h"
#include <iostream>
#include <queue>

Circuit::~Circuit() {
    for (auto& pair : components) delete pair.second;
    for (auto& pair : wires) delete pair.second;
}

void Circuit::addComponent(Component* comp) {
    components[comp->getName()] = comp;
}

Wire* Circuit::createOrGetWire(const std::string& id) {
    if (wires.find(id) == wires.end()) {
        wires[id] = new Wire(id);
    }
    return wires[id];
}

Wire* Circuit::getWire(const std::string& id) {
    auto it = wires.find(id);
    if (it != wires.end()) return it->second;
    return nullptr;
}

void Circuit::connectInput(Component* comp, const std::string& wireId) {
    Wire* w = createOrGetWire(wireId);
    comp->addInput(w);
    w->addDrivenComponent(comp);
}

void Circuit::connectOutput(Component* comp, const std::string& wireId) {
    Wire* w = createOrGetWire(wireId);
    comp->addOutput(w);
}

bool Circuit::topologicalSort(std::vector<Component*>& sortedComps) {
    std::unordered_map<Component*, int> inDegree;
    for (const auto& pair : components) {
        inDegree[pair.second] = 0;
    }

    for (const auto& pair : components) {
        for (Wire* outWire : pair.second->getOutputs()) {
            for (Component* driven : outWire->getDrivenComponents()) {
                inDegree[driven]++;
            }
        }
    }

    std::queue<Component*> q;
    for (const auto& pair : inDegree) {
        if (pair.second == 0) q.push(pair.first);
    }

    int count = 0;
    while (!q.empty()) {
        Component* curr = q.front();
        q.pop();
        sortedComps.push_back(curr);
        
        for (Wire* outWire : curr->getOutputs()) {
            for (Component* driven : outWire->getDrivenComponents()) {
                if (--inDegree[driven] == 0) {
                    q.push(driven);
                }
            }
        }
        count++;
    }

    return count == components.size(); // returns false if cycle exists (combinational loop)
}

void Circuit::applyStimulus(uint64_t time, const std::string& wireId, LogicState state) {
    simulator.scheduleEvent(time, wireId, state);
}

void Circuit::addMonitor(const std::string& wireId) {
    monitoredWires.push_back(wireId);
}

void Circuit::printMonitors(uint64_t time) {
    std::cout << "Time " << time << ": ";
    for (const auto& id : monitoredWires) {
        Wire* w = wires[id];
        std::cout << id << "=" << stateToString(w->getState()) << " ";
    }
    std::cout << "\n";
}

void Circuit::simulate(uint64_t maxTime, const std::string& vcdFilename) {
    std::cout << "Starting simulation..." << std::endl;
    
    // 1. COMBINATIONAL LOOP DETECTION (The Algorithmic Flex)
    std::vector<Component*> sorted;
    if (!topologicalSort(sorted)) {
        std::cerr << "\n[FATAL ERROR] Combinational Loop Detected! Simulation aborted to prevent infinite loop.\n" << std::endl;
        return;
    }

    // 2. VCD GENERATION SETUP (The Visual Flex)
    std::vector<Wire*> allWires;
    for (auto& pair : wires) allWires.push_back(pair.second);
    
    std::unique_ptr<VcdWriter> vcd;
    if (!vcdFilename.empty()) {
        vcd = std::make_unique<VcdWriter>(vcdFilename, allWires);
    }

    // Initial print
    printMonitors(0);

    while (simulator.hasEvents()) {
        Event ev = simulator.popNextEvent();
        
        if (ev.timestamp > maxTime) break;
        
        simulator.setCurrentTime(ev.timestamp);

        Wire* w = wires[ev.wireId];
        if (w && w->getState() != ev.newState) {
            
            // 3. STATIC HAZARD / GLITCH DETECTION (The Hardware Flex)
            // If the wire changed state again within a very small time window (e.g. < 5 delay units)
            if (ev.timestamp > 0 && (ev.timestamp - w->getLastTransitionTime()) < 5) {
                std::cout << "\n[WARNING] Static Hazard (Glitch) detected on wire '" 
                          << ev.wireId << "' at time " << ev.timestamp << " ns!\n";
            }

            w->setState(ev.newState, ev.timestamp);
            
            if (vcd) {
                vcd->writeChange(ev.timestamp, w, ev.newState);
            }
            
            // Print only when monitored wire changes
            bool isMonitored = false;
            for (const auto& id : monitoredWires) {
                if (id == ev.wireId) { isMonitored = true; break; }
            }
            if (isMonitored) {
                printMonitors(ev.timestamp);
            }

            // Wake up driven components
            for (Component* comp : w->getDrivenComponents()) {
                comp->evaluate(simulator);
            }
        }
    }
    std::cout << "Simulation complete. Final time: " << simulator.getCurrentTime() << std::endl;
}

