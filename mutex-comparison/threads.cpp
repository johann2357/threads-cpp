#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <time.h>
#include <cstring>

template <typename T>
void fill_random(T** matrix, int rows, int columns, T from, T to);

template <typename T>
void print(T** matrix, int rows, int columns);

template <typename T>
T** create_matrix(int r, int c);

template <typename T>
T** create_empty_matrix(int r, int c);


struct counter
{
  char** matrix_;
  int columns_;
  int* result_;
  int start_;
  int end_;
  counter(char** m, int r, int c, int* result, int start, int end): matrix_(m), result_(result) 
  {
    start_ = start;
    end_ = end;
    columns_ = c;
  }
  void operator() ()
  {
    for (int i = start_; i <= end_; ++i)
      for (int j = 0; j < columns_; ++j)
      {
        char current = matrix_[i][j];
        if ((current >= 'a') && (current <= 'j'))
          ++result_[current - 'a'];
      }
  }
};


std::vector<int> share(char** matrix, int rows, int columns)
{
  std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
  int nthreads = std::thread::hardware_concurrency();
  int** result = create_empty_matrix<int> (nthreads, 10);
  std::vector<std::thread> threads;
  int start = 0, end = 1;
  int part = rows/nthreads - 1;
  if (part < 0)
    part = 0;
  int i;
  for (i = 0; start < rows; ++i)
  {
    end = start + part;
    if ((end >= rows) || (i == (nthreads - 1)))
      end = rows - 1;
    threads.push_back(std::thread(counter(matrix, rows, columns, result[i], start, end)));
    start = end + 1;
  }
  for (auto& th : threads)
    th.join();
  for (int j = 1; j < i; ++j)
    for (int k = 0; k < 10; ++k)
      result[0][k] += result[j][k];
  std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
  std::cout << "It took me " <<  std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count() << " milliseconds.\n";
  std::vector<int> sum(result[0], result[0] + 10);
  delete [] (*result);
  delete [] result;
  return sum;
}

int main()
{
  int rows = 99999;
  int columns = 999;
  std::cout << "Create matrix\n";
  char** test = create_matrix<char> (rows, columns);
  std::cout << "Fill out matrix\n";
  fill_random<char> (test, rows, columns, 'a', 'z');
  std::cout << "Create threads\n";
  std::vector<int> result = share(test, rows, columns);
  for (auto& i : result)
    std::cout << i << ' ';
  std::cout << '\n';
  delete [] (*test);
  delete [] test;
  return 0;
}

template <typename T>
void fill_random(T** matrix, int rows, int columns, T from, T to)
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator (seed);
  std::uniform_int_distribution<T> distribution(from, to);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < columns; ++j)
      matrix[i][j] = distribution(generator);
}

template <typename T>
void print(T** matrix, int rows, int columns)
{
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < columns; ++j)
      std::cout << matrix[i][j] << ' ';      
    std::cout << std::endl;
  }
}

template <typename T>
T** create_matrix(int r, int c)
{
  T** m = new T* [r];
  m[0] = new T [r*c];
  for (int i = 1; i < r; ++i)
    m[i] = &(m[0][c*i]);
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
