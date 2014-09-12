#include "adt.h"
#include <stdexcept>
#include <memory>

Adt::Adt() {
    printf("Did you mean to call this constructor?\n");
    abort();
}

Adt::~Adt() {

     for (int i = 0; i < SearchTree.size(); i++) {
        delete SearchTree[i];
    }
}

Adt::Adt(int adt_type) {
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
    switch (adt_type) {
        case ADT_2D_POINT:
            ndim = 2;
            // printf("# Creating Adt for storing 2d points\n");
            break;
        case ADT_3D_POINT:
            ndim = 3;
            // printf("# Creating Adt for storing 3d points\n");
            break;
        case ADT_2D_EXTENT:
            ndim = 4;
            // printf("# Creating Adt for storing 2d objects (extents)\n");
            break;
        case ADT_3D_EXTENT:
            ndim = 6;
            // printf("# Creating Adt for storing 3d objects (extents)\n");
            break;
        default:
            assert(false);
    }
    type = adt_type;
}

// ============= Public Interface ===============

std::vector<int> Adt::retrieve(const double *extent) {
    double a[6], b[6];
    std::vector<int> ids;
    if(SearchTree.size() == 0){
        return ids;    
    }
    create_hyper_rectangle_from_extent(extent, a, b);
    retrieve(ids, a, b);
    return ids;
}

void Adt::store(int object_id, const double *x) {
    stored = false;
    if(SearchTree.size() == 0){
        double xmin[ndim];
        double xmax[ndim];
        for(int i = 0; i < ndim; i++){
            xmin[i] = 0.0;
            xmax[i] = 1.0;
        }
        SearchTree.push_back(new Adt_elem(4, xmin, xmax, ndim, object_id, x));
        nelem = 1;
    } else {
        store(0, object_id, x);
    }
}

// ========== Private Implementation ==============
void Adt::store(int elem_id, int object_id, const double *x) {
    if (stored) return;
    Adt_elem *elem = SearchTree[elem_id];
    if (elem->contains_object(x, ndim)) {
        // if kids exist, pass to them
        if (elem->lchild != ADT_NO_CHILD) store(elem->lchild, object_id, x);
        if (elem->rchild != ADT_NO_CHILD) store(elem->rchild, object_id, x);
        if (stored) return;
        // now both branches have been attempted if they
        // exist, so there are 3 possibilities
        // 1. both kids blank
        // 2. left kid blank
        // 3. right kid blank
        int split_axis = elem->level % ndim;
        int child_level = elem->level + 1;
        double midpoint = 0.5 * (elem->xmax[split_axis] + elem->xmin[split_axis]);
        bool create_left_child = false;
        bool create_right_child = false;
        if ((elem->lchild == ADT_NO_CHILD) && (elem->rchild == ADT_NO_CHILD)) {
            // figure out which child contains the object
            if (x[split_axis] < midpoint)
                create_left_child = true;
            else
                create_right_child = true;
        } else if ((elem->lchild == ADT_NO_CHILD) &&
                   (elem->rchild != ADT_NO_CHILD))
            create_left_child = true;
        else if ((elem->lchild != ADT_NO_CHILD) && (elem->rchild == ADT_NO_CHILD))
            create_right_child = true;
        else
            assert(false);
        // since ndim will be a max of 6, allocate
        // on the stack C99 style for speed.
        double xmin[ndim], xmax[ndim];
        // clone the extent of elem
        for (int i = 0; i < ndim; i++) {
            xmin[i] = elem->xmin[i];
            xmax[i] = elem->xmax[i];
        }
        // then split in half, on the correct axis
        // and create the child
        if (create_left_child) {
            xmax[split_axis] = midpoint;
            elem->lchild = nelem;
        } else {
            xmin[split_axis] = midpoint;
            elem->rchild = nelem;
        }
        SearchTree.push_back(
            new Adt_elem(child_level, xmin, xmax, ndim, object_id, x));
        nelem++;
        stored = true;
    }
}

void Adt::retrieve(std::vector<int> &ids, double *a, double *b) {
    retrieve(0, ids, a, b);
}

void Adt::retrieve(int elem_id, std::vector<int> &ids, double *a, double *b) {
    Adt_elem *elem = SearchTree[elem_id];
    if (!elem->contains_hyper_rectangle(a, b, ndim)) return;
    if (elem->hyper_rectangle_contains_object(a, b, ndim))
        ids.push_back(elem->object_id);
    if (elem->lchild != ADT_NO_CHILD) retrieve(elem->lchild, ids, a, b);
    if (elem->rchild != ADT_NO_CHILD) retrieve(elem->rchild, ids, a, b);
}

void Adt::create_hyper_rectangle_from_extent(const double *extent, double *a,
                                             double *b) {
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
        printf(
            "ADT ERROR: Only 2d and 3d extent boxes can be turned into hyper "
            "rectangles\n");
        abort();
    }
}

void Adt::print_debug_stats(FILE *f) {
    // count how many nodes of the tree have 0,1,2 kids
    // to help see how balanced / unbalanced it is
    int no_kids = 0, left_kid = 0, right_kid = 0, two_kids = 0;
    for (int i = 0; i < nelem; i++) {
        if ((SearchTree[i]->lchild != ADT_NO_CHILD) &&
            (SearchTree[i]->rchild != ADT_NO_CHILD))
            two_kids++;
        if ((SearchTree[i]->lchild == ADT_NO_CHILD) &&
            (SearchTree[i]->rchild == ADT_NO_CHILD))
            no_kids++;
        if ((SearchTree[i]->lchild != ADT_NO_CHILD) &&
            (SearchTree[i]->rchild == ADT_NO_CHILD))
            left_kid++;
        if ((SearchTree[i]->lchild == ADT_NO_CHILD) &&
            (SearchTree[i]->rchild != ADT_NO_CHILD))
            right_kid++;
    }

    fprintf(f, "Adt\n");
    fprintf(f, " -- nodes with 0 kids %i\n", no_kids);
    fprintf(f, " -- nodes with 2 kids %i\n", two_kids);
    fprintf(f, " -- nodes with only left kid  %i\n", left_kid);
    fprintf(f, " -- nodes with only right kid %i\n", right_kid);
}
