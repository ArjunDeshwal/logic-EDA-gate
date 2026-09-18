#pragma once
#include "Types.h"
#include <string>
#include <vector>

class Component; // Forward declaration

class Wire {
public:
    Wire(std::string id) : id(std::move(id)), state(LogicState::UNKNOWN) {}

    std::string getId() const { return id; }
    LogicState getState() const { return state; }
    void setState(LogicState newState, uint64_t time) { 
        state = newState; 
        lastTransitionTime = time;
    }
    
    uint64_t getLastTransitionTime() const { return lastTransitionTime; }

    void addDrivenComponent(Component* comp) {
        drivenComponents.push_back(comp);
    }
    
    const std::vector<Component*>& getDrivenComponents() const {
        return drivenComponents;
    }

private:
    std::string id;
    LogicState state;
    uint64_t lastTransitionTime = 0;
    std::vector<Component*> drivenComponents; // Components connected to this wire's output
};

