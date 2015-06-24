
namespace Parfait {
    template<class MeshType>
    VtkSliceWriter<MeshType>::VtkSliceWriter(MeshType &mesh,
                                             const Extent<double> &domain_in,
                                             std::string vtkFilename)
            : slice(mesh, domain_in), domain(domain_in), filename(vtkFilename),
              writer(slice, filename) { }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::writeAscii() {
        writer.writeAscii();
    }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::addScalarField(bool isNodeData, std::string fieldName,
                                                  int *data) {

        if (isNodeData) {
            addNodeData_int(data, fieldName, 1);
        } else {
            addCellData_int(data, fieldName, 1);
        }
    }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::addScalarField(bool isNodeData, std::string fieldName,
                                                  float *data) {

        if (isNodeData) {
            addNodeData_float(data, fieldName, 1);
        } else {
            addCellData_float(data, fieldName, 1);
        }
    }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::addScalarField(bool isNodeData, std::string fieldName,
                                                  double *data) {

        if (isNodeData) {
            addNodeData_double(data, fieldName, 1);
        } else {
            addCellData_double(data, fieldName, 1);
        }
    }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::addNodeData_int(int *data, std::string fieldName,
                                                   int blockSize) {

        intFields.push_back(std::vector<int>(slice.numberOfNodes()));
        std::vector<int> &field = intFields[intFields.size() - 1];
        for (int globalId = 0; globalId < slice.globalToLocal_nodeId.size(); globalId++) {
            int localId = slice.globalToLocal_nodeId[globalId];
            if (localId == -1) continue;
            for (int i = 0; i < blockSize; i++) {
                field[localId * blockSize + i] = data[blockSize * globalId + i];
            }
        }
        writer.addVectorField(true, fieldName, blockSize, field.data());
    }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::addCellData_int(int *data, std::string fieldName,
                                                   int blockSize) {

        intFields.push_back(std::vector<int>(slice.numberOfCells()));
        std::vector<int> &field = intFields[intFields.size() - 1];
        for (int globalCellId = 0; globalCellId < slice.globalToLocal_cellId.size();
             globalCellId++) {

            int localCellId = slice.globalToLocal_cellId[globalCellId];
            if (localCellId != -1) {
                for (int i = 0; i < blockSize; i++) {
                    field[localCellId * blockSize + i] =
                            data[blockSize * globalCellId + i];
                }
            }
        }

        writer.addVectorField(false, fieldName, blockSize, field.data());
    }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::addNodeData_double(double *data, std::string fieldName,
                                                      int blockSize) {

        doubleFields.push_back(std::vector<double>(slice.numberOfNodes()));
        std::vector<double> &field = doubleFields[doubleFields.size() - 1];
        for (int globalId = 0; globalId < slice.globalToLocal_nodeId.size(); globalId++) {
            int localId = slice.globalToLocal_nodeId[globalId];
            if (localId == -1) continue;
            for (int i = 0; i < blockSize; i++) {
                field[localId * blockSize + i] = data[blockSize * globalId + i];
            }
        }
        writer.addVectorField(true, fieldName, blockSize, field.data());
    }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::addNodeData_float(float *data, std::string fieldName,
                                                     int blockSize) {

        floatFields.push_back(std::vector<float>(slice.numberOfNodes()));
        std::vector<float> &field = floatFields[floatFields.size() - 1];
        for (int globalId = 0; globalId < slice.globalToLocal_nodeId.size(); globalId++) {
            int localId = slice.globalToLocal_nodeId[globalId];
            if (localId == -1) continue;
            for (int i = 0; i < blockSize; i++) {
                field[localId * blockSize + i] = data[blockSize * globalId + i];
            }
        }
        writer.addVectorField(true, fieldName, blockSize, field.data());
    }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::addCellData_double(double *data,
                                                      std::string fieldName,
                                                      int blockSize) {

        doubleFields.push_back(std::vector<double>(slice.numberOfCells()));
        std::vector<double> &field = doubleFields[doubleFields.size() - 1];
        for (int globalCellId = 0; globalCellId < slice.globalToLocal_cellId.size();
             globalCellId++) {

            int localCellId = slice.globalToLocal_cellId[globalCellId];
            if (localCellId == -1) continue;
            for (int i = 0; i < blockSize; i++) {
                field[localCellId * blockSize + i] =
                        data[blockSize * globalCellId + i];
            }
        }

        writer.addVectorField(false, fieldName, blockSize, field.data());
    }

    template<class MeshType>
    void VtkSliceWriter<MeshType>::addCellData_float(float *data,
                                                     std::string fieldName,
                                                     int blockSize) {

        floatFields.push_back(std::vector<float>(slice.numberOfCells()));
        std::vector<float> &field = floatFields[floatFields.size() - 1];
        for (int globalCellId = 0; globalCellId < slice.globalToLocal_cellId.size();
             globalCellId++) {

            int localCellId = slice.globalToLocal_cellId[globalCellId];
            if (localCellId == -1) continue;
            for (int i = 0; i < blockSize; i++) {
                field[localCellId * blockSize + i] =
                        data[blockSize * globalCellId + i];
            }
        }

        writer.addVectorField(false, fieldName, blockSize, field.data());
    }
}
