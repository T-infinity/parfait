
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

#include "UgridReader.h"
#include "ImportedUgrid.h"
#include <vector>
#include <string>
#include <stdexcept>

namespace Parfait {
    class ImportedUgridFactory {
    public:
        static ImportedUgrid readUgrid(std::string filename);
        static ImportedUgrid readUgrid(std::string filename, bool isBigEndian);
        //static ImportedUgrid readUgrid(std::vector<std::string> &filenames,
        //							   std::vector<bool> &isBigEndian);
        static ImportedUgrid readUgridAscii(std::string filename);

        static void createBoundaryConditionsFromTags(
                std::string filename,
                std::vector<int> &triangleTags,
                std::vector<int> &quadTags,
                std::vector<int> &triangleBoundaryConditions,
                std::vector<int> &quadBoundaryTags
        );
    };

    inline ImportedUgrid ImportedUgridFactory::readUgrid(std::string filename) {
        return readUgrid(filename, false);
    }

    inline void ImportedUgridFactory::createBoundaryConditionsFromTags(
            std::string filename,
            std::vector<int> &triangleTags,
            std::vector<int> &quadTags,
            std::vector<int> &triangleBoundaryConditions,
            std::vector<int> &quadBoundaryConditions
    ) {

        std::string mapbcFile = filename;
        mapbcFile.resize(filename.size() - 5);
        auto position = mapbcFile.find("lb8.");
        if (std::string::npos != position)
            mapbcFile.resize(position);
        position = mapbcFile.find("b8.");
        if (std::string::npos != position)
            mapbcFile.resize(position);
        mapbcFile += "mapbc";
        MapbcReader reader(mapbcFile);

        triangleBoundaryConditions.clear();
        triangleBoundaryConditions.reserve(triangleTags.size());
        quadBoundaryConditions.clear();
        quadBoundaryConditions.reserve(quadTags.size());

        for (int tag:triangleTags)
            triangleBoundaryConditions.push_back(reader.boundaryCondition(tag));
        for (int tag:quadTags)
            quadBoundaryConditions.push_back(reader.boundaryCondition(tag));
    }

    inline ImportedUgrid ImportedUgridFactory::readUgrid(std::string filename, bool isBigEndian) {
        std::vector<double> nodes;
        std::vector<int> cellMap;
        std::vector<int> faceMap;
        std::vector<int> triangles;
        std::vector<int> quads;
        std::vector<int> tets;
        std::vector<int> pyramids;
        std::vector<int> prisms;
        std::vector<int> hexs;
        std::vector<int> triangleTags;
        std::vector<int> quadTags;
        std::vector<int> triangleBoundaryConditions;
        std::vector<int> quadBoundaryConditions;

        using namespace UgridReader;
        nodes = readNodes(filename, isBigEndian);
        triangles = readTriangles(filename, isBigEndian);
        quads = readQuads(filename, isBigEndian);
        tets = readTets(filename, isBigEndian);
        pyramids = readPyramids(filename, isBigEndian);
        prisms = readPrisms(filename, isBigEndian);
        hexs = readHexs(filename, isBigEndian);
        triangleTags = readTriangleBoundaryTags(filename, isBigEndian);
        quadTags = readQuadBoundaryTags(filename, isBigEndian);

        createBoundaryConditionsFromTags(filename,
                                         triangleTags, quadTags,
                                         triangleBoundaryConditions,
                                         quadBoundaryConditions);


        return ImportedUgrid(nodes, triangles, quads, tets, pyramids, prisms, hexs,
                             triangleTags, quadTags);
    }

#if 0
inline ImportedUgrid ImportedUgridFactory::readUgrid(std::vector<std::string> &filenames,
		vector<bool> &isBigEndian)
{
	std::vector<ImportedUgrid> grids;
	for(int i=0;i<filenames.size();i++)
		grids.push_back(readUgrid(filenames[i],isBigEndian[i]));
}
#endif

    inline ImportedUgrid ImportedUgridFactory::readUgridAscii(std::string filename) {

        std::vector<double> nodes;
        std::vector<int> cellMap;
        std::vector<int> faceMap;
        std::vector<int> triangles;
        std::vector<int> quads;
        std::vector<int> tets;
        std::vector<int> pyramids;
        std::vector<int> prisms;
        std::vector<int> hexs;

        std::vector<int> triangleTags;
        std::vector<int> quadTags;
        std::vector<int> triangleBoundaryConditions;
        std::vector<int> quadBoundaryConditions;

        FILE *fp = fopen(filename.c_str(), "r");
        if (fp == NULL) {
            throw std::domain_error("Could not open .ugrid file: " + filename);
        }
        int numNodes, numTris, numQuads, numTets, numPyramids, numPrisms, numHexs;
        fscanf(fp, "%d %d %d %d %d %d %d\n",
               &numNodes,
               &numTris,
               &numQuads,
               &numTets,
               &numPyramids,
               &numPrisms,
               &numHexs
        );

        nodes.resize(3 * numNodes);
        triangles.resize(3 * numTris);
        quads.resize(4 * numQuads);
        tets.resize(4 * numTets);
        pyramids.resize(5 * numPyramids);
        prisms.resize(6 * numPrisms);
        hexs.resize(8 * numHexs);
        triangleTags.resize(numTris);
        quadTags.resize(numQuads);

        for (int nodeId = 0; nodeId < numNodes; nodeId++) {
            fscanf(fp, "%lf %lf %lf",
                   &nodes[3 * nodeId + 0],
                   &nodes[3 * nodeId + 1],
                   &nodes[3 * nodeId + 2]
            );
        }

        for (unsigned int triId = 0; triId < triangles.size() / 3; triId++) {
            for (int i = 0; i < 3; i++) {
                fscanf(fp, "%d", &triangles[3 * triId + i]);
                triangles[3 * triId + i]--;
            }
        }
        for (unsigned int quadId = 0; quadId < quads.size() / 4; quadId++) {
            for (int i = 0; i < 4; i++) {
                fscanf(fp, "%d", &quads[4 * quadId + i]);
                quads[4 * quadId + i]--;
            }
        }


        for (unsigned int triId = 0; triId < triangles.size() / 3; triId++) {
            fscanf(fp, "%d", &triangleTags[triId]);
        }
        for (unsigned int quadId = 0; quadId < quads.size() / 4; quadId++) {
            fscanf(fp, "%d", &quadTags[quadId]);
        }


        for (unsigned int elem = 0; elem < tets.size() / 4; elem++) {
            for (int i = 0; i < 4; i++) {
                fscanf(fp, "%d", &tets[4 * elem + i]);
                tets[4 * elem + i]--;
            }
        }

        for (unsigned int elem = 0; elem < pyramids.size() / 5; elem++) {
            for (int i = 0; i < 5; i++) {
                fscanf(fp, "%d", &pyramids[5 * elem + i]);
                pyramids[5 * elem + i]--;
            }
        }

        for (unsigned int elem = 0; elem < prisms.size() / 6; elem++) {
            for (int i = 0; i < 6; i++) {
                fscanf(fp, "%d", &prisms[6 * elem + i]);
                prisms[6 * elem + i]--;
            }
        }

        for (unsigned int elem = 0; elem < hexs.size() / 8; elem++) {
            for (int i = 0; i < 8; i++) {
                fscanf(fp, "%d", &hexs[8 * elem + i]);
                hexs[8 * elem + i]--;
            }
        }

        createBoundaryConditionsFromTags(filename,
                                         triangleTags, quadTags,
                                         triangleBoundaryConditions,
                                         quadBoundaryConditions);

        fclose(fp);
        return ImportedUgrid(nodes, triangles, quads, tets, pyramids, prisms, hexs,
                             triangleTags, quadTags);
    }
}