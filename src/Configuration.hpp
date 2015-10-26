
namespace Parfait {
    inline Configuration::Configuration(int number_of_grids, std::shared_ptr<std::vector<std::string>> grid_filenames,
                                 std::shared_ptr<std::vector<bool>> is_big_endian,
                                        std::shared_ptr<std::vector<Parfait::BoundaryConditionMap>> map_bcs,
                                        std::shared_ptr<std::vector<Parfait::MotionMatrix>> motion_matrices)
            :ngrids(number_of_grids),
             gridFilenames(grid_filenames),
             bigEndian(is_big_endian),
             mapbcVector(map_bcs),
            motionMatrices(motion_matrices)
    {
    }

    inline int Configuration::numberOfGrids() { return ngrids; }

    inline bool Configuration::isBigEndian(int gridId) { return bigEndian->at(gridId); }

    inline std::string Configuration::getFilename(int gridId) { return gridFilenames->at(gridId); }

    inline Parfait::MotionMatrix Configuration::getMotionMatrix(int gridId) { return motionMatrices->at(gridId); }

    inline Parfait::BoundaryConditionMap Configuration::getMapbcObject(int gridId) {
        return mapbcVector->at(gridId);
    }

    inline int Configuration::getBoundaryCondition(int gridId, int tag) {
        return mapbcVector->at(gridId)[gridId].first;
    }

    inline void Configuration::print() {
        if (0 == MessagePasser::Rank()) {
            for (unsigned int i = 0; i < gridFilenames->size(); i++) {
                printf("Grid %i:\n", i);
                printf("  -file        %s\n", gridFilenames->at(i).c_str());
                if (bigEndian->at(i))
                    printf("  -endianness  big\n");
                else
                    printf("  -endianness  little\n");
                printf("  -Motion matrix:");
                motionMatrices->at(i).printMatrix(stdout);
            }
        }
    }
}