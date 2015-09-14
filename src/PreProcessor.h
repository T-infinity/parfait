#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <string>
#include "ConfigurationReader.h"
#include <MessagePasser.h>
#include <memory>
#include <MeshBasicParallel.h>

namespace Parfait {
	class PreProcessor {
	public:
		PreProcessor(std::string xml_input_filename);
		std::shared_ptr<MeshBasicParallel> createMesh();

		int getNumberOfGrids() { return gridNames.size(); }

		std::string getGridName(int i) { return gridNames[i]; }

		bool getIsBigEndian(int i) { return isBigEndian[i]; }

	private:
		ConfigurationReader config;
		std::vector<std::string> gridNames;
		std::vector<bool> isBigEndian;
		std::vector<Parfait::MapbcReader> mapbcVector;
		std::vector<Parfait::MotionMatrix> grid_initial_positions;

		void setUpGridInfo(std::string xml_input_filename);
		std::vector<int> calculateNewPartitioning(std::shared_ptr<MeshBasicParallel> mesh);
	};
}
#include "PreProcessor.hpp"

#endif
