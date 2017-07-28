#pragma once
#include <array>
#include <set>
#include <vector>

namespace Parfait {
    class EdgeBuilder {
    public:
        template<typename T>
        void addCell(int *cell, const T &local_edge_map) {
            for (auto &edge:local_edge_map) {
                int a = std::min(cell[edge[0]], cell[edge[1]]);
                int b = std::max(cell[edge[0]], cell[edge[1]]);
                e.insert({a, b});
            }
        }

        std::vector<std::array<int, 2>> edges() {
            return std::vector<std::array<int, 2>>(e.begin(), e.end());
        };
    private:
        std::set<std::array<int, 2>> e;
    };
}
