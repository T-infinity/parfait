#include <mpi.h>
#include "parfait_preprocessor.h"

int main(int argc, char *argv[]){
    MPI_Init(&argc,&argv);
    parfait_create_grid_system("sample.xml");

    parfait_cleanup();
    MPI_Finalize();
}