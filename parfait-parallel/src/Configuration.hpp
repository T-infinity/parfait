#include <vector>
#include <string>
#include "MessagePasser.h"


namespace Parfait {
    inline Configuration::Configuration(int number_of_grids, const std::vector<std::string>& grid_filenames,
            const std::vector<bool>& is_big_endian,
            const std::vector<Parfait::BoundaryConditionMap>& map_bcs,
            const std::vector<Parfait::MotionMatrix>& motion_matrices)
        :ngrids(number_of_grids),
        gridFilenames(grid_filenames),
        bigEndian(is_big_endian),
        mapbcVector(map_bcs),
        motionMatrices(motion_matrices)
    { }

    inline int Configuration::numberOfGrids() const { return ngrids; }

    inline bool Configuration::isBigEndian(int gridId) const { return bigEndian.at(gridId); }

    inline std::string Configuration::getFilename(int gridId) const { return gridFilenames.at(gridId); }

    inline Parfait::MotionMatrix Configuration::getMotionMatrix(int gridId) const { return motionMatrices.at(gridId); }

    inline Parfait::BoundaryConditionMap Configuration::getMapbcObject(int gridId) const {
        return mapbcVector.at(gridId);
    }

    inline int Configuration::getBoundaryCondition(int gridId, int tag) const {
        return mapbcVector.at(gridId).at(gridId).first;
    }

    inline void Configuration::print() const {
        if (0 == MessagePasser::Rank()) {
            for (unsigned int i = 0; i < gridFilenames.size(); i++) {
                printf("Grid %i:\n", i);
                printf("  -file        %s\n", gridFilenames.at(i).c_str());
                if (bigEndian.at(i))
                    printf("  -endianness  big\n");
                else
                    printf("  -endianness  little\n");
                printf("  -Motion matrix:");
                motionMatrices.at(i).printMatrix(stdout);
            }
        }
    }
}
