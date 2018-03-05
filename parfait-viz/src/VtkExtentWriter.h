
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

#ifdef PARFAIT_WITH_VTK
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkHexahedron.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <stdio.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLPUnstructuredGridWriter.h>


static inline std::array<Parfait::Point<double>, 8> getExtentPoints(const Parfait::Extent<double> &e) {
    std::array<Parfait::Point<double>, 8> points;

    points[0] = {e.lo[0], e.lo[1], e.lo[2]};
    points[1] = {e.hi[0], e.lo[1], e.lo[2]};
    points[2] = {e.hi[0], e.hi[1], e.lo[2]};
    points[3] = {e.lo[0], e.hi[1], e.lo[2]};

    points[4] = {e.lo[0], e.lo[1], e.hi[2]};
    points[5] = {e.hi[0], e.lo[1], e.hi[2]};
    points[6] = {e.hi[0], e.hi[1], e.hi[2]};
    points[7] = {e.lo[0], e.hi[1], e.hi[2]};

    return points;
}

namespace Parfait {
    class VtkExtentWriter {
        public:
            template <typename T> 
                void addExtent(const Parfait::Extent<T>& e) {
                    Parfait::Extent<double> d({e.lo[0],e.lo[1],e.lo[2]},
                            {e.hi[0], e.hi[1], e.hi[2]});
                    extents.push_back(d);
                }

            void writeBinary(std::string filename){
                filename = filename + ".vtu";
                setPoints();
                setCells();
                vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
                    vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
                writer->SetFileName(filename.c_str());
                writer->SetInputData(vtk_grid);
                writer->Write();
            }
        private:
            std::string base_name;
            vtkSmartPointer<vtkUnstructuredGrid> vtk_grid =
                vtkSmartPointer<vtkUnstructuredGrid>::New();
            std::vector<Parfait::Extent<double>> extents;

            void setCells() {
                vtkSmartPointer<vtkCellArray> cells =
                    vtkSmartPointer<vtkCellArray>::New();
                std::vector<int> cell_types;

                for(int i = 0; i < extents.size(); i++){
                    std::vector<int> cell_nodes = {
                        8*i+0, 8*i+1, 8*i+2, 8*i+3,
                        8*i+4, 8*i+5, 8*i+6, 8*i+7,
                    };
                    vtkSmartPointer<vtkHexahedron> cell =
                        vtkSmartPointer<vtkHexahedron>::New();
                    for(int j=0;j<8;j++)
                        cell->GetPointIds()->SetId(j,cell_nodes[j]);
                    cells->InsertNextCell(cell);
                    cell_types.push_back(VTK_HEXAHEDRON);
                }
                vtk_grid->SetCells(cell_types.data(), cells);
            }

            void setPoints() {
                vtkSmartPointer<vtkPoints> points =
                    vtkSmartPointer<vtkPoints>::New();

                for(int i = 0; i < extents.size(); i++){
                    auto e_points = getExtentPoints(extents[i]);
                    for(int j = 0; j < 8; j++){
                        auto p = e_points[j];
                        points->InsertNextPoint(p[0], p[1], p[2]);
                    }
                }
                vtk_grid->SetPoints(points);
            }

    };
}
#endif
