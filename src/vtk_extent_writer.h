#ifndef VTK_EXTENT_BINARY_WRITER_H
#define VTK_EXTENT_BINARY_WRITER_H

#include <vector>
#include <string>

#ifndef EXTENT_H
#include "extent.h"
#endif

namespace Parfait {
    class VtkExtentWriter {
    public:
        VtkExtentWriter(std::string filename);
        void addExtent(const Extent &b);
        void addExtent(const Extent &b, int tag);
        void writeFile();
        static void writeExtents(std::string filename, std::vector<Extent> &boxes,
                                 std::vector<int> &tags);
    private:
        static void writeHeader(FILE *f, int numBoxes);
        static void writeTags(FILE *f, std::vector<int> &tags);
        std::vector<Extent> boxes;
        std::vector<int> tags;
        std::string filename;
    };
}

#endif
