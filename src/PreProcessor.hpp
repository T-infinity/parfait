#include <Partitioner.h>
#include <ParallelMeshReader.h>
#include <ParallelMeshRedistributor.h>
#include "ComponentPlacer.h"

namespace Parfait{
	inline PreProcessor::PreProcessor(std::string xml_input_filename)
			:config(xml_input_filename)
	{
		setUpGridInfo(xml_input_filename);
	}

	inline std::vector<int> PreProcessor::calculateNewPartitioning(Parfait::ImportedUgrid &ugrid){
		Partitioner <Parfait::ImportedUgrid> partitioner(ugrid);
		return partitioner.generatePartVector();
	}

	inline void PreProcessor::setUpGridInfo(std::string xml_input_filename){
		for(int i:range(config.numberOfGrids())){
			gridNames.push_back(config.getFilename(i));
			isBigEndian.push_back(config.isBigEndian(i));
			mapbcVector.push_back(config.getMapbcObject(i));
			grid_initial_positions.push_back(config.getMotionMatrix(i));
		}
	}

	inline int PreProcessor::createFun3DMesh(){
		Parfait::ParallelMeshReader naiveReader(gridNames,isBigEndian);

        #if 0
		auto ugrid = naiveReader.distributeGridsEvenly();
		auto part = calculateNewPartitioning(ugrid);
		auto gridNodeMap = naiveReader.getGridNodeMap();
		if(MessagePasser::Rank() == 0)
			printf("Redistribute according to new partitioning:\n");
		ParallelMeshReDistributor distributor(ugrid,part);
		if(MessagePasser::Rank() == 0)
			printf("Create Fun3DMesh\n");
		auto fun_mesh = distributor.createFun3DMesh(mapbcVector);
		if(MessagePasser::Rank() == 0)
			printf("Translate and rotate components appropriately:\n");
		//TODO: actually return a valid mesh, and place the components
		//placeComponents(fun_mesh,config);
		return fun_mesh;
		#endif
        return 0;
	}
}
