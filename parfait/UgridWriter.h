
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
#include "ImportedUgrid.h"
namespace Parfait {
    namespace UgridWriter {
        void writeImportedUgrid(ImportedUgrid &ugrid, std::string filename, bool swapBytes);

        /// Tools for writing files in native endianness
        void writeHeader(std::string filename, int nnodes,
                         int ntri, int nquad,
                         int ntet, int npyr, int prism, int hex, bool swapBytes);

        void writeNodes(std::string filename, int nnodes, double *nodes, bool swapBytes);
        void writeTriangles(std::string filename, int ntriangles, int *triangles, bool swapBytes);
        void writeQuads(std::string filename, int nquads, int *quads, bool swapBytes);
        void writeTets(std::string filename, int ntets, int *tets, bool swapBytes);
        void writePyramids(std::string filename, int npyramyds, int *pyramids, bool swapBytes);
        void writePrisms(std::string filename, int nprisms, int *prisms, bool swapBytes);
        void writeHexs(std::string filename, int nhexs, int *hexs, bool swapBytes);
        void writeTriangleBoundaryTags(std::string filename, int ntriangles, int *triangleTags,
                                       bool swapBytes);
        void writeQuadBoundaryTags(std::string filename, int nquads, int *quadTags, bool swapBytes);

        void writeIntegerField(std::string filename, int n, int *fieldData, bool swapBytes);
    }

    class UgridWriterFactory {

    public:
        UgridWriterFactory() : doByteSwap(false) { }

        void setName(std::string fileName);

        void setTets(int *tets, int numberOfTets);
        void setHexes(int *hexes, int numberOfHexes);
        void setPyramids(int *pyramids, int numberOfPyramids);
        void setPrisms(int *prisms, int numberOfPrisms);
        void setTriangles(int *triangles, int numberOfTriangles);
        void setQuads(int *quads, int numberOfQuads);
        void setTriangleTags(int *triangleTags);
        void setQuadTags(int *quadTags);
        void writeFile();
        void setNodes(double *points, int numberOfNodes);
    private:
        bool doByteSwap;
        std::string fileName;
        int numberOfTets;
        int *tets;

        int numberOfHexes;
        int *hexes;

        int *pyramids;
        int numberOfPyramids;

        int *prisms;
        int numberOfPrisms;

        int *triangles;
        int numberOfTriangles;

        int numberOfQuads;
        int *quads;

        int *triangleTags;
        int *quadTags;

        int numberOfNodes;
        double *nodes;
    };
}

#include "UgridWriter.hpp"