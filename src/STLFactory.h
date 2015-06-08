#ifndef STLFACTORY_H
#define STLFACTORY_H


#include "stl.h"
namespace Parfait {

    namespace STLFactory {
        STL getUnitCube();
        STL getLShapedSurface();
    };

    inline STL STLFactory::getLShapedSurface() {
        STL stl;
        std::vector <Point> points(6);
        points[0] = {0, 0, 0};
        points[1] = {1, 0, 0};
        points[2] = {1, 1, 0};
        points[3] = {0, 1, 0};
        points[4] = {1, 0, 1};
        points[5] = {1, 1, 1};

        stl.facets.push_back({points[0], points[1], points[2]});
        stl.facets.push_back({points[0], points[2], points[3]});
        stl.facets.push_back({points[1], points[4], points[5]});
        stl.facets.push_back({points[1], points[5], points[2]});
        return stl;
    }

    inline STL STLFactory::getUnitCube() {
        STL cube;

        Facet bottom1;
        bottom1[0] = {0, 0, 0};
        bottom1[1] = {1, 1, 0};
        bottom1[2] = {1, 0, 0};
        cube.facets.push_back(bottom1);

        Facet bottom2;
        bottom2[0] = {0, 0, 0};
        bottom2[1] = {0, 1, 0};
        bottom2[2] = {1, 1, 0};
        cube.facets.push_back(bottom2);

        Facet front1;
        front1[0] = {0, 0, 0};
        front1[1] = {1, 0, 0};
        front1[2] = {1, 0, 1};
        cube.facets.push_back(front1);

        Facet front2;
        front2[0] = {0, 0, 0};
        front2[1] = {1, 0, 1};
        front2[2] = {0, 0, 1};
        cube.facets.push_back(front2);

        Facet right1;
        right1[0] = {1, 0, 0};
        right1[1] = {1, 1, 0};
        right1[2] = {1, 1, 1};
        cube.facets.push_back(right1);

        Facet right2;
        right2[0] = {1, 0, 0};
        right2[1] = {1, 1, 1};
        right2[2] = {1, 0, 1};
        cube.facets.push_back(right2);

        Facet back1;
        back1[0] = {0, 1, 0};
        back1[1] = {0, 1, 1};
        back1[2] = {1, 1, 1};
        cube.facets.push_back(back1);

        Facet back2;
        back2[0] = {0, 1, 0};
        back2[1] = {1, 1, 1};
        back2[2] = {1, 1, 0};
        cube.facets.push_back(back2);

        Facet left1;
        left1[0] = {0, 0, 0};
        left1[1] = {0, 1, 1};
        left1[2] = {0, 1, 0};
        cube.facets.push_back(left1);

        Facet left2;
        left2[0] = {0, 0, 0};
        left2[1] = {0, 0, 1};
        left2[2] = {0, 1, 1};
        cube.facets.push_back(left2);

        Facet top1;
        top1[0] = {0, 0, 1};
        top1[1] = {1, 0, 1};
        top1[2] = {1, 1, 1};
        cube.facets.push_back(top1);

        Facet top2;
        top2[0] = {0, 0, 1};
        top2[1] = {1, 1, 1};
        top2[2] = {0, 1, 1};
        cube.facets.push_back(top2);


        return cube;
    }
}

#endif
