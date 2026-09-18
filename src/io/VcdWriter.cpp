#include "VcdWriter.h"
#include <iostream>

VcdWriter::VcdWriter(const std::string& filename, const std::vector<Wire*>& wires) {
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open VCD file." << std::endl;
        return;
    }
    
    file << "$date Today $end\n";
    file << "$version LogicSimulator 1.0 $end\n";
    file << "$timescale 1ns $end\n";
    
    file << "$scope module TOP $end\n";
    
    char symbol = '!'; // Start of printable ASCII chars used by VCD
    for (Wire* w : wires) {
        std::string symStr(1, symbol++);
        wireMap[w->getId()] = symStr;
        file << "$var wire 1 " << symStr << " " << w->getId() << " $end\n";
    }
    file << "$upscope $end\n";
    file << "$enddefinitions $end\n";
    
    // Dump initial values
    file << "$dumpvars\n";
    for (Wire* w : wires) {
        file << "x" << wireMap[w->getId()] << "\n";
    }
    file << "$end\n";
}

VcdWriter::~VcdWriter() {
    if (file.is_open()) file.close();
}

void VcdWriter::writeChange(uint64_t time, Wire* wire, LogicState state) {
    if (!file.is_open()) return;
    
    // VCD time must strictly increase
    if (time > lastTime) {
        file << "#" << time << "\n";
        lastTime = time;
    } else if (time == 0 && lastTime == 0) {
        file << "#" << time << "\n";
    }
    
    char stateChar = 'x';
    if (state == LogicState::LOW) stateChar = '0';
    else if (state == LogicState::HIGH) stateChar = '1';
    else if (state == LogicState::HIGH_Z) stateChar = 'z';
    
    file << stateChar << wireMap[wire->getId()] << "\n";
}

