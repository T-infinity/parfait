#ifndef ADT_ELEM_H
#define ADT_ELEM_H

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
        int object_id;
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

#endif
