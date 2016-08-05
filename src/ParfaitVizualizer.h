#pragma once
#include <string>

class ParfaitVizualizer {
public:
    virtual void writeBinary(std::string filename) const = 0;
};

