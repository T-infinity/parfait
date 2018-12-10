
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
#include <math.h>
#include "Point.h"
#include <iostream>

namespace Parfait {


  template<typename PointCollection>
  auto calcGramSchmidtWeights(const PointCollection &points, const Parfait::Point<double> &center) {
      GSWeights w(0, 0, 0, 0, 0, 0);

      for (const auto &p : points) {
          Parfait::Point<double> dx = p - center;
          w.r11 += dx[0] * dx[0];
          w.r12 += dx[0] * dx[1];
          w.r13 += dx[0] * dx[2];
      }

      w.r11 = std::sqrt(w.r11);
      w.r12 = w.r12 / w.r11;
      w.r13 = w.r13 / w.r11;

      for (const auto &p : points) {
          Parfait::Point<double> dx = p - center;
          double l = dx[1] - dx[0] * w.r12 / w.r11;
          w.r22 += l * l;
          w.r23 += dx[2] * l;
      }
      w.r22 = std::sqrt(w.r22);
      w.r23 = w.r23 / w.r22;

      for (const auto &p : points) {
          Parfait::Point<double> dx = p - center;
          double l = dx[2] - dx[0] * w.r13 / w.r11 - w.r23 / w.r22 * (dx[1] - dx[0] * w.r12 / w.r11);
          w.r33 += l * l;
      }
      w.r33 = std::sqrt(w.r33);
      return w;
  }

  template<typename Point>
  auto calcLSCoeffs(const GSWeights &gs, const Point &dx) {
      double r11inv = 1.0 / gs.r11;
      double r22inv = 1.0 / gs.r22;
      double w11 = r11inv * r11inv;
      double w22 = r22inv * r22inv;
      double w33 = 1.0 / (gs.r33 * gs.r33);
      double r12r11 = gs.r12 * r11inv;
      double r13r11 = gs.r13 * r11inv;
      double r23r22 = gs.r23 * r22inv;
      double rmult = (r12r11 * r23r22 - r13r11) * w33;

      double xt1 = dx[0] * r12r11;
      double xt2 = dx[0] * r13r11;
      double xt3 = dx[0] * w11;

      double t1 = w22 * r12r11;
      double t2 = r23r22 * w33;

      double coef1 = dx[1] - xt1;
      double coef2 = dx[2] - xt2 - r23r22 * coef1;

      std::array<double, 3> c;
      c[0] = xt3 - t1 * coef1 + rmult * coef2;
      c[1] = w22 * coef1 - t2 * coef2;
      c[2] = w33 * coef2;
      return c;
  }

  template<typename PointCollection, typename Point>
  auto calcLeastSquaresGradient(const PointCollection &points, const Point &center, const GSWeights &gs,
                                const std::vector<double> &u, double u0) {

      Parfait::Point<double> grad(0, 0, 0);
      double dqq;
      for (int i = 0; i < points.size(); i++) {
          const auto &p = points[i];
          auto dx = p - center;
          dqq = u[i] - u0;

          auto c = calcLSCoeffs(gs, dx);

          grad[0] += c[0] * dqq;
          grad[1] += c[1] * dqq;
          grad[2] += c[2] * dqq;
      }
      return grad;
  }

  template<typename GetPoint>
  auto calcGramSchmidtWeightsOnStencil(const GetPoint& getEdge, int num_edges){
      GSWeights w(0,0,0,0,0,0);


      for(int i = 0; i < num_edges; i++){
          auto dx = getEdge(i);
          w.r11 += dx[0] * dx[0];
          w.r12 += dx[0] * dx[1];
          w.r13 += dx[0] * dx[2];
      }
      w.r11 = std::sqrt(w.r11);
      w.r12 = w.r12 / w.r11;
      w.r13 = w.r13 / w.r11;

      for(int i = 0; i < num_edges; i++){
          auto dx = getEdge(i);
          double l = dx[1] - dx[0] * w.r12 / w.r11;
          w.r22 += l * l;
          w.r23 += dx[2] * l;
      }
      w.r22 = std::sqrt(w.r22);
      w.r23 = w.r23 / w.r22;

      for(int i = 0; i < num_edges; i++){
          auto dx = getEdge(i);
          double l = dx[2] - dx[0] * w.r13 / w.r11 - w.r23 / w.r22 * (dx[1] - dx[0] * w.r12 / w.r11);
          w.r33 += l * l;
      }
      w.r33 = std::sqrt(w.r33);

      return w;
  }

  template <typename GetEdge>
  auto calcLeastSquaresWeightsOnStencil(const GetEdge& getEdge, int num_edges){
      auto w = calcGramSchmidtWeightsOnStencil(getEdge, num_edges);
      std::vector<std::array<double, 3>> ls_weights(num_edges, {0,0,0});
      for(int i = 0; i < num_edges; i++){
          ls_weights[i] = calcLSCoeffs(w, getEdge(i));
      }
      return ls_weights;
  }

}
