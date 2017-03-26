#include "VolumeMeshInterface.h"
#include <VtkUnstructuredWriter.h>
#include "ParfaitVizualizer.h"
#include "VtkPlugin.h"

class VtkVizualizer : public ParfaitVizualizer {
private:
    const Parfait::VolumeMeshInterface* mesh;
    std::vector<int*> integer_datas;
    std::vector<std::string>  integer_names;
    std::vector<int>  integer_counts;
    std::vector<double*> double_datas;
    std::vector<std::string>  double_names;
    std::vector<int>  double_counts;
public:
    VtkVizualizer(const Parfait::VolumeMeshInterface* mesh) : mesh(mesh){}

    virtual void writeBinary(std::string name) const override {
        Parfait::VtkUnstructuredWriter writer(name, *mesh);
        printf("Plugin: writing serial binary\n");
        writer.writeBinary();
    }

    virtual void writeBinary(std::string name, int rank, int nproc) const override {
        Parfait::VtkUnstructuredWriter writer(name, *mesh);
        printf("Plugin: Rank %i of %i\n",rank,nproc);
        for(int i=0;i<integer_names.size();++i){
          writer.addNodeData(integer_names[i],integer_datas[i],integer_counts[i]);
        }
        for(int i=0;i<double_names.size();++i){
          writer.addNodeData(double_names[i],double_datas[i],double_counts[i]);
        }
        writer.writeBinary(rank,nproc);
    }

    virtual void addNodeData(std::string name, int* data, int number_of_components){
        integer_datas.push_back(data);
        integer_names.push_back(name);
        integer_counts.push_back(number_of_components);
    }
    
    virtual void addNodeData(std::string name, double* data, int number_of_components){
        double_datas.push_back(data);
        double_names.push_back(name);
        double_counts.push_back(number_of_components);
    }
};

ParfaitVizualizer* registerMesh(const Parfait::VolumeMeshInterface* mesh){
    printf("Creating a new VtkVizualizer...\n");
    return new VtkVizualizer(mesh);
}
