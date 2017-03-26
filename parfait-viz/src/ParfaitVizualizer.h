#pragma once
#include <string>

class ParfaitVizualizer {
public:
    virtual void writeBinary(std::string filename) const = 0;
    virtual void writeBinary(std::string filename, int rank, int nproc) const = 0;

    virtual void addNodeData(std::string name,int* data,int number_of_components) = 0;
    virtual void addNodeData(std::string name,double* data,int number_of_components) = 0;
};

