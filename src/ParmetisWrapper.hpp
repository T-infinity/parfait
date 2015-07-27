#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef PARFAIT_WITH_PARMETIS

#include <MessagePasser.h>

inline void PartitionMesh(int rank,int nproc,
		int *proc_node_map,
		int *ia,
		int *ja,
		int *part_vec)
{
	if(rank == 0)
		printf("Setting up input for ParMetis\n");
	idx_t *vtxdist = NULL;
	idx_t *xadj    = NULL;
	idx_t *adjncy  = NULL;
	idx_t *vwgt   = NULL;
	idx_t *adjwgt = NULL;
	idx_t wgtflag  = 0;
    idx_t numflag  = 0;
	idx_t ncon    = 1;
	idx_t nparts   = nproc;
	real_t *tpwgts = NULL;
	real_t *ubvec  = NULL;
	idx_t options[3];
	idx_t edgecut;
	idx_t *part    = NULL;
	MPI_Comm comm = MPI_COMM_WORLD;
	options[0] = options[1] = options[2] = 0;

	// set up vertex distribution
	vtxdist = (idx_t*)malloc((nproc+1)*sizeof(idx_t));
	assert(vtxdist != NULL);
	for(int i=0;i<=nproc;i++)
		vtxdist[i] = proc_node_map[i];
	int nnodes = proc_node_map[rank+1]-proc_node_map[rank];
	int ia_size = nnodes+1;
	// conveniently copy ia into xadj and ja into adjncy
	xadj = (idx_t*)malloc(ia_size*sizeof(int));
	assert(xadj != NULL);
	adjncy = (idx_t*)malloc(ia[ia_size-1]*sizeof(idx_t));
	assert(adjncy != NULL);
	for(int i=0;i<ia_size;i++)
		xadj[i] = ia[i];
	for(int i=0;i<ia[ia_size-1];i++)
		adjncy[i] = ja[i];
	// set up tpwgts
	tpwgts = (real_t*) malloc(ncon*nparts*sizeof(real_t));
	assert(tpwgts != NULL);
	real_t w = 1.0/(real_t)nparts;
	for(int i=0;i<ncon*nparts;i++)
		tpwgts[i] = w;
	
	// set up ubvec
	ubvec = (real_t*) malloc(ncon*sizeof(real_t));
	assert(ubvec != NULL);
	for(int i=0;i<ncon;i++)
		ubvec[i] = 1.05;
	
	// allocate space for part vector
	part = (idx_t*)malloc(nnodes*sizeof(idx_t));
	assert(part!=NULL);
	for(int i=0;i<nnodes;i++)
		part[i] = 0;

	int metis = 0;
    printf("Rank %i: has %i nodes for ParMETIS\n",rank,nnodes);
	metis = ParMETIS_V3_PartKway(vtxdist,xadj,adjncy,vwgt,adjwgt,&wgtflag,
				&numflag,&ncon,&nparts,tpwgts,ubvec,options,&edgecut,part,&comm);
	if(metis == METIS_OK && rank == 0)
		printf("METIS Ok!\n");

	// load part vector from ParMETIS into part_vec
	for(int i=0;i<nnodes;i++)
		part_vec[i] = part[i];
	free(ubvec);
	free(part);
	free(tpwgts);
	free(vtxdist);
	free(xadj);
	free(adjncy);
		
}	
#else
void PartitionMesh(int rank,int nproc,
		int *proc_node_map,
		int *ia,
		int *ja,
		int *part_vec)
{
	if(nproc>1)
	{
		printf("Parfait Error: Must link against ParMETIS if partitioning meshes.\n");
	}
}

#endif
