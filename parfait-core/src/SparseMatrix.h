#pragma once

namespace Parfait {
  class SparseMatrix {
  public:
      SparseMatrix(SparseMatrix&&);
      SparseMatrix(SparseMatrix&) = delete;
      ~SparseMatrix();
      static SparseMatrix fromCompressedRowStorage(int num_rows, int num_non_zeroes, int *ia, int *ja);
      double& operator()(int row, int col);
  private:
      SparseMatrix(int num_rows, int num_non_zeroes);
      int num_rows;
      int num_non_zeroes;
      int *ia = nullptr;
      int *ja = nullptr;
      double *data = nullptr;
  };
}

#include "SparseMatrix.hpp"