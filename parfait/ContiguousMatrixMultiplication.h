
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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

namespace Parfait {

// A*v stored in x
    void MatrixVectorMultiply(const double *A, const double *v, double *x, int nrows, int ncols);

// A * B stored in O
    void
    MatrixMatrixMultiply(const double *A, const double *B, int anrows, int ancols, int bnrows, int bncols, double *O);

    inline void MatrixVectorMultiply(const double *A, const double *v, double *x, int nrows, int ncols) {

      for (int i = 0; i < nrows; i++) {
        x[i] = 0.0;
      }

      for (int row = 0; row < nrows; row++)
        for (int col = 0; col < ncols; col++)
          x[row] += A[row * ncols + col] * v[col];
    }

    inline void
    MatrixMatrixMultiply(const double *A, const double *B, int anrows, int ancols, int bnrows, int bncols, double *O) {

      assert(ancols == bnrows);
      for (int i = 0; i < anrows * bncols; i++)
        O[i] = 0.0;

      for (int arow = 0; arow < anrows; arow++)
        for (int acol = 0; acol < bncols; acol++)
          for (int brow = 0; brow < bnrows; brow++)
            O[arow * bncols + acol] += A[arow * ancols + brow] * B[brow * bncols + acol];
    }
}