#pragma once
#ifndef _RB_TREE_
#define _RB_TREE_
#include <utility>
#include "allocator.h"
#include "iterator_qmj.h"

namespace qmj
{
typedef bool rbt_color_type;
constexpr rbt_color_type rbt_red = false;
constexpr rbt_color_type rbt_black = true;

template <typename value_type>
struct rb_tree_node;

template <typename value_type>
struct rb_tree_base_node
{
  typedef rb_tree_node<value_type> *link_type;

  rb_tree_base_node(rbt_color_type color = rbt_red, link_type p = nullptr,
                    link_type left = nullptr, link_type right = nullptr)
      : color(color), p(p), left(left), right(right) {}

  rbt_color_type color;
  link_type p;
  link_type left;
  link_type right;
};

template <typename value_type>
struct rb_tree_node : public rb_tree_base_node<value_type>
{
  typedef rb_tree_node<value_type> *link_type;
  typedef rb_tree_base_node<value_type> base_node_type;

  rb_tree_node(const value_type &val) : value(val) {}

  rb_tree_node(value_type &&val) : value(std::move(val)) {}

  rb_tree_node(rbt_color_type color = rbt_red, link_type p = nullptr,
               link_type left = nullptr, link_type right = nullptr)
      : base_node_type(color, p, left, right), value() {}

  rb_tree_node(const value_type &value, rbt_color_type color = rbt_red,
               link_type p = nullptr, link_type left = nullptr,
               link_type right = nullptr)
      : base_node_type(color, p, left, right), value(value) {}

  rb_tree_node(value_type &&value, rbt_color_type color = rbt_red,
               link_type p = nullptr, link_type left = nullptr,
               link_type right = nullptr)
      : base_node_type(color, p, left, right), value(std::move(value)) {}

  value_type value;

  static link_type minimum(link_type rt)
  {
    while (rt->left->p)
      rt = rt->left;
    return (rt);
  }

  static link_type maximum(link_type rt)
  {
    while (rt->right->p)
      rt = rt->right;
    return (rt);
  }
};

template <typename value_type_>
struct rb_tree_const_iterator
{
  template <typename traits>
  friend class rb_tree;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef value_type_ value_type;
  typedef const value_type &reference;
  typedef const value_type *pointer;
  typedef ptrdiff_t difference_type;

  typedef rb_tree_node<value_type> node_type;
  typedef rb_tree_node<value_type> *link_type;
  typedef rb_tree_const_iterator<value_type> self;

  rb_tree_const_iterator() : node(nullptr) {}

  rb_tree_const_iterator(link_type node) : node(node) {}

  rb_tree_const_iterator(const self &x) : node(x.node) {}

  bool operator==(const self &x) const { return node == x.node; }

  bool operator!=(const self &x) const { return (!(operator==(x))); }

  reference operator*() const { return (node->value); }

  pointer operator->() const { return (&(operator*())); }

  self &operator=(const self &x)
  {
    node = x.node;
    return (*this);
  }

  self &operator--()
  {
    if (node->left->p) // node have a left node that not nil
      node = node_type::maximum(node->left);
    else
    { // node have a left node is nil
      while (node->p->p && node->p->left == node)
        node = node->p;
      node = node->p;
    }
    return *this;
  }

  self operator--(int)
  {
    auto temp = *this;
    operator--();
    return (temp);
  }

  self &operator++()
  {
    if (node->right->p)
      node = node_type::minimum(node->right);
    else
    {
      while (node->p->p && node->p->right == node)
        node = node->p;
      node = node->p;
    }
    return (*this);
  }

  self operator++(int)
  {
    auto temp = *this;
    operator++();
    return temp;
  }

protected:
  link_type get_node() { return (node); }

protected:
  link_type node;
};

template <typename value_type_>
struct rb_tree_iterator : public rb_tree_const_iterator<value_type_>
{
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef value_type_ value_type;
  typedef value_type &reference;
  typedef value_type *pointer;
  typedef ptrdiff_t difference_type;

  typedef rb_tree_const_iterator<value_type> base_iterator;
  typedef typename base_iterator::node_type node_type;
  typedef typename base_iterator::link_type link_type;
  typedef rb_tree_iterator<value_type> self;

  rb_tree_iterator() : base_iterator() {}

  rb_tree_iterator(link_type node) : base_iterator(node) {}

  rb_tree_iterator(const self &x) : base_iterator(x.node) {}

  reference operator*() const { return (this->node->value); }

  pointer operator->() const { return (&(operator*())); }

  self &operator=(const self &x)
  {
    this->node = x.node;
    return (*this);
  }

  self &operator--()
  {
    if (this->node->left->p) // node have a left node that not nil
      this->node = node_type::maximum(this->node->left);
    else
    { // node have a left node is nil
      while (this->node->p->p && this->node->p->left == this->node)
        this->node = this->node->p;
      this->node = this->node->p;
    }
    return *this;
  }

  self operator--(int)
  {
    auto temp = *this;
    operator--();
    return temp;
  }

  self &operator++()
  {
    if (this->node->right->p)
      this->node = node_type::minimum(this->node->right);
    else
    {
      while (this->node->p->p && this->node->p->right == this->node)
        this->node = this->node->p;
      this->node = this->node->p;
    }
    return *this;
  }

  self operator++(int)
  {
    auto temp = *this;
    operator++();
    return temp;
  }
};

template <typename traits>
class rb_tree
{
public:
  typedef rb_tree<traits> self;
  typedef typename traits::key_type key_type;
  typedef typename traits::allocator_type allocator_type;
  typedef typename traits::value_compare value_compare;
  typedef size_t size_type;

  typedef typename traits::value_type value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef std::ptrdiff_t difference_type;

  typedef rb_tree_node<value_type> node_type;
  typedef rb_tree_base_node<value_type> base_node_type;
  typedef rb_tree_node<value_type> *link_type;
  typedef rb_tree_base_node<value_type> *base_link_type;
  typedef typename allocator_type::template rebind<node_type>::other alloc;
  typedef typename allocator_type::template rebind<base_node_type>::other
      alloc_type;
  enum
  { // get multi parameter
    is_multi = traits::is_multi
  };
  typedef typename traits::key_compare key_compare;
  typedef key_compare Compare;

  typedef rb_tree_const_iterator<value_type> const_iterator;
  typedef typename If<is_same<key_type, value_type>::value, const_iterator,
                      rb_tree_iterator<value_type>>::type iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::pair<iterator, bool> pairib;
  typedef std::pair<iterator, iterator> pairii;
  typedef std::pair<const_iterator, const_iterator> paircc;

  rb_tree() : nil(create_nil()), root(nil), comp(), node_count(0) {}

  explicit rb_tree(const Compare &comp)
      : nil(create_nil()), root(nil), comp(comp), node_count(0) {}

  rb_tree(const self &x)
      : nil(create_nil()), root(nil), comp(x.comp), node_count(0)
  {
    if (x.get_root() != x.get_nil())
      root = copy_assign(root, x.get_root(), x.get_nil());
    node_count = x.node_count;
    nil->left = maximum(get_root());
    nil->right = minimum(get_root());
  }

  rb_tree(self &&x)
      : nil(x.nil), root(x.root), comp(x.comp), node_count(x.node_count)
  {
    nil = root = nullptr;
    node_count = 0;
  }

  self &operator=(self x)
  {
    swap(x);
    return *this;
  }

  ~rb_tree()
  {
    if (!empty())
      rbt_destroy(get_root());
    destroy_nil();
  }

  void print_rbt() { print(get_root()); } //������

  iterator end() { return iterator(nil); }

  iterator begin() { return iterator(nil->right); }

  const_iterator end() const { return const_iterator(nil); }

  const_iterator begin() const { return const_iterator(nil->right); }

  const_iterator cend() const { return const_iterator(nil); }

  const_iterator cbegin() const { return const_iterator(nil->right); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rbeing() const
  {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(begin());
  }

  const_reverse_iterator crbegin() const
  {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator crend() const
  {
    return const_reverse_iterator(cbegin());
  }

  void clear()
  {
    if (!empty())
      rbt_destroy(get_root());
    root = nil;
    nil->left = nil->right = nil;
    node_count = 0;
  }

  void swap(self &x) noexcept
  {
    std::swap(nil, x.nil);
    std::swap(root, x.root);
    std::swap(comp, x.comp);
    std::swap(node_count, x.node_count);
  }

  size_type size() const { return (node_count); }

  bool empty() const { return (!(node_count)); }

  size_type count(const key_type &val) const
  {
    paircc range = equal_range(val);
    return (qmj::distance(range.first, range.second));
  }

  allocator_type get_allocator() const { return (allocator_type()); }

  key_compare key_comp() const { return (key_compare()); }

  value_compare value_comp() const { return (value_compare()); }

  size_type max_size() const { return size_type(-1); }

  iterator erase(const_iterator x)
  {
    const_iterator temp = x;
    const_iterator succ = x;
    ++succ;
    if (x == begin())
      nil->right = succ.get_node();
    if (x == --end())
      nil->left = (--temp).get_node();
    rbt_delete(x.get_node());
    return make_iter(succ);
  }

  size_type erase(const key_type &k)
  {
    pairii range = equal_range(k);
    size_type count = 0;
    for (iterator first = range.first, last = range.second; first != last;
         ++count)
      first = erase(first);
    return (count);
  }

  iterator erase(const_iterator first, const_iterator last)
  {
    for (; first != last;)
      first = erase(first);
    return make_iter(last);
  }

  const_iterator lower_bound(const key_type &key) const
  {
    return lower_bound_imple(key);
  }

  iterator lower_bound(const key_type &key)
  {
    return make_iter(lower_bound_imple(key));
  }

  const_iterator upper_bound(const key_type &key) const
  {
    return upper_bound_imple(key);
  }

  iterator upper_bound(const key_type &key)
  {
    return make_iter(upper_bound_imple(key));
  }

  paircc equal_range(const key_type &key) const
  {
    return {lower_bound(key), upper_bound(key)};
  }

  pairii equal_range(const key_type &key)
  {
    return {lower_bound(key), upper_bound(key)};
  }

  const_iterator find(
      const key_type &key) const
  { // may not first inserted element
    return find_imple(key);
  }

  iterator find(const key_type &key)
  { // may not first inserted element
    return make_iter(find_imple(key));
  }

  template <bool multi = is_multi>
  enable_if_t<multi, iterator> insert(const value_type &val)
  {
    return insert_equal_imple(get_root(), val);
  }

  template <bool multi = is_multi>
  enable_if_t<!multi, pairib> insert(const value_type &value)
  {
    return insert_unique_imple(get_root(), value);
  }

  template <bool multi = is_multi>
  enable_if_t<!multi, pairib> insert(value_type &&value)
  {
    return insert_unique_imple(get_root(), std::forward<value_type>(value));
  }

  template <bool multi = is_multi>
  enable_if_t<multi, iterator> insert(value_type &&val)
  {
    return insert_equal_imple(get_root(), std::forward<value_type>(val));
  }

  template <typename Iter>
  void insert(Iter first, Iter last)
  {
    for (; first != last; ++first)
      insert(*first);
  }

  void insert(const std::initializer_list<value_type> &lst)
  {
    insert(lst.begin(), lst.end());
  }

  iterator insert(const_iterator pos, value_type &&val)
  {
    return (emplace_hint(pos, std::forward<value_type>(val)));
  }

  iterator insert(const_iterator pos, const value_type &val)
  {
    return (emplace_hint(pos, val));
  }

  template <bool multi = is_multi, typename... types>
  enable_if_t<multi, iterator> emplace(types &&... args)
  {
    return insert_equal_imple(get_root(), std::forward<types>(args)...);
  }

  template <bool multi = is_multi, typename... types>
  enable_if_t<!multi, pairib> emplace(types &&... args)
  {
    return insert_unique_imple(get_root(), std::forward<types>(args)...);
  }

  template <bool multi = is_multi, typename... types>
  enable_if_t<!multi, iterator> emplace_hint(const_iterator pos,
                                             types &&... args)
  {
    link_type tar = create_insert_node(nil, std::forward<types>(args)...);
    const value_type &value = tar->value;
    link_type cur = pos.get_node();
    if (cur == nil)
    {
      if (comp(get_key(back()), get_key(value)))
      {
        tar->p = nil->left;
        return (insert_imple(tar->p, tar));
      }
    }
    else if (comp(get_key(value), get_key(*pos)))
    {
      if (cur == nil->right || comp(get_key(*--pos), get_key(value)))
      {
        if (cur->left == nil)
          tar->p = cur;
        else
          tar->p = cur = pos.get_node();
        return (insert_imple(cur, tar));
      }
    }
    cur = get_root();
    link_type par = nil;
    while (cur != nil)
    {
      par = cur;
      if (comp(get_key(value), get_key(cur->value)))
        cur = cur->left;
      else if (comp(get_key(cur->value), get_key(value)))
        cur = cur->right;
      else
      {
        destroy_and_free_node(tar);
        return (iterator(cur));
      }
    }
    tar->p = par;
    return (insert_imple(par, tar));
  }

  template <bool multi = is_multi, typename value_type>
  enable_if_t<!multi, iterator> emplace_hint(const_iterator pos,
                                             value_type &&value)
  {
    link_type cur = pos.get_node();
    if (cur == nil)
    {
      if (comp(get_key(back()), get_key(value)))
      {
        link_type tar =
            create_insert_node(nil->left, std::forward<value_type>(value));
        return (insert_imple(tar->p, tar));
      }
    }
    else if (comp(get_key(value), get_key(*pos)))
    {
      if (cur == nil->right || comp(get_key(*--pos), get_key(value)))
      {
        if (cur->left != nil)
          cur = pos.get_node();
        link_type tar =
            create_insert_node(cur, std::forward<value_type>(value));
        return (insert_imple(cur, tar));
      }
    }
    cur = get_root();
    link_type par = nil;
    while (cur != nil)
    {
      par = cur;
      if (comp(get_key(value), get_key(cur->value)))
        cur = cur->left;
      else if (comp(get_key(cur->value), get_key(value)))
        cur = cur->right;
      else
        return (iterator(cur));
    }
    link_type tar = create_insert_node(par, std::forward<value_type>(value));
    return (insert_imple(par, tar));
  }

  template <bool multi = is_multi, typename... types>
  enable_if_t<multi, iterator> emplace_hint(
      const_iterator pos,
      types &&... args)
  { // using args to construct value_type try to insert
    // before pos
    link_type tar = create_insert_node(nil, std::forward<types>(args)...);
    const value_type &value = tar->value;
    link_type cur = pos.get_node();
    if (!empty())
    { // tree is not empty
      if (cur == nil)
      { // insert after max ele
        if (!comp(get_key(value),
                  get_key(back())))
        { // pos is a good position to insert
          tar->p = nil->left;
          tar->p->right = tar;
          nil->left = tar;
          return (rbt_insert_fixup(tar));
        }
      }
      else if (!comp(get_key(cur->value),
                     get_key(value)))
      { // pos may be is a good pos to insert
        // straightly before
        if (cur == nil->right)
        {
          tar->p = cur;
          cur->left = tar;
          nil->right = tar;
          return (rbt_insert_fixup(tar));
        }
        else
        {
          const_iterator tmp = pos;
          if (!(comp(get_key(value), get_key(*--tmp))))
          {
            if (cur->left == nil)
            {
              tar->p = cur;
              cur->left = tar;
              return (rbt_insert_fixup(tar));
            }
            else
            {
              tar->p = tmp.get_node();
              tar->p->right = tar;
              return (rbt_insert_fixup(tar));
            }
          }
        }
      }
      else
      {
        const_iterator new_pos = lower_bound(get_key(value));
        destroy_and_free_node(tar);
        return emplace_hint(new_pos, std::forward<types>(args)...);
      }
    }
    cur = get_root();
    link_type par = nil;
    while (cur != nil)
    {
      par = cur;
      cur = comp(get_key(value), get_key(par->value)) ? par->left : par->right;
    }
    tar->p = par;
    return insert_imple(par, tar);
  }

protected:
  void print(link_type rt, int counter_hight = 0) //������
  {
    if (rt != nil)
    {
      print(rt->right, counter_hight + 1);
      for (auto n = counter_hight; n != -1; --n)
        std::cout << "\t";
      std::cout << rt->value;
      rt->color ? std::cout << "|��" << std::endl : std::cout << "|��" << std::endl;
      print(rt->left, counter_hight + 1);
      std::cout << std::endl;
    }
  }

protected:
  const key_type &get_key(const value_type &val) const
  {
    return (traits::keyOfValue(val));
  }

  template <typename... types>
  link_type create_node(types &&... args)
  {
    link_type ptr = alloc::allocate();
    alloc_type::construct((base_link_type)ptr, std::forward<types>(args)...);
    return (ptr);
  }

  template <typename... types>
  void construct_value(link_type ptr, types &&... args)
  {
    allocator_type::construct((&(ptr->value)), std::forward<types>(args)...);
  }

  template <typename... types>
  link_type create_insert_node(link_type par, types &&... args)
  {
    link_type tar = alloc::allocate();
    alloc::construct(tar, std::forward<types>(args)..., rbt_red, par, nil, nil);
    return (tar);
  }

  void destroy_and_free_node(link_type node)
  {
    allocator_type::destroy((&(node->value)));
    alloc::deallocate(node);
  }

  void rbt_delete(link_type tar);

  link_type get_root() const { return (root); }

  link_type get_nil() const { return (nil); }

  link_type create_nil() const
  {
    link_type ptr = (link_type)alloc_type::allocate();
    alloc_type::construct(ptr, rbt_black, nullptr, ptr, ptr);
    return (ptr);
  }

  void destroy_nil() const { alloc_type::deallocate((base_link_type)nil); }

  static link_type minimum(link_type rt) { return node_type::minimum(rt); }

  static link_type maximum(link_type rt) { return node_type::maximum(rt); }

  void rbt_left_rotate(link_type x);

  void rbt_right_rotate(link_type x);

  void rbt_destroy(link_type x);

  void rbt_delete_fixup(link_type x);

  iterator rbt_insert_fixup(link_type tar);

  const_iterator lower_bound_imple(const key_type &key) const;

  const_iterator upper_bound_imple(const key_type &key) const;

  const_iterator find_imple(const key_type &key) const;

  link_type copy_assign(link_type par, const link_type cur,
                        const link_type nl)
  {
    link_type my = create_node(cur->color, par);
    construct_value(my, cur->value);
    my->left = cur->left != nl ? copy_assign(my, cur->left, nl) : nil;
    my->right = cur->right != nl ? copy_assign(my, cur->right, nl) : nil;
    return (my);
  }

  void rbt_transplant(link_type lhs, link_type rhs)
  {
    if (lhs->p == nil)
      root = rhs;
    else if (lhs == lhs->p->left)
      lhs->p->left = rhs;
    else
      lhs->p->right = rhs;
    rhs->p = lhs->p;
  }

  template <typename value_type>
  iterator insert_euqal_imple(link_type cur, value_type &&value)
  {
    link_type par = nil;
    while (cur != nil)
    {
      par = cur;
      cur = comp(get_key(value), get_key(par->value)) ? par->left : par->right;
    }
    link_type tar = create_insert_node(par, std::forward<value_type>(value));
    return insert_imple(par, tar);
  }

  template <typename... types>
  iterator insert_equal_imple(link_type cur, types &&... args)
  {
    link_type par = nil;
    link_type tar = create_insert_node(par, std::forward<types>(args)...);
    const value_type &val = tar->value;
    while (cur != nil)
    {
      par = cur;
      cur = comp(get_key(val), get_key(par->value)) ? par->left : par->right;
    }
    tar->p = par;
    return insert_imple(par, tar);
  }

  template <typename value_type>
  pairib insert_unique_imple(link_type cur, value_type &&value)
  {
    link_type par = nil;
    while (cur != nil)
    {
      par = cur;
      if (comp(get_key(value), get_key(cur->value)))
        cur = cur->left;
      else if (comp(get_key(cur->value), get_key(value)))
        cur = cur->right;
      else
        return pairib(iterator(cur), false);
    }
    link_type tar = create_insert_node(par, std::forward<value_type>(value));
    return {(insert_imple(par, tar)), true};
  }

  template <typename... types>
  pairib insert_unique_imple(link_type cur, types &&... args)
  {
    link_type par = nil;
    link_type tar = create_insert_node(par, std::forward<types>(args)...);
    const value_type &value = tar->value;
    while (cur != nil)
    {
      par = cur;
      if (comp(get_key(value), get_key(cur->value)))
        cur = cur->left;
      else if (comp(get_key(cur->value), get_key(value)))
        cur = cur->right;
      else
      {
        destroy_and_free_node(tar);
        return pairib(iterator(cur), false);
      }
    }
    tar->p = par;
    return {insert_imple(par, tar), true};
  }

  iterator insert_imple(link_type par, link_type tar)
  {
    if (par == nil)
      root = tar;
    else if (comp(get_key(tar->value), get_key(par->value)))
      par->left = tar;
    else
      par->right = tar;
    update_extremum(tar);
    return rbt_insert_fixup(tar);
  }

  void update_extremum(link_type tar)
  {
    if (nil->left != nil)
    {
      if (comp(get_key(tar->value), get_key(nil->right->value)))
        nil->right = tar;
      else if (!comp(get_key(tar->value), get_key(nil->left->value)))
        nil->left = tar;
    }
    else
      nil->left = nil->right = tar;
  }

  reference front() const { return (nil->right->value); }

  reference back() const { return (nil->left->value); }

  iterator make_iter(const_iterator citer) const
  {
    return iterator(citer.get_node());
  }

private:
  link_type nil;
  link_type root;
  size_type node_count;
  Compare comp;
};

template <typename traits>
void rb_tree<traits>::rbt_destroy(link_type x)
{
  if (x->left != nil)
    rbt_destroy(x->left);
  if (x->right != nil)
    rbt_destroy(x->right);
  destroy_and_free_node(x);
}

template <typename traits>
typename rb_tree<traits>::const_iterator rb_tree<traits>::lower_bound_imple(
    const key_type &key) const
{
  auto cur = get_root();
  auto result = cur;
  while (cur != nil)
  {
    result = cur;
    if (comp(get_key(cur->value), key))
      cur = cur->right;
    else
      cur = cur->left;
  }
  if (result == nil)
    return (cend());
  else if (comp(get_key(result->value), key))
    return (++const_iterator(result));
  else
    return const_iterator(result);
}

template <typename traits>
typename rb_tree<traits>::const_iterator rb_tree<traits>::upper_bound_imple(
    const key_type &key) const
{
  link_type cur = get_root();
  link_type result = cur;
  while (cur != nil)
  {
    result = cur;
    if (comp(key, get_key(cur->value)))
      cur = cur->left;
    else
      cur = cur->right;
  }
  if (result == nil)
    return (cend());
  else if (comp(key, get_key(result->value)))
    return const_iterator(result);
  else
    return (++const_iterator(result));
}

template <typename traits>
typename rb_tree<traits>::const_iterator rb_tree<traits>::find_imple(
    const key_type &key) const
{
  link_type cur = get_root();
  while (cur != nil)
  {
    if (comp(key, get_key(cur->value)))
      cur = cur->left;
    else if (comp(get_key(cur->value), key))
      cur = cur->right;
    else
      return const_iterator(cur);
  }
  return (cend());
}

template <typename traits>
typename rb_tree<traits>::iterator rb_tree<traits>::rbt_insert_fixup(
    link_type tar)
{
  iterator iter = iterator(tar);
  while (!tar->p->color)
  {
    auto grandpar = tar->p->p;
    if (tar->p == grandpar->left)
    {
      if (!grandpar->right->color)
      {
        grandpar->left->color = rbt_black;
        grandpar->right->color = rbt_black;
        grandpar->color = rbt_red;
        tar = grandpar;
      }
      else
      {
        if (tar == tar->p->right)
        {
          rbt_left_rotate(tar->p);
          tar = tar->left;
        }
        grandpar->color = rbt_red;
        grandpar->left->color = rbt_black;
        rbt_right_rotate(grandpar);
      }
    }
    else
    {
      if (!grandpar->left->color)
      {
        grandpar->right->color = rbt_black;
        grandpar->left->color = rbt_black;
        grandpar->color = rbt_red;
        tar = grandpar;
      }
      else
      {
        if (tar == tar->p->left)
        {
          rbt_right_rotate(tar->p);
          tar = tar->right;
        }
        grandpar->color = rbt_red;
        grandpar->right->color = rbt_black;
        rbt_left_rotate(grandpar);
      }
    }
  }
  root->color = rbt_black;
  ++node_count;
  return (iter);
}

template <typename traits>
void rb_tree<traits>::rbt_delete(link_type tar)
{ //ɾ���ڵ�
  if (tar == nil)
    return;
  link_type y = tar;
  rbt_color_type y_original_color = y->color;
  link_type x;
  if (tar->left == nil)
  {
    x = tar->right;
    rbt_transplant(tar, x);
  }
  else if (tar->right == nil)
  {
    x = tar->left;
    rbt_transplant(tar, x);
  }
  else
  {
    y = minimum(tar->right);
    y_original_color = y->color;
    x = y->right;
    if (y->p == tar)
      x->p = y; //��x���ڱ�ʱΪɾ����׼��
    else
    {
      rbt_transplant(y, x);
      y->right = tar->right;
      y->right->p = y;
    }
    rbt_transplant(tar, y);
    y->left = tar->left;
    y->left->p = y;
    y->color = tar->color;
  }
  --node_count;
  if (y_original_color)
    rbt_delete_fixup(x);
  nil->p = nullptr;
  destroy_and_free_node(tar);
}

template <typename traits>
void rb_tree<traits>::rbt_delete_fixup(link_type x)
{
  while (x != root && x->color)
  {
    if (x == x->p->left)
    {
      link_type w = x->p->right;
      if (!w->color)
      { //���1������ֵܽڵ��Ǻ�ɫ
        x->p->color = rbt_red;
        w->color = rbt_black;
        rbt_left_rotate(x->p);
        w = x->p->right;
      }

      if (w->right->color &&
          w->left->color)
      { //��������ֵܽڵ�������ӽڵ㶼�Ǻ�ɫ
        w->color = rbt_red;
        x = x->p;
      }
      else if (w->right->color)
      { //�ֵܽڵ����ڵ�Ϊ��ɫ
        w->color = rbt_red;
        w->left->color = rbt_black;
        rbt_right_rotate(w);
        w = x->p->right;
      }
      if (!w->right->color)
      { //�ֵܽڵ���ҽڵ�Ϊ��ɫ
        w->color = x->p->color;
        x->p->color = rbt_black;
        w->right->color = rbt_black;
        rbt_left_rotate(x->p);
        x = root;
      }
    }
    else
    {
      link_type w = x->p->left;
      if (!w->color)
      {
        x->p->color = rbt_red;
        w->color = rbt_black;
        rbt_right_rotate(x->p);
        w = x->p->left;
      }
      if (w->right->color && w->left->color)
      {
        w->color = rbt_red;
        x = x->p;
      }
      else if (w->left->color)
      {
        w->color = rbt_red;
        w->right->color = rbt_black;
        rbt_left_rotate(w);
        w = x->p->left;
      }
      if (!w->left->color)
      {
        w->color = x->p->color;
        x->p->color = rbt_black;
        w->left->color = rbt_black;
        rbt_right_rotate(x->p);
        x = root;
      }
    }
  }
  x->color = rbt_black;
}

template <typename traits>
void rb_tree<traits>::rbt_left_rotate(link_type x)
{ //����
  auto xRight = x->right;
  xRight->p = x->p;

  if (xRight->left != nil)
    xRight->left->p = x;
  x->right = xRight->left;

  if (x->p == nil)
    root = xRight;
  else if (x->p->left == x)
    x->p->left = xRight;
  else
    x->p->right = xRight;
  xRight->left = x;
  x->p = xRight;
}

template <typename traits>
void rb_tree<traits>::rbt_right_rotate(link_type x)
{ //����
  auto xLeft = x->left;
  xLeft->p = x->p;

  if (xLeft->right != nil)
    xLeft->right->p = x;
  x->left = xLeft->right;

  if (x->p == nil)
    root = xLeft;
  else if (x->p->left == x)
    x->p->left = xLeft;
  else
    x->p->right = xLeft;
  xLeft->right = x;
  x->p = xLeft;
}

template <typename traits>
inline bool operator==(const rb_tree<traits> &left,
                       const rb_tree<traits> &right)
{
  return (left.size() == right.size() && std::equal(left.begin(), right.end()));
}

template <typename traits>
inline bool operator!=(const rb_tree<traits> &left,
                       const rb_tree<traits> &right)
{
  return (!(left == right));
}

template <typename traits>
inline bool operator<(const rb_tree<traits> &left,
                      const rb_tree<traits> &right)
{
  return std::lexicographical_compare(left.begin(), left.end(), right.begin(),
                                      right.end());
}

template <typename traits>
inline bool operator>(const rb_tree<traits> &left,
                      const rb_tree<traits> &right)
{
  return (right < left);
}

template <typename traits>
inline bool operator<=(const rb_tree<traits> &left,
                       const rb_tree<traits> &right)
{
  return (!(right < left));
}

template <typename traits>
inline bool operator>=(const rb_tree<traits> &left,
                       const rb_tree<traits> &right)
{
  return (!(left < right));
}
} // namespace qmj

#endif
