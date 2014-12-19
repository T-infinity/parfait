#ifndef IMPORTED_UGRID_FACTORY_H
#define IMPORTED_UGRID_FACTORY_H

#ifndef UGRID_READER_H
#include "ugrid_reader.h"
#endif

#ifndef IMPORTED_UGRID_H
#include "imported_ugrid.h"
#endif

#include <vector>
#include <string>
#include <stdexcept>

using std::vector;

class ImportedUgridFactory
{
    public:
        static ImportedUgrid readUgrid(std::string filename);
        static ImportedUgrid readUgrid(std::string filename,bool isBigEndian);
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

inline ImportedUgrid ImportedUgridFactory::readUgrid(std::string filename)
{
	return readUgrid(filename,false);
}

inline void ImportedUgridFactory::createBoundaryConditionsFromTags(
		std::string filename,
		std::vector<int> &triangleTags,
		std::vector<int> &quadTags,
		std::vector<int> &triangleBoundaryConditions,
		std::vector<int> &quadBoundaryConditions
		){

	std::string mapbcFile = filename;
	for(int i=0;i<5;i++)
		mapbcFile.pop_back();
	mapbcFile += "mapbc";
	MapbcReader reader(mapbcFile);

	triangleBoundaryConditions.clear();
	triangleBoundaryConditions.reserve(triangleTags.size());
	quadBoundaryConditions.clear();
	quadBoundaryConditions.reserve(quadTags.size());
	
	for(int tag:triangleTags)
		triangleBoundaryConditions.push_back(reader.boundaryCondition(tag));
	for(int tag:quadTags)
		quadBoundaryConditions.push_back(reader.boundaryCondition(tag));
}

inline ImportedUgrid ImportedUgridFactory::readUgrid(std::string filename,bool isBigEndian)
{
    vector<double> nodes;
    vector<int> cellMap;
    vector<int> faceMap;
    vector<int> triangles;
    vector<int> quads;
    vector<int> tets;
    vector<int> pyramids;
    vector<int> prisms;
    vector<int> hexs;
    vector<int> triangleTags;
    vector<int> quadTags;
	vector<int> triangleBoundaryConditions;
	vector<int> quadBoundaryConditions;

	using namespace UgridReader;
	nodes        = readNodes(filename,isBigEndian);
	triangles    = readTriangles(filename,isBigEndian);
	quads        = readQuads(filename,isBigEndian);
	tets         = readTets(filename,isBigEndian);
	pyramids     = readPyramids(filename,isBigEndian);
	prisms       = readPrisms(filename,isBigEndian);
	hexs         = readHexs(filename,isBigEndian);
    triangleTags = readTriangleBoundaryTags(filename,isBigEndian);
    quadTags     = readQuadBoundaryTags(filename,isBigEndian);
	
	createBoundaryConditionsFromTags(filename,
									triangleTags,quadTags,
									triangleBoundaryConditions,
									quadBoundaryConditions);


	return ImportedUgrid(nodes,triangles,quads,tets,pyramids,prisms,hexs, 
			triangleTags, quadTags,
			triangleBoundaryConditions,
			quadBoundaryConditions);
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

inline ImportedUgrid ImportedUgridFactory::readUgridAscii(std::string filename)
{

    vector<double> nodes;
    vector<int> cellMap;
    vector<int> faceMap;
    vector<int> triangles;
    vector<int> quads;
    vector<int> tets;
    vector<int> pyramids;
    vector<int> prisms;
    vector<int> hexs;

    vector<int> triangleTags;
    vector<int> quadTags;
	vector<int> triangleBoundaryConditions;
	vector<int> quadBoundaryConditions;

    FILE *fp = fopen(filename.c_str(), "r");
    if(fp == NULL){
        throw std::domain_error("Could not open .ugrid file: "+filename);
    }
    int numNodes, numTris, numQuads, numTets, numPyramids, numPrisms, numHexs;
    fscanf(fp,"%d %d %d %d %d %d %d\n",
            &numNodes,
            &numTris,
            &numQuads,
            &numTets,
            &numPyramids,
            &numPrisms,
            &numHexs
          );

    nodes.resize(3*numNodes);
    triangles.resize(3*numTris);
    quads.resize(4*numQuads);
    tets.resize(4*numTets);
    pyramids.resize(5*numPyramids);
    prisms.resize(6*numPrisms);
    hexs.resize(8*numHexs);
    triangleTags.resize(numTris);
    quadTags.resize(numQuads);

    for (int nodeId = 0; nodeId < numNodes; nodeId++){
        fscanf(fp, "%lf %lf %lf",
                &nodes[3*nodeId + 0],
                &nodes[3*nodeId + 1],
                &nodes[3*nodeId + 2]
              );
    }

    for(int triId = 0; triId < triangles.size()/3; triId++){
        for(int i = 0; i < 3; i++){
            fscanf(fp, "%d",&triangles[3*triId+i]);
            triangles[3*triId+i]--;
        }
    }
    for(int quadId = 0; quadId < quads.size()/4; quadId++){
        for(int i = 0; i < 4; i++){
            fscanf(fp, "%d",&quads[4*quadId+i]);
            quads[4*quadId+i]--;
        }
    }


    for(int triId = 0; triId < triangles.size()/3; triId++){
        fscanf(fp,"%d",&triangleTags[triId]);
    }
    for(int quadId = 0; quadId < quads.size()/4; quadId++){
        fscanf(fp,"%d",&quadTags[quadId]);
    }


    for(int elem = 0; elem < tets.size()/4; elem++){
        for(int i = 0; i < 4; i++){
            fscanf(fp,"%d",&tets[4*elem+i]);
            tets[4*elem+i]--;
        }
    }

    for(int elem = 0; elem < pyramids.size()/5; elem++){
        for(int i = 0; i < 5; i++){
            fscanf(fp,"%d",&pyramids[5*elem+i]);
            pyramids[5*elem+i]--;
        }
    }

    for(int elem = 0; elem < prisms.size()/6; elem++){
        for(int i = 0; i < 6; i++){
            fscanf(fp,"%d",&prisms[6*elem+i]);
            prisms[6*elem+i]--;
        }
    }

    for(int elem = 0; elem < hexs.size()/8; elem++){
        for(int i = 0; i < 8; i++){
            fscanf(fp,"%d",&hexs[8*elem+i]);
            hexs[8*elem+i]--;
        }
    }
	
	createBoundaryConditionsFromTags(filename,
									triangleTags,quadTags,
									triangleBoundaryConditions,
									quadBoundaryConditions);

    fclose(fp);
    return ImportedUgrid(nodes,triangles,quads,tets,pyramids,prisms,hexs, 
			triangleTags, quadTags,
			triangleBoundaryConditions,
			quadBoundaryConditions);
}

#endif
