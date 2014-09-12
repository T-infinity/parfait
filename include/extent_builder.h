#ifndef EXTENT_BUILDER_H
#define EXTENT_BUILDER_H

#include "extent.h"

namespace ExtentBuilder
{
	// Build an extent box around an entire mesh, or any
	// object that has points in it.
	//
	// In other words, you can pass any object that has
	// the following 2 member functions:
	//
	// 1. int numberOfNodes();
	// 2. void getNode(int,double[3]);
	template<typename T>
	Extent build(const T &objectWithPointsInIt);

    // build an extent around a point like object
    // must be able to access object with [] operators 
    // to get x, y, and z coords.
    template <typename P>
    Extent build(const P &p, double distance);






	//------------Implementation---------------------
	template<typename T>
	Extent build(const T &objectWithPointsInIt)
	{
		double p[3];
		objectWithPointsInIt.getNode(0,p);
		Point min(p),max(p);
		int n = objectWithPointsInIt.numberOfNodes();
		for(int i=1;i<n;i++)
		{
			objectWithPointsInIt.getNode(i,p);
			for(int j=0;j<3;j++)
			{
				if(p[j] < min[j])
					min[j] = p[j];
				else if(p[j] > max[j])
					max[j] = p[j];
			}
		}
		return Extent(min,max);
	}

    template <typename P>
        Extent build(const P &p, double dist){
            Point min(p), max(p);
            min[0] -= dist;
            min[1] -= dist;
            min[2] -= dist;

            max[0] += dist;
            max[1] += dist;
            max[2] += dist;
            return Extent(min, max);
        }

}


#endif
