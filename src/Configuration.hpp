
namespace Parfait {
    Configuration::Configuration(int number_of_grids, std::shared_ptr<std::vector<std::string>> grid_filenames,
                                 std::shared_ptr<std::vector<bool>> is_big_endian,
                                 std::shared_ptr<std::vector<Parfait::MapbcReader>> map_bcs,
                                 std::shared_ptr<std::vector<Parfait::MotionMatrix>> motion_matrices) {

        throw std::logic_error("constructor not implemented yet")
    }

	inline int Configuration::numberOfGrids() { return ngrids; }

    inline std::vector<std::string> Configuration::getGridFilenames() {
        return gridFilenames;
    }

    inline std::vector<bool> Configuration::getGridEndianness() {
        return bigEndian;
    }

	inline bool Configuration::isBigEndian(int gridId) { return bigEndian[gridId]; }

	inline std::string Configuration::getFilename(int gridId) { return gridFilenames[gridId]; }

	inline Parfait::MotionMatrix Configuration::getMotionMatrix(int gridId) { return motionMatrices[gridId]; }

	inline Parfait::MapbcReader Configuration::getMapbcObject(int gridId) {
		return mapbcVector[gridId];
	}

	inline int Configuration::getBoundaryCondition(int gridId, int tag) {
		return mapbcVector[gridId].boundaryCondition(tag);
	}

    void Configuration::print() {
        if (0 == MessagePasser::Rank()) {
            for (int i = 0; i < gridFilenames.size(); i++) {
                printf("Grid %i:\n", i);
                printf("  -file        %s\n", gridFilenames[i].c_str());
                if (bigEndian[i])
                    printf("  -endianness  big\n");
                else
                    printf("  -endianness  little\n");
                printf("  -Motion matrix:");
                motionMatrices[i].printMatrix(stdout);
            }
        }
    }
}