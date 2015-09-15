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
		PreProcessor(Configuration& configuration);
		std::shared_ptr<MeshBasicParallel> createMesh();

		int getNumberOfGrids() { return config.numberOfGrids(); }

		std::string getGridName(int i) { return config.getFilename(i); }

		bool getIsBigEndian(int i) { return config.isBigEndian(i); }

	private:
		Configuration& config;
		std::vector<int> calculateNewPartitioning(std::shared_ptr<MeshBasicParallel> mesh);
	};
}
#include "PreProcessor.hpp"

#endif
