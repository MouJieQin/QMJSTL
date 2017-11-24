#pragma once
#ifndef _ALGORITHM_QMJ_	
#define _ALGORITHM_QMJ_
#include<utility>
#include"type_traits_qmj.h"
namespace qmj
{
	constexpr size_t sort_threshold = 32;

	template<typename Iter>inline
		Iter next(Iter cur, _QMJ iter_dif_t<Iter> dif)
	{
		_QMJ advance(cur, dif);
		return (cur);
	}

	template<typename Iter>inline
		Iter next(Iter cur)
	{
		return (++next);
	}

	template<typename Iter>inline
		Iter prev(Iter cur, _QMJ iter_dif_t<Iter> dif)
	{
		_QMJ advance(cur, dif);
		return (cur);
	}

	template<typename Iter>inline
		Iter prev(Iter cur)
	{
		return (--cur);
	}

	template<typename FIter,typename=void>
	struct has_mem_iter_swap :
		_QMJ false_type
	{
	};

	template<typename FIter>
	struct has_mem_iter_swap<FIter, void_t<decltype(std::declval<FIter>().iter_swap(FIter()))>> :
		_QMJ true_type
	{
	};

	template<typename FIter>inline
		void _iter_swap_imple(FIter left, FIter right, _QMJ false_type)
	{
		std::iter_swap(left, right);
	}

	template<typename FIter>inline
		void _iter_swap_imple(FIter left, FIter right, _QMJ true_type)
	{
		left.iter_swap(right);
	}

	template<typename FIter1,
		typename FIter2>inline
		void _iter_swap_same(FIter1 left, FIter2 right, _QMJ true_type)
	{
		_QMJ _iter_swap_imple(left, right, has_mem_iter_swap<FIter1>());
	}

	template<typename FIter1,
		typename FIter2>inline
		void _iter_swap_same(FIter1 left, FIter2 right, _QMJ false_type)
	{
		std::iter_swap(left, right);
	}

	template<typename FIter1,
	typename FIter2>inline
	void iter_swap(FIter1 left, FIter2 right)
	{
		_QMJ _iter_swap_same(left, right, _QMJ is_same<FIter1, FIter2>());
	}

	template<typename Iter,
		typename Comp>inline
		iter_val_t<Iter>
		max(Iter first, Iter last, const Comp&cmp)
	{
		iter_val_t<Iter>val = *first;
		for (; ++first != last;)
			if (cmp(val, *first))
				val = *first;
		return (val);
	}

	template<typename Iter>inline
		iter_val_t<Iter>
		max(Iter first, Iter last)
	{
		return (_QMJ max(first, last, std::less<>()));
	}

	template<typename Iter,
		typename Comp>inline
		iter_val_t<Iter>
		min(Iter first, Iter last, const Comp&cmp)
	{
		iter_val_t<Iter>val = *first;
		for (; ++first != last;)
			if (cmp(*first, val))
				val = *first;
		return (val);
	}

	template<typename Iter>
	iter_val_t<Iter>inline
		min(Iter first, Iter last)
	{
		return (_QMJ min(first, last, std::less<>()));
	}

	template<typename Iter1,
		typename Iter2,
		typename Pred>inline
		std::pair<Iter1, Iter2>
		mismatch(Iter1 first1, Iter1 last1, Iter2 first2,
			const Pred&pred)
	{
		for (; first1 != last1&&pred(*first1, *first2);)
		{
			++first1;
			++first2;
		}
		return (std::pair<Iter1, Iter2>(first1, first2));
	}

	template<typename Iter1,
		typename Iter2>
		std::pair<Iter1, Iter2>inline
		mismatch(Iter1 first1, Iter1 last1, Iter2 first2)
	{
		return (_QMJ mismatch(first1, last1, first2, std::equal_to<>()));
	}

	template<typename Iter1,
		typename Iter2,
		typename Pred>inline
		std::pair<Iter1, Iter2>
		mismatch(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2,
			const Pred&pred)
	{
		for (; first1 != last1&&first2 != last2
			&&pred(*first1, *first2);)
		{
			++first1;
			++first2;
		}
		return (std::pair<Iter1, Iter2>(first1, first2));
	}

	template<typename Iter1,
		typename Iter2>inline
		std::pair<Iter1, Iter2>
		mismatch(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
	{
		return (_QMJ mismatch(first1, last1, first2, last2, std::equal_to<>()));
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter,
		typename Comp>inline
		OIter set_union(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2,
			OIter dest, const Comp&cmp)
	{
		if (first1 != last1&&first2 != last2)
		{
			for (;;)
			{
				if (cmp(*first1, *first2))
				{
					*dest++ = *first1;
					if (++first1 == last1)
						break;
				}
				else if (cmp(*first2, *first1))
				{
					*dest++ = *first2;
					if (++first2 == last2)
						break;
				}
				else
				{
					*dest = *first1;
					++first1;
					++first2;
					if (first1 == last1
						|| first2 == last2)
						break;
				}
			}
		}
		dest = std::copy(first1, last1, dest);
		return (std::copy(first2, last2, dest));
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter>inline
		OIter set_union(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2, OIter dest)
	{
		return (_QMJ set_union(first1, last1,
			first2, last2, dest, std::less<>()));
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter,
		typename Comp>inline
		OIter set_intersection(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2,
			OIter dest, const Comp&cmp)
	{
		if (first1 != last1&&first2 != last2)
		{
			for (;;)
			{
				if (cmp(*first1, *first2))
				{
					if (++first1 == last1)
						break;
				}
				else if (cmp(*first2, *first1))
				{
					if (++first2 == last2)
						break;
				}
				else
				{
					*dest++ = *first1;
					++first1;
					++first2;
					if (first1 == last1
						|| first2 == last2)
						break;
				}
			}
		}
		return (dest);
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter>inline
		OIter set_intersection(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2,
			OIter dest)
	{
		return (_QMJ set_intersection(first1, last1,
			first2, last2, dest, std::less<>()));
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter,
		typename Comp>inline
		OIter set_difference(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2,
			OIter dest, const Comp&cmp)
	{
		if (first1 != last1&&first2 != last2)
		{
			for (;;)
			{
				if (cmp(*first1, *first2))
				{
					*dest++ = *first1;
					if (++first1 == last1)
						breakk;
				}
				else if (cmp(*first2, *first1))
				{
					if (++first2 == last2)
						break;
				}
				else
				{
					++first1;
					++first2;
					if (first1 == last1
						|| first2 == last2)
						break;
				}
			}
		}
		return (std::copy(first1, last1, dest));
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter>inline
		OIter set_difference(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2,
			OIter dest)
	{
		return (_QMJ set_difference(first1, last1,
			first2, last2, dest, std::less<>()));
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter,
		typename Comp>inline
		OIter set_symmetric_difference(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2,
			OIter dest, const Comp&cmp)
	{
		if (first1 != last1&&first2 != last2)
		{
			for (;;)
			{
				if (cmp(*first1, *first2))
				{
					*dest++ = *first1;
					if (++first1 == last1)
						break;
				}
				else if (cmp(*first2, *first1))
				{
					*dest++ = *first2;
					if (++first2 == last2)
						break;
				}
				else
				{
					++first1;
					++first2;
					if (first1 == last1
						|| first2 == last2)
						break;
				}
			}
		}
		dest = std::copy(first2, last2, dest);
		return (std::copy(first1, last1, dest));
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter>inline
		OIter set_symmetric_difference(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2,
			OIter dest)
	{
		return (_QMJ set_symmetric_difference(first1, last1,
			first2, last2, dest, std::less<>()));
	}

	template<typename FIter,
		typename Pred>inline
		FIter adjacent_find(FIter first, FIter last, const Pred&pred)
	{
		if (first != last)
		{
			for (FIter next = first; ++next != last; first = next)
				if (pred(*first, *next))
					return (first);
		}
		return (last);
	}

	template<typename FIter>inline
		FIter adjacent_find(FIter first, FIter last)
	{
		return (_QMJ adjacent_find(first, last, std::equal_to<>()));
	}

	template<typename Iter,
		typename Pred>inline
		iter_dif_t<Iter>
		count_if(Iter first, Iter last, const Pred&pred)
	{
		iter_dif_t<Iter> counter = 0;
		for (; first != last; ++first)
			if (pred(*first))
				++count;
		return (counter);
	}

	template<typename Iter,
		typename value_type>
		iter_dif_t<Iter>inline
		count(Iter first, Iter last, const value_type&val)
	{
		iter_dif_t<Iter> counter = 0;
		for (; first != last; ++first)
			if (*first == val)
				++counter;
		return (counter);
	}

	template<typename FIter,
		typename Pred>
		FIter _partition_imple(FIter first, FIter last, const Pred&pred, std::forward_iterator_tag)
	{
		for (; first != last&&pred(*first);)
			++first;

		if (first == last)
			return (first);

		for (FIter next = first; ++next != last;)
			if (pred(*next))
				_QMJ iter_swap(first++, next);

		return (first);
	}


	template<typename BIter,
		typename Pred>
		BIter _partition_imple(BIter first, BIter last,
			const Pred&pred, std::bidirectional_iterator_tag)
	{
		for (;; ++first)
		{
			for (; first != last&&pred(*first);)
				++first;
			if (first == last)
				break;

			for (; first != --last && (!pred(*last));)
				;
			if (first == last)
				break;

			_QMJ iter_swap(first, last);
		}
		return (first);
	}

	template<typename Iter,
		typename Pred>
		Iter partition(Iter first, Iter last, const Pred& pred)
	{
		return (_QMJ _partition_imple(first, last, pred, iter_cate_t<Iter>()));
	}

	template<typename FIter1,
		typename FIter2,
		typename Pred>inline
		FIter1 _search_imple(FIter1 first1, FIter1 last1,
			FIter2 first2, FIter2 last2, const Pred&pred,
			std::forward_iterator_tag, std::forward_iterator_tag)
	{
		for (; first1 != last1; ++first1)
		{
			FIter1 cpf1 = first1;
			for (FIter2 cpf2 = first2;; ++cpf1, ++cpf2)
				if (cpf2 == last2)
					return (first1);
				else if (!pred(*cpf1, *cpf2))
					break;
		}
		return (last1);
	}

	template<typename FIter1,
		typename FIter2,
		typename Pred>inline
		FIter1 _search_imple(FIter1 first1, FIter1 last1,
			FIter2 first2, FIter2 last2, const Pred&pred,
			std::random_access_iterator_tag, std::random_access_iterator_tag)
	{
		iter_dif_t<FIter1>count1 = last1 - first1;
		iter_dif_t<FIter2>count2 = last2 - first2;
		for (; count2 <= count1; --count1, ++first1)
		{
			FIter1 cpf1 = first1;
			for (FIter2 cpf2 = first2;; ++cpf1, ++cpf2)
			{
				if (cpf2 == last2)
					return (first1);
				else if (!pred(*cpf1, *cpf2))
					break;
			}
		}
		return (last1);
	}


	template<typename FIter1,
		typename FIter2,
		typename Pred>inline
		FIter1 search(FIter1 first1, FIter1 last1,
			FIter2 first2, FIter2 last2, const Pred&pred)
	{
		return (_QMJ _search_imple(first1, last1,
			first2, last2, pred, iter_cate_t<FIter1>(),
			iter_cate_t<FIter2>()));
	}

	template<typename FIter1,
		typename FIter2>inline
		FIter1 search(FIter1 first1, FIter1 last1,
			FIter2 first2, FIter2 last2)
	{
		return (_QMJ search(first1, last1,
			first2, last2, std::equal_to<>()));
	}

	template<typename FIter,
		typename Dif,
		typename value_type,
		typename Pred>inline
		FIter _search_n_imple(FIter first, FIter last,
			Dif n, const value_type&val, const Pred&pred,
			std::forward_iterator_tag)
	{
		for (; first != last; ++first)
		{
			if (pred(*first, val))
			{//first may be result
				FIter cur = first;
				++cur;
				for (Dif cpn = n - 1;; --cpn, ++cur)
					if (!cpn)
						return (first);
					else if (!pred(*cur, val))
					{
						first = cur;
						break;
					}
			}
		}
		return (last);
	}

	template<typename FIter,
		typename Dif,
		typename value_type,
		typename Pred>inline
		FIter _search_n_imple(FIter first, FIter last,
			Dif n, const value_type&val, const Pred&pred,
			std::random_access_iterator_tag)
	{
		if (n <= 0)
			return (first);
		iter_dif_t<FIter> count = last - first;//size of [first,ast)
		FIter result = first;//may be result
		for (Dif mark = n - 1; mark < count; mark += n)
		{
			if (pred(first[mark], val))
			{//[ret,first+mark] may be satisifyed
				for (Dif cur = 0;; ++cur)
					if (cur == n)
						return (result);
					else if (!pred(result[cur], val))
					{//adjust mark
						mark -= (n - cur - 1);
						break;
					}
			}
			result = first + mark + 1;//update ret
		}
		return (last);
	}

	template<typename FIter,
		typename Dif,
		typename value_type,
		typename Pred>inline
		FIter search_n(FIter first, FIter last, Dif n,
			const value_type&val, const Pred&pred)
	{
		return (_QMJ _search_n_imple(first, last, n,
			val, pred, iter_cate_t<FIter>()));
	}

	template<typename FIter,
		typename Dif,
		typename value_type>inline
		FIter search_n(FIter first, FIter last, Dif n,
			const value_type&val)
	{
		return (_QMJ search_n(first, last,
			n, val, std::equal_to<>()));
	}

	template<typename Iter,
		typename value_type>inline
		Iter find(Iter first, Iter last, const value_type&val)
	{
		for (; first != last && *first != val;)
			++first;
		return (first);
	}

	template<typename Iter,
		typename Pred>inline
		Iter find_if(Iter first, Iter last, const Pred&pred)
	{
		for (; first != last && (!pred(*first)))
			++first;
		return (first);
	}

	template<typename FIter1,
		typename FIter2,
		typename Pred>inline
		FIter1 _find_end_imple(FIter1 first1, FIter1 last1,
			FIter2 first2, FIter2 last2, const Pred&pred,
			std::forward_iterator_tag, std::forward_iterator_tag)
	{
		FIter1 result = last1;
		FIter1 new_result;
		while (true)
		{
			new_result = _QMJ search(first1, last1,
				first2, last2, pred);
			if (new_result == last1)
				return (result);
			else
			{
				result = new_result;
				first1 = ++new_result;
			}
		}
		return (last1);
	}

	template<typename BIter1,
		typename BIter2,
		typename Pred>inline
		BIter1 _find_end_imple(BIter1 first1, BIter1 last1,
			BIter2 first2, BIter2 last2, const Pred&pred,
			std::bidirectional_iterator_tag, std::bidirectional_iterator_tag)
	{
		typedef std::reverse_iterator<BIter1>RBIter1;
		typedef std::reverse_iterator<BIter2>RBIter2;

		RBIter1 rlast1(first1);
		RBIter2 rlast2(first2);
		RBIter1 rresult = _QMJ search(RBIter1(last1),
			rlast1, RBIter2(last2), rlast2, pred);
		if (rresult == rlast1)
			return (last1);
		else
		{
			BIter1 result = rresult.base();
			_QMJ advance(result, -distance(first2, last2));
			return (result);
		}
	}

	template<typename Iter1,
		typename Iter2,
		typename Pred>inline
		Iter1 find_end(Iter1 first1, Iter1 last1,
			Iter2 first2, Iter2 last2, const Pred&pred)
	{
		return (_QMJ _find_end_imple(first1, last1,
			first2, last2, pred, iter_cate_t<Iter1>(),
			iter_cate_t<Iter2>()));
	}

	template<typename Iter1,
		typename Iter2>inline
		Iter1 find_end(Iter1 first1, Iter1 last1,
			Iter2 first2, Iter2 last2)
	{
		return (_QMJ find_end(first1, last1, first2, last2,
			std::equal_to<>()));
	}

	template<typename FIter1,
		typename FIter2,
		typename Pred>inline
		FIter1 find_first_of(FIter1 first1, FIter1 last1,
			FIter2 first2, FIter2 last2, const Pred&pred)
	{
		for (; first1 != last1; ++first1)
			for (FIter2 cur = first2; cur != last2; ++cur)
				if (pred(*first1, *cur))
					return (first1);
		return (last1);
	}

	template<typename FIter1,
		typename FIter2>inline
		FIter1 find_first_of(FIter1 first1, FIter1 last1,
			FIter2 first2, FIter2 last2)
	{
		return (_QMJ find_first_of(first1, last1,
			first2, last2, std::equal_to<>()));
	}

	template<typename Iter,
		typename Fn1>inline
		Fn1 for_earch(Iter first, Iter last, const Fn1& fn1)
	{
		for (; first != last; ++first)
			fn1(*first);
		return (fn1);
	}

	template<typename FIter,
		typename Fn0>inline
		void generate(FIter first, FIter last, const Fn0& fn0)
	{
		for (; first != last; ++first)
			*first = fn0();
	}

	template<typename OIter,
		typename Dif,
		typename Fn0>
		OIter generate_n(OIter first, Dif n, const Fn0&fn0)
	{
		for (; n != 0; --n)
			*first++ = fn0();
		return (first);
	}

	template<typename IIter1,
		typename IIter2,
		typename Comp>inline
		bool includes(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2, const Comp&cmp)
	{
		if (first1 != last1&&first2 != last2)
		{
			for (;;)
			{
				if (cmp(*first2, *first1))
					return (false);
				else if (cmp(*first1, *first2))
				{
					if (++first1 == last1)
						break;
				}
				else
				{
					++first1;
					++first2;
					if (first1 == last1
						|| first2 == last2)
						break;
				}
			}
		}
		return (first2 == last2);
	}

	template<typename IIter1,
		typename IIter2>inline
		bool includes(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2)
	{
		return (_QMJ includes(first1, last1,
			first2, last2, std::less<>()));
	}

	template<typename FIter,
		typename Comp>inline
		FIter max_element(FIter first, FIter last,
			const Comp&cmp)
	{
		FIter result = first;
		if (first != last)
		{
			for (iter_val_t<FIter>val = *first; ++first != last;)
				if (cmp(val, *first))
				{
					val = *first;
					result = first;
				}
		}
		return (result);
	}

	template<typename FIter>inline
		FIter max_element(FIter first, FIter last)
	{
		return (_QMJ max_element(first, last, std::less<>()));
	}

	template<typename FIter,
		typename Comp>inline
		FIter min_element(FIter first, FIter last,
			const Comp&cmp)
	{
		FIter result = first;
		if (first != last)
		{
			for (iter_val_t<FIter>val = *first; ++first != last;)
				if (cmp(*first, val))
				{
					val = *first;
					result = first;
				}
		}
		return (result);
	}

	template<typename FIter>inline
		FIter min_element(FIter first, FIter last)
	{
		return (_QMJ min_element(first, last, std::less<>()));
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter,
		typename Comp>inline
		OIter merge(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2, OIter dest,
			const Comp&cmp)
	{
		if (first1 != last1&&first2 != last2)
		{
			for (;;)
			{
				if (cmp(*first2, *first1))
				{
					*dest++ = *first2;
					if (++first2 == last2)
						break;
				}
				else
				{
					*dest++ = *first1;
					if (++first1 == last1)
						break;
				}
			}
		}
		dest = std::copy(first2, last2, dest);
		return (std::copy(first1, last1, dest));
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter>inline
		OIter merge(IIter1 first1, IIter1 last1,
			IIter2 first2, IIter2 last2, OIter dest)
	{
		return (_QMJ merge(first1, last1, first2, last2, dest, std::less<>()));
	}

	template<typename Iter,
		typename value_type>inline
		Iter remove(Iter first, Iter last, const value_type&val)
	{
		first = _QMJ find(first, last, val);
		if (first != last)
		{
			for (Iter next = first; ++next != last;)
				if (*next != val)
					*first++ = std::move(*next);
		}
		return (first);
	}

	template<typename Iter,
		typename OIter,
		typename value_type>inline
		OIter remove_copy(Iter first, Iter last, OIter dest, const value_type&val)
	{
		for (; first != last; ++first)
			if (*first != val)
				*dest++ = *first;
		return (dest);
	}

	template<typename Iter,
		typename Pred>inline
		Iter remove_if(Iter first, Iter last, const Pred&pred)
	{
		first = _QMJ find(first, last, pred);
		if (first != last)
		{
			for (Iter next = first; ++next != last;)
				if (!pred(*next))
					*first++ = std::move(*next);
		}
		return (first);
	}

	template<typename IIter,
		typename OIter,
		typename Pred>inline
		OIter remove_copy_if(IIter first, IIter last,
			OIter dest, const Pred&pred)
	{
		for (; first != last; ++first)
			if (!pred(*first))
				*dest++ = std::move(*first);
		return (dest);
	}

	template<typename FIter,
		typename value_type>inline
		void replace(FIter first, FIter last,
			const value_type&old_val, const value_type&new_val)
	{
		for (; first != last; ++first)
			if (*first == old_val)
				*first = new_val;
	}

	template<typename FIter,
		typename OIter,
		typename value_type>inline
		OIter replace_copy(FIter first, FIter last,
			OIter dest, const value_type&old_val,
			const value_type&new_val)
	{
		for (; first != last; ++first, ++dest)
			*dest = *first == old_val ? new_val : *first;
		return (dest);
	}

	template<typename FIter,
		typename Pred,
		typename value_type>inline
		void replace_if(FIter first, FIter last,
			const Pred&pred, const value_type&val)
	{
		for (; first != last; ++first)
			if (pred(*first))
				*first = val;
	}

	template<typename IIter,
		typename OIter,
		typename Pred,
		typename value_type>inline
		OIter replace_copy_if(IIter first, IIter last,
			OIter dest, const Pred&pred, const value_type&val)
	{
		for (; first != last; ++first, ++dest)
			*dest = pred(*first) ? val : *first;
		return (dest);
	}

	template<typename BIter>inline
		void reverse(BIter first, BIter last)
	{
		for (; first != last&&first != --last; ++first)
			_QMJ iter_swap(first, last);
	}

	template<typename BIter,
		typename OIter>inline
		OIter reverse_copy(BIter first, BIter last, OIter dest)
	{
		for (; first != last;)
			*dest++ = *--last;
		return (dest);
	}

	template<typename FIter>inline
		void _rotate_imple(FIter first, FIter middle,
			FIter last, std::forward_iterator_tag)
	{
		for (FIter mid = middle;;)
		{
			_QMJ iter_swap(first++, mid++);
			if (first == middle)
			{
				if (mid == last)
					return;
				middle = mid;
			}
			else if (mid == last)
				mid = middle;
		}
	}

	template<typename BIter>inline
		void _rotate_imple(BIter first, BIter middle,
			BIter last, std::bidirectional_iterator_tag)
	{
		_QMJ reverse(first, middle);
		_QMJ reverse(middle, last);
		_QMJ reverse(first, last);
	}

	template<typename Iter>inline
		void rotate(Iter first, Iter middle, Iter last)
	{
		_QMJ _rotate_imple(first, middle, last,
			iter_cate_t<Iter>());
	}

	template<typename FIter,
		typename OIter>inline
		void rotate_copy(FIter first, FIter middle,
			FIter last, OIter dest)
	{
		dest = std::copy(middle, last, dest);
		return (std::copy(first, middle, dest));
	}

	template<typename FIter1,
		typename FIter2>inline
		FIter2 swap_ranges(FIter1 first1,
			FIter1 last1, FIter2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
			_QMJ iter_swap(first1, first2);
		return (first2);
	}

	template<typename IIter,
		typename OIter,
		typename Fn1>inline
		OIter transform(IIter first, IIter last,
			OIter dest, const Fn1&fn1)
	{
		for (; first != last; ++first, ++dest)
			*dest = fn1(*first);
		return (dest);
	}

	template<typename IIter1,
		typename IIter2,
		typename OIter,
		typename Fn2>inline
		OIter transform(IIter1 first1, IIter1 last1,
			IIter2 first2, OIter dest, const Fn2&fn2)
	{
		for (; first1 != last1; ++first1, ++first2, ++dest)
			*dest = fn2(*first1, *first2);
		return (dest);
	}

	template<typename FIter,
		typename OIter,
		typename Pred>inline
		OIter unique_copy(FIter first, FIter last,
			OIter dest, const Pred&pred)
	{
		if (first != last)
		{
			iter_val_t<FIter> val = *first;
			for (++first; first != last; ++first)
				if (!pred(*first, val))
				{
					*dest++ = std::move(val);
					val = *first;
				}
			*dest++ = std::move(val);
		}
		return (dest);
	}

	template<typename FIter,
		typename OIter>inline
		OIter unique_copy(FIter first, FIter last,
			OIter dest)
	{
		return (_QMJ unique_copy(first, last,
			dest, std::equal_to<>()));
	}

	template<typename FIter,
		typename Pred>inline
		FIter unique(FIter first, FIter last,
			const Pred&pred)
	{
		first = _QMJ adjacent_find(first, last, pred);
		if (first != last)
		{
			FIter next = first;
			for (++next; ++next != last;)
				if (!pred(*first, *next))
					*++first = std::move(*next);
			++first;
		}
		return (first);
	}

	template<typename FIter>inline
		FIter unique(FIter first, FIter last)
	{
		return (_QMJ unique(first, last, std::equal_to<>()));
	}

	template<typename FIter,
		typename value_type,
		typename Comp>inline
		FIter lower_bound(FIter first, FIter last,
			const value_type&val, const Comp&cmp)
	{
		iter_dif_t<FIter>len = _QMJ distance(first, last);
		iter_dif_t<FIter>half;
		FIter mid;
		while (len > 0)
		{
			half = (size_t)len >> 1;
			mid = first;
			_QMJ advance(mid, half);
			if (cmp(*mid, val))
			{
				first = ++mid;
				len = len - half - 1;
			}
			else
				len = half;
		}
		return (first);
	}

	template<typename Iter,
		typename value_type>inline
		Iter lower_bound(Iter first, Iter last,
			const value_type&val)
	{
		return (_QMJ lower_bound(first, last, val,
			std::less<>()));
	}

	template<typename FIter,
		typename value_type,
		typename Comp>inline
		FIter upper_bound(FIter first, FIter last,
			const value_type&val, const Comp&cmp)
	{
		iter_dif_t<FIter>len = _QMJ distance(first, last);
		iter_dif_t<FIter>half;
		FIter mid;
		while (len > 0)
		{
			half = (size_t)len >> 1;
			mid = first;
			_QMJ advance(mid, half);
			if (cmp(val, *mid))
				len = half;
			else
			{
				first = ++mid;
				len = len - half - 1;
			}
		}
		return (first);
	}

	template<typename FIter,
		typename value_type>inline
		FIter upper_bound(FIter first, FIter last,
			const value_type&val)
	{
		return (_QMJ upper_bound(first, last, val, std::less<>()));
	}

	template<typename FIter,
		typename value_type,
		typename Comp>inline
		bool binary_search(FIter first, FIter last,
			const value_type&val, const Comp&cmp)
	{
		FIter result = lower_bound(first, last, val, cmp);//*result>=val if result!=last
		return (result != last && (!cmp(val, *first)));
	}

	template<typename FIter,
		typename value_type>inline
		bool binary_search(FIter first, FIter last,
			const value_type&val)
	{
		return (_QMJ binary_search(first, last, val, std::less<>()));
	}

	template<typename BIter,
		typename Comp>inline
		bool next_permutation(BIter first, BIter last,
			const Comp&cmp)
	{
		BIter before = last;
		if (first == last || --before == first)
			return (false);

		for (BIter next = before;; next = before)
		{
			if (cmp(*--before, *next))
			{
				BIter instead = last;
				for (--instead; !cmp(*before, *instead); --instead)
					;
				_QMJ iter_swap(instead, before);
				_QMJ reverse(next, last);
				return (true);
			}
			if (before == first)
			{
				_QMJ reverse(first, last);
				return (false);
			}
		}
	}

	template<typename BIter>inline
		bool next_permutation(BIter first, BIter last)
	{
		return (_QMJ next_permutation(first, last,
			std::less<>()));
	}

	template<typename BIter,
		typename Comp>inline
		bool prev_permutation(BIter first, BIter last,
			const Comp&cmp)
	{
		BIter before = last;
		if (before == first || --before == first)
			return (false);

		for (BIter next = before;; next = before)
		{
			if (cmp(*next, *--before))
			{
				BIter instead = last;
				for (--instead; !cmp(*instead, *before); --instead)
					;
				_QMJ iter_swap(instead, before);
				_QMJ reverse(next, last);
				return (true);
			}
			if (before == first)
			{
				_QMJ reverse(first, last);
				return (false);
			}
		}
	}

	template<typename BIter>inline
		bool prev_permutation(BIter first, BIter last)
	{
		return (_QMJ prev_permutation(first, last,
			std::less<>()));
	}

	template<typename FIter1,
		typename FIter2,
		typename Pred>inline
		void _match_suffixes(FIter1& last1, FIter2& last2, const Pred&pred,
			std::forward_iterator_tag, std::forward_iterator_tag)
	{
	}

	template<typename BIter1,
		typename BIter2,
		typename Pred>inline
		void _match_suffixes(BIter1& last1, BIter2& last2, const Pred&pred,
			std::bidirectional_iterator_tag, std::bidirectional_iterator_tag)
	{
		for (; pred(*--last1, *--last2);)
			;
		++last1;
		++last2;
	}

	template<typename Iter,
		typename value_type,
		typename Pred>inline
		Iter _find_fn2(Iter first, Iter last, const value_type&val,
			const Pred&pred)
	{
		for (; first != last; ++first)
			if (pred(*first, val))
				break;
		return (first);
	}

	template<typename Iter,
		typename value_type,
		typename Pred>inline
		iter_dif_t<Iter> _count_fn2(Iter first, Iter last,
			const value_type&val, const Pred&pred)
	{
		iter_dif_t<Iter> counter = 0;
		for (; first != last; ++first)
			if (pred(*first, val))
				++counter;
		return (counter);
	}

	template<typename Iter1,
		typename Iter2,
		typename Pred>inline
		bool _match_count(Iter1 first1, Iter1 last1,
			Iter2 first2, Iter2 last2, const Pred&pred)
	{
		_QMJ _match_suffixes(last1, last2, pred,
			iter_cate_t<Iter1>(), iter_cate_t<Iter2>());
		Iter1 skip;
		for (Iter1 cur = first1; cur != last1; ++cur)
			if (_QMJ _find_fn2(first1, cur, *cur, pred) == cur)
			{
				iter_dif_t<Iter2> count2 = _QMJ _count_fn2(first2, last2, *cur, pred);
				if (!count2)
					return (false);
				skip = cur;
				iter_dif_t<Iter1> count1 = _QMJ _count_fn2(++skip, last1, *cur, pred) + 1;
				if (count1 != count2)
					return (false);
			}
		return (true);
	}

	template<typename FIter1,
		typename FIter2,
		typename Pred>inline
		bool _is_permutation_imple(FIter1 first1, FIter1 last1,
			FIter2 first2, const Pred&pred)
	{
		for (; first1 != last1; ++first1, ++first2)
			if (!pred(*first1, *first2))
			{
				FIter2 last2 = first2;
				_QMJ advance(last2, _QMJ distance(first1, last1));
				return (_QMJ _match_count(first1, last1, first2, last2, pred));
			}
		return (true);
	}

	template<typename RIter1,
		typename RIter2,
		typename Pred>inline
		bool _is_permutation_imple(RIter1 first1, RIter1 last1,
			RIter2 first2, RIter2 last2, const Pred&pred,
			std::random_access_iterator_tag, std::random_access_iterator_tag)
	{
		if (last1 - first1 != last2 - first2)
			return (false);
		return (_QMJ _is_permutation_imple(first1, last1, first2, pred));
	}

	template<typename FIter1,
		typename FIter2,
		typename Pred>inline
		bool _is_permutation_imple(FIter1 first1, FIter1 last1,
			FIter2 first2, FIter2 last2, const Pred&pred,
			std::forward_iterator_tag, std::forward_iterator_tag)
	{
		for (; first1 != last1&&first2 != last2; ++first1, ++first2)
			if (!pred(*first1, *first2))
			{
				if (_QMJ distance(first1, last1) != _QMJ distance(first2, last2))
					return (false);
				else
					return (_QMJ _match_count(first1, last1, first2, last2, pred));
			}
		return (first1 == last1&&first2 == last2);
	}

	template<typename Iter1,
		typename Iter2,
		typename Pred>inline
		bool is_permutation(Iter1 first1, Iter1 last1,
			Iter2 first2, Iter2 last2, const Pred&pred)
	{
		return (_QMJ _is_permutation_imple(first1, last1,
			first2, last2, pred, iter_cate_t<Iter1>(), iter_cate_t<Iter2>()));
	}

	template<typename Iter1,
		typename Iter2>inline
		bool is_permutation(Iter1 first1, Iter1 last1,
			Iter2 first2, Iter2 last2)
	{
		return (_QMJ is_permutation(first1, last1,
			first2, last2, std::equal_to<>()));
	}

	template<typename Iter1,
		typename Iter2,
		typename Pred>inline
		bool is_permutation(Iter1 first1, Iter1 last1,
			Iter2 first2, const Pred&pred)
	{
		for (; first1 != last1; ++first1, ++first2)
			if (!pred(*first1, *first2))
			{
				Iter2 last2 = first2;
				_QMJ advance(last2, _QMJ distance(first1, last1));
				return (_match_count(first1, last1, first2, last2, pred));
			}
		return (true);
	}

	template<typename Iter1,
		typename Iter2>inline
		bool is_permutation(Iter1 first1, Iter1 last1,
			Iter2 first2)
	{
		return (_QMJ is_permutation(first1, last1,
			first2, std::equal_to<>()));
	}

	template<typename RIter>inline
		void push_heap(RIter first, RIter last)
	{//push *(last-1) int heap [first,last-1),using <
		_QMJ push_heap(first, last, std::less<>());
	}

	template<typename RIter, typename Comp>inline
		void push_heap(RIter first, RIter last, const Comp& cmp)
	{//push *(last-1) int heap [first,last-1),using cmp
		_QMJ _push_heap_imple(first,
			iter_dif_t<RIter>(last - first - 1),
			std::move(*(last - 1)), cmp);
	}

	template<typename RIter,
		typename Dif,
		typename value_type,
		typename Comp>inline
		void _push_heap_imple(RIter first, Dif index,
			value_type&& val, const Comp&cmp)
	{//percolate index to where val belong,using cmp
		for (Dif par_index = (size_t)(index - 1) >> 1;
			index&&cmp(*(first + par_index), val);
			par_index = (size_t)(index - 1) >> 1)
		{//move val up to parent
			*(first + index) = std::move(*(first + par_index));
			index = par_index;
		}
		*(first + index) = std::move(val);
	}

	template<typename RIter, typename Dif,
		typename value_type, typename Comp>inline
		void _heapify(RIter first,
			Dif index, const Dif len, value_type&&val,
			const Comp&cmp)
	{//percolate down,put val in a good node in heap of [first,first+len),using cmp
		Dif right_index = (index << 1) + 2;//right node of index
		for (; right_index < len;)
		{
			if (cmp(*(first + right_index), *(first + (right_index - 1))))
				--right_index;//left node of index
			if (cmp(val, *(first + right_index)))
				*(first + index) = std::move(*(first + right_index));
			else
			{//where val belong
				*(first + index) = std::move(val);
				return;
			}
			index = right_index;
			right_index = (index << 1) + 2;
		}
		if (right_index == len
			&&cmp(val, *(first + (--right_index))))
		{//index have a left node and satisfy cmp
			*(first + index) = *(first + right_index);
			index = right_index;
		}
		*(first + index) = std::move(val);
	}

	template<typename RIter, typename Comp>inline
		void _pop_heap_imple(RIter first, RIter last, const Comp&cmp)
	{
		iter_val_t<RIter> val = std::move(*--last);
		*last = std::move(*first);
		_QMJ _heapify(first, iter_dif_t<RIter>(0),
			iter_dif_t<RIter>(last - first),
			std::move(val), cmp);
	}

	template<typename RIter, typename Comp>inline
		void pop_heap(RIter first, RIter last, const Comp&cmp)
	{//pop *first to *(last - 1) and reheap, using cmp
		iter_dif_t<RIter> dif = last - first;
		if (dif >= 2)
			_QMJ _pop_heap_imple(first, last, cmp);
	}

	template<typename RIter>inline
		void pop_heap(RIter first, RIter last)
	{//pop *first to *(last - 1) and reheap, using <
		_QMJ pop_heap(first, last, std::less<>());
	}

	template<typename RIter>inline
		void sort_heap(RIter first, RIter last)
	{//sort the heap of [first,last) ,using <
		_QMJ sort_heap(first, last, std::less<>());
	}

	template<typename RIter, typename Comp>inline
		void sort_heap(RIter first, RIter last, const Comp&cmp)
	{//sort the heap of [first,last) ,using cmp
		for (; last - first >= 2; --last)
			_QMJ _pop_heap_imple(first, last, cmp);
	}

	template<typename RIter,
		typename Dif,
		typename Comp>inline
		void _make_heap_imple(RIter first, Dif len, const Comp&cmp)
	{
		iter_val_t<RIter>val;
		for (Dif index = (size_t)(len - 1) >> 1;
			index >= 0; --index)
		{
			val = std::move(*(first + index));
			_QMJ _heapify(first, index, len, std::move(val), cmp);
		}
	}

	template<typename RIter, typename Comp>inline
		void make_heap(RIter first, RIter last, const Comp&cmp)
	{//make heap in [first,last),using cmp
		const iter_dif_t<RIter> len = last - first;
		if (len >= 2)
			_QMJ _make_heap_imple(first, len, cmp);
	}

	template<typename RIter>
	void make_heap(RIter first, RIter last)
	{//make heap in [firs,last),using <
		_QMJ make_heap(first, last, std::less<>());
	}

	template<typename RIter,
		typename Comp>inline
		void partial_sort(RIter first, RIter middle, RIter last,
			const Comp&cmp)
	{
		iter_dif_t<RIter> len = middle - first;
		if (!len)
			return;
		else if (len == 1)
		{
			_QMJ iter_swap(first, _QMJ min_element(first, last, cmp));
			return;
		}
		_QMJ make_heap(first, middle,cmp);
		for (RIter cur = middle; cur != last; ++cur)
			if (cmp(*cur, *first))
			{
				iter_val_t<RIter>val = std::move(*cur);
				*cur = std::move(*first);
				_QMJ _heapify(first, iter_dif_t<RIter>(0), len, std::move(val), cmp);
			}
		_QMJ sort_heap(first, middle,cmp);
	}

	template<typename RIter>inline
		void partial_sort(RIter first, RIter middle, RIter last)
	{
		_QMJ partial_sort(first, middle, last, std::less<>());
	}

	template<typename IIter,
		typename RIter,
		typename Comp>inline
		RIter partial_sort_copy(IIter first, IIter last,
			RIter dest_first, RIter dest_last, const Comp&cmp)
	{
		RIter result = std::copy(first, last, dest_first);
		_QMJ partial_sort(dest_first, result, result, cmp);
		return (result);
	}

	template<typename IIter,
		typename RIter>inline
		RIter partial_sort_copy(IIter first, IIter last,
			RIter dest_first, RIter dest_last)
	{
		return (_QMJ partial_sort_copy(first, last,
			dest_first, dest_last, std::less<>()));
	}

	template<typename BIter,
		typename value_type,
		typename Comp>inline
		void _unguarded_linear_insert(BIter last, value_type val,
			const Comp&cmp)
	{
		for (BIter prev = _QMJ prev(last); cmp(val, *prev); last = prev, --prev)
			*last = std::move(*prev);
		*last = std::move(val);
	}

	template<typename BIter,
		typename Comp>inline
		void _unguarded_insert_sort(BIter first, BIter last,
			const Comp&cmp)
	{
		for (BIter i = first; i != last; ++i)
			_unguarded_linear_insert(i, std::move(*i), cmp);
	}

	template<typename BIter,
		typename Comp>inline
		void _linear_insert(BIter first, BIter last,
			const Comp&cmp)
	{
		iter_val_t<BIter> val = std::move(*last);
		if (cmp(val, *first))
		{
			std::copy_backward(first, last, last + 1);
			*first = std::move(val);
		}
		else
			_unguarded_linear_insert(last, std::move(val), cmp);
	}

	template<typename BIter,
		typename Comp>inline
		void _insert_sort_imple(BIter first, BIter last, const Comp&cmp)
	{
		for (BIter cur = first; ++cur != last;)
			_linear_insert(first, cur, cmp);
	}

	template<typename BIter,
		typename Comp>inline
		void _insert_sort(BIter first, BIter last, const Comp&cmp)
	{
		if (last == first)return;
		_QMJ _insert_sort_imple(first, last, cmp);
	}

	template<typename value_type,
		typename Comp>inline
		const value_type& _get_piovt(const value_type&a,
			const value_type&b, const value_type&c,
			const Comp&cmp)
	{
		if (cmp(a, b))
		{
			if (cmp(b, c))
				return (b);
			else if (cmp(a, c))
				return (c);
			else
				return (a);
		}
		else if (cmp(a, c))
			return (a);
		else if (cmp(b, c))
			return (c);
		else
			return (b);
	}

	template<typename RIter,
		typename Comp>inline
		RIter _mid_partition(RIter first, RIter last,
			const Comp&cmp)
	{
		iter_val_t<RIter> piovt = _get_piovt(
			*first, *(first + ((size_t)(last - first) >> 1)), *(last - 1), cmp);
		for (;; ++first)
		{
			for (; cmp(*first, piovt); ++first)
				;

			for (--last; cmp(piovt, *last); --last)
				;

			if (!(first < last))
				return (first);
			_QMJ iter_swap(first, last);
		}
		return (first);
	}

	template<typename RIter,
		typename Comp>inline
		void _introsort_imple(RIter first, RIter last, size_t depth_limit,
			const Comp&cmp)
	{
		for (; last - first > sort_threshold;)
		{
			if (depth_limit-- == 0)
			{
				_QMJ _make_heap_imple(first, iter_dif_t<RIter>(last - first), cmp);
				_QMJ sort_heap(first, last, cmp);
				return;
			}

			RIter cut = _mid_partition(first, last, cmp);

			if (cut - first > last - cut)
			{
				_introsort_imple(cut, last, depth_limit, cmp);
				last = cut;
			}
			else
			{
				_introsort_imple(first, cut, depth_limit, cmp);
				first = cut;
			}
		}
	}

	template<typename RIter,
		typename Comp>inline
		void _final_insert_sort(RIter first, RIter last,
			const Comp&cmp)
	{
		if (last - first > sort_threshold)
		{
			_insert_sort(first, first + sort_threshold, cmp);
			_unguarded_insert_sort(first + sort_threshold, last, cmp);
		}
		else
			_insert_sort(first, last, cmp);
	}

	inline size_t _lg2(size_t n)
	{
		size_t k = 0;
		for (; n > 1; n >>= 1)
			++k;
		return (k);
	}

	template<typename RIter,
		typename Comp>inline
		void sort(RIter first, RIter last, const Comp&cmp)
	{
		if (last - first > sort_threshold)
			_introsort_imple(first, last, size_t(_lg2((size_t)(last - first))*1.5), cmp);
		_final_insert_sort(first, last, cmp);
	}

	template<typename RIter>inline
		void sort(RIter first, RIter last)
	{
		_QMJ sort(first, last, std::less<>());
	}

	template<typename RIter,
		typename value_type,
		typename Comp>inline
		std::pair<RIter, RIter> equal_range(RIter first, RIter last,
			const value_type&val, const Comp&cmp)
	{
		iter_dif_t<RIter> len = last - first;
		iter_dif_t<RIter>half;
		RIter middle;
		while (len > 0)
		{
			half = (size_t)len >> 1;
			middle = first + half;
			if (cmp(*middle, val))
			{
				first = middle + 1;
				len = len - half - 1;
			}
			else if (cmp(val, *middle))
				len = half;
			else
			{
				RIter left = _QMJ lower_bound(first, middle, val, cmp);
				RIter right = _QMJ upper_bound(++middle, first + len, val, cmp);
				return (std::pair<RIter, RIter>(left, right));
			}
		}
		return (std::pair<RIter, RIter>(first, last));
	}

	template<typename RIter,
		typename value_type>inline
		std::pair<RIter, RIter> equal_range(RIter first, RIter last,
			const value_type&val)
	{
		return (_QMJ equal_range(first, last, val, std::less<>()));
	}

	template<typename value_type,typename Alloc= _QMJ allocator<value_type>>
	class temporary_buffer
	{
	public:
		typedef Alloc alloc;
		typedef value_type* pointer;
		temporary_buffer(size_t len) :
			len(len)
		{
			allocate_construct();
		}

		~temporary_buffer()
		{
			alloc::destroy(first, first + len);
			alloc::deallocate(first);
		}

		size_t size()const
		{
			return (len);
		}

		pointer begin()
		{
			return (first);
		}

		pointer end()
		{
			return (first + len);
		}

	private:
		void allocate_construct()
		{
			while (len)
			{
				try
				{
					first = alloc::allocate(len);
					alloc::construct(first, len);
					return;
				}
				catch (_BAD_ALLOC)
				{
					len >>= 1;
				}
				catch (...)
				{
					throw;
				}
			}
		}

	private:
		size_t len;
		pointer first;
	};

	template<typename BIter1,
		typename BIter2,
		typename Dif>inline
		BIter1 _rotate_adaptive(BIter1 first, BIter1 middle, BIter1 last,
			Dif len1, Dif len2, BIter2 buf, Dif buf_size)
	{
		BIter2 buf_end;
		if (len1 > len2&&len2 <= buf_size)
		{
			buf_end = copy(middle, last, buf);
			std::copy_backward(first, middle, last);
			return (std::copy(buf, buf_end, first));
		}
		else if (len1 <= buf_size)
		{
			buf_end = std::copy(first, middle, buf);
			std::copy(middle, last, first);
			return (std::copy_backward(buf, buf_end, last));
		}
		else
		{
			_QMJ rotate(first, middle, last);
			_QMJ advance(first, len2);
			return (first);
		}
	}

	template<typename BIter1,
		typename BIter2,
		typename OIter,
		typename Comp>inline
		void _merge_backward(BIter1 first1, BIter1 last1,
			BIter2 first2, BIter2 last2, OIter dest, const Comp&cmp)
	{
		if (first1 != last1&&first2 != last2)
		{
			for (--last1, --last2;;)
			{
				if (cmp(*last2, *last1))
				{
					*--dest = *last1;
					if (last1 == first1)
						break;
					--last1;
				}
				else
				{
					*--dest = *last2;
					if (last2 == first2)
						break;
					--last2;
				}
			}
		}
		std::copy_backward(first2, last2, dest);
		dest = std::copy_backward(first1, last1, dest);
	}

	template<typename BIter,
		typename Dif,
		typename pointer,
		typename Comp>inline
		void _merge_adaptive(BIter first, BIter middle, BIter last,
			Dif len1, Dif len2, pointer buf, Dif buf_size, const Comp&cmp)
	{
		if (len1 <= len2&&len1 <= buf_size)
		{
			pointer end_buf = std::copy(first, middle, buf);
			_QMJ merge(buf, end_buf, middle, last, first, cmp);
		}
		else if (len2 <= buf_size)
		{
			pointer end_buf = std::copy(middle, last, buf);
			_QMJ _merge_backward(first, middle, buf, end_buf, last, cmp);
		}
		else
		{
			BIter first_cut = first;
			BIter second_cut = middle;
			Dif len11 = 0;
			Dif len22 = 0;
			if (len1 > len2)
			{
				len11 = (size_t)len2 >> 1;
				_QMJ advance(first_cut, len11);
				second_cut = _QMJ lower_bound(middle, last, *first_cut,cmp);
				len2 = _QMJ distance(middle, second_cut);
			}
			else
			{
				len22 = (size_t)len2 >> 1;
				_QMJ advance(second_cut, len22);
				first_cut = _QMJ upper_bound(first, middle, *second_cut,cmp);
				len11 = _QMJ distance(first, first_cut);
			}
			BIter new_middle = _QMJ _rotate_adaptive(first_cut, middle, second_cut,
				len1 - len11, len22, buf, buf_size);
			_QMJ _merge_adaptive(first, first_cut, new_middle,
				len11, len22, buf, buf_size, cmp);
			_QMJ _merge_adaptive(new_middle, second_cut, last,
				len1 - len11, len2 - len22, buf, buf_size, cmp);
		}
	}

	template<typename BIter,
		typename Comp>inline
		void _inplace_merge_imple(BIter first, BIter middle,
			BIter last, const Comp&cmp)
	{
		iter_dif_t<BIter> len1 = _QMJ distance(first, middle);
		iter_dif_t<BIter>len2 = _QMJ distance(middle, last);
		_QMJ temporary_buffer<iter_val_t<BIter>>buf((len1 < len2 ? len1 : len2));
		_QMJ _merge_adaptive(first, middle, last, len1, len2,
			buf.begin(), iter_dif_t<BIter>(buf.size()), cmp);
	}

	template<typename BIter,
		typename Comp>inline
		void inplace_merge(BIter first, BIter middle,
			BIter last, const Comp&cmp)
	{
		if (first == middle || middle == last)
			return;
		_QMJ _inplace_merge_imple(first, middle, last, cmp);
	}

	template<typename BIter>inline
		void inplace_merge(BIter first, BIter middle,
			BIter last)
	{
		_QMJ inplace_merge(first, middle, last, std::less<>());
	}

	template<typename BIter,
		typename Dif,
		typename pointer,
		typename Comp>inline
		void _stable_sort_imple(BIter first, BIter last,
			pointer buf, Dif buf_size, const Comp&cmp)
	{
		iter_dif_t<BIter> len = _QMJ distance(first, last);
		if (len > sort_threshold)
		{
			Dif len1 = (size_t)len >> 1;
			Dif len2 = len - len1;
			BIter middle = first;
			_QMJ advance(middle, len1);
			_QMJ _stable_sort_imple(first, middle, buf, buf_size, cmp);
			_QMJ _stable_sort_imple(middle, last, buf, buf_size, cmp);
			_QMJ _merge_adaptive(first, middle, last, len1, len2,
				buf, buf_size, cmp);
		}
		else
			_insert_sort(first, last, cmp);
	}

	template<typename BIter,
		typename Comp>inline
		void stable_sort(BIter first, BIter last, const Comp&cmp)
	{
		iter_dif_t<BIter>len = _QMJ distance(first, last);
		if (1 < len)
		{
			iter_dif_t<BIter> len1 = (size_t)(len >> 1);
			iter_dif_t<BIter>len2 = len - len1;
			_QMJ temporary_buffer<iter_val_t<BIter>>buf((len1 < len2 ? len1 : len2));
			_QMJ _stable_sort_imple(first, last, buf.begin(),
				iter_dif_t<BIter>(buf.size()), cmp);
		}
		else
			_insert_sort(first, last, cmp);
	}

	template<typename BIter>inline
		void stable_sort(BIter first, BIter last)
	{
		_QMJ stable_sort(first, last, std::less<>());
	}

	template<typename RIter,
		typename Comp>inline
		void nth_element(RIter first, RIter nth, RIter last,
			const Comp&cmp)
	{
		RIter cut;
		while (last - first > 3)
		{
			cut = _QMJ _mid_partition(first, last, cmp);
			if (cut <= nth)
				first = cut;
			else
				last = cut;
		}
		_QMJ _insert_sort(first, last, cmp);
	}

	template<typename RIter>inline
		void nth_element(RIter first, RIter nth, RIter last)
	{
		_QMJ nth_element(first, nth, last, std::less<>());
	}

}

#endif
