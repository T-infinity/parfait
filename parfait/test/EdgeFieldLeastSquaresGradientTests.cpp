
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
#include <catch.hpp>
#include <parfait/EdgeFieldLeastSquaresGradient.h>

TEST_CASE("Edge collection least squares gradient of more than 1 variable"){
    std::vector<std::array<int, 2>> edges = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
    std::vector<Parfait::Point<double>> points = {{0,0,0},{1,0,0},{0,1,0},{0,0,1}};
    auto getXyz = [&](int i){return points[i];};
    auto getEdge = [&](int edge){return edges[edge];};

    std::vector<std::array<double, 2>> var(points.size());
    for(int i = 0; i < points.size(); i++){
        auto p = getXyz(i);
        var[i][0] = 5*p[0] + 1*p[1] + 3*p[2];
        var[i][1] = 5.5*p[0] + 1.1*p[1] + 3.3*p[2];
    }

    auto gs = Parfait::Gradient::calcGSWeights(getEdge, edges.size(), getXyz, points.size());

    auto getFirstVariable = [&](int node_id){return var[node_id][0];};
    auto getSecondVariable = [&](int node_id){return var[node_id][1];};

    auto grad_first = Parfait::Gradient::calculateNodeGradientsFromEdges<double>(getEdge, edges.size(), getXyz, points.size(),
                                                                                 gs, getFirstVariable);
    auto grad_second = Parfait::Gradient::calculateNodeGradientsFromEdges<double>(getEdge, edges.size(), getXyz, points.size(),
                                                                                  gs, getSecondVariable);

    REQUIRE(grad_first.size() == points.size());
    for(auto& g : grad_first) {
        REQUIRE(g[0] == Approx(5));
        REQUIRE(g[1] == Approx(1));
        REQUIRE(g[2] == Approx(3));
    }
    for(auto& g : grad_second){
        REQUIRE(g[0] == Approx(5.5));
        REQUIRE(g[1] == Approx(1.1));
        REQUIRE(g[2] == Approx(3.3));
    }
}

