
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
namespace Parfait {
    template<int ndim>
    Adt_elem<ndim>::Adt_elem(int set_level, std::array<double, ndim> const &set_xmin,
                             std::array<double, ndim> const &set_xmax) {
        double blank_x[ndim];
        for (int i = 0; i < ndim; i++) blank_x[i] = 0.0;
        construct(set_level, set_xmin, set_xmax, ADT_OBJECT_NOT_SET, blank_x);
    }

    template<int ndim>
    Adt_elem<ndim>::Adt_elem(int set_level,
                             std::array<double, ndim> const &set_xmin,
                             std::array<double, ndim> const &set_xmax, int set_object,
                             const double *set_object_x) {
        construct(set_level, set_xmin, set_xmax, set_object, set_object_x);
    }

    template<int ndim>
    void Adt_elem<ndim>::construct(int set_level, std::array<double, ndim> const &set_xmin,
                                   std::array<double, ndim> const &set_xmax, int set_object,
                                   const double *set_object_x) {
        level = set_level;
        tag = set_object;
        lchild = nullptr;
        rchild = nullptr;

        for (int i=0; i<ndim; i++) {
            xmin[i] = set_xmin[i];
            xmax[i] = set_xmax[i];
            object_x[i] = set_object_x[i];
        }
    }

    template<int ndim>
    bool Adt_elem<ndim>::contains_object(const double* x_in) const {
        for (int i = 0; i < ndim; i++)
            if (x_in[i] < xmin[i] - ADT_TOL || x_in[i] > xmax[i] + ADT_TOL)
                return false;
        return true;
    }

    template<int ndim>
    bool Adt_elem<ndim>::contains_hyper_rectangle(const double* a,
                                                  const double* b) const {

        for (int i = 0; i < ndim; i++)
            if (b[i] < xmin[i] - ADT_TOL || a[i] > xmax[i] + ADT_TOL) return false;
        return true;
    }

    template<int ndim>
    bool Adt_elem<ndim>::hyper_rectangle_contains_object(
            const double* a, const double* b) const {
        for (int i = 0; i < ndim; i++)
            if (b[i] < object_x[i] - ADT_TOL || a[i] > object_x[i] + ADT_TOL)
                return false;
        return true;
    }

    template<int ndim>
    Adt_elem<ndim>::~Adt_elem<ndim>() {
        if (lchild != nullptr) delete lchild;
        if (rchild != nullptr) delete rchild;
    }
}