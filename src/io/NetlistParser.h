#pragma once
#include <string>
#include "engine/Circuit.h"

class NetlistParser {
public:
    static bool parse(const std::string& filename, Circuit& circuit);
};

