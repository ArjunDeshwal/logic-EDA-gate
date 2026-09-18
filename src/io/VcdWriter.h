#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "core/Wire.h"

class VcdWriter {
public:
    VcdWriter(const std::string& filename, const std::vector<Wire*>& wires);
    ~VcdWriter();
    void writeChange(uint64_t time, Wire* wire, LogicState state);

private:
    std::ofstream file;
    uint64_t lastTime = 0;
    std::unordered_map<std::string, std::string> wireMap;
};

