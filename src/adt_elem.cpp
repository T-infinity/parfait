#include "adt_elem.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Adt_elem::Adt_elem(int set_level, const double *set_xmin,
                   const double *set_xmax, int ndim) {
    double blank_x[ndim];
    for (int i = 0; i < ndim; i++) blank_x[i] = 0.0;
    construct(set_level, set_xmin, set_xmax, ndim, ADT_OBJECT_NOT_SET, blank_x);
}

Adt_elem::Adt_elem(int set_level, const double *set_xmin,
                   const double *set_xmax, int ndim, int set_object,
                   const double *set_object_x) {
    construct(set_level, set_xmin, set_xmax, ndim, set_object, set_object_x);
}

void Adt_elem::construct(int set_level, const double *set_xmin,
                         const double *set_xmax, int ndim, int set_object,
                         const double *set_object_x) {
    level = set_level;
    object_id = set_object;
    lchild = ADT_NO_CHILD;
    rchild = ADT_NO_CHILD;

    xmin.resize(ndim);
    xmax.resize(ndim);
    object_x.resize(ndim);
    for (int i = 0; i < ndim; i++) {
        xmin[i] = set_xmin[i];
        xmax[i] = set_xmax[i];
        object_x[i] = set_object_x[i];
    }
}

bool Adt_elem::contains_object(const double *x_in, int ndim) const {
    for (int i = 0; i < ndim; i++)
        if (x_in[i] < xmin[i] - ADT_TOL || x_in[i] > xmax[i] + ADT_TOL)
            return false;
    return true;
}

bool Adt_elem::contains_hyper_rectangle(const double *a,
        const double *b, int ndim) const {

    for (int i = 0; i < ndim; i++)
        if (b[i] < xmin[i] - ADT_TOL || a[i] > xmax[i] + ADT_TOL) return false;
    return true;
}

bool Adt_elem::hyper_rectangle_contains_object(const double *a, const double *b,
                                               int ndim) const {
    for (int i = 0; i < ndim; i++)
        if (b[i] < object_x[i] - ADT_TOL || a[i] > object_x[i] + ADT_TOL)
            return false;
    return true;
}
