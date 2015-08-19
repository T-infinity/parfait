#ifndef CONFIGURATION_READER_H
#define CONFIGURATION_READER_H
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

namespace Parfait {
	class ConfigurationReader {
	public:
		ConfigurationReader(std::string xmlFile);
		int numberOfGrids();
		std::string getFilename(int gridId);
		bool isBigEndian(int gridId);
		Parfait::MotionMatrix getMotionMatrix(int gridId);
		int getBoundaryCondition(int gridId, int tag);
		Parfait::MapbcReader getMapbcObject(int gridId);
		void print();
	private:
		int ngrids;
		std::string filename;
		std::vector<std::string> gridFilenames;
		std::vector<bool> bigEndian;
		std::vector<Parfait::MapbcReader> mapbcVector;
		std::vector<Parfait::MotionMatrix> motionMatrices;
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
