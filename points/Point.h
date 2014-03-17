#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <time.h>
#include <cstring>
#include <cmath>
#include "debug.h"

template <typename C, typename D>
struct Point
{
  C* coord_ = nullptr;
  D distance_ = -1;
  int n_ = 0;
  Point() {}
  ~Point()
  {
    if (coord_)
      delete coord_;
  }
  bool calculate_distance(Point<C, D>* p2)
  {
    if (distance_ != -1)
      return false;
    distance_ = 0;
    for (int i = 0; i < n_; ++i)
      distance_ += (coord_[i] - p2->coord_[i]) * (coord_[i] - p2->coord_[i]);
    distance_ = std::sqrt(distance_);
    return true;
  }
  void operator() (int n, C from, C to)
  {
    n_ = n;
    coord_ = new C[n];
    unsigned seed;
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::uniform_int_distribution<C> distribution(from, to);
    for (int i = 0; i < n_; ++i)
      coord_[i] = distribution(generator);
  }
};

template <typename C, typename D>
void print(Point<C, D>* p)
{
  Debug("Distance: ");
  Debug(p->distance_);
  Debug("\t at ");
  for (int j = 0; j < p->n_; ++j)
    Debug( p->coord_[j] );
  Debug("\n");
}

template <typename C, typename D>
std::vector< Point<C, D> >* create_points(int num, int r_n, C from, C to)
{
  std::vector< Point<C, D> >* result = new std::vector< Point<C, D> >(num);
  for (int i = 0; i < num; ++i)
    (*result)[i](r_n, from, to);
  return result;
}
