#include "imported_ugrid.h"
#include <stdio.h>
#include <stdexcept>
#include <string>

using std::vector;
ImportedUgrid::ImportedUgrid(vector<double> nodes_in,vector<int> triangles_in,
		vector<int> quads_in,vector<int> tets_in,vector<int> pyramids_in,
		vector<int> prisms_in,vector<int> hexs_in,
        vector<int> triangleTags_in, vector<int> quadTags_in,
		vector<int> triangleBoundaryConditions_in,
		vector<int> quadBoundaryConditions_in)
	: nodes(nodes_in),
	triangles(triangles_in),
	quads(quads_in),
	tets(tets_in),
	pyramids(pyramids_in),
	prisms(prisms_in),
	hexs(hexs_in),
    triangleTags(triangleTags_in),
    quadTags(quadTags_in),
	triangleBoundaryConditions(triangleBoundaryConditions_in),
	quadBoundaryConditions(quadBoundaryConditions_in)
{
	nnodes = (int)nodes.size()/3;
	cellMap.push_back((int)tets.size()/4);
	cellMap.push_back(cellMap.back() + (int)pyramids.size()/5);
	cellMap.push_back(cellMap.back() + (int)prisms.size()/6);
	cellMap.push_back(cellMap.back() + (int)hexs.size()/8);
	faceMap.push_back((int)triangles.size()/3);
	faceMap.push_back(faceMap.back() + (int)quads.size()/4);
	ncells = cellMap.back();
	nfaces = faceMap.back();
}

void ImportedUgrid::getNode(int nodeId,double node[3]) const
{
	node[0] = nodes[3*nodeId+0];
	node[1] = nodes[3*nodeId+1];
	node[2] = nodes[3*nodeId+2];
}

void ImportedUgrid::setNode(int nodeId,double node[3])
{
	nodes[3*nodeId+0] = node[0];
	nodes[3*nodeId+1] = node[1];
	nodes[3*nodeId+2] = node[2];
}

int ImportedUgrid::numberOfNodes() const
{
	return nnodes;
}

int ImportedUgrid::numberOfCells() const
{
	return ncells;
}

int ImportedUgrid::numberOfBoundaryFaces() const
{
	return nfaces;
}

int ImportedUgrid::numberOfFacesInCell(int id) const
{
	if(id < cellMap[0])
		return 4;
	if(id < cellMap[2])
		return 5;	
	if(id < cellMap[3])
		return 6;
	assert(false);
}

int ImportedUgrid::numberOfNodesInCell(int id) const
{
	if(id < cellMap[0])
		return 4;
	if(id < cellMap[1])
		return 5;	
	if(id < cellMap[2])
		return 6;
	if(id < cellMap[3])
		return 8;
	assert(false);
}

int ImportedUgrid::numberOfNodesInBoundaryFace(int id) const
{
    if(id < faceMap[0])
        return 3;
    if(id < faceMap[1])
        return 4;
    assert(false);
}

int ImportedUgrid::numberOfNodesInCellFace(int cellId,int faceId) const
{
	if(cellId < cellMap[0])
		return 3;
	if(cellId < cellMap[1])
	{
		if(faceId == 0)
			return 4;
		else 
			return 3;
	}
	if(cellId < cellMap[2])
	{
		if(faceId < 3)
			return 4;
		else
			return 3;
	}
	if(cellId < cellMap[3])
		return 4;
	assert(false);
}

std::vector<int> ImportedUgrid::getNodesInBoundaryFace(int faceId) const
{

    if(faceId < faceMap[0])
    {
        std::vector<int> nodes(3);
        nodes[0] = triangles[3*faceId+0];
        nodes[1] = triangles[3*faceId+1];
        nodes[2] = triangles[3*faceId+2];
        return nodes;
    }
    if(faceId < faceMap[1])
    {
        faceId -= triangles.size() / 3;
        std::vector<int> nodes(4);
        nodes[0]  = quads[4*faceId+0];
        nodes[1]  = quads[4*faceId+1];
        nodes[2]  = quads[4*faceId+2];
        nodes[3]  = quads[4*faceId+3];
        return nodes;
    }
    assert(faceId < faceMap[1]);

    return std::vector<int>();
}

void ImportedUgrid::getNodesInBoundaryFace(int faceId,int *face) const
{
    if(faceId < faceMap[0])
    {
        std::vector<int> nodes(3);
        face[0] = triangles[3*faceId+0];
        face[1] = triangles[3*faceId+1];
        face[2] = triangles[3*faceId+2];
    }
	else if(faceId < faceMap[1])
    {
        faceId -= triangles.size() / 3;
        std::vector<int> nodes(4);
        face[0]  = quads[4*faceId+0];
        face[1]  = quads[4*faceId+1];
        face[2]  = quads[4*faceId+2];
        face[3]  = quads[4*faceId+3];
    }
    assert(faceId < faceMap[1]);
}


std::vector<int> ImportedUgrid::getNodesInCellFace(int cellId,int faceId) const
{
	assert(cellId >= 0);
    const int *cell = NULL;
    // tet
	if(cellId < cellMap[0])
	{
        // tet
        cell = &tets[4*cellId];
		if(faceId == 0)
		{
            std::vector<int> face(3);
			face[0] = cell[0];
			face[1] = cell[2];
			face[2] = cell[1];
            return face;
		}
		else if(faceId == 1)
		{
            std::vector<int> face(3);
			face[0] = cell[0];
			face[1] = cell[1];
			face[2] = cell[3];
            return face;
		}
		else if(faceId == 2)
		{
            std::vector<int> face(3);
			face[0] = cell[1];
			face[1] = cell[2];
			face[2] = cell[3];
            return face;
		}
		else if(faceId == 3)
		{
            std::vector<int> face(3);
			face[0] = cell[2];
			face[1] = cell[0];
			face[2] = cell[3];
            return face;
		}
	}
    // pyramid
	else if(cellId < cellMap[1])
	{
        cell = &pyramids[5*(cellId-cellMap[0])];
		if(faceId == 0)
		{
            std::vector<int> face(4);
			face[0] = cell[1];
			face[1] = cell[4];
			face[2] = cell[3];
			face[3] = cell[0];
            return face;
		}
		else if(faceId == 1)
		{
            std::vector<int> face(3);
			face[0] = cell[3];
			face[1] = cell[2];
			face[2] = cell[0];
            return face;
		}
		else if(faceId == 2)
		{
            std::vector<int> face(3);
			face[0] = cell[4];
			face[1] = cell[2];
			face[2] = cell[3];
            return face;
		}
		else if(faceId == 3)
		{
            std::vector<int> face(3);
			face[0] = cell[1];
			face[1] = cell[2];
			face[2] = cell[4];
            return face;
		}
		else if(faceId == 4)
		{
            std::vector<int> face(3);
			face[0] = cell[0];
			face[1] = cell[2];
			face[2] = cell[1];
            return face;
		}
	}
    // prism
	else if(cellId < cellMap[2])
	{
        cell = &prisms[6*(cellId - cellMap[1])];
		if(faceId == 0)
		{
            std::vector<int> face(4);
			face[0] = cell[0];
			face[1] = cell[1];
			face[2] = cell[4];
			face[3] = cell[3];
            return face;
		}
		else if(faceId == 1)
		{
            std::vector<int> face(4);
			face[0] = cell[1];
			face[1] = cell[2];
			face[2] = cell[5];
			face[3] = cell[4];
            return face;
		}
		else if(faceId == 2)
		{
            std::vector<int> face(4);
			face[0] = cell[2];
			face[1] = cell[0];
			face[2] = cell[3];
			face[3] = cell[5];
            return face;
		}
		else if(faceId == 3)
		{
            std::vector<int> face(3);
			face[0] = cell[0];
			face[1] = cell[2];
			face[2] = cell[1];
            return face;
		}
		else if(faceId == 4)
		{
            std::vector<int> face(3);
			face[0] = cell[3];
			face[1] = cell[4];
			face[2] = cell[5];
            return face;
		}
	}
    // hex
	else if(cellId < cellMap[3])
	{
        cell = &hexs[8*(cellId - cellMap[2])];
		if(faceId == 0)
		{
            std::vector<int> face(4);
			face[0] = cell[0];
			face[1] = cell[3];
			face[2] = cell[2];
			face[3] = cell[1];
            return face;
		}
		else if(faceId == 1)
		{
            std::vector<int> face(4);
			face[0] = cell[0];
			face[1] = cell[1];
			face[2] = cell[5];
			face[3] = cell[4];
            return face;
		}
		else if(faceId == 2)
		{
            std::vector<int> face(4);
			face[0] = cell[1];
			face[1] = cell[2];
			face[2] = cell[6];
			face[3] = cell[5];
            return face;
		}
		else if(faceId == 3)
		{
            std::vector<int> face(4);
			face[0] = cell[2];
			face[1] = cell[3];
			face[2] = cell[7];
			face[3] = cell[6];
            return face;
		}
		else if(faceId == 4)
		{
            std::vector<int> face(4);
			face[0] = cell[3];
			face[1] = cell[0];
			face[2] = cell[4];
			face[3] = cell[7];
            return face;
		}
		else if(faceId == 5)
		{
            std::vector<int> face(4);
			face[0] = cell[4];
			face[1] = cell[5];
			face[2] = cell[6];
			face[3] = cell[7];
            return face;
		}
	}
	assert(cellId < cellMap[3]);
    throw std::domain_error("ERROR: Cell "+std::to_string(cellId)+" is not in the cellMap.");
}

std::vector<int> ImportedUgrid::getNodesInCell(int cellId) const
{
	int nvertices = numberOfNodesInCell(cellId);
	const int *cell_list=NULL;
    int id=-1;
	int ntet=(int)tets.size()/4;
	int npyr=(int)pyramids.size()/5;
	int nprism=(int)prisms.size()/6;
	switch(nvertices)
	{
		case 4:
			cell_list = &tets[0];
			id = cellId;
			break;
		case 5:
			cell_list = &pyramids[0];
			id = cellId - ntet;
			break;
		case 6:
			cell_list = &prisms[0];
			id = cellId - ntet - npyr;
			break;
		case 8:
			cell_list = &hexs[0];
			id = cellId - ntet - npyr - nprism;
			break;
	}
    std::vector<int> cell(nvertices);
	for(int i=0;i<nvertices;i++)
		cell[i] = cell_list[nvertices*id+i];
    return cell;
}

std::vector<int> ImportedUgrid::getVtkOrderedNodesInCell(int cellId) const{
    return getNodesInCell(cellId); // update correct ordering later...
}

void ImportedUgrid::getNodesInCell(int cellId,int *cell) const
{
	int nvertices = numberOfNodesInCell(cellId);
	const int *cell_list=NULL;
    int id=-1;
	int ntet=(int)tets.size()/4;
	int npyr=(int)pyramids.size()/5;
	int nprism=(int)prisms.size()/6;
	switch(nvertices)
	{
		case 4:
			cell_list = &tets[0];
			id = cellId;
			break;
		case 5:
			cell_list = &pyramids[0];
			id = cellId - ntet;
			break;
		case 6:
			cell_list = &prisms[0];
			id = cellId - ntet - npyr;
			break;
		case 8:
			cell_list = &hexs[0];
			id = cellId - ntet - npyr - nprism;
			break;
	}
	for(int i=0;i<nvertices;i++)
		cell[i] = cell_list[nvertices*id+i];
}

int ImportedUgrid::getBoundaryTag(int id) const 
{
    if(id < faceMap[0])
    {
        id -= 0;
        return triangleTags[id];
    }
    if(id < faceMap[1])
    {
        id -= triangles.size()/3;
        return quadTags[id];
    }
    assert(false);
}

int ImportedUgrid::getBoundaryCondition(int faceId) const 
{
    if(faceId < faceMap[0])
    {
        faceId -= 0;
        return triangleBoundaryConditions[faceId];
    }
    if(faceId < faceMap[1])
    {
        faceId -= triangles.size()/3;
        return quadBoundaryConditions[faceId];
    }
    assert(false);
}
