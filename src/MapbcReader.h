#ifndef MAPBC_READER_H
#define MAPBC_READER_H
#include <string>
#include <map>

namespace Parfait {
   
    typedef std::map<int, std::pair<int,std::string>> BoundaryConditionMap;
    
    class MapbcReader {
    public:
        MapbcReader(std::string filename);
        int boundaryCondition(int tag);

        int numberOfTags() { return bcMap.size(); }

        BoundaryConditionMap getMap() { return bcMap; }

        void print();
    private:
        bool failedToOpenMapbc;
        std::string filename;
        BoundaryConditionMap bcMap;
    };
}

#include "MapbcReader.hpp"

#endif
