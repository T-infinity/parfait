#ifndef PARALLEL_VTK_H
#define PARALLEL_VTK_H
#include <vector>
#include <string>
#include <stdlib.h>
#include <assert.h>
#include "VtkField.h"
#include "MessagePasser.h"
#include "GenericMesh.h"

namespace Parfait {
    template<class MeshType>
    class VtkWriter {
    public:
        VtkWriter(MeshType &meshInterface, std::string vtkFilename);
        void writeAscii();
        void writeFile();
        void addScalarField(bool isNodeData, std::string fieldName, int *data);
        void addScalarField(bool isNodeData, std::string fieldName, float *data);
        void addScalarField(bool isNodeData, std::string fieldName, double *data);
        void addVectorField(bool isNodeData, std::string fieldName, int blockSize, int *data);
        void addVectorField(bool isNodeData, std::string fieldName, int blockSize, float *data);
        void addVectorField(bool isNodeData, std::string fieldName, int blockSize, double *data);
    private:
        Mesh<MeshType> mesh;
        std::vector<VtkField<int> > intFields;
        std::vector<VtkField<float> > floatFields;
        std::vector<VtkField<double> > doubleFields;
        int global_nnodes;
        int global_ncells;
        std::string filename;

        void writeHeader();
        void writeFooter();
        void writePiece();
        void writePoints();
        void writeCellConnectivity();
        void writeCellOffsets();
        void writeCellTypes();
        void writeFaces();
        void writeFaceOffsets();
        void writeFieldData();
    };
}
#include "VtkGenericWriter.hpp"

#endif
