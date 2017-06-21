namespace Parfait {
  template<typename Mesh>
  VtkSurfaceWriter<Mesh>::VtkSurfaceWriter(Mesh &mesh_in, std::string name_in)
          : mesh(mesh_in),
            filename(name_in) {
      filename.append(".vtu");
      setUpLocalLists();
  }

  template<class Mesh>
  void VtkSurfaceWriter<Mesh>::addScalarField(bool isNodeData, std::string fieldName, int *data) {
      int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
      intFields.push_back(VtkField<int>(isNodeData, fieldName, 1, n, data, boundaryNodeIdMap));
  }

  template<class Mesh>
  void VtkSurfaceWriter<Mesh>::addScalarField(bool isNodeData, std::string fieldName, float *data) {
      int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
      floatFields.push_back(
              VtkField<float>(isNodeData, fieldName, 1, n, data, boundaryNodeIdMap));
  }

  template<class Mesh>
  void VtkSurfaceWriter<Mesh>::addScalarField(bool isNodeData, std::string fieldName, double *data) {
      int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
      doubleFields.push_back(
              VtkField<double>(isNodeData, fieldName, 1, n, data, boundaryNodeIdMap));
  }

  template<class Mesh>
  void VtkSurfaceWriter<Mesh>::addVectorField(bool isNodeData, std::string fieldName, int blockSize,
                                              int *data) {
      int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
      intFields.push_back(
              VtkField<int>(isNodeData, fieldName, blockSize, n, data, boundaryNodeIdMap));
  }

  template<class Mesh>
  void VtkSurfaceWriter<Mesh>::addVectorField(bool isNodeData, std::string fieldName, int blockSize,
                                              float *data) {
      int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
      floatFields.push_back(
              VtkField<float>(isNodeData, fieldName, blockSize, n, data, boundaryNodeIdMap));
  }

  template<class Mesh>
  void VtkSurfaceWriter<Mesh>::addVectorField(bool isNodeData, std::string fieldName, int blockSize,
                                              double *data) {
      int n = isNodeData ? mesh.numberOfNodes() : mesh.numberOfBoundaryFaces();
      doubleFields.push_back(
              VtkField<double>(isNodeData, fieldName, blockSize, n, data, boundaryNodeIdMap));
  }


//// --------------------------write vtk -------------------------------------------
  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::writeAscii() {
          writeHeader();
          writePiece();
          writeFooter();
  }

  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::setUpLocalLists() {
      int nfaces = mesh.numberOfBoundaryFaces();
      std::vector<bool> isBoundNode(mesh.numberOfNodes(), false);
      for(int i = 0; i < nfaces; i++) {
          auto face = mesh.getNodesInBoundaryFace(i);
          for (unsigned int j = 0; j < face.size(); j++) {
              isBoundNode[face[j]] = true;
          }
      }
      numberOfSurfaceNodes = 0;
      for (int i = 0; i < mesh.numberOfNodes(); i++)
          if (isBoundNode[i])
              numberOfSurfaceNodes++;
      boundaryNodeIdMap.assign(mesh.numberOfNodes(), -1);
      int count = 0;
      for (int i = 0; i < mesh.numberOfNodes(); i++)
          if (isBoundNode[i])
              boundaryNodeIdMap[i] = count++;
  }
  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::writeHeader() {
      FILE *f = fopen(filename.c_str(), "w");
      assert(f != NULL);
      fprintf(f, "<?xml version=\"1.0\"?>\n");
      fprintf(f,
              "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n");
      fprintf(f, "   <UnstructuredGrid>\n");
      fclose(f);
  }

  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::writeFooter() {
      FILE *f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      fprintf(f, "   </UnstructuredGrid>\n");
      fprintf(f, "</VTKFile>\n");
      fclose(f);
  }

  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::writePiece() {
      FILE *f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      fprintf(f, "<Piece NumberOfPoints=\"%i\" NumberOfCells=\"%i\">\n",
              numberOfSurfaceNodes, mesh.numberOfBoundaryFaces());
      fclose(f);
      writeFieldData();
      writeNodes();
      f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      fprintf(f, "  <Cells>\n");
      fclose(f);
      writeCellConnectivity();
      writeCellOffsets();
      writeCellTypes();
      f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      fprintf(f, "  </Cells>\n");
      fprintf(f, "</Piece>\n");
      fflush(f);
      fclose(f);
  }

//// ---------------------------------------write field data------------------------------
  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::writeFieldData() {
      FILE *f = fopen(filename.c_str(), "a");
      assert(f != NULL);

      fprintf(f, "	<CellData>\n");
      fclose(f);
      // append cell based integer data
      for (auto &field:intFields)
          if (!field.isNodeData()) {
              field.append(filename);
          }
      // append cell based float data
      for (auto &field:floatFields)
          if (!field.isNodeData())
              field.append(filename);
      // append cell based double data
      for (auto &field:doubleFields)
          if (!field.isNodeData())
              field.append(filename);
      f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      fprintf(f, "	</CellData>\n");
      fprintf(f, " <PointData>\n");
      fclose(f);
      // append node based integer data
      for (auto &field:intFields)
          if (field.isNodeData())
              field.append(filename);
      // append node based float data
      for (auto &field:floatFields)
          if (field.isNodeData())
              field.append(filename);
      // append node based double data
      for (auto &field:doubleFields)
          if (field.isNodeData())
              field.append(filename);
      f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      fprintf(f, "	</PointData>\n");
      fclose(f);
  }


//// --------------------------write points -------------------------------------------
  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::writeNodes() {
      FILE *f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      fprintf(f, "  <Points>\n");
      fprintf(f, "    <DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n");
      int count = 0;
      for(int i = 0; i < mesh.numberOfNodes(); i++){
          if (boundaryNodeIdMap[i] == -1)
              continue;
          double p[3];
          mesh.getNode(i, p);
          fprintf(f, "            %lf %lf %lf\n", p[0], p[1], p[2]);
          count++;
      }
      assert(count == numberOfSurfaceNodes);
      fprintf(f, "    </DataArray>\n");
      fprintf(f, "  </Points>\n");
      fclose(f);
  }

  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::writeCellOffsets() {
      FILE *f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      fprintf(f, "     <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n");
      int offset = 0;
      for (int i = 0; i < mesh.numberOfBoundaryFaces(); i++) {
          offset += mesh.numberOfNodesInBoundaryFace(i);
          fprintf(f, "          %i\n", offset);
      }
      fprintf(f, "     </DataArray>\n");
      fclose(f);
  }

  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::writeCellTypes() {
      FILE *f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      // set cell type VTK_POLYGON (=7)
      fprintf(f, "     <DataArray type=\"Int32\" Name=\"types\" format=\"ascii\">\n");
      for (int i=0;i<mesh.numberOfBoundaryFaces();i++)
          fprintf(f, "          7\n");
      fprintf(f, "     </DataArray>\n");
      fclose(f);
  }


/// ---------------------------------write faces-----------------------------------------
  template<typename Mesh>
  void VtkSurfaceWriter<Mesh>::writeCellConnectivity() {
      FILE *f = fopen(filename.c_str(), "a");
      assert(f != NULL);
      fprintf(f, "     <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n");
      for (int i=0;i<mesh.numberOfBoundaryFaces();i++) {
          auto face = mesh.getNodesInBoundaryFace(i);
          fprintf(f, "					");
          for (unsigned   int j = 0; j < face.size(); j++) {
              fprintf(f, "%i ", boundaryNodeIdMap[face[j]]);
          }
          // newline after each cell
          fprintf(f, "\n");
      }

      fprintf(f, "     </DataArray>\n");
      fclose(f);
  }
}
