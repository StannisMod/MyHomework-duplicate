#include "persistent_set.h"
#include <random>
#include <set>
#include <time.h>

void test1() {
	persistent_set y;
	for (int i = 0; i < 4; i++) {
		y.insert(i);
		y.insert(-i);
	}
	y.erase(-2);
	y.erase(-1);
	y.erase(0);
	y.erase(3);
	for (auto it = y.begin(); it != y.end(); it++)
		std::cout << *it << " ";
	std::cout << "\n";
}

void test2() {
	const int N = 3000, MOD = 2000;
	persistent_set my_set, last_version[2 * N];
	std::set <int> s[2 * N];
	int t;
	bool ans = true;
	for (int i = 1; i < N; i++) {
		t = rand() % MOD;
		last_version[i - 1] = my_set;
		s[i] = s[i - 1];
		s[i].insert(t);
		my_set.insert(t);
		if (*my_set.find(t) != t)
			ans = false;
	}
	for (int i = 1; i < N; i++) {
		t = rand() % MOD;
		auto it1 = s[N - 1].find(t);
		auto it2 = my_set.find(t);
		if (it1 != s[N - 1].end() && *it2 != *it1)
			ans = false;
	}
	for (int i = N; i < 2 * N; i++) {
		t = rand() % MOD;
		s[i] = s[i - 1];
		last_version[i - 1] = my_set;
		s[i].erase(t);
		my_set.erase(t);
		if (my_set.find(t) != my_set.end())
			ans = false;
	}
	last_version[2 * N - 1] = my_set;
	for (int i = 0; i < 2 * N; i++) {
		auto it1 = s[i].begin();
		auto it2 = last_version[i].begin();
		while (it1 != s[i].end() && it2 != last_version[i].end()) {
			if (*it1 != *it2)
				ans = false;
			it1++;
			++it2;
		}
		it2 != last_version[i].end();
		if (it1 == s[i].end() && it2 != last_version[i].end() || it1 != s[i].end() && it2 == last_version[i].end())
			ans = false;
	}
	std::cout << ans << "\n";
}

void test3() {
	const int N = 3000, MOD = 2000;
	persistent_set my_set, last_version[2 * N];
	std::set <int> s[2 * N];
	int t;
	bool ans = true;
	for (int i = 1; i < N; i++) {
		t = rand() % MOD;
		last_version[i - 1] = my_set;
		s[i] = s[i - 1];
		if (i % 2 == 0)
			continue;
		s[i].insert(t);
		my_set.insert(t);
		if (*my_set.find(t) != t)
			ans = false;
	}
	for (int i = 1; i < N; i++) {
		t = rand() % MOD;
		auto it1 = s[N - 1].find(t);
		auto it2 = my_set.find(t);
		if (it1 != s[N - 1].end() && *it2 != *it1)
			ans = false;
	}
	for (int i = N; i < 2 * N; i++) {
		t = rand() % MOD;
		s[i] = s[i - 1];
		last_version[i - 1] = my_set;
		s[i].erase(t);
		my_set.erase(t);
		if (my_set.find(t) != my_set.end())
			ans = false;
	}
	last_version[2 * N - 1] = my_set;
	for (int i = 0; i < 2 * N; i++) {
		auto it1 = s[i].begin();
		auto it2 = last_version[i].begin();
		while (it1 != s[i].end() && it2 != last_version[i].end()) {
			if (*it1 != *it2)
				ans = false;
			it1++;
			++it2;
		}
		it2 != last_version[i].end();
		if (it1 == s[i].end() && it2 != last_version[i].end() || it1 != s[i].end() && it2 == last_version[i].end())
			ans = false;
	}
	std::cout << ans << "\n";
}


int main() {
	srand(time(0));
	std::string k = "";
	test1();
	test2();
	while (1);
	return 0;
}