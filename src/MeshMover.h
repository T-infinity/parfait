#pragma once
#include <MotionMatrix.h>
// Requirements:  Mesh must implement the following
//
// meshBuilder.numberOfNodes();
// meshBuilder.getPoint(int,double*);
// meshBuilder.setNode(int,double*);
//
// Then the meshBuilder can be moved by passing the meshBuilder
// and a MotionMatrix to the move() function.
// (The motion matrix can be used to create
// arbitrary combinations of translations and
// rotations.)
//
namespace Parfait {
    namespace MeshMover {
        template<typename Mesh>
        void move(Mesh &mesh, const MotionMatrix &motion) {
            int nnodes = mesh.numberOfNodes();
            for (int i = 0; i < nnodes; i++) {
                double p[3];
                mesh.getNode(i, p);
                motion.movePoint(p);
                mesh.setNode(i, p);
            }
        }
    }
}