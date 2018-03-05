
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
#include <cmath>
#include "LeastSquaresGradient.h"

namespace Parfait {
    namespace Gradient {

        template<typename GetEdge, typename GetXyz>
        std::vector<GSWeights> calcGSWeights(GetEdge getEdge, int number_of_edges,
                                             GetXyz getXyz, int number_of_nodes) {

            std::vector<GSWeights> gs(number_of_nodes, {0, 0, 0, 0, 0, 0});
            for (int i = 0; i < number_of_edges; i++) {
                auto edge = getEdge(i);
                int node1 = edge[0];
                int node2 = edge[1];
                auto a1 = getXyz(node1);
                auto a2 = getXyz(node2);
                Parfait::Point<double> o{a1[0], a1[1], a1[2]};
                Parfait::Point<double> p{a2[0], a2[1], a2[2]};
                auto dx = p - o;
                gs[node1].r11 += dx[0] * dx[0];
                gs[node1].r12 += dx[0] * dx[1];
                gs[node1].r13 += dx[0] * dx[2];

                dx *= -1.0;
                gs[node2].r11 += dx[0] * dx[0];
                gs[node2].r12 += dx[0] * dx[1];
                gs[node2].r13 += dx[0] * dx[2];
            }

            for (int i = 0; i < number_of_nodes; i++) {
                auto &g = gs[i];
                gs[i].r11 = std::sqrt(gs[i].r11);
                if (fabs(g.r11) > 1.0e-20) {
                    gs[i].r12 = gs[i].r12 / gs[i].r11;
                    gs[i].r13 = gs[i].r13 / gs[i].r11;
                }
            }

            for (int i = 0; i < number_of_edges; i++) {
                auto edge = getEdge(i);
                int node1 = edge[0];
                int node2 = edge[1];
                auto a1 = getXyz(node1);
                auto a2 = getXyz(node2);
                Parfait::Point<double> o{a1[0], a1[1], a1[2]};
                Parfait::Point<double> p{a2[0], a2[1], a2[2]};
                auto dx = p - o;
                double l = 0;
                if (fabs(gs[node1].r11) > 1.0e-20) {
                    l = dx[1] - dx[0] * gs[node1].r12 / gs[node1].r11;
                }
                gs[node1].r22 += l * l;
                gs[node1].r23 += dx[2] * l;

                dx *= -1;
                l = 0;
                if (fabs(gs[node2].r11) > 1.0e-20) {
                    l = dx[1] - dx[0] * gs[node2].r12 / gs[node2].r11;
                }
                gs[node2].r22 += l * l;
                gs[node2].r23 += dx[2] * l;
            }

            for (int i = 0; i < number_of_nodes; i++) {
                gs[i].r22 = std::sqrt(gs[i].r22);
                if (fabs(gs[i].r22) > 1.0e-20) {
                    gs[i].r23 = gs[i].r23 / gs[i].r22;
                }
            }
            for (int i = 0; i < number_of_edges; i++) {
                auto edge = getEdge(i);
                int node1 = edge[0];
                int node2 = edge[1];
                auto a1 = getXyz(node1);
                auto a2 = getXyz(node2);
                Parfait::Point<double> o{a1[0], a1[1], a1[2]};
                Parfait::Point<double> p{a2[0], a2[1], a2[2]};
                auto dx = p - o;
                double l = 0;
                if (fabs(gs[node1].r11) > 1.0e-20 and fabs(gs[node1].r22) > 1.0e-20) {
                    l = dx[2] - dx[0] * gs[node1].r13 / gs[node1].r11
                        - gs[node1].r23 / gs[node1].r22 * (dx[1] - dx[0] * gs[node1].r12 / gs[node1].r11);
                }
                gs[node1].r33 += l * l;

                dx *= -1;
                l = 0;
                if (fabs(gs[node2].r11) > 1.0e-20 and fabs(gs[node2].r22) > 1.0e-20) {
                    l = dx[2] - dx[0] * gs[node2].r13 / gs[node2].r11
                        - gs[node2].r23 / gs[node2].r22 * (dx[1] - dx[0] * gs[node2].r12 / gs[node2].r11);
                }
                gs[node2].r33 += l * l;
            }
            for (int i = 0; i < number_of_nodes; i++) {
                gs[i].r33 = std::sqrt(gs[i].r33);
            }
            return gs;
        }

        template<typename T, typename GetEdge, typename GetXyz, typename GetVariable>
        std::vector<Parfait::Point<T>> calculateNodeGradientsFromEdges(
                GetEdge getEdge, int number_of_edges, GetXyz getXyz,
                int number_of_nodes, const std::vector<GSWeights> &gs, const GetVariable var) {

            std::vector<Parfait::Point<T>> grad(number_of_nodes, {0, 0, 0});

            for (int i = 0; i < number_of_edges; i++) {
                auto edge = getEdge(i);
                int node1 = edge[0];
                int node2 = edge[1];

                Parfait::Point<double> o{getXyz(node1)};
                Parfait::Point<double> p{getXyz(node2)};
                auto dx = p - o;
                auto dq = var(node2) - var(node1);
                auto &g = gs[node1];

                auto c = calcLSCoeffs(gs[node1], dx);
                grad[node1][0] += c[0] * dq;
                grad[node1][1] += c[1] * dq;
                grad[node1][2] += c[2] * dq;

                dx *= -1;
                c = calcLSCoeffs(gs[node2], dx);
                grad[node2][0] -= c[0] * dq;
                grad[node2][1] -= c[1] * dq;
                grad[node2][2] -= c[2] * dq;
            }
            return grad;
        }
    }
}
