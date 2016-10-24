#include <limits>
#include <stdexcept>
#include <iostream>

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
struct container <float>
{
	typedef signed_tag tag;
};

template <>
struct container <double>
{
	typedef signed_tag tag;
};

template <>
struct container <long double>
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


template <typename c>
struct safe_operations
{
	c value;
	safe_operations<c>() {}
	safe_operations<c>(c val) {
		value = val;
	}
	~safe_operations<c>() {}

	safe_operations<c>& operator=(safe_operations<c> const& other) {
		value = other.value;
	}

	bool check_add(c rhs, c second, signed_tag tag)
	{
		return !(rhs > 0 && second > 0 && std::numeric_limits<c>::max() - rhs < second ||
			rhs < 0 && second < 0 && std::numeric_limits<c>::min() - rhs > second);
	}

	bool check_add(c rhs, c second, unsigned_tag tag)
	{
		return !(rhs > 0 && second > 0 && std::numeric_limits<c>::max() - rhs < second);
	}

	safe_operations<c>& operator +=(safe_operations<c> const& rhs)
	{
		if (!check_add(valur, rhs.value, typename container <c>::tag()))
			throw std::overflow_error("In expression is ovwerflow");
		value += rhs.value;
		return (*this);
	}

	bool check_sub(c rhs, c second, signed_tag tag)
	{
		return !(rhs > 0 && second < 0 && std::numeric_limits<c>::max() + second < rhs ||
			rhs < 0 && second > 0 && std::numeric_limits<c>::min() - rhs > -second);
	}

	bool check_sub(c rhs, c second, unsigned_tag tag)
	{
		return (rhs >= second);
	}

	safe_operations<c>& operator -=(safe_operations<c> const& rhs)
	{
		if (!check_sub(value, rhs.value, typename container <c>::tag()))
			throw std::overflow_error("In expression is ovwerflow");
		value -= rhs.value;
		return (*this);
	}

	bool check_mul(c rhs, c second, signed_tag tag)
	{
		if (rhs == 0 || second == 0)
			return true;
		return (((c)(rhs * second)) / second == rhs && ((c)(second * rhs)) / rhs == second);
	}

	bool check_mul(c rhs, c second, unsigned_tag tag)
	{
		if (rhs == 0 || second == 0)
			return true;
		return (((c)(rhs * second)) / second == rhs && ((c)(second * rhs)) / rhs == second);
	}

	safe_operations<c>& operator *=(safe_operations<c> const& rhs)
	{
		if (!check_mul(value, rhs.value, typename container <c>::tag()))
			throw std::overflow_error("In expression is ovwerflow");
		value *= rhs.value;
		return (*this);
	}

	bool check_div(c rhs, c second, signed_tag tag)
	{
		return !(rhs < 0 && second < 0 && rhs / second < 0);
	}

	bool check_div(c rhs, c second, unsigned_tag tag)
	{
		return true;
	}

	safe_operations<c>& operator/=(safe_operations<c> const& rhs)
	{
		if (!check_div(value, rhs.value, typename container <c>::tag()))
			throw std::overflow_error("In expression is ovwerflow");
		value /= rhs.value;
		return (*this);
	}

	bool check_unary_minus(c rhs, signed_tag tag)
	{
		return !(rhs < 0 && -rhs < 0);
	}

	bool check_unary_minus(c rhs, unsigned_tag tag)
	{
		return (rhs == 0);
	}

	safe_operations<c> operator -()
	{
		if (!check_unary_minus(value, second, typename container <c>::tag()))
			std::overflow_error("In expression is ovwerflow");
		value = -value;
		return (*this);
	}
};

template <typename c>
safe_operations<c> operator+(safe_operations<c> a, safe_operations<c> const& b) {
	safe_operations tmp = a;
	return tmp += b;
}

template <typename c>
safe_operations<c> operator-(safe_operations<c> a, safe_operations<c> const& b) {
	safe_operations tmp = a;
	return tmp -= b;
}

template <typename c>
safe_operations<c> operator*(safe_operations<c> a, safe_operations<c> const& b) {
	safe_operations tmp = a;
	return tmp *= b;
}
template <typename c>
safe_operations<c> operator/(safe_operations<c> a, safe_operations<c> const& b) {
	safe_operations tmp = a;
	return tmp /= b;
}

/*int main() {
	safe_operations <int> a = INT_MAX, b = 5;
	try {
		a *= b;
		std::cout << a.value;
	}
	catch (std::runtime_error e) {
		std::cout << e.what();
	}
}*/