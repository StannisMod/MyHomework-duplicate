#ifndef SAFE_OPERATIONS_H
#define SAFE_OPERATIONS_H

#include <limits>

struct signed_tag
{};

struct unsigned_tag
{};

template <typename c>
struct container
{};

template <>
struct container <int>
{
	typedef signed_tag tag;
};

template <>
struct container <long>
{
	typedef signed_tag tag;
};

template <>
struct container <char>
{
	typedef signed_tag tag;
};

template <>
struct container <short>
{
	typedef signed_tag tag;
};

template <>
struct container <long long>
{
	typedef signed_tag tag;
};


template <>
struct container <unsigned int>
{
	typedef unsigned_tag tag;
};

template <>
struct container <unsigned long>
{
	typedef unsigned_tag tag;
};

template <>
struct container <unsigned char>
{
	typedef unsigned_tag tag;
};

template <>
struct container <unsigned short>
{
	typedef unsigned_tag tag;
};

template <>
struct container <unsigned long long>
{
	typedef unsigned_tag tag;
};


//									safe add


template <typename c>
bool safe_add(c first, c second)
{
	if (first > 0 && second > 0 && std::numeric_limits<c>::max() - first < second)
		return false;
	else if (first < 0 && second < 0 && std::numeric_limits<c>::min() - first > second)
		return false;
	return true;
}


//                              safe sub



template <typename c>
bool safe_sub_impl(c first, c second, signed_tag tag)
{
	if (first > 0 && second < 0 && std::numeric_limits<c>::max() + second < first)
		return false;
	else if (first < 0 && second > 0 && std::numeric_limits<c>::min() - first > -second)
		return false;
	return true;
}

template <typename c>
bool safe_sub_impl(c first, c second, unsigned_tag tag)
{
	return second > first;
}

template <typename c>
bool safe_sub(c first, c second)
{
	return safe_sub_impl(first, second, typename container <c>::tag());
}


//									safe unary minus



template <typename c>
bool unary_minus_impl(c first, signed_tag tag)
{
	return !(c == std::numeric_limits<c>::min());
}

template <typename c>
bool unary_minus_impl(c first, unsigned_tag tag)
{
	return c == std::numeric_limits<c>::min();
}

template <typename c>
bool unary_minus(c first)
{
	return unary_minus_impl(first, typename container <c>::tag());
}


//                                      safe mul



template <typename c>
bool safe_mul(c first, c second)
{
	if (first == 0 || second == 0)
		return true;
	return (((c)(first * second)) / second == first && ((c)(second * first)) / first == second);
}



//                                       safe div



template <typename c>
bool safe_div_impl(c first, c second, signed_tag tag)
{
	return second != (c)-1;
}

template <typename c>
bool safe_div_impl(c first, c second, unsigned_tag tag)
{
	return true;
}

template <typename c>
bool safe_div(c first, c second)
{
	return safe_div_impl(first, second, typename container <c>::tag());
}

#endif 