#include <string>
#include <ParallelMeshReader.h>
#include <PreProcessor.h>
#include <GenericMeshTools.h>
#include <DualMetrics.h>



int main(int argc, char*argv[]){
    MessagePasser::Init();

    std::string input_xml = SIX_CELL_TEST_XML;
    std::cout << input_xml << std::endl;
    ConfigurationReader reader(input_xml);
    Configuration config = reader.createConfiguration();

    PreProcessor preProcessor(config);
    auto mesh = preProcessor.createMesh();

    int number_of_owned_nodes = mesh->countNodesAtDegree(0);
    std::vector<double> node_volumes(number_of_owned_nodes, 0);
    auto edges = Parfait::GenericMeshTools::getUniqueEdges(*mesh);
    std::vector<Parfait::Point<double>> edge_areas(edges.size(), {0,0,0});

    DualMetrics metrics = {node_volumes, edge_areas};
    auto getTet     = [&](int i){return mesh->getTet(i);};
    auto getPyramid = [&](int i){return mesh->getPyramid(i);};
    auto getPrism   = [&](int i){return mesh->getPrism(i);};
    auto getHex     = [&](int i){return mesh->getHex(i);};
    auto getXyz     = [&](int i){return mesh->getXyz(i);};

    addTetDualMetrics(getTet, getXyz, mesh->numberOfTets(), edges, metrics);
    addPyramidDualMetrics(getPyramid, getXyz, mesh->numberOfPyramids(), edges, metrics);
    addPrismDualMetrics(getPrism, getXyz, mesh->numberOfPrisms(), edges, metrics);
    addHexDualMetrics(getPrism, getXyz, mesh->numberOfPrisms(), edges, metrics);

    BoundaryConditionMap boundaryConditionMap = config.getMapbcObject(0);
    MessagePasser::Finalize();
}
