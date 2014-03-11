#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <time.h>
#include <cstring>
#include <cmath>
#include "List.h"
#include "debug.h"

template <typename C, typename D>
struct Point
{
  C* coord_;
  D distance_;
  int n_;
  Point()
  {
    n_ = 0;
    distance_ = -1;
    coord_ = NULL;
  }
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
struct Result
{
  int n_;
  int min_;
  int size_;
  int* data_;
  Result()
  {
    n_ = 0;
    size_ = 0;
    min_ = -1;
    data_ = NULL;
  }
  Result(int n)
  {
    n_ = n;
    data_ = new int[n_];
    size_ = 0;
    min_ = -1;
  }
  ~Result()
  {
    if (data_)
      delete data_;
  }
  void check(Point<C, D>* tmp, int index)
  {
  }
  void operator() (int n)
  {
    n_ = n;
    data_ = new int[n_];
  }
};

template <typename C, typename D>
void print(Point<C, D>* p);

template <typename C, typename D>
std::vector< Point<C, D> >* create_points(int num, int r_c, C from, C to);

template <typename C, typename D>
struct counter
{
  std::vector< Point<C, D> >* data_;
  Point<C, D>* base_;
  Result<C, D>* result_;
  int current_;
  int stop_;
  int end_of_chunk_;
  counter(std::vector< Point<C, D> >* data, Result<C, D>* result, Point<C, D>* base,
      int* current, int stop, char e_o_c, int n_o_r)
  {
    data_ = data;
    result_ = result;
    base_ = base;
    current_ = current;
    stop_ = stop;
    end_of_chunk_ = e_o_c;
    (*result_)(n_o_r);
  }
  void operator() ()
  {
    while (true)
    {
      if (current_ >= stop_)
        return;
      while(current_ <= end_of_chunk_)
      {
        data_[current_].calculate_distance(base_);
        result_.check(data_[current_].distance_, current_);
        ++current_;
      }
    } 
  }
};

/*
std::vector<int>* share(char** matrix, int rows, char end_of_row)
{
  using namespace std;
  int nthreads = thread::hardware_concurrency();
  Result<int, double>* result = new Result<int, double>[nthreads];
  vector<thread> threads;
  int* current_row = new int(0); 
  for (int i = 0; i < nthreads; ++i)
    threads.push_back(thread( counter(matrix, result[i], current_row, rows, end_of_row) ));
  for (auto& th : threads)
    th.join();
  for (int j = 1; j < nthreads; ++j)
    for (int k = 0; k < 10; ++k)
      result[0][k] += result[j][k];
  vector<int> sum(result[0], result[0] + 10);
  delete [] (*result);
  delete [] result;
  return sum;
}
*/

int main()
{
  typedef Point<int, double> point_id;
  int r_n = 5;
  int points = 66;
  int from = 1;
  int to = 66;
  int number_of_points = 7;
  point_id base;
  base(r_n, from, to);
  std::vector< point_id >* test;
  Debug("Create point vector\n");
  test = create_points<int, double> (points, r_n, from, to);
  List< point_id* > result(number_of_points);
  for (auto& i : *test)
  {
    i.calculate_distance(&base);
    result.insert(&i);
  }
  Debug("Print point vectors\n");
  for (List< point_id* >::iterator it = result.begin();  it != result.end(); ++it)
    print<int, double>(*it);
  Debug(result.size_);
  Debug("The End.");
  return 0;
}

template <typename C, typename D>
void print(Point<C, D>* p)
{
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
