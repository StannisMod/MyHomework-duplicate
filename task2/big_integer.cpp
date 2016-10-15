#include "big_integer.h"

const uint32_t  BASE = 32;
const uint64_t MOD = ((uint64_t)1) << BASE;
const uint32_t MAX = UINT32_MAX;

void copy(uint32_t *a, uint32_t *tmp, uint32_t s, uint32_t f) {
	for (uint32_t i = s; i <= f; i++)
		tmp[i] = a[i];
}

void big_integer::increase() {
	if (size + 1 == capasity) {
		capasity = 2 * capasity;
		uint32_t *tmp = new uint32_t[capasity];
		copy(arg, tmp, 0, size);
		delete[] arg;
		arg = tmp;
	}
}

void big_integer::reduce() {
	for (uint32_t i = size - 1; i > 0; i--) {
		if (arg[i] == 0 && arg[size] == 0 || arg[i] == MAX && arg[size] == 1) {
			arg[i] = arg[i + 1];
			size--;
		}
		else
			break;
	}
	if (4 * size < capasity - 1) {
		capasity /= 2;
		uint32_t *tmp = new uint32_t[capasity];
		copy(arg, tmp, 0, size);
		delete[] arg;
		arg = tmp;
	}
}

void big_integer::set_sign(uint32_t s) {
	capasity = 2;
	arg = new uint32_t[capasity];
	arg[size] = s;
}

void big_integer::push_back(uint32_t a) {
	increase();
	arg[size + 1] = arg[size];
	arg[size] = a;
	size++;
}

void big_integer::resize(uint32_t s) {
	if (size >= s)
		return;
	uint32_t tmp = arg[size] == 0 ? 0 : MAX;
	while (size < s) {
		push_back(tmp);
	}
}

big_integer::big_integer() {
	set_sign(0);
	push_back(0);
}

big_integer::big_integer(int a) {
	if (a >= 0) {
		set_sign(0);
	}
	else {
		set_sign(1);
	}
	push_back(a);
}

big_integer::big_integer(big_integer const& other) {
	set_sign(other.arg[other.size]);
	resize(other.size);
	copy(other.arg, arg, 0, other.size);
}

big_integer::big_integer(std::string const& str) {
	uint32_t i = 0, s = str.length();
	if (str[0] == '-')
		i++;
	big_integer tmp((int)(str[i] - '0'));
	i++;
	while (i < s) {
		tmp = smul(tmp, 10);
		tmp += (int)(str[i] - '0');
		i++;
	}
	if (str[0] == '-')
		tmp = -tmp;
	set_sign(0);
	resize(tmp.size);
	copy(tmp.arg, arg, 0, tmp.size);
}

big_integer& big_integer::operator=(big_integer const& other) {
	if (&other == (this))
		return (*this);
	resize(other.size);
	if (other.size < size) {
		delete[] arg;
		arg = new uint32_t[other.capasity];
		capasity = other.capasity;
	}
	size = other.size;
	copy(other.arg, arg, 0, other.size);
	reduce();
	return (*this);
}

big_integer::~big_integer() {
	delete[] arg;
}

bool operator==(big_integer const& a, big_integer const& b) {
	if (a.size != b.size)
		return false;
	for (uint32_t i = 0; i <= a.size; i++)
		if (a.arg[i] != b.arg[i])
			return false;
	return true;
}

bool operator!=(big_integer const& a, big_integer const& b) {
	return !(a == b);
}

bool operator<(big_integer const& a, big_integer const& b) {
	if (a.arg[a.size] != b.arg[b.size])
		return a.arg[a.size] > b.arg[b.size];
	if (a.size != b.size)
		return (a.size > b.size) == a.arg[a.size];
	for (int32_t i = (int32_t)(b.size - 1); i >= 0; i--)
		if (b.arg[i] > a.arg[i])
			return true;
	return false;
}

bool operator>(big_integer const& a, big_integer const& b) {
	return !(a <= b);
}

bool operator<=(big_integer const& a, big_integer const& b) {
	return a < b || a == b;
}

bool operator>=(big_integer const& a, big_integer const& b) {
	return !(a < b);
}

big_integer& big_integer::operator++() {
	(*this) += 1;
	return *this;
}

big_integer big_integer::operator++(int) {
	big_integer tmp = (*this);
	++(*this);
	return tmp;
}

big_integer& big_integer::operator--() {
	(*this) -= 1;
	return *this;
}

big_integer big_integer::operator--(int) {
	big_integer tmp = (*this);
	--(*this);
	return tmp;
}

big_integer big_integer::operator+() const {
	big_integer tmp = (*this);
	return tmp;
}

big_integer big_integer::operator-() const {
	big_integer tmp = ~(*this) + 1;
	return tmp;
}

big_integer big_integer::operator~() const {
	big_integer tmp = (*this);
	tmp.arg[size] = !(arg[size]);
	for (uint32_t i = 0; i < size; i++)
		tmp.arg[i] = ~arg[i];
	return tmp;
}

big_integer& big_integer::operator&=(big_integer const& rhs) {
	resize(rhs.size);
	big_integer tmp = rhs;
	tmp.resize(size);
	for (uint32_t i = 0; i <= size; i++)
		arg[i] = tmp.arg[i] & arg[i];
	return (*this);
}

big_integer& big_integer::operator|=(big_integer const& rhs) {
	resize(rhs.size);
	big_integer tmp = rhs;
	tmp.resize(size);
	for (uint32_t i = 0; i <= size; i++)
		arg[i] = tmp.arg[i] | arg[i];
	return (*this);
}

big_integer& big_integer::operator^=(big_integer const& rhs) {
	resize(rhs.size);
	big_integer tmp = rhs;
	tmp.resize(size);
	for (uint32_t i = 0; i <= size; i++)
		arg[i] = tmp.arg[i] ^ arg[i];
	return (*this);
}

big_integer& big_integer::operator+=(big_integer const& rhs) {
	uint64_t res;
	uint32_t i;
	uint32_t carry = 0;
	resize(rhs.size);
	big_integer tmp = rhs;
	tmp.resize(size);
	for (i = 0; i < size; i++) {
		res = (uint64_t)arg[i] + carry + tmp.arg[i];
		carry = (uint8_t)(res >> BASE);
		res = res - ((res >> BASE) << BASE);
		arg[i] = (uint32_t)res;
	}
	if ((arg[size] == rhs.arg[rhs.size]) && (arg[size] ^ carry)) {
		if (arg[size] == 1)
			push_back(MAX);
		else
			push_back(1);
	}
	if ((arg[size] ^ rhs.arg[rhs.size]) && (arg[size] == carry)) {
		if (arg[size] == 1)
			arg[size] = 0;
		else
			arg[size] = 1;
	}
	reduce();
	return (*this);
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
	return (*this) += -rhs;
}

big_integer smul(big_integer const& a, uint32_t b) {
	big_integer tmp;
	tmp.size = 0;
	uint64_t res, carry = 0;
	for (uint32_t j = 0; j < a.size; j++) {
		res = (uint64_t)a.arg[j] * b + carry;
		carry = res >> BASE;
		res = res - (carry << BASE);
		tmp.push_back((uint32_t)res);
	}
	if (carry > 0)
		tmp.push_back((uint32_t)carry);
	return tmp;
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
	uint32_t is_negate = (arg[size] ^ rhs.arg[rhs.size]);
	big_integer ans;
	if (arg[size] == 1)
		(*this) = -(*this);
	if (rhs.size == 1) {
		uint32_t c = rhs.arg[rhs.size] == 0 ? rhs.arg[0] : ~rhs.arg[0] + 1;
		ans = smul((*this), c);
	}
	else {
		ans = 0;
		big_integer modrhs, tmp;
		if (rhs.arg[rhs.size] == 1)
			modrhs = -rhs;
		else
			modrhs = rhs;
		for (uint32_t i = 0; i < modrhs.size; i++) {
			ans += smul((*this), modrhs.arg[i]) << (i * BASE);
		}
	}
	if (is_negate)
		(*this) = -ans;
	else
		(*this) = ans;
	return (*this);
}

big_integer sdiv(big_integer const& a, uint32_t &b) {
	big_integer tmp;
	tmp.resize(a.size);
	uint64_t res, carry = 0;
	for (uint32_t j = a.size - 1; j < UINT32_MAX; j--) { // j >= 0
		res = ((carry << BASE) + a.arg[j]) / b;
		carry = ((carry << BASE) + a.arg[j]) - b * res;
		tmp.arg[j] = (uint32_t)res;
	}
	b = (uint32_t)carry;
	tmp.reduce();
	return tmp;
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
	uint32_t is_negate = (arg[size] ^ rhs.arg[rhs.size]);
	big_integer ans;
	if (arg[size] == 1)
		(*this) = -(*this);
	if (rhs > (*this))
		ans = big_integer(0);
	else if (rhs.size == 1) {
		uint32_t c = rhs.arg[rhs.size] == 0 ? rhs.arg[0] : ~rhs.arg[0] + 1;
		ans = sdiv((*this), c);
	}
	else {
		ans = 0;
		big_integer modrhs, tmp;
		if (rhs.arg[rhs.size] == 1)
			modrhs = -rhs;
		else
			modrhs = rhs;
		uint32_t n = modrhs.size, m = size - modrhs.size;
		ans.resize(m + 1);
		uint64_t temp1, temp2, guess, r, carry, borrow, temp, scale;
		scale = MOD / (modrhs.arg[n - 1] + 1);
		if (scale > 1) {
			(*this) = smul((*this), (uint32_t)scale);
			modrhs = smul(modrhs, (uint32_t)scale);
		}
		uint32_t v, u;
		for (v = m, u = m + n; v < MAX; v--, u--) {
			guess = (((uint64_t)arg[u] << BASE) + arg[u - 1]) / modrhs.arg[n - 1];
			r = (((uint64_t)arg[u] << BASE) + arg[u - 1]) % modrhs.arg[n - 1];
			uint32_t *shift = arg + v, i;
			while (r < MOD) {
				temp2 = modrhs.arg[n - 2] * guess;
				temp1 = (r << BASE) + arg[u - 2];
				if ((temp2 > temp1) || (guess == MOD)) {
					guess--;
					r += modrhs.arg[n - 1];
				}
				else break;
			}
			carry = 0;
			borrow = 0;
			for (i = 0; i < n; i++) {
				temp1 = modrhs.arg[i] * guess + carry;
				carry = temp1 >> BASE;
				temp1 = temp1 - (carry << BASE);
				if (shift[i] < temp1 + borrow) {
					shift[i] = (uint32_t)(MOD + shift[i] - temp1 - borrow);
					borrow = 1;
				}
				else {
					shift[i] = (uint32_t)(shift[i] - borrow - temp1);
					borrow = 0;
				}
			}
			if (shift[i] < carry + borrow) {
				shift[i] = (uint32_t)(MOD + shift[i] - carry - borrow);
				borrow = 1;
			}
			else {
				shift[i] = (uint32_t)(shift[i] - borrow - temp1);
				borrow = 0;
			}
			if (borrow == 0) {
				ans.arg[v] = (uint32_t)guess;
			}
			else {
				ans.arg[v] = (uint32_t)(guess - 1);
				carry = 0;
				for (i = 0; i < n; i++) {
					temp = (uint64_t)shift[i] + modrhs.arg[i] + carry;
					if (temp >= MOD) {
						shift[i] = (uint32_t)(temp - MOD);
						carry = 1;
					}
					else {
						shift[i] = (uint32_t)temp;
						carry = 0;
					}
				}
				shift[i] = (uint32_t)((uint64_t)shift[i] + carry - MOD);
			}
			reduce();
		}
		ans.reduce();
	}
	if (is_negate)
		(*this) = -ans;
	else
		(*this) = ans;
	return (*this);
}
big_integer& big_integer::operator%=(big_integer const& rhs) {
	big_integer modrhs = rhs.arg[rhs.size] == 0 ? rhs : -rhs;
	(*this) -= (((*this) / rhs) * rhs);
	return (*this);
}

big_integer& big_integer::operator<<=(int rhs) {
	uint32_t is_neg = arg[size];
	if (is_neg)
		(*this) = -(*this);
	big_integer tmp = (*this);
	uint32_t q = rhs / BASE, i;
	rhs -= q * BASE;
	resize(size + (q + 1));
	for (i = 0; i < q; i++)
		arg[i] = 0;
	uint64_t res;
	uint32_t carry = 0;
	for (; i - q < tmp.size; i++) {
		res = carry + ((uint64_t)tmp.arg[i - q] << (rhs));
		carry = (res >> BASE);
		res = res - ((uint64_t)carry << BASE);
		arg[i] = (uint32_t)res;
	}
	arg[size - 1] = carry;
	reduce();
	if (is_neg)
		(*this) = -(*this);

	return (*this);
}

big_integer& big_integer::operator>>=(int rhs) {
	big_integer tmp = (*this);
	uint32_t q = rhs / BASE, i;
	rhs -= q * BASE;
	for (i = q; i <= tmp.size; i++)
		arg[i - q] = tmp.arg[i];
	size -= q;
	uint64_t res;
	for (i = 0; i < size; i++) {
		if (i == size - 1) {
			if (arg[size] == 1)
				res = MAX;
			else
				res = 0;
		}
		else
			res = arg[i + 1];
		res -= ((res >> rhs) << rhs);
		res <<= (BASE - rhs);
		res += (arg[i] >> rhs);
		arg[i] = (uint32_t)res;
	}

	reduce();
	return (*this);
}

big_integer operator+(big_integer a, big_integer const& b) {
	big_integer tmp = a;
	tmp += b;
	return tmp;
}

big_integer operator-(big_integer a, big_integer const& b) {
	big_integer tmp = a;
	tmp -= b;
	return tmp;
}

big_integer operator*(big_integer a, big_integer const& b) {
	big_integer tmp = a;
	tmp *= b;
	return tmp;
}

big_integer operator/(big_integer a, big_integer const& b) {
	big_integer tmp = a;
	tmp /= b;
	return tmp;
}

big_integer operator%(big_integer a, big_integer const& b) {
	big_integer tmp = a;
	tmp %= b;
	return tmp;
}

big_integer operator&(big_integer a, big_integer const& b) {
	big_integer tmp = a;
	tmp &= b;
	return tmp;
}

big_integer operator|(big_integer a, big_integer const& b) {
	big_integer tmp = a;
	tmp |= b;
	return tmp;
}

big_integer operator^(big_integer a, big_integer const& b) {
	big_integer tmp = a;
	tmp ^= b;
	return tmp;
}

big_integer operator<<(big_integer a, int b) {
	big_integer tmp = a;
	tmp <<= b;
	return tmp;
}
big_integer operator >> (big_integer a, int b) {
	big_integer tmp = a;
	tmp >>= b;
	return tmp;
}

std::string to_string(big_integer const& a) {
	if (a == 0)
		return "0";
	big_integer tmp;
	std::string s;
	a.arg[a.size] == 1 ? tmp = -a : tmp = a;
	uint32_t b;
	while (tmp > 0) {
		b = 10;
		tmp = sdiv(tmp, b);
		s.push_back((char)(b + '0'));
	}
	if (a.arg[a.size] == 1)
		s.push_back('-');
	std::reverse(std::begin(s), std::end(s));
	return s;
}

std::ostream& operator<<(std::ostream& s, big_integer const& a) {
	s << to_string(a);
	return s;
}