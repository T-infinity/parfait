
namespace Parfait {
    inline ConfigurationReader::ConfigurationReader(std::string filename)
            : filename(filename),
              gridFilenames(std::vector<std::string>()),
              bigEndian(std::vector<bool>()),
              mapbcVector(std::vector<BoundaryConditionMap>()),
              motionMatrices(std::vector<Parfait::MotionMatrix>())
    {
		load();
	}

    inline Configuration ConfigurationReader::createConfiguration() {
        return Configuration(ngrids,gridFilenames,bigEndian,
                mapbcVector,motionMatrices);
    }

    inline auto ConfigurationReader::loadConfigFileAsJsonObject(
            std::string filename){
        std::ifstream i(filename);
        return nlohmann::json::parse(i); 
    }
    
    inline bool ConfigurationReader::getIsBigEndian(nlohmann::json& component){
        if(component.find("is_big_endian") != component.end())
            return component["is_big_endian"];
        return false;
    }

    inline auto ConfigurationReader::getTranslation(nlohmann::json& component){
        std::array<double,3> T;
        if(component.find("translation") != component.end())
            for(int i=0;i<3;i++)
                T[i] = component["translation"][i];
        return T;
    }

    inline auto ConfigurationReader::createMotionMatrixForComponent(
            nlohmann::json& component){
        MotionMatrix m;
        if(component.find("translation") != component.end())
            m.setTranslation(getTranslation(component).data());
        if(component.find("rotation") != component.end()){
            auto rotation = component["rotation"];
            std::array<double,3> p0,p1;
            for(int i=0;i<3;++i){
             p0[i] = rotation.at("axis_begin")[i];
             p1[i] = rotation.at("axis_end")[i];     
            }
            double angle = rotation.at("angle");
            m.setRotation(p0.data(),p1.data(),angle);
        }
        return m;
    }
    
    inline void ConfigurationReader::load() {
        auto config = loadConfigFileAsJsonObject(filename);
        ngrids = 0;
        for(auto& component:config["components"]){
            ngrids++;
            gridFilenames.push_back(component.at("filename"));
            bigEndian.push_back(getIsBigEndian(component));
            motionMatrices.push_back(createMotionMatrixForComponent(component));
            std::vector<int> tags;
            std::vector<int> bcs;
            std::vector<std::string> names;
            auto gridFilename = gridFilenames.back();
            auto reader = MapbcReader(gridFilename.substr(0, gridFilename.length() - 5).append("mapbc"));
            for(auto x:reader.getMap()){
                tags.push_back(x.first);
                bcs.push_back(x.second.first);
                names.push_back(x.second.second);
            }
            BoundaryConditionMap bcMap;
            for(int i=0;i<(int)tags.size();++i){
                bcMap[tags[i]] = std::make_pair(bcs[i],names[i]);
            }
            mapbcVector.push_back(bcMap);
        }
    }

    inline void ConfigurationReader::throwIfConfigurationIsInsane() {
        if(gridFilenames.size() != motionMatrices.size() or
                gridFilenames.size() != bigEndian.size())
            throw std::logic_error("Configuration doesn't make sense");
    }
}
