#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <string>
#include "ConfigurationReader.h"
#include "ParallelImportedUgrid.h"
#include <Fun3dMesh.h>
#include <ImportedUgrid.h>
#include <MessagePasser.h>

namespace Parfait {
	class PreProcessor {
	public:
		PreProcessor(std::string xml_input_filename);
		ParallelImportedUgrid createMesh();

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
		std::vector<int> calculateNewPartitioning(Parfait::ImportedUgrid &ugrid);
	};
}
#include "PreProcessor.hpp"

#endif
