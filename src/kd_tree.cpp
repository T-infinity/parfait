#ifdef PARFAIT_WITH_ANN
#include "kd_tree.h"
#include <stdexcept>

KdTree::KdTree()
{
	printf("Empty constructor called\n");
	fflush(stdout);
	nPts = 0;
	dim = 0;
	previousK = 0;
	nnIdx = NULL;
	dists = NULL;
	dataPts = NULL;
	queryPt = NULL;
	tree = NULL;
}

KdTree::KdTree(int numberOfPoints,double *dataPoints,int dimensions)
	:nPts(numberOfPoints), dim(dimensions)
{
	nnIdx = new ANNidx[1];
	dists = new ANNdist[1];
	previousK = 1;
	dataPts = annAllocPts(nPts,dim);
	queryPt = annAllocPt(dim);
	for(int i=0;i<nPts;i++)
		for(int j=0;j<dim;j++)
			dataPts[i][j] = dataPoints[i*dim+j];
	tree = new ANNkd_tree(dataPts,nPts,dim);
}

KdTree::KdTree(const KdTree &obj)
{
    throw std::domain_error("KdTree: these trees can be quite large, so you probably don't want\nto call the copy constructor.  You probably want to use the move constructor instead.\n");
}

KdTree::KdTree(KdTree &&other)    // move constructor
	:nPts(other.nPts),
	dim(other.dim),
	previousK(other.previousK),
	dataPts(other.dataPts),
	queryPt(other.queryPt),
	nnIdx(other.nnIdx),
	dists(other.dists),
	tree(other.tree)
{
	other.nPts = 0;
	other.dim = 0;
	other.dataPts = NULL;
	other.queryPt = NULL;
	other.nnIdx = NULL;
	other.dists = NULL;
	other.tree = NULL;
}

KdTree::~KdTree()
{
	if(dataPts != NULL) annDeallocPts(dataPts);
	if(queryPt != NULL) annDeallocPt(queryPt);
	delete tree;
	delete[] nnIdx;
	delete[] dists;
	annClose();
}

void KdTree::findNearestNeighbors(int k,double *queryPoint,int *nbrIds,double *distances)
{
	// default to exact search
	findNearestNeighbors(k,queryPoint,nbrIds,distances,0.0);
}

// approximate nbr search (fuzz = wiggle room for the search)
void KdTree::findNearestNeighbors(int k,double *queryPoint,int *nbrIds,double *distances,double fuzz)
{
	for(int i=0;i<dim;i++)
		queryPt[i] = queryPoint[i];
	if(k > previousK)
	{
		delete[] nnIdx;
		delete[] dists;
		nnIdx = new ANNidx[k];
		dists = new ANNdist[k];
	}
	tree->annkSearch(queryPt,k,nnIdx,dists,fuzz);
	for(int i=0;i<k;i++)
	{
		nbrIds[i] = nnIdx[i];
		distances[i]  = dists[i];
	}
}
#endif
