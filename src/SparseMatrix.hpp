#pragma once
#include <cstring>
#include <cassert>

namespace Parfait {

  inline SparseMatrix::SparseMatrix(SparseMatrix &&m)
          : num_rows(m.num_rows),
            num_non_zeroes(m.num_non_zeroes){
      ia = m.ia;
      ja = m.ja;
      data = m.data;
      m.ia = nullptr;
      m.ja = nullptr;
      m.data = nullptr;
  }
  inline SparseMatrix::~SparseMatrix() {
      if(data != nullptr)
          free(data);
      if(ia != nullptr)
          free(ia);
      if(ja != nullptr)
          free(ja);
  }
  inline SparseMatrix::SparseMatrix(int num_rows_in, int num_non_zeroes_in)
          : num_rows(num_rows_in), num_non_zeroes(num_non_zeroes_in){
      data = (double*)malloc(sizeof(double) * num_non_zeroes);
      if(data == nullptr)
          throw std::logic_error("Could not allocate space for sparse matrix");
  }
  inline SparseMatrix SparseMatrix::fromCompressedRowStorage(int num_rows, int num_non_zeros, int *ia_in, int *ja_in) {
      SparseMatrix m(num_rows, num_non_zeros);
      m.ia = (int*)malloc(sizeof(int) * (num_rows+1));
      m.ja = (int*)malloc(sizeof(int) * num_non_zeros);
      if(m.ia == nullptr or m.ja == nullptr)
          throw std::logic_error("Could not allocate space for csr format");
      std::memcpy(m.ia, ia_in, sizeof(int)*(num_rows+1));
      std::memcpy(m.ja, ja_in, sizeof(int)*(num_non_zeros));
      return std::move(m);
  }
  inline double& SparseMatrix::operator()(int row, int col){
      assert(row < num_rows);
      for(long index = ia[row]; index < ia[row+1]; index++){
          if(ja[index] == col)
              return data[index];
      }
      throw std::logic_error("Could not find row and colum of matrix: " + std::to_string(row) + " " + std::to_string(col));
  }
}