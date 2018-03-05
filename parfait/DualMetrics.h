
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
#include <tuple>
#include "CGNSElements.h"
#include "Metrics.h"

namespace Parfait {

class DualMetrics {
public:
    explicit DualMetrics(int number_of_nodes, int number_of_edges)
        : node_volumes(number_of_nodes, 0), edge_areas(number_of_edges, {0, 0, 0}) {}
    std::vector<double> node_volumes;
    std::vector<Parfait::Point<double>> edge_areas;
};

template<typename Cell, typename C1, typename C2, typename C3>
// I'm sorry for terrible type names.
std::tuple<std::vector<double>, std::vector<Parfait::Point<double>>> calcDualMetrics(const Cell &cell, int num_nodes,
                                                                                     const C1 &edge_to_node,
                                                                                     const C1 &edge_to_face,
                                                                                     const C2 &edge_centers,
                                                                                     const C3 &face_centers,
                                                                                     const Parfait::Point<double> &cell_center) {

    int num_edges = edge_to_node.size();
    std::vector<double> node_volumes(num_nodes, 0);
    std::vector<Parfait::Point<double>> edge_areas(num_edges, {0, 0, 0});
    for (int edge = 0; edge < edge_to_node.size(); edge++) {

        auto lnode = edge_to_node[edge][0];
        auto hnode = edge_to_node[edge][1];
        auto lface = edge_to_face[edge][0];
        auto rface = edge_to_face[edge][1];
        node_volumes[lnode] += Metrics::computeTetVolume(cell[lnode], edge_centers[edge], face_centers[lface], cell_center);
        node_volumes[lnode] += Metrics::computeTetVolume(cell[lnode], face_centers[rface], edge_centers[edge], cell_center);

        node_volumes[hnode] += Metrics::computeTetVolume(edge_centers[edge], cell[hnode], face_centers[lface], cell_center);
        node_volumes[hnode] += Metrics::computeTetVolume(face_centers[rface], cell[hnode], edge_centers[edge], cell_center);

        edge_areas[edge] += Metrics::computeTriangleArea(edge_centers[edge], face_centers[lface], cell_center);
        edge_areas[edge] += Metrics::computeTriangleArea(face_centers[rface], edge_centers[edge], cell_center);
    }
    return std::make_tuple(node_volumes, edge_areas);
}

inline int findEdge(const std::vector<std::array<int, 2>> &edges, int e1, int e2) {
    if (e2 < e1) std::swap(e1, e2);
    auto iter = std::lower_bound(edges.begin(), edges.end(), std::array < int, 2 > {e1, e2});
    if (iter != edges.end())
        return std::distance(edges.begin(), iter);
    else
        throw std::logic_error("Could not find edge " + std::to_string(e1) + " " + std::to_string(e2));
}

template<typename GetTet, typename GetXyz>
void addTetDualMetrics(GetTet getTet, GetXyz getXyz, int numberOfCells, const std::vector<std::array<int, 2>> &edges,
                       DualMetrics &metrics) {
    using namespace Parfait::CGNS::Tet;
    int num_nodes = 4;
    int num_edges = 6;

    std::vector<Parfait::Point<double>> cell_xyz(num_nodes);
    for (int t = 0; t < numberOfCells; t++) {
        auto cell_nodes = getTet(t);
        for (int i = 0; i < num_nodes; i++)
            cell_xyz[i] = getXyz(cell_nodes[i]);
        std::vector<double> volumes;
        std::vector<Parfait::Point<double>> areas;

        tie(volumes, areas) = calcDualMetrics(cell_xyz, num_nodes, edge_to_node, edge_to_face,
                                              computeEdgeCenters(cell_xyz),
                                              computeFaceCenters(cell_xyz),
                                              computeCenter(cell_xyz));
        for (int i = 0; i < num_nodes; i++)
            metrics.node_volumes[cell_nodes[i]] += volumes[i];
        for (int i = 0; i < num_edges; i++) {
            int l_cell_node = edge_to_node[i][0];
            int r_cell_node = edge_to_node[i][1];
            int lnode = cell_nodes[l_cell_node];
            int rnode = cell_nodes[r_cell_node];
            auto a = areas[i];
            if (lnode > rnode)
                a *= -1.0;
            auto edgeId = findEdge(edges, lnode, rnode);
            metrics.edge_areas[edgeId] += a;
        }
    }
}

template<typename GetCell, typename GetXyz>
void addPyramidDualMetrics(GetCell getCell,
                           GetXyz getXyz,
                           int numberOfCells,
                           const std::vector<std::array<int, 2>> &edges,
                           DualMetrics &metrics) {
    using namespace Parfait::CGNS::Pyramid;
    int num_nodes = 5;
    int num_edges = 8;
    std::vector<Parfait::Point<double>> cell_xyz(num_nodes);
    for (int t = 0; t < numberOfCells; t++) {

        auto cell_nodes = getCell(t);
        for (int i = 0; i < num_nodes; i++)
            cell_xyz[i] = getXyz(cell_nodes[i]);
        std::vector<double> volumes;
        std::vector<Parfait::Point<double>> areas;

        tie(volumes, areas) = calcDualMetrics(cell_xyz, num_nodes, edge_to_node, edge_to_face,
                                              computeEdgeCenters(cell_xyz),
                                              computeFaceCenters(cell_xyz),
                                              computeCenter(cell_xyz));
        for (int i = 0; i < num_nodes; i++)
            metrics.node_volumes[cell_nodes[i]] += volumes[i];
        for (int i = 0; i < num_edges; i++) {
            int l_cell_node = edge_to_node[i][0];
            int r_cell_node = edge_to_node[i][1];
            int lnode = cell_nodes[l_cell_node];
            int rnode = cell_nodes[r_cell_node];
            auto edgeId = findEdge(edges, lnode, rnode);
            auto a = areas[i];
            if (lnode > rnode)
                a *= -1.0;
            metrics.edge_areas[edgeId] += a;
        }
    }
}

template<typename GetCell, typename GetXyz>
void addPrismDualMetrics(GetCell getCell,
                         GetXyz getXyz,
                         int numberOfCells,
                         const std::vector<std::array<int, 2>> &edges,
                         DualMetrics &metrics) {

    using namespace Parfait::CGNS::Prism;
    int num_nodes = 6;
    int num_edges = 9;

    std::vector<Parfait::Point<double>> cell_xyz(num_nodes);
    for (int t = 0; t < numberOfCells; t++) {

        auto cell_nodes = getCell(t);
        for (int i = 0; i < num_nodes; i++)
            cell_xyz[i] = getXyz(cell_nodes[i]);
        std::vector<double> volumes;
        std::vector<Parfait::Point<double>> areas;

        tie(volumes, areas) = calcDualMetrics(cell_xyz, num_nodes, edge_to_node, edge_to_face,
                                              computeEdgeCenters(cell_xyz),
                                              computeFaceCenters(cell_xyz),
                                              computeCenter(cell_xyz));
        for (int i = 0; i < num_nodes; i++)
            metrics.node_volumes[cell_nodes[i]] += volumes[i];
        for (int i = 0; i < num_edges; i++) {
            int l_cell_node = edge_to_node[i][0];
            int r_cell_node = edge_to_node[i][1];
            int lnode = cell_nodes[l_cell_node];
            int rnode = cell_nodes[r_cell_node];
            auto edgeId = findEdge(edges, lnode, rnode);
            auto a = areas[i];
            if (lnode > rnode)
                a *= -1.0;
            metrics.edge_areas[edgeId] += a;
        }
    }
}

template<typename GetCell, typename GetXyz>
void addHexDualMetrics(GetCell getCell, GetXyz getXyz, int numberOfCells, const std::vector<std::array<int, 2>> &edges,
                       DualMetrics &metrics) {

    using namespace Parfait::CGNS::Hex;
    int num_nodes = 8;
    int num_edges = 12;

    std::vector<Parfait::Point<double>> cell_xyz(num_nodes);
    for (int t = 0; t < numberOfCells; t++) {

        auto cell_nodes = getCell(t);
        for (int i = 0; i < num_nodes; i++)
            cell_xyz[i] = getXyz(cell_nodes[i]);
        std::vector<double> volumes;
        std::vector<Parfait::Point<double>> areas;

        tie(volumes, areas) = calcDualMetrics(cell_xyz, num_nodes, edge_to_node, edge_to_face,
                                              computeEdgeCenters(cell_xyz),
                                              computeFaceCenters(cell_xyz),
                                              computeCenter(cell_xyz));
        for (int i = 0; i < num_nodes; i++)
            metrics.node_volumes[cell_nodes[i]] += volumes[i];
        for (int i = 0; i < num_edges; i++) {
            int l_cell_node = edge_to_node[i][0];
            int r_cell_node = edge_to_node[i][1];
            int lnode = cell_nodes[l_cell_node];
            int rnode = cell_nodes[r_cell_node];
            auto edgeId = findEdge(edges, lnode, rnode);
            auto a = areas[i];
            if (lnode > rnode)
                a *= -1.0;
            metrics.edge_areas[edgeId] += a;
        }
    }
}

template<typename Cell>
std::vector<Parfait::Point<double>> calculateTetAreas(const Cell &cell) {
    std::vector<Parfait::Point<double>> areas(4);
    std::vector<double> volumes;
    int num_nodes = 4;
    using namespace Parfait::CGNS::Tet;
    tie(volumes, areas) = calcDualMetrics(cell, num_nodes, edge_to_node, edge_to_face,
                                          computeEdgeCenters(cell),
                                          computeFaceCenters(cell),
                                          computeCenter(cell));
    return areas;
}

template<typename Cell>
std::vector<Parfait::Point<double>> calculatePrismAreas(const Cell &cell) {
    std::vector<Parfait::Point<double>> areas(9);
    std::vector<double> volumes;
    int num_nodes = 6;
    using namespace Parfait::CGNS::Prism;
    tie(volumes, areas) = calcDualMetrics(cell, num_nodes, edge_to_node, edge_to_face,
                                          computeEdgeCenters(cell),
                                          computeFaceCenters(cell),
                                          computeCenter(cell));
    return areas;
}

template<typename ParallelMesh>
DualMetrics calcMetrics(const ParallelMesh &mesh,
                        const std::vector<std::array<int, 2>> &edges) {
    DualMetrics metrics(mesh.numberOfNodes(), edges.size());
    auto getTet = [&](int i) { return mesh.getTet(i); };
    auto getPyramid = [&](int i) { return mesh.getPyramid(i); };
    auto getPrism = [&](int i) { return mesh.getPrism(i); };
    auto getHex = [&](int i) { return mesh.getHex(i); };
    auto getXyz = [&](int i) { return mesh.getXyz(i); };

    addTetDualMetrics(getTet, getXyz, mesh.numberOfTets(), edges, metrics);
    addPyramidDualMetrics(getPyramid, getXyz, mesh.numberOfPyramids(), edges, metrics);
    addPrismDualMetrics(getPrism, getXyz, mesh.numberOfPrisms(), edges, metrics);
    addHexDualMetrics(getHex, getXyz, mesh.numberOfHexes(), edges, metrics);
    return metrics;
}

}