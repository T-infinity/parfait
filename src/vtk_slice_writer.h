#ifndef VTK_SLICE_WRITER_H
#define VTK_SLICE_WRITER_H

#include <vector>
#include <string>
#include <stdexcept>

#include "vtk_field.h"
#include "vtk_generic_writer.h"
#include "extent.h"
#include "mesh_slice.h"

using std::string;
using std::vector;
namespace Parfait {
    template<class MeshType>
    class VtkSliceWriter {
    public:
        VtkSliceWriter(MeshType &mesh, const Extent<double> &domain, string vtkFilename);
        void writeAscii();
        void addScalarField(bool isNodeData, string fieldName, int *data);
        void addScalarField(bool isNodeData, string fieldName, float *data);
        void addScalarField(bool isNodeData, string fieldName, double *data);
        void addVectorField(bool isNodeData, string fieldName, int blockSize,
                            int *data);
        void addVectorField(bool isNodeData, string fieldName, int blockSize,
                            float *data);
        void addVectorField(bool isNodeData, string fieldName, int blockSize,
                            double *data);

    private:
        MeshSlice<MeshType> slice;
        Extent<double> domain;
        vector<vector<int>> intFields;
        vector<vector<float>> floatFields;
        vector<vector<double>> doubleFields;

        std::string filename;
        VtkWriter<MeshSlice<MeshType>> writer;
        void addNodeData_int(int *data, string fieldName, int blocksize);
        void addCellData_int(int *data, string fieldName, int blocksize);

        void addNodeData_float(float *data, string fieldName, int blocksize);
        void addCellData_float(float *data, string fieldName, int blocksize);

        void addNodeData_double(double *data, string fieldName, int blocksize);
        void addCellData_double(double *data, string fieldName, int blocksize);
    };
}

#include "vtk_slice_writer.hpp"

#endif
