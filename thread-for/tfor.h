#include <vector>
#include "distribution1.h"
#include "distribution2.h"

//!  Thread For. 
/*!
  Function object that takes a Distribution as a template parameter.
  The Distribution defines the container's type.
*/
template <class D>
struct tfor
{
  typedef typename D::C C;
  //! Takes a container and process it using the Distribution passed.
  /*!
    \param container A pointer to a container to process.
    \param nt The number of threads that are going to be used.
  */
  void operator() (C* container, int nt=0)
  {
    D dist(container, nt);
    dist();
  }
};

