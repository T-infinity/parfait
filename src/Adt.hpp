
namespace Parfait {
    template<int ndim>
    Adt<ndim>::~Adt() {
        delete root;
    }

    template<int ndim>
    Adt<ndim>::Adt(Adt<ndim> &&other) {
        root = other.root;
        other.root = nullptr;
    }

    template<int ndim>
    Adt<ndim>::Adt() {
        //------------------------------------------------------------------
        // Explanation of how the dimension of the tree is
        // chosen:
        // ----- All objects have to be stored as points
        // -- A 2d point can obviously be stored as a 2d point
        // -- A 3d point can be stored as a 3d point
        // -- A 2d extent has 4 components (xmin,ymin)(xmax,ymax),
        // 			so it can be viewed as a "point" in 4d space
        // -- A 3d extent has 6 components (xmin,ymin,zmin)(xmax,ymax,zmax),
        // 			so it can be viewed as a "point" in 6d space
        //------------------------------------------------------------------
        //  For further explanation, see
        //  	An Alternating Digial Tree (ADT) Algorithm for 3D Geometric
        //  	Searching and Intersection Problems.
        //  	Jaime Peraire, Javier Bonet
        //  	International Journal for Numerical Methods In Eng, vol 31,
        //  	1-17, (1991)
        //------------------------------------------------------------------
        root = nullptr;
    }

    template<int ndim>
    std::vector<int> Adt<ndim>::retrieve(const double *extent) const {
        double a[6], b[6];
        std::vector<int> ids;
        if (root == nullptr) {
            return ids;
        }
        create_hyper_rectangle_from_extent(extent, a, b);
        retrieve(ids, a, b);
        return ids;
    }

    template<int ndim>
    void Adt<ndim>::store(int object_id, const double *x) {

        if (root == nullptr) {
            rootLock.lock();
            if (root == nullptr) {
                std::array<double, ndim> xmin;
                std::array<double, ndim> xmax;
                for (int i = 0; i < ndim; i++) {
                    xmin[i] = 0.0;
                    xmax[i] = 1.0;
                }
                root = new Adt_elem<ndim>{4, xmin, xmax, object_id, x};
                rootLock.unlock();
            } else {
                rootLock.unlock();
                store(root, object_id, x);
            }
        }
        else {
            store(root, object_id, x);
        }
    }

    template<int ndim>
    void Adt<ndim>::store(Adt_elem<ndim> *elem, int object_id, const double *x) {

        auto whichChild = determineChild(elem, x);

        int split_axis = elem->level % ndim;
        double midpoint = 0.5 * (elem->xmax[split_axis] + elem->xmin[split_axis]);
        int child_level = elem->level + 1;
        std::array<double, ndim> xmin = elem->xmin;
        std::array<double, ndim> xmax = elem->xmax;
        if (whichChild == LEFT)
            xmax[split_axis] = midpoint;
        else
            xmin[split_axis] = midpoint;

        if (whichChild == LEFT) {
            if (elem->lchild == nullptr) {
                elem->leftLock.lock();
                if (elem->lchild == nullptr) {
                    elem->lchild = new Adt_elem<ndim>{child_level, xmin, xmax, object_id, x};
                    elem->leftLock.unlock();
                    return;
                } else {
                    elem->leftLock.unlock();
                    store(elem->lchild, object_id, x);
                    return;
                }
            }
            else {
                store(elem->lchild, object_id, x);
                return;
            }
        }
        if (whichChild == RIGHT) {
            if (elem->rchild == nullptr) {
                elem->rightLock.lock();
                if (elem->rchild == nullptr) {
                    elem->rchild = new Adt_elem<ndim>{child_level, xmin, xmax, object_id, x};
                    elem->rightLock.unlock();
                    return;
                }
                else {
                    elem->rightLock.unlock();
                    store(elem->rchild, object_id, x);
                    return;
                }
            }
            else {
                store(elem->rchild, object_id, x);
                return;
            }
        }
        else {
            throw std::logic_error("Adt child was not determined to be left or right");
        }
    }

    template<int ndim>
    void Adt<ndim>::retrieve(std::vector<int> &ids, double *a, double *b) const {
        retrieve(root, ids, a, b);
    }

    template<int ndim>
    void Adt<ndim>::retrieve(Adt_elem<ndim> *elem, std::vector<int> &ids, double *a,
                             double *b) const {
        if (!elem->contains_hyper_rectangle(a, b)) return;
        if (elem->hyper_rectangle_contains_object(a, b))
            ids.push_back(elem->object_id);
        if (elem->lchild != nullptr) retrieve(elem->lchild, ids, a, b);
        if (elem->rchild != nullptr) retrieve(elem->rchild, ids, a, b);
    }

    template<int ndim>
    void Adt<ndim>::create_hyper_rectangle_from_extent(const double *extent, double *a,
                                                       double *b) const {
        if (ndim == 2) {
            a[0] = extent[0];
            a[1] = extent[1];
            b[0] = extent[2];
            b[1] = extent[3];
        } else if (ndim == 3) {
            a[0] = extent[0];
            a[1] = extent[1];
            a[2] = extent[2];
            b[0] = extent[3];
            b[1] = extent[4];
            b[2] = extent[5];
        } else if (ndim == 4)  // 2d extent box
        {
            double dx, dy;
            dx = extent[2] - extent[0];
            dy = extent[3] - extent[1];
            a[0] = 0.0;
            a[1] = 0.0;
            a[2] = extent[2] - dx;
            a[3] = extent[3] - dy;
            b[0] = extent[0] + dx;
            b[1] = extent[1] + dy;
            b[2] = 1.0;
            b[3] = 1.0;
        } else if (ndim == 6)  // 3d extent box
        {
            double dx, dy, dz;
            dx = extent[3] - extent[0];
            dy = extent[4] - extent[1];
            dz = extent[5] - extent[2];
            a[0] = 0.0;
            a[1] = 0.0;
            a[2] = 0.0;
            a[3] = extent[3] - dx;
            a[4] = extent[4] - dy;
            a[5] = extent[5] - dz;
            b[0] = extent[0] + dx;
            b[1] = extent[1] + dy;
            b[2] = extent[2] + dz;
            b[3] = 1.0;
            b[4] = 1.0;
            b[5] = 1.0;
        } else {
            throw std::logic_error(
                    "ADT ERROR: Only 2d and 3d extent boxes can be turned"
                            " into hyper rectangles\n");
        }
    }

    template<int ndim>
    typename Adt<ndim>::ChildType Adt<ndim>::determineChild(Adt_elem<ndim> *elem, double const *x) {

        int split_axis = elem->level % ndim;
        double midpoint = 0.5 * (elem->xmax[split_axis] + elem->xmin[split_axis]);
        if (x[split_axis] < midpoint)
            return LEFT;
        else
            return RIGHT;
    }
}