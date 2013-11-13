#include <vector>
#include "tfor.h"
#include "debug.h"

struct Process
{
  void operator() (char* it)
  {
    Debug(*it);
  }
};

int main()
{
  std::vector<char>* ctnr = NULL;
  ctnr = new std::vector<char>({'a','b','c','d','e','f','g','h'});
  Debug("test\n");
  tfor< Distribution<std::vector<char>, Process> > test;
  test(ctnr);
  Debug("\ntest2\n");
  tfor< Distribution2<std::vector<char>, Process> > test2;
  test2(ctnr);
  delete ctnr;
  Debug("\nthe end\n");
  return 0;
}

