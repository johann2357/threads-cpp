#include <thread>
#include <mutex>

//! Mutex that is used by FO2
std::mutex mtx;

//!  Function object that each thread will used. 
/*!
  The Distribution will call it.
  Takes an iterator I and a process P that will use it.
  Each thread has one iterator to process it and the end of the container.
*/
template <class I, class P>
struct FO2
{
  //! Iterator pointing to the past-the-end element of the container.
  I end_;
  //! Keeps track the current container's iterator that is processing.
  I current_;
  //! Point to the global container's iterator.
  /*!
    All threads use it to get the next element to process or to know 
    if its job is done if its equal to end_.
  */
  I* it_;
  //! Constructor that sets the global and the end container's iterator.
  /*!
    \param it Points to the global container's iterator.
    \param end Point past-the-end container's iterator.
  */
  FO2(I* it, I end): it_(it), end_(end) {}
  //! Process each element of the container, one at a time.
  /*!
    It uses a mutex mtx to lock to copy the current iterator 
    and to increment it.
  */
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

//! Distribution that creates a global iterator used by all threads.
/*!
  It should be used when each element of the container
  will take different amount of time to process it.
  It should also be used when the size of the container is unknown.
*/
template <class _C, class _P>
struct Distribution2
{
  typedef _C C;
  typedef _P P;
  //! The number of threads to be used.
  int nthreads_;
  //! A pointer to the container.
  C* container_;
  //! Sets the container's pointer and the number of threads.
  /*!
    \param ctnr The container's pointer.
    \param nt By default use all the threads available.
  */
  Distribution2(C* ctnr, int nt=0): nthreads_(nt), container_(ctnr)
  {
    if (! nthreads_)
      nthreads_ = std::thread::hardware_concurrency();
  }
  //! Executes the Distribution and joins all the threads.
  /*!
    It uses the FO2 to create the threads.
  */
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

