#include <iostream>
#include <vector>
#include "engine/Circuit.h"
#include "io/NetlistParser.h"

void testHalfAdder() {
    std::cout << "--- Testing Half Adder ---" << std::endl;
    Circuit circuit;
    if (!NetlistParser::parse("examples/half_adder.net", circuit)) return;

    // Monitor I/O
    circuit.addMonitor("A");
    circuit.addMonitor("B");
    circuit.addMonitor("SUM");
    circuit.addMonitor("CARRY");

    // Apply Stimuli (Time, Wire, State)
    circuit.applyStimulus(0, "A", LogicState::LOW);
    circuit.applyStimulus(0, "B", LogicState::LOW);
    
    circuit.applyStimulus(100, "A", LogicState::HIGH);
    circuit.applyStimulus(100, "B", LogicState::LOW);
    
    circuit.applyStimulus(200, "A", LogicState::LOW);
    circuit.applyStimulus(200, "B", LogicState::HIGH);
    
    circuit.applyStimulus(300, "A", LogicState::HIGH);
    circuit.applyStimulus(300, "B", LogicState::HIGH);

    circuit.simulate(500, "half_adder.vcd");
}

void testDFlipFlop() {
    std::cout << "\n--- Testing D Flip-Flop ---" << std::endl;
    Circuit circuit;
    if (!NetlistParser::parse("examples/d_flipflop.net", circuit)) return;

    circuit.addMonitor("CLK");
    circuit.addMonitor("D");
    circuit.addMonitor("Q");

    // Setup initial state
    circuit.applyStimulus(0, "CLK", LogicState::LOW);
    circuit.applyStimulus(0, "D", LogicState::LOW);

    // D changes, CLK is low (Q should not change)
    circuit.applyStimulus(50, "D", LogicState::HIGH);
    
    // Rising edge of CLK (Q should become HIGH after delay)
    circuit.applyStimulus(100, "CLK", LogicState::HIGH);
    
    // CLK goes low
    circuit.applyStimulus(150, "CLK", LogicState::LOW);
    
    // D changes to LOW, CLK is low (Q should not change)
    circuit.applyStimulus(200, "D", LogicState::LOW);
    
    // Rising edge of CLK (Q should become LOW)
    circuit.applyStimulus(250, "CLK", LogicState::HIGH);

    circuit.simulate(400, "d_flipflop.vcd");
}

void testCombinationalLoop() {
    std::cout << "\n--- Testing Combinational Loop ---" << std::endl;
    Circuit circuit;
    if (!NetlistParser::parse("examples/loop.net", circuit)) return;
    circuit.simulate(100); // Should abort gracefully
}

void testHazard() {
    std::cout << "\n--- Testing Static Hazard ---" << std::endl;
    Circuit circuit;
    circuit.applyStimulus(0, "FAST_WIRE", LogicState::LOW);
    circuit.applyStimulus(10, "FAST_WIRE", LogicState::HIGH);
    circuit.applyStimulus(12, "FAST_WIRE", LogicState::LOW); // Changed in 2ns, should glitch
    circuit.simulate(50);
}

int main() {
    testHalfAdder();
    testDFlipFlop();
    testCombinationalLoop();
    testHazard();

    return 0;
}

