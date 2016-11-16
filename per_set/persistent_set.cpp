#include "persistent_set.h"
#include <algorithm>

persistent_set::persistent_set() {}

persistent_set::persistent_set(persistent_set const& other) : root(other.root) {}

persistent_set& persistent_set::operator=(persistent_set const& other) {
	root = other.root;
	return (*this);
}

persistent_set::~persistent_set() {}

persistent_set::iterator persistent_set::find(persistent_set::value_type val) {
	if (!bool(root))
		return end();
	auto tmp = root.get();
	std::vector <node*> s;
	while (tmp->value != val) {
		s.push_back(tmp);
		if (val < tmp->value && !bool(tmp->left) || val > tmp->value && !bool(tmp->right)) {
			return end();
		}
		if (val < tmp->value)
			tmp = tmp->left.get();
		else
			tmp = tmp->right.get();
	}
	s.push_back(tmp);
	persistent_set::iterator ans;
	ans.root = root.get();
	ans.path = s;
	return ans;
}

std::pair<persistent_set::iterator, bool> persistent_set::insert(persistent_set::value_type val) {
	auto ans = find(val);
	if (ans != end())
		return std::make_pair(ans, false);
	if (!bool(root)) {
		root = std::make_shared <node> (node(val));
		ans.path.push_back(root.get());
		return std::make_pair(ans, true);
	}
	auto tmp = root.get();
	std::vector <node*> s;
	while (val < tmp->value && bool(tmp->left) || val > tmp->value && bool(tmp->right)) {
		s.push_back(tmp);
		if (val < tmp->value)
			tmp = tmp->left.get();
		else
			tmp = tmp->right.get();
	}
	s.push_back(tmp);
	auto tmp2 = std::make_shared <node>(node(val));
	while (!s.empty()) {
		tmp = s[s.size() - 1];
		s.pop_back();
		if (tmp2.get()->value < tmp->value)
			tmp2 = std::make_shared <node>(tmp->value, tmp2, tmp->right);
		else
			tmp2 = std::make_shared <node>(tmp->value, tmp->left, tmp2);
	}
	root = tmp2;
	return std::make_pair(find(val), true);
}

void persistent_set::erase(persistent_set::iterator it) {
	if (it.path.empty())
		return;
	node* tmp;
	auto last = it.top();
	auto del = it.top();
	auto tmp2 = it.top()->left;
	if (bool(it.top()->right)) {
		it.path.push_back(it.top()->right.get());
		while (bool(it.top()->left))
			it.path.push_back(it.top()->left.get());
		tmp2 = std::make_shared <node>(it.top()->value, tmp2, it.top()->right);
		it.path.pop_back();
	}
	while (!it.path.empty()) {
		tmp = it.top();
		it.path.pop_back();
		if (tmp == del)
			continue;
		if (last->value < tmp->value)
			tmp2 = std::make_shared <node>(tmp->value, tmp2, tmp->right);
		else
			tmp2 = std::make_shared <node>(tmp->value, tmp->left, tmp2);
		last = tmp2.get();
	}
	root = tmp2;
}

void persistent_set::erase(persistent_set::value_type val) {
	erase(find(val));
}

persistent_set::iterator persistent_set::begin() const{
	iterator ans;
	if (!bool(root)) {
		ans.root = NULL;
		return ans;
	}
	ans.root = root.get();
	node* tmp = root.get();
	while (bool(tmp->left)) {
		ans.path.push_back(tmp);
		tmp = tmp->left.get();
	}
	ans.path.push_back(tmp);
	return ans;
}

persistent_set::iterator persistent_set::end() const {
	iterator ans;
	if (bool(root))
		ans.root = root.get();
	else
		ans.root = NULL;
	return ans;
}

persistent_set::node::node() {}

persistent_set::node::node(value_type value) : value(value) {}

persistent_set::node::node(value_type value, std::shared_ptr <node> left, std::shared_ptr <node> right) : 
	value(value), left(left), right(right) {}

persistent_set::node* persistent_set::iterator::top() {
	return path[path.size() - 1];
}

persistent_set::value_type const& persistent_set::iterator::operator*() const {
	if (!path.empty())
		return value_type(path[path.size() - 1]->value);
	else
		throw ":(";
}

persistent_set::iterator& persistent_set::iterator::operator++() {
	if (path.empty()) {
		path.push_back(root);
		while (bool(path[path.size() - 1]->left))
			path.push_back(path[path.size() - 1]->left.get());
		return (*this);
	}
	if (bool(path[path.size() - 1]->right)) {
		path.push_back(path[path.size() - 1]->right.get());
		while (bool(path[path.size() - 1]->left))
			path.push_back(path[path.size() - 1]->left.get());
	}
	else {
		node* last = path[path.size() - 1];
		path.pop_back();
		while (!path.empty() && last == path[path.size() - 1]->right.get()) {
			last = path[path.size() - 1];
			path.pop_back();
		}
	}
	return (*this);
}

persistent_set::iterator persistent_set::iterator::operator++(int) {
	persistent_set::iterator it = (*this);
	++(*this);
	return it;
}

persistent_set::iterator& persistent_set::iterator::operator--() {
	if (path.empty()) {
		path.pop_back();
		path.push_back(root);
		while (bool(path[path.size() - 1]->right))
			path.push_back(path[path.size() - 1]->right.get());
		return (*this);
	}
	if (bool(path[path.size() - 1]->left)) {
		path.push_back(path[path.size() - 1]->left.get());
		while (bool(path[path.size() - 1]->right))
			path.push_back(path[path.size() - 1]->right.get());
	}
	else {
		node* last = path[path.size() - 1];
		path.pop_back();
		while (!path.empty() && last == path[path.size() - 1]->left.get()) {
			last = path[path.size() - 1];
			path.pop_back();
		}
	}
	return (*this);
}
persistent_set::iterator persistent_set::iterator::operator--(int) {
	persistent_set::iterator it = (*this);
	--(*this);
	return it;
}

bool operator == (persistent_set::iterator a, persistent_set::iterator b) {
	return (a.root == b.root) && (a.path == b.path);
}

bool operator != (persistent_set::iterator a, persistent_set::iterator b) {
	return !(a == b);
}
