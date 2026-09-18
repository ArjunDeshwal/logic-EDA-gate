#pragma once
#include <queue>
#include <vector>
#include <functional>
#include "core/Event.h"

class Simulator {
public:
    Simulator() : currentTime(0) {}

    void scheduleEvent(uint64_t delay, const std::string& wireId, LogicState newState) {
        eventQueue.emplace(currentTime + delay, wireId, newState);
    }

    bool hasEvents() const { return !eventQueue.empty(); }
    
    Event popNextEvent() {
        Event e = eventQueue.top();
        eventQueue.pop();
        return e;
    }

    uint64_t getCurrentTime() const { return currentTime; }
    void setCurrentTime(uint64_t t) { currentTime = t; }

private:
    uint64_t currentTime;
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> eventQueue;
};

