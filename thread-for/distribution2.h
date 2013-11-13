#include <thread>
#include <mutex>

std::mutex mtx;

template <class I, class P>
struct FO2
{
  I end_;
  I current_;
  I* it_;
  FO2(I* it, I end): it_(it), end_(end) {}
  void operator() ()
  {
    bool proceed = true;
    while(proceed)
    {
      mtx.lock();
      current_ = *it_;
      if ((*it_) != end_)
        ++(*it_);
      else 
        proceed = false;
      mtx.unlock();
      if (proceed)
      {
        P process;
        process(&(*current_));
      }
    }
  }
};

template <class _C, class _P>
struct Distribution2
{
  typedef _C C;
  typedef _P P;
  int nthreads_;
  C* container_;
  Distribution2(C* ctnr, int nt=0): nthreads_(nt), container_(ctnr)
  {
    if (! nthreads_)
      nthreads_ = std::thread::hardware_concurrency();
  }
  void operator() ()
  {
    std::vector<std::thread> threads;
    typename C::iterator end = container_->end();
    typename C::iterator* current = new typename C::iterator;
    (*current) = container_->begin();
    for (int i = 0; i < nthreads_; ++i)
      threads.push_back(std::thread(FO2<typename C::iterator,P>(current,end)));
    for (auto& th : threads)
      th.join();
    delete current;
  }
};

