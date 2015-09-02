#include <vector>
#include <MessagePasser.h>


using namespace MessagePasser;
#if 0
TEST_GROUP(AllGatherv){
    void setup(){
    }
    void teardown(){
    }
};

TEST(AllGatherv,TestWithIntegers){
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
  AllGatherv(send_vec,recv_vec,map);
  LONGS_EQUAL(NumberOfProcesses()+1,(int)map.size());
  LONGS_EQUAL(map.back(),(int)recv_vec.size());
  for(int i=0;i<NumberOfProcesses();i++){
    int counter = 0;
    for(int j=map[i];j<map[i+1];j++){
      LONGS_EQUAL(i+counter,recv_vec[j]);
      ++counter;
    }
  }
}

TEST(AllGatherv,TestWithNoMapPassedFromRoot){
  int root = 0;
  std::vector<int> send_vec;
  std::vector<int> recv_vec;
  std::vector<int> map(NumberOfProcesses()+1);
  int num = 0;
  // even and odd procs have size 2 & 3 vectors respectively
  if(Rank() % 2 == 0)
    num = 2;
  else
    num = 3;
  for(int i=0;i<num;i++)
    send_vec.push_back(Rank()+i);
  AllGatherv(send_vec,recv_vec);

  // set up map
  for(int i=1;i<NumberOfProcesses()+1;i++){
    num = (i-1)%2==0 ? 2 : 3;
    map[i] = map[i-1] + num;
  }
  LONGS_EQUAL(NumberOfProcesses()+1,(int)map.size());
  LONGS_EQUAL(map.back(),(int)recv_vec.size());
  for(int i=0;i<NumberOfProcesses();i++)
  {
    int counter = 0;
    for(int j=map[i];j<map[i+1];j++)
    {
      LONGS_EQUAL(i+counter,recv_vec[j]);
      ++counter;
    }
  }
}

TEST(AllGatherv,TestWithVectorOfVectors)

{
  int root = 0;
  std::vector <int> send_vec;
  std::vector <std::vector<int>> result;
  std::vector <int> map(NumberOfProcesses() + 1);
  int num = 0;
  // even and odd procs have size 2 & 3 vectors respectively
  if (Rank() % 2 == 0)
    num = 2;
  else
    num = 3;
  for (int i = 0; i < num; i++)
    send_vec.push_back(Rank() + i);
  AllGatherv(send_vec, result);
  for(int i=0;i<NumberOfProcesses();i++){
    if(i % 2 == 0) {
      LONGS_EQUAL(2, result[i].size());
      for(int j=0;j<2;j++)
        LONGS_EQUAL(i+j, result[i][j]);
    }
    else{
      LONGS_EQUAL(3, result[i].size());
      for(int j=0;j<3;j++)
        LONGS_EQUAL(i+j,result[i][j]);
    }

  }
}
#endif
