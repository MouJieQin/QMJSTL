#pragma once
#ifndef _hashtable_
#define _hashtable_
#include<utility>
#include"hashfunction.h"
#include"vector_qmj.h"
#include"type_traits_qmj.h"
#include"algorithm_qmj.h"

namespace qmj
{
	template<typename value_type>
	struct hashtable_node
	{
	public:
		typedef hashtable_node<value_type>* link_type;

		hashtable_node(link_type next = nullptr) :
			value(), next(next)
		{
		}

		hashtable_node(const value_type&value,
			link_type next = nullptr) :
			value(value), next(next)
		{
		}

		value_type value;
		link_type next;
	};

	template<typename traits>
	class hashtable;

	template<typename traits>
	class hashtable_const_iterator
	{
	public:
		friend class hashtable<traits>;

		typedef std::forward_iterator_tag iterator_category;
		typedef typename traits::value_type value_type;
		typedef const value_type& reference;
		typedef const value_type* pointer;
		typedef std::ptrdiff_t difference_type;

		typedef size_t size_type;
		typedef hashtable_const_iterator<traits> self;
		typedef const hashtable<traits>* hashtable_link;
		typedef hashtable_node<value_type>* link_type;

		hashtable_const_iterator(
			link_type cur = nullptr,
			hashtable_link ht = nullptr) :
			cur(cur), ht(ht)
		{
		}

		hashtable_const_iterator(const self&x) :
			cur(x.cur), ht(x.ht)
		{
		}

		self& operator=(const self&x)
		{
			cur = x.cur;
			ht = x.ht;
			return (*this);
		}

		bool operator==(const self& it)const
		{
			return (cur == it.cur);
		}

		bool operator!=(const self& it)const
		{
			return (!(operator==(it)));
		}

		reference operator*()const
		{
			return (cur->value);
		}

		pointer operator->()const
		{
			return (&(operator*()));
		}

		self& operator++()
		{
			const link_type old = cur;
			cur = cur->next;
			if (!cur)
			{
				size_type index = ht->get_bucket_num(ht->get_key(old->value));
				for (size_t n = ht->buckets.size(); !cur&&++index != n;)
					cur = ht->buckets[index];
			}
			return (*this);
		}

		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return (tmp);
		}
	protected:
		link_type cur;
		hashtable_link ht;
	};

	template<typename traits>
	class hashtable_iterator :
		public hashtable_const_iterator<traits>
	{
	public:
		typedef hashtable_const_iterator<traits> base_type;

		typedef std::forward_iterator_tag iterator_category;
		typedef typename traits::value_type value_type;
		typedef value_type& reference;
		typedef value_type* pointer;
		typedef std::ptrdiff_t difference_type;

		typedef size_t size_type;
		typedef hashtable_iterator<traits> self;
		typedef const hashtable<traits>* hashtable_link;
		typedef hashtable_node<value_type>* link_type;

		hashtable_iterator(
			const link_type cur = nullptr,
			const hashtable_link ht = nullptr) :
			base_type(cur, ht)
		{
		}

		hashtable_iterator(const self&x) :
			base_type(x.cur, x.ht)
		{
		}

		self&operator=(const self&x)
		{
			cur = x.cur;
			ht = x.ht;
		}

		bool operator==(const self& it)const
		{
			return (cur == it.cur);
		}

		bool operator!=(const self& it)const
		{
			return (!(operator==(it)));
		}

		reference operator*()const
		{
			return (cur->value);
		}

		pointer operator->()const
		{
			return (&(operator*()));
		}

		self& operator++()
		{
			const link_type old = cur;
			cur = cur->next;
			if (!cur)
			{
				size_type index = ht->get_bucket_num(ht->get_key(old->value));
				for (size_t n = ht->buckets.size(); !cur&&++index != n;)
					cur = ht->buckets[index];
			}
			return (*this);
		}

		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return (tmp);
		}
	};

	template<typename traits>
	class hashtable_const_local_iterator
	{
	public:
		friend class hashtable<traits>;

		typedef std::forward_iterator_tag iterator_category;
		typedef typename traits::value_type value_type;
		typedef const value_type& reference;
		typedef const value_type* pointer;
		typedef std::ptrdiff_t difference_type;

		typedef size_t size_type;
		typedef hashtable_const_local_iterator<traits> self;
		typedef hashtable_node<value_type>* link_type;

		hashtable_const_local_iterator(link_type cur = nullptr) :
			cur(cur)
		{
		}

		hashtable_const_local_iterator(const self&x) :
			cur(x.cur)
		{
		}

		bool operator==(const self& it)const
		{
			return (cur == it.cur);
		}

		bool operator!=(const self& it)const
		{
			return (!(operator==(it)));
		}

		reference operator*()const
		{
			return (cur->value);
		}

		pointer operator->()const
		{
			return (&(operator*()));
		}

		self& operator++()
		{
			cur = cur->next;
			return (*this);
		}

		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return (tmp);
		}

	protected:
		link_type cur;
	};

	template<typename traits>
	class hashtable_local_iterator :
		public hashtable_const_local_iterator<traits>
	{
	public:
		friend class hashtable<traits>;

		typedef std::forward_iterator_tag iterator_category;
		typedef typename traits::value_type value_type;
		typedef value_type& reference;
		typedef value_type* pointer;
		typedef std::ptrdiff_t difference_type;

		typedef size_t size_type;
		typedef hashtable_const_local_iterator<traits> base_type;
		typedef hashtable_local_iterator<traits> self;
		typedef hashtable_node<value_type>* link_type;

		hashtable_local_iterator(link_type cur = nullptr) :
			base_type(cur)
		{
		}

		hashtable_local_iterator(const self&x) :
			base_type(x)
		{
		}

		bool operator==(const self& it)const
		{
			return (cur == it.cur);
		}

		bool operator!=(const self& it)const
		{
			return (!(operator==(it)));
		}

		reference operator*()const
		{
			return (cur->value);
		}

		pointer operator->()const
		{
			return (&(operator*()));
		}

		self& operator++()
		{
			cur = cur->next;
			return (*this);
		}

		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return (tmp);
		}
	};

	static const int num_primes = 31;
	static const std::size_t prime_list[num_primes]
		= {
		7,19,37,
		53,97,193,389,769,
		1543,3079,6151,12289,24593,
		49157,98317,196613,393241,786433,
		1572869,3145739,6291469,12582917,25165843,
		50331653,100663319,201326611,402653189,805306457,
		1610612741,3221225437ul,4294967291ul
	};

	inline std::size_t next_prime(std::size_t n)
	{
		const size_t* first = prime_list;
		auto last = first + num_primes;
		auto pos = qmj::lower_bound(first, last, n);
		return
			pos == last ? *(last - 1) : *pos;
	}

	template<typename traits>
	class hashtable
	{
	public:
		friend class hashtable_const_iterator<traits>;
		friend class hashtable_iterator<traits>;

		typedef typename traits::allocator_type allocator_type;
		typedef typename traits::HashFunction hashfunction;
		typedef typename traits::EqualKey equalkey;
		typedef typename traits::HashFunction hasher;
		typedef typename traits::EqualKey key_equal;
		typedef typename traits::key_type  key_type;
		typedef typename traits::value_type value_type;
		typedef value_type* pointer;
		typedef const value_type*  const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef typename allocator_type::template rebind<hashtable_node<value_type>>::other alloc;
		typedef hashtable_const_iterator<traits> const_iterator;
		typedef typename If<is_same<key_type, value_type>::value,
			const_iterator, hashtable_iterator<traits>>::type iterator;
		typedef hashtable_const_local_iterator<traits> const_local_iterator;
		typedef typename If<is_same<key_type, value_type>::value,
			const_local_iterator, hashtable_local_iterator<traits>>::type local_iterator;
		enum
		{
			is_multi = traits::is_multi
		};

		typedef hashtable_node<value_type>* link_type;
		typedef hashtable<traits>self;
		typedef qmj::vector<link_type> container;

		typedef std::pair<iterator, bool> PairIB;
		typedef std::pair<local_iterator, local_iterator>PairII;
		typedef std::pair<const_local_iterator, const_local_iterator>PairCC;
		typedef std::pair<link_type, link_type>PairLL;

		hashtable() :
			hash(), equals(), num_elements(0)
		{
			init_buckets(0);
		}

		explicit hashtable(size_t n) :
			hash(), equals(), num_elements(0)
		{
			init_buckets(n);
		}

		hashtable(size_t n, const hashfunction&hash) :
			hash(hash), equals(), num_elements(0)
		{
			init_buckets(n);
		}

		hashtable(const hashfunction&hash,
			const equalkey&equals) :
			hash(hash), equals(equals),
			num_elements(0)
		{
			init_buckets(0);
		}

		hashtable(const size_t n,
			const hashfunction& hash,
			const equalkey& equals) :
			hash(hash), equals(equals),
			num_elements(0)
		{
			init_buckets(n);
		}

		hashtable(const self&x) :
			buckets(x.buckets.size(), nullptr),
			hash(x.hash), equals(x.equals)
			num_elements(x.num_elements)
		{
			for (size_t i = 0; i != buckets.size(); ++i)
				for (cur = x.buckets[i]; cur; cur = cur->next)
					buckes[i] = create_node(cur->value, buckets[i]);
		}

		hashtable(self&&x) :
			num_elements(x.num_elements),
			hash(std::move(x.hash)),
			equals(std::move(x.equals)),
			buckets(std::move(x.buckets))
		{
		}

		self& operator=(self x)
		{
			hash = x.hash;
			equals = x.equals;
			num_elements = x.num_elements;
			buckets.swap(x.buckets);
			return (*this);
		}

		~hashtable()
		{
			clear();
		}

		void swap(self&x)noexcept
		{
			using std;
			swap(hash, x.hash);
			swap(equals, x.equals);
			std::swap(num_elements, x.num_elements);
			buckets.swap(x.buckets);
		}

		iterator begin()
		{
			if (empty())
				return (end());
			size_t i = 0;
			for (const size_t len = buckets.size(); i != len && (!buckets[i]);)
				++i;
			return  (iterator(buckets[i], this));
		}

		local_iterator begin(size_type index)
		{
			return (local_iterator(buckets[index]));
		}

		iterator end()
		{
			return (iterator(nullptr, this));
		}

		local_iterator end(size_type index)
		{
			return (iterator(nullptr));
		}

		const_iterator begin()const
		{
			return (cbegin());
		}

		const_local_iterator begin(size_type index)const
		{
			return (cbegin(index));
		}

		const_iterator end()const
		{
			return (cend());
		}

		const_local_iterator end(size_type index)const
		{
			return (cend(index));
		}

		const_iterator cbegin()const
		{
			if (empty())
				return (cend());
			size_t i = 0;
			for (const size_t len = buckets.size(); i != len && (!buckets[i]);)
				++i;
			return  (const_iterator(buckets[i], this));
		}

		const_local_iterator cbegin(size_type index)const
		{
			return (const_local_iterator(buckets[index]));
		}

		const_iterator cend()const
		{
			return (const_iterator(nullptr, this));
		}

		const_local_iterator cend(size_type index)const
		{
			return (const_local_iterator(nullptr));
		}

		size_type size()const
		{
			return (num_elements);
		}

		bool empty()const
		{
			return (!size());
		}

		float load_factor() const noexcept
		{	// return elements per bucket
			return ((float)size() / (float)bucket_count());
		}

		size_type erase(const key_type&k)
		{
			size_t n = get_bucket_num(k);
			link_type cur = buckets[n];
			size_type count = 0;
			for (link_type tmp = cur; cur&&equals(get_key(cur->value), k); tmp = cur)
			{
				--num_elements;
				cur = cur->next;
				++count;
				destroy_and_free_node(tmp);
			}
			if (buckets[n] = cur)
			{
				for (link_type next = cur->next; next; next = cur->next)
				{
					if (equals(get_key(next->value), k))
					{
						++count;
						--num_elements;
						cur->next = next->next;
						destroy_and_free_node(next);
					}
					else
						cur = next;
				}
			}
			return (count);
		}

		void erase(const_iterator x)
		{
			erase(get_key(*x));
		}

		void erase(const_iterator first, const_iterator last)
		{
			for (; first != last;)
				erase(first++);
		}

		void clear()
		{
			const size_t len = buckets.size();
			for (size_t i = 0; i != len; ++i)
			{
				for (link_type cur = buckets[i], next; cur; cur = next)
				{
					next = cur->next;
					destroy_and_free_node(cur);
				}
			}
			memset(&buckets[0], 0, len);
			num_elements = 0;
		}

		size_type bucket(const key_type&k)const
		{
			return (get_bucket_num(k));
		}

		size_type count(const key_type&k)const
		{
			size_type counter = 0;
			size_t n = get_bucket_num(k);
			for (link_type cur = buckets[n]; cur; cur = cur->next)
				if (equals(get_key(cur->key), k))
					++counter;
			return (counter);
		}

		const_iterator find(const key_type&k)const
		{
			return (const_iterator(find_imple(k), this));
		}

		iterator find(const key_type&k)
		{
			return (iterator(find_imple(k), this));
		}

		size_type bucket_count()const
		{
			return (buckets.size());
		}

		size_type bucket_size(const size_type n)const
		{
			return (elements_in_bucket(n));
		}

		size_type max_size()const
		{
			return (prime_list[num_primes - 1]);
		}

		template<bool multi = is_multi>
		enable_if_t<!multi, PairIB>
			emplace(value_type&&val)
		{
			return (emplace_imple(std::forward<value_type>(val)));
		}

		template<bool multi = is_multi,
			typename...types>
			enable_if_t<!multi, PairIB>
			emplace(types&&...args)
		{
			return (emplace_imple(std::forward<types>(args)...));
		}

		template<bool multi = is_multi>
		enable_if_t<multi, iterator>
			emplace(value_type&& val)
		{
			return (emplace_imple(std::forward<value_type>(val)));
		}

		template<bool multi = is_multi,
			typename...types>
			enable_if_t<multi, iterator>
			emplace(types&&...args)
		{
			return (emplace_imple(std::forward<types>(args)...));
		}

		template<bool multi = is_multi>
		enable_if_t<!multi, PairIB>
			emplace_hint(const_iterator, value_type&&val)
		{
			return (emplace_imple(std::forward<value_type>(arg)));
		}

		template<bool multi = is_multi,
			typename...types>
			enable_if_t<!multi, PairIB>
			emplace_hint(const_iterator, types&&...args)
		{
			return (emplace_imple(std::forward<types>(args)...));
		}

		template<bool multi = is_multi>
		enable_if_t<multi, iterator>
			emplace_hint(const_iterator, value_type&& val)
		{
			return (emplace_imple(std::forward<value_type>(val)));
		}

		template<bool multi = is_multi,
			typename...types>
			enable_if_t<multi, iterator>
			emplace_hint(const_iterator, types&&...args)
		{
			return (emplace_imple(std::forward<types>(args)...));
		}

		template<bool multi = is_multi, enable_if_t<!multi, int > = 0>
		PairIB insert(const value_type& val)
		{
			return (emplace(val));
		}

		template<bool multi = is_multi, enable_if_t<!multi, int > = 0>
		PairIB insert(value_type&& val)
		{
			return (emplace(std::forward<value_type>(val)));
		}

		template<bool multi = is_multi, enable_if_t<multi, int> = 0>
		iterator insert(const value_type& val)
		{
			return (emplace(val));
		}

		template<bool multi = is_multi, enable_if_t<multi, int> = 0>
		iterator insert(value_type&& val)
		{
			return (emplace(std::forward<value_type>(val)));
		}

		template<bool multi = is_multi, enable_if_t<multi, int> = 0>
		iterator insert(const_iterator, value_type&&val)
		{
			return (emplace(std::forward<value_type>(val)));
		}

		template<bool multi = is_multi, enable_if_t<!multi, int> = 0>
		iterator insert(const_iterator, value_type&&val)
		{
			return (emplace(std::forward<value_type>(val)).first);
		}

		template<bool multi = is_multi, enable_if_t<multi, int> = 0>
		iterator insert(const_iterator, const value_type&val)
		{
			return (emplace(val));
		}

		template<bool multi = is_multi, enable_if_t<!multi, int> = 0>
		iterator insert(const_iterator, const value_type&val)
		{
			return (emplace(val).first);
		}

		template<typename IIter>
		void insert(IIter first, IIter last)
		{
			for (; first != last; ++first)
				insert(*first);
		}

		void insert(const std::initializer_list<value_type>&lst)
		{
			insert(lst.begin(), lst.end());
		}

		void rehash(const size_type new_n)
		{
			resize(new_n);
		}

		void reserve(const size_type new_n)
		{
			resize(new_n);
		}

		PairII equal_range(const key_type&key)
		{
			PairLL ret = equal_range_imple(key);
			return (PairII(local_iterator(ret.first),
				local_iterator(ret.second)));
		}

		PairCC equal_range(const key_type&key)const
		{
			PairLL ret = equal_range_imple(key);
			return (PairCC(const_local_iterator(ret.first),
				const_local_iterator(ret.second)));
		}

		allocator_type get_allocator()const
		{
			return (allocator_type());
		}

		hasher hash_function()const
		{
			return (hash);
		}

		equalkey key_eq()const
		{
			return (equals);
		}

	private:

		void splice_after(link_type first1, link_type first2)const
		{
			link_type tar = first2->next;
			first2->next = tar->next;
			tar->next = first1->next;
			first1->next = tar;
		}

		PairLL equal_range_imple(const key_type&key)const
		{
			link_type first = find_imple(key);
			if (!first)
				return (PairLL(nullptr, nullptr));
			link_type cur = first;
			link_type next = cur->next;
			for (; next&&equals(key, get_key(next->value)); next = cur->next)
				cur = next;
			if (next)
			{
				link_type before = next;
				for (next = before->next; next; next = before->next)
				{
					if (equals(key, get_key(next->value)))
					{
						splice_after(cur, before);
						cur = cur->next;
					}
					else
						before = next;
				}
			}
			return (PairLL(first, cur->next));
		}

		template<typename...types>
		link_type create_node(types&&...args)
		{
			link_type node = alloc::allocate();
			alloc::construct(node, std::forward<types>(args)...);
			return (node);
		}

		void destroy_and_free_node(link_type node)
		{
			alloc::destroy(node);
			alloc::deallocate(node);
		}

		void init_buckets(const size_type n)
		{
			const size_type n_buckets = next_prime(n);
			buckets.resize(n_buckets, nullptr);
		}

		size_t get_bucket_num(const key_type&key,
			const size_t n)const
		{
			return hash(key) % n;
		}

		size_type get_bucket_num(const key_type& key)const
		{
			return get_bucket_num(key, buckets.size());
		}

		size_type elements_in_bucket(const size_type n)const
		{
			size_type counter = 0;
			for (link_type cur = buckets[n]; cur; cur = cur->next)
				++counter;
			return (counter);
		}

		link_type find_imple(const key_type&k)const
		{
			size_t n = get_bucket_num(k);
			link_type cur = buckets[n];
			for (; cur && (!equals(k, get_key(cur->value)));)
				cur = cur->next;
			return (cur);
		}

		const key_type& get_key(const value_type&val)const
		{
			return (traits::ExtractKey(val));
		}

		iterator make_iter(const_iterator& citer)const
		{
			return (iterator(citer.cur, citer.ht));
		}

		template<typename...types>
		PairIB insert_unique_noresize(types&&...args)
		{
			link_type node = create_node(std::forward<types>(args)...);
			const value_type&val = node->value;
			size_type n = get_bucket_num(get_key(val));
			link_type first = buckets[n];

			for (link_type cur = first; cur; cur = cur->next)
			{
				if (equals(get_key(cur->value), get_key(val)))
				{
					destroy_and_free_node(node);
					return (PairIB(iterator(cur, this), false));
				}
			}
			node->next = first;
			buckets[n] = node;
			++num_elements;
			return (PairIB(iterator(node, this), true));
		}

		template<typename value_type>
		PairIB insert_unique_noresize(value_type&& val)
		{
			size_type n = get_bucket_num(get_key(val));
			link_type first = buckets[n];
			for (link_type cur = first; cur; cur = cur->next)
				if (equals(get_key(cur->value), get_key(val)))
					return (PairIB(iterator(cur, this), false));
			link_type node = create_node(std::forward<value_type>(val), first);
			buckets[n] = node;
			++num_elements;
			return (PairIB(iterator(node, this), true));
		}

		template<typename...types>
		iterator insert_equal_noresize(types&&...args)
		{
			link_type node = create_node(std::forward<types>(args)...);
			const value_type&val = node->value;
			const size_type n = get_bucket_num(get_key(val));
			node->next = buckets[n];
			buckets[n] = node;
			++num_elements;
			return iterator(node, this);
		}

		template<typename value_type>
		iterator insert_equal_noresize(value_type&&val)
		{
			const size_type n = get_bucket_num(get_key(val));
			link_type tmp = create_node(std::forward<value_type>(val), buckets[n]);
			buckets[n] = tmp;
			++num_elements;
			return iterator(tmp, this);
		}

		template<bool multi = is_multi, typename...types>
		enable_if_t<!multi, PairIB>
			emplace_imple(types&&...args)
		{
			resize(num_elements + 1);
			return (insert_unique_noresize(std::forward<types>(args)...));
		}

		template<bool multi = is_multi>
		enable_if_t<!multi, PairIB>
			emplace_imple(value_type&&val)
		{
			resize(num_elements + 1);
			return (insert_unique_noresize(std::forward<value_type>(val)));
		}

		template<bool multi = is_multi, typename...types>
		enable_if_t<multi, iterator>
			emplace_imple(types&&...args)
		{
			resize(num_elements + 1);
			return (insert_equal_noresize(std::forward<types>(args)...));
		}

		template<bool multi = is_multi>
		enable_if_t<multi, iterator>
			emplace_imple(value_type&&val)
		{
			resize(num_elements + 1);
			return (insert_equal_noresize(std::forward<value_type>(val)));
		}

		void resize(const size_type new_n)
		{
			const size_type old_n = buckets.size();
			if (new_n > old_n)
			{
				const size_type n = next_prime(new_n);
				if (n > old_n)
				{
					container tmp(n, nullptr);
					size_type new_bucket_num;
					for (size_type i = 0; i != old_n; ++i)
						for (link_type first = buckets[i]; first; first = buckets[i])
						{
							new_bucket_num = get_bucket_num(get_key(first->value), n);
							buckets[i] = first->next;
							first->next = tmp[new_bucket_num];
							tmp[new_bucket_num] = first;
						}
					buckets.swap(tmp);
				}
			}
		}

	private:
		hasher hash;
		key_equal equals;
		container buckets;
		size_type num_elements;
	};

	template<typename traits>inline
		void swap(hashtable<traits>&left, hashtable<traits>&right)noexcept
	{
		left.swap(right);
	}

	template<typename traits>inline
		bool _hash_element_equal(const hashtable<traits>&left,
			const hashtable<traits>&right, true_type)
	{
		typedef typename hashtable<traits>::const_iterator Iter;
		typedef typename hashtable<traits>::const_local_iterator LIter;
		typedef std::pair<LIter, LIter> PairLII;
		PairLII Lrange;
		PairLII Rrange;
		for (Iter first = left.cbegin(), last = right.cend(); first != last; ++first)
		{
			Lrange = left.equal_range(traits::ExtractKey(*first));
			Rrange = right.equal_range(traits::ExtractKey(*first));
			if (!_QMJ is_permutation(Lrange.first, Lrange.second,
				Rrange.first, Rrange.second))
				return (false);
		}
		return (true);
	}

	template<typename traits>inline
		bool _hash_element_equal_not_multi(const hashtable<traits>&left,
			const hashtable<traits>&right, false_type)
	{
		typedef typename hashtable<traits>::const_iterator Iter;
		Iter ret;
		Iter last2 = right.cend();
		for (Iter cur = left.cbegin(), last1 = left.cend(); cur != last1; ++cur)
		{
			ret = right.find(traits::ExtractKey(*cur));
			if (ret == last2
				|| (!(traits::ExtractData(*ret) == traits::ExtractData(*cur))))
				return (false);
		}
		return (true);
	}

	template<typename traits>inline
		bool _hash_element_equal_not_multi(const hashtable<traits>&left,
			const hashtable<traits>&right, true_type)
	{
		typedef typename hashtable<traits>::const_iterator Iter;
		Iter last2 = right.cend();
		for (Iter cur = left.cbegin(), last1 = left.cend(); cur != last1; ++cur)
			if (right.find(traits::ExtractKey(*cur)) == last2)
				return (false);
		return (true);
	}

	template<typename traits>inline
		bool _hash_element_equal(const hashtable<traits>&left,
			const hashtable<traits>&right, false_type)
	{
		return (_QMJ _hash_element_equal_not_multi(left, right,
			_QMJ is_same<typename traits::key_type, typename traits::value_type>()));
	}

	template<typename traits>inline
		bool operator==(const hashtable<traits>&left, const hashtable<traits>&right)
	{
		return (left.size() == right.size()
			&& _QMJ _hash_element_equal(left, right,
				_QMJ integral_constant<bool, traits::is_multi>()));
	}

	template<typename traits>inline
		bool operator!=(const hashtable<traits>&left, const hashtable<traits>&right)
	{
		return (!(left == right));
	}
}


#endif