#include <string>
#include <stdexcept>

namespace Parfait {

    class CRS {
        public:
            CRS(const std::vector<std::vector<int>>& graph){
                ia.resize(graph.size()+1);
                ia[0] = 0;
                for(int i = 0; i < graph.size(); i++){
                    ia[i+1] = ia[i] + graph[i].size();
                    for(int j = 0; j < graph[i].size(); j++){
                        auto id = graph[i][j];
                        ja.push_back(id);
                        if(id == i){
                            iau.push_back(ia[i] + j);
                        }
                    }
                }

            }

            int getLocation(int i, int j){
                if(i == j){
                    return iau[i];
                }
                for(int search = ia[i]; search < ia[i+1]; search++){
                    if(ja[search] == j)
                        return search;
                }

                throw std::logic_error("Could not find location of item " + std::to_string(i) + " " + std::to_string(j));
            }


            std::vector<int> ia;
            std::vector<int> ja;
            std::vector<int> iau;
    };
}
