namespace Parfait {
    template<typename MeshType>
    VtkUnstructuredSurfaceWriter::VtkUnstructuredSurfaceWriter(std::string name, MeshType &mesh)
    :base_name(name){
        setPoints(mesh);
        setCells(mesh);
    }

    template<typename MeshType>
    void VtkUnstructuredSurfaceWriter::setPoints(MeshType &mesh) {
        vtkSmartPointer<vtkPoints> points =
            vtkSmartPointer<vtkPoints>::New();
        for (int i = 0; i < mesh.numberOfNodes(); i++) {
            auto p = mesh.getNode(i);
            points->InsertNextPoint(p[0], p[1], p[2]);
        }
        vtk_grid->SetPoints(points);
    }

    template<typename MeshType>
    void VtkUnstructuredSurfaceWriter::setCells(MeshType &mesh) {

        vtkSmartPointer<vtkCellArray> cells =
            vtkSmartPointer<vtkCellArray>::New();
        std::vector<int> cell_types;


        for (int i = 0; i < mesh.numberOfBoundaryFaces(); i++) {
            auto cell_nodes = mesh.getNodesInBoundaryFace(i);
            int n = cell_nodes.size();
            if (3 == n) {
                vtkSmartPointer<vtkTriangle> cell =
                    vtkSmartPointer<vtkTriangle>::New();
                for (int j = 0; j < n; j++)
                    cell->GetPointIds()->SetId(j, cell_nodes[j]);
                cells->InsertNextCell(cell);
                cell_types.push_back(VTK_TRIANGLE);
            }
            else if(4 == n){
                vtkSmartPointer<vtkQuad> cell =
                    vtkSmartPointer<vtkQuad>::New();
                for (int j = 0; j < n; j++)
                    cell->GetPointIds()->SetId(j, cell_nodes[j]);
                cells->InsertNextCell(cell);
                cell_types.push_back(VTK_QUAD);
            }
        }
        vtk_grid->SetCells(cell_types.data(), cells);
    }

    inline void VtkUnstructuredSurfaceWriter::writeBinary() {
        writeLocalFile(base_name+".vtu");
    }

    inline void VtkUnstructuredSurfaceWriter::writeBinary(int rank,int nproc) {
        writeLocalFile(base_name+"_"+std::to_string(rank)+".vtu");
        if (0 == rank) {
            vtkSmartPointer<vtkXMLPUnstructuredGridWriter> pwriter =
                vtkSmartPointer<vtkXMLPUnstructuredGridWriter>::New();
            std::string parallel_filename = base_name + ".pvtu";
            pwriter->SetFileName(parallel_filename.c_str());
            pwriter->SetNumberOfPieces(nproc);
            pwriter->SetInputData(vtk_grid);
            pwriter->Write();
        }

    }

    inline void VtkUnstructuredSurfaceWriter::writeLocalFile(std::string filename){
      vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
          vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
      writer->SetFileName(filename.c_str());
      writer->SetInputData(vtk_grid);
      writer->Write();
  }

    template<typename T>
    void VtkUnstructuredSurfaceWriter::addNodeData(std::string name,
                                            T *data, int number_of_components) {
        auto stuff = createVtkArrayPointer(data);
        stuff->SetNumberOfComponents(number_of_components);
        stuff->SetName(name.c_str());
        for (int i = 0; i < vtk_grid->GetNumberOfPoints(); i++)
            stuff->InsertNextValue(data[i]);
        vtk_grid->GetPointData()->AddArray(stuff);
    }

    template<typename T>
    void VtkUnstructuredSurfaceWriter::addCellData(std::string name,
                                            T *data, int number_of_components) {
        auto stuff = createVtkArrayPointer(data);
        stuff->SetNumberOfComponents(number_of_components);
        stuff->SetName(name.c_str());
        for (int i = 0; i < vtk_grid->GetNumberOfCells(); i++)
            stuff->InsertNextValue(data[i]);
        vtk_grid->GetCellData()->AddArray(stuff);
    }

}