#ifndef ADT_H
#define ADT_H

#include "adt_elem.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "unit_transformer.h"
#include <memory>

#define ADT_2D_POINT  1
#define ADT_3D_POINT  2
#define ADT_2D_EXTENT 3
#define ADT_3D_EXTENT 4

template<int ndim>
class Adt
{
	public:
        Adt(const Adt &) = delete;
		Adt();
        Adt(Adt && other);
        ~Adt();
		void store(int object_id, const double *x);	
        std::vector<int> retrieve(const double *extent) const;
	private:
        Adt_elem<ndim> *root;
		bool stored;
		void retrieve(std::vector<int> &ids,double *a,double *b) const;
		void create_hyper_rectangle_from_extent( const double *extent,double *a,double *b) const;
		void store(Adt_elem<ndim> *elem_id, int object_id, const double *x);
		void retrieve(Adt_elem<ndim> *elem, std::vector<int> &ids, double *a, double *b) const;
};

#include "adt.hpp"

#endif
