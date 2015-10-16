#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <string>
#include <vector>
#include <ConfigurationReader.h>
#include <MotionMatrix.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <MapbcReader.h>
#include <MessagePasser.h>
#include "Point.h"
#include "tinyxml.h"
#include <memory>

namespace Parfait {
	class Configuration {
	public:
        Configuration(int number_of_grids,
                      std::shared_ptr<std::vector<std::string>> grid_filenames,
                      std::shared_ptr<std::vector<bool>> is_big_endian,
                      std::shared_ptr<std::vector<Parfait::MapbcReader>> map_bcs,
                      std::shared_ptr<std::vector<Parfait::MotionMatrix>> motion_matrices);

        int numberOfGrids();
		std::string getFilename(int gridId);
		bool isBigEndian(int gridId);
		Parfait::MotionMatrix getMotionMatrix(int gridId);
		int getBoundaryCondition(int gridId, int tag);
		Parfait::MapbcReader getMapbcObject(int gridId);
		void print();
	private:
		int ngrids;
		std::shared_ptr<std::vector<std::string>> gridFilenames;
		std::shared_ptr<std::vector<bool>> bigEndian;
		std::shared_ptr<std::vector<Parfait::MapbcReader>> mapbcVector;
		std::shared_ptr<std::vector<Parfait::MotionMatrix>> motionMatrices;
		void load();
		void readMapbcFiles();
		void loadComponents(TiXmlHandle globalHandle);
		bool figureOutEndianness(std::string s);

		const Parfait::Point<double> getXYZ(TiXmlElement *element) const;
		void throwIfBadElement(TiXmlElement* element);
		void throwIfConfigurationIsInsane();
		bool getEndiannessForComponent(const TiXmlElement *ComponentElement);

		void getRotationForComponent(const TiXmlHandle &ComponentHandle);
	};
}
#include "Configuration.hpp"

#endif
