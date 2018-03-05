
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
#include <algorithm>
#include <vector>

namespace Parfait {
template<typename T>
class MinimumStorageSet {
public:
    void insert(const T &element) {
        if (std::binary_search(vec.begin(), vec.end(), element))
            return;
        auto it = getIteratorToBigger(element);
        vec.insert(it, element);
    }

    auto size() { return vec.size(); };

    auto convertToVector() { return std::move(vec); }
private:
    std::vector<T> vec;

    auto getIteratorToBigger(const T &e) {
        for (int i = 0; i < vec.size(); i++)
            if (vec[i] > e)
                return vec.begin() + i;
        return vec.end();
    }
};
}
