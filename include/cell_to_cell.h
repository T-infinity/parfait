#ifndef CELL_TO_CELL_H
#define CELL_TO_CELL_H

#include <stdio.h>
#include <algorithm>
#include "vector_tools.h"
#include "generic_mesh.h"
#include <assert.h>
#include "adt_builder.h"
#include "extent_builder.h"
#include <algorithm>

#include <string>

using std::vector;
using std::string;


template<typename T>
bool facesMatch(T faceOne,T faceTwo)
{
	auto nodeList = faceOne.getNodes();
	std::sort(nodeList.begin(),nodeList.end());
	
	for(int node : faceTwo.getNodes())
		insertUnique(nodeList,node);
	
	if(faceOne.numberOfNodes() == nodeList.size())
		return true;
	return false;
}
#if 0
template <typename MeshType> 
std::vector<std::vector<int>> buildNodeToCell(MeshType &mesh_i){
    

    Mesh<MeshType> mesh(mesh_i);
    std::vector<std::vector<int>> nodeToCell(mesh.numberOfNodes());

    for(auto cell : mesh.cells()){
        auto cellNodes = cell.getNodes(); 
        for(int node : cellNodes){
            insertUnique(nodeToCell[node], cell.Id());
        }
    }
    return nodeToCell;
}
#endif

template <typename T> 
std::vector<std::vector<int>> buildCellToCell_NoAdt(T &meshInterface)
{
	Mesh<T> mesh(meshInterface);
	
	vector<vector<int> > c2c;
	c2c.resize(mesh.numberOfCells());
	printf("Building node2cell....\n");
	fflush(stdout);
    auto node2cell = buildNodeToCell(meshInterface);
	
	printf("populating...\n");
	fflush(stdout);
	// populate c2c connectivity
	for(auto cell:mesh.cells())
	{
		if(cell.Id() % 10000 == 0)
		{
			printf("---processed %i cells of %i\n",cell.Id(),mesh.numberOfCells());
			fflush(stdout);
		}
        auto cellNodes = cell.getNodes();
        std::vector<int> neighborMaybe;
        for(int cellNode : cellNodes)
        {
            for(int neighbor : node2cell[cellNode])
            {
                neighborMaybe.push_back(neighbor);
            }
        }
		for(int neighborId:neighborMaybe){
			if(neighborId == cell.Id())
				continue;
			for(auto face:cell)
			{
				for(auto nbrFace : mesh.cell(neighborId))
					if(facesMatch(face,nbrFace))
					{
						insertUnique(c2c[cell.Id()],neighborId);
						insertUnique(c2c[neighborId],cell.Id());
						break;
					}
			}
		}
	}
	return c2c;
}

template<typename T>
vector<vector<int> > buildCellToCell(T &meshInterface)
{
	Mesh<T> mesh(meshInterface);
	printf("Parfait-buildCellToCell: putting cells in ADT for searching\n");
	Adt3DExtent adt = AdtBuilder::putCellsInAdt(meshInterface);

	vector<vector<int> > c2c;
	c2c.resize(mesh.numberOfCells());
	
	printf("Parfait-buildCellToCell: populating c2c connectivity\n");
	// populate c2c connectivity
	for(auto cell:mesh.cells())
	{
		for(int neighborId:adt.retrieve(ExtentBuilder::build(cell)))
		{
			if(neighborId == cell.Id())
				continue;
			for(auto face:cell)
			{
				for(auto nbrFace : mesh.cell(neighborId))
					if(facesMatch(face,nbrFace))
					{
						insertUnique(c2c[cell.Id()],neighborId);
						insertUnique(c2c[neighborId],cell.Id());
						break;
					}
			}
		}
	}
	return c2c;
}

inline void plotConnectivityBandwidth(string filename, const vector<vector<int>> & e2e)
{
    FILE *fp = fopen(filename.c_str(), "w");

    for(int entity1 = 0; entity1 < e2e.size(); entity1++){
        fprintf(fp, "%d %d\n", entity1, entity1);
        for(int entity2 : e2e[entity1]){
            fprintf(fp, "%d %d\n", entity1, entity2); 
        }
    }

    fclose(fp);
}

#endif
