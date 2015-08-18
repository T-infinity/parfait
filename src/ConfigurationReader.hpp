#include <iostream>
#include <string>
namespace Parfait {
	inline ConfigurationReader::ConfigurationReader(std::string xmlFile)
			: filename(xmlFile) {
		load();
		readMapbcFiles();
	}

	inline int ConfigurationReader::numberOfGrids() { return ngrids; }

	inline bool ConfigurationReader::isBigEndian(int gridId) { return bigEndian[gridId]; }

	inline std::string ConfigurationReader::getFilename(int gridId) { return gridFilenames[gridId]; }

	inline Parfait::MotionMatrix ConfigurationReader::getMotionMatrix(int gridId) { return motionMatrices[gridId]; }

	inline Parfait::MapbcReader ConfigurationReader::getMapbcObject(int gridId) {
		return mapbcVector[gridId];
	}

	inline int ConfigurationReader::getBoundaryCondition(int gridId, int tag) {
		return mapbcVector[gridId].boundaryCondition(tag);
	}

	inline void ConfigurationReader::readMapbcFiles() {
		for (auto filename:gridFilenames)
			mapbcVector.push_back(filename.substr(0, filename.length() - 5).append("mapbc"));
	}

	inline void ConfigurationReader::load() {
		TiXmlDocument doc(filename.c_str());
		bool loadOkay = doc.LoadFile();
		if (loadOkay) {
			TiXmlHandle docHandle(&doc);
			TiXmlHandle GlobalHandle = docHandle.FirstChild("global");
			loadComponents(GlobalHandle);
		}
		else
			throw std::domain_error(std::string("Problem loading xml file: ")+filename);
	}


	inline void ConfigurationReader::loadComponents(TiXmlHandle GlobalHandle) {
		int i = 1;
		ngrids = 0;
		// Get the element for the first component grid
		TiXmlElement *ComponentElement = GlobalHandle.FirstChild("component").ToElement();
		// Get a handle for the first component grid
		TiXmlHandle ComponentHandle = GlobalHandle.FirstChild("component");
		// loop over all components
		while (ComponentElement) {
			// set the filename for this component
			gridFilenames.push_back(ComponentElement->Attribute("filename"));
			const char *endian = ComponentElement->Attribute("endian");
			int state = -1;
			bool big_endian = false;
			if (endian) {
				if (endian[0] == 'b' || endian[0] == 'B') if (endian[1] == 'i' || endian[1] == 'I') if (endian[2] == 'g' ||
																																																endian[2] == 'G')
					state = 1;

				if (endian[0] == 'l' || endian[0] == 'L') if (endian[1] == 'i' || endian[1] == 'I') if (endian[2] == 't' ||
																																																endian[2] == 'T')
					state = 0;
				if (state == -1)
					fprintf(stderr, "Warning: endian=(%s) is invalid, defaulting to little endian\n", endian);
			}
			if (state == 1) {
				big_endian = true;
				if (MessagePasser::Rank() == 0)
					fprintf(stdout, "Reading %s as big endian\n", gridFilenames.back().c_str());
			}
			else {
				big_endian = false;
				if (MessagePasser::Rank() == 0)
					fprintf(stdout, "Reading %s as little endian\n", gridFilenames.back().c_str());
			}

			// set endianness for component
			bigEndian.push_back(big_endian);

			// if the component has a translation defined, set it
			TiXmlElement *TranslateElement;
			TranslateElement = ComponentHandle.FirstChild("translate").ToElement();
			double translation[3];
			translation[0] = translation[1] = translation[2] = 0.0;
			if (TranslateElement) {
				double tmp;
				int flag = TranslateElement->QueryDoubleAttribute("x", &tmp);
				if (flag == TIXML_SUCCESS)
					translation[0] = tmp;
				flag = TranslateElement->QueryDoubleAttribute("y", &tmp);
				if (flag == TIXML_SUCCESS)
					translation[1] = tmp;
				flag = TranslateElement->QueryDoubleAttribute("z", &tmp);
				if (flag == TIXML_SUCCESS)
					translation[2] = tmp;
			}
			//printf("translation = %lf %lf %lf\n",translation[0],translation[1],translation[2]);
			motionMatrices.push_back(Parfait::MotionMatrix());
			motionMatrices.back().setTranslation(translation);

			// if the component has a rotation defined
			TiXmlElement *RotateElement;
			RotateElement = ComponentHandle.FirstChild("rotate").ToElement();
			double p1[3], p2[3], rotation_angle = 0.0;
			p1[0] = p1[1] = p1[2] = p2[1] = p2[2] = 0.0;
			p2[0] = 1.0; // default to 0 deg rotation about x
			if (RotateElement) {
				double tmp;
				int flag = RotateElement->QueryDoubleAttribute("angle", &tmp);
				if (flag == TIXML_SUCCESS)
					rotation_angle = tmp;
				else {
					rotation_angle = 0.0;
					fprintf(stdout, "Warning: Rotation defined with no angle of rotation\n");
				}
				TiXmlHandle RotationHandle = ComponentHandle.FirstChild("rotate");
				TiXmlElement *AxisBeginElement = RotationHandle.FirstChild("axis_begin").ToElement();
				TiXmlElement *AxisEndElement = RotationHandle.FirstChild("axis_end").ToElement();
				if (AxisBeginElement) {
					flag = AxisBeginElement->QueryDoubleAttribute("x", &tmp);
					if (flag == TIXML_SUCCESS)
						p1[0] = tmp;
					flag = AxisBeginElement->QueryDoubleAttribute("y", &tmp);
					if (flag == TIXML_SUCCESS)
						p1[1] = tmp;
					flag = AxisBeginElement->QueryDoubleAttribute("z", &tmp);
					if (flag == TIXML_SUCCESS)
						p1[2] = tmp;
				}
				if (AxisEndElement) {
					flag = AxisEndElement->QueryDoubleAttribute("x", &tmp);
					if (flag == TIXML_SUCCESS)
						p2[0] = tmp;
					flag = AxisEndElement->QueryDoubleAttribute("y", &tmp);
					if (flag == TIXML_SUCCESS)
						p2[1] = tmp;
					flag == AxisEndElement->QueryDoubleAttribute("z", &tmp);
					if (flag == TIXML_SUCCESS)
						p2[2] = tmp;
				}
				else {
					fprintf(stdout, "Warning, no endpoint defined for axis of rotation,\n");
					fprintf(stdout, "     defaulting to rotation about x-axis at provided point (%lf %lf %lf)\n",
									p1[0], p1[1], p1[2]);
					p2[0] = p1[0] + 1.0;
					p2[1] = p1[1];
					p2[2] = p1[2];
				}
			}
			motionMatrices.back().addRotation(p1, p2, rotation_angle);

			// get the next component & its handle
			ComponentElement = GlobalHandle.Child("component", i).ToElement();
			ComponentHandle = GlobalHandle.Child("component", i);
			++i;
			ngrids++;
		}
	}
}