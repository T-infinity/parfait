
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.

namespace Parfait {
  inline ConfigurationReader::ConfigurationReader(std::string filename)
          : filename(filename),
            gridFilenames(std::vector<std::string>()),
            bigEndian(std::vector<bool>()),
            mapbcVector(std::vector<BoundaryConditionMap>()),
            motionMatrices(std::vector<Parfait::MotionMatrix>()) {
      load();
  }

  inline Configuration ConfigurationReader::createConfiguration() {
      return Configuration(ngrids, gridFilenames, bigEndian,
                           mapbcVector, motionMatrices);
  }

  inline auto ConfigurationReader::loadConfigFileAsJsonObject(
          std::string filename) {
      std::ifstream i(filename);
      return nlohmann::json::parse(i);
  }

    inline void throwIfNotFound(nlohmann::json json,const std::string& s){
        if(json.find(s) == json.end())
            throw std::logic_error("Parfait::ConfigurationReader: "+s+" not found in json object");
    }

  inline bool ConfigurationReader::getIsBigEndian(nlohmann::json& component) {
      if (component.find("is_big_endian") != component.end())
          return component["is_big_endian"];
      return false;
  }

  inline auto ConfigurationReader::getTranslation(nlohmann::json& component) {
      std::array<double, 3> T;
      if (component.find("translation") != component.end())
          for (int i = 0; i < 3; i++)
              T[i] = component["translation"][i];
      return T;
  }

  inline auto ConfigurationReader::createMotionMatrixForComponent(
          nlohmann::json& component) {
      MotionMatrix m;
      if (component.find("translation") != component.end())
          m.setTranslation(getTranslation(component).data());
      if (component.find("rotation") != component.end()) {
          auto rotation = component["rotation"];
          std::array<double, 3> p0, p1;
          for (int i = 0; i < 3; ++i) {
              throwIfNotFound(rotation,"axis_begin");
              p0[i] = rotation.at("axis_begin")[i];
              throwIfNotFound(rotation,"axis_end");
              p1[i] = rotation.at("axis_end")[i];
          }
          throwIfNotFound(rotation,"angle");
          double angle = rotation.at("angle");
          m.addRotation(p0.data(), p1.data(), angle);
      }
      return m;
  }


  inline void ConfigurationReader::load() {
      auto config = loadConfigFileAsJsonObject(filename);
      ngrids = 0;
      throwIfNotFound(config,"components");
      for (auto& component:config["components"]) {
          ngrids++;
          throwIfNotFound(component,"filename");
          gridFilenames.push_back(component.at("filename"));
          bigEndian.push_back(getIsBigEndian(component));
          motionMatrices.push_back(createMotionMatrixForComponent(component));
          std::vector<int> tags;
          std::vector<int> bcs;
          std::vector<std::string> names;
          auto gridFilename = gridFilenames.back();
          auto reader = MapbcReader(gridFilename.substr(0, gridFilename.length() - 5).append("mapbc"));
          for (auto x:reader.getMap()) {
              tags.push_back(x.first);
              bcs.push_back(x.second.first);
              names.push_back(x.second.second);
          }
          BoundaryConditionMap bcMap;
          for (int i = 0; i < (int) tags.size(); ++i) {
              bcMap[tags[i]] = std::make_pair(bcs[i], names[i]);
          }
          mapbcVector.push_back(bcMap);
      }
  }

  inline void ConfigurationReader::throwIfConfigurationIsInsane() {
      if (gridFilenames.size() != motionMatrices.size() or
          gridFilenames.size() != bigEndian.size())
          throw std::logic_error("Configuration doesn't make sense");
  }
}
