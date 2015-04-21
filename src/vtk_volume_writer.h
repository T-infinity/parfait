#ifndef VTK_VOLUME_WRITER_H
#define VTK_VOLUME_WRITER_H

#ifdef PARFAIT_WITH_VTK
#include <string>
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellArray.h>
#include <vtkTetra.h>
#include <stdio.h>
class VtkVolumeWriter{
    public:
        template<typename MeshType>
        VtkVolumeWriter(std::string name,MeshType& mesh);

        void writeBinary();

        void addNodeData(std::string name,int* data,int number_of_components);
        void addNodeData(std::string name,float* data,int number_of_components);
        void addNodeData(std::string name,double* data,int number_of_components);
        void addCellData(std::string name,int* data,int number_of_components);
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
