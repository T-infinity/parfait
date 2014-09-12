#ifndef COMPONENT_PLACER_H
#define COMPONENT_PLACER_H
#include "motion_matrix.h"
#include "configuration_reader.h"
#include "range_loop.h"
#include<vector>

template<typename Mesh>
void placeComponents(Mesh &mesh,ConfigurationReader &config)
{
	std::vector<MotionMatrix> gridMotions;
	for(int i:range(config.numberOfGrids()))
		gridMotions.push_back(config.getMotionMatrix(i));
	for(int i:range(mesh.numberOfNodes()))
	{
		double p[3];
		mesh.getNode(i,p);
		gridMotions[mesh.getImesh(i)].movePoint(p);
		mesh.setNode(i,p);
	}
}


#endif
