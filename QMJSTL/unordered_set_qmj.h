#pragma once
#ifndef _UNORDERED_SET_QMJ_
#define _UNORDERED_SET_QMJ_

#include <initializer_list>

#include "hashtable.h"

namespace qmj
{
template <typename key_type_, typename HashFunction_, typename EqualKey_,
          typename Alloc, bool is_multi_>
struct uset_traits
{
  typedef key_type_ key_type;
  typedef key_type value_type;
  typedef HashFunction_ HashFunction;
  typedef EqualKey_ EqualKey;
  typedef Alloc allocator_type;

  enum
  {
    is_multi = is_multi_
  };

  static const key_type &ExtractKey(const value_type &val) { return (val); }
};

template <typename key_type_, typename HashFunction = qmj::hash<key_type_>,
          typename EqualKey = std::equal_to<key_type_>,
          typename Alloc = qmj::allocator<key_type_>>
class unordered_set
    : public hashtable<
          uset_traits<key_type_, HashFunction, EqualKey, Alloc, false>>
{
public:
  typedef hashtable<uset_traits<key_type_, HashFunction, EqualKey, Alloc, false>>
      base_type;
  typedef unordered_set<key_type_, HashFunction, EqualKey> self;

  typedef typename base_type::key_type key_type;
  typedef typename base_type::value_type value_type;
  typedef typename base_type::hasher hasher;
  typedef typename base_type::key_equal key_equal;
  typedef typename base_type::size_type size_type;

  typedef typename base_type::difference_type difference_type;
  typedef typename base_type::const_pointer pointer;
  typedef typename base_type::const_pointer const_pointer;
  typedef typename base_type::const_reference reference;
  typedef typename base_type::const_reference const_reference;

  typedef typename base_type::const_iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::const_local_iterator const_local_iterator;
  typedef typename base_type::local_iterator local_iterator;

public:
  unordered_set() : base_type() {}

  explicit unordered_set(size_type n) : base_type(n) {}

  unordered_set(size_type n, const hasher &hf) : base_type(n, hf) {}

  unordered_set(const size_type n, const hasher &hf, const key_equal &eql)
      : base_type(n, hf, eql) {}

  template <typename IIter>
  unordered_set(IIter first, IIter last) : base_type()
  {
    base_type::insert(first, last);
  }

  unordered_set(const std::initializer_list<value_type> &lst)
      : unordered_set(lst.begin(), lst.end())
  { // construct with init list
  }

  template <typename IIter>
  unordered_set(IIter first, IIter last, size_type n) : base_type(n)
  {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_set(IIter first, IIter last, size_type n, const hasher &hf)
      : base_type(n, hf)
  {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_set(IIter first, IIter last, const size_type n, const hasher &hf,
                const key_equal &eql)
      : base_type(n, hf, eql)
  {
    base_type::insert_unique(first, last);
  }

  unordered_set(const self &x) : base_type(x) {}

  self &operator=(const self &x)
  {
    base_type::operator=(x);
    return (*this);
    ;
  }

  self &operator=(self &&x)
  {
    base_type::operator=(std::move(x));
    return (*this);
  }

  self &operator=(const std::initializer_list<value_type> &lst)
  {
    base_type::clear();
    base_type::insert(lst.begin(), lst.end());
    return (*this);
  }

  void swap(self &x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename HashFunction, typename EqualKey,
          typename Alloc>
void swap(
    unordered_set<key_type, HashFunction, EqualKey, Alloc> &left,
    unordered_set<key_type, HashFunction, EqualKey, Alloc> &right) noexcept
{
  left.swap(right);
}

template <typename key_type_, typename HashFunction = qmj::hash<key_type_>,
          typename EqualKey = std::equal_to<key_type_>,
          typename Alloc = qmj::allocator<key_type_>>
class unordered_multiset
    : public hashtable<
          uset_traits<key_type_, HashFunction, EqualKey, Alloc, true>>
{
public:
  typedef hashtable<uset_traits<key_type_, HashFunction, EqualKey, Alloc, true>>
      base_type;

  typedef unordered_multiset<key_type_, HashFunction, EqualKey> self;

  typedef typename base_type::key_type key_type;
  typedef typename base_type::value_type value_type;
  typedef typename base_type::hasher hasher;
  typedef typename base_type::key_equal key_equal;
  typedef typename base_type::size_type size_type;

  typedef typename base_type::difference_type difference_type;
  typedef typename base_type::const_pointer pointer;
  typedef typename base_type::const_pointer const_pointer;
  typedef typename base_type::const_reference reference;
  typedef typename base_type::const_reference const_reference;

  typedef typename base_type::const_iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::const_local_iterator const_local_iterator;
  typedef typename base_type::local_iterator local_iterator;

public:
  unordered_multiset() : base_type() {}

  explicit unordered_multiset(size_type n) : base_type(n) {}

  unordered_multiset(size_type n, const hasher &hf) : base_type(n, hf) {}

  unordered_multiset(const size_type n, const hasher &hf, const key_equal &eql)
      : base_type(n, hf, eql) {}

  template <typename IIter>
  unordered_multiset(IIter first, IIter last) : base_type()
  {
    base_type::insert(first, last);
  }

  unordered_multiset(const std::initializer_list<value_type> &lst)
      : unordered_multiset(lst.begin(), lst.end())
  { // construct with init list
  }

  template <typename IIter>
  unordered_multiset(IIter first, IIter last, size_type n) : base_type(n)
  {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_multiset(IIter first, IIter last, size_type n, const hasher &hf)
      : base_type(n, hf)
  {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_multiset(IIter first, IIter last, const size_type n,
                     const hasher &hf, const key_equal &eql)
      : base_type(n, hf, eql)
  {
    base_type::insert_unique(first, last);
  }

  unordered_multiset(const self &x) : base_type(x) {}

  self &operator=(const self &x)
  {
    base_type::operator=(x);
    return (*this);
    ;
  }

  self &operator=(self &&x)
  {
    base_type::operator=(std::move(x));
    return (*this);
  }

  self &operator=(const std::initializer_list<value_type> &lst)
  {
    base_type::clear();
    base_type::insert(lst.begin(), lst.end());
    return (*this);
  }

  void swap(self &x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename HashFunction, typename EqualKey,
          typename Alloc>
void swap(unordered_multiset<key_type, HashFunction, EqualKey, Alloc> &left,
          unordered_multiset<key_type, HashFunction, EqualKey, Alloc> &
              right) noexcept
{
  left.swap(right);
}
} // namespace qmj

#endif