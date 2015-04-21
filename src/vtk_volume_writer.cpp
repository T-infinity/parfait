#ifdef PARFAIT_WITH_VTK
#include "vtk_volume_writer.h"
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLPUnstructuredGridWriter.h>
#include "message_passer.h"

void VtkVolumeWriter::writeBinary(){
    using namespace MessagePasser;
    vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
        vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    std::string filename;
    if(NumberOfProcesses() > 1)
        filename = base_name + "_" 
            + std::to_string(Rank())+".vtu";
    else
        filename = base_name + ".vtu";

    writer->SetFileName(filename.c_str());
    writer->SetInputData(vtk_grid);
    writer->Write();
    if(NumberOfProcesses() > 1 and Rank() == 0){
        vtkSmartPointer<vtkXMLPUnstructuredGridWriter> pwriter =
            vtkSmartPointer<vtkXMLPUnstructuredGridWriter>::New();
        std::string parallel_filename = base_name + ".pvtu";
        pwriter->SetFileName(parallel_filename.c_str());
        pwriter->SetNumberOfPieces(NumberOfProcesses());
        pwriter->SetInputData(vtk_grid);
        pwriter->Write(); 
    }
}
#endif
