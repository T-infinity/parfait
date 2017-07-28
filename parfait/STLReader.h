#pragma once
#include <string>

namespace Parfait{
  class Facet;
  namespace STL{
    class Reader{
      public:
        Reader(std::string fileName);
        int numberOfFacets();
        std::vector<Facet> readFacets();
        std::vector<Facet> readFacets(int start, int end);
    private:
        std::string fileName;
        void throwIfFileDoesNotExist();
        void *putPointerAtFacet(FILE *fp, int start);
        void chompHeader(FILE *fp);
        void readFacet(FILE *fp, Facet &facet);
    };

  }
}

#include "STLReader.hpp"