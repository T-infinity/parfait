#ifndef VTK_VOLUME_WRITER_H
#define VTK_VOLUME_WRITER_H

#ifdef PARFAIT_WITH_VTK
#include <string>
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
class VtkVolumeWriter{
    public:
        template<typename MeshType>
        VtkVolumeWriter(std::string name,MeshType& mesh);

        void addNodeData(std::string name,int* data,int number_of_components);
    private:
        std::string base_name;
        vtkSmartPointer<vtkUnstructuredGrid> vtk_grid =
            vtkSmartPointer<vtkUnstructuredGrid>::New();
        
        template<typename MeshType>
        void setPoints(MeshType& mesh);

        template<typename MeshType>
        void setCells(MeshType& mesh);
};


#include "vtk_volume_writer.hpp"

#endif // PARFAIT_WITH_VTK
#endif
