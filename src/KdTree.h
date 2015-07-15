#ifdef PARFAIT_WITH_ANN
#ifndef KD_TREE_H
#define KD_TREE_H

#include "ANN/ANN.h"

class KdTree
{
	public: 
		KdTree(int numberOfPoints,double *dataPoints,int dimensions);
		KdTree();
		KdTree(const KdTree &obj); // copy constructor
		KdTree(KdTree &&obj); // move constructor
		// find k nearest nbrs and the squared distance to each
		void findNearestNeighbors(int k,double *queryPoint,int *nbrIds,double *distances);
		void findNearestNeighbors(int k,double *queryPoint,int *nbrIds,double *distances,double fuzz);
		~KdTree();
	private:
	bool isEmpty;
	int nPts;
	int dim;
	int previousK;
	ANNpointArray  	dataPts;
	ANNpoint		queryPt;
	ANNidxArray 	nnIdx;
	ANNdistArray 	dists;
	ANNkd_tree*		tree;
};

#endif 
#endif
