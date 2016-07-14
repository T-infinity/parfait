#pragma once
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <string>
namespace Parfait {
    namespace UgridReader {
        /// Tools for reading files in native endianness
        void readHeader(std::string filename, int &nnodes,
                        int &ntri, int &nquad,
                        int &ntet, int &npyr, int &prism, int &hex);
        // read all elements of a type
        std::vector<double> readNodes(std::string filename);
        std::vector<int> readTriangles(std::string filename);
        std::vector<int> readQuads(std::string filename);
        std::vector<int> readTets(std::string filename);
        std::vector<int> readPyramids(std::string filename);
        std::vector<int> readPrisms(std::string filename);
        std::vector<int> readHexs(std::string filename);
        std::vector<int> readTriangleBoundaryTags(std::string filename);
        std::vector<int> readQuadBoundaryTags(std::string filename);
        // read a slice of the elements of a type
        std::vector<double> readNodes(std::string filename,
                                      int begin, int end);
        std::vector<int> readTriangles(std::string filename,
                                       int begin, int end);
        std::vector<int> readQuads(std::string filename,
                                   int begin, int end);
        std::vector<int> readTets(std::string filename,
                                  int begin, int end);
        std::vector<int> readPyramids(std::string filename,
                                      int begin, int end);
        std::vector<int> readPrisms(std::string filename,
                                    int begin, int end);
        std::vector<int> readHexs(std::string filename,
                                  int begin, int end);
        std::vector<int> readBoundaryTags(std::string filename,
                                          int begin, int end);
        std::vector<int> readTriangleBoundaryTags(std::string filename,
                                                  int begin, int end);
        std::vector<int> readQuadBoundaryTags(std::string filename,
                                              int begin, int end);

        /// Same tools as above, but able to swap endianness
        void readHeader(std::string filename, int &nnodes,
                        int &ntri, int &nquad,
                        int &ntet, int &npyr, int &prism, int &hex, bool swapBytes);
        // read all elements of a type
        std::vector<double> readNodes(std::string filename, bool swapBytes);
        std::vector<int> readTriangles(std::string filename, bool swapBytes);
        std::vector<int> readQuads(std::string filename, bool swapBytes);
        std::vector<int> readTets(std::string filename, bool swapBytes);
        std::vector<int> readPyramids(std::string filename, bool swapBytes);
        std::vector<int> readPrisms(std::string filename, bool swapBytes);
        std::vector<int> readHexs(std::string filename, bool swapBytes);
        std::vector<int> readTriangleBoundaryTags(std::string filename, bool swapBytes);
        std::vector<int> readQuadBoundaryTags(std::string filename, bool swapBytes);
        // read a slice of the elements of a type
        std::vector<double> readNodes(std::string filename,
                                      int begin, int end, bool swapBytes);
        std::vector<int> readTriangles(std::string filename,
                                       int begin, int end, bool swapBytes);
        std::vector<int> readQuads(std::string filename,
                                   int begin, int end, bool swapBytes);
        std::vector<int> readTets(std::string filename,
                                  int begin, int end, bool swapBytes);
        std::vector<int> readPyramids(std::string filename,
                                      int begin, int end, bool swapBytes);
        std::vector<int> readPrisms(std::string filename,
                                    int begin, int end, bool swapBytes);
        std::vector<int> readHexs(std::string filename,
                                  int begin, int end, bool swapBytes);
        std::vector<int> readBoundaryTags(std::string filename,
                                          int begin, int end, bool swapBytes);
        std::vector<int> readTriangleBoundaryTags(std::string filename,
                                                  int begin, int end, bool swapBytes);
        std::vector<int> readQuadBoundaryTags(std::string filename,
                                              int begin, int end, bool swapBytes);
    }
}

#include "UgridReader.hpp"