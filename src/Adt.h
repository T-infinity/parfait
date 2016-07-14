#pragma once

#include "AdtElem.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "UnitTransformer.h"
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
        void store(int object_id, const double *x);
        std::vector<int> retrieve(const double *extent) const;
    private:
        enum ChildType {
            LEFT, RIGHT
        };
        Adt_elem<ndim> *root;
        std::mutex rootLock;

        void retrieve(std::vector<int> &ids, double *a, double *b) const;
        void create_hyper_rectangle_from_extent(const double *extent, double *a, double *b) const;
        void store(Adt_elem<ndim> *elem_id, int object_id, const double *x);
        void retrieve(Adt_elem<ndim> *elem, std::vector<int> &ids, double *a, double *b) const;
        ChildType determineChild(Adt_elem<ndim> *elem, double const *x);
        Adt_elem<ndim> *&getChildPointer(Adt_elem<ndim> *elem, Adt<ndim>::ChildType &whichChild);
    };
}
#include "Adt.hpp"