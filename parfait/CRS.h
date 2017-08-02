#include <string>
#include <stdexcept>
#include <vector>

namespace Parfait {

    class CRS {
        public:
            static CRS buildCRSFromGraph(const std::vector<std::vector<int>> &graph){
                CRS crs;
                crs.ia.resize(graph.size()+1);
                crs.ia[0] = 0;
                for(int i = 0; i < graph.size(); i++){
                    crs.ia[i+1] = crs.ia[i] + graph[i].size();
                    for(int j = 0; j < graph[i].size(); j++){
                        auto id = graph[i][j];
                        crs.ja.push_back(id);
                        if(id == i){
                            crs.iau.push_back(crs.ia[i] + j);
                        }
                    }
                }
                return crs;
            }

            int getLocation(int i, int j){
                if(i == j){
                    return iau[i];
                }
                for(auto search = ia[i]; search < ia[i+1]; search++){
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
