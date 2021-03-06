#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <time.h>

template <typename T>
void fill_random(T** matrix, int rows, int columns, T from, T to);

template <typename T>
void print(T** matrix, int rows, int columns);

template <typename T>
T** create_matrix(int r, int c);

std::vector<int> share(char** matrix, int rows, int columns)
{
  std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
  std::vector<int> result({0,0,0,0,0,0,0,0,0,0});
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < columns; ++j)
    {
      char current = matrix[i][j];
      if ((current >= 'a') && (current <= 'j'))
        ++result[current - 'a'];
    }
  std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
  std::cout << "It took me " <<  std::chrono::duration_cast<std::chrono::milliseconds> (t2 - t1).count() << " milliseconds.\n";
  return result;
}

int main()
{
  int rows = 99999;
  int columns = 999;
  std::cout << "Create matrix\n";
  char** test = create_matrix<char> (rows, columns);
  std::cout << "Fill out matrix\n";
  fill_random<char> (test, rows, columns, 'a', 'z');
  std::cout << "Counting\n";
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
