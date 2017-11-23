#pragma once
#ifndef _NUMERIC_QMJ_
#define _NUMERIC_QMJ_
#include"type_traits_qmj.h"
#include"iterator_qmj.h"

namespace qmj
{
	template<typename Iter,
		typename value_type,
		typename Fn2>inline
		value_type accumulate(Iter first, Iter last,
			value_type init, const Fn2& fn2)
	{
		for (; first != last; ++first)
			init = fn2(*first, init);
		return (init);
	}

	template<typename Iter,
		typename value_type>inline
		value_type accumulate(Iter first, Iter last,
			value_type init)
	{
		return (_QMJ accumulate(first, last, init, std::plus<>()));
	}

	template<typename Iter,
		typename OIter>
		OIter adjacent_difference(Iter first, Iter last,
			OIter dest)
	{
		return (_QMJ adjacent_difference(first, last,
			dest, std::minus<>()));
	}

	template<typename Iter,
		typename OIter,
		typename Fn2>inline
		OIter adjacent_difference(Iter first, Iter last,
			OIter dest, const Fn2& fn2)
	{
		if (first != last)
		{
			iter_val_t<Iter> val = *first;
			for (*dest = val; ++first != last;)
			{
				iter_val_t<Iter>tmp = *first;
				*++dest = fn2(tmp, val);
				val = std::move(tmp);
			}
			++dest;
		}
		return (dest);
	}

	template<typename Iter1,
		typename Iter2,
		typename value_type,
		typename Fn2_1,
		typename Fn2_2>inline
		value_type inner_product(Iter1 first1, Iter1 last1,
			Iter2 first2, value_type init, const Fn2_1&fn2_1,
			const Fn2_2& fn2_2)
	{
		for (; first1 != last1; ++first1, ++first2)
			init = fn2_1(init, fn2_2(*first1, *first2));
		return (init);
	}

	template<typename Iter1,
		typename Iter2,
		typename value_type>inline
		value_type inner_product(Iter1 first1, Iter1 last1,
			Iter2 first2, value_type init)
	{
		return (_QMJ inner_product(first1, last1, first2, init,
			std::plus<>(), std::multiplies<>()));
	}

	template<typename Iter,
		typename OIter,
		typename Fn2>inline
		OIter partial_sum(Iter first, Iter last,
			OIter dest, const Fn2& fn2)
	{
		if (first != last)
		{
			iter_val_t<Iter>val = *first;
			for (*dest = val; ++first != last; *++dest = val)
				val = fn2(*first, val);
			++dest;
		}
		return (dest);
	}

	template<typename Iter,
		typename OIter>inline
		OIter partial_sum(Iter first, Iter last,
			OIter dest)
	{
		return (_QMJ partial_sum(first, last, dest, std::plus<>()));
	}

	template<typename value_type,
		typename Integer,
		typename Fn2>
		value_type power(value_type x, Integer n, const Fn2& fn2)
	{
		if (n <= 0)
			return (x);
		for (; !(n & 1);)
		{
			n >>= 1;
			x = fn2(x, x);
		}
		value_type result = x;
		for (n >>= 1; n != 0; n >>= 1)
		{
			x = fn2(x, x);
			if (n & 1)
				result = fn2(result, x);
		}
		return (result);
	}

	template<typename value_type,
		typename Integer>
		value_type	power(value_type x, Integer n)
	{
		return (_QMJ power(x, n, std::multiplies<>()));
	}

	template<typename Iter,
		typename value_type>
		void iota(Iter first, Iter last, value_type val)
	{
		for (; first != last; ++first, ++val)
			*first = val;
	}

}





#endif
