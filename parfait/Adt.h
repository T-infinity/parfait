
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
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