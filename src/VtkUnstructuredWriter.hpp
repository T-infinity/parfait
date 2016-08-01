namespace Parfait {
    template<typename MeshType>
    VtkUnstructuredWriter::VtkUnstructuredWriter(std::string name, MeshType &mesh)
            : base_name(name) {

        setPoints(mesh);
        setCells(mesh);
    }

    template<typename MeshType>
    void VtkUnstructuredWriter::setPoints(MeshType &mesh) {
        vtkSmartPointer<vtkPoints> points =
                vtkSmartPointer<vtkPoints>::New();
        for (int i = 0; i < mesh.numberOfNodes(); i++) {
            auto p = mesh.getNode(i);
            points->InsertNextPoint(p[0], p[1], p[2]);
        }
        vtk_grid->SetPoints(points);
    }

    template<typename MeshType>
    void VtkUnstructuredWriter::setCells(MeshType &mesh) {

        vtkSmartPointer<vtkCellArray> cells =
                vtkSmartPointer<vtkCellArray>::New();
        std::vector<int> cell_types;


        for (int i = 0; i < mesh.numberOfCells(); i++) {
            auto cell_nodes = mesh.getNodesInCell(i);
            int n = cell_nodes.size();
            if (4 == n) {
                vtkSmartPointer<vtkTetra> cell =
                        vtkSmartPointer<vtkTetra>::New();
                for (int j = 0; j < n; j++)
                    cell->GetPointIds()->SetId(j, cell_nodes[j]);
                cells->InsertNextCell(cell);
                cell_types.push_back(VTK_TETRA);
            }
            if (5 == n) {
                vtkSmartPointer<vtkPyramid> cell =
                        vtkSmartPointer<vtkPyramid>::New();
                for (int j = 0; j < n; j++)
                    cell->GetPointIds()->SetId(j, cell_nodes[j]);
                cells->InsertNextCell(cell);
                cell_types.push_back(VTK_PYRAMID);
            }
            if (6 == n) {
                vtkSmartPointer<vtkWedge> cell =
                        vtkSmartPointer<vtkWedge>::New();
                for (int j = 0; j < n; j++)
                    cell->GetPointIds()->SetId(j, cell_nodes[j]);
                cells->InsertNextCell(cell);
                cell_types.push_back(VTK_WEDGE);
            }
            if(8 == n) {
                vtkSmartPointer<vtkHexahedron> cell =
                        vtkSmartPointer<vtkHexahedron>::New();
                for(int j=0;j<8;j++)
                    cell->GetPointIds()->SetId(j,cell_nodes[j]);
                cells->InsertNextCell(cell);
                cell_types.push_back(VTK_HEXAHEDRON);
            }
        }
        vtk_grid->SetCells(cell_types.data(), cells);
    }

    template<typename T>
    void VtkUnstructuredWriter::addNodeData(std::string name,
                                            T *data, int number_of_components) {
        auto stuff = createVtkArrayPointer(data);
        stuff->SetNumberOfComponents(number_of_components);
        stuff->SetName(name.c_str());
        for (int i = 0; i < vtk_grid->GetNumberOfPoints(); i++) {
            for(int j=0;j<number_of_components;++j)
                stuff->InsertNextValue(data[i*number_of_components+j]);
        }
        vtk_grid->GetPointData()->AddArray(stuff);
    }

    template<typename T>
    void VtkUnstructuredWriter::addCellData(std::string name,
                                            T *data, int number_of_components) {
        auto stuff = createVtkArrayPointer(data);
        stuff->SetNumberOfComponents(number_of_components);
        stuff->SetName(name.c_str());
        for (int i = 0; i < vtk_grid->GetNumberOfCells(); i++)
            stuff->InsertNextValue(data[i]);
        vtk_grid->GetCellData()->AddArray(stuff);
    }

    inline void VtkUnstructuredWriter::writeBinary() {
        writeLocalFile(base_name+".vtu");
    }
  inline void VtkUnstructuredWriter::writeBinary(int rank,int nproc) {
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

  inline void VtkUnstructuredWriter::writeLocalFile(std::string filename){
      vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
          vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
      writer->SetFileName(filename.c_str());
      writer->SetInputData(vtk_grid);
      writer->Write();
  }
}