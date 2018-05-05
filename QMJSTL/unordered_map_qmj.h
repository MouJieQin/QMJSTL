#pragma once
#ifndef _UNORDERED_MAP_QMJ_
#define _UNORDERED_MAP_QMJ_

#include "hashtable.h"

namespace qmj {
template <typename key_type, typename data_type, typename HashFunction,
          typename EqualKey, typename Alloc, bool is_multi>
struct unordered_map_traits {
  typedef key_type key_type;
  typedef data_type data_type;
  typedef std::pair<const key_type, data_type> value_type;
  typedef HashFunction HashFunction;
  typedef EqualKey EqualKey;
  typedef Alloc allocator_type;

  enum { is_multi = is_multi };

  template <typename type1, typename type2>
  inline static const key_type& ExtractKey(const std::pair<type1, type2>& pr) {
    return (pr.first);
  }

  template <typename type1, typename type2>
  inline static const data_type& ExtractData(
      const std::pair<type1, type2>& pr) {
    return (pr.second);
  }
};

template <typename key_type, typename data_type,
          typename HashFunction = qmj::hash<key_type>,
          typename EqualKey = std::equal_to<key_type>,
          typename Alloc = qmj::allocator<std::pair<const key_type, data_type>>>
class unordered_map
    : public hashtable<unordered_map_traits<key_type, data_type, HashFunction,
                                            EqualKey, Alloc, false>> {
 public:
  typedef hashtable<unordered_map_traits<key_type, data_type, HashFunction,
                                         EqualKey, Alloc, false>>
      base_type;

  typedef data_type data_type;
  typedef typename base_type::key_type key_type;
  typedef typename base_type::value_type value_type;
  typedef typename base_type::hasher hasher;
  typedef typename base_type::key_equal key_equal;
  typedef typename base_type::size_type size_type;

  typedef unordered_map<key_type, data_type, HashFunction, EqualKey, Alloc>
      self;

  typedef typename base_type::difference_type difference_type;
  typedef typename base_type::pointer pointer;
  typedef typename base_type::const_pointer const_pointer;
  typedef typename base_type::reference reference;
  typedef typename base_type::const_reference const_reference;
  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::const_local_iterator const_local_iterator;
  typedef typename base_type::local_iterator local_iterator;

  unordered_map() : base_type() {}

  explicit unordered_map(size_type n) : base_type(n) {}

  unordered_map(size_type n, const hasher& hf) : base_type(n, hf) {}

  unordered_map(const size_type n, const hasher& hf, const key_equal& eql)
      : base_type(n, hf, eql) {}

  template <typename IIter>
  unordered_map(IIter first, IIter last) : base_type() {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_map(IIter first, IIter last, size_type n) : base_type(n) {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_map(IIter first, IIter last, size_type n, const hasher& hf)
      : base_type(n, hf) {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_map(IIter first, IIter last, const size_type n, const hasher& hf,
                const key_equal& eql)
      : base_type(n, hf, eql) {
    base_type::insert_unique(first, last);
  }

  unordered_map(const self& x) : base_type(x) {}

  self& operator=(const self& x) {
    base_type::operator=(x);
    return (*this);
    ;
  }

  self& operator=(self&& x) {
    base_type::operator=(std::move(x));
    return (*this);
  }

  self& operator=(const std::initializer_list<value_type>& lst) {
    base_type::clear();
    base_type::insert(lst.begin(), lst.end());
    return (*this);
  }

  data_type& operator[](const key_type& k) {
    return (*((base_type::insert({k, data_type()})).first)).second;
  }

  void swap(self& x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename data_type, typename HashFunction,
          typename EqualKey, typename Alloc>
void swap(
    unordered_map<key_type, data_type, HashFunction, EqualKey, Alloc>& left,
    unordered_map<key_type, data_type, HashFunction, EqualKey, Alloc>&
        right) noexcept {
  left.swap(right);
}

template <typename key_type, typename data_type,
          typename HashFunction = qmj::hash<key_type>,
          typename EqualKey = std::equal_to<key_type>,
          typename Alloc = qmj::allocator<std::pair<const key_type, data_type>>>
class unordered_multimap
    : public hashtable<unordered_map_traits<key_type, data_type, HashFunction,
                                            EqualKey, Alloc, true>> {
 public:
  typedef hashtable<unordered_map_traits<key_type, data_type, HashFunction,
                                         EqualKey, Alloc, true>>
      base_type;

  typedef data_type data_type;
  typedef typename base_type::key_type key_type;
  typedef typename base_type::value_type value_type;
  typedef typename base_type::hasher hasher;
  typedef typename base_type::key_equal key_equal;
  typedef typename base_type::size_type size_type;

  typedef unordered_multimap<key_type, data_type, HashFunction, EqualKey, Alloc>
      self;

  typedef typename base_type::difference_type difference_type;
  typedef typename base_type::pointer pointer;
  typedef typename base_type::const_pointer const_pointer;
  typedef typename base_type::reference reference;
  typedef typename base_type::const_reference const_reference;
  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;

  unordered_multimap() : base_type() {}

  explicit unordered_multimap(size_type n) : base_type(n) {}

  unordered_multimap(size_type n, const hasher& hf) : base_type(n, hf) {}

  unordered_multimap(const size_type n, const hasher& hf, const key_equal& eql)
      : base_type(n, hf, eql) {}

  template <typename IIter>
  unordered_multimap(IIter first, IIter last) : base_type() {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_multimap(IIter first, IIter last, size_type n) : base_type(n) {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_multimap(IIter first, IIter last, size_type n, const hasher& hf)
      : base_type(n, hf) {
    base_type::insert(first, last);
  }

  template <typename IIter>
  unordered_multimap(IIter first, IIter last, size_type n, const hasher& hf,
                     const key_equal& eql)
      : base_type(n, hf, eql) {
    base_type::insert(first, last);
  }

  unordered_multimap(const self& x) : base_type(x) {}

  self& operator=(const self& x) {
    base_type::operator=(x);
    return (*this);
    ;
  }

  self& operator=(self&& x) {
    base_type::operator=(std::move(x));
    return (*this);
  }

  self& operator=(const std::initializer_list<value_type>& lst) {
    base_type::clear();
    base_type::insert(lst.begin(), lst.end());
    return (*this);
  }

  data_type& operator[](const key_type& k) {
    return (*((base_type::insert({k, data_type()})).first)).second;
  }

  void swap(self& x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename data_type, typename HashFunction,
          typename EqualKey, typename Alloc>
void swap(unordered_multimap<key_type, data_type, HashFunction, EqualKey,
                             Alloc>& left,
          unordered_multimap<key_type, data_type, HashFunction, EqualKey,
                             Alloc>& right) noexcept {
  left.swap(right);
}
}

#endif