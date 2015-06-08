#ifndef VTK_FIELD_H
#define VTK_FIELD_H

#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include <vector>
#include <string>

#define INT32_FIELD 301
#define FLOAT32_FIELD 302
#define FLOAT64_FIELD 303

using std::vector;

namespace Parfait {
    template<typename T>
    class VtkField {
    public:
        VtkField(bool isNodeData, std::string fieldName, int blockSize, int nBlocks, T *data);
        VtkField(bool isNodeData, std::string fieldName, int blockSize, int nBlocks, T *data,
                 std::vector<int> &localIds);
        // get data at a single location (scalar data)
        T getElement(int id);
        // get pointer to block of data (vector data)
        const T *getBlock(int id);

        inline int Type() {
            T t;
            return Type(t);
        }

        std::string name();

        bool isNodeData() { return nodeBased; }

        void append(std::string filename);
    private:
        bool nodeBased;
        std::string fieldName;
        std::vector<int> localIds;
        int blockSize;
        int nBlocks;
        const T *data;

        inline int Type(int x) { return INT32_FIELD; }

        inline int Type(float x) { return FLOAT32_FIELD; }

        inline int Type(double x) { return FLOAT64_FIELD; }
    };
}
#include "vtk_field.hpp"

#endif
