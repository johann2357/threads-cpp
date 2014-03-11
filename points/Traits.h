#ifndef TRAITS_H
#define TRAITS_H

template <typename T>
class Node;
template <typename T>
class ListIterator;
template <typename T, class Tr>
class List;

template <typename T>
struct Less
{
  bool operator() (T a, T b)
  {
    return (a->distance_ < b->distance_);
  }
};

template <typename T>
struct Greater
{
  bool operator() (T a, T b)
  {
    return (a > b);
  }
};

template <typename T>
struct Descending
{
  typedef Greater<T> C;
};

template <typename T>
struct Forward
{
  typedef Less<T> C;
};

#endif // TRAITS_H
