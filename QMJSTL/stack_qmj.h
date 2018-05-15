#pragma once
#ifndef _STACK_QMJ_
#define _STACK_QMJ_
#include <vector>
#include "vector_qmj.h"

namespace qmj
{
template <typename value_type_, typename Container = _QMJ vector<value_type_>>
class stack
{
public:
  typedef typename Container::value_type value_type;
  typedef Container container_type;
  typedef typename container_type::size_type size_type;
  typedef typename container_type::reference reference;
  typedef typename container_type::const_reference const_reference;

  typedef stack<value_type, Container> self;

  stack() : c() {}

  template <typename Iter>
  stack(Iter first, Iter last) : c(first, last) {}

  stack(const self &x) : c(x.c) {}

  stack(self &&x) : c(std::move(x.c)) {}

  explicit stack(Container &&c) : c(std::move(c)) {}

  explicit stack(const Container &cont) : c(cont) {}

  self &operator=(const self &x)
  {
    c = x.c;
    return (*this);
  }

  self &operator=(self &&x)
  {
    c = std::move(x.c);
    return (*this);
  }

  bool empty() const { return c.empty(); }

  reference top() { return c.back(); }

  const_reference top() const { return c.back(); }

  void push(const value_type &val) { c.push_back(val); }

  template <typename... types>
  void emplace(types &&... args)
  {
    c.emplace_back(std::forward<types>(args)...);
  }

  void pop() { c.pop_back(); }

  size_t size() const { return c.size(); }

  value_type pop_top()
  {
    auto top = c.back();
    c.pop_back();
    return top;
  }

  void swap(self &x) noexcept { swap(c, x.c); }

  const Container &_get_constainer() const { return c; }

private:
  Container c;
};

template <typename value_type, typename Container>
inline void swap(_QMJ stack<value_type, Container> &left,
                 _QMJ stack<value_type, Container> &right) noexcept
{
  left.swap(right);
}

template <typename value_type, typename Container>
inline bool operator==(const _QMJ stack<value_type, Container> &left,
                       const _QMJ stack<value_type, Container> &right)
{
  return (left._get_constainer() == right._get_constainer());
}

template <typename value_type, typename Container>
inline bool operator!=(const _QMJ stack<value_type, Container> &left,
                       const _QMJ stack<value_type, Container> &right)
{
  return (left._get_constainer() != right._get_constainer());
}

template <typename value_type, typename Container>
inline bool operator<(const _QMJ stack<value_type, Container> &left,
                      const _QMJ stack<value_type, Container> &right)
{
  return (left._get_constainer() < right._get_constainer());
}

template <typename value_type, typename Container>
inline bool operator<=(const _QMJ stack<value_type, Container> &left,
                       const _QMJ stack<value_type, Container> &right)
{
  return (left._get_constainer() <= right._get_constainer());
}

template <typename value_type, typename Container>
inline bool operator>(const _QMJ stack<value_type, Container> &left,
                      const _QMJ stack<value_type, Container> &right)
{
  return (left._get_constainer() > right._get_constainer());
}

template <typename value_type, typename Container>
inline bool operator>=(const _QMJ stack<value_type, Container> &left,
                       const _QMJ stack<value_type, Container> &right)
{
  return (left._get_constainer() >= right._get_constainer());
}
} // namespace qmj

#endif
