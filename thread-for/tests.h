#include <fstream>
#include <list>
#include <chrono>
#include "tfor.h"
#include "debug.h"

#define MAX_MOD 1000

struct Process
{
  void operator() (int* it)
  {
    int x = 1;
    for (int i = 0; i < MAX_MOD; ++i)
      ++x;
  }
};

struct Process2
{
  void operator() (int* val)
  {
    long long sum = 0;
    for (int i = 1; i <= (*val)%MAX_MOD; i++)
       sum += (*val) % i;
  }
};

struct UnitTest
{
  void test_dist1(std::list<int>* ctnr)
  {
    Debug("test dist 1\n");
    using namespace std;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    //
    tfor< Distribution<std::list<int>, Process2> > test;
    test(ctnr);
    //
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    Debug("It took me ") ;
    Debug(chrono::duration_cast<chrono::nanoseconds>(t2-t1).count()*0.000000001);
    Debug(" seconds.\n");
  }
  void test_dist2(std::list<int>* ctnr)
  {
    Debug("test dist 2\n");
    using namespace std;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    //
    tfor< Distribution2<std::list<int>, Process2> > test;
    test(ctnr);
    //
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    Debug("It took me ") ;
    Debug(chrono::duration_cast<chrono::nanoseconds>(t2-t1).count()*0.000000001);
    Debug(" seconds.\n");
  }
  void run()
  {
    Debug("loading data_long.dat ... ");
    std::ifstream fin("data_long.dat");
    std::list<int> data;
    while(!fin.eof())
    {
      int val;
      fin>>val;
      data.push_back(val);
    }
    Debug("done\n");
    Debug("MAX_MOD = " << MAX_MOD << "\n");
    test_dist1(&data);
    test_dist2(&data);
    Debug("\nthe end\n");
  }
};

