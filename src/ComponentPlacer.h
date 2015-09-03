#ifndef COMPONENT_PLACER_H
#define COMPONENT_PLACER_H
#include "ConfigurationReader.h"
#include<vector>
#include <RangeLoop.h>

namespace Parfait{

template<typename Mesh>
void placeComponents(Mesh &mesh,ConfigurationReader &config)
{
	std::vector<Parfait::MotionMatrix> gridMotions;
	for(int i:range(config.numberOfGrids()))
		gridMotions.push_back(config.getMotionMatrix(i));
	for(int i:range(mesh.numberOfNodes()))
	{
		double p[3];
		mesh.getNode(i,p);
		gridMotions[mesh.getAssociatedComponentId(i)].movePoint(p);
		mesh.setNode(i,p);
	}
}

}

#endif
