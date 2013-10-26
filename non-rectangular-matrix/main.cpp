#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <time.h>
#include <cstring>
#include <mutex>
#include "debug.h"

std::mutex mtx;

template <typename T>
void fill_random(std::vector<T>* v, int columns, T from, T to);

template <typename T>
void print(std::vector<T>** matrix, int rows);

template <typename T>
std::vector<T>** create_matrix(int r, int min_c, int max_c, T from, T to);

template <typename T>
T** create_empty_matrix(int r, int c);

struct counter
{
  std::vector<char>** matrix_;
  int* result_;
  int* current_;
  int stop_;
  counter(std::vector<char>** m, int* result, int* current, int stop)
  {
    matrix_ = m;
    result_ = result;
    current_ = current;
    stop_ = stop;
  }
  void operator() ()
  {
    while (true)
    {
      int row;
      mtx.lock();
      row = (*current_)++;
      mtx.unlock();
      if (row >= stop_)
        return;
      std::vector<char>::iterator it;
      for (it = matrix_[row]->begin(); it != matrix_[row]->end(); ++it)
        if (((*it) >= 'a') && ((*it) <= 'j'))
          ++result_[(*it) - 'a'];
    } 
  }
};


std::vector<int> share(std::vector<char>** matrix, int rows)
{
  using namespace std;
  chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
  int nthreads = thread::hardware_concurrency();
  int** result = create_empty_matrix<int> (nthreads, 10);
  vector<thread> threads;
  int* current_row = new int(0); 
  for (int i = 0; i < nthreads; ++i)
    threads.push_back(thread( counter(matrix, result[i], 
                                           current_row, rows) ));
  for (auto& th : threads)
    th.join();
  for (int j = 1; j < nthreads; ++j)
    for (int k = 0; k < 10; ++k)
      result[0][k] += result[j][k];
  chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
  cout <<"It took me " ;
  cout <<chrono::duration_cast<chrono::nanoseconds>(t2-t1).count()*0.000000001;
  cout <<" seconds.\n";
  vector<int> sum(result[0], result[0] + 10);
  delete [] (*result);
  delete [] result;
  return sum;
}

std::vector<int> no_threads(std::vector<char>** matrix, int rows)
{
  using namespace std;
  chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
  vector<int> result({0,0,0,0,0,0,0,0,0,0});                               
  for (int i = 0; i < rows; ++i)                                                
    for (auto& current : (*matrix[i]))
    {                                                                           
      if ((current >= 'a') && (current <= 'j'))                                 
        ++result[current - 'a'];                                                
    }
  chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
  cout <<"It took me " ;
  cout <<chrono::duration_cast<chrono::nanoseconds>(t2-t1).count()*0.000000001;
  cout <<" seconds.\n";
  return result;
}  

int main()
{
  int rows = 66;
  int min_columns = 666;
  int max_columns = 999999;
  Debug("Create matrix");
  std::vector<char>** test = NULL;
  test =  create_matrix<char> (rows, min_columns, max_columns, 'a', 'z');
  Debug("Result using threads");
  std::vector<int> result = share(test, rows);
  for (auto& i : result)
    Debug( i );
  Debug("Result no-threads");
  std::vector<int> result2 = no_threads(test, rows);
  for (auto& i : result2)
    Debug( i );
  for (int i = 0; i < rows; ++i)
    delete test[i];
  delete [] test;
  return 0;
}

template <typename T>
void fill_random(std::vector<T>* v, int columns, T from, T to)
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator (seed);
  std::uniform_int_distribution<T> distribution(from, to);
  for (int i = 0; i < columns; ++i)
    v->push_back(distribution(generator));
}

template <typename T>
void print(std::vector<T>** matrix, int rows)
{
  for (int i = 0; i < rows; ++i)
  {
    typename std::vector<T>::iterator j;
    for (j = (*matrix[i]).begin(); j != (*matrix[i]).end(); ++j)
      std::cout << (*j) << ' ';      
    std::cout << std::endl;
  }
}

template <typename T>
std::vector<T>** create_matrix(int r, int min_c, int max_c, T from, T to)
{
  unsigned seed;
  seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(min_c, max_c);
  std::vector<T>** m = new std::vector<T>* [r];
  for (int i = 0; i < r; ++i)
  {
    m[i] = new std::vector<T>;
    fill_random<T>(m[i], distribution(generator), from, to);
  }
  return m;
}

template <typename T>
T** create_empty_matrix(int r, int c)
{
  T** m = new T* [r];
  m[0] = new T [r*c];
  std::memset(m[0],0,r*c*sizeof(T));
  for (int i = 1; i < r; ++i)
    m[i] = &(m[0][c*i]);
  return m;
}
