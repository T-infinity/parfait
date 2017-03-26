#include <iostream>
#include <ConfigurationReader.h>
#include <ParallelMesh.h>
#include <PreProcessor.h>

namespace Parfait {

    inline std::vector<std::shared_ptr<Parfait::ParallelMesh>> importMeshesAsSystem(std::string filename){
        ConfigurationReader reader(filename);
        auto config = reader.createConfiguration();

        std::vector<Parfait::Configuration> system_configs;
        for(int g = 0; g < config.numberOfGrids(); g++){
            auto file_names = std::vector<std::string>();
            file_names.push_back(config.getFilename(g));

            auto is_big_endian = std::vector<bool>();
            is_big_endian.push_back(config.isBigEndian(g));

            auto map_bcs = std::vector<Parfait::BoundaryConditionMap>();
            map_bcs.push_back(config.getMapbcObject(g));

            auto motion_matrices = std::vector<Parfait::MotionMatrix>();
            motion_matrices.push_back(config.getMotionMatrix(g));

            system_configs.push_back({1, file_names, is_big_endian, map_bcs, motion_matrices});
        }


        std::vector<std::shared_ptr<Parfait::ParallelMesh>> grid_system;
        for(auto c : system_configs){
            PreProcessor preProcessor(c);
            auto g = preProcessor.createMesh();
            grid_system.push_back(g);
        }
        return grid_system;
    }
}
