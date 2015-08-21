
namespace Parfait {
	inline ConfigurationReader::ConfigurationReader(std::string xmlFile)
			: filename(xmlFile) {
		load();
		readMapbcFiles();
	}

	inline int ConfigurationReader::numberOfGrids() { return ngrids; }

    inline std::vector<std::string> ConfigurationReader::getGridFilenames() {
        return gridFilenames;
    }

    inline std::vector<bool> ConfigurationReader::getGridEndianness() {
        return bigEndian;
    }

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
		TiXmlElement *ComponentElement = GlobalHandle.FirstChild("component").ToElement();
		TiXmlHandle ComponentHandle = GlobalHandle.FirstChild("component");
		while (ComponentElement) {
			gridFilenames.push_back(ComponentElement->Attribute("filename"));
            bigEndian.push_back(getEndiannessForComponent(ComponentElement));

            // if the component has a translation defined, set it
            TiXmlElement *TranslateElement;
            TranslateElement = ComponentHandle.FirstChild("translate").ToElement();
            Point<double> translation;
            translation[0] = translation[1] = translation[2] = 0.0;
            motionMatrices.push_back(Parfait::MotionMatrix());
            if (TranslateElement) {
                translation = getXYZ(TranslateElement);
                motionMatrices.back().setTranslation(translation.data());
            }
            getRotationForComponent(ComponentHandle);

            // get the next component & its handle
			ComponentElement = GlobalHandle.Child("component", i).ToElement();
			ComponentHandle = GlobalHandle.Child("component", i);
			++i;
			ngrids++;
		}
        throwIfConfigurationIsInsane();
        print();
	}

    void ConfigurationReader::getRotationForComponent(const TiXmlHandle &ComponentHandle) {
        TiXmlElement *RotateElement;
        RotateElement = ComponentHandle.FirstChild("rotate").ToElement();
        double rotation_angle = 0.0;
        Point<double> p1,p2;
        if (RotateElement) {
                double tmp;
                int flag;
                flag = RotateElement->QueryDoubleAttribute("angle", &tmp);
                if (flag == TIXML_SUCCESS)
                    rotation_angle = tmp;
                else
                	throw std::logic_error("Warning: Rotation defined with no angle of rotation");

                TiXmlHandle RotationHandle = ComponentHandle.FirstChild("rotate");
                TiXmlElement *AxisBeginElement = RotationHandle.FirstChild("axis_begin").ToElement();
                TiXmlElement *AxisEndElement = RotationHandle.FirstChild("axis_end").ToElement();

                throwIfBadElement(AxisBeginElement);
                throwIfBadElement(AxisEndElement);

                p1 = getXYZ(AxisBeginElement);
                p2 = getXYZ(AxisEndElement);
                motionMatrices.back().addRotation(p1.data(), p2.data(), rotation_angle);
            }
    }

    bool ConfigurationReader::getEndiannessForComponent(const TiXmlElement *ComponentElement) {
        return figureOutEndianness(ComponentElement->Attribute("endian"));
    }

    const Parfait::Point<double> ConfigurationReader::getXYZ(TiXmlElement *element) const {
        Parfait::Point<double> p;
        int flag;
        double tmp;
        flag = element->QueryDoubleAttribute("x", &tmp);
        if (flag == TIXML_SUCCESS)
                        p[0] = tmp;
        flag = element->QueryDoubleAttribute("y", &tmp);
        if (flag == TIXML_SUCCESS)
                        p[1] = tmp;
        flag = element->QueryDoubleAttribute("z", &tmp);
        if (flag == TIXML_SUCCESS)
                        p[2] = tmp;
        return p;
    }

    inline bool ConfigurationReader::figureOutEndianness(std::string s){
        bool isLittle = 'l' == tolower(s.front());
        bool isBig = 'b' == tolower(s.front());
        if(isLittle)
            return false;
        if(isBig)
            return true;
        throw std::domain_error(std::string("Could not determine endianness based on input: ")+s);
    }

    void ConfigurationReader::throwIfBadElement(TiXmlElement *element) {
        if(not element)
            throw std::domain_error("Couldn't find expected xml tag");
    }

    void ConfigurationReader::throwIfConfigurationIsInsane() {
        if(gridFilenames.size() != motionMatrices.size() or
                gridFilenames.size() != bigEndian.size())
            throw std::logic_error("Configuration doesn't make sense");
    }

    void ConfigurationReader::print() {
        for(int i=0;i<gridFilenames.size();i++){
            printf("Grid %i:\n",i);
            printf("  -file        %s\n",gridFilenames[i].c_str());
            if(bigEndian[i])
                printf("  -endianness  big\n");
            else
                printf("  -endianness  little\n");
            printf("  -Motion matrix:");
            motionMatrices[i].printMatrix(stdout);
        }
    }
}