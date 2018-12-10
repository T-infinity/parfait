
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
#include <utility>

#pragma once
#include <parfait/Point.h>
#include <utility>
#include <string>
#include <functional>
#include <parfait/Throw.h>
#include <parfait/ByteSwap.h>
#include <parfait/CellWindingConverters.h>

namespace Parfait {
    namespace vtk {
        class Writer {
        public:
            enum CellType {NODE=1,EDGE=3,TRI=5,QUAD=9,TET=10,PYRAMID=14,PRISM=13,HEX=12};
            inline Writer(std::string filename, long num_points, long num_cells,
                   std::function<void(long, double*)> getPoint,
                   std::function<int(long)> getVTKCellType,
                   std::function<void(int, int*)> getCellNodes)
                    : filename(std::move(filename)), num_points(num_points), num_cells(num_cells),
                      getPoint(std::move(getPoint)),
                      getVTKCellType(std::move(getVTKCellType)),
                      getCellNodes(std::move(getCellNodes)),
                      fp(nullptr){
            }

            inline void addCellField(const std::string &name, std::function<double(int)> f){
                cell_fields[name] = std::move(f);
            }

            inline void addNodeField(const std::string& name, std::function<double(int)> f){
                node_fields[name] = std::move(f);
            }

            inline void visualize(){
                open();
                writePoints();
                writeCells();
                writeCellFields();
                writeNodeFields();
                close();
            }

            void writeNodeFields() {
                fprintf(fp, "\nPOINT_DATA %li\n", num_points);
                for(auto& pair : node_fields){
                    writeNodeField(pair.first, pair.second);
                }
            }

            void writeCellFields() {
                fprintf(fp, "\nCELL_DATA %li\n", num_cells);
                for(auto& pair : cell_fields){
                    writeCellField(pair.first, pair.second);
                }
            }

        private:
            std::string filename;
            long num_points;
            long num_cells;
            std::function<void(int, double*)> getPoint;
            std::function<int(int)> getVTKCellType;
            std::function<void(int, int*)> getCellNodes;
            std::map<long, int> global_to_local_cells;
            std::map<std::string, std::function<double(int)>> cell_fields;
            std::map<std::string, std::function<double(int)>> node_fields;
            FILE* fp;

            inline void open(){
                filename += ".vtk";
                fp = fopen(filename.c_str(), "w");
                fprintf(fp, "# vtk DataFile Version 3.0\n");
                fprintf(fp, "Extent boxes\n");
                fprintf(fp, "BINARY\n");
                fprintf(fp, "DATASET UNSTRUCTURED_GRID\n");
            }

            inline void writePoints(){
                fprintf(fp, "POINTS %ld double\n", num_points);
                Parfait::Point<double> p;
                for(long node = 0; node < num_points; node++){
                    getPoint(node, p.data());
                    bswap_64(&p[0]);
                    bswap_64(&p[1]);
                    bswap_64(&p[2]);
                    fwrite(p.data(), sizeof(double), 3, fp);
                }
            }

            size_t calcCellBufferSize() {
                size_t buffer_size = num_cells;
                for(long cell_id = 0; cell_id < num_cells; cell_id++){
                    int type = getVTKCellType(cell_id);
                    buffer_size += getNumPointsFromVTKType(type);
                }

                return buffer_size;
            }

            void rewindCellNodes(int type, std::vector<int> &cell_nodes) {
                switch(type){
                    case NODE: return;
                    case EDGE: return;
                    case TRI: return;
                    case QUAD: return;
                    case TET: Parfait::CGNSToVtk::convertTet(cell_nodes.data()); return;
                    case PYRAMID: Parfait::CGNSToVtk::convertPyramid(cell_nodes.data()); return;
                    case PRISM: Parfait::CGNSToVtk::convertPrism(cell_nodes.data()); return;
                    case HEX: Parfait::CGNSToVtk::convertHex(cell_nodes.data()); return;
                    default:
                        THROW("Element type is unknown.");
                }
            }

            inline void writeCells(){
                size_t cell_buffer_size = calcCellBufferSize();
                fprintf(fp, "\nCELLS %li %zu\n", num_cells, cell_buffer_size);

                int num_points = 8;
                int num_points_big_endian;
                std::vector<int> cell_nodes(num_points);
                for(long cell_id = 0; cell_id < num_cells; cell_id++){
                    int cell_type = getVTKCellType(cell_id);
                    num_points = getNumPointsFromVTKType(cell_type);
                    num_points_big_endian = num_points;
                    bswap_32(&num_points_big_endian);
                    fwrite(&num_points_big_endian, sizeof(num_points), 1, fp);
                    cell_nodes.resize(num_points);
                    getCellNodes(cell_id, cell_nodes.data());
                    rewindCellNodes(cell_type, cell_nodes);
                    cell_nodes.resize(num_points);
                    for(int local_node = 0; local_node < num_points; local_node++){
                        bswap_32(&cell_nodes[local_node]);
                    }
                    fwrite(cell_nodes.data(), sizeof(int), num_points, fp);
                }

                fprintf(fp, "\nCELL_TYPES %li\n", num_cells);
                for(int cell_id = 0; cell_id < num_cells; cell_id++){
                    int cell_type = getVTKCellType(cell_id);
                    bswap_32(&cell_type);
                    fwrite(&cell_type, sizeof(cell_type), 1, fp);
                }
            }

            inline int getNumPointsFromVTKType(int vtk_type){
                switch (vtk_type){
                    case  1: return 1; // node
                    case  3: return 2; // edge
                    case  5: return 3; // triangle
                    case  9: return 4; // quad
                    case 10: return 4; // tet
                    case 12: return 8; // hex
                    case 13: return 6; // prism
                    case 14: return 5; // pyramid
                    default:
                        THROW("Need to map additional number of nodes for vtk type: " + std::to_string(vtk_type));
                }
            }

            inline void writeCellField(std::string field_name, std::function<double(int)> field){
                fprintf(fp, "SCALARS %s DOUBLE 1\n", field_name.c_str());
                fprintf(fp, "LOOKUP_TABLE default\n");
                for(long cell_id = 0; cell_id < num_cells; cell_id++){
                    double f = field(cell_id);
                    bswap_64(&f);
                    fwrite(&f, sizeof(f), 1, fp);
                }
            }

            inline void writeNodeField(std::string field_name, std::function<double(int)> field){
                fprintf(fp, "SCALARS %s DOUBLE 1\n", field_name.c_str());
                fprintf(fp, "LOOKUP_TABLE DEFAULT\n");
                for(long node_id = 0; node_id < num_points; node_id++){
                    double f = field(node_id);
                    bswap_64(&f);
                    fwrite(&f, sizeof(f), 1, fp);
                }
            }

            inline void close() {
                if(fp){
                    fclose(fp);
                    fp = nullptr;
                }
            }
        };
    }
}
