#ifndef PERSISTENT_SET
#define PERSISTENT_SET

#include <iostream>
#include <memory>
#include <vector>

struct persistent_set
{
	// Вы можете определить этот тайпдеф по вашему усмотрению.
	typedef int value_type;

	// Bidirectional iterator.
	struct iterator;

	// Создает пустой persistent_set.
	persistent_set();

	// Создает копию указанного persistent_set-а.
	persistent_set(persistent_set const&);

	// Изменяет this так, чтобы он содержал те же элементы, что и rhs.
	// Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
	persistent_set& operator=(persistent_set const& rhs);

	// Деструктор. Вызывается при удалении объектов persistent_set.
	// Инвалидирует все итераторы ссылающиеся на элементы этого persistent_set
	// (включая итераторы ссылающиеся на элементы следующие за последними).
	~persistent_set();

	// Поиск элемента.
	// Возвращает итератор на элемент найденный элемент, либо end().
	iterator find(value_type val);

	// Вставка элемента.
	// 1. Если такой ключ уже присутствует, вставка не производиться, возвращается итератор
	//    на уже присутствующий элемент и false.
	// 2. Если такого ключа ещё нет, производиться вставка, возвращается итератор на созданный
	//    элемент и true.
	// Если вставка произведена, инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
	std::pair<iterator, bool> insert(value_type val);

	// Удаление элемента.
	// Инвалидирует все итераторы, принадлежащие persistent_set'у this, включая end().
	void erase(iterator);

	void erase(value_type);

	// Возващает итератор на элемент с минимальный ключом.
	iterator begin() const;
	// Возващает итератор на элемент следующий за элементом с максимальным ключом.
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
		// Элемент на который сейчас ссылается итератор.
		// Разыменование итератора end() неопределено.
		// Разыменование невалидного итератора неопределено.
		value_type const& operator*() const;

		// Переход к элементу со следующим по величине ключом.
		// Инкремент итератора end() неопределен.
		// Инкремент невалидного итератора неопределен.
		iterator& operator++();
		iterator operator++(int);

		// Переход к элементу со следующим по величине ключом.
		// Декремент итератора begin() неопределен.
		// Декремент невалидного итератора неопределен.
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
// Сравнение. Итераторы считаются эквивалентными если одни ссылаются на один и тот же элемент.
// Сравнение с невалидным итератором не определено.
// Сравнение итераторов двух разных контейнеров не определено.
bool operator==(persistent_set::iterator, persistent_set::iterator);
bool operator!=(persistent_set::iterator, persistent_set::iterator);

#endif