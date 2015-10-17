#include <catch.hpp>
#include "SparseMatrix.h"

using namespace Parfait;
TEST_CASE("SparseMatrix Exists"){
    int nrows = 3;
    int nnz = 9;
    std::vector<int> ia = {0,3,6,9};
    std::vector<int> ja = {0,1,2,0,1,2,0,1,2};
    auto m = SparseMatrix::fromCompressedRowStorage(nrows, nnz, ia.data(), ja.data());
    m(0,0) = 1;
    m(1,1) = 1;
    m(2,2) = 1;
    REQUIRE((m(0,0) == 1));
    REQUIRE((m(1,1) == 1));
    REQUIRE((m(2,2) == 1));
}