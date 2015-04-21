#include "vtk_volume_writer.h"
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkIntArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>

void VtkVolumeWriter::addNodeData(std::string name,
        int* data,int number_of_components){
    vtkSmartPointer<vtkIntArray> stuff =
        vtkSmartPointer<vtkIntArray>::New();
    stuff->SetNumberOfComponents(number_of_components);
    stuff->SetName(name.c_str());
    for(int i=0;i<vtk_grid->GetNumberOfPoints();i++)
        stuff->InsertNextValue(data[i]);
    vtk_grid->GetPointData()->AddArray(stuff);
}

void VtkVolumeWriter::addCellData(std::string name,
        int* data,int number_of_components){
    vtkSmartPointer<vtkIntArray> stuff =
        vtkSmartPointer<vtkIntArray>::New();
    stuff->SetNumberOfComponents(number_of_components);
    stuff->SetName(name.c_str());
    for(int i=0;i<vtk_grid->GetNumberOfCells();i++)
        stuff->InsertNextValue(data[i]);
    vtk_grid->GetCellData()->AddArray(stuff);
}

void VtkVolumeWriter::writeBinary(){
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
        vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    std::string filename = base_name + ".vtu";
    writer->SetFileName(filename.c_str());
    writer->SetInputData(vtk_grid);
    writer->Write();
}
