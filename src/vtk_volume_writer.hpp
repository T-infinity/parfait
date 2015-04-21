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
    vtkSmartPointer<vtkCellArray> tets = 
        vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> pyramids = 
        vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> prisms = 
        vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> hexs = 
        vtkSmartPointer<vtkCellArray>::New();

    for(int i=0;i<mesh.numberOfCells();i++){
        auto cell = mesh.getVtkOrderedNodesInCell(i);
        if(4 == cell.size()){
            vtkSmartPointer<vtkTetra> tet = 
                vtkSmartPointer<vtkTetra>::New();
            for(int j=0;j<4;j++)
                tet->GetPointIds()->SetId(j,cell[j]);
            tets->InsertNextCell(tet);
        }
    }
    vtk_grid->SetCells(VTK_TETRA,tets);
}
