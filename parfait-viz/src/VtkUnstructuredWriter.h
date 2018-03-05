
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#pragma once


#include "VtkHacking.h"
#include <string>
#include <vector>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellArray.h>
#include <vtkTetra.h>
#include <vtkPyramid.h>
#include <vtkWedge.h>
#include <vtkHexahedron.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <stdio.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLPUnstructuredGridWriter.h>
namespace Parfait {
    class VtkUnstructuredWriter {
    public:
        template<typename MeshType>
        VtkUnstructuredWriter(std::string name, MeshType &mesh);

        void writeBinary();
        void writeBinary(int rank,int nproc);

        template<typename T>
        void addNodeData(std::string name, const T *data, int number_of_components);

        template<typename T>
        void addCellData(std::string name, const T *data, int number_of_components);

    private:
        std::string base_name;
        vtkSmartPointer<vtkUnstructuredGrid> vtk_grid =
                vtkSmartPointer<vtkUnstructuredGrid>::New();

        template<typename MeshType>
        void setPoints(MeshType &mesh);

        template<typename MeshType>
        void setCells(MeshType &mesh);

        vtkSmartPointer<vtkIntArray> createVtkArrayPointer(int *data) {
            return vtkSmartPointer<vtkIntArray>::New();
        }

        vtkSmartPointer<vtkFloatArray> createVtkArrayPointer(float *data) {
            return vtkSmartPointer<vtkFloatArray>::New();
        }

        vtkSmartPointer<vtkDoubleArray> createVtkArrayPointer(double *data) {
            return vtkSmartPointer<vtkDoubleArray>::New();
        }

        void writeLocalFile(std::string filename);
    };
}

#include "VtkUnstructuredWriter.hpp"

