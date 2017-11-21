#pragma once
#ifndef _FORWARD_LIST_QMJ_
#define _FORWARD_LIST_QMJ_
#include"allocator.h"
#include"iterator_qmj.h"

namespace qmj
{

	template<typename value_type>
	struct forward_list_node;

	template<typename value_type>
	struct forward_list_base_node
	{
		typedef forward_list_node<value_type>* link_type;

		forward_list_base_node(link_type next = nullptr) :
			next(next)
		{
		}

		link_type next;
	};

	template<typename value_type>
	struct forward_list_node:
		public forward_list_base_node<value_type>
	{
		typedef forward_list_base_node<value_type> base_type;
		typedef typename base_type::link_type link_type;

		forward_list_node(link_type next = nullptr) :
			base_type(next), data()
		{
		}

		forward_list_node(base_type*next) :
			base_type(next)
		{
		}

		forward_list_node(const value_type&val, link_type next = nullptr) :
			base_type(next), data(val)
		{
		}

		value_type data;
	};

	template<typename value_type>
	class forward_list_const_iterator
	{
	public:
		template<typename value_type, typename alloc>
		friend class forward_list;
		typedef std::forward_iterator_tag iterator_category;
		typedef value_type value_type;
		typedef const value_type* pointer;
		typedef const value_type& reference;
		typedef ptrdiff_t difference_type;

		typedef forward_list_const_iterator<value_type> self;
		typedef forward_list_node<value_type>* link_type;

		forward_list_const_iterator(link_type node = nullptr) :
			node(node)
		{
		}

		bool operator==(const self& x)
		{
			return node == x.node;
		}

		bool operator!=(const self& x)
		{
			return !(operator==(x));
		}

		self& operator=(const self&x)
		{
			node = x.node;
			return (*this);
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
			return (*this);
		}

		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return (tmp);
		}
	private:
		link_type get_node()
		{
			return node;
		}

	protected:
		link_type node;
	};

	template<typename value_type>
	class forward_list_iterator :
		public forward_list_const_iterator<value_type>
	{
	public:
		template<typename value_type, typename alloc>
		friend class forward_list;

		typedef std::forward_iterator_tag iterator_category;
		typedef value_type value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef ptrdiff_t difference_type;

		typedef forward_list_iterator<value_type> self;
		typedef forward_list_node<value_type>* link_type;
		typedef forward_list_const_iterator<value_type> iterator_base;

		forward_list_iterator(link_type node = nullptr) :
			iterator_base(node)
		{
		}

		forward_list_iterator(const self&x) :
			iterator_base(x.node)
		{
		}

		bool operator==(const self& x)
		{
			return node == x.node;
		}

		bool operator!=(const self& x)
		{
			return !(operator==(x));
		}

		self& operator=(const self&x)
		{
			node = x.node;
			return (*this);
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
			return (*this);
		}

		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return (tmp);
		}
	private:
		link_type get_node()
		{
			return node;
		}
	};

	template<typename value_type,
		typename Alloc = _QMJ allocator<value_type>>
		class forward_list
	{
	public:
		typedef Alloc allocator_type;
		typedef forward_list<value_type, Alloc> self;
		typedef forward_list_node<value_type> node_type;
		typedef typename forward_list_node<value_type>::base_type base_node_type;
		typedef typename allocator_type::template rebind<node_type>::other alloc;
		typedef typename allocator_type::template rebind<base_node_type>::other alloc_type;
		typedef node_type* link_type;
		typedef base_node_type* base_link_type;

		typedef value_type value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t differene_type;

		typedef forward_list_iterator<value_type> iterator;
		typedef forward_list_const_iterator<value_type> const_iterator;

		forward_list() :
			b_head(create_base_node())
		{//construct empty list
		}

		explicit forward_list(size_type n) :
			forward_list()
		{//construct list from n*value_tpe()
			resize(n);
		}

		forward_list(size_type n, const value_type&val) :
			forward_list()
		{//construct list from n*val
			insert_after_n(before_begin(), n, val);
		}

		forward_list(const self&x) :
			forward_list()
		{//copy construct
			insert_after(before_begin(), x.begin(), x.end());
		}

		forward_list(self&&x) :
			forward_list()
		{//move construct 
			b_head->next = x.b_head->next;
			x.b_head->next = nullptr;
		}

		template<typename Iter,
		typename=typename enable_if<is_iterator<Iter>::value,void>::type>
		forward_list(Iter first, Iter last) :
			forward_list()
		{//construct list using [first,last)
			insert_after(before_begin(), first, last);
		}

		forward_list(const std::initializer_list<value_type>&lst) :
			forward_list()
		{//construct list usign lst
			insert_after(before_begin(), lst.begin(), lst.end());
		}

		~forward_list()
		{//destroy and deallocate list
			clear();
			free_base_node(b_head);
		}

		self& operator=(self x)
		{//copy and move assign
			swap(x);
			return (*this);
		}

		self&operator=(std::initializer_list<value_type>&lst)
		{//assign a init list
			assign(lst);
		}

		reference front()
		{//return reference of first ele
			return (*begin());
		}

		const_reference front()const
		{//return reference of first ele
			return (*begin());
		}

		void push_front(const value_type&val)
		{//insert val as first ele
			insert_after_imple(before_begin().get_node(), val);
		}

		void push_front(value_type&&val)
		{//insert a rvalue val as first ele
			insert_after_imple(before_begin().get_node(), std::forward<value_type>(val));
		}

		void pop_front()
		{//pop first ele
			erase_after(before_begin());
		}

		void remove(const value_type&val)
		{// remove all val from list
			link_type before = b_head;
			for (link_type cur = before->next; cur;)
			{
				if (cur->data == val)
				{
					before->next = cur->next;
					destroy_and_free_node(cur);
				}
				else
					before = cur;
				cur = before->next;
			}
		}

		template<typename Pred>
		void remove_if(const Pred pred)
		{//remove all ele if satified pred
			link_type before = b_head;
			for (link_type cur = before->next; cur;)
			{
				if (pred(cur->data))
				{
					before->next = cur->next;
					destroy_and_free_node(cur);
				}
				else
					before = cur;
				cur = before->next;
			}
		}

		void resize(size_type n)
		{//redistribution size,use default of value_type supplement
			size_type old_size = size();
			iterator b_end = before_end();
			if (old_size < n)
				insert_after_n_default(b_end, n - old_size);
			else if (old_size > n)
			{
				iterator before = before_begin();
				_QMJ advance(before, n);
				erase_after(before, end());
			}
		}

		void resize(size_type n, const value_type&val)
		{//redistribution size,use val supplement
			size_type old_size = size();
			iterator b_end = before_end();
			if (old_size < n)//cur size less n
				insert_after_n(b_end, n - old_size, val);
			else if (old_size > n)
			{
				iterator before = before_begin();
				_QMJ advance(before, n);
				erase_after(before, end());
			}
		}

		allocator_type get_allocator()const
		{//return allocator of value
			return allocator_type();
		}

		size_type max_size()const
		{//return max size list can container
			return size_type(-1);
		}

		void swap(self&x)noexcept
		{//swap two list
			std::swap(b_head, x.b_head);
		}

		void clear()
		{//clear entire list
			erase_after(before_begin(), end());
		}

		bool empty()const noexcept
		{//return ture if list is empty
			return (!(b_head->next));
		}

		void assign(size_type n, const value_type&val)
		{//assign n val
			clear();
			insert_after(before_begin(), n, val);
		}

		template<typename Iter>
		void assign(Iter first, Iter last)
		{//assign [first,last)
			clear();
			insert_after(before_begin(), first, last);
		}

		void assign(const std::initializer_list<value_type>&lst)
		{//assign init list
			assign(lst.begin(), lst.end());
		}

		iterator before_begin()noexcept
		{//return iterator before begin;
			return iterator(b_head);
		}

		const_iterator before_begin()const noexcept
		{//return iterator before begin;
			return const_iterator(b_head);
		}

		const_iterator cbefore_begin()const noexcept
		{//return iterator before begin;
			return const_iterator(b_head);
		}

		iterator begin()noexcept
		{
			return iterator(b_head->next);
		}

		iterator end()noexcept
		{
			return iterator(nullptr);
		}

		const_iterator begin()const noexcept
		{
			return const_iterator(b_head->next);
		}

		const_iterator end()const noexcept
		{
			return const_iterator(nullptr);
		}

		const_iterator cbegin()const noexcept
		{
			return const_iterator(b_head->next);
		}

		const_iterator cend()const noexcept
		{
			return const_iterator(nullptr);
		}

		iterator insert_after(const_iterator pos, value_type&&val)
		{//insert rvalue after pos
			return emplace_after(pos, val);
		}

		iterator insert_after(const_iterator pos, const value_type&val)
		{//insert val after pos
			insert_after_imple(pos.get_node(), val);
			return (make_iter(++pos));
		}

		iterator insert_after(const_iterator pos, value_type n, const value_type&val)
		{//insert n val after pos
			return insert_after_n(pos, n, val);
		}

		template<typename Iter>
		iterator insert_after(const_iterator pos, Iter first, Iter last)
		{//insert element [first,last) after pos
			return insert_after_range(pos, first, last, _QMJ iterator_category(first));
		}

		iterator insert_after(const_iterator pos, const std::initializer_list<value_type>&lst)
		{//insert a init list after pos
			return insert_after(pos, lst.begin(), lst.end());
		}

		template<typename...types>
		void emplace_front(types&&...args)
		{//insert val as first ele
			insert_after_imple(before_begin().get_node(), std::forward<types>(args)...);
		}

		template<typename...types>
		iterator emplace_after(const_iterator pos, types&&...args)
		{//insert val after pos
			insert_after_imple(pos.get_node(), std::forward<types>(args)...);
			return (++pos);
		}

		iterator erase_after(const_iterator pos)
		{//erase element after pos
			link_type cur = pos.get_node();
			link_type tar = cur->next;
			cur->next = tar->next;
			destroy_and_free_node(tar);
			return (make_iter(++pos));
		}

		iterator erase_after(const_iterator b_first, const_iterator last)
		{//erase (b_first,last)
			link_type ed = last.get_node();
			for (link_type cur = b_first.get_node()->next; cur != ed;)
			{
				link_type tmp = cur->next;
				destroy_and_free_node(cur);
				cur = tmp;
			}
			b_first.get_node()->next = last.get_node();
			return (make_iter(last));
		}

		void splice_after(const_iterator pos, self&x)
		{//splice entire list x after pos
			if (this != &x && !x.empty())
				transfer_after(pos, x.before_begin(), x.end());
		}

		void splice_after(const_iterator pos, self&&x)
		{//splice entire rvalue list x after pos
			splice_after(pos, (self&)x);;
		}

		void splice_after(const_iterator pos, self&x, const_iterator first)
		{//splice after first element after pos
			const_iterator last = first;
			if (first != pos&&++last != x.end())
				transfer_after(pos, first, ++last);
		}

		void splice_after(const_iterator pos, self&&x, const_iterator first)
		{//splice after first element after pos
			splice_after(pos, (self&)x, first);
		}

		void splice_after(const_iterator pos, self&x, const_iterator first, const_iterator last)
		{//splice (first,last) after pos
			const_iterator after = first;
			if (++after != last&&pos != first)
				transfer_after(pos, first, last);
		}

		void splice_after(const_iterator pos, self&&x, const_iterator first, const_iterator last)
		{//splice (first,last) after pos
			splice_after(pos, (self&)x, first, last);
		}

		void reverse()noexcept
		{//reverse entire list
			if (!empty())
			{//before if pointer before the cur
				link_type before = b_head->next;
				link_type cur = before->next;
				before->next = nullptr;
				link_type next;
				for (; cur;)
				{
					next = cur->next;//next is next pointer of cur
					cur->next = before;
					before = cur;
					cur = next;
				}
				b_head->next = before;
			}
		}

		void unique()
		{//default remove a element if adjance two element is satisfied std::equal_to
			unique(std::equal_to<value_type>());
		}

		template<typename Pred>
		void unique(const Pred& pred)
		{//remove a element if adjance two element is satisfied pred
			link_type before = b_head->next;
			if (before)
			{
				for (link_type cur = before->next; cur;)
				{
					if (pred(cur->data, before->data))
					{
						before->next = cur->next;
						destroy_and_free_node(cur);
					}
					else
						before = cur;
					cur = before->next;
				}
			}
		}

		void merge(self&&x)
		{//self is sroted,merge another sorted rvalue list
			merge_imple(x, std::less<value_type>());
		}

		void merge(self&x)
		{//self is sroted,merge another sorted list
			merge_imple(x, std::less<value_type>());
		}

		template<typename Pred>
		void merge(self&&x, const Pred&pred)
		{//self is sroted,merge another sorted rvalue list,use pred
			merge_imple(x, pred);
		}

		template<typename Pred>
		void merge(self&x, const Pred&pred)
		{//self is sroted,merge another sorted list,use pred
			merge_imple(x, pred);
		}

		void sort()
		{// deafult sort by usign std::less
			sort(std::less<value_type>());
		}

		template<typename Pred>
		void sort(const Pred& pred)
		{//sort by usign pred
			sort_merge(before_begin(), end(), _QMJ distance(begin(), end()), pred);
		}

	private:
		template<typename...types>
		link_type insert_after_imple(link_type cur_node, types&&...args)
		{
			link_type new_node = create_node(cur_node->next,std::forward<types>(args)...);
			cur_node->next = new_node;
			return (new_node);
		}

		iterator insert_after_n(const_iterator pos, size_type n, const value_type&val)
		{
			link_type tmp = pos.get_node();
			link_type cur = tmp->next;
			for (; n != 0; --n)
				cur = create_node(cur, val);
			if (cur != pos.get_node())
				pos.get_node()->next = cur;
			return iterator(tmp);
		}

		iterator insert_after_n_default(const_iterator pos, size_type n)
		{
			link_type tmp = pos.get_node();
			link_type cur = tmp->next;
			for (; n != 0; --n)
				cur = create_node(cur);
			if (cur != pos.get_node())
				pos.get_node()->next = cur;
			return iterator(tmp);
		}

		template<typename InputIter>
		iterator insert_after_range(const_iterator pos, 
			InputIter first, InputIter last, std::input_iterator_tag)
		{
			link_type ret = pos.get_node();
			for (; first != last; ++first)
				ret=insert_after_imple(ret, *first);
			return iterator(ret);
		}

		template<typename FwdIter>
		iterator insert_after_range(const_iterator pos,
			FwdIter first, FwdIter last, std::forward_iterator_tag)
		{
			link_type ret = pos.get_node();
			for (; first != last; ++first)
				ret = insert_after_imple(ret, *first);
			return iterator(ret);
		}

		template<typename BidIter>
		iterator insert_after_range(const_iterator pos,
			BidIter first, BidIter last, std::bidirectional_iterator_tag)
		{
			link_type tmp = pos.get_node();
			link_type cur = tmp->next;
			if (first != last)
				tmp = cur = create_node(cur, *--last);
			for (; first != last;)
				cur = create_node(cur, *--last);
			if (cur != pos.get_node())
				pos.get_node()->next = cur;
			return iterator(tmp);
		}

		template<typename Pred>
		void merge_imple(self&right, const Pred& pred)
		{//merge sorted right by using pred
			iterator first1 = before_begin();
			iterator after1 = begin();
			iterator last1 = end();
			iterator first2 = right.before_begin();
			iterator after2 = right.begin();
			iterator last2 = right.end();
			if (after1 == last1)//self is empty
				splice_after(first1, right);
			else if (after2 == last2)//right is empty
				return;
			else
				for (;;)
				{
					if (pred(*after2, *after1))
					{
						iterator tmp = after2;
						transfer_after(first1, first2, ++tmp);
						++first1;
						if ((after2 = tmp) == last2)
							return;
					}
					else
					{
						++first1;
						if (++after1 == last1)
							return;
					}
				}
		}

		void transfer_after(const_iterator pos, const_iterator b_first, const_iterator last)
		{//transfer (b_first,last) after pos
			link_type bg = b_first.get_node();
			link_type tail = bg;
			link_type ed = last.get_node();
			for (link_type cur = tail->next; cur != ed; cur = tail->next)
				tail = cur;
			link_type pos_node = pos.get_node();
			tail->next = pos_node->next;
			pos_node->next = bg->next;
			bg->next = ed;
		}

		size_type size()const
		{//size of list
			size_type count = 0;
			for (link_type cur = b_head->next; cur; cur = cur->next)
				++count;
			return count;
		}

		iterator before_end()const
		{// the last second iterator
			link_type ret = b_head;
			for (link_type cur = ret->next; cur; cur = cur->next)
				ret = cur;
			return iterator(ret);
		}

		iterator next(const_iterator iter, size_type n = 1)
		{//return nth iterator after iter
			_QMJ advance(iter, n);
			return (make_iter(iter));
		}

		template<typename Pred>
		void sort_merge(const_iterator before, const_iterator end, size_type len, const Pred&pred)
		{//use algorithm of merge sort
			if (len < 2)//do nothing if size is 0 or 1
				return;
			size_type half = len >> 1;
			const_iterator before_mid = next(before, half);//iterator before mid
			sort_merge(before_mid, end, len - half, pred);//sort(mid,end)
			sort_merge(before, ++before_mid, half, pred);//sort(before,mid)
			const_iterator first1 = next(before);//the iterator of min element [first1,mid)
			before_mid = next(before, half);
			const_iterator first2 = next(before_mid);//the iterator of min element [mid,end) 

			for (;;)
			{//merge sorted [first1,mid) and [mid,end)
				if (pred(*first2, *first1))
				{
					++first2;
					transfer_after(before, before_mid, next(before_mid, 2));// splice_after(before,*this,before_mid)
					++before;
					if (first2 == end)
						return;
				}
				else
				{
					++before;
					if (++first1 == first2)
						return;
				}
			}
		}

	private:
		template<typename...types>
		link_type create_node(link_type cur,types&&...args)
		{//allocate and construct a list_node by using args
			link_type ptr = alloc::allocate();
			alloc::construct(ptr, std::forward<types>(args)..., cur);
			return (ptr);
		}

		link_type create_base_node()
		{//create a list_base_node ,return pointer of list_node
			link_type ptr = (link_type) alloc_type::allocate();
			alloc_type::construct((base_link_type)ptr,nullptr);
			return (ptr);
		}

		void free_base_node(link_type node)
		{//the object that node pointer is list_base_node
			//std::free(node);
			alloc_type::deallocate((base_link_type)node);
		}

		void destroy_and_free_node(link_type node)
		{//destroy and deallocate a list_node by pointer
			alloc::destroy(&(node->data));
			alloc::deallocate(node);
		}

		iterator make_iter(const_iterator tar)const
		{
			return (iterator(tar.node));
		}

	private:
		//the object that node pointer is list_base_node
		link_type b_head;
	};

	template<typename value_type,typename alloc>inline
	void swap(_QMJ forward_list<value_type, alloc>&left,
		_QMJ forward_list<value_type, alloc>&right)noexcept
	{
		left.swap(right);
	}

	template<typename value_type, typename alloc>inline
	bool operator==(const _QMJ forward_list<value_type, alloc>&left,
		const _QMJ forward_list<value_type, alloc>&right)
	{
		return std::equal(left.begin(), left.end(), right.begin(), right.end());
	}

	template<typename value_type, typename alloc>inline
		bool operator!=(const _QMJ forward_list<value_type, alloc>&left,
			const _QMJ forward_list<value_type, alloc>&right)
	{
		return (!(left == right));
	}

	template<typename value_type, typename alloc>inline
		bool operator<(const _QMJ forward_list<value_type, alloc>&left,
			const _QMJ forward_list<value_type, alloc>&right)
	{
		return std::lexicographical_compare
		(left.begin(), left.end(), right.begin(), right.end());
	}

	template<typename value_type, typename alloc>inline
		bool operator>(const _QMJ forward_list<value_type, alloc>&left,
			const _QMJ forward_list<value_type, alloc>&right)
	{
		return (right < left);
	}

	template<typename value_type, typename alloc>inline
		bool operator<=(const _QMJ forward_list<value_type, alloc>&left,
			const _QMJ forward_list<value_type, alloc>&right)
	{
		return (!(right < left));
	}

	template<typename value_type, typename alloc>inline
		bool operator>=(const _QMJ forward_list<value_type, alloc>&left,
			const _QMJ forward_list<value_type, alloc>&right)
	{
		return (!(left < right));
	}

}

#endif
