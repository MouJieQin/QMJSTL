#pragma once
#ifndef _QUEUE_QMJ_
#define _QUEUE_QMJ_
#include"deque_qmj.h"
#include"heap.h"

namespace qmj
{
	template<typename value_type,
		typename Container = _QMJ deque<value_type>>
		class queue
	{
	public:
		typedef typename Container container_type;
		typedef typename  container_type::value_type value_type;
		typedef typename container_type::size_type size_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::const_reference const_reference;

		typedef queue<value_type, Container> self;

		queue() :
			c()
		{
		}

		explicit queue(const Container&cont) :
			c(cont)
		{
		}

		template<typename Iter>
		queue(Iter first, Iter last) :
			c(first, last)
		{
		}

		queue(const self&x) :
			c(x.c)
		{
		}

		self& operator=(const self&x)
		{
			c = x.c;
			return (*this);
		}

		void swap(self&x)noexcept
		{
			swap(c, x.c);
		}

		bool empty()const
		{
			return c.empty();
		}

		size_t size()const
		{
			return c.size();
		}

		reference front()
		{
			return c.front();
		}

		const_reference front()const
		{
			return c.front();
		}

		reference back()
		{
			return c.back();
		}

		const_reference back()const
		{
			return c.back();
		}

		void push(const value_type&val)
		{
			c.push_back(val);
		}

		void pop()
		{
			c.pop_front();
		}

		value_type pop_front()
		{
			value_type ret = c.front();
			c.pop_front();
			return ret;
		}

		const Container& _get_container()const
		{
			return c;
		}

	private:
		Container c;
	};

	template<typename value_type, typename Container>inline
		void swap(const _QMJ queue<value_type, Container>&left,
			const _QMJ queue<value_type, Container>&right)noexcept
	{
		left.swap(right);
	}

	template<typename value_type, typename Container>inline
		bool operator==(const _QMJ queue<value_type, Container>&left,
			const _QMJ queue<value_type, Container>&right)
	{
		return (left._get_constainer() == right._get_constainer());
	}

	template<typename value_type, typename Container>inline
		bool operator!=(const _QMJ queue<value_type, Container>&left,
			const _QMJ queue<value_type, Container>&right)
	{
		return (left._get_constainer() != right._get_constainer());
	}

	template<typename value_type, typename Container>inline
		bool operator<(const _QMJ queue<value_type, Container>&left,
			const _QMJ queue<value_type, Container>&right)
	{
		return (left._get_constainer() < right._get_constainer());
	}

	template<typename value_type, typename Container>inline
		bool operator<=(const _QMJ queue<value_type, Container>&left,
			const _QMJ queue<value_type, Container>&right)
	{
		return (left._get_constainer() <= right._get_constainer());
	}

	template<typename value_type, typename Container>inline
		bool operator>(const _QMJ queue<value_type, Container>&left,
			const _QMJ queue<value_type, Container>&right)
	{
		return (left._get_constainer() > right._get_constainer());
	}

	template<typename value_type, typename Container>inline
		bool operator>=(const _QMJ queue<value_type, Container>&left,
			const _QMJ queue<value_type, Container>&right)
	{
		return (left._get_constainer() >= right._get_constainer());
	}
}

namespace qmj
{
	template<typename value_type,
		typename Container=_QMJ vector<value_type>,
		typename Compare=std::less<value_type>>
		class priority_queue:
		public binary_heap<value_type,Container,Compare>
	{
	public:
		typedef priority_queue<value_type,Container,Compare> self;
		typedef Container container_type;
		typedef binary_heap<value_type,Container, Compare> my_base;

		typedef typename container_type::value_type value_type;
		typedef typename container_type::size_type size_type;
		typedef typename container_type::reference reference;
		typedef typename container_type::const_reference const_reference;

		priority_queue() :
			my_base()
		{
		}

		explicit priority_queue(const Compare&comp) :
			my_base(comp)
		{
		}

		priority_queue(const Compare&comp, const Container&cont) :
			my_base(cont, comp)
		{
		}

		template<typename Iter>
		priority_queue(Iter first, Iter last) :
			my_base(first, last)
		{
		}

		template<typename Iter>
		priority_queue(Iter first, Iter last, const Compare&comp) :
			my_base(first, last,comp)
		{
		}

		template<typename Iter>
		priority_queue
		(Iter first, Iter last, const Compare&comp, const Container&cont) :
			my_base(first, last, cont, comp)
		{
		}

		self& operator=(const self&x)
		{
			my_base::operator=(x);
			return (*this);
		}
	};
}

#endif