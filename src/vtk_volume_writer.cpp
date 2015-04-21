#include "vtk_volume_writer.h"
#include <vtkXMLUnstructuredGridWriter.h>


void VtkVolumeWriter::writeBinary(){
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
        vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    std::string filename = base_name + ".vtu";
    writer->SetFileName(filename.c_str());
    writer->SetInputData(vtk_grid);
    writer->Write();
}
