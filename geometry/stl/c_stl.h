#ifndef __C_STL_H
#define __C_STL_H

#include <string>
#include <vector>
#include <Point.h>
#include <stdint.h>
#include "adt.h"

using std::string;
using std::vector;

#define STL_NEAREST_EXACT  1
#define STL_NEAREST_APPROX 2


/**
 * @brief An stl facet datatype.  Contains the points and normal vector of the stl facet.
 */

class c_STL_FACET {

    public:
        Point p1, p2, p3;
        Point n;

        void GetExtent(double min_spacing, double extent[6]);
        bool WhereDoesEdgeIntersect(Point &edgelo, Point &edgehi, Point &point);

        bool DoesRayIntersect(double orig[3], double dir[3], double &t);
        void TestRayIntersection();

        Point GetClosestPoint_Approx(Point point, double &dist);
        Point GetClosestPoint_Exact (Point point, double &dist);

        Point &operator [] (int i);
        double GetXMin();
        double GetYMin();
        double GetZMin();
        double GetXMax();
        double GetYMax();
        double GetZMax();
    private:
        double GetMax(int i);
        double GetMin(int i);
};

/**
 * @brief A class for maintaining stl information.
 */
class c_STL {
    public:
        c_STL():TreeIsInit(false), tree(ADT_3D_EXTENT){}

        string filename; 
        vector<c_STL_FACET> facets;
        Adt tree;

        unsigned long  size() {return facets.size();}
        void    LoadFileB (string fname);
        void    LoadFileA (string fname);
        void    WriteFileA(string fname, string solid_name);
        void    Center();
        void    Rescale();
        void    Rescale(double scale);
        void    Translate(double shift[3]);
        void    InitTree(double extent[6]);
        double  MaxDistance();

        //--- Selects the nearest vertex of stl on the geometry.
        Point   GetClosestPoint_Approx(Point &point, double &dist, double seed_distance = 0.001);
        Point   GetClosestPoint_Exact (Point &point, double &dist, double seed_distance = 0.001);
        Point   LoopClosest(Point &point, double &dist, int flag);
        bool    GetRayIntersectionPoint(Point orig, Point &dir, Point &intersection_point);
        int     ReadyForSearching(){return TreeIsInit;}
    private:
        bool    TreeIsInit;
        double  domain_size;

        void    GetExtentsInsideExtent(std::vector<int> &inside, double extent[6]);
        Point   GetClosestPoint(Point &point, double &dist, int flag);

};







#endif

