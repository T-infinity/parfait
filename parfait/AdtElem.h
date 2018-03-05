
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

#define ADT_TOL 1.0e-12
#define ADT_NO_CHILD -1
#define ADT_OBJECT_NOT_SET -10

#include <array>
#include <vector>
#include <memory>
#include <mutex>
namespace Parfait {
    template<int ndim>
    class Adt_elem {
    public:
        int level;
        int tag;
        Adt_elem *lchild;
        Adt_elem *rchild;
        std::array<double, ndim> xmin;
        std::array<double, ndim> xmax;
        std::array<double, ndim> object_x;
        std::mutex leftLock;
        std::mutex rightLock;


        Adt_elem() = delete;
        ~Adt_elem();
        Adt_elem(int set_level, std::array<double, ndim> const &set_xmin,
                 std::array<double, ndim> const &set_xmax);
        Adt_elem(int set_level, std::array<double, ndim> const &set_xmin,
                 std::array<double, ndim> const &set_xmax,
                 int set_object_id, const double *set_object_x);
        bool contains_object(const double *x_in) const;
        bool contains_hyper_rectangle(const double *a, const double *b) const;
        bool hyper_rectangle_contains_object(const double *a, const double *b) const;

    private:
        void construct(int set_level, std::array<double, ndim> const &set_xmin,
                       std::array<double, ndim> const &set_xmax, int set_object_id,
                       const double *set_object_x);
    };
}
#include "AdtElem.hpp"