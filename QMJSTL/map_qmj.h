#pragma once
#ifndef _MAP_QMJ_
#define _MAP_QMJ_

#include "rb_tree.h"

namespace qmj {
template <typename key_type, typename data_type, typename Compare,
          typename Alloc, bool is_multi>
struct map_traits {
  typedef key_type key_type;
  typedef std::pair<const key_type, data_type> value_type;
  typedef Compare key_compare;
  typedef Alloc allocator_type;

  enum { is_multi = is_multi };

  struct value_compare {
    friend map_traits<key_type, data_type, key_compare, Alloc, is_multi>;
    bool operator()(const value_type& left, const value_type& right) const {
      return key_compare()(left.first, right.first);
    }
  };

  template <typename type1, typename type2>
  static const key_type& keyOfValue(const std::pair<type1, type2>& pr) {
    return (pr.first);
  }
};

template <typename key_type, typename data_type,
          typename Compare = std::less<key_type>,
          typename Alloc = qmj::allocator<std::pair<const key_type, data_type>>>
class map
    : public rb_tree<map_traits<key_type, data_type, Compare, Alloc, false>> {
 public:
  typedef key_type key_type;
  typedef data_type data_type;
  typedef std::pair<const key_type, data_type> value_type;
  typedef Compare key_Compare;

  typedef rb_tree<map_traits<key_type, data_type, Compare, Alloc, false>>
      base_type;
  typedef map<key_type, data_type, Compare> self;

  typedef typename base_type::pointer pointer;
  typedef typename base_type::const_pointer const_pointer;
  typedef typename base_type::reference reference;
  typedef typename base_type::const_reference const_reference;
  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::reverse_iterator reverse_iterator;
  typedef typename base_type::const_reverse_iterator const_reverse_iterator;
  typedef typename base_type::size_type size_type;
  typedef typename base_type::difference_type difference_type;

  map() : base_type() {}

  explicit map(const Compare& comp) : base_type(comp) {}

  template <typename Iter>
  map(Iter first, Iter last) : base_type() {
    base_type::insert(first, last);
  }

  template <typename InputIterator>
  map(InputIterator first, InputIterator last, const Compare& comp)
      : base_type(comp) {
    base_type::insert(first, last);
  }

  map(const std::initializer_list<value_type>& lst) : base_type(lst) {}

  map(const std::initializer_list<value_type>& lst, const Compare& comp)
      : base_type(lst, comp) {}

  map(const self& x) : base_type(x) {}

  map(self&& x) : base_type(std::move(x)) {}

  self& operator=(const self& x) {
    base_type::operator=(x);
    return (*this);
  }

  self& operator=(self&& x) {
    base_type::operator=(std::move(x));
    return (*this);
  }

  data_type& operator[](const key_type& k) {
    return (*((base_type::insert({k, data_type()})).first)).second;
  }

  void swap(self& x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename data_type, typename Compare,
          typename Alloc>
inline void swap(map<key_type, data_type, Compare, Alloc>& left,
                 map<key_type, data_type, Compare, Alloc>& right) noexcept {
  left.swap(right);
}
}

namespace qmj {
template <typename key_type, typename data_type,
          typename Compare = std::less<key_type>,
          typename Alloc = qmj::allocator<std::pair<const key_type, data_type>>>
class multimap
    : public rb_tree<map_traits<key_type, data_type, Compare, Alloc, true>> {
 public:
  typedef key_type key_type;
  typedef data_type data_type;
  typedef std::pair<const key_type, data_type> value_type;
  typedef Compare key_Compare;

  typedef rb_tree<map_traits<key_type, data_type, Compare, Alloc, true>>
      base_type;
  typedef multimap<key_type, data_type, Compare> self;

  typedef typename base_type::pointer pointer;
  typedef typename base_type::const_pointer const_pointer;
  typedef typename base_type::reference reference;
  typedef typename base_type::const_reference const_reference;
  typedef typename base_type::iterator iterator;
  typedef typename base_type::const_iterator const_iterator;
  typedef typename base_type::reverse_iterator reverse_iterator;
  typedef typename base_type::const_reverse_iterator const_reverse_iterator;
  typedef typename base_type::size_type size_type;
  typedef typename base_type::difference_type difference_type;

  multimap() : base_type() {}

  explicit multimap(const Compare& comp) : base_type(comp) {}

  template <typename Iter>
  multimap(Iter first, Iter last) : base_type() {
    base_type::insert(first, last);
  }

  template <typename InputIterator>
  multimap(InputIterator first, InputIterator last, const Compare& comp)
      : base_type(comp) {
    base_type::insert(first, last);
  }

  multimap(const self& x) : base_type(x) {}

  multimap(self&& x) : base_type(std::move(x)) {}

  self& operator=(const self& x) {
    base_type::operator=(x);
    return (*this);
  }

  self& operator=(self&& x) {
    base_type::operator=(std::move(x));
    return (*this);
  }

  data_type& operator[](const key_type& k) {
    return (*(insert({k, data_type()}))).second;
  }

  void swap(self& x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename data_type, typename Compare,
          typename Alloc>
inline void swap(
    multimap<key_type, data_type, Compare, Alloc>& left,
    multimap<key_type, data_type, Compare, Alloc>& right) noexcept {
  left.swap(right);
}
}

#endif