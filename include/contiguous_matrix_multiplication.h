#ifndef CONTIGUOUS_MATRIX_MULTIPLICATION_H
#define CONTIGUOUS_MATRIX_MULTIPLICATION_H

// A*v stored in x
void MatrixVectorMultiply(const double *A,const double *v,double *x, int nrows, int ncols);

// A * B stored in O
void MatrixMatrixMultiply(const double *A,const double *B, int anrows, int ancols, int bnrows, int bncols, double *O);
#endif

