#pragma once
#ifndef _VECTOR_QMJ_
#define _VECTOR_QMJ_
#include <initializer_list>
#include "algorithm_qmj.h"
#include "allocator.h"
#include "iterator_qmj.h"

namespace qmj {
template <typename value_type_>
class vector_const_iterator {  // const_iterator of vector
 public:
  typedef std::random_access_iterator_tag iterator_category;
  typedef value_type_ value_type;
  typedef const value_type *pointer;
  typedef const value_type &reference;
  typedef ptrdiff_t difference_type;

  typedef true_type memory_copy_tag;
  typedef vector_const_iterator<value_type> self;

  vector_const_iterator(value_type *cur = nullptr)
      : cur(cur) {  // default or pointer construct
  }

  vector_const_iterator(const self &x) : cur(x.cur) {  // copy construct
  }

  self &operator=(const self &x) {  // assign
    cur = x.cur;
    return (*this);
  }

  bool operator==(const self &x) const { return (cur == x.cur); }

  bool operator!=(const self &x) const { return (!(operator==(x))); }

  bool operator<(const self &x) const { return (cur < x.cur); }

  bool operator<=(const self &x) const { return (cur <= x.cur); }

  bool operator>(const self &x) const { return (cur > x.cur); }

  bool operator>=(const self &x) const { return (cur >= x.cur); }

  reference operator*() const { return (*cur); }

  pointer operator->() const { return (&(operator*())); }

  reference operator[](difference_type off) const { return (*(cur + off)); }

  self &operator++() {
    ++cur;
    return (*this);
  }

  self operator++(int) {
    auto ret = *this;
    operator++();
    return (ret);
  }

  self &operator--() {
    --cur;
    return (*this);
  }

  self operator--(int) {
    auto ret = *this;
    operator--();
    return (ret);
  }

  self operator+(const difference_type n) const { return self(cur + n); }

  self &operator+=(const difference_type n) {
    cur += n;
    return (*this);
  }

  self operator-(const difference_type n) const { return self(cur - n); }

  difference_type operator-(const self &x) const { return (cur - x.cur); }

  self &operator-=(const difference_type n) {
    cur -= n;
    return (*this);
  }

 protected:
  value_type *cur;
};

template <typename value_type_>
class vector_iterator
    : public vector_const_iterator<value_type_> {  // iterator of vector
 public:
  typedef std::random_access_iterator_tag iterator_category;
  typedef value_type_ value_type;
  typedef value_type *pointer;
  typedef value_type &reference;
  typedef ptrdiff_t difference_type;
  typedef true_type memory_copy_tag;
  typedef vector_iterator<value_type> self;
  typedef vector_const_iterator<value_type> base_type;

  using base_type::cur;

  vector_iterator(const pointer cur = nullptr)
      : base_type(cur) {  // default or pointer construct
  }

  vector_iterator(const self &x) : base_type(x.cur) {  // copy construct
  }

  self &operator=(const self &x) {  // assign
    cur = x.cur;
    return *this;
  }

  bool operator==(const self &x) const { return (cur == x.cur); }

  bool operator!=(const self &x) const { return (!(operator==(x))); }

  bool operator<(const self &x) const { return (cur < x.cur); }

  bool operator<=(const self &x) const { return (cur <= x.cur); }

  bool operator>(const self &x) const { return (cur > x.cur); }

  bool operator>=(const self &x) const { return (cur >= x.cur); }

  reference operator*() { return (*cur); }

  pointer operator->() { return (&(operator*())); }

  reference operator[](difference_type off) const { return (*(cur + off)); }

  self &operator++() {
    ++cur;
    return (*this);
  }

  self operator++(int) {
    auto ret = *this;
    operator++();
    return (ret);
  }

  self &operator--() {
    --cur;
    return (*this);
  }

  self operator--(int) {
    auto ret = *this;
    operator--();
    return (ret);
  }

  self operator+(const difference_type n) const { return self(cur + n); }

  self &operator+=(const difference_type n) {
    cur += n;
    return (*this);
  }

  self operator-(const difference_type n) const { return (cur - n); }

  difference_type operator-(const self &x) const { return (cur - x.cur); }

  self &operator-=(const difference_type n) {
    cur -= n;
    return (*this);
  }
};

template <typename value_type_, typename Alloc = _QMJ allocator<value_type_>>
class vector {
 public:
  typedef value_type_ value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t differene_type;

  typedef vector_iterator<value_type> iterator;
  typedef vector_const_iterator<value_type> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef vector<value_type, Alloc> self;
  typedef Alloc allocator_type;
  typedef Alloc alloc;

  vector()
      : first(nullptr),
        last(nullptr),
        end_storage(nullptr) {  // default construct
  }

  explicit vector(const size_type n)
      : vector() {  // construct with n*vale_type()
    first = alloc::allocate(n);
    alloc::construct(first, n);
    last = end_storage = first + n;
  }

  vector(const size_type n, const value_type &val)
      : vector() {  // construct with n*val
    first = alloc::allocate(n);
    end_storage = last = alloc::copy_construct(first, n, val);
  }

  vector(const std::initializer_list<value_type> &lst)
      : vector(lst.begin(), lst.end()) {  // construct with init list
  }

  template <typename Iter,
            typename = typename enable_if<is_iterator<Iter>::value, void>::type>
  vector(Iter bg, Iter ed) : vector() {  // construct with [bg,ed)
    assign_imple(bg, ed, _QMJ iterator_category(bg));
  }

  vector(const self &x) {  // copy construct
    first = alloc::allocate(x.size());
    last = alloc::copy_construct(x.first, x.last, first);
    end_storage = first + x.size();
  }

  vector(self &&x) noexcept :  // move construct
                               first(x.first),
                               last(x.last),
                               end_storage(x.end_storage) {
    x.first = x.last = x.end_storage = nullptr;
  }

  self &operator=(self x) {  // copy ande move assign
    swap(x);
    return (*this);
  }

  self &operator=(
      const std::initializer_list<value_type> &lst) {  // assign init_list
    assign(lst.begin(), lst.end());
    return *this;
  }

  ~vector() {  // destroy func
    alloc::destroy(first, last);
    alloc::deallocate(first);
  }

  void shrink_to_fit() {
    if (end_storage != last &&
        !empty()) {  // have extra space,redistribute space of size
      size_t len = this->size();
      pointer ptr = alloc::allocate(len);
      pointer new_last = alloc::copy_construct(first, last, ptr);
      deallocate_and_update_ptr(ptr, new_last, len);
    }
  }

  void swap(self &x) noexcept {  // swap with x
    std::swap(first, x.first);
    std::swap(last, x.last);
    std::swap(end_storage, x.end_storage);
  }

  allocator_type get_allocator() const {  // return allocator of value_type
    return allocator_type();
  }

  void reserve(const size_type n) {  // reserve n space
    if (n > capacity()) {            // redistrubted space
      pointer ptr = alloc::allocate(n);
      pointer new_last = alloc::copy_construct(first, last, ptr);
      deallocate_and_update_ptr(ptr, new_last, n);
    }
  }

  iterator begin() { return iterator(first); }

  const_iterator begin() const { return const_iterator(first); }

  iterator end() { return iterator(last); }

  const_iterator end() const { return const_iterator(last); }

  const_iterator cbegin() const { return const_iterator(first); }

  const_iterator cend() const { return const_iterator(last); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }

  size_type size() const {  // return size
    return (last - first);
  }

  reference at(const size_type n) {  // reference of ele in subscribe n
    return (*(first + n));
  }

  const_reference at(
      const size_type n) const {  // const_reference of ele in subscribe n
    return (*(first + n));
  }

  pointer data() {  // pointer of adress of first ele
    return (first);
  }

  const_pointer data() const {  // const_pointer of address of first ele
    return (first);
  }

  iterator erase(iterator pos) {  // erase and destroy ele in pos
    std::copy(pos + 1, end(), pos);
    --last;
    alloc::destroy(last);
    return pos;
  }

  iterator erase(iterator bg, iterator ed) {  // erase and destroy ele of
                                              // [bg,ed)
    iterator iter = std::copy(ed, end(), bg);
    alloc::destroy(first + (iter - begin()), last);
    last = first + (iter - begin());
    return bg;
  }

  void clear() {  // clear and destroy all ele
    alloc::destroy(first, last);
    last = first;
  }

  void assign(const size_type n, const value_type &val) {  // assign n*val
    clear();
    insert(begin(), n, val);
  }

  template <typename Iter>
  typename enable_if<is_iterator<Iter>::value, void>::type assign(
      Iter bg, Iter ed) {  // assign ele in [bg,ed)
    clear();
    assign_imple(bg, ed, typename _QMJ iterator_traits<Iter>::iterator_category());
  }

  void assign(
      const std::initializer_list<value_type> &lst) {  // assign init list
    assign(lst.begin(), lst.end());
  }

  size_type max_size() const {  // return max size can container of value_type
    return alloc::max_size();
  }

  void resize(const size_type n) {  // resize as n,extra ele is value_type()
    if (n <= size()) {              // destory extra ele
      alloc::destroy(first + n, last);
      last = first + n;
    } else if (n <= capacity()) {  // have adequate space
      alloc::construct(last, first + n);
      last = first + n;
    } else {  // redistribute the space and move old ele
      pointer ptr = alloc::allocate(n);
      alloc::copy_construct(first, last, ptr);
      alloc::construct(ptr + size(), ptr + n);
      deallocate_and_update_ptr(ptr, ptr + n, n);
    }
  }

  void resize(const size_type n, const value_type &val) {
    if (n <= size()) {
      alloc::destroy(first + n, last);
      last = first + n;
    } else if (n <= capacity()) {
      alloc::copy_construct(last, first + n, val);
      last = first + n;
    } else {
      pointer ptr = alloc::allocate(n);
      alloc::copy_construct(first, last, ptr);
      alloc::copy_construct(ptr + size(), ptr + n, val);
      deallocate_and_update_ptr(ptr, ptr + n, n);
    }
  }

  size_type capacity() const { return (end_storage - first); }

  bool empty() const { return (first == last); }

  reference operator[](const size_type n) { return *(first + n); }

  const_reference operator[](const size_type n) const { return *(first + n); }

  reference front() { return (*first); }

  const_reference front() const { return (*first); }

  reference back() { return *(last - 1); }

  const_reference back() const { return *(last - 1); }

  iterator insert(iterator pos, value_type &&val) {
    return emplace(pos, std::move(val));
  }

  iterator insert(iterator pos, const size_t n, const value_type &val);

  iterator insert(iterator pos, const value_type &val) {
    return (insert(pos, 1, val));
  }

  template <typename Iter>
  typename enable_if<is_iterator<Iter>::value, iterator>::type insert(
      iterator pos, Iter bg, Iter ed) {
    return insert_imple(pos, bg, ed,
                       typename _QMJ iterator_traits<Iter>::iterator_category());
  }

  iterator insert(iterator pos, const std::initializer_list<value_type> &lst) {
    return insert(pos, lst.begin(), lst.end());
  }

  void push_back(const value_type &val) {
    if (last == end_storage) {
      const size_type len = size() ? 2 * size() : 1;
      auto ptr = alloc::allocate(len);
      auto new_last = alloc::copy_construct(first, last, ptr);
      deallocate_and_update_ptr(ptr, new_last, len);
    }
    alloc::copy_construct(last++, val);
  }

  void push_back(value_type &&val) {
    emplace_back(std::forward<value_type>(val));
  }

  void pop_back() { alloc::destroy(--last); }

  template <typename... types>
  void emplace_back(types &&... args) {
    if (last == end_storage) {
      const size_type len = size() ? 2 * size() : 1;
      auto ptr = alloc::allocate(len);
      auto new_last = alloc::copy_construct(first, last, ptr);
      deallocate_and_update_ptr(ptr, new_last, len);
    }
    alloc::construct(last++, std::forward<types>(args)...);
  }

  template <typename... types>
  iterator emplace(iterator pos, types &&... args) {
    size_type off = pos - begin();
    emplace_back(std::forward<types>(args)...);
    _QMJ rotate(begin() + off, --end(), end());
    return (begin() + off);
  }

 private:
  template <typename Iter>
  void assign_imple(Iter bg, Iter ed, std::input_iterator_tag) {
    for (; bg != ed; ++bg) push_back(*bg);
  }

  template <typename Iter>
  void assign_imple(Iter bg, Iter ed, std::forward_iterator_tag) {
    const size_t len = std::distance(bg, ed);
    if (capacity() < len) {
      alloc::deallocate(first);
      first = alloc::allocate(len);
      end_storage = first + len;
    }
    last = alloc::copy_construct(bg, len, first);
  }

  template <typename Iter>
  iterator insert_imple(iterator pos, Iter bg, Iter ed,
                        std::input_iterator_tag) {
    const size_type off = pos - begin();  // offset
    const size_type old_size = size();
    for (; bg != ed; ++bg) push_back(*bg);
    _QMJ rotate(begin() + off, begin() + old_size, end());
    return (begin() + off);
  }

  template <typename Iter>
  iterator insert_imple(iterator pos, Iter bg, Iter ed,
                        std::forward_iterator_tag) {  // insert [bg,ed) before
                                                      // pos if bg is forward
                                                      // iterator
    const size_type off = pos - begin();  // offset
    const size_type count =
        _QMJ distance(bg, ed);  // for calculate new space if need
    if (count <=
        static_cast<size_type>(
            end_storage - last)) {  // there is plenty of storage space in tail
      const size_type after_ele = end() - pos;
      std::copy_backward(pos, end(), last + count);
      last += count;
      std::copy(bg, ed, pos);
    } else {  // redistribute space
      const size_type old_size = capacity();
      const size_type len = old_size + (old_size > count ? old_size : count);
      pointer ptr = alloc::allocate(len);
      pointer new_last = alloc::copy_construct(first, first + off, ptr);
      new_last = alloc::copy_construct(bg, ed, new_last);
      new_last = alloc::copy_construct(first + off, last, new_last);
      deallocate_and_update_ptr(ptr, new_last, len);
    }
    return (begin() + off);
  }

  void deallocate_and_update_ptr(pointer new_first, pointer new_last,
                                 const size_type n) {  // destroy old data and
                                                       // dallocate old space
                                                       // ,update pointer for
                                                       // new space
    alloc::destroy(first, last);
    alloc::deallocate(first);
    first = new_first;
    last = new_last;
    end_storage = first + n;
  }

 private:
  pointer first;
  pointer last;
  pointer end_storage;
};

template <typename value_type, typename alloc>
typename vector<value_type, alloc>::iterator vector<value_type, alloc>::insert(
    iterator pos, const size_t n,
    const value_type &val) {           // insert n val before pos
  if (!n) return pos;                  // do nothing if n==0
  differene_type off = pos - begin();  // offset
  if (static_cast<size_t>(end_storage - last) >=
      n) {  // there is plenty of storage space in tail
    const size_type after_ele = end() - pos;
    if (after_ele > n) {  // the ele of [pos,end())>n
      alloc::copy_construct(end() - n, end(), last);
      std::copy_backward(pos, end() - n, end());
      std::fill(pos, pos + n, val);
    } else {
      alloc::copy_construct(last, n - after_ele, val);
      alloc::copy_construct(pos, end(), last + (n - after_ele));
      std::fill(pos, end(), val);
    }
    last += n;
  } else {  // redistribute space
    const size_type old_size = end_storage - first;
    const size_type len = old_size + (old_size > n ? old_size : n);  // new size
    auto ptr = alloc::allocate(len);
    auto new_last = alloc::copy_construct(first, first + (pos - first), ptr);
    new_last = alloc::copy_construct(new_last, n, val);
    new_last = alloc::copy_construct(pos, end(), new_last);
    deallocate_and_update_ptr(ptr, new_last, len);
  }
  return (begin() + off);
}

template <typename value_type, typename alloc>
inline void swap(_QMJ vector<value_type> &left,
                 _QMJ vector<value_type> &right) noexcept {
  left.swap(right);
}

template <typename value_type, typename alloc>
inline bool operator==(const _QMJ vector<value_type, alloc> &left,
                       const _QMJ vector<value_type, alloc> &right) {
  return (left.size() == right.size() &&
          std::equal(left.begin(), left.end(), right.begin()));
}

template <typename value_type, typename alloc>
inline bool operator!=(const _QMJ vector<value_type, alloc> &left,
                       const _QMJ vector<value_type, alloc> &right) {
  return !(left == right);
}

template <typename value_type, typename alloc>
inline bool operator<(const _QMJ vector<value_type, alloc> &left,
                      const _QMJ vector<value_type, alloc> &right) {
  return std::lexicographical_compare(left.begin(), left.end(), right.begin(),
                                      right.end());
}

template <typename value_type, typename alloc>
inline bool operator<=(const _QMJ vector<value_type, alloc> &left,
                       const _QMJ vector<value_type, alloc> &right) {
  return !(right < left);
}

template <typename value_type, typename alloc>
inline bool operator>(const _QMJ vector<value_type, alloc> &left,
                      const _QMJ vector<value_type, alloc> &right) {
  return right < left;
}

template <typename value_type, typename alloc>
inline bool operator>=(const _QMJ vector<value_type, alloc> &left,
                       const _QMJ vector<value_type, alloc> &right) {
  return !(left < right);
}
}

#endif
