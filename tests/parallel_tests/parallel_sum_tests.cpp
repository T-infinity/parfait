//
// Created by Cameron T Druyor Jr on 6/9/15.
//

#include "MessagePasser.h"
#include "mpi_compare_tols.h"
#include "catch.hpp"

using namespace MessagePasser;

TEST_CASE("SumIntegers"){
  int root = 0;
  int value = Rank();
  int psum = 0;
  psum = ParallelSum(value,root);
  if(Rank() == root)
  {
    int sum = 0;
    for(int i=0;i<NumberOfProcesses();i++)
      sum += i;
    REQUIRE(sum == psum);
  }
}
#if 0
TEST(ParallelSumTests,SumFloats)
{
  int root = 0;
  float junk = 1.7e-1;
  float value = junk + (float)Rank();
  float psum = 0.0;
  psum = ParallelSum(value,root);
  if(Rank() == root)
  {
    float sum = 0.0;
    for(int i=0;i<NumberOfProcesses();i++)
      sum += junk + (float)i;
    DOUBLES_EQUAL(sum,psum,MPI_FLOAT_TOL);
  }
}

TEST(ParallelSumTests,SumDoubles)
{
  int root = 0;
  double junk = 1.7e-9;
  double value = junk + (double)Rank();
  double psum = 0.0;
  psum = ParallelSum(value,root);
  if(Rank() == root)
  {
    double sum = 0.0;
    for(int i=0;i<NumberOfProcesses();i++)
      sum += junk + (double)i;
    DOUBLES_EQUAL(sum,psum,MPI_DOUBLE_TOL);
  }
}
#endif
