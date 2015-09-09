#ifndef PARFAIT_CARTBLOCKSLICE_H_H
#define PARFAIT_CARTBLOCKSLICE_H_H

#include "CartBlock.h"
namespace Parfait {
    class CartBlockSlice {
    public:
        CartBlockSlice(CartBlock &block, int lo, int hi)
                : parent_block(block),
                  lo_cell(lo),
                  hi_cell(hi)
                { }

        class Iterator {
        public:
            Iterator(const CartBlock& block, int lo,int hi,int id):
                    b(block)
            {
                b.convertCellIdTo_ijk(lo,min_ijk[0],min_ijk[1],min_ijk[2]);
                b.convertCellIdTo_ijk(hi,max_ijk[0],max_ijk[1],max_ijk[2]);
                b.convertCellIdTo_ijk(id,ijk[0],ijk[1],ijk[2]);
            };
            void operator++(){
                if(ijk[0] < max_ijk[0])
                    ijk[0]++;
                else if(ijk[1] < max_ijk[1])
                    ijk[1]++;
                else if(ijk[2] < max_ijk[2])
                    ijk[2]++;
                printf("ijk (%i %i %i)\n",ijk[0],ijk[1],ijk[2]);
            };
            bool operator!=(const Iterator &rhs){
                printf("%i %i %i == %i %i %i\n",ijk[0],ijk[1],ijk[2],rhs.ijk[0],rhs.ijk[1],rhs.ijk[2]);
                return ijk[0] == rhs.ijk[0] and
                       ijk[1] == rhs.ijk[1] and
                       ijk[2] == rhs.ijk[2];
            };
            int  operator*(){return 0;};
        private:
            const CartBlock& b;
            int min_ijk[3];
            int max_ijk[3];
            int ijk[3];
        };

        Iterator begin() {
            return Iterator(parent_block,lo_cell,hi_cell,lo_cell);
        }

        Iterator end() {
            return Iterator(parent_block,lo_cell,hi_cell,hi_cell);
        }
    private:
        CartBlock &parent_block;
        int lo_cell;
        int hi_cell;
    };
}
#endif //PARFAIT_CARTBLOCKSLICE_H_H
