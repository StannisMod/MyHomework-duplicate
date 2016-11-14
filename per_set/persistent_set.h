#ifndef PERSISTENT_SET
#define PERSISTENT_SET

#include <iostream>
#include <memory>
#include <vector>

struct persistent_set
{
	// �� ������ ���������� ���� ������� �� ������ ����������.
	typedef int value_type;

	// Bidirectional iterator.
	struct iterator;

	// ������� ������ persistent_set.
	persistent_set();

	// ������� ����� ���������� persistent_set-�.
	persistent_set(persistent_set const&);

	// �������� this ���, ����� �� �������� �� �� ��������, ��� � rhs.
	// ������������ ��� ���������, ������������� persistent_set'� this, ������� end().
	persistent_set& operator=(persistent_set const& rhs);

	// ����������. ���������� ��� �������� �������� persistent_set.
	// ������������ ��� ��������� ����������� �� �������� ����� persistent_set
	// (������� ��������� ����������� �� �������� ��������� �� ����������).
	~persistent_set();

	// ����� ��������.
	// ���������� �������� �� ������� ��������� �������, ���� end().
	iterator find(value_type val);

	// ������� ��������.
	// 1. ���� ����� ���� ��� ������������, ������� �� �������������, ������������ ��������
	//    �� ��� �������������� ������� � false.
	// 2. ���� ������ ����� ��� ���, ������������� �������, ������������ �������� �� ���������
	//    ������� � true.
	// ���� ������� �����������, ������������ ��� ���������, ������������� persistent_set'� this, ������� end().
	std::pair<iterator, bool> insert(value_type val);

	// �������� ��������.
	// ������������ ��� ���������, ������������� persistent_set'� this, ������� end().
	void erase(iterator);

	void erase(value_type);

	// ��������� �������� �� ������� � ����������� ������.
	iterator begin() const;
	// ��������� �������� �� ������� ��������� �� ��������� � ������������ ������.
	iterator end() const;

	struct node {

		node();
		node(value_type);
		node(value_type, std::shared_ptr <node>, std::shared_ptr <node>);

		friend struct persistent_set;
		friend struct persistent_set::iterator;
	private:
		value_type value;
		std::shared_ptr <node> left, right;
	};

private:
	std::shared_ptr <node> root;
};





struct persistent_set::iterator
	{
		// ������� �� ������� ������ ��������� ��������.
		// ������������� ��������� end() ������������.
		// ������������� ����������� ��������� ������������.
		value_type const& operator*() const;

		// ������� � �������� �� ��������� �� �������� ������.
		// ��������� ��������� end() �����������.
		// ��������� ����������� ��������� �����������.
		iterator& operator++();
		iterator operator++(int);

		// ������� � �������� �� ��������� �� �������� ������.
		// ��������� ��������� begin() �����������.
		// ��������� ����������� ��������� �����������.
		iterator& operator--();
		iterator operator--(int);

		friend bool operator ==(iterator, iterator);
		friend bool operator !=(iterator, iterator);

		friend struct persistent_set;
	private:
		std::vector <node*> path;
		node* root;
		node* top();
	};
// ���������. ��������� ��������� �������������� ���� ���� ��������� �� ���� � ��� �� �������.
// ��������� � ���������� ���������� �� ����������.
// ��������� ���������� ���� ������ ����������� �� ����������.
bool operator==(persistent_set::iterator, persistent_set::iterator);
bool operator!=(persistent_set::iterator, persistent_set::iterator);

#endif