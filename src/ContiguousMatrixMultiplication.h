#ifndef CONTIGUOUS_MATRIX_MULTIPLICATION_H
#define CONTIGUOUS_MATRIX_MULTIPLICATION_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// A*v stored in x
void MatrixVectorMultiply(const double *A,const double *v,double *x, int nrows, int ncols);

// A * B stored in O
void MatrixMatrixMultiply(const double *A,const double *B, int anrows, int ancols, int bnrows, int bncols, double *O);

inline void MatrixVectorMultiply(const double *A,const double *v, double* x, int nrows, int ncols){

  for(int i = 0; i < nrows; i++){
    x[i] = 0.0;
  }

  for(int row=0;row<nrows;row++)
    for(int col=0;col<ncols;col++)
      x[row] += A[row*ncols+col]*v[col];
}

inline void MatrixMatrixMultiply(const double *A,const double *B, int anrows, int ancols, int bnrows, int bncols, double *O){

  assert(ancols == bnrows);
  for(int i=0;i<anrows*bncols;i++)
    O[i] = 0.0;

  for(int arow=0;arow<anrows;arow++)
    for(int acol=0;acol<bncols;acol++)
      for(int brow=0;brow<bnrows;brow++)
        O[arow*bncols+acol] += A[arow*ancols+brow]*B[brow*bncols+acol];
}
#endif

