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
	std::stack <node*> s;
	while (tmp->value != val) {
		s.push(tmp);
		if (val < tmp->value && !bool(tmp->left) || val > tmp->value && !bool(tmp->right)) {
			return end();
		}
		if (val < tmp->value)
			tmp = tmp->left.get();
		else
			tmp = tmp->right.get();
	}
	s.push(tmp);
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
		ans.path.push(root.get());
		return std::make_pair(ans, true);
	}
	auto tmp = root.get();
	std::stack <node*> s;
	while (val < tmp->value && bool(tmp->left) || val > tmp->value && bool(tmp->right)) {
		s.push(tmp);
		if (val < tmp->value)
			tmp = tmp->left.get();
		else
			tmp = tmp->right.get();
	}
	s.push(tmp);
	auto tmp2 = std::make_shared <node>(node(val));
	while (!s.empty()) {
		tmp = s.top();
		s.pop();
		if (tmp2.get()->value < tmp->value)
			tmp2 = std::make_shared <node>(tmp->value, tmp2, tmp->right);
		else
			tmp2 = std::make_shared <node>(tmp->value, tmp->left, tmp2);
	}
	root = tmp2;
	return std::make_pair(find(val), true);
}

void persistent_set::erase(persistent_set::iterator it) {
	if (it == end())
		return;
	node* tmp;
	value_type last_val = *it;
	value_type del = *it;
	auto tmp2 = it.path.top()->left;
	if (bool(it.path.top()->right)) {
		it.path.push(it.path.top()->right.get());
		while (bool(it.path.top()->left))
			it.path.push(it.path.top()->left.get());
		tmp2 = std::make_shared <node>(it.path.top()->value, tmp2, it.path.top()->right);
		it.path.pop();
	}
	while (!it.path.empty()) {
		tmp = it.path.top();
		it.path.pop();
		if (tmp->value == del)
			continue;
		if (last_val < tmp->value)
			tmp2 = std::make_shared <node>(tmp->value, tmp2, tmp->right);
		else
			tmp2 = std::make_shared <node>(tmp->value, tmp->left, tmp2);
		last_val = tmp2.get()->value;
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
		ans.path.push(tmp);
		tmp = tmp->left.get();
	}
	ans.path.push(tmp);
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

persistent_set::value_type const& persistent_set::iterator::operator*() const {
	return value_type(path.top()->value);
}

persistent_set::iterator& persistent_set::iterator::operator++() {
	if (path.empty()) {
		path.push(root);
		while (bool(path.top()->left))
			path.push(path.top()->left.get());
		return (*this);
	}
	if (bool(path.top()->right)) {
		path.push(path.top()->right.get());
		while (bool(path.top()->left))
			path.push(path.top()->left.get());
	}
	else {
		node* last = path.top();
		path.pop();
		while (!path.empty() && last == path.top()->right.get()) {
			last = path.top();
			path.pop();
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
		path.pop();
		path.push(root);
		while (bool(path.top()->right))
			path.push(path.top()->right.get());
		return (*this);
	}
	if (bool(path.top()->left)) {
		path.push(path.top()->left.get());
		while (bool(path.top()->right))
			path.push(path.top()->right.get());
	}
	else {
		node* last = path.top();
		path.pop();
		while (!path.empty() && last == path.top()->left.get()) {
			last = path.top();
			path.pop();
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