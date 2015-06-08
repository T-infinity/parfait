#ifndef VTK_HEX_WRITER_H
#define VTK_HEX_WRITER_H

#include <vector>
#include <string>

#ifndef __HEX_H
#include "hex.h"
#endif

namespace Parfait{
    class VtkHexWriter {
    public:
        VtkHexWriter(std::string filename);
        void addHex(Hex b);
        void addHex(Hex b, int tag);
        void writeFile();
        static void writeBoxes(std::string filename, std::vector<Hex> &boxes, std::vector<int> &tags);
    private:
        std::vector<Hex> boxes;
        std::vector<int> tags;
        std::string filename;
    };
}

#endif
