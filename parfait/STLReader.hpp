
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
#ifndef PARFAIT_STLREADER_HPP
#define PARFAIT_STLREADER_HPP

#include <stdexcept>
#include "Facet.h"

using namespace Parfait::STL;

inline Reader::Reader(std::string fileName_in) : fileName(fileName_in){
  throwIfFileDoesNotExist();
}

inline int Reader::numberOfFacets() {
  FILE *fp = fopen(fileName.c_str(), "r'");
  chompHeader(fp);
  int num_facets = 0;
  fread(&num_facets, sizeof(int),1, fp);
  fclose(fp);
  return num_facets;
}
inline void Reader::chompHeader(FILE *fp){
  fseek(fp, 80*sizeof(char), SEEK_SET);
}

inline void Reader::throwIfFileDoesNotExist() {
  FILE *fp = fopen(fileName.c_str(), "r");
  if(fp == NULL)
    throw std::logic_error("Could not open file " + fileName);
}

inline std::vector<Parfait::Facet> Reader::readFacets() {
  return readFacets(0, numberOfFacets());
}

inline std::vector<Parfait::Facet> Reader::readFacets(int start, int end) {
  int readCount = end - start;
  std::vector<Facet> facets(readCount);
  FILE *fp = fopen(fileName.c_str(), "r");
  putPointerAtFacet(fp, start);

  for(int i = 0; i < readCount; i++){
    readFacet(fp, facets[i]);
  }

  fclose(fp);
  return facets;
}

inline void *Reader::putPointerAtFacet(FILE *fp, int start) {
  int headerOffset = 80*sizeof(char);
  int facetCountOffset = 1*sizeof(int);
  int sizeofFacet = 12*sizeof(float) + 2*sizeof(char);

  int offset = headerOffset + facetCountOffset + start * sizeofFacet;
  fseek(fp, offset, SEEK_SET);
  return fp;
}

inline void Reader::readFacet(FILE *fp, Facet &facet) {
  float ddum[3] = {-999, -999, -999};
  fread(ddum, sizeof(float), 3, fp); // Normal xyz
  facet.normal[0] = ddum[0];
  facet.normal[1] = ddum[1];
  facet.normal[2] = ddum[2];
  fread(ddum, sizeof(float), 3, fp); // Point 1
  facet[0][0] = ddum[0];
  facet[0][1] = ddum[1];
  facet[0][2] = ddum[2];
  fread(ddum, sizeof(float), 3, fp); // Point 2
  facet[1][0] = ddum[0];
  facet[1][1] = ddum[1];
  facet[1][2] = ddum[2];
  fread(ddum, sizeof(float), 3, fp); // Point 3
  facet[2][0] = ddum[0];
  facet[2][1] = ddum[1];
  facet[2][2] = ddum[2];
  fread(ddum, sizeof(char), 2, fp);
}

#endif 
