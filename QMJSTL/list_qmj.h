#pragma once
#ifndef _LIST_QMJ_
#define _LIST_QMJ_
#include<initializer_list>
#include"allocator.h"

namespace qmj
{

	template<typename value_type>
	struct list_node;

	template<typename value_type>
	struct list_base_node
	{
		typedef list_node<value_type>* link_type;

		list_base_node(link_type prev = nullptr, link_type next = nullptr) :
			prev(prev), next(next)
		{
		}

		link_type prev;
		link_type next;
	};


	template<typename value_type>
	struct list_node :
		public list_base_node<value_type>
	{
		typedef list_node<value_type>* link_type;
		typedef list_base_node<value_type> base_type;

		list_node(link_type prev = nullptr, link_type next = nullptr) :
			base_type(prev, next), data()
		{
		}

		list_node(link_type prev,
			link_type next,
			const value_type&data) :
			base_type(prev,next), data(data)
		{
		}

		value_type data;
	};

	template<typename value_type>
	class list_const_iterator
	{
	public:
		template<typename value_type, typename alloc>
		friend class slist;

		typedef std::bidirectional_iterator_tag iterator_category;
		typedef value_type value_type;
		typedef const value_type* pointer;
		typedef const value_type& reference;
		typedef ptrdiff_t difference_type;

		typedef list_const_iterator<value_type> self;
		typedef list_node<value_type>* link_type;
		typedef size_t size_type;

		list_const_iterator(link_type node = nullptr) :
			node(node)
		{
		}

		list_const_iterator(const self&x) :node(x.node)
		{
		}

		self& operator=(const self &x)
		{
			node = x.node;
			return (*this);
		}

		bool operator==(const self&x)const
		{
			return (node == x.node);
		}

		bool operator!=(const self&x)const
		{
			return !operator==(x);
		}

		reference operator*()const
		{
			return node->data;
		}

		pointer operator->()const
		{
			return &(operator*());
		}

		self& operator++()
		{
			node = node->next;
			return *this;
		}

		self operator++(int)
		{
			auto ret = *this;
			++*this;
			return ret;
		}

		self& operator--()
		{
			node = node->prev;
			return *this;
		}

		self operator--(int)
		{
			auto ret = *this;
			++*this;
			return ret;
		}
	protected:
		link_type get_node()
		{
			return node;
		}

	protected:
		link_type node;
	};

	template<typename value_type>
	class list_iterator :
		public list_const_iterator<value_type>
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef value_type value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef ptrdiff_t difference_type;

		typedef list_iterator<value_type> self;
		typedef list_const_iterator<value_type> iterator_base;
		typedef list_node<value_type>* link_type;
		typedef size_t size_type;

		list_iterator(const link_type node = nullptr) :
			iterator_base(node)
		{
		}

		list_iterator(const self&x) :iterator_base(x.node)
		{
		}

		self& operator=(const self &x)
		{
			node = x.node;
			return (*this);
		}

		bool operator==(const self&x)const
		{
			return node == x.node;
		}

		bool operator!=(const self&x)const
		{
			return !operator==(x);
		}

		reference operator*()const
		{
			return node->data;
		}

		pointer operator->()const
		{
			return &(operator*());
		}

		self& operator++()
		{
			node = node->next;
			return *this;
		}

		self operator++(int)
		{
			auto ret = *this;
			++*this;
			return ret;
		}

		self& operator--()
		{
			node = node->prev;
			return *this;
		}

		self operator--(int)
		{
			auto ret = *this;
			++*this;
			return ret;
		}
	};

	template<typename value_type,
		typename Alloc = _QMJ allocator<value_type>>
		class slist
	{//is bidirect list,not forward_list,splice is O(1),size is O(n)
	public:
		typedef slist<value_type, Alloc> self;
		typedef Alloc allocator_type;
		typedef list_node<value_type>* link_type;
		typedef typename allocator_type::template rebind<list_node<value_type>>::other alloc;
		typedef list_base_node<value_type> base_node_type;
		typedef base_node_type* base_link_type;
		typedef typename allocator_type::template rebind<base_node_type>::other alloc_type;

		typedef value_type value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t differene_type;

		typedef list_iterator<value_type> iterator;
		typedef list_const_iterator<value_type> const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		slist()
		{
			empty_init();
		}

		template<typename Iter,
			typename = typename enable_if<is_iterator<Iter>::value, void>::type>
			slist(Iter first, Iter last) :slist()
		{
			assign(first, last);
		}

		explicit slist(const size_type n) :slist()
		{
			resize(n);
		}

		slist(const size_type n, const value_type&val) :slist()
		{
			insert(begin(), n, val);
		}

		slist(const std::initializer_list<value_type>&lst) :slist()
		{
			assign(lst.begin(), lst.end());
		}

		slist(const self&x) :slist()
		{
			insert(begin(), x.begin(), x.end());
		}

		slist(self&&x) :slist()
		{
			std::swap(node, x.node);
		}

		self& operator=(self x)
		{
			swap(x);
			return (*this);
		}

		self& operator=(const std::initializer_list<value_type>&lst)
		{
			assign(lst.begin(), lst.end());
			return (*this);
		}

		~slist()
		{
			clear();
			alloc_type::deallocate((base_link_type)node);
		}

		void swap(self&x)noexcept
		{
			std::swap(node, x.node);
		}

		iterator erase(const_iterator pos)
		{
			const_iterator next = pos;
			++next;
			erase_imple(pos);
			return make_iter(next);
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			while (first != last)
				first = erase(first);
			return make_iter(last);
		}

		void resize(const size_type new_size)
		{
			size_type n = size();
			if (n < new_size)
				for (; n != new_size; ++n)
					insert_imple(end());
			else
				for (; n != new_size; --n)
					pop_back();
		}

		void resize(const size_type new_size, const value_type&val)
		{
			const size_type n = size();
			if (n < new_size)
				insert_n(end(), n - new_size, val);
			else
				for (; n != new_size; --n)
					pop_back();
		}

		allocator_type get_allocator()const
		{
			return allocator_type();
		}

		void pop_front()
		{
			erase(begin());
		}

		void pop_back()
		{
			erase(--end());
		}

		size_type max_size()const
		{
			return size_type(-1);
		}

		void clear();

		void unique()
		{
			unique(std::equal_to<value_type>());
		}

		template<typename Pred>
		void unique(const Pred& pred)
		{
			iterator first = begin();
			iterator last = end();
			if (first == last)return;
			for (iterator next = first; ++next != last;)
			{
				if (pred(*next, *first))
					erase(next);
				else
					first = next;
				next = first;
			}
		}

		void remove(const value_type&val);

		void reverse();

		void sort()
		{
			sort(std::less<value_type>());
		}

		template<typename Compare>
		void sort(const Compare cmp)
		{
			merge_sort(begin(), end(), size(), cmp);
		}

		template<typename Pred>
		void remove_if(const Pred&pred)
		{
			iterator ed = end();
			for (iterator bg = begin(); bg != ed;)
				if (pred(*bg))
					bg = erase(bg);
				else
					++bg;
		}

		iterator insert(const_iterator pos, const value_type&val)
		{
			insert_imple(pos, val);
			return make_iter(--pos);
		}

		iterator insert(const_iterator pos, value_type&&val)
		{
			return emplace(pos, std::move(val));
		}

		iterator insert(const_iterator pos, size_type n, const value_type&val)
		{
			const_iterator prev = pos;
			if (prev == cbegin())
			{
				insert_n(pos, n, val);
				return begin();
			}
			else
			{
				--prev;
				insert_n(pos, n, val);
				return make_iter(++prev);
			}
		}

		template<typename Iter>
		typename enable_if<is_iterator<Iter>::value, iterator>::type
			insert(const_iterator pos, Iter first, Iter last)
		{
			const_iterator prev = pos;
			if (prev == cbegin())
			{
				insert_range
				(pos, first, last, _QMJ iterator_category(first));
				return begin();
			}
			else
			{
				--prev;
				insert_range
				(pos, first, last, _QMJ iterator_category(first));
				return make_iter(++prev);
			}
		}

		iterator insert(const_iterator pos, const std::initializer_list<value_type>&lst)
		{
			return insert(pos, lst.begin(), lst.end());
		}

		template<typename Iter>
		typename enable_if<is_iterator<Iter>::value, void>::type
			assign(Iter first, Iter last)
		{
			assign_imple
			(first, last, _QMJ iterator_category(first));
		}

		void assign(const std::initializer_list<value_type>&lst)
		{
			assign(lst.begin(), lst.end());
		}

		void assign(size_type n, const value_type&val)
		{
			iterator bg = begin();
			iterator ed = end();
			for (; bg != ed&&n != 0; --n, ++bg)
				reuseNode(bg, val);
			for (; n != 0; --n)
				insert_imple(bg, val);
			erase(bg, ed);
		}

		void push_back(const value_type&val)
		{
			insert_imple(end(), val);
		}

		void push_back(value_type&&val)
		{
			insert_imple(end(), std::forward<value_type>(val));
		}

		bool empty()const
		{
			return node->next == node;
		}

		size_type size()const
		{
			return std::distance(begin(), end());
		}

		iterator begin()
		{
			return iterator(node->next);
		}

		iterator end()
		{
			return iterator(node);
		}

		const_iterator begin()const
		{
			return const_iterator(node->next);
		}

		const_iterator end()const
		{
			return const_iterator(node);
		}

		const_iterator cbegin()const
		{
			return (begin());
		}

		const_iterator cend()const
		{
			return end();
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator crbegin()const
		{
			return const_reverse_iterator(cend());
		}

		const_reverse_iterator crend()const
		{
			return const_reverse_iterator(cbegin());
		}

		reference front()
		{
			return *begin();
		}

		const_reference front()const
		{
			return *begin();
		}

		reference back()
		{
			return *(--end());
		}

		const_reference back()const
		{
			return *(--end());
		}

		void splice(const_iterator pos, self&x)
		{
			if (!x.empty())
				transfer(pos, x.begin(), x.end());
		}

		void splice(const_iterator pos, self&&x)
		{
			splice(pos, (self&)x)
		}

		void splice(const_iterator pos, self&&x, const_iterator first)
		{
			splice(pos, (self&)x, first);
		}

		void splice(const_iterator pos, self&lst, const_iterator tar)
		{
			const_iterator last = tar;
			if (pos != tar)
				transfer(pos, tar, ++last);
		}

		void splice(const_iterator pos, self&&x, const_iterator first, const_iterator last)
		{
			splice(pos, (self&)x, first, last);
		}

		void splice(const_iterator pos, self& lst, const_iterator first, const_iterator last)
		{
			if (first != pos)
				transfer(pos, first, last);
		}

		void push_front(const value_type& val)
		{
			insert_imple(begin(), val);
		}

		void push_front(value_type&&val)
		{
			insert_imple(begin(), std::forward<value_type>(val));
		}

		template<typename...types>
		void emplace_front(types&&...args)
		{
			insert_imple(begin(), std::forward<types>(args)...);
		}

		template<typename...types>
		void emplace_back(types&&...args)
		{
			insert_imple(end(), std::forward<types>(args)...);
		}

		template<typename...types>
		iterator emplace(const_iterator pos, types&&...args)
		{
			insert_imple(pos, std::forward<types>(args)...);
			return make_iter(--pos);
		}

		void merge(self&x)
		{
			merge(x, std::less<value_type>());
		}

		void merge(self&&x)
		{
			merge((self&)x, std::less<value_type>());
		}

		template<typename Compare>
		void merge(self&&lst, const Compare& cmp)
		{
			merge((self&)lst, cmp);
		}

		template<typename Compare>
		void merge(self&lst, const Compare& cmp)
		{
			iterator first1 = begin();
			iterator last1 = end();
			iterator first2 = lst.begin();
			iterator last2 = lst.end();
			if (first1 == last1)
				splice(end(), lst);
			else if (first2 == last2)
				return;
			else
				for (;;)
				{
					if (cmp(*first2, *first1))
					{
						iterator next = first2;
						transfer(first1, first2, ++next);
						first2 = next;
						if (first2 == last2)
							return;
					}
					else
					{
						if (++first1 == last1)
						{
							if (first2 != last2)
								transfer(last1, first2, last2);
							return;
						}
					}
				}
		}

	protected:
		void transfer(const_iterator pos, const_iterator first, const_iterator last)
		{
			if (pos != last)
			{
				first.get_node()->prev->next = last.get_node();
				last.get_node()->prev->next = pos.get_node();
				link_type tmp = pos.get_node()->prev;
				tmp->next = first.get_node();
				pos.get_node()->prev = last.get_node()->prev;
				last.get_node()->prev = first.get_node()->prev;
				first.get_node()->prev = tmp;
			}
		}

		void insert_n(const_iterator pos, size_type n, const value_type&val)
		{
			for (; n != 0; --n)
				insert_imple(pos, val);
		}

		template<typename...types>
		void insert_imple(const_iterator pos, types&&...args)
		{
			link_type cur = pos.get_node();
			link_type tmp = create_node(cur, std::forward<types>(args)...);
			cur->prev->next = tmp;
			cur->prev = tmp;
		}

		void erase_imple(const_iterator pos)
		{
			link_type next_node = pos.get_node()->next;
			link_type prev_node = pos.get_node()->prev;
			prev_node->next = next_node;
			next_node->prev = prev_node;
			destroy_and_free_node(pos.get_node());
		}

		template<typename...types>
		link_type create_node(link_type cur, types&&...args)
		{
			link_type ptr = alloc::allocate();
			alloc::construct(ptr, cur->prev, cur, std::forward<types>(args)...);
			return (ptr);
		}

		void destroy_and_free_node(link_type ptr)
		{
			alloc::destroy(&(ptr->data));
			alloc::deallocate(ptr);
		}

		template<typename type>
		void reuseNode(const_iterator pos, type&&val)
		{
			allocator_type::destroy(&(pos.get_node()->data));
			allocator_type::construct(&(pos.get_node()->data), std::forward<type>(val));
		}

		void empty_init()
		{
			node = (link_type)alloc_type::allocate();
			alloc_type::construct(node, node, node);
		}

		template<typename Compare = std::less<value_type>>
		iterator merge_sort
		(iterator first, iterator last, size_type len, Compare& cmp = Compare())
		{
			if (len >= 2)
			{
				iterator med = first;
				size_type half = len >> 1;
				_QMJ advance(med, half);
				first = merge_sort(first, med, half, cmp);
				med = merge_sort(med, last, len - half, cmp);
				iterator new_first = first;
				if (cmp(*med, *first))
					new_first = med;

				for (;;)
				{
					if (cmp(*med, *first))
					{
						splice(first, *this, med++);
						if (med == last)
							return new_first;
					}
					else
					{
						if (++first == med)
							return new_first;
					}
				}
			}
			return first;
		}

		iterator make_iter(const_iterator iter)const
		{
			return iterator(iter.get_node());
		}
	private:
		link_type node;
	};

	template<typename value_type, typename alloc>
	void slist<value_type, alloc>::clear()
	{
		for (link_type cur = node->next; cur != node;)
		{
			link_type tmp = cur->next;
			destroy_and_free_node(cur);
			cur = tmp;
		}
		node->next = node;
		node->prev = node;
	}

	template<typename value_type, typename alloc>
	void slist<value_type, alloc>::remove(const value_type&val)
	{
		auto first = begin();
		auto last = end();
		while (first != last)
		{
			if (*first == val)
				first = erase(first);
			else
				++first;
		}
	}

	template<typename value_type, typename alloc>
	void slist<value_type, alloc>::reverse()
	{
		link_type cur = node;
		do
		{
			auto next = cur->next;
			cur->next = cur->prev;
			cur->prev = next;
			cur = next;
		} while (cur != node);
	}

	template<typename value_type, typename alloc>inline
		void swap(_QMJ slist<value_type, alloc>&left,
			_QMJ slist<value_type, alloc>&right) noexcept
	{
		left.swap(right);
	}

	template<typename value_type, typename alloc>inline
		bool operator==(const _QMJ slist<value_type, alloc>&left,
			const _QMJ slist<value_type, alloc>&right)
	{
		return (std::equal(left.begin(), left.end(), right.begin(), right.end()));
	}

	template<typename value_type, typename alloc>inline
		bool operator!=(const _QMJ slist<value_type, alloc>&left,
			const _QMJ slist<value_type, alloc>&right)
	{
		return !(left == right);
	}

	template<typename value_type, typename alloc>inline
		bool operator<(const _QMJ slist<value_type, alloc>&left,
			const _QMJ slist<value_type, alloc>&right)
	{
		return std::lexicographical_compare
		(left.begin(), left.end(), right.begin(), right.end());
	}

	template<typename value_type, typename alloc>inline
		bool operator<=(const _QMJ slist<value_type, alloc>&left,
			const _QMJ slist<value_type, alloc>&right)
	{
		return !(right < left);
	}

	template<typename value_type, typename alloc>inline
		bool operator>(const _QMJ slist<value_type, alloc>&left,
			const _QMJ slist<value_type, alloc>&right)
	{
		return right < left;
	}

	template<typename value_type, typename alloc>inline
		bool operator>=(const _QMJ slist<value_type, alloc>&left,
			const _QMJ slist<value_type, alloc>&right)
	{
		return !(left < right);
	}
}

namespace qmj
{
	template<typename value_type, typename Alloc = _QMJ allocator<value_type>>
	class list :
		public slist<value_type, Alloc>
	{
	public:
		typedef list<value_type, Alloc> self;
		typedef slist<value_type, Alloc> base_type;
		typedef Alloc allocator_type;
		typedef list_node<value_type>* link_type;
		typedef _QMJ allocator<list_node<value_type>> alloc;
		typedef list_base_node<value_type> base_node_type;
		typedef base_node_type* base_link_type;

		typedef value_type value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t differene_type;

		typedef typename base_type::iterator iterator;
		typedef typename base_type::const_iterator const_iterator;
		typedef typename base_type::reverse_iterator reverse_iterator;
		typedef typename base_type::const_reverse_iterator const_reverse_iterator;
		using base_type::make_iter;

		list() :
			base_type(), node_count(0)
		{
		}

		template<typename Iter,
			typename = typename enable_if<is_iterator<Iter>::value, void>::type>
			list(Iter first, Iter last) : list()
		{
			assign(first, last);
		}

		list(const size_type n) :list()
		{
			resize(n);
		}

		list(const size_type n, const value_type&val) :list()
		{
			insert(base_type::begin(), n, val);
		}

		list(const std::initializer_list<value_type>&lst) :list()
		{
			assign(lst.begin(), lst.end());
		}

		list(const self&x) :list()
		{
			insert(begin(), x.begin(), x.end());
		}

		list(self&&x) :list()
		{
			base_type::swap(x);
			std::swap(node_count, x.node_count);
		}

		self& operator=(self x)
		{
			swap(x);
			return (*this);
		}

		self& operator=(const std::initializer_list<value_type>&lst)
		{
			assign(lst.begin(), lst.end());
			return (*this);
		}

		void swap(self&x)noexcept
		{
			base_type::swap(node, x.node);
			std::swap(node_count, x.node_count);
		}

		iterator erase(const_iterator pos)
		{
			const_iterator next = pos;
			++next;
			base_type::erase_imple(pos);
			decrease();
			return make_iter(next);
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			while (first != last)
				first = erase(first);
			return make_iter(last);
		}

		void resize(const size_type new_size)
		{
			base_type::resize(new_size);
			count_set(new_size);
		}

		void resize(const size_type new_size, const value_type&val)
		{
			base_type::resize(new_size, val);
			count_set(new_size);
		}

		void pop_front()
		{
			erase(begin());
		}

		void pop_back()
		{
			erase(--end());
		}

		void clear()
		{
			base_type::clear();
			count_set();
		}

		void unique()
		{
			unique(std::equal_to<value_type>());
		}

		template<typename Pred>
		void unique(const Pred& pred)
		{
			auto first = begin();
			auto last = end();
			if (first == last)return;
			auto next = begin();
			while (++next != last)
			{
				if (pred(*next, *first))
					erase(next);
				else
					first = next;
				next = first;
			}
		}

		void remove(const value_type&val)
		{
			auto first = begin();
			auto last = end();
			while (first != last)
			{
				if (*first == val)
					first = erase(first);
				else
					++first;
			}
		}

		template<typename Pred>
		void remove_if(const Pred&pred)
		{
			iterator ed = end();
			for (iterator bg = begin(); bg != ed;)
				if (pred(*bg))
					bg = erase(bg);
				else
					++bg;
		}

		iterator insert(const_iterator pos, const value_type&val)
		{
			insert_imple(pos, val);
			return make_iter(--pos);
		}

		iterator insert(const_iterator pos, value_type&&val)
		{
			return emplace(pos, std::forward<value_type>(val));
		}

		iterator insert(const_iterator pos, size_type n, const value_type&val)
		{
			const_iterator prev = pos;
			if (prev == cbegin())
			{
				base_type::insert_n(pos, n, val);
				increase(n);
				return begin();
			}
			else
			{
				--prev;
				base_type::insert_n(pos, n, val);
				increase(n);
				return make_iter(++prev);
			}
		}

		template<typename Iter>
		typename enable_if<is_iterator<Iter>::value, iterator>::type
			insert(const_iterator pos, Iter first, Iter last)
		{
			const_iterator prev = pos;
			if (prev == base_type::cbegin())
			{
				insert_range
				(pos, first, last, _QMJ iterator_category(first));
				return (base_type::begin());
			}
			else
			{
				--prev;
				insert_range
				(pos, first, last, _QMJ iterator_category(first));
				return make_iter(++prev);
			}
		}

		iterator insert(const_iterator pos, const std::initializer_list<value_type>&lst)
		{
			return insert(pos, lst.begin(), lst.end());
		}

		template<typename Iter>
		typename enable_if<is_iterator<Iter>::value, void>::type
			assign(Iter first, Iter last)
		{
			assign_imple
			(first, last, _QMJ iterator_category(first));
		}

		void assign(const std::initializer_list<value_type>&lst)
		{
			assign(lst.begin(), lst.end());
		}

		void assign(size_type n, const value_type&val)
		{
			base_type::assign(n, val);
			count_set(n);
		}

		void push_back(const value_type&val)
		{
			insert_imple(end(), val);
		}

		void push_back(value_type&&val)
		{
			insert_imple(end(), std::forward<value_type>(val));
		}

		bool empty()const
		{
			return (!(node_count));
		}

		size_type size()const
		{
			return (node_count);
		}

		void splice(const_iterator pos, self&x)
		{
			if (this != &x && !x.empty())
			{
				increase(x.size());
				transfer(pos, x.begin(), x.end());
				x.count_set();
			}
		}

		void splice(const_iterator pos, self&&x)
		{
			splice(pos, (self&)x)
		}

		void splice(const_iterator pos, self&&x, const_iterator first)
		{
			splice(pos, (self&)x, first);
		}

		void splice(const_iterator pos, self&lst, const_iterator tar)
		{
			const_iterator last = tar;
			if (pos != tar)
			{
				base_type::transfer(pos, tar, ++last);
				if (this != &lst)
				{
					increase();
					x.decrease();
				}
			}
		}

		void splice(const_iterator pos, self&&x, const_iterator first, const_iterator last)
		{
			splice(pos, (self&)x, first, last);
		}

		void splice(const_iterator pos, self& lst, const_iterator first, const_iterator last)
		{
			if (first != pos)
			{
				const_iterator tmp = pos;
				if (tmp != end())
					++tmp;//the last iterator after insert finished,for count size
				base_type::transfer(pos, first, last);
				if (this != &lst)
				{
					size_type num = _QMJ distance(first, tmp);
					increase(num);//undate size
					lst.decrease(num);
				}
			}
		}

		void push_front(const value_type& val)
		{
			insert_imple(begin(), val);
		}

		void push_front(value_type&&val)
		{
			insert_imple(begin(), std::forward<value_type>(val));
		}

		template<typename...types>
		void emplace_front(types&&...args)
		{
			insert_imple(begin(), std::forward<types>(args)...);
		}

		template<typename...types>
		void emplace_back(types&&...args)
		{
			insert_imple(end(), std::forward<types>(args)...);
		}

		template<typename...types>
		iterator emplace(const_iterator pos, types&&...args)
		{
			insert_imple(pos, std::forward<types>(args)...);
			return make_iter(--pos);
		}

		void merge(self&x)
		{
			merge(x, std::less<value_type>());
		}

		void merge(self&&x)
		{
			merge((self&)x, std::less<value_type>());
		}

		template<typename Compare>
		void merge(self&&lst, const Compare& cmp)
		{
			merge((self&)lst, cmp);
		}

		template<typename Compare>
		void merge(self&lst, const Compare& cmp)
		{
			if (this != &lst)
			{
				base_type::merge(lst, cmp);
				increase(lst.size());
				lst.count_set(0);
			}
		}

	private:
		void count_set(size_type ele = 0)
		{
			node_count = ele;
		}

		void increase(size_type inc = 1)
		{
			node_count += inc;
		}

		void decrease(size_type dec = 1)
		{
			node_count -= dec;
		}

		template<typename Iter>
		void assign_imple(Iter first, Iter last, std::input_iterator_tag)
		{
			iterator bg = begin();
			iterator ed = end();
			for (; bg != ed&&first != last; ++first, ++bg)
				reuseNode(bg, *first);
			for (; first != last; ++first)
				insert_imple(bg, *first);
			erase(bg, ed);
		}

		template<typename Iter>
		void assign_imple(Iter first, Iter last, std::random_access_iterator_tag)
		{
			size_type n = last - first;
			iterator bg = begin();
			iterator ed = end();
			for (; bg != ed&&n != 0; --n, ++first, ++bg)
				reuseNode(bg, *first);
			for (; n != 0; --n, ++first)
				insert_imple(bg, *first);
			erase(bg, ed);
		}

		template<typename Iter>
		void insert_range
		(const_iterator pos, Iter first, Iter last, std::input_iterator_tag)
		{
			for (; first != last; ++first)
				insert_imple(pos, *first);
		}

		template<typename Iter>
		void insert_range
		(const_iterator pos, Iter first, Iter last, std::random_access_iterator_tag)
		{
			const size_type num = last - first;
			for (; num != 0; --num, ++first)
				insert_imple(pos, *first);
		}

		template<typename...types>
		void insert_imple(const_iterator pos, types&&...args)
		{
			base_type::insert_imple(pos, std::forward<types>(args)...);
			increase();
		}
	private:
		size_type node_count;
	};

	template<typename value_type, typename Alloc>inline
		void swap(list<value_type, Alloc>&left,
			list<value_type, Alloc>&right)noexcept
	{
		left.swap(right);
	}

	template<typename value_type, typename alloc>inline
		bool operator==(const _QMJ list<value_type, alloc>&left,
			const _QMJ list<value_type, alloc>&right)
	{
		return (std::equal(left.begin(), left.end(), right.begin(), right.end()));
	}

	template<typename value_type, typename alloc>inline
		bool operator!=(const _QMJ list<value_type, alloc>&left,
			const _QMJ list<value_type, alloc>&right)
	{
		return !(left == right);
	}

	template<typename value_type, typename alloc>inline
		bool operator<(const _QMJ list<value_type, alloc>&left,
			const _QMJ list<value_type, alloc>&right)
	{
		return std::lexicographical_compare
		(left.begin(), left.end(), right.begin(), right.end());
	}

	template<typename value_type, typename alloc>inline
		bool operator<=(const _QMJ list<value_type, alloc>&left,
			const _QMJ list<value_type, alloc>&right)
	{
		return !(right < left);
	}

	template<typename value_type, typename alloc>inline
		bool operator>(const _QMJ list<value_type, alloc>&left,
			const _QMJ list<value_type, alloc>&right)
	{
		return right < left;
	}

	template<typename value_type, typename alloc>inline
		bool operator>=(const _QMJ list<value_type, alloc>&left,
			const _QMJ list<value_type, alloc>&right)
	{
		return !(left < right);
	}
}

#endif