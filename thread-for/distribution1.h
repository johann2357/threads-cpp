#include <thread>

//!  Function object that each thread will used. 
/*!
  The Distribution will call it.
  Takes an iterator I and a process P that will use it.
  Each thread has a chunk of the container to process it.
*/
template <class I, class P>
struct FO
{
  //! Iterator pointing to the begining its chunk.
  I begin_;
  //! Iterator referring to the past-the-end element of its chunk. 
  I end_;
  //! Constructor that set the begining and the end of its chunk.
  FO(I begin, I end): begin_(begin), end_(end) {}
  //! Process each element of its chunk.
  void operator() ()
  {
    for (I it = begin_; it != end_; ++it)
    {
      P process;
      process(&(*it));
    }
  }
};

//! Distribution that splits the container into chunks to process it.
/*!
  It should be used when each element of the container
  will take similar amount of time to process it.
*/
template <class _C, class _P>
struct Distribution
{
  typedef _C C;
  typedef _P P;
  //! The number of threads to be used.
  int nthreads_;
  //! The size of each chunk.
  int part_;
  //! A pointer to the container.
  C* container_;
  //! Sets the container's pointer and the number of threads.
  /*!
    \param ctnr The container's pointer.
    \param nt By default use all the threads available.
  */
  Distribution(C* ctnr, int nt=0): nthreads_(nt), container_(ctnr)
  {
    if (! nthreads_)
      nthreads_ = std::thread::hardware_concurrency();
    part_ = container_->size()/nthreads_ + 1;
  }
  //! Executes the Distribution and joins all the threads.
  /*!
    It uses the FO to create the threads.
  */
  void operator() ()
  {
    std::vector<std::thread> threads;
    typename C::iterator start = container_->begin();
    typename C::iterator end =  start;
    bool not_finished = true;
    for (int i = 0; ((i < nthreads_) && not_finished); ++i)
    {
      for (int k = 0; ((k < part_) && not_finished); ++k)
        if (end != container_->end())
          ++end;
        else
          not_finished = false;
      threads.push_back(std::thread(FO< typename C::iterator,P >(start, end)));
      start = end;
    }
    for (auto& th : threads)
      th.join();
  }
};

