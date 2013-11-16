#include <vector>
#include "tfor.h"
#include "debug.h"

struct Process
{
  void operator() (int* it)
  {
    int x = 1;
    for (int i = 0; i < 9999; ++i)
      for (int j = 0; j < 999; ++j)
        ++x;
  }
};

struct Process2
{
  void operator() (int* it)
  {
    int x = 1;
    for (int i = 0; i < 999; ++i)
      for (int j = 0; j < ((*it)*9); ++j)
        ++x;
  }
};

void test_dist1(std::vector<int>* ctnr)
{
  Debug("test dist 1\n");
  tfor< Distribution<std::vector<int>, Process> > test;
  test(ctnr);
}

void test_dist2(std::vector<int>* ctnr)
{
  Debug("test dist 2\n");
  tfor< Distribution2<std::vector<int>, Process2> > test;
  test(ctnr);
}

int main()
{
  std::vector<int>* ctnr = NULL;
  ctnr = new std::vector<int>;
  for (int i = 0; i < 999; ++i)
    ctnr->push_back((i));
  test_dist1(ctnr);
  //test_dist2(ctnr);
  delete ctnr;
  Debug("\nthe end\n");
  return 0;
}

