#ifndef VTK_VOLUME_WRITER_H
#define VTK_VOLUME_WRITER_H

//#ifdef PARFAIT_WITH_VTK
#if 1

#include <string>
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellArray.h>
#include <vtkTetra.h>
#include <vtkPyramid.h>
#include <vtkWedge.h>
#include <vtkHexahedron.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <stdio.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLPUnstructuredGridWriter.h>
#include <MessagePasser.h>

class VtkVolumeWriter{
    public:
        template<typename MeshType>
        VtkVolumeWriter(std::string name,MeshType& mesh);

        void writeBinary();
        
        template<typename T>
        void addNodeData(std::string name,T* data,int number_of_components);
       
        template<typename T>
        void addCellData(std::string name,T* data,int number_of_components);
    private:
        std::string base_name;
        vtkSmartPointer<vtkUnstructuredGrid> vtk_grid =
            vtkSmartPointer<vtkUnstructuredGrid>::New();
        
        template<typename MeshType>
        void setPoints(MeshType& mesh);

        template<typename MeshType>
        void setCells(MeshType& mesh);

        vtkSmartPointer<vtkIntArray> createVtkArrayPointer(int* data){
            return vtkSmartPointer<vtkIntArray>::New();
        }
        vtkSmartPointer<vtkFloatArray> createVtkArrayPointer(float* data){
            return vtkSmartPointer<vtkFloatArray>::New();
        }
        vtkSmartPointer<vtkDoubleArray> createVtkArrayPointer(double* data){
            return vtkSmartPointer<vtkDoubleArray>::New();
        }
};


#include "VtkVolumeWriter.hpp"

#endif // PARFAIT_WITH_VTK
#endif
