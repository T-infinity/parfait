#ifndef CONFIGURATION_READER_H
#define CONFIGURATION_READER_H
#include <string>
#include <vector>
#include <ConfigurationReader.h>
#include "Configuration.h"
#include <MotionMatrix.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <MapbcReader.h>
#include <MessagePasser.h>
#include "Point.h"
#include "tinyxml.h"

namespace Parfait {
	class ConfigurationReader {
	public:
		ConfigurationReader(std::string xmlFile);
		Configuration createConfiguration();
	private:
		int ngrids;
		std::string filename;
		std::shared_ptr<std::vector<std::string>> gridFilenames;
		std::shared_ptr<std::vector<bool>> bigEndian;
		std::shared_ptr<std::vector<Parfait::BoundaryConditionMap>> mapbcVector;
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
#include "ConfigurationReader.hpp"

#endif
