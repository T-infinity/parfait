#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <stdio.h>
#define CHECKPOINT fprintf(stderr, "CHECKPOINT: %s %d\n", __FILE__, __LINE__);

#define PARALLEL_CHECKPOINT fprintf(stderr, "RANK %d, CHECKPOINT: %s %d\n", MessagePasser::Rank(), __FILE__, __LINE__);

#include <mpi.h>
#define CHECK_MPI_CLEAN    int incoming_exists = false; \
                           MPI_Status status; \
                           int rank; \
                           MPI_Comm_rank(MPI_COMM_WORLD, &rank); \
                           MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &incoming_exists, &status); \
                           if(incoming_exists) {\
                               fprintf(stderr, "RANK %d, Detected non-clean incomming MPI message. %s %d",rank, __FILE__,__LINE__); \
                               abort(); \
                           }

#endif
