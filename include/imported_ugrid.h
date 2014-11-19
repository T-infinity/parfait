#ifndef IMPORTED_UGRID_H
#define IMPORTED_UGRID_H
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include "mapbc_reader.h"

using std::vector;

//-------------------------------------------------------------
// This serves as an example of a mesh format that meets the
// criteria of a generic mesh that can be used with any of
// the templated tools.
// ------------------------------------------------------------

class ImportedUgrid
{
	public:
		// Any Generic mesh must implement the following 
		// functions (no inheritance required)
		void getNode(int nodeId,double node[3]) const;
		int numberOfNodes() const;
		int numberOfCells() const;
		int numberOfNodesInCell(int id) const;
		int numberOfFacesInCell(int id) const;
		int numberOfNodesInCellFace(int cellId,int faceId) const;

        std::vector<int> getNodesInCellFace(int cellId, int faceId) const;

        std::vector<int> getNodesInCell(int cellId) const;
		void getNodesInCell(int cellId,int *cell) const;

		void setNode(int nodeId,double node[3]);

        // Any Generic Surface mesh must implement the following
		int numberOfBoundaryFaces() const;
        int numberOfNodesInBoundaryFace(int boundaryFaceId) const;

        std::vector<int> getNodesInBoundaryFace(int boundaryFaceId) const;
        void getNodesInBoundaryFace(int boundaryFaceId,int *face) const;
	
		// Everything below is specific to ImportedUgrid and
		// has nothing to do with any other generic mesh
		// (i.e., this stuff doesn't have to be in your mesh)	
        
        int getBoundaryFaceTag(int boundaryFaceId) const;
		int getBoundaryCondition(int BoundaryFaceId) const;

		ImportedUgrid(vector<double> nodes,vector<int> triangles,
				vector<int> quads,vector<int> tets,vector<int> pyramids,
				vector<int> prisms,vector<int> hexs,
                vector<int> triangleTags, vector<int> quadTags,
				vector<int> triangleBoundaryConditions,
				vector<int> quadBoundaryConditions);
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
    private:
		int nnodes;
		int ncells;
		int nfaces;
};


#endif
