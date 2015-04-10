template<int ndim>
Adt_elem<ndim>::Adt_elem(int set_level, std::array<double,ndim> const &set_xmin,
                         std::array<double,ndim> const &set_xmax) {
    double blank_x[ndim];
    for (int i = 0; i < ndim; i++) blank_x[i] = 0.0;
    construct(set_level, set_xmin, set_xmax,ADT_OBJECT_NOT_SET, blank_x);
}

template<int ndim>
Adt_elem<ndim>::Adt_elem(int set_level,
                         std::array<double, ndim> const &set_xmin,
                         std::array<double, ndim> const &set_xmax, int set_object,
                         const double *set_object_x) {
    construct(set_level, set_xmin, set_xmax,set_object, set_object_x);
}

template<int ndim>
void Adt_elem<ndim>::construct(int set_level, std::array<double, ndim> const &set_xmin,
                               std::array<double, ndim> const &set_xmax, int set_object,
                               const double *set_object_x) {
    level = set_level;
    object_id = set_object;
    lchild = nullptr;
    rchild = nullptr;
    
    for (int i = 0; i < ndim; i++) {
        xmin[i] = set_xmin[i];
        xmax[i] = set_xmax[i];
        object_x[i] = set_object_x[i];
    }
}

template<int ndim>
bool Adt_elem<ndim>::contains_object(const double *x_in) const {
    for (int i = 0; i < ndim; i++)
        if (x_in[i] < xmin[i] - ADT_TOL || x_in[i] > xmax[i] + ADT_TOL)
            return false;
    return true;
}

template<int ndim>
bool Adt_elem<ndim>::contains_hyper_rectangle(const double *a,
        const double *b) const {

    for (int i = 0; i < ndim; i++)
        if (b[i] < xmin[i] - ADT_TOL || a[i] > xmax[i] + ADT_TOL) return false;
    return true;
}

template<int ndim>
bool Adt_elem<ndim>::hyper_rectangle_contains_object(
        const double *a, const double *b) const {
    for (int i = 0; i < ndim; i++)
        if (b[i] < object_x[i] - ADT_TOL || a[i] > object_x[i] + ADT_TOL)
            return false;
    return true;
}

template<int ndim>
Adt_elem<ndim>::~Adt_elem<ndim>() {
    if(lchild != nullptr) delete lchild;
    if(rchild != nullptr) delete rchild;
}
