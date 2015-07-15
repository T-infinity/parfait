#include "fun3d_mesh.h"

inline Parfait::Fun3DMesh::Fun3DMesh(int nnodes0_in,int nnodes01_in,double *x_in,double *y_in,double *z_in,
		int *globalNodeIds_in,
		int *imesh_in,
		int ntriangles_in,int *triangles_in,int *triangleTags_in,int *triangleBcs_in,
		int nquads_in,int *quads_in,int *quadTags_in,int *quadBcs_in,
		int ntets_in,int *tets_in,
		int npyramids_in,int *pyramids_in,
		int nprisms_in,int *prisms_in,
		int nhexs_in,int *hexs_in)
	:
	imesh(imesh_in),
	nnodes0(nnodes0_in),
	nnodes01(nnodes01_in),
	ntriangles(ntriangles_in),
	nquads(nquads_in),
	ntets(ntets_in),
	npyramids(npyramids_in),
	nprisms(nprisms_in),
	nhexs(nhexs_in),
	ncells(ntets+npyramids+nprisms+nhexs),
	nfaces(ntriangles+nquads),
	x(x_in),
	y(y_in),
	z(z_in),
	globalNodeIds(globalNodeIds_in),
	triangles(triangles_in),
	quads(quads_in),
	triangleTags(triangleTags_in),
	quadTags(quadTags_in),
	triangleBcs(triangleBcs_in),
	quadBcs(quadBcs_in),
	tets(tets_in),
	pyramids(pyramids_in),
	prisms(prisms_in),
	hexs(hexs_in)
{
	cellMap[0] = ntets;
	cellMap[1] = cellMap[0] + npyramids;
	cellMap[2] = cellMap[1] + nprisms;
	cellMap[3] = cellMap[2] + nhexs;
	faceMap[0] = ntriangles;
	faceMap[1] = faceMap[0] + nquads;
}

inline void Parfait::Fun3DMesh::freePointers()
{
	free(x);
	free(y);
	free(z);
	free(imesh);
	free(globalNodeIds);
	free(triangles);
	free(triangleTags);
	free(triangleBcs);
	free(quads);
	free(quadTags);
	free(quadBcs);
	free(tets);
	free(pyramids);
	free(prisms);
	free(hexs);
}

inline void Parfait::Fun3DMesh::getNode(int nodeId,double node[3]) const
{
	node[0] = x[nodeId];
	node[1] = y[nodeId];
	node[2] = z[nodeId];
}

inline void Parfait::Fun3DMesh::setNode(int nodeId,double node[3])
{
	x[nodeId] = node[0];
	y[nodeId] = node[1];
	z[nodeId] = node[2];
}

inline int Parfait::Fun3DMesh::getGlobalNodeId(int localNodeId)
{
	return globalNodeIds[localNodeId];
}

inline int Parfait::Fun3DMesh::getImesh(int localNodeId)
{
	return imesh[localNodeId];
}

inline int Parfait::Fun3DMesh::getBoundaryTag(int boundaryFaceId)
{
	if(boundaryFaceId < ntriangles)
		return triangleTags[boundaryFaceId];
	return quadTags[boundaryFaceId-ntriangles];
}

inline int Parfait::Fun3DMesh::getBoundaryCondition(int boundaryFaceId)
{
	if(boundaryFaceId < ntriangles)
		return triangleBcs[boundaryFaceId];
	return quadBcs[boundaryFaceId-ntriangles];
}

inline int Parfait::Fun3DMesh::numberOfNodes() const
{
	return nnodes01;
}

inline int Parfait::Fun3DMesh::numberOfCells() const
{
	return ncells;
}

inline int Parfait::Fun3DMesh::numberOfBoundaryFaces() const
{
	return nfaces;
}

inline int Parfait::Fun3DMesh::numberOfNodesInBoundaryFace(int id) const
{
    if(id < faceMap[0])
        return 3;
    if(id < faceMap[1])
        return 4;
	assert(false);
}

inline std::vector<int> Parfait::Fun3DMesh::getNodesInBoundaryFace(int faceId) const
{
	std::vector<int> nodes(3,0);
    if(faceId < faceMap[0])
    {
        nodes[0] = triangles[3*faceId+0];
        nodes[1] = triangles[3*faceId+1];
        nodes[2] = triangles[3*faceId+2];
    }
	else if(faceId < faceMap[1])
    {
		nodes.push_back(0);
        faceId -= ntriangles;
        nodes[0]  = quads[4*faceId+0];
        nodes[1]  = quads[4*faceId+1];
        nodes[2]  = quads[4*faceId+2];
        nodes[3]  = quads[4*faceId+3];
    }
	else
	{
    	assert(faceId < faceMap[1]);
	}
	return nodes;
}

inline void Parfait::Fun3DMesh::getNodesInBoundaryFace(int faceId,int *face) const
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
        faceId -= ntriangles;
        std::vector<int> nodes(4);
        face[0]  = quads[4*faceId+0];
        face[1]  = quads[4*faceId+1];
        face[2]  = quads[4*faceId+2];
        face[3]  = quads[4*faceId+3];
    }
    assert(faceId < faceMap[1]);
}

inline int Parfait::Fun3DMesh::numberOfFacesInCell(int id) const
{
	if(id < cellMap[0])
		return 4;
	if(id < cellMap[2])
		return 5;	
	if(id < cellMap[3])
		return 6;
	assert(false);
}

inline int Parfait::Fun3DMesh::numberOfNodesInCell(int id) const
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

inline int Parfait::Fun3DMesh::numberOfNodesInCellFace(int cellId,int faceId) const
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

inline std::vector<int> Parfait::Fun3DMesh::getNodesInCellFace(int cellId,int faceId) const
{
	assert(cellId >= 0);
	assert(cellId < cellMap[3]);
	std::vector<int> face(3,0);
	if(cellId < cellMap[0])
	{
		int cell[4];
		getNodesInCell(cellId,cell);
		if(faceId == 0)
		{
			face[0] = cell[0];
			face[1] = cell[2];
			face[2] = cell[1];
		}
		else if(faceId == 1)
		{
			face[0] = cell[0];
			face[1] = cell[1];
			face[2] = cell[3];
		}
		else if(faceId == 2)
		{
			face[0] = cell[1];
			face[1] = cell[2];
			face[2] = cell[3];
		}
		else if(faceId == 3)
		{
			face[0] = cell[2];
			face[1] = cell[0];
			face[2] = cell[3];
		}
	}
	else if(cellId < cellMap[1])
	{
		int cell[5];
    getNodesInCell(cellId,cell);
		if(faceId == 0)
		{
			face.push_back(0);
			face[0] = cell[1];
			face[1] = cell[4];
			face[2] = cell[3];
			face[3] = cell[0];
		}
		else if(faceId == 1)
		{
			face[0] = cell[3];
			face[1] = cell[2];
			face[2] = cell[0];
		}
		else if(faceId == 2)
		{
			face[0] = cell[4];
			face[1] = cell[2];
			face[2] = cell[3];
		}
		else if(faceId == 3)
		{
			face[0] = cell[1];
			face[1] = cell[2];
			face[2] = cell[4];
		}
		else if(faceId == 4)
		{
			face[0] = cell[0];
			face[1] = cell[2];
			face[2] = cell[1];
		}
		#if 0
		getNodesInCell(cellId,cell);
		if(faceId == 0)
		{
			face.push_back(0);
			face[0] = cell[0];
			face[1] = cell[3];
			face[2] = cell[4];
			face[3] = cell[1];
		}
		else if(faceId == 1)
		{
			face[0] = cell[0];
			face[1] = cell[2];
			face[2] = cell[3];
		}
		else if(faceId == 2)
		{
			face[0] = cell[3];
			face[1] = cell[2];
			face[2] = cell[4];
		}
		else if(faceId == 3)
		{
			face[0] = cell[4];
			face[1] = cell[2];
			face[2] = cell[1];
		}
		else if(faceId == 4)
		{
			face[0] = cell[1];
			face[1] = cell[2];
			face[2] = cell[0];
		}
		#endif
	}
	else if(cellId < cellMap[2])
	{
		int cell[6];
		getNodesInCell(cellId,cell);
		if(faceId == 0)
		{
			face.push_back(0);
			face[0] = cell[0];
			face[1] = cell[1];
			face[2] = cell[4];
			face[3] = cell[3];
		}
		else if(faceId == 1)
		{
			face.push_back(0);
			face[0] = cell[1];
			face[1] = cell[2];
			face[2] = cell[5];
			face[3] = cell[4];
		}
		else if(faceId == 2)
		{
			face.push_back(0);
			face[0] = cell[2];
			face[1] = cell[0];
			face[2] = cell[3];
			face[3] = cell[5];
		}
		else if(faceId == 3)
		{
			face[0] = cell[0];
			face[1] = cell[2];
			face[2] = cell[1];
		}
		else if(faceId == 4)
		{
			face[0] = cell[3];
			face[1] = cell[4];
			face[2] = cell[5];
		}
	}
	else if(cellId < cellMap[3])
	{
		int cell[8];
		getNodesInCell(cellId,cell);
		face.push_back(0);
		if(faceId == 0)
		{
			face[0] = cell[0];
			face[1] = cell[3];
			face[2] = cell[2];
			face[3] = cell[1];
		}
		else if(faceId == 1)
		{
			face[0] = cell[0];
			face[1] = cell[1];
			face[2] = cell[5];
			face[3] = cell[4];
		}
		else if(faceId == 2)
		{
			face[0] = cell[1];
			face[1] = cell[2];
			face[2] = cell[6];
			face[3] = cell[5];
		}
		else if(faceId == 3)
		{
			face[0] = cell[2];
			face[1] = cell[3];
			face[2] = cell[7];
			face[3] = cell[6];
		}
		else if(faceId == 4)
		{
			face[0] = cell[3];
			face[1] = cell[0];
			face[2] = cell[4];
			face[3] = cell[7];
		}
		else if(faceId == 5)
		{
			face[0] = cell[4];
			face[1] = cell[5];
			face[2] = cell[6];
			face[3] = cell[7];
		}
	}
	return face;
}

inline void Parfait::Fun3DMesh::getNodesInCell(int cellId,int cell[]) const
{
	int nvertices = numberOfNodesInCell(cellId);
	const int *cell_list=NULL;
    int id=-1;
	switch(nvertices)
	{
		case 4:
			cell_list = tets;
			id = cellId;
			break;
		case 5:
			cell_list = pyramids;
			id = cellId - ntets;
			break;
		case 6:
			cell_list = prisms;
			id = cellId - ntets - npyramids;
			break;
		case 8:
			cell_list = hexs;
			id = cellId - ntets - npyramids - nprisms;
			break;
	}
	for(int i=0;i<nvertices;i++)
		cell[i] = cell_list[nvertices*id+i];
}

inline std::vector<int> Parfait::Fun3DMesh::getNodesInCell(int cellId) const
{
	int nvertices = numberOfNodesInCell(cellId);
	std::vector<int> cell(nvertices,0);
	getNodesInCell(cellId,cell.data());
	return cell;
}

inline std::vector<int> Parfait::Fun3DMesh::getVtkOrderedNodesInCell(int cellId) const
{
  auto cell = getNodesInCell(cellId);
  auto fixed = cell;
  if(5 == cell.size()){
    fixed[0] = cell[0];
    fixed[1] = cell[3];
    fixed[2] = cell[4];
    fixed[3] = cell[1];
    fixed[4] = cell[2];
  }
  return fixed; // actually fix ordering later
}

