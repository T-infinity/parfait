#include "mock_bc_interface.h"
#include <ConfigurationReader.h>

using namespace Parfait;

class MockBoundaryCondition{
public:

    std::array<int, 4> buffer;
    int (*getFaceTag)(int);
    int* (*getFaceNodes)(int, int*);
    int (*getComponentId)(int);

    std::vector<BoundaryConditionMap> bcMaps;
    int getBoundaryCondition(int id) {
        int tag = getFaceTag(id);
        getFaceNodes(id, buffer.data());
        int first_vertex_id = buffer[0];
        int component_id = getComponentId(first_vertex_id);
        return bcMaps[component_id][tag].first;
    }
};

static MockBoundaryCondition mockBoundaryCondition;

void mock_bc_initialize(const char* filename,
                        int (*getFaceTag)(int),
                        void (*getFaceNodes)(int, int*),
                        int (*getComponentId)(int)
) {
    Parfait::ConfigurationReader reader(filename);
	auto config = reader.createConfiguration();
    for (int i = 0; i < config.numberOfGrids(); i++)
        mockBoundaryCondition.bcMaps.push_back(config.getMapbcObject(i));
}

int mock_get_boundary_condition(int face_id){
   mockBoundaryCondition.getBoundaryCondition(face_id);
}
