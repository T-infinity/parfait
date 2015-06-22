#ifndef VTK_SLICE_WRITER_H
#define VTK_SLICE_WRITER_H

#include <vector>
#include <string>
#include <stdexcept>

#include "vtk_field.h"
#include "vtk_generic_writer.h"
#include "extent.h"
#include "mesh_slice.h"

namespace Parfait {
    template<class MeshType>
    class VtkSliceWriter {
    public:
        VtkSliceWriter(MeshType &mesh, const Extent<double> &domain, std::string vtkFilename);
        void writeAscii();
        void addScalarField(bool isNodeData, std::string fieldName, int *data);
        void addScalarField(bool isNodeData, std::string fieldName, float *data);
        void addScalarField(bool isNodeData, std::string fieldName, double *data);
        void addVectorField(bool isNodeData, std::string fieldName, int blockSize,
                            int *data);
        void addVectorField(bool isNodeData, std::string fieldName, int blockSize,
                            float *data);
        void addVectorField(bool isNodeData, std::string fieldName, int blockSize,
                            double *data);

    private:
        MeshSlice<MeshType> slice;
        Extent<double> domain;
        std::vector<std::vector<int>> intFields;
        std::vector<std::vector<float>> floatFields;
        std::vector<std::vector<double>> doubleFields;

        std::string filename;
        VtkWriter<MeshSlice<MeshType>> writer;
        void addNodeData_int(int *data, std::string fieldName, int blocksize);
        void addCellData_int(int *data, std::string fieldName, int blocksize);

        void addNodeData_float(float *data, std::string fieldName, int blocksize);
        void addCellData_float(float *data, std::string fieldName, int blocksize);

        void addNodeData_double(double *data, std::string fieldName, int blocksize);
        void addCellData_double(double *data, std::string fieldName, int blocksize);
    };
}

#include "vtk_slice_writer.hpp"

#endif
