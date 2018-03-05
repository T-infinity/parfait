
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
#include <map>

namespace Parfait {

template <typename T, int BlockRow, int BlockCol>
class Block{
public:
    T& operator()(int row, int col) {
        return data[BlockRow*row + col];
    }
    const T& operator()(int row, int col) const {
        return data[BlockRow*row + col];
    }
private:
    std::array<T, BlockRow*BlockCol> data;
};

template <typename T, int BlockRow, int BlockCol>
class MatrixBlockSparse{
public:
    MatrixBlockSparse(const std::vector<long> &global_rows, const std::vector<std::vector<long>>& global_cols_of_local_rows)
    : data(countEntries(global_cols_of_local_rows)){
        local_row_start.resize(global_rows.size()+1);
        local_row_start[0] = 0;
        global_column_index.resize(data.size());
        size_t index = 0;
        for(int r = 0; r < global_rows.size(); r++){
            local_row_start[r+1] = local_row_start[r] + global_cols_of_local_rows[r].size();
            for (auto& column : global_cols_of_local_rows[r]){
                global_column_index[index++] = column;
            }
        }

        for (int local_row = 0; local_row < global_rows.size(); ++local_row) {
            global_to_local[global_rows[local_row]] = local_row;
        }
    }
    size_t countEntries(const std::vector<std::vector<long>> &global_cols_of_local_rows) const {
        size_t total_entries = 0;
        for(auto& row : global_cols_of_local_rows)
            total_entries += row.size();
        return total_entries;
    }

    Block<T, BlockRow, BlockCol>& operator()(long row, long col){
        int local_row = global_to_local[row];
        for(long index = local_row_start[local_row]; index < local_row_start[local_row+1]; index++){
            if(global_column_index[index] == col)
                return data[index];
        }
        throw std::logic_error("Could not find entry for row " + std::to_string(row) + " col: " + std::to_string(col));
    }

private:
    std::vector< Block<T, BlockRow, BlockCol> > data;
    std::vector<long> local_row_start, global_column_index;
    std::map<long, int> global_to_local;
};

}

TEST_CASE("Block Entries can be set with paren") {
    Parfait::Block<double, 5, 5> block;
    block(0, 0) = 6;
    block(1, 1) = 7;
    block(4, 4) = 9;
    block(4, 0) = 8.8;
    block(0, 4) = 8.9;
    REQUIRE(block(0, 0) == 6);
    REQUIRE(block(1, 1) == 7);
    REQUIRE(block(4, 4) == 9);
    REQUIRE(block(4, 0) == 8.8);
    REQUIRE(block(0, 4) == 8.9);
}

TEST_CASE("Block Entries can be read on a const object") {
    Parfait::Block<double, 5, 5> block;
    block(0, 0) = 6;
    block(1, 1) = 7;
    block(4, 4) = 9;
    block(4, 0) = 8.8;
    block(0, 4) = 8.9;
    const Parfait::Block<double, 5, 5>& const_block = block;
    REQUIRE(const_block(0, 0) == 6);
    REQUIRE(const_block(1, 1) == 7);
    REQUIRE(const_block(4, 4) == 9);
    REQUIRE(const_block(4, 0) == 8.8);
    REQUIRE(const_block(0, 4) == 8.9);
}

TEST_CASE("Sparse matrix exists") {
    std::vector<long> global_rows = {7,8,9,10};
    std::vector<std::vector<long>> global_colums_in_local_row = {{7, 8},
                                                                 {8, 9},
                                                                 {9, 10},
                                                                 {10, 11}};


    Parfait::MatrixBlockSparse<double, 5, 5> matrix(global_rows, global_colums_in_local_row);

    Parfait::Block<double, 5, 5> block;
    block(0,0) = 0;
    block(1,1) = 1;
    block(2,3) = 2;

    matrix(7,7) = block;
    block(1,1) = 100;
    matrix(8,8) = block;
    Parfait::Block<double, 5, 5>& b1 = matrix(7,7);
    REQUIRE(b1(1,1) == 1);
    Parfait::Block<double, 5, 5>& b2 = matrix(8,8);
    REQUIRE(b2(1,1) == 100);

}