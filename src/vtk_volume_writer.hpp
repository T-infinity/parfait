template<typename MeshType>
VtkVolumeWriter::VtkVolumeWriter(std::string name,MeshType& mesh)
    :base_name(name)
{

    setPoints(mesh);
    setCells(mesh);
}

template<typename MeshType>
void VtkVolumeWriter::setPoints(MeshType& mesh){
    vtkSmartPointer<vtkPoints> points = 
        vtkSmartPointer<vtkPoints>::New();
    for(int i=0;i<mesh.numberOfNodes();i++){
        double p[3];
        mesh.getNode(i,p);
        points->InsertNextPoint(p[0],p[1],p[2]);
    }
    vtk_grid->SetPoints(points);
}

template<typename MeshType>
void VtkVolumeWriter::setCells(MeshType& mesh){
    vtkSmartPointer<vtkTetra> tet = 
        vtkSmartPointer<vtkTetra>::New();
    vtkSmartPointer<vtkPyramid> pyramid = 
        vtkSmartPointer<vtkPyramid>::New();
    vtkSmartPointer<vtkWedge> prism = 
        vtkSmartPointer<vtkWedge>::New();
    vtkSmartPointer<vtkHexahedron> hex = 
        vtkSmartPointer<vtkHexahedron>::New();
    
    vtkSmartPointer<vtkCellArray> tets = 
        vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> pyramids = 
        vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> prisms = 
        vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> hexs = 
        vtkSmartPointer<vtkCellArray>::New();

    for(int i=0;i<mesh.numberOfCells();i++){
        auto cell_nodes = mesh.getVtkOrderedNodesInCell(i);
        int n = cell_nodes.size();
        if(4 == n){
            for(int j=0;j<n;j++)
                tet->GetPointIds()->SetId(j,cell_nodes[j]);
            tets->InsertNextCell(tet);
        }
        if(5 == n){
            for(int j=0;j<n;j++)
                pyramid->GetPointIds()->SetId(j,cell_nodes[j]);
            pyramids->InsertNextCell(tet);
        }
        if(6 == n){
            for(int j=0;j<n;j++)
                prism->GetPointIds()->SetId(j,cell_nodes[j]);
            prisms->InsertNextCell(tet);
        }
        if(8 == n){
            for(int j=0;j<n;j++)
                hex->GetPointIds()->SetId(j,cell_nodes[j]);
            hexs->InsertNextCell(tet);
        }
    }
    vtk_grid->SetCells(VTK_TETRA,tets);
}

template<typename T>
void VtkVolumeWriter::addNodeData(std::string name,
        T* data,int number_of_components){
    auto stuff = createVtkArrayPointer(data);
    stuff->SetNumberOfComponents(number_of_components);
    stuff->SetName(name.c_str());
    for(int i=0;i<vtk_grid->GetNumberOfPoints();i++)
        stuff->InsertNextValue(data[i]);
    vtk_grid->GetPointData()->AddArray(stuff);
}

template<typename T>
void VtkVolumeWriter::addCellData(std::string name,
        T* data,int number_of_components){
    auto stuff = createVtkArrayPointer(data);
    stuff->SetNumberOfComponents(number_of_components);
    stuff->SetName(name.c_str());
    for(int i=0;i<vtk_grid->GetNumberOfCells();i++)
        stuff->InsertNextValue(data[i]);
    vtk_grid->GetCellData()->AddArray(stuff);
}


inline void VtkVolumeWriter::writeBinary(){
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
