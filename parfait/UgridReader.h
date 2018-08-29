
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#pragma once
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <string>
namespace Parfait {
    namespace UgridReader {

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