#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <MessagePasser.h>
#include <parmetis.h>
#include <metis.h>

struct ParMetisInfo{
    idx_t *vertex_weights = NULL;
    idx_t *edge_weights = NULL;
    idx_t weight_flag = 0; // don't weight vertices or edges
    idx_t numflag  = 0;    // use C indexing, not fortran
    idx_t ncon    = 1;     // Number of weights.....not sure whytf this is 1 instead of 0...
    idx_t options[3] = {0,0,0};
    idx_t edgecut;
    MPI_Comm comm = MPI_COMM_WORLD;
};

inline std::vector<idx_t> getVertexDistributionAcrossProcs(int nproc, const long *proc_node_map);

inline std::vector<idx_t> getAdjacencyMap(const long *ia, int nnodes);

inline std::vector<idx_t> getAdjacency(const long *ia, const long *ja, int nnodes);

inline int getMyNumberOfNodes(int rank, const long *proc_node_map);

inline std::vector<real_t> getTpWeights(idx_t nparts);

void printParmetisInput(const std::vector<idx_t>& vtxdist, const std::vector<idx_t>& xadj,
                        const std::vector<idx_t>& adjncy, const std::vector<real_t>& tpwgts);
inline void partitionMesh(int rank, int nproc,
                          long* proc_node_map,
                          long* ia,
                          long* ja,
                          int* part_vec)
{
    int nnodes = getMyNumberOfNodes(rank, proc_node_map);
    ParMetisInfo parMetisInfo;
    if(0 == rank)
        printf("Sanitizing input to parmetis\n");
    for(int i=0;i<ia[nnodes];i++)
        if(ja[i] < 0)
            throw std::logic_error("Rank "+std::to_string(MessagePasser::Rank())
                                   +" negative id ["+std::to_string(i)+"] of ["
                                    +std::to_string(nnodes)+"]");

    idx_t number_of_partitions = MessagePasser::NumberOfProcesses();

    auto vtxdist = getVertexDistributionAcrossProcs(nproc, proc_node_map);
    auto xadj = getAdjacencyMap(ia, nnodes);
    auto adjncy = getAdjacency(ia, ja, nnodes);
    auto tpwgts = getTpWeights(number_of_partitions);
    std::vector<real_t> ubvec(parMetisInfo.ncon,1.05);
	std::vector<idx_t> part(nnodes,0);


    printf("Rank %i: has %i nodes for ParMETIS\n",rank,nnodes);
    printParmetisInput(vtxdist, xadj, adjncy, tpwgts);
	auto metis = ParMETIS_V3_PartKway(vtxdist.data(),
                                      xadj.data(),
                                      adjncy.data(),
                                      parMetisInfo.vertex_weights,
                                      parMetisInfo.edge_weights,
                                      &parMetisInfo.weight_flag,
				                      &parMetisInfo.numflag,
                                      &parMetisInfo.ncon,
                                      &number_of_partitions,
                                      tpwgts.data(),
                                      ubvec.data(),
                                      parMetisInfo.options,
                                      &parMetisInfo.edgecut,
                                      part.data(),
                                      &parMetisInfo.comm);

    if(metis != METIS_OK)
		throw std::logic_error("Metis reported a failure");
	for(int i=0;i<nnodes;i++)
		part_vec[i] = part[i];
    if(0 == rank)
        printf("ParMETIS done...\n");
}
void printParmetisInput(const std::vector<idx_t>& vtxdist, const std::vector<idx_t>& xadj,
                        const std::vector<idx_t>& adjncy, const std::vector<real_t>& tpwgts) {

    for(int r = 0; r < MessagePasser::NumberOfProcesses(); r++){
        if(r == MessagePasser::Rank()){
            printf("\n\n\nRank %d\n", MessagePasser::Rank());
            printf("Vertex Distribution:\n");
            for(auto d : vtxdist)
                printf("%d, ", d);
            printf("\nAdjacency Map:");
            for(auto m : xadj)
                printf("%d, ", m);
            printf("\nAdjacency: ");
            for(auto a : adjncy)
                printf("%d, ", a);
            printf("\nTpWeights:");
            for(auto w : tpwgts)
                printf("%f, ", w);
            fflush(stdout);
        }
        MessagePasser::Barrier();
    }
}

std::vector<real_t> getTpWeights(idx_t nparts) {
    ParMetisInfo parMetisInfo;
    std::vector<real_t> tpwgts(parMetisInfo.ncon*nparts);
    real_t w = 1.0/(real_t)nparts;
    for(int i=0;i< parMetisInfo.ncon*nparts;i++)
		tpwgts[i] = w;
    return tpwgts;
}

int getMyNumberOfNodes(int rank, const long *proc_node_map) { return proc_node_map[rank+1]-proc_node_map[rank]; }

inline std::vector<idx_t> getAdjacency(const long *ia, const long *ja, int nnodes) {
    int ia_size = nnodes + 1;
    std::vector<idx_t> adjncy(ia[ia_size-1]);
    for(int i=0;i<ia[ia_size-1];i++)
		adjncy[i] = ja[i];
    return adjncy;
}

inline std::vector<idx_t> getAdjacencyMap(const long *ia, int nnodes) {
    int ia_size = nnodes + 1;
    std::vector<idx_t> xadj(ia_size);
    for(int i=0;i<ia_size;i++)
		xadj[i] = ia[i];
    return xadj;
}

inline std::vector<idx_t> getVertexDistributionAcrossProcs(int nproc, const long *proc_node_map) {
    std::vector<idx_t> vtxdist(nproc+1);
    for(int i=0;i<=nproc;i++)
		vtxdist[i] = proc_node_map[i];
    return vtxdist;
};
