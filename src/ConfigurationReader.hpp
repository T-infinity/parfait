
namespace Parfait {
    inline ConfigurationReader::ConfigurationReader(std::string xmlFile)
            : filename(xmlFile),
              gridFilenames(std::make_shared<std::vector<std::string>>()),
              bigEndian(std::make_shared<std::vector<bool>>()),
              mapbcVector(std::make_shared<std::vector<BoundaryConditionMap>>()),
              motionMatrices(std::make_shared<std::vector<Parfait::MotionMatrix>>())
    {
		load();
		readMapbcFiles();
	}

    inline Configuration ConfigurationReader::createConfiguration() {
        return Configuration(ngrids,gridFilenames,bigEndian,mapbcVector,motionMatrices);
    }

	inline void ConfigurationReader::readMapbcFiles() {
		for (auto filename:*gridFilenames) {
            std::vector<int> tags;
            std::vector<int> bcs;
            std::vector<std::string> names;
            if(MessagePasser::Rank() == 0) {
                auto reader = MapbcReader(MapbcReader(filename.substr(0, filename.length() - 5).append("mapbc")));
                for(auto x:reader.getMap()){
                    tags.push_back(x.first);
                    bcs.push_back(x.second.first);
                    names.push_back(x.second.second);
                }
            }
            MessagePasser::Broadcast(tags,0);
            MessagePasser::Broadcast(bcs,0);
            names.resize(tags.size());
            for(int i=0;i<int(names.size());++i){
                int n = (int)names[i].size();
                MessagePasser::Broadcast(n,0);
                std::vector<char> tmp(n);
                if(MessagePasser::Rank() == 0)
                    tmp = std::vector<char>(names[i].data(),names[i].data()+names[i].size());
                MessagePasser::Broadcast(tmp,0);
                names[i] = std::string(tmp.data(),tmp.data()+tmp.size());
            }
            BoundaryConditionMap bcMap;
            for(int i=0;i<(int)tags.size();++i)
                bcMap[tags[i]] = std::make_pair(bcs[i],names[i]);

            mapbcVector->push_back(bcMap);
        }
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
			gridFilenames->push_back(ComponentElement->Attribute("filename"));
            bigEndian->push_back(getEndiannessForComponent(ComponentElement));

            // if the component has a translation defined, set it
            TiXmlElement *TranslateElement;
            TranslateElement = ComponentHandle.FirstChild("translate").ToElement();
            Point<double> translation;
            translation[0] = translation[1] = translation[2] = 0.0;
            motionMatrices->push_back(Parfait::MotionMatrix());
            if (TranslateElement) {
                translation = getXYZ(TranslateElement);
                motionMatrices->back().setTranslation(translation.data());
            }
            getRotationForComponent(ComponentHandle);

            // get the next component & its handle
			ComponentElement = GlobalHandle.Child("component", i).ToElement();
			ComponentHandle = GlobalHandle.Child("component", i);
			++i;
			ngrids++;
		}
        throwIfConfigurationIsInsane();
	}

    inline void ConfigurationReader::getRotationForComponent(const TiXmlHandle &ComponentHandle) {
        for(auto Rotation = ComponentHandle.FirstChild("rotate").ToElement(); Rotation;) {
                double angle;
                if (TIXML_SUCCESS != Rotation->QueryDoubleAttribute("angle", &angle))
                	throw std::logic_error("Rotation defined with no angle of rotation");

                auto AxisBeginElement = Rotation->FirstChild("axis_begin")->ToElement();
                auto AxisEndElement = Rotation->FirstChild("axis_end")->ToElement();

                throwIfBadElement(AxisBeginElement);
                throwIfBadElement(AxisEndElement);

                auto p1 = getXYZ(AxisBeginElement);
                auto p2 = getXYZ(AxisEndElement);
                motionMatrices->back().addRotation(p1.data(), p2.data(), angle);
                auto next = Rotation->NextSibling("rotate");
                if(not next)
                    break;
                Rotation = next->ToElement();
            }
    }

    inline bool ConfigurationReader::getEndiannessForComponent(const TiXmlElement *ComponentElement) {
        return figureOutEndianness(ComponentElement->Attribute("endian"));
    }

    inline const Parfait::Point<double> ConfigurationReader::getXYZ(TiXmlElement *element) const {
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

    inline void ConfigurationReader::throwIfBadElement(TiXmlElement *element) {
        if(not element)
            throw std::domain_error("Couldn't find expected xml tag");
    }

    inline void ConfigurationReader::throwIfConfigurationIsInsane() {
        if(gridFilenames->size() != motionMatrices->size() or
                gridFilenames->size() != bigEndian->size())
            throw std::logic_error("Configuration doesn't make sense");
    }
}