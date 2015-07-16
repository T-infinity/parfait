//
// Created by Cameron T Druyor Jr on 7/1/15.
//

#include <vector>
#include <MessagePasser.h>
#include "mpi_compare_tols.h"

#include "CppUTest/CommandLineTestRunner.h"

using namespace MessagePasser;

TEST_GROUP(Gatherv){
  void setup(){
  }
  void teardown(){
  }
};

TEST(Gatherv,TestWithIntegers){
  // test Gatherv for integers
  int root = 0;
  std::vector<int> send_vec;
  std::vector<int> recv_vec;
  std::vector<int> map;
  int num = 0;
  // even and odd procs have size 2 & 3 vectors respectively
  if(Rank() % 2 == 0)
    num = 2;
  else
    num = 3;
  for(int i=0;i<num;i++)
    send_vec.push_back(Rank()+i);
  Gatherv(send_vec,recv_vec,map,root);
  if(Rank() == root) {
    LONGS_EQUAL(NumberOfProcesses()+1,(int)map.size());
    LONGS_EQUAL(map.back(),(int)recv_vec.size());
    for(int i=0;i<NumberOfProcesses();i++) {
      int counter = 0;
      for(int j=map[i];j<map[i+1];j++) {
        LONGS_EQUAL(i+counter,recv_vec[j]);
        ++counter;
      }
    }
  }

}

TEST(Gatherv,TestWithIntegersNoMapPassedIn){
  int root = 0;
  std::vector<int> send_vec;
  std::vector<int> recv_vec;
  int num = 0;
  // even and odd procs have size 2 & 3 vectors respectively
  if(Rank() % 2 == 0)
    num = 2;
  else
    num = 3;
  for(int i=0;i<num;i++)
    send_vec.push_back(Rank()+i);
  Gatherv(send_vec,recv_vec,root);
  // just make sure it gets called, since it is just a delegation
}

TEST(Gatherv,TestWithFloats){
  int root = 0;
  std::vector<float> send_vec;
  std::vector<float> recv_vec;
  float junk = 1.7e-5;
  std::vector<int> map;
  int num = 0;
  // even and odd procs have size 2 & 3 vectors respectively
  if(Rank() % 2 == 0)
    num = 2;
  else
    num = 3;
  for(int i=0;i<num;i++)
    send_vec.push_back(junk+(float)(Rank()+i));
  Gatherv(send_vec,recv_vec,map,root);
  if(Rank() == root) {
    LONGS_EQUAL(NumberOfProcesses()+1,(int)map.size());
    LONGS_EQUAL(map.back(),(int)recv_vec.size());
    for(int i=0;i<NumberOfProcesses();i++) {
      int counter = 0;
      for(int j=map[i];j<map[i+1];j++) {
        DOUBLES_EQUAL(junk+(float)(i+counter),recv_vec[j],MPI_FLOAT_TOL);
        ++counter;
      }
    }
  }
}

TEST(Gatherv,TestWithDoubles){
  int root = 0;
  std::vector<double> send_vec;
  std::vector<double> recv_vec;
  double junk = 1.7e-13;
  std::vector<int> map;
  int num = 0;
// even and odd procs have size 2 & 3 vectors respectively
  if(Rank() % 2 == 0)
    num = 2;
  else
    num = 3;
  for(int i=0;i<num;i++)
    send_vec.push_back(junk+(double)(Rank()+i));
  Gatherv(send_vec,recv_vec,map,root);
  if(Rank() == root) {
    LONGS_EQUAL(NumberOfProcesses()+1,(int)map.size());
    LONGS_EQUAL(map.back(),(int)recv_vec.size());
    for(int i=0;i<NumberOfProcesses();i++) {
      int counter = 0;
      for(int j=map[i];j<map[i+1];j++) {
        DOUBLES_EQUAL(junk+(double)(i+counter),recv_vec[j],MPI_DOUBLE_TOL);
        ++counter;
      }
    }
  }
}

TEST(Gatherv,TestWhenAllVectorsAreEmpty){
  int root = 0;
  std::vector<int> send_vec;
  std::vector<int> recv_vec;
  std::vector<int> map;
  Gatherv(send_vec,recv_vec,map,root);
  if(Rank() == root) {
    LONGS_EQUAL(NumberOfProcesses()+1,(int)map.size());
    LONGS_EQUAL(0,(int)recv_vec.size());
    LONGS_EQUAL(0,map.back());
  }
}

TEST(Gatherv,TestGatheringAsVectorOfVectors){
  int root = 0;
  std::vector<double> send_vec;
  std::vector<std::vector<double>> result;
  double junk = 1.7e-13;
  std::vector<int> map;
  int num = 0;
// even and odd procs have size 2 & 3 vectors respectively
  if(Rank() % 2 == 0)
    num = 2;
  else
    num = 3;
  for(int i=0;i<num;i++)
    send_vec.push_back(junk+(double)(Rank()+i));
  Gatherv(send_vec, result,root);
  if(Rank() == root) {
    LONGS_EQUAL(NumberOfProcesses(),(int) result.size());
    for(int i=0;i<NumberOfProcesses();i++) {
      int counter = 0;
      LONGS_EQUAL(i%2==0?2:3,result[i].size());
      for(int j=0;j< result[i].size();j++) {
        DOUBLES_EQUAL(junk+(double)(i+counter),result[i][j],MPI_DOUBLE_TOL);
        ++counter;
      }
    }
  }
}