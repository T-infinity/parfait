#ifndef MESH_MOVER_H
#define MESH_MOVER_H
#include "motion_matrix.h"

// Requirements:  Mesh must implement the following
//
// mesh.numberOfNodes();
// mesh.getNode(int,double*);
// mesh.setNode(int,double*);
//
// Then the mesh can be moved by passing the mesh
// and a MotionMatrix to the move() function.
// (The motion matrix can be used to create
// arbitrary combinations of translations and
// rotations.)
//

namespace MeshMover
{
	template<typename Mesh>
	void move(Mesh &mesh,const MotionMatrix &motion)
	{
		int nnodes = mesh.numberOfNodes();
		for(int i=0;i<nnodes;i++)
		{
			double p[3];
			mesh.getNode(i,p);
			motion.movePoint(p);
			mesh.setNode(i,p);
		}
	}
}
#endif
