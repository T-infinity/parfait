
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
#include <catch.hpp>
#include <parfait/SparseMatrix.h>

using namespace Parfait;
TEST_CASE("SparseMatrix create from Compressed Row Storage"){
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