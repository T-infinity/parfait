#include <ParmetisPrepper.h>
#include <ParallelMeshReaderNaive.h>
#include <ParallelMeshRedistributor.h>
#include "ComponentPlacer.h"

namespace Parfait{
	inline PreProcessor::PreProcessor(std::string xml_input_filename)
			:config(xml_input_filename)
	{
		setUpGridInfo(xml_input_filename);
	}

	inline std::vector<int> PreProcessor::calculateNewPartitioning(Parfait::ImportedUgrid &ugrid){
		if(MessagePasser::Rank() == 0)
			printf("Create ParmetisPrepper:\n");
		ParMetisPrepper<Parfait::ImportedUgrid> prepper(ugrid);

		if(MessagePasser::Rank() == 0)
			printf("Build node to node connectivity:\n");
		prepper.buildNodeToNodeConnectivity();
		return prepper.getPartVector();
	}

	inline void PreProcessor::setUpGridInfo(std::string xml_input_filename){
		for(int i:range(config.numberOfGrids())){
			gridNames.push_back(config.getFilename(i));
			isBigEndian.push_back(config.isBigEndian(i));
			mapbcVector.push_back(config.getMapbcObject(i));
			grid_initial_positions.push_back(config.getMotionMatrix(i));
		}
	}

	inline Parfait::Fun3DMesh PreProcessor::createFun3DMesh(){
		Parfait::ParallelMeshReaderNaive naiveReader(gridNames,isBigEndian);

		auto ugrid = naiveReader.distributeGridsEvenly();
		auto part = calculateNewPartitioning(ugrid);
		auto gridNodeMap = naiveReader.getGridNodeMap();
		if(MessagePasser::Rank() == 0)
			printf("Redistribute according to new partitioning:\n");
		ParallelMeshReDistributor distributor(ugrid,gridNodeMap,part);
		if(MessagePasser::Rank() == 0)
			printf("Create Fun3DMesh\n");
		auto fun_mesh = distributor.createFun3DMesh(mapbcVector);
		if(MessagePasser::Rank() == 0)
			printf("Translate and rotate components appropriately:\n");
		placeComponents(fun_mesh,config);
		return fun_mesh;
	}
}
