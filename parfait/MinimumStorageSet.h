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
