#include <stdio.h>
#include "ImportedUgridFactory.h"
#include "STL.h"

int main(int argc, const char* argv[]) {

    if(argc != 2) {
        printf("\nUsage: to convert ugrid surfaces to stl");
        exit(0);
    }

    std::string filename = argv[1];

    auto ugrid = Parfait::ImportedUgridFactory::readUgridAscii(filename);
//    printf("File <%s> has: %d triangles %d nodes", filename.c_str(),
//           ugrid.triangles.size()/3, ugrid.nodes.size());

    Parfait::STL::STL surface;
    for(int cell_id = 0; cell_id < ugrid.triangles.size() / 3; cell_id++){
        auto* tri = &ugrid.triangles[3*cell_id];
        Parfait::Facet f;
        for(int i = 0; i < 3; i++){
            int node_id = 3*tri[i];
            f[i][0] = ugrid.nodes[node_id + 0];
            f[i][1] = ugrid.nodes[node_id + 1];
            f[i][2] = ugrid.nodes[node_id + 2];
        }
        surface.facets.push_back(f);
    }

    surface.writeAsciiFile(filename+".stl");
}