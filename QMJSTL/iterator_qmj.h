#pragma once
#ifndef _ITERATOR_QMJ_
#define _ITERATOR_QMJ_
#include"type_traits_qmj.h"

namespace qmj
{
	template<typename Iter>inline
		typename qmj::iterator_traits<Iter>::iterator_category
		iterator_category(const Iter&iter)
	{
		return qmj::iterator_traits<Iter>::iterator_category();
	}

	template<typename Iter>inline
		typename qmj::iterator_traits<Iter>::value_type
		value_type(const Iter&iter)
	{
		return qmj::iterator_traits<Iter>::value_type();
	}

	template<typename Iter>inline
		typename qmj::iterator_traits<Iter>::difference_type
		difference_type(const Iter&iter)
	{
		return qmj::iterator_traits<Iter>::difference_type();
	}

	template<typename Iter>inline
		typename qmj::iterator_traits<Iter>::pointer
		pointer(const Iter&iter)
	{
		return qmj::iterator_traits<Iter>::pointer();
	}

	template<typename Iter>inline
		typename qmj::iterator_traits<Iter>::reference
		reference(const Iter&iter)
	{
		return qmj::iterator_traits<Iter>::reference();
	}
}

namespace qmj
{
	template<typename Iter,typename difference>
	inline void advance_imple
	(Iter& iter, difference n, std::input_iterator_tag)
	{
		while (n--) ++iter;
	}

	template<typename Iter,typename difference>
	inline void advance_imple
	(Iter&iter, difference n, std::bidirectional_iterator_tag)
	{
		if (n >= 0)
			while (n--)++iter;
		else
			while (n++)--iter;
	}

	template<typename Iter,typename difference>
	inline void advance_imple
	(Iter&iter, difference n, std::random_access_iterator_tag)
	{
		iter += n;
	}

	template<typename Iter,typename difference>
	inline void advance(Iter&iter, difference n)
	{
		advance_imple(iter, n, qmj::iterator_category(iter));
	}

	template<typename Iter>
	inline typename qmj::iterator_traits<Iter>::difference_type
		distance_imple(Iter first, Iter last, std::input_iterator_tag)
	{
		qmj::iterator_traits<Iter>::difference_type n = 0;
		for (; first != last; ++first)
			++n;
		return (n);
	}

	template<typename Iter>
		inline typename qmj::iterator_traits<Iter>::difference_type
		distance_imple(Iter first, Iter last, std::random_access_iterator_tag)
	{
		return last - first;
	}

	template<typename Iter>
	inline typename qmj::iterator_traits<Iter>::difference_type
		distance(Iter first, Iter last)
	{
		return distance_imple(first, last, qmj::iterator_category(first));
	}

}



#endif
