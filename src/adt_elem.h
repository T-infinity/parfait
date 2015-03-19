#ifndef ADT_ELEM
#define ADT_ELEM

#define ADT_TOL 1.0e-12
#define ADT_NO_CHILD -1
#define ADT_OBJECT_NOT_SET -10

#include <vector>

class Adt_elem {
    public:
    int level;
    int object_id;
    int lchild;
    int rchild;
    std::vector<double> xmin;
    std::vector<double> xmax;
    std::vector<double> object_x;

    Adt_elem() = delete;
    ~Adt_elem() = default;
    Adt_elem(int set_level, const double *set_xmin, const double *set_xmax,
             int ndim);
    Adt_elem(int set_level, const double *set_xmin, const double *set_xmax,
             int ndim, int set_object_id, const double *set_object_x);
    bool contains_object(const double *x_in, int ndim) const;
    bool contains_hyper_rectangle(const double *a, const double *b, int ndim) const;
    bool hyper_rectangle_contains_object(const double *a, const double *b,
                                         int ndim) const;

    private:
    void construct(int set_level, const double *set_xmin,
                   const double *set_xmax, int ndim, int set_object_id,
                   const double *set_object_x);
};

#endif
