
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
#include <vector>
#include "ParallelMesh.h"
#include "Metrics.h"

namespace Parfait {
struct BoundaryGhostEdge {
public:
    int boundary_tag;
    int real_node;
    Parfait::Point<double> area;
};

inline std::vector<BoundaryGhostEdge> buildBoundaryEdges(const Parfait::ParallelMesh& mesh) {
    std::vector<BoundaryGhostEdge> output;
    for (int t = 0; t < mesh.numberOfTriangles(); t++) {
        auto tri = mesh.getTriangle(t);
        auto a = mesh.getXyz(tri[0]);
        auto b = mesh.getXyz(tri[1]);
        auto c = mesh.getXyz(tri[2]);
        auto center = Metrics::average({a, b, c});
        auto e1 = 0.5 * (a + b);
        auto e2 = 0.5 * (b + c);
        auto e3 = 0.5 * (a + c);
        {
            auto area = Metrics::computeTriangleArea(a, e1, center);
            area += Metrics::computeTriangleArea(center, e3, a);
            output.push_back(BoundaryGhostEdge{mesh.getTriangleTag(t), tri[0], area});
        }
        {
            auto area = Metrics::computeTriangleArea(b, e2, center);
            area += Metrics::computeTriangleArea(center, e1, b);
            output.push_back(BoundaryGhostEdge{mesh.getTriangleTag(t), tri[1], area});
        }
        {
            auto area = Metrics::computeTriangleArea(c, e3, center);
            area += Metrics::computeTriangleArea(center, e2, c);
            output.push_back(BoundaryGhostEdge{mesh.getTriangleTag(t), tri[2], area});
        }
    }
    for (int q = 0; q < mesh.numberOfQuads(); q++) {
        auto quad = mesh.getQuad(q);
        auto a = mesh.getXyz(quad[0]);
        auto b = mesh.getXyz(quad[1]);
        auto c = mesh.getXyz(quad[2]);
        auto d = mesh.getXyz(quad[3]);
        auto center = Metrics::average({a, b, c, d});
        auto e1 = 0.5 * (a + b);
        auto e2 = 0.5 * (b + c);
        auto e3 = 0.5 * (c + d);
        auto e4 = 0.5 * (d + a);
        {
            auto area = Metrics::computeTriangleArea(a, e1, center);
            area += Metrics::computeTriangleArea(center, e4, a);
            output.push_back(BoundaryGhostEdge{mesh.getQuadTag(q), quad[0], area});
        }
        {
            auto area = Metrics::computeTriangleArea(b, e2, center);
            area += Metrics::computeTriangleArea(center, e1, b);
            output.push_back(BoundaryGhostEdge{mesh.getQuadTag(q), quad[1], area});
        }
        {
            auto area = Metrics::computeTriangleArea(c, e3, center);
            area += Metrics::computeTriangleArea(center, e2, c);
            output.push_back(BoundaryGhostEdge{mesh.getQuadTag(q), quad[2], area});
        }
        {
            auto area = Metrics::computeTriangleArea(d, e4, center);
            area += Metrics::computeTriangleArea(center, e3, d);
            output.push_back(BoundaryGhostEdge{mesh.getQuadTag(q), quad[3], area});
        }
    }
    return output;
}
}
