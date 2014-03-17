#ifndef LIST_H
#define LIST_H

#include <iostream>
#include "Traits.h"

template <typename T>
class Node
{
  template <typename X, class Y> friend class List;
  template <typename X> friend class ListIterator;
  T data_;
  Node<T>* next_;
 public:
  Node(T x)
  {
    data_ = x;
    next_ = NULL;
  }
  Node(T x, Node<T>*& nxt)
  {
    data_ = x;
    next_ = nxt;
  }
  virtual ~Node() {}
};

template < typename T, class Tr=Forward<T> >
class List
{
  typedef typename Tr::C C;
  Node<T>* head_;
  Node<T>* tail_;
  int limit_;
  C compare_;
public:
  int size_;
  typedef ListIterator<T> iterator;
  List(int limit=0)
  {
    head_ = NULL;
    size_ = 0;
    limit_ = limit;
  }
  virtual ~List()
  {
    Node<T>* t;
    while (head_)
    {
      t = head_->next_;
      delete head_;
      head_ = t;
    }
  }
  bool find(T x, Node<T>**& p)
  {
    p = &head_;
    while ( *p && compare_ ((*p)->data_, x) )
      p = &(*p)->next_;
    return ( *p && ((*p)->data_ == x) );
  }
  bool insert(T x)
  {
    Node<T>** p;
    if ( find(x, p) ) return false;
    (*p) = new Node<T> (x, (*p));
    ++size_;
    /*
    if ((*p == NULL) && (size_ == limit_))
      return false;
    if (*p != NULL)
    {
      (*p) = new Node<T> (x, (*p));
      ++size_;
    }
    if (size_ < limit_)
      (*p) = new Node<T> (x, (*p));
      ++size_;
    */
    return true;
  }
  bool remove(T x)
  {
    Node<T>** p;
    if ( !find(x, p) ) return false;
    Node<T>* tmp = *p;
    (*p) = (*p)->next_;
    delete tmp;
    --size_;
    return true;
  }
  iterator begin()
  {
    iterator tmp(head_);
    return tmp;
  }
  iterator end()
  {
    return NULL;
  }
};

template <typename T>
class ListIterator
{
  typedef ListIterator<T> self;
  Node<T>* i_;
public:
  ListIterator(Node<T>* p=NULL)
  {
    i_ =  p;
  }
  ListIterator(const self& ci)
  {
    i_ = ci.i_;
  }
  self& operator= (const self& i)
  {
    i_ = i.i_;
    return *this;
  }
  bool operator!= (self i)
  {
    return (i_ != i.i_);
  }
  bool operator== (self i)
  {
    return (i_ == i.i_);
  }
  T operator* ()
  {
    return i_->data_;
  }
  self& operator++ ()
  {
    i_ = i_->next_;
    return *this;
  }
  self operator++ (int)
  {
    self tmp(*this);
    ++(*this);
    return tmp;
  }
  friend std::ostream& operator<<(std::ostream& out, const self& x)
  {
    std::cout << x.i_;
  }
};

#endif // LIST_H
