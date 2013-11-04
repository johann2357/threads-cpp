#include "distribution1.h"
#include "distribution2.h"

template <class D>
struct tfor
{
  typedef typename D::C C;
  void operator() (C* container, int nt=0)
  {
    D dist(container, nt);
    dist();
  }
};

