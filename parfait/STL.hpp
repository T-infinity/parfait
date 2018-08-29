
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
#include <assert.h>
#include <math.h>
#include "Point.h"
#include "Extent.h"
#include <stdexcept>
#include <exception>
#include "STLReader.h"

namespace Parfait {
namespace STL {

template <typename P>
void STL::addFacet(const P& p1, const P& p2, const P& p3){
    facets.push_back({{p1[0], p1[1], p1[2]},
                      {p2[0], p2[1], p2[2]},
                      {p3[0], p3[1], p3[2]}});
}

inline void STL::rescale(double scale) {
    for (unsigned int i = 0; i < facets.size(); i++) {
        for (unsigned int j = 0; j < 3; j++) { facets[i][0][j] *= scale; }
        for (unsigned int j = 0; j < 3; j++) { facets[i][1][j] *= scale; }
        for (unsigned int j = 0; j < 3; j++) { facets[i][2][j] *= scale; }
    }
}

inline void STL::scaleToUnitLength() {
    auto d = findDomain();
    scaleToUnitLength(d);
}

inline void STL::scaleToUnitLength(const Extent &d) {
    translateCenterToOrigin(d);
    double length = getLongestCartesianLength(d);
    rescale(1.0/length);
}

inline double STL::getLongestCartesianLength(const Extent &d){
    double dx = d.hi[0] - d.lo[0];
    double dy = d.hi[1] - d.lo[1];
    double dz = d.hi[2] - d.lo[2];
    double max = -20e20;
    max = (max > dx) ? (max) : (dx);
    max = (max > dy) ? (max) : (dy);
    max = (max > dz) ? (max) : (dz);
    return max;
}

inline double STL::getLongestCartesianLength() {
    Extent domain = findDomain();
    return getLongestCartesianLength(domain);
}

inline void STL::translateCenterToOrigin(const Extent &d) {
    double x_mid = 0.5 * (d.lo[0] + d.hi[0]);
    double y_mid = 0.5 * (d.lo[1] + d.hi[1]);
    double z_mid = 0.5 * (d.lo[2] + d.hi[2]);

    for(auto &f : facets){
        for(int i = 0; i < 3; i++){
            f[i][0] -= x_mid;
            f[i][1] -= y_mid;
            f[i][2] -= z_mid;
        }
    }
}

inline void STL::translateCenterToOrigin() {

    auto d = findDomain();
    return translateCenterToOrigin(d);
}

inline void STL::loadBinaryFile(std::string fileName) {

    Reader reader(fileName);
    facets = reader.readFacets();
}

inline void STL::writeAsciiFile(std::string fname, std::string solidName) const {

    fname += ".stl";
    FILE *fp = fopen(fname.c_str(), "w");
    assert(fp != NULL);
    printf("\nWriting stl to <%s>", fname.c_str());
    fflush(stdout);

    fprintf(fp, "solid %s", solidName.c_str());
    for (unsigned int i = 0; i < facets.size(); i++) {
        fprintf(fp, "\nfacet normal %lf %lf %lf", facets[i].normal[0], facets[i].normal[1], facets[i].normal[2]);
        fprintf(fp, "\nouter loop");
        fprintf(fp, "\nvertex %lf %lf %lf", facets[i][0][0], facets[i][0][1], facets[i][0][2]);
        fprintf(fp, "\nvertex %lf %lf %lf", facets[i][1][0], facets[i][1][1], facets[i][1][2]);
        fprintf(fp, "\nvertex %lf %lf %lf", facets[i][2][0], facets[i][2][1], facets[i][2][2]);
        fprintf(fp, "\nendloop\nendfacet");
    }
    fprintf(fp, "\nendsolid %s", solidName.c_str());

    fclose(fp);
}

inline Parfait::Extent<double> STL::findDomain() const {
    Extent domain(Point(20e20, 20e20, 20e20), Point(-20e20, -20e20, -20e20));

    for (auto &facet : facets) {
        for (int i = 0; i < 3; i++) {
            domain.lo[i] = (domain.lo[i] < facet[0][i]) ? (domain.lo[i]) : (facet[0][i]);
            domain.lo[i] = (domain.lo[i] < facet[1][i]) ? (domain.lo[i]) : (facet[1][i]);
            domain.lo[i] = (domain.lo[i] < facet[2][i]) ? (domain.lo[i]) : (facet[2][i]);

            domain.hi[i] = (domain.hi[i] > facet[0][i]) ? (domain.hi[i]) : (facet[0][i]);
            domain.hi[i] = (domain.hi[i] > facet[1][i]) ? (domain.hi[i]) : (facet[1][i]);
            domain.hi[i] = (domain.hi[i] > facet[2][i]) ? (domain.hi[i]) : (facet[2][i]);
        }
    }
    return domain;
}

}
}