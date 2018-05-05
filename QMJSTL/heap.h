#pragma once
#ifndef _HEAP_
#define _HEAP_
#include "vector_qmj.h"
/*二叉堆和斐波那契堆*/
namespace qmj {
template <typename value_type, typename Container = _QMJ vector<value_type>,
          typename Compare = std::less<value_type>>
class binary_heap {  //二叉最大/最小堆
 public:
  typedef Container container_type;
  typedef value_type value_type;
  typedef typename container_type::size_type size_type;
  typedef typename container_type::reference reference;
  typedef typename container_type::const_reference const_reference;

  typedef binary_heap<value_type, Container, Compare> self;

  binary_heap() : heap_v(), comp() {}

  template <typename Iter>
  binary_heap(Iter first, Iter last) : heap_v(first, last) {
    build_heap();
  }

  template <typename Iter>
  binary_heap(Iter first, Iter last, const Compare& cmp)
      : heap_v(first, last), comp(cmp) {
    build_heap();
  }

  template <typename Iter>
  binary_heap(Iter first, Iter last, const Container& cont, const Compare& comp)
      : heap_v(cont), comp(comp) {
    heap_v.insert(end(), first, last);
    build_heap();
  }

  binary_heap(const Container& cont, const Compare& comp = Compare())
      : heap_v(cont), comp(comp) {
    build_heap();
  }

  explicit binary_heap(const Compare& cmp) : heap_v(), comp(cmp) {}

  binary_heap(const self& x) : heap_v(x.heap_v), comp(x.comp) {}

  binary_heap(self&& x) : heap_v(std::move(x.heap_v)), comp(x.comp) {}

  self& operator=(const self& x) {
    heap_v = x.heap_v;
    comp = x.comp;
    return (*this);
  }

  self& operator=(self&& x) {
    heap_v = std::move(x.heap_v);
    comp = std::move(x.comp);
    return (*this);
  }

  bool empty() const { return (heap_v.empty()); }

  const_reference top() const { return (heap_v.front()); }

  void push(const value_type& val) {
    heap_v.push_back(val);
    change_key_imple(heap_v.size() - 1, val);
  }

  void pop() {
    heap_v[0] = std::move(heap_v.back());
    heap_v.pop_back();
    heapify(0);
  }

  value_type pop_top() {  //返回顶端元素并让其出队
    value_type top = heap_v[0];
    heap_v[0] = heap_v.back();
    heap_v.pop_back();
    heapify(0);
    return (top);
  }

  void change_key(
      const size_t index,
      const value_type& val) {  //视情况(最大堆/最小堆)改变下标index的键值并上溯
    if (comp(heap_v[index], val)) return;
    change_key_imple(index, val);
  }

  void swap(self& x) noexcept {
    using std::swap;
    swap(comp, x.comp);
    swap(heap_v, x.heap_v);
  }

 protected:
  void change_key_imple(size_t index, const value_type& val) {
    for (size_t par_index = parent(index);
         index && comp(heap_v[par_index], val); par_index = parent(index)) {
      heap_v[index] = std::move(heap_v[par_index]);
      index = par_index;
    }
    heap_v[index] = val;
  }

  void heapify(size_t index) {  //将index节点下溯
    const value_type val = heap_v[index];
    size_t right_index = right(index);
    const size_t len = heap_v.size();
    for (; right_index < len;) {
      if (comp(heap_v[right_index], heap_v[right_index - 1])) --right_index;
      if (comp(val, heap_v[right_index]))
        heap_v[index] = std::move(heap_v[right_index]);
      else {
        heap_v[index] = val;
        return;
      }
      index = right_index;
      right_index = right(index);
    }
    if (right_index == len && comp(val, heap_v[--right_index])) {
      heap_v[index] = std::move(heap_v[right_index]);
      index = right_index;
    }
    heap_v[index] = std::move(val);
  }

  void build_heap() {
    size_t index = heap_v.size();
    if (index < 2) return;
    for (index = parent(index) + 1;  //有子节点的最大下标节点+1
         index != 0;)
      heapify(--index);
  }

  //返回左右孩子或父节点下标
  size_t left(const size_t index) { return (index << 1) + 1; }

  size_t right(const size_t index) { return (index << 1) + 2; }

  size_t parent(const size_t index) { return (index - 1) >> 1; }

 private:
  Container heap_v;
  Compare comp;
};

template <typename value_type, typename Container, typename Compare>
inline void swap(binary_heap<value_type, Container, Compare>& left,
                 binary_heap<value_type, Container, Compare>& right) noexcept {
  left.swap(right);
}
}

namespace qmj {
template <typename value_type>
struct fib_heap_node {
  typedef value_type value_type;
  typedef fib_heap_node<value_type>* link_type;

  fib_heap_node(const value_type& k, bool m = false, size_t d = 0,
                link_type par = nullptr, link_type lef = nullptr,
                link_type righ = nullptr, link_type ch = nullptr)
      : key(k), mark(m), degree(d), p(par), left(lef), right(righ), child(ch) {}

  value_type key;
  size_t degree;
  link_type p;
  link_type left;
  link_type right;
  link_type child;
  bool mark;
};

template <typename value_type,
          typename Compare = std::less<typename value_type>,
          typename Alloc = _QMJ allocator<value_type>>
class fib_heap {  //斐波那契最大/最小堆
 public:
  typedef value_type value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef _QMJ vector<value_type> container_type;
  typedef typename container_type::size_type size_type;
  typedef fib_heap_node<value_type>* link_type;
  typedef fib_heap<value_type, Compare> self;
  typedef Alloc allocator_type;
  typedef fib_heap_node<value_type> node_type;
  typedef typename allocator_type::template rebind<node_type>::other alloc;

  fib_heap() : n(0), header(nullptr), comp() {}

  fib_heap(const Compare& comp) : n(0), header(nullptr), comp(comp) {}

  template <typename FowardIterator>
  fib_heap(FowardIterator first, FowardIterator last, Compare comp = Compare())
      : fib_heap(comp) {
    while (first != last) insert(*first++);
  }

  template <typename FowardIterator>
  fib_heap(
      FowardIterator first, FowardIterator last,
      _QMJ vector<link_type>& map,  //这种映射堆中图中指针的方式给图算法使用
      Compare comp = Compare())
      : fib_heap(comp) {
    while (first != last) insert(*first++, map);
  }

  ~fib_heap() { destroy_heap(header); }

  void swap(self& x) noexcept {
    std::swap(n, x.n);
    std::swap(header, x.header);
    std::swap(comp, x.comp);
  }

  bool empty() const { return (!n); }

  const_reference top() const { return (header->key); }

  void insert(const value_type& key) {
    link_type tar = create_node(key);
    insert_imple(tar);
  }

  void push(const value_type& val) {
    link_type tar = create_node(val);
    insert_imple(tar);
  }

  void push(value_type&& val) {
    link_type tar = create_node(std::forward(val));
    insert_imple(tar);
  }

  void insert(const value_type& key, _QMJ vector<link_type>& map) {
    link_type tar = create_node(key);
    map.push_back(tar);
    insert_imple(tar);
  }

  void fib_union(self& rhs) {
    if (!rhs.header) return;
    if (!header) {
      header = rhs.header;
      n = rhs.n;
    } else {
      link_type lhs_next = header->right;
      link_type rhs_next = rhs.header->right;

      header->right = rhs_next;
      rhs.header->right = lhs_next;

      rhs_next->left = header;
      lhs_next->left = rhs.header;

      n += rhs.n;
      if (comp(rhs.header->key, header->key)) header = rhs.header;
    }
    rhs.header = nullptr;
    rhs.n = 0;
  }

  void pop() {
    link_type top = header;
    if (top) {
      link_type ch = top->child;
      if (ch) {
        do {
          ch->p = nullptr;
          link_type temp = ch->right;
          insert_root_list(ch);
          ch = temp;
        } while (ch != top->child);
      }
      out_of_list(top);
      if (top == top->right)
        header = nullptr;
      else {
        header = header->right;
        consolidate();
      }
      n -= 1;
    }
    destroy_ande_free_node(top);
  }

  value_type extract_top() {
    value_type ret = top();
    pop();
    return (ret);
  }

  void change_key(link_type tar)  //当元素是指针，并且已经改变该指针所指对象的值
  {
    auto k = tar->key;
    auto par = tar->p;
    if (par && comp(k, par->key)) {
      cut(tar);
      cascading_cut(par);
    }
    if (comp(k, header->key)) header = tar;
  }

  void change_key(link_type tar, const value_type& k) {
    if (comp(k, tar->key)) {
      tar->key = k;
      auto par = tar->p;
      if (par && comp(k, par->key)) {
        cut(tar);
        cascading_cut(par);
      }
      if (comp(k, header->key)) header = tar;
    }
  }

 public:
  void print_fib() {  //调试用
    print(get_header());
  }

 protected:
  void destroy_heap(link_type rt) {
    if (rt) {
      link_type next = rt;
      do {
        destroy_heap(rt->child);
        link_type copy = next;
        next = next->right;
        destroy_ande_free_node(copy);
      } while (next != rt);
    }
  }

  void get_header() { return (header); }

 private:
  void print(link_type rt, int counter = 0) {
    if (rt) {
      auto next = rt;
      do {
        print(next->child, counter + 1);

        cout << "\n" << endl;
        for (int n = counter; n != -1; --n) cout << "\t";
        cout << next->key << "|";
        next->mark ? cout << "T" : cout << "F" << endl;

        next = next->right;
      } while (next != rt);
    }
  }

 protected:
  void insert_imple(link_type tar) {
    if (!header) {
      tar->left = tar;
      tar->right = tar;
      header = tar;
    } else {
      insert_root_list(tar);
      if (comp(tar->key, header->key)) header = tar;
    }
    n += 1;
  }

 private:
  void insert_root_list(link_type tar) {
    auto righ = header->right;
    header->right = tar;
    tar->left = header;
    tar->right = righ;
    righ->left = tar;
  }

  void consolidate() {
    _QMJ vector<link_type> vect(static_cast<size_t>(log(n) / log(1.618)) + 1,
                                nullptr);
    auto temp = header;
    _QMJ vector<link_type> v_root;
    v_root.push_back(temp);
    for (auto t = temp->right; t != header; t = t->right) v_root.push_back(t);
    for (auto next : v_root) {
      size_t d = next->degree;
      while (vect[d]) {
        if (comp(vect[d]->key, next->key)) std::swap(next, vect[d]);
        fib_heap_link(next, vect[d]);
        vect[d] = nullptr;
        d += 1;
      }
      vect[d] = next;
    }
    header = nullptr;
    for (auto i : vect) {
      if (i) {
        if (!header) {
          header = i;
          header->left = header;
          header->right = header;
        } else {
          insert_root_list(i);
          if (i->key < header->key) header = i;
        }
      }
    }
  }

  void out_of_list(link_type tar) {
    if (tar == tar->right) {
      if (tar->p)
        tar->p->child = nullptr;
      else
        header = nullptr;
      return;
    } else {
      if (tar->p) tar->p->child = tar->right;
    }
    auto lef = tar->left;
    auto righ = tar->right;
    lef->right = righ;
    righ->left = lef;
  }

  void fib_heap_link(link_type par, link_type ch) {
    out_of_list(ch);
    auto c = par->child;
    if (!c) {
      par->child = ch;
      ch->left = ch;
      ch->right = ch;
    } else {
      auto next = c->right;
      c->right = ch;
      ch->left = c;
      ch->right = next;
      next->left = ch;
    }
    ch->p = par;
    par->degree += 1;
  }

  void cut(link_type tar) {
    tar->p->degree -= 1;
    tar->mark = false;
    out_of_list(tar);
    tar->p = nullptr;
    insert_root_list(tar);
  }

  void cascading_cut(link_type tar) {
    auto par = tar->p;
    while (par) {
      if (!par->mark)
        par->mark = true;
      else {
        cut(tar);
        par = par->p;
      }
    }
  }

 private:
  template <typename... types>
  link_type create_node(types&&... args) {
    link_type node = alloc::allocate();
    alloc::construct(node, std::forward<types>(args)...);
    return (node);
  }

  void destroy_ande_free_node(link_type node) {
    alloc::destroy(node);
    alloc::deallocate(node);
  }

 private:
  link_type header;
  size_t n;
  Compare comp;
};

template <typename value_type, typename Compare, typename Alloc>
inline void swap(fib_heap<value_type, Compare, Alloc>& left,
                 fib_heap<value_type, Compare, Alloc>& right) noexcept {
  left.swap(right);
}
}

#endif