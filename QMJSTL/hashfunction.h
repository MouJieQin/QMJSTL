#pragma once
#ifndef _HASHFUNCTION_
#define _HASHFUNCTION_

#include<string>

namespace qmj
{
	template<typename key>
	struct hash {};

	template<>
	struct hash<std::string>
	{
		size_t operator()(const std::string& str)const
		{
			size_t h = 0;
			for (auto iter = str.cbegin(); iter != str.cend(); ++iter)
				h = (h << 2) + *iter;
			return h;
		}
	};

	inline size_t hash_string(const char*s)
	{
		size_t h = 0;
		for (; *s; ++s)
			h = (h << 2) + *s;
	}

	template<>
	struct hash<char*>
	{
		size_t operator()(char* s)const
		{
			return hash_string(s);
		}
	};


	template<>
	struct hash<const char*>
	{
		size_t operator()(const char*s)const
		{
			return hash_string(s);
		}
	};

	template<>
	struct hash<char>
	{
		size_t operator()(char x)const
		{
			return x;
		}
	};

	template<>
	struct hash<signed char>
	{
		size_t operator()(signed char x)const
		{
			return x;
		}
	};

	template<>
	struct hash<unsigned char>
	{
		size_t operator()(unsigned char x)const
		{
			return x;
		}
	};

	template<>
	struct hash<short>
	{
		size_t operator()(short x)const
		{
			return x;
		}
	};

	template<>
	struct hash<unsigned short>
	{
		size_t operator()(unsigned short x)const
		{
			return x;
		}
	};

	template<>
	struct hash<int>
	{
		size_t operator()(int x)const
		{
			return x;
		}
	};

	template<>
	struct hash<unsigned int>
	{
		size_t operator()(unsigned int x)const
		{
			return x;
		}
	};

	template<>
	struct hash<long>
	{
		size_t operator()(long x)const
		{
			return x;
		}
	};

	template<>
	struct hash<unsigned long>
	{
		size_t operator()(unsigned long x)const
		{
			return x;
		}
	};
}

#endif