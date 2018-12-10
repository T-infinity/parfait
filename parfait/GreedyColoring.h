#pragma once
#include <set>
#include <vector>

class GreedyColoring {
public:
    static std::vector<int> colorGraph(const std::vector<std::vector<int>> &graph) {
        std::vector<int> colors(graph.size(), -1);
        for (int i = 0; i < graph.size(); i++) {
            std::set<int> nbr_colors;
            for (int nbr:graph[i])
                nbr_colors.insert(colors[nbr]);
            colors[i] = determineLowestAvailableColor(nbr_colors);
        }
        return colors;
    }

private:
    static int determineLowestAvailableColor(const std::set<int> &nbr_colors) {
        for (int i = 0; i < nbr_colors.size(); i++)
            if (nbr_colors.count(i) == 0)
                return i;
        return int(nbr_colors.size());
    }
};
