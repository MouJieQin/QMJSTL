#pragma once
#ifndef _SET_QMJ_
#define _SET_QMJ_

#include"rb_tree.h"


namespace qmj
{

	template<
		typename key_type,
		typename key_compare,
		typename Alloc,
		bool is_multi>
		struct set_traits
	{
		typedef key_type key_type;
		typedef key_type value_type;
		typedef key_compare key_compare;
		typedef key_compare value_compare;
		typedef Alloc allocator_type;
		enum
		{
			is_multi = is_multi
		};

		static const key_type& keyOfValue(const key_type&key)
		{
			return (key);
		}

		/*struct keyOfValue:
			public unary_function<key_type, key_type>
		{
			const key_type& operator()(const key_type&key)const
			{
				return key;
			}
		};*/
	};

	template<typename key_type,
		typename Compare = std::less<key_type>,
		typename Alloc = qmj::allocator<key_type>>
		class set:
		public rb_tree<set_traits<key_type,Compare,Alloc,false>>
	{
	public:
		typedef key_type key_type;
		typedef key_type value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef rb_tree<set_traits<key_type, Compare, Alloc, false>> base_type;
		typedef typename base_type::iterator rbt_iterator;

		typedef typename base_type::const_pointer pointer;
		typedef typename base_type::const_pointer const_pointer;
		typedef typename base_type::const_reference reference;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::const_iterator iterator;
		typedef typename base_type::const_iterator const_iterator;
		typedef typename base_type::const_reverse_iterator reverse_iterator;
		typedef typename base_type::const_reverse_iterator const_reverse_iterator;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;

		typedef set<key_type, Compare, Alloc>	self;
		typedef typename std::pair<iterator, bool> pairib;

		set() :
			base_type()
		{
		}

		explicit set(const Compare& comp) :
			base_type(comp)
		{
		}

		set(const self&x) :
			base_type(x)
		{
		}

		template<typename Iter>
		set(Iter first, Iter last) :
			base_type()
		{
			base_type::insert(first, last);
		}

		template<typename InputIterator>
		set(InputIterator first, InputIterator last, const Compare& comp) :
			base_type(comp)
		{
			base_type::insert(first, last);
		}

		set(const std::initializer_list<key_type>&lst, const Compare&comp) :
			set(lst.begin(),lst.end(),comp)
		{
		}

		set(const std::initializer_list<key_type>&lst) :
			set(lst.begin(),lst.end())
		{
		}

		set(self&&x) :
			base_type(std::move(x))
		{
		}

		self& operator=(const self& x)
		{
			base_type::operator=(x);
			return (*this);
		}

		self& operator=(self&&x)
		{
			base_type::operator=(std::move(x));
			return (*this);
		}

		void swap(self&x)noexcept
		{
			base_type::swap(x);
		}
	};

	template<typename key_type,
		typename Compare,
		typename Alloc>inline
		void swap(set<key_type, Compare, Alloc>&left,
			set<key_type, Compare, Alloc>&right)noexcept
	{
		left.swap(right);
	}
}

namespace qmj
{

	template<typename key_type,
		typename Compare = std::less<key_type>,
		typename Alloc = qmj::allocator<key_type>>
		class multiset :
		public rb_tree<set_traits<key_type, Compare, Alloc, true>>
	{
	public:
		typedef key_type key_type;
		typedef key_type value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef rb_tree<set_traits<key_type, Compare, Alloc, true>> base_type;
		typedef typename base_type::iterator rbt_iterator;

		typedef typename base_type::const_pointer pointer;
		typedef typename base_type::const_pointer const_pointer;
		typedef typename base_type::const_reference reference;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::const_iterator iterator;
		typedef typename base_type::const_iterator const_iterator;
		typedef typename base_type::const_reverse_iterator reverse_iterator;
		typedef typename base_type::const_reverse_iterator const_reverse_iterator;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;

		typedef multiset<key_type, Compare, Alloc>	self;
		typedef typename std::pair<iterator, bool> pairib;

		multiset() :
			base_type()
		{
		}

		explicit multiset(const Compare& comp) :
			base_type(comp)
		{
		}

		multiset(const self&x) :
			base_type(x)
		{
		}

		template<typename Iter>
		multiset(Iter first, Iter last) :
			base_type()
		{
			base_type::insert(first, last);
		}

		template<typename InputIterator>
		multiset(InputIterator first, InputIterator last, const Compare& comp) :
			base_type(comp)
		{
			base_type::insert(first, last);
		}

		multiset(const std::initializer_list<key_type>&lst, const Compare&comp) :
			multiset(lst.begin(), lst.end(), comp)
		{
		}

		multiset(const std::initializer_list<key_type>&lst) :
			multiset(lst.begin(), lst.end())
		{
		}

		multiset(self&&x) :
			base_type(std::move(x))
		{
		}

		self& operator=(const self& x)
		{
			base_type::operator=(x);
			return (*this);
		}

		self& operator=(self&&x)
		{
			base_type::operator=(std::move(x));
			return (*this);
		}

		void swap(self&x)noexcept
		{
			base_type::swap(x);
		}
	};

	template<typename key_type,
		typename Compare,
		typename Alloc>inline
		void swap(multiset<key_type, Compare, Alloc>&left,
			multiset<key_type, Compare, Alloc>&right)noexcept
	{
		left.swap(right);
	}
}


#endif