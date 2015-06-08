#ifndef MAPBC_READER_H
#define MAPBC_READER_H
#include <string>
#include <map>

namespace Parfait {
    class MapbcReader {
    public:
        MapbcReader(std::string filename);
        int boundaryCondition(int tag);

        int numberOfTags() { return bcMap.size(); }

        std::map<int, std::pair<int, std::string>> getMap() { return bcMap; }

        void print();
    private:
        bool failedToOpenMapbc;
        std::string filename;
        std::map<int, std::pair<int, std::string>> bcMap;
    };
}


#endif
