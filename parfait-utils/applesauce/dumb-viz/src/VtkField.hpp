
namespace Parfait {
    template<typename T>
    VtkField<T>::VtkField(bool isNodeData_in, std::string fieldName_in,
                          int blockSize_in, int nBlocks_in, T *data_in)
            : nodeBased(isNodeData_in),
              fieldName(fieldName_in),
              blockSize(blockSize_in),
              nBlocks(nBlocks_in),
              data(data_in) {
        localIds.assign(nBlocks, 0);
        for (int i = 0; i < nBlocks; i++)
            localIds[i] = i;
    }

    template<typename T>
    VtkField<T>::VtkField(bool isNodeData_in, std::string fieldName_in,
                          int blockSize_in, int nBlocks_in, T *data_in,
                          std::vector<int> &localIds_in)
            : nodeBased(isNodeData_in),
              fieldName(fieldName_in),
              blockSize(blockSize_in),
              nBlocks(nBlocks_in),
              data(data_in),
              localIds(localIds_in) {
    }

    template<typename T>
    T VtkField<T>::getElement(int id) {
        return data[id];
    }

    template<typename T>
    const T *VtkField<T>::getBlock(int id) {
        return &data[blockSize * id];
    }

    template<typename T>
    std::string VtkField<T>::name() {
        return fieldName;
    }

    template<typename T>
    void VtkField<T>::append(std::string filename) {
        FILE *f = fopen(filename.c_str(), "a");
        assert(f != NULL);
        std::string typeName;
        int type = Type(T());
        if (INT32_FIELD == type)
            typeName = "Int32";
        else if (FLOAT32_FIELD == type)
            typeName = "Float32";
        else if (FLOAT64_FIELD == type)
            typeName = "Float64";
        else
            assert(false);
        fprintf(f,
                "			<DataArray NumberOfComponents=\"%i\" type=\"%s\" Name=\"%s\" format=\"ascii\">\n",
                blockSize, typeName.c_str(), fieldName.c_str());
        int count = 0;
        for (int i = 0; i < nBlocks; i++) {
            if (localIds[i] == -1 && nodeBased)
                continue;
            count++;
            for (int j = 0; j < blockSize; j++) {
                if (type == INT32_FIELD)
                    fprintf(f, "				%i\n", (int) getElement(i * blockSize + j));
                else if (type == FLOAT32_FIELD)
                    fprintf(f, "				%f\n", (double) getElement(i * blockSize + j));
                else if (type == FLOAT64_FIELD)
                    fprintf(f, "				%e\n", (double) getElement(i * blockSize + j));
                else
                    assert(false);
            }
        }
        fprintf(f, "			</DataArray>\n");
        fclose(f);
    }
}
