#pragma once
#ifndef _DEQUE_QMJ_
#define _DEQUE_QMJ_
#include"allocator.h"
#include"iterator_qmj.h"
#include"vector_qmj.h"

namespace qmj
{
	template<typename value_type, typename alloc, typename>
	class deque;

	template<typename value_type>
	class deque_const_iterator
	{
	public:
		template<typename value_type, typename alloc, typename>
		friend class deque;

		typedef std::random_access_iterator_tag iterator_category;
		typedef value_type value_type;
		typedef const value_type* pointer;
		typedef const value_type& reference;
		typedef ptrdiff_t difference_type;

		typedef const pointer* map_pointer;
		typedef deque_const_iterator<value_type> self;

		deque_const_iterator() :node(nullptr)
		{
		}

		deque_const_iterator(value_type** node) :
			node(node)
		{
		}

		deque_const_iterator(const self& x) :
			node(x.node)
		{
		}

		self& operator=(const self&x)
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

		bool operator<(const self&x)const
		{
			return node < x.node;
		}

		bool operator<=(const self&x)const
		{
			return node <= x.node;
		}

		bool operator>(const self&x)const
		{
			return node > x.node;
		}

		bool operator>=(const self&x)const
		{
			return node >= x.node;
		}

		reference operator*()const
		{
			return (**node);
		}

		pointer operator->()const
		{
			return &(operator*());
		}

		reference operator[](difference_type off)const
		{
			return (**(node + off));
		}

		self& operator++()
		{
			++node;
			return *this;
		}

		self operator++(int)
		{
			auto ret = *this;
			operator++();
			return ret;
		}

		self& operator--()
		{
			--node;
			return *this;
		}

		self operator--(int)
		{
			auto ret = *this;
			operator--();
			return ret;
		}

		self operator+(const difference_type n)const
		{
			return self(node + n);
		}

		self& operator+=(const difference_type n)
		{
			node += n;
			return *this;
		}

		self operator-(const difference_type n)const
		{
			return self(node - n);
		}

		difference_type operator-(const self&x)const
		{
			return node - x.node;
		}

		self& operator-=(const difference_type n)
		{
			node -= n;
			return *this;
		}

	private:
		value_type** get_node()
		{
			return node;
		}

	protected:
		value_type** node;
	};

	template<typename value_type>
	class deque_iterator :
		public deque_const_iterator<value_type>
	{
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef value_type value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef ptrdiff_t difference_type;

		typedef pointer* map_pointer;
		typedef deque_iterator<value_type> self;
		typedef deque_const_iterator<value_type> iterator_base;

		deque_iterator() :
			iterator_base()
		{
		}

		deque_iterator(value_type** node) :
			iterator_base(node)
		{
		}

		deque_iterator(const self& x) :
			iterator_base(x.node)
		{
		}

		self& operator=(const self&x)
		{
			node = x.node;
			return (*this);
		}

		void iter_swap(self&right)noexcept
		{
			std::iter_swap(*node, *(right.node));
		}

		bool operator==(const self&x)const
		{
			return node == x.node;
		}

		bool operator!=(const self&x)const
		{
			return !operator==(x);
		}

		bool operator<(const self&x)const
		{
			return node < x.node;
		}

		bool operator<=(const self&x)const
		{
			return node <= x.node;
		}

		bool operator>(const self&x)const
		{
			return node > x.node;
		}

		bool operator>=(const self&x)const
		{
			return node >= x.node;
		}

		reference operator*()const
		{
			return (*(*node));
		}

		pointer operator->()const
		{
			return &(operator*());
		}

		reference operator[](difference_type off)const
		{
			return (**(node + off));
		}

		self& operator++()
		{
			++node;
			return *this;
		}

		self operator++(int)
		{
			auto ret = *this;
			operator++();
			return ret;
		}

		self& operator--()
		{
			--node;
			return *this;
		}

		self operator--(int)
		{
			auto ret = *this;
			operator--();
			return ret;
		}

		self operator+(const difference_type n)const
		{
			return self(node + n);
		}

		self& operator+=(const difference_type n)
		{
			node += n;
			return *this;
		}

		self operator-(const difference_type n)const
		{
			return self(node - n);
		}

		difference_type operator-(const self&x)const
		{
			return node - x.node;
		}

		self& operator-=(const difference_type n)
		{
			node -= n;
			return *this;
		}
	};

	template<typename value_type,
		typename Alloc = _QMJ allocator<value_type>,
		typename = void>
		class deque
	{
	public:
		typedef value_type value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t differene_type;
		typedef pointer* map_type;

		typedef deque_iterator<value_type> iterator;
		typedef deque_const_iterator<value_type> const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		typedef deque<value_type, Alloc, void> self;
		typedef Alloc allocator_type;
		typedef Alloc alloc;
		typedef typename allocator_type::template rebind<pointer>::other map_alloc;

		deque() :map(nullptr), first(nullptr),
			last(nullptr), end_storage(nullptr)
		{
		}

		deque(const size_type n) :deque()
		{
			resize(n);
		}

		deque(const size_type n, const value_type&val) :
			deque()
		{
			assign(n, val);
		}

		deque(const std::initializer_list<value_type>&lst) :
			deque(lst.begin(), lst.end())
		{
		}

		template<typename Iter,
			typename = typename enable_if<is_iterator<Iter>::value, void>::type>
			deque(Iter bg, Iter ed) : deque()
		{
			insert(end(), bg, ed);
		}

		deque(const self&x) :deque()
		{
			insert(end(), x.begin(), x.end());
		}

		deque(self &&x) :
			map(x.map), first(x.first),
			last(x.last), end_storage(x.end_storage)noexcept
		{
			x.map = x.first = x.last = x.end_storage = nullptr;
		}

		self& operator = (self x)
		{
			swap(x);
			return (*this);
		}

		self& operator=(const std::initializer_list<value_type>&lst)
		{
			assign(lst.begin(), lst.end());
			return (*this);
		}

		~deque()
		{
			clear();
			free_oldMap();
		}

		void swap(self&x)noexcept
		{
			std::swap(map, x.map);
			std::swap(first, x.first);
			std::swap(last, x.last);
			std::swap(end_storage, x.end_storage);
		}

		void iter_swap(const_iterator left, const_iterator right)noexcept
		{
			std::swap(*left.get_node(), *right.get_node());
		}

		void shrink_to_fit()
		{
			if (first != map || end_storage != last)
			{
				size_type old_size = size();
				map_type new_map = map_allocate(old_size);
				last = my_memcpy(new_map, first, old_size);
				first = new_map;
				deallocate_and_update_map(new_map, old_size);
			}
		}

		void resize(const size_type n)
		{
			const size_type old_size = size();
			if (n <= old_size)
			{
				map_type new_last = first + n;
				for (map_type cur = new_last; cur != last; ++cur)
					destory_ele(cur);
				last = new_last;
			}
			else
			{
				size_type after_storage = end_storage - last;
				if (after_storage < n - old_size)
				{
					size_type len = n + old_size;
					map_type new_map = map_allocate(len);
					last = my_memcpy(new_map, first, old_size);
					first = new_map;
					deallocate_and_update_map(new_map, len);
				}
				last = copy_n_default(last, n - old_size);
			}
		}

		void resize(const size_type n, const value_type&val)
		{
			const size_type old_size = size();
			if (n <= old_size)
			{
				map_type new_last = first + n;
				for (map_type cur = new_last; cur != last; ++cur)
					destory_ele(cur);
				last = new_last;
			}
			else
			{
				size_type after_storage = end_storage - last;
				if (after_storage < n - old_size)
				{
					size_type len = n + size();
					map_type new_map = map_allocate(len);
					last = my_memcpy(new_map, first, old_size);
					first = new_map;
					deallocate_and_update_map(new_map, len);
				}
				last = copy_n(last, n - old_size, val);
			}
		}

		bool empty()const
		{
			return first == last;
		}

		allocator_type get_allocator()const
		{
			return allocator_type();
		}

		iterator erase(iterator bg, iterator ed)
		{
			map_type cur = bg.get_node();
			map_type finish = ed.get_node();
			size_type before_ele = cur - first;
			size_type after_ele = last - finish;
			for (map_type cp = cur; cp != finish; ++cp)
				destory_ele(cp);
			if (after_ele < before_ele)
				last = my_memcpy(cur, finish, after_ele);
			else
				first = my_memmove(finish - before_ele, first, before_ele);
			return (begin() + before_ele);
		}

		iterator erase(iterator pos)
		{
			return erase(pos, pos + 1);
		}

		void pop_back()
		{
			destory_ele(--last);
		}

		void pop_front()
		{
			destory_ele(first++);
		}

		void clear()
		{
			for (; first != last; ++first)
				destory_ele(first);
			last = first = map + (end_storage - map) / 2;
		}

		reference at(const size_type n)
		{
			return (*first[n]);
		}

		const_reference at(const size_type n)const
		{
			return (*first[n]);
		}

		void assign(size_type n, const value_type&val)
		{
			clear();
			size_type len = end_storage - map;
			if (len >= n)
			{
				first = map + ((len - n) >> 1);
				last = copy_n(first, n, val);
			}
			else
			{
				len = len + std::max(len, n);
				map_type new_map = map_allocate(len);
				first = new_map + ((len - n) >> 1);
				last = copy_n(first, n, val);
				deallocate_and_update_map(new_map, len);
			}
		}

		void assign(const std::initializer_list<value_type>&lst)
		{
			assign(lst.begin(), lst.end());
		}

		template<typename Iter>
		typename enable_if<is_iterator<Iter>::value, void>::type
			assign(Iter bg, Iter ed)
		{
			clear();
			insert(end(), bg, ed);
		}

		iterator begin()
		{
			return iterator(first);
		}

		iterator end()
		{
			return iterator(last);
		}

		const_iterator begin()const
		{
			return const_iterator(first);
		}

		const_iterator end()const
		{
			return const_iterator(last);
		}

		const_iterator cbegin()const
		{
			return const_iterator(first);
		}

		const_iterator cend()const
		{
			return const_iterator(last);
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rbegin()const
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator rend()const
		{
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crbegin()const
		{
			return const_reverse_iterator(cend());
		}

		const_reverse_iterator crend()const
		{
			return const_reverse_iterator(cbegin());
		}

		reference operator[](const size_type n)
		{
			return (*first[n]);
		}

		const_reference operator[](const size_type n)const
		{
			return (*first[n]);
		}

		reference front()
		{
			return **first;
		}

		const_reference front()const
		{
			return (**first);
		}

		reference back()
		{
			return  (**(last - 1));
		}

		const_reference back()const
		{
			return (**(last - 1));
		}

		size_type size()const
		{
			return (last - first);
		}

		size_type max_size()const
		{
			return size_type(-1);
		}

		iterator insert(iterator pos, const size_type n, const value_type&val)
		{
			if (!n)return pos;
			map_type cur = pos.get_node();
			size_type before_ele = cur - first;
			size_type after_ele = last - cur;
			size_type before_storage = first - map;
			size_type after_storage = end_storage - last;
			if (after_ele < before_ele)
			{
				if (after_storage >= n)
					return insert_after(cur, before_ele, after_ele, n, val);
				else if (before_storage >= n)
					return insert_before(cur, before_ele, n, val);
			}
			else
			{
				if (before_storage >= n)
					return insert_before(cur, before_ele, n, val);
				else if (after_storage >= n)
					return insert_after(cur, before_ele, after_ele, n, val);
			}
			return deallocate_and_update_ptr(size() + n, before_ele, after_ele, cur, n, val);
		}

		iterator insert(iterator pos, const value_type&val)
		{
			return insert(pos, 1, val);
		}

		iterator insert(iterator pos, value_type&&val)
		{
			return emplace(pos, std::move(val));
		}

		template<typename Iter>
		typename enable_if<is_iterator<Iter>::value, iterator>::type
			insert(iterator pos, Iter bg, Iter ed)
		{
			return insert_range(pos, bg, ed, _QMJ iterator_category(bg));
		}

		iterator insert
		(iterator pos, const std::initializer_list<value_type>&lst)
		{
			return insert(pos, lst.begin(), lst.end());
		}

		template<typename...types>
		iterator emplace(iterator pos, types&&...args)
		{
			pointer ptr = create_ele(std::forward<types>(args)...);
			map_type cur = pos.get_node();
			size_type before_ele = cur - first;
			size_type after_ele = last - cur;
			size_type before_storage = first - map;
			size_type after_storage = end_storage - last;
			if (after_ele < before_ele)
			{
				if (after_storage != last)
					return insert_after(cur, ptr, pos);
				else if (before_storage >= n)
					return insert_before(cur, ptr, pos);
			}
			else
			{
				if (first != map)
					return insert_before(cur, ptr, pos);
				else if (after_storage >= n)
					return insert_after(cur, ptr, pos);
			}
			return deallocate_and_update_ptr(size() + 1, before_ele, after_ele, cur, ptr);
		}

		void push_front(const value_type&val)
		{
			pointer ptr = create_ele(val);
			insert_front(ptr);
		}

		void push_front(value_type&&val)
		{
			emplace_front(std::forward<value_type>(val));
		}

		void push_back(const value_type&val)
		{
			pointer ptr = create_ele(val);
			insert_back(ptr);
		}

		void push_back(value_type&&val)
		{
			emplace_back(std::forward<value_type>(val));
		}

		template<typename...types>
		void emplace_front(types&&...args)
		{
			pointer ptr = create_ele(std::forward<types>(args)...);
			insert_front(ptr);
		}

		template<typename...types>
		void emplace_back(types&&...args)
		{
			pointer ptr = create_ele(std::forward<value_type>(args)...);
			insert_back(ptr);
		}

	private:
		void insert_front(pointer ptr)
		{
			if (first != map)
				*--first = ptr;
			else
			{
				const size_type old_size = size();
				size_type len = old_size != 0 ? old_size << 1 : 1;
				map_type new_map = map_allocate(len);
				map_type new_first = new_map + (old_size >> 1) + 1;
				last = my_memcpy(new_first, first, old_size);
				first = new_first;
				*--first = ptr;
				deallocate_and_update_map(new_map, len);
			}
		}

		void insert_back(pointer ptr)
		{
			if (last != end_storage)
				*last++ = ptr;
			else
			{
				const size_type old_size = size();
				size_type len = old_size != 0 ? old_size << 1 : 1;
				map_type new_map = map_allocate(len);
				map_type new_first = new_map + (old_size >> 1);
				last = my_memcpy(new_first, first, old_size);
				*last++ = ptr;
				first = new_first;
				deallocate_and_update_map(new_map, len);
			}
		}

		template<typename Iter>
		iterator insert_range
		(iterator pos, Iter bg, Iter ed, std::input_iterator_tag)
		{
			size_type off = pos - begin();
			size_type old_size = size();
			for (; bg != ed; ++bg)
				push_back(*bg);
			std::rotate(begin() + off, begin() + old_size, end());
			return (begin() + off);
		}

		template<typename Iter>
		iterator insert_range
		(iterator pos, Iter bg, Iter ed, std::forward_iterator_tag)
		{
			size_type n = _QMJ distance(bg, ed);
			if (!n)return pos;
			map_type cur = pos.get_node();
			size_type before_ele = cur - first;
			size_type after_ele = last - cur;
			size_type before_storage = first - map;
			size_type after_storage = end_storage - last;
			if (after_ele < before_ele)
			{
				if (after_storage >= n)
					return insert_after(cur, before_ele, after_ele, n, bg);
				else if (before_storage >= n)
					return insert_before(cur, before_ele, n, bg);
			}
			else
			{
				if (before_storage >= n)
					return insert_before(cur, before_ele, n, bg);
				else if (after_storage >= n)
					return insert_after(cur, before_ele, after_ele, n, bg);
			}
			return deallocate_and_update_ptr(size() + n, before_ele, after_ele, cur, n, bg);
		}

		template<typename Iter>
		iterator insert_before
		(map_type cur, size_type before_ele, size_type n, Iter bg)
		{
			my_memmove(first - n, first, before_ele);
			copy_range(cur - n, n, bg);
			first -= n;
			return (begin() + before_ele);
		}

		template<typename Iter>
		iterator insert_after
		(map_type cur, size_type before_ele, size_type after_ele, size_type n, Iter bg)
		{
			my_memmove(cur + n, cur, after_ele);
			copy_range(cur, n, bg);
			last += n;
			return (begin() + before_ele);
		}

		iterator insert_before
		(map_type cur, size_type before_ele, size_type n, const value_type&val)
		{
			my_memmove(first - n, first, before_ele);
			copy_n(cur - n, n, val);
			first -= n;
			return (begin() + before_ele);
		}

		iterator insert_after
		(map_type cur, size_type before_ele, size_type after_ele, size_type n, const value_type&val)
		{
			my_memmove(cur + n, cur, after_ele);
			copy_n(cur, n, val);
			last += n;
			return (begin() + before_ele);
		}

		iterator insert_before
		(map_type cur, map_type ptr, iterator pos)
		{
			my_memcpy(first - 1, first, cur - first);
			*cur = ptr;
			return (pos);
		}

		iterator insert_after
		(map_type cur, map_type ptr, iterator pos)
		{
			my_memove(cur + 1, cur, last - cur);
			*cur = ptr;
			return (pos);
		}

		template<typename Iter>
		iterator deallocate_and_update_ptr
		(size_type new_size, size_type before_ele,
			size_type after_ele, map_type cur,
			size_type n, Iter bg)
		{
			size_type len = new_size << 1;
			map_type new_map = map_allocate(len);
			map_type new_first = new_map + (new_size >> 1);
			last = my_memcpy(new_first, first, before_ele);
			last = copy_range(last, n, bg);
			last = my_memcpy(last, cur, after_ele);
			first = new_first;
			deallocate_and_update_map(new_map, len);
			return (begin() + before_ele);
		}

		iterator deallocate_and_update_ptr
		(size_type new_size, size_type bofore_ele, map_type cur,
			size_type after_ele, map_type ptr)
		{
			size_type len = new_size << 1;
			map_type new_map = map_allocate(len);
			map_type new_first = new_map + (new_size >> 1);
			last = my_memcpy(new_first, first, before_ele);
			*last++ = ptr;
			last = my_memcpy(last, cur, after_ele);
			first = new_first;
			deallocate_and_update_map(new_map, len);
			return (begin() + before_ele);
		}

		iterator deallocate_and_update_ptr
		(size_type new_size, size_type before_ele,
			size_type after_ele, map_type cur,
			size_type n, const value_type&val)
		{
			size_type len = new_size << 1;
			map_type new_map = map_allocate(len);
			map_type new_first = new_map + (new_size >> 1);
			last = my_memcpy(new_first, first, before_ele);
			last = copy_n(last, n, val);
			last = my_memcpy(last, cur, after_ele);
			first = new_first;
			deallocate_and_update_map(new_map, len);
			return (begin() + before_ele);
		}

	private:
		map_type copy_n(map_type cur, size_type n, const value_type&val)
		{
			for (; n != 0; --n, ++cur)
				*cur = create_ele(val);
			return cur;
		}

		map_type copy_n_default(map_type cur, size_type n)
		{//use default contructor,for resize
			for (; n != 0; --n, ++cur)
			{
				pointer ptr = alloc::allocate();
				alloc::construct(ptr);
				*cur = ptr;
			}
			return cur;
		}

		template<typename InputIterator>
		map_type copy_range
		(map_type cur, size_type n, InputIterator bg)
		{
			for (; n != 0; --n, ++cur, ++bg)
				*cur = create_ele(*bg);
			return cur;
		}

		template<typename...types>
		pointer create_ele(types&&...args)
		{
			pointer ptr = alloc::allocate();
			alloc::construct(ptr, std::forward<types>(args)...);
			return (ptr);
		}

		void destory_ele(map_type cur)
		{
			alloc::destroy(*cur);
			alloc::deallocate(*cur);
		}

		map_type map_allocate(const size_type n)
		{
			return (map_alloc::allocate(n));
		}

		void free_oldMap()
		{
			map_alloc::deallocate(map, end_storage - map);
		}

		map_type my_memmove
		(map_type dst, map_type src, const size_type n)
		{
			std::memmove(dst, src, sizeof(pointer)*n);
			return dst;
		}

		map_type my_memcpy
		(map_type  dst, map_type src, const size_type n)
		{
			std::memcpy(dst, src, sizeof(pointer)*n);
			return (dst + n);
		}

		void deallocate_and_update_map(map_type new_map, size_type len)
		{
			free_oldMap();
			map = new_map;
			end_storage = map + len;
		}

	private:
		map_type map;
		map_type first;
		map_type last;
		map_type end_storage;
	};

	template<typename value_type, typename alloc, typename NoType>inline
		void swap(_QMJ deque<value_type, alloc, NoType>&left,
			_QMJ deque<value_type, alloc, NoType>&right)noexcept
	{
		left.swap(right);
	}

	template<typename value_type, typename alloc, typename NoType>inline
		bool operator==(const _QMJ deque<value_type, alloc, NoType>&left,
			const _QMJ deque<value_type, alloc, NoType>&right)
	{
		return (left.size() == right.size()
			&& std::equal(left.begin(), left.end(), right.begin()));
	}

	template<typename value_type, typename alloc, typename NoType>inline
		bool operator!=(const _QMJ deque<value_type, alloc, NoType>&left,
			const _QMJ deque<value_type, alloc, NoType>&right)
	{
		return !(left == right);
	}

	template<typename value_type, typename alloc, typename NoType>inline
		bool operator<(const _QMJ deque<value_type, alloc, NoType>&left,
			const _QMJ deque<value_type, alloc, NoType>&right)
	{
		return std::lexicographical_compare
		(left.begin(), left.end(), right.begin(), right.end());
	}

	template<typename value_type, typename alloc, typename NoType>inline
		bool operator<=(const _QMJ deque<value_type, alloc, NoType>&left,
			const _QMJ deque<value_type, alloc, NoType>&right)
	{
		return !(right < left);
	}

	template<typename value_type, typename alloc, typename NoType>inline
		bool operator>(const _QMJ deque<value_type, alloc, NoType>&left,
			const _QMJ deque<value_type, alloc, NoType>&right)
	{
		return right < left;
	}

	template<typename value_type, typename alloc, typename NoType>inline
		bool operator>=(const _QMJ deque<value_type, alloc, NoType>&left,
			const _QMJ deque<value_type, alloc, NoType>&right)
	{
		return !(right < left);
	}
}

namespace qmj
{
	template<typename value_type, typename Alloc>
	class deque < value_type, Alloc,
		typename _QMJ enable_if < sizeof(value_type) < sizeof(value_type*)
		|| (_QMJ is_pod<value_type>::value && sizeof(value_type) <= 10 * sizeof(value_type*))
		, void > ::type >
	{
	public:
		typedef value_type value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t differene_type;
		typedef pointer map_type;

		typedef vector_iterator<value_type> iterator;
		typedef vector_const_iterator<value_type> const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		typedef deque<value_type, Alloc, void> self;
		typedef Alloc allocator_type;
		typedef Alloc alloc;

		deque() :map(nullptr), first(nullptr),
			last(nullptr), end_storage(nullptr)
		{
		}

		deque(const size_type n) :deque()
		{
			resize(n);
		}

		deque(const size_type n, const value_type&val) :
			deque()
		{
			assign(n, val);
		}

		deque(const std::initializer_list<value_type>&lst) :
			deque(lst.begin(), lst.end())
		{
		}

		template<typename Iter,
			typename = typename enable_if<is_iterator<Iter>::value, void>::type>
			deque(Iter bg, Iter ed) : deque()
		{
			insert(end(), bg, ed);
		}

		deque(const self&x) :deque()
		{
			insert(end(), x.begin(), x.end());
		}

		deque(self &&x) :
			map(x.map), first(x.first),
			last(x.last), end_storage(x.end_storage)noexcept
		{
			x.map = x.first = x.last = x.end_storage = nullptr;
		}

		self& operator = (self x)
		{
			swap(x);
			return (*this);
		}

		self& operator=(const std::initializer_list<value_type>&lst)
		{
			assign(lst.begin(), lst.end());
			return (*this);
		}

		~deque()
		{
			free_oldMap();
		}

		void swap(self&x)noexcept
		{
			std::swap(map, x.map);
			std::swap(first, x.first);
			std::swap(last, x.last);
			std::swap(end_storage, x.end_storage);
		}

		void shrink_to_fit()
		{
			if (first != map || end_storage != last)
			{
				size_type old_size = size();
				map_type new_map = map_allocate(old_size);
				last = my_memcpy(new_map, first, old_size);
				first = new_map;
				deallocate_and_update_map(new_map, old_size);
			}
		}

		void resize(const size_type n)
		{
			const size_type old_size = size();
			if (n < old_size)
				last = first + n;
			else if (n > old_size)
			{
				size_type after_storage = end_storage - last;
				if (after_storage < n - old_size)
				{
					size_type len = n + old_size;
					map_type new_map = map_allocate(len);
					last = my_memcpy(new_map, first, old_size);
					first = new_map;
					deallocate_and_update_map(new_map, len);
				}
				last = alloc::construct(last, n - old_size);
			}
		}

		void resize(const size_type n, const value_type&val)
		{
			const size_type old_size = size();
			if (n < old_size)
				last = first + n;
			else if (n > old_size)
			{
				size_type after_storage = end_storage - last;
				if (after_storage < n - old_size)
				{
					size_type len = n + old_size;
					map_type new_map = map_allocate(len);
					last = my_memcpy(new_map, first, old_size);
					first = new_map;
					deallocate_and_update_map(new_map, len);
				}
				last = alloc::copy_construct(last, n - old_size, val);
			}
		}

		bool empty()const
		{
			return (first == last);
		}

		allocator_type get_allocator()const
		{
			return allocator_type();
		}

		iterator erase(iterator bg, iterator ed)
		{
			map_type cur = get_node(bg);
			map_type finish = get_node(ed);
			size_type before_ele = cur - first;
			size_type after_ele = last - finish;
			if (after_ele < before_ele)
				last = my_memcpy(cur, finish, after_ele);
			else
				first = my_memmove(finish - before_ele, first, before_ele);
			return (begin() + before_ele);
		}

		iterator erase(iterator pos)
		{
			return erase(pos, pos + 1);
		}

		void pop_back()
		{
			(--last);
		}

		void pop_front()
		{
			(++first);
		}

		void clear()
		{
			first = map + (end_storage - map) / 2;
			last = first;
		}

		reference at(const size_type n)
		{
			return (first[n]);
		}

		const_reference at(const size_type n)const
		{
			return (first[n]);
		}

		void assign(size_type n, const value_type&val)
		{
			clear_for_assign();
			size_type len = end_storage - map;
			if (len >= n)
			{
				first = map + ((len - n) >> 1);
				last = alloc::copy_construct(first, n, val);
			}
			else
			{
				len = len + std::max(len, n);
				map_type new_map = map_allocate(len);
				first = new_map + ((len - n) >> 1);
				last = alloc::copy_construct(first, n, val);
				deallocate_and_update_map(new_map, len);
			}
		}

		void assign(const std::initializer_list<value_type>&lst)
		{
			assign(lst.begin(), lst.end());
		}

		template<typename Iter>
		typename enable_if<is_iterator<Iter>::value, void>::type
			assign(Iter bg, Iter ed)
		{
			clear_for_assign();
			insert(end(), bg, ed);
		}

		iterator begin()
		{
			return iterator(first);
		}

		iterator end()
		{
			return iterator(last);
		}

		const_iterator begin()const
		{
			return const_iterator(first);
		}

		const_iterator end()const
		{
			return const_iterator(last);
		}

		const_iterator cbegin()const
		{
			return const_iterator(first);
		}

		const_iterator cend()const
		{
			return const_iterator(last);
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rbegin()const
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator rend()const
		{
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crbegin()const
		{
			return const_reverse_iterator(cend());
		}

		const_reverse_iterator crend()const
		{
			return const_reverse_iterator(cbegin());
		}

		reference operator[](const size_type n)
		{
			return (first[n]);
		}

		const_reference operator[](const size_type n)const
		{
			return (first[n]);
		}

		reference front()
		{
			return (*first);
		}

		const_reference front()const
		{
			return (*first);
		}

		reference back()
		{
			return  (*(last - 1));
		}

		const_reference back()const
		{
			return (*(last - 1));
		}

		size_type size()const
		{
			return (last - first);
		}

		size_type max_size()const
		{
			return alloc::max_size();
		}

		iterator insert(iterator pos, const size_type n, const value_type&val)
		{
			if (!n)return pos;
			map_type cur = get_node(pos);
			size_type before_ele = cur - first;
			size_type after_ele = last - cur;
			size_type before_storage = first - map;
			size_type after_storage = end_storage - last;
			if (after_ele < before_ele)
			{
				if (after_storage >= n)
					return insert_after(cur, before_ele, after_ele, n, val);
				else if (before_storage >= n)
					return insert_before(cur, before_ele, n, val);
			}
			else
			{
				if (before_storage >= n)
					return insert_before(cur, before_ele, n, val);
				else if (after_storage >= n)
					return insert_after(cur, before_ele, after_ele, n, val);
			}
			return deallocate_and_update_ptr(size() + n, before_ele, after_ele, cur, n, val);
		}

		iterator insert(iterator pos, const value_type&val)
		{
			return insert(pos, 1, val);
		}

		iterator insert(iterator pos, value_type&&val)
		{
			return emplace(pos, std::move(val));
		}

		template<typename Iter>
		typename enable_if<is_iterator<Iter>::value, iterator>::type
			insert(iterator pos, Iter bg, Iter ed)
		{
			return insert_range(pos, bg, ed, _QMJ iterator_category(bg));
		}

		iterator insert
		(iterator pos, const std::initializer_list<value_type>&lst)
		{
			return insert(pos, lst.begin(), lst.end());
		}

		template<typename...types>
		iterator emplace(iterator pos, types&&...args)
		{
			map_type cur = get_node(pos);
			size_type before_ele = cur - first;
			size_type after_ele = last - cur;
			size_type before_storage = first - map;
			size_type after_storage = end_storage - last;
			if (after_ele < before_ele)
			{
				if (after_storage != last)
					return insert_after(cur, pos, std::frward<types>(args)...);
				else if (before_storage >= n)
					return insert_before(cur, pos, std::forward<types>(args)...);
			}
			else
			{
				if (first != map)
					return insert_before(cur, pos, std::forward<types>(args)...);
				else if (after_storage >= n)
					return insert_after(cur, pos, std::frward<types>(args)...);
			}
			return deallocate_and_update_ptr(size() + 1, before_ele, after_ele,
				cur, std::forward<types>(args)...);
		}

		void push_front(const value_type&val)
		{
			insert_front(val);
		}

		void push_front(value_type&&val)
		{
			emplace_front(std::forward<value_type>(val));
		}

		void push_back(const value_type&val)
		{
			insert_back(val);
		}

		void push_back(value_type&&val)
		{
			emplace_back(std::forward<value_type>(val));
		}

		template<typename...types>
		void emplace_front(types&&...args)
		{
			insert_front(std::forward<types>(args)...);
		}

		template<typename...types>
		void emplace_back(types&&...args)
		{
			insert_back(std::forward<value_type>(args)...);
		}

	private:
		template<typename...types>
		void insert_front(types&&...args)
		{
			if (first != map)
				alloc::construct(--first, std::forward<types>(args)...);
			else
			{
				const size_type old_size = size();
				size_type len = old_size != 0 ? old_size << 1 : 1;
				map_type new_map = map_allocate(len);
				map_type new_first = new_map + (old_size >> 1) + 1;
				last = my_memcpy(new_first, first, old_size);
				first = new_first;
				alloc::construct(--first, std::forward<types>(args)...);
				deallocate_and_update_map(new_map, len);
			}
		}

		template<typename...types>
		void insert_back(types&&...args)
		{
			if (last != end_storage)
				alloc::construct(last++, std::forward<types>(args)...);
			else
			{
				const size_type old_size = size();
				size_type len = old_size != 0 ? old_size << 1 : 1;
				map_type new_map = map_allocate(len);
				map_type new_first = new_map + (old_size >> 1);
				last = my_memcpy(new_first, first, old_size);
				alloc::construct(last++, std::forward<types>(args)...);
				first = new_first;
				deallocate_and_update_map(new_map, len);
			}
		}

		template<typename Iter>
		iterator insert_range
		(iterator pos, Iter bg, Iter ed, std::input_iterator_tag)
		{
			size_type off = pos - begin();
			size_type old_size = size();
			for (; bg != ed; ++bg)
				push_back(*bg);
			std::rotate(begin() + off, begin() + old_size, end());
			return (begin() + off);
		}

		template<typename Iter>
		iterator insert_range
		(iterator pos, Iter bg, Iter ed, std::forward_iterator_tag)
		{
			size_type n = _QMJ distance(bg, ed);
			if (!n)return pos;
			map_type cur = get_node(pos);
			size_type before_ele = cur - first;
			size_type after_ele = last - cur;
			size_type before_storage = first - map;
			size_type after_storage = end_storage - last;
			if (after_ele < before_ele)
			{
				if (after_storage >= n)
					return insert_after(cur, before_ele, after_ele, n, bg);
				else if (before_storage >= n)
					return insert_before(cur, before_ele, n, bg);
			}
			else
			{
				if (before_storage >= n)
					return insert_before(cur, before_ele, n, bg);
				else if (after_storage >= n)
					return insert_after(cur, before_ele, after_ele, n, bg);
			}
			return deallocate_and_update_ptr(size() + n, before_ele, after_ele, cur, n, bg);
		}

		template<typename Iter>
		iterator insert_before
		(map_type cur, size_type before_ele, size_type n, Iter bg)
		{
			my_memmove(first - n, first, before_ele);
			alloc::copy_construct(bg, n, cur - n);
			first -= n;
			return (begin() + before_ele);
		}

		template<typename Iter>
		iterator insert_after
		(map_type cur, size_type before_ele, size_type after_ele, size_type n, Iter bg)
		{
			my_memmove(cur + n, cur, after_ele);
			alloc::copy_construct(bg, n, cur);
			last += n;
			return (begin() + before_ele);
		}

		iterator insert_before
		(map_type cur, size_type before_ele, size_type n, const value_type&val)
		{
			my_memmove(first - n, first, before_ele);
			alloc::copy_construct(cur - n, n, val);
			first -= n;
			return (begin() + before_ele);
		}

		iterator insert_after
		(map_type cur, size_type before_ele, size_type after_ele, size_type n, const value_type&val)
		{
			my_memmove(cur + n, cur, after_ele);
			alloc::copy_construct(cur, n, val);
			last += n;
			return (begin() + before_ele);
		}

		template<typename...types>
		iterator insert_before
		(map_type cur, iterator pos, types&&...args)
		{
			my_memcpy(first - 1, first, cur - first);
			alloc::construct(cur, std::forward<types>(args)...);
			return (pos);
		}

		template<typename...types>
		iterator insert_after
		(map_type cur, iterator pos, types&&...args)
		{
			my_memove(cur + 1, cur, last - cur);
			alloc::construct(cur, std::forward<types>(args)...);
			return (pos);
		}

		template<typename Iter>
		iterator deallocate_and_update_ptr
		(size_type new_size, size_type before_ele,
			size_type after_ele, pointer cur,
			size_type n, Iter bg)
		{
			size_type len = new_size << 1;
			map_type new_map = map_allocate(len);
			map_type new_first = new_map + (new_size >> 1);

			last = my_memcpy(new_first, first, before_ele);
			last = alloc::copy_construct(bg, n, last);
			last = my_memcpy(last, cur, after_ele);
			first = new_first;
			deallocate_and_update_map(new_map, len);
			return (begin() + before_ele);
		}

		template<typename...types>
		iterator deallocate_and_update_ptr
		(size_type new_size, size_type bofore_ele,
			size_type after_ele, pointer cur, types&&...args)
		{
			size_type len = new_size << 1;
			map_type new_map = map_allocate(len);
			map_type new_first = new_map + (new_size >> 1);

			last = my_memcpy(new_first, first, before_ele);
			alloc::construct(last++, std::forward<types>(args)...);
			last = my_memcpy(last, cur, after_ele);
			first = new_first;
			deallocate_and_update_map(new_map, len);
			return (begin() + before_ele);
		}

		iterator deallocate_and_update_ptr
		(size_type new_size, size_type before_ele,
			size_type after_ele, pointer cur,
			size_type n, const value_type&val)
		{
			size_type len = new_size << 1;
			map_type new_map = map_allocate(len);
			map_type new_first = new_map + (new_size >> 1);

			last = my_memcpy(new_first, first, before_ele);
			last = alloc::copy_construct(last, n, val);
			last = my_memcpy(last, cur, after_ele);
			first = new_first;
			deallocate_and_update_map(new_map, len);
			return (begin() + before_ele);
		}

		pointer get_node(iterator pos)
		{
			return (first + (pos - begin()));
		}

		void clear_for_assign()
		{
			first = last = map;
		}

	private:

		map_type map_allocate(const size_type n)
		{
			return alloc::allocate(n);
		}

		void free_oldMap()
		{
			alloc::deallocate(map, end_storage - map);
		}

		map_type my_memmove
		(map_type dst, map_type src, const size_type n)
		{
			std::memmove(dst, src, sizeof(value_type)*n);
			return dst;
		}

		map_type my_memcpy
		(map_type  dst, map_type src, const size_type n)
		{
			std::memcpy(dst, src, sizeof(value_type)*n);
			return (dst + n);
		}

		void deallocate_and_update_map(map_type new_map, size_type len)
		{
			free_oldMap();
			map = new_map;
			end_storage = map + len;
		}

	private:
		map_type map;
		map_type first;
		map_type last;
		map_type end_storage;
	};
}

#endif
