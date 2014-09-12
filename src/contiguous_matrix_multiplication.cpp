#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "contiguous_matrix_multiplication.h"


void MatrixVectorMultiply(const double *A,const double *v, double* x, int nrows, int ncols){

    for(int i = 0; i < nrows; i++){
        x[i] = 0.0;
    }

	for(int row=0;row<nrows;row++)
		for(int col=0;col<ncols;col++)
			x[row] += A[row*ncols+col]*v[col];
}

void MatrixMatrixMultiply(const double *A,const double *B, int anrows, int ancols, int bnrows, int bncols, double *O){
    
    assert(ancols == bnrows);
	for(int i=0;i<anrows*bncols;i++)
		O[i] = 0.0;

	for(int arow=0;arow<anrows;arow++)
		for(int acol=0;acol<bncols;acol++)
			for(int brow=0;brow<bnrows;brow++)
				O[arow*bncols+acol] += A[arow*ancols+brow]*B[brow*bncols+acol];
}
