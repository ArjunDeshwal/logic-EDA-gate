#include "NetlistParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "components/Gates.h"
#include "components/Sequential.h"

bool NetlistParser::parse(const std::string& filename, Circuit& circuit) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string type, name;
        iss >> type >> name;

        Component* comp = nullptr;
        if (type == "AND") comp = new AndGate(name);
        else if (type == "OR") comp = new OrGate(name);
        else if (type == "NOT") comp = new NotGate(name);
        else if (type == "NAND") comp = new NandGate(name);
        else if (type == "NOR") comp = new NorGate(name);
        else if (type == "XOR") comp = new XorGate(name);
        else if (type == "DLATCH") comp = new DLatch(name);
        else if (type == "DFF") comp = new DFlipFlop(name);
        else {
            std::cerr << "Unknown component type: " << type << std::endl;
            continue;
        }

        circuit.addComponent(comp);

        std::string pin;
        std::vector<std::string> pins;
        while (iss >> pin) {
            pins.push_back(pin);
        }

        if (pins.empty()) continue;

        // Last pin is always assumed to be output in this simple format
        for (size_t i = 0; i < pins.size() - 1; ++i) {
            circuit.connectInput(comp, pins[i]);
        }
        circuit.connectOutput(comp, pins.back());
    }

    return true;
}

