# Gate-Level Logic & Timing Simulator

## Overview
A custom Electronic Design Automation (EDA) simulation engine built from scratch in C++. This tool parses digital hardware netlists and simulates logic gate implementations, sequential memory elements, and precise signal propagation delays chronologically. 

It bridges the gap between software algorithms and hardware physics by modeling circuits as a **Directed Acyclic Graph (DAG)** and utilizing an **Event-Driven Simulation Engine**. This allows the tool to accurately replicate physical hardware concurrency and timing constraints.

## Features
- **Structural Netlist Parsing**: Dynamically reads text-based circuit netlists (e.g., `AND U1 IN_A IN_B OUT_Y`) and constructs an in-memory DAG.
- **Event-Driven Engine**: Utilizes a Min-Heap Priority Queue (`std::priority_queue`) to simulate hardware concurrency. Instead of brute-force calculating the circuit at every tick, it efficiently processes only state transitions (events).
- **Physical Delay Modeling**: Simulates intrinsic propagation delays for different logic gates, allowing for accurate chronological timing analysis.
- **Sequential & Combinational Logic**: Supports truth-table implementations for basic gates (AND, OR, NOT, NAND, NOR, XOR) and sequential edge-triggered elements (D-FlipFlops, Latches).
- **Combinational Loop Detection**: Implements Kahn's Algorithm (Topological Sort) to detect infinite feedback loops before simulation begins, ensuring stable execution and preventing infinite loops.
- **Static Hazard (Glitch) Detection**: Analyzes transient logic states to detect and warn users of race conditions and rapid, unintended signal toggles (glitches) caused by input skews.
- **VCD Waveform Generation**: Automatically dumps simulation results to `.vcd` (Value Change Dump) files, which can be viewed in industry-standard waveform viewers like GTKWave.

## Architecture

The project is structured into modular, object-oriented C++ components:

- **`src/core/`**: Defines fundamental logic states, `Wire` representations (DAG edges), and `Event` nodes for the Priority Queue.
- **`src/components/`**: Abstract Base `Component` definitions and derived truth-table implementations for digital gates.
- **`src/engine/`**: The core graph logic. `Circuit` manages the DAG and topological sorting, while `Simulator` drives the chronological Priority Queue time engine.
- **`src/io/`**: Contains the `NetlistParser` for generating the DAG from text, and the `VcdWriter` for waveform logging.

## Building and Running

Ensure you have **CMake (v3.10+)** and a **C++17** compatible compiler (GCC, Clang, or MSVC) installed.

```bash
# 1. Create a build directory
mkdir build
cd build

# 2. Generate build files and compile
cmake ..
cmake --build .

# 3. Run the executable (Linux/macOS)
./LogicSimulator

# 3. Run the executable (Windows)
.\Debug\LogicSimulator.exe
```

## Examples
The repository includes sample netlists in the `examples/` directory:
- `half_adder.net`: Demonstrates combinational logic delays and static hazard detection.
- `d_flipflop.net`: Demonstrates edge-triggered sequential logic isolating state.
- `loop.net`: Demonstrates the engine safely catching and aborting on an invalid combinational feedback loop.
# logic-EDA-gate
