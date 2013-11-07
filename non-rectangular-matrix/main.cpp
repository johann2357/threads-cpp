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
void fill_random(T* v, int columns, T from, T to, T end_of_row);

template <typename T>
void print(T** matrix, int rows, T end_of_row);

template <typename T>
T** create_matrix(int r, int min_c, int max_c, T from, T to, T end_of_row);

template <typename T>
T** create_empty_matrix(int r, int c);

struct counter
{
  char** matrix_;
  int* result_;
  int* current_;
  int stop_;
  char end_of_row_;
  counter(char** m, int* result, int* current, int stop, char e_o_r)
  {
    matrix_ = m;
    result_ = result;
    current_ = current;
    stop_ = stop;
    end_of_row_ = e_o_r;
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
      for (int j = 0; matrix_[row][j] != end_of_row_; ++j)
        if ((matrix_[row][j] >= 'a') && (matrix_[row][j] <= 'j'))
          ++result_[matrix_[row][j] - 'a'];
    } 
  }
};


std::vector<int> share(char** matrix, int rows, char end_of_row)
{
  using namespace std;
  chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
  int nthreads = thread::hardware_concurrency();
  int** result = create_empty_matrix<int> (nthreads, 10);
  vector<thread> threads;
  int* current_row = new int(0); 
  for (int i = 0; i < nthreads; ++i)
    threads.push_back(thread( counter(matrix, result[i], 
                              current_row, rows, end_of_row) ));
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

std::vector<int> no_threads(char** matrix, int rows, char end_of_row)
{
  using namespace std;
  chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
  vector<int> result({0,0,0,0,0,0,0,0,0,0});       
  for (int i = 0; i < rows; ++i)
    for (int j = 0; matrix[i][j] != end_of_row; ++j)
    {
      if ((matrix[i][j] >= 'a') && (matrix[i][j] <= 'j'))
        ++result[matrix[i][j] - 'a'];
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
  int min_columns = 66;
  int max_columns = 999999;
  Debug("Create matrix");
  char** test = NULL;
  test =  create_matrix<char> (rows, min_columns, max_columns, 'a', 'z', '\0');
  Debug("Result using threads");
  std::vector<int> result = share(test, rows, '\0');
  for (auto& i : result)
    Debug( i );
  Debug("Result no-threads");
  std::vector<int> result2 = no_threads(test, rows, '\0');
  for (auto& i : result2)
    Debug( i );
  for (int i = 0; i < rows; ++i)
    delete test[i];
  delete [] test;
  return 0;
}

template <typename T>
void fill_random(T* v, int columns, T from, T to, T end_of_row)
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator (seed);
  std::uniform_int_distribution<T> distribution(from, to);
  for (int i = 0; i < (columns-1); ++i)
    v[i] = distribution(generator);
  v[columns - 1] = end_of_row;
}

template <typename T>
void print(T** matrix, int rows, T end_of_row)
{
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; matrix[i][j] != end_of_row; ++j)
      std::cout << matrix[i][j] << ' ';      
    std::cout << std::endl;
  }
}

template <typename T>
T** create_matrix(int r, int min_c, int max_c, T from, T to, T end_of_row)
{
  unsigned seed;
  seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(min_c, max_c);
  T** m = new T* [r];
  for (int i = 0; i < r; ++i)
  {
    int columns = distribution(generator);
    m[i] = new T [columns];
    fill_random<T>(m[i], columns, from, to, end_of_row);
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
