#pragma once
#ifndef _ALLOCATOR_
#define _ALLOCATOR_
#if !defined THROW_BAD_ALLOC
#define THROW_BAD_ALLOC throw bad_alloc()
#endif
#if !defined _BAD_ALLOC
#define _BAD_ALLOC bad_alloc
#endif
#include<new>
#include<cstdlib>
#include"iterator_qmj.h"
#include"type_traits_qmj.h"

//**************内存池/内存分配器***********************
//*********照搬自<<STL源码剖析>>书中SGI某一版本的实现
namespace qmj
{
	template<int inst>
	class malloc_alloc_template
	{//一级分配器，直接使用malloc/free分配和释放内存
	public://并模拟标准的内存不足处理函数
		static void *allocate(size_t n)
		{
			void *result = malloc(n);
			if (result == nullptr)
				result = oom_malloc(n);
			return (result);
		}

		static void deallocate(void*p,size_t)
		{
			free(p);
		}

		static void *reallocate(void *p,size_t, size_t new_size)
		{
			void* result = realloc(p, new_size);
			if (result == nullptr)
				result = oom_realloc(p, new_size);
			return (result);
		}

		static void(*set_malloc_handler(void(*f)()))()
		{
			void(*old)() = malloc_alloc_oom_handler;
			malloc_alloc_oom_handler = f;
			return (old);
		}
	private:
		static void *oom_malloc(size_t);
		static void *omm_realloc(void*, size_t);
		static void(*malloc_alloc_oom_handler)();
	};

	template<int inst>
	void(*malloc_alloc_template<inst>::malloc_alloc_oom_handler)() = nullptr;

	template<int inst>
	void* malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();
		void *result;
		for (;;)
		{
			my_malloc_handler = malloc_alloc_oom_handler;
			if (my_malloc_handler == nullptr)
				THROW_BAD_ALLOC;
			(*my_malloc_handler)();
			result = malloc(n);
			if (result)
				return (result);
		}
	}

	template<int inst>
	void* malloc_alloc_template<inst>::omm_realloc(void *p, size_t n)
	{
		void(*my_malloc_handler)();
		void* result;
		for (;;)
		{
			my_malloc_handler = malloc_alloc_oom_handler;
			if (my_malloc_handler == nullptr)
				THROW_BAD_ALLOC;
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result)
				return (result);
		}
	}

	typedef malloc_alloc_template<0> malloc_alloc;

	enum
	{
		align = 8
	};
	enum
	{
		max_bytes = 128
	};
	enum
	{
		nfreelists = max_bytes / align
	};

	template<bool threads, int inst>
	class default_alloc_template
	{//二级分配器，基于内存池的实现
	private:
		static size_t round_up(size_t bytes)
		{
			return (((bytes)+align - 1)&(~(align - 1)));
		}

	private:
		union obj
		{
			union obj* free_list_link;
			char client_data[1];
		};
	private:
		static obj*volatile free_list[nfreelists];

		static size_t free_list_index(size_t bytes)
		{
			return (((bytes)+align - 1) / align - 1);
		}

		static void*refill(size_t n);
		static char*chunk_alloc(size_t size, size_t&nobjs);
	public:
		static void* allocate(size_t n)
		{
			if (n > (size_t)max_bytes)
			{
				return (malloc_alloc::allocate(n));
			}
			obj* volatile *my_free_list = free_list + free_list_index(n);
			obj* result = *my_free_list;
			if (result == nullptr)
			{
				void*r = refill(round_up(n));
				return (r);
			}
			*my_free_list = result->free_list_link;
			return (result);
		}

		static void deallocate(void*p, size_t n)
		{
			if (n > (size_t)max_bytes)
			{
				malloc_alloc::deallocate(p,n);
				return;
			}
			obj*q = (obj*)p;
			obj*volatile*my_free_list = free_list + free_list_index(n);
			q->free_list_link = *my_free_list;
			*my_free_list = q;
		}
		//	static void* reallocate(void* p, size_t old_size, size_t new_size);
	private:
		static char* start_free;
		static char* end_free;
		static size_t heap_size;
	};

	template<bool threads, int inst>
	char* default_alloc_template<threads, inst>::start_free = nullptr;

	template<bool threads, int inst>
	char* default_alloc_template<threads, inst>::end_free = nullptr;

	template<bool threads, int inst>
	size_t default_alloc_template<threads, inst>::heap_size = 0;

	template<bool threads, int inst>
	typename default_alloc_template<threads, inst>::obj* volatile
		default_alloc_template<threads, inst>::free_list[nfreelists] =
	{
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,nullptr
	};

	template<bool threads, int inst>
	void* default_alloc_template<threads, inst>::refill(size_t n)
	{
		size_t nobjs = 20;
		char* chunk = chunk_alloc(n, nobjs);
		if (nobjs == 1)
			return (chunk);
		obj*volatile*my_free_list = free_list + free_list_index(n);
		obj*result = (obj*)chunk;
		obj*cur_obj, *next_obj;
		*my_free_list = next_obj = (obj*)(chunk + n);
		for (size_t i = 1;; ++i)
		{
			cur_obj = next_obj;
			if (nobjs - 1 == i)
			{
				cur_obj->free_list_link = nullptr;
				break;
			}
			else
			{
				next_obj = (obj*)((char*)next_obj + n);
				cur_obj->free_list_link = next_obj;
			}
		}
		return (result);
	}

	template<bool threads, int inst>
	char* default_alloc_template<threads, inst>::
		chunk_alloc(size_t size, size_t&nobjs)
	{
		char* result;
		size_t total_bytes = size*nobjs;
		size_t bytes_left = end_free - start_free;
		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else if (bytes_left >= size)
		{
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else
		{
			size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
			if (bytes_left > 0)
			{
				obj* volatile*my_free_list =
					free_list + free_list_index(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			start_free = (char*)malloc(bytes_to_get);
			if (start_free == nullptr)
			{
				obj *volatile*my_first_free, *p;
				for (size_t i = size; i <= max_bytes; i += align)
				{
					my_first_free = free_list + free_list_index(i);
					p = *my_first_free;
					if (p)
					{
						*my_first_free = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						return (chunk_alloc(size, nobjs));
					}
				}
				end_free = nullptr;
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return (chunk_alloc(size, nobjs));
		}
	}
}

namespace qmj
{
	typedef default_alloc_template<true, 0> default_alloc;

	template<typename value_type,typename allocator_type>
	class allocator_base
	{//分配器基类，根基allocator_type 决定调用哪一次分配器
	public:
		typedef value_type value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef allocator_type alloc;

		inline static pointer allocate()
		{
			return static_cast<pointer>(alloc::allocate(sizeof(value_type)));
		}

		inline static pointer allocate(const size_type n)
		{
			return (n ? static_cast<pointer>(alloc::allocate(sizeof(value_type)*n)) : nullptr);
		}

		template<typename ...types>inline
			static void construct(pointer ptr, types&&...args)
		{
			new (ptr) value_type(std::forward<types>(args)...);
		}

		inline static void construct(pointer ptr)
		{
			new (ptr) value_type();
		}

		inline static pointer construct(pointer first, size_type n)
		{
			for (; n != 0; --n, ++first)
				new (first) value_type();
			return first;
		}

		inline static pointer construct(pointer first, pointer last)
		{
			for (; first != last; ++first)
				new (first) value_type();
			return (first);
		}

		template<typename type>
		inline static void copy_construct(type* ptr, const type&val)
		{
			copy_construct_imple
			(ptr, val, qmj::type_traits<type>::has_trivial_copy_constructor());
		}

		inline static void copy_construct(pointer ptr, const value_type&val)
		{
			copy_construct_imple
			(ptr, val, qmj::type_traits<value_type>::has_trivial_copy_constructor());
		}

		inline static void copy_construct(pointer ptr, value_type&&val)
		{
			new (ptr) value_type(std::move(val));
		}

		inline static pointer copy_construct
		(pointer first, pointer last, const value_type&val)
		{
			for (; first != last; ++first)
				copy_construct(first, val);
			return (first);
		}

		template<typename Iter>inline
			static pointer copy_construct
			(Iter first, Iter last, pointer dest)
		{
			return copy_construct_imple
			(first, last, dest, qmj::type_traits<value_type>::has_trivial_copy_constructor());
		}

		template<typename Iter>inline
			static pointer copy_construct(Iter first, const size_type distance, pointer dest)
		{
			return copy_construct_imple
			(first, distance, dest, qmj::type_traits<value_type>::has_trivial_copy_constructor());
		}

		inline static pointer copy_construct(pointer first, size_type n, const value_type&val)
		{
			for (; n != 0; --n, ++first)
				copy_construct(first, val);
			return (first);
		}

		template<typename type>inline
			static void destroy(type* ptr)
		{
			destroy_imple(ptr, qmj::type_traits<type>::has_trivial_destructor());
		}

		inline static void destroy(pointer ptr)
		{
			destroy_imple(ptr, type_traits<value_type>::has_trivial_destructor());
		}

		inline static void destroy(pointer first, pointer last)
		{
			destroy(first, last, type_traits<value_type>::has_trivial_destructor());
		}

		inline static void deallocate(pointer ptr)
		{
			if (ptr)
				alloc::deallocate(ptr, sizeof(value_type));
		}

		inline static void deallocate(pointer ptr, size_t n)
		{
			if (ptr)
				alloc::deallocate(ptr, sizeof(value_type)*n);
		}

		inline static size_type max_size()
		{
			return ((size_t)(-1) / sizeof(value_type));
		}

	private:
		inline static void copy_construct_imple
		(pointer ptr, const value_type&val, true_type)
		{
			*ptr = val;
		}

		inline static void copy_construct_imple
		(pointer ptr, const value_type&val, false_type)
		{
			new (ptr) value_type(val);
		}

		template<typename type>inline
			static void copy_construct_imple(type* ptr, const type&val, true_type)
		{
			*ptr = val;
		}

		template<typename type>inline
			static void copy_construct_imple(type* ptr, const type&val, false_type)
		{
			new (ptr) type(val);
		}

		template<typename Iter>inline
			static pointer copy_construct_imple(Iter first, Iter last, pointer dest, false_type)
		{
			return copy_construct_imple_td(first, last, dest, qmj::iterator_traits<Iter>::iterator_category());
		}

		template<typename Iter>inline
			static pointer copy_construct_imple_td(Iter first, Iter last, pointer dest, std::input_iterator_tag)
		{
			while (first != last)
				new (dest++) value_type(*first++);
			return dest;
		}

		template<typename Iter>inline
			static pointer copy_construct_imple_td(Iter first, Iter last, pointer dest, std::random_access_iterator_tag)
		{
			for (size_t distance = last - first; distance != 0; --distance, ++first, ++dest)
				new (dest) value_type(*first);
			return dest;
		}

		template<typename Iter>inline
			static pointer copy_construct_imple(Iter first, Iter last, pointer dest, true_type)
		{
			return copy_construct_imple_nt_d
			(first, last, dest, qmj::iterator_traits<Iter>::iterator_category());
		}

		template<typename Iter>inline
			static pointer copy_construct_imple_nt_d(Iter first, Iter last, pointer dest, std::input_iterator_tag)
		{
			while (first != last)
				*dest++ = *first++;
			return dest;
		}

		template<typename Iter>inline
			static pointer copy_construct_imple_nt_d(Iter first, Iter last, pointer dest, std::random_access_iterator_tag)
		{
			return copy_construct_imple_nt_d_m
			(first, last, dest, qmj::is_mem_copy<Iter>());
		}

		template<typename Iter>inline
			static pointer copy_construct_imple_nt_d_m(Iter first, Iter last, pointer dest, false_type)
		{
			for (size_t distance = last - first; distance != 0; --distance, ++first, ++dest)
				*dest = *first;
			return dest;
		}

		template<typename Iter>inline
			static pointer copy_construct_imple_nt_d_m(Iter first, Iter last, pointer dest, true_type)
		{
			const size_type distance = last - first;
			std::memcpy(dest, &*first, sizeof(value_type)*distance);
			return (dest + distance);
		}

		template<typename Iter>inline
			static pointer copy_construct_imple
			(Iter first, const size_type distance, pointer dest, false_type)
		{
			for (; distance != 0; --distance, ++first, ++dest)
				new (dest) value_type(*first);
			return dest;
		}

		template<typename Iter>inline
			static pointer copy_construct_imple
			(Iter first, const size_type distance, pointer dest, true_type)
		{
			return copy_construct_imple_memory
			(dest, first, distance, qmj::is_mem_copy<Iter>());
		}

		template<typename Iter>inline
			static pointer copy_construct_imple_memory
			(pointer dest, Iter first, size_type distance, true_type)
		{
			std::memcpy(dest, (&(*first)), sizeof(value_type)*distance);
			return (dest + distance);
		}

		template<typename Iter>inline
			static pointer copy_construct_imple_memory
			(pointer dest, Iter first, size_type distance, false_type)
		{
			for (; distance != 0; --distance, ++first, ++dest)
				new (dest) value_type(*first);
			return (dest);
		}

		static void destroy_imple(pointer ptr, true_type)
		{
		}

		static void destroy_imple(pointer ptr, false_type)
		{
			ptr->~value_type();
		}

		template<typename type>inline
			static void destroy_imple(type*ptr, true_type)
		{
		}

		template<typename type>inline
			static void destroy_imple(type*ptr, false_type)
		{
			ptr->~type();
		}

		inline static void destroy(pointer first, pointer last, true_type)
		{
		}

		inline static void destroy(pointer first, pointer last, false_type)
		{
			for (; first != last; ++first)
				first->~value_type();
		}
	};

	template<typename value_type>
	class allocator :
		public allocator_base<value_type, default_alloc>
	{//默认分配器，使用二级分配器
	public:
		typedef allocator_base<value_type, default_alloc> base_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::const_pointer const_pointer;
		typedef typename base_type::reference reference;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::alloc alloc;

		template<typename value_type>
		struct rebind
		{
			typedef allocator<value_type> other;
		};
	};

	template<typename value_type>
	class simple_allocator :
		public allocator_base<value_type, malloc_alloc>
	{//使用一级分配器
	public:
		typedef allocator_base<value_type, malloc_alloc> base_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::const_pointer const_pointer;
		typedef typename base_type::reference reference;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::alloc alloc;

		template<typename value_type>
		struct rebind
		{
			typedef simple_allocator<value_type> other;
		};
	};
}

#endif
