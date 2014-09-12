#ifndef GENERIC_MESH_TOOLS_H
#define GENERIC_MESH_TOOLS_H

#ifndef RANGE_LOOP_H
#include "range_loop.h"
#endif

#ifndef POINT_H
#include "point.h"
#endif

#ifndef GENERIC_MESH_H
#include "generic_mesh.h"
#endif

#ifndef VECTOR_TOOLS_H
#include "vector_tools.h"
#endif

#include <cmath>
#include <stdexcept>

namespace GenericMeshTools {

/// Computes the volume of a tet given the xyz coords of its
//  vertices.  It returns the signed volume, so in order for it
//  to be positive, the nodes have to be in the expected order
//  (matching ugrid format).
// 
//          D 
//         /|\
//        / | \
//       /  |  \  
//      /   |   \
//     /    |    \
//  A /     |     \ C 
//    \` ` `|` ` `/ 
//     \    |    /
//      \   |   /
//       \  |  /
//        \ | /
//         \|/
//          B                  
//           
//
inline double CompTetVolume(Point &a, Point &b, Point &c, Point &d) {

    Point v1 = a - d;
    Point v2 = b - d;
    Point v3 = c - d;
    Point v = Point::cross(v2, v3);
    return -Point::dot(v1, v) / 6.0;
}
template <class MeshType>
Point computeCenter(MeshType &mesh, const std::vector<int> &nodes) {
    Point center(0, 0, 0);
    for (auto node : nodes) {
        Point a;
        mesh.getNode(node, &a[0]);
        center += a;
    }

    center /= (double)nodes.size();
    return center;
}

template <class MeshType, class CellType>
Point cellCenter(MeshType &mesh, CellType &cell) {
    auto nodes = cell.getNodes();
    return GenericMeshTools::computeCenter(mesh, nodes);
}

template <class MeshType, class CellType>
inline double computeCellVolume(MeshType &mesh_in, CellType &&cell) {
    Mesh<MeshType> mesh(mesh_in);
    double volume_ = 0;
    Point centerOfCell = cellCenter(mesh, cell);

    for (int faceId : range(cell.numberOfFaces())) {
        auto face = cell.faces(faceId);
        auto faceNodes = face.getNodes();
        Point faceCenter = computeCenter(mesh, faceNodes);

        for (int index1 : range(face.numberOfNodes())) {
            int index2 = (index1 + 1) % face.numberOfNodes();
            Point node1, node2;
            mesh.getNode(faceNodes[index1], &node1[0]);
            mesh.getNode(faceNodes[index2], &node2[0]);

            volume_ += CompTetVolume(node2, node1, faceCenter, centerOfCell);
        }
    }
    return volume_;
}

template <class MeshType, class FaceType>
Point faceCenter(MeshType &mesh, FaceType &face) {
    auto faceNodes = face.getNodes();
    return GenericMeshTools::computeCenter(mesh,faceNodes);
}

template <class MeshType, class FaceType>
Point getFaceArea(MeshType &mesh, FaceType &face) {
    auto faceNodeIds = face.getNodes();
    if(3 == faceNodeIds.size()){
        Point a, b, c;
        mesh.getNode(faceNodeIds[0], &a[0]);
        mesh.getNode(faceNodeIds[1], &b[0]);
        mesh.getNode(faceNodeIds[2], &c[0]);

        Point v1 = b - a;
        Point v2 = c - a;

        Point area = 0.5*Point::cross(v1, v2);
        return area;

    } else {
        Point start = computeCenter(mesh, faceNodeIds);
        Point area(0, 0, 0);
        mesh.getNode(faceNodeIds[0], &start[0]);
        for (auto index : range((int)faceNodeIds.size() - 1)) {

            int node1 = faceNodeIds[index];
            int node2 = faceNodeIds[(index + 1) % faceNodeIds.size()];
            Point a, b;
            mesh.getNode(node1, &a[0]);
            mesh.getNode(node2, &b[0]);

            a -= start;
            b -= start;
            area += 0.5 * Point::cross(a, b);
        }
        return area;
    }
}

template <typename MeshType>
std::vector<std::vector<int>> buildNode2Cell(MeshType &mesh_in) {
    Mesh<MeshType> mesh(mesh_in);
    std::vector<std::vector<int>> node2Cell(mesh.numberOfNodes());

    for (auto cell : mesh.cells()) {
        for (auto face : cell) {
            for (auto node : face) {
                insertUnique(node2Cell[node], cell.Id());
            }
        }
    }
    return node2Cell;
}

template <typename MeshType, class CellType>
bool containsPoint(MeshType &mesh, CellType &&cell, double p[3]) {
	Point a,b,searchPoint(p);
	for(auto face:cell)
	{
		// Make tets with the search point and the 
		// triangles of each face tesselation and 
		// check if the volume is positive for all (p is inside cell)
		auto faceNodeIds = face.getNodes();
		Point faceCentroid = faceCenter(mesh,face);
		int n = faceNodeIds.size();
		for(int i=0;i<n;i++)
		{
			mesh.getNode(faceNodeIds[i],a.data());
			mesh.getNode(faceNodeIds[(i+1)%n],b.data());
			if(0 > CompTetVolume(b,a,faceCentroid,searchPoint))
				return false;	
		}
	}
    return true;
}

template <typename MeshType, class CellType>
double interpolate(MeshType &mesh, CellType &&cell,
                   std::vector<double> &nodeData, double p[3]) {
    double interpolatedValue = 0.0;
    // for now, just average the values at the cell vertices
    // later, this will be tri-linear interpolation, etc.
    for (int nodeId : cell.getNodes()) interpolatedValue += nodeData[nodeId];
    interpolatedValue /= (double)cell.numberOfNodes();
    return interpolatedValue;
}


template <typename MeshType, class CellType>
std::vector<double> calculateInterpolationWeights(MeshType &mesh, CellType &&cell,
                   double p[3]) {
	// for now, just return all weights equal
	double w = 1.0 / (double)cell.numberOfNodes();
	std::vector<double> weights(cell.numberOfNodes(),w);

	return weights;
}


};

#endif
