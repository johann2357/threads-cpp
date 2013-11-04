#include <thread>

template <class I, class P>
struct FO
{
  I begin_;
  I end_;
  FO(I begin, I end): begin_(begin), end_(end) {}
  void operator() ()
  {
    for (I it = begin_; it != end_; ++it)
    {
      P process;
      process(it);
    }
  }
};

template <class _C, class _P>
struct Distribution
{
  typedef _C C;
  typedef _P P;
  int nthreads_;
  int part_;
  C* container_;
  Distribution(C* ctnr, int nt=0): nthreads_(nt), container_(ctnr)
  {
    if (! nthreads_)
      nthreads_ = std::thread::hardware_concurrency();
    part_ = container_->size()/nthreads_ + 1;
  }
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

