#pragma once

#include "AdtElem.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "UnitTransformer.h"
#include <tuple>
#include <memory>
#include <mutex>

namespace Parfait {
    template<int ndim>
    class Adt {
    public:
        Adt(const Adt &) = delete;
        Adt();
        Adt(Adt &&other);
        ~Adt();
        void store(int tag, const double *x);
        std::vector<int> retrieve(const double *extent) const;
        void removeFirst(int tag, const double* extent);
    private:
        enum ChildType {
            LEFT, RIGHT
        };
        Adt_elem<ndim> *root;
        std::mutex rootLock;

        void retrieve(std::vector<int> &tags, double *a, double *b) const;
        void create_hyper_rectangle_from_extent(const double *extent, double *a, double *b) const;
        void store(Adt_elem<ndim> *elem_id, int tag, const double *x);
        void retrieve(Adt_elem<ndim> *elem, std::vector<int> &tags, double *a, double *b) const;
        ChildType determineChild(Adt_elem<ndim> *elem, double const *x);
        Adt_elem<ndim> *&getChildPointer(Adt_elem<ndim> *elem, Adt<ndim>::ChildType &whichChild);
        void removeFirst(int tag, const double* a, const double* b);
        void removeFirst(Adt_elem<ndim>* elem, Adt_elem<ndim>* parent, int tag, const double* a, const double* b);
        void replaceElementWithLeaf(Adt_elem<ndim>* elem, Adt_elem<ndim>* parent);
        std::tuple<Adt_elem<ndim>*, Adt_elem<ndim>*> findFirstLeaf(Adt_elem<ndim>* elem, Adt_elem<ndim>* parent);
        bool isLeaf(const Adt_elem<ndim>* elem) const;
    };
}
#include "Adt.hpp"