#include <stdio.h>
#include <array>
#include <VtkUnstructuredWriter.h>
#include <Point.h>
#include "parfait_vis_interface.h"
#include "VtkUnstructuredWriter.h"

using namespace Parfait;

class MeshWrapper{
public:
    int numberOfNodes(){return nnodes();};
    Point<double> getNode(int id){
        Point<double> p;
        getPoint(id,p.data());
        return p;
    }
    int numberOfNodesInCell(int id){return nnodesInCell(id);};
    std::vector<int> getNodesInCell(int id){
        std::vector<int> cell(numberOfNodesInCell(id));
        getCell(id,cell.data());
        return cell;
    };
    int numberOfCells(){return ncells();}

    int (*nnodes)();
    int (*ncells)();
    int (*nnodesInCell)(int);
    void (*getCell)(int,int*);
    void (*getPoint)(int,double*);

};


static MeshWrapper mesh;



void vtk_register(int (*numberOfNodes)(),
                                 int (*numberOfCells)(),
                                 int (*numberOfNodesInCell)(int),
                                 void (*getNodesInCell)(int,int*),
                                 void (*getPoint)(int,double*)){
    printf("parfait vtk vis, registering functions from solver\n");
    printf("Mesh data\n");
    printf("------------------------------------------\n");
    printf("# nodes: %i\n",numberOfNodes());
    printf("# cells: %i\n",numberOfCells());

    mesh.nnodes = numberOfNodes;
    mesh.ncells = numberOfCells;
    mesh.getCell = getNodesInCell;
    mesh.getPoint = getPoint;
    mesh.nnodesInCell = numberOfNodesInCell;
}


void vtk_cleanup(){
}

void vtk_create_volume_visualization(const char* name){
    VtkUnstructuredWriter writer(name,mesh);
    writer.writeBinary();
}
