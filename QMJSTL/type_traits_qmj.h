#pragma once
#ifndef _TYPE_TRAITS_QMJ_
#define _TYPE_TRAITS_QMJ_
#include <utility>
#if !defined _QMJ
#define _QMJ qmj::
#endif

using std::ptrdiff_t;

namespace qmj
{
template <typename val_type, val_type val>
struct integral_constant
{
	static constexpr val_type value = val;

	typedef val_type value_type;
	typedef integral_constant<value_type, val> type;

	constexpr operator value_type() const noexcept { return (value); }

	constexpr value_type operator()() const noexcept { return (value); }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <bool val>
using bool_constant = integral_constant<bool, val>;

template <bool val>
struct bool_type : bool_constant<val>
{
};

template <typename...>
struct param_tester
{
	typedef void type;
	typedef true_type is_typedef;
};

template <typename... types>
using void_t = typename param_tester<types...>::type;

template <typename... types>
using typedef_t = typename param_tester<types...>::is_typedef;

template <bool val, typename = void>
struct enable_if
{
};

template <typename type_>
struct enable_if<true, type_>
{
	typedef type_ type;
};

template <bool test, typename type = void>
using enable_if_t = typename enable_if<test, type>::type;

template <typename type>
struct type_traits
{
	typedef false_type has_trivial_default_constructor;
	typedef false_type has_trivial_copy_constructor;
	typedef false_type has_trivial_assignment_operator;
	typedef false_type has_trivial_destructor;
	typedef false_type is_POD_type;
};

template <>
struct type_traits<char>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<signed char>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<unsigned char>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<wchar_t>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<short>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<unsigned short>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<int>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<unsigned int>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<long>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<unsigned long>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<float>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<double>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <>
struct type_traits<long double>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <typename type>
struct type_traits<type *>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template <typename type>
struct type_traits<const type *>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};
}

namespace qmj
{
template <typename, typename = void>
struct iterator_traits_base
{
};

template <typename Iter>
struct iterator_traits_base<
	Iter, void_t<typename Iter::iterator_category, typename Iter::value_type,
				 typename Iter::difference_type, typename Iter::pointer,
				 typename Iter::reference>>
{
	typedef typename Iter::iterator_category iterator_category;
	typedef typename Iter::value_type value_type;
	typedef typename Iter::difference_type difference_type;
	typedef typename Iter::pointer pointer;
	typedef typename Iter::difference_type reference;
	typedef false_type memory_copy_tag;
};

template <typename Iter>
struct iterator_traits : iterator_traits_base<Iter>
{
	typedef false_type memory_copy_tag;
};

template <typename type>
struct iterator_traits<type *>
{
	typedef std::random_access_iterator_tag iterator_category;
	typedef type value_type;
	typedef ptrdiff_t difference_type;
	typedef type *pointer;
	typedef type &reference;
	typedef true_type memory_copy_tag;
};

template <typename type>
struct iterator_traits<const type *>
{
	typedef std::random_access_iterator_tag iterator_category;
	typedef type value_type;
	typedef ptrdiff_t difference_type;
	typedef const type *pointer;
	typedef const type &reference;
	typedef true_type memory_copy_tag;
};

template <typename Iter>
using iter_val_t = typename iterator_traits<Iter>::value_type;

template <typename Iter>
using iter_dif_t = typename iterator_traits<Iter>::difference_type;

template <typename Iter>
using iter_cate_t = typename iterator_traits<Iter>::iterator_category;

template <typename Iter, typename = void>
struct is_iterator : false_type
{
};

template <typename Iter>
struct is_iterator<Iter,
				   void_t<typename iterator_traits<Iter>::iterator_category>>
	: true_type
{
};

template <typename type1, typename type2>
struct is_same : false_type
{
};

template <typename type>
struct is_same<type, type> : true_type
{
};

template <bool, typename type1, typename type2>
struct If
{
	typedef type2 type;
};

template <typename type1, typename type2>
struct If<true, type1, type2>
{
	typedef type1 type;
};

template <typename value_type, typename = void>
struct is_typedef_pod : false_type
{
	typedef false_type type;
};

template <typename value_type>
struct is_typedef_pod<value_type, void_t<typename value_type::is_POD_tag>>
	: true_type
{
	//		typedef typename value_type::is_POD_tag type;
};

template <typename value_type, typename = void>
struct is_pod : bool_type<type_traits<value_type>::is_POD_type::value ||
						  If<is_typedef_pod<value_type>::value,
							 typename is_typedef_pod<value_type>::type,
							 false_type>::type::value>
{
};

template <typename Iter, typename = void>
struct is_typedef_mem_copy : false_type
{
	typedef false_type type;
};

template <typename Iter>
struct is_typedef_mem_copy<Iter, void_t<typename Iter::memory_copy_tag>>
	: true_type
{
	typedef typename Iter::memory_copy_tag type;
};

template <typename Iter, typename = void>
struct is_mem_copy : bool_type<iterator_traits<Iter>::memory_copy_tag::value ||
							   If<is_typedef_mem_copy<Iter>::value,
								  typename is_typedef_mem_copy<Iter>::type,
								  false_type>::type::value>
{
};
}

#endif
