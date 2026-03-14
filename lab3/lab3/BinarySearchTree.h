#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include "../../lab2/lab2/BinaryTree.h"

class BinarySearchTree : public BinaryTree
{
public:
	// Итератор для LNR (Left-Node-Right) обхода
	class iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = int;
		using pointer = int*;
		using reference = int&;
		using iterator_category = std::forward_iterator_tag;

		iterator(Node* root);
		iterator();

		int operator*() const;
		iterator& operator++();
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;

	private:
		Node* current;
		std::stack<Node*> nodeStack;

		void pushLeft(Node* node);
	};

	// Константный итератор для LNR обхода
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = int;
		using pointer = const int*;
		using reference = const int&;
		using iterator_category = std::forward_iterator_tag;

		const_iterator(const Node* root);
		const_iterator();

		int operator*() const;
		const_iterator& operator++();
		bool operator==(const const_iterator& other) const;
		bool operator!=(const const_iterator& other) const;

	private:
		const Node* current;
		std::stack<const Node*> nodeStack;

		void pushLeft(const Node* node);
	};

	// Конструкторы (по умолчанию, копирования, перемещения)
	BinarySearchTree();
	BinarySearchTree(const BinarySearchTree& other);
	BinarySearchTree(BinarySearchTree&& other) noexcept;
	// Деструктор
	~BinarySearchTree();
	// Получение минимального ключа дерева (имеет другой алгоритм)
	int getMinKey() const override;
	int getMinKey(const Node* node) const override;
	// Добавление узла в дерево (имеет другой алгоритм)
	void addNode(int key) override;
	// Удаление узла из дерева по ключу (возвращает true, если узел был удалён) (имеет другой алгоритм)
	bool removeNode(int key);
	// Поиск узла дерева по ключу (имеет другой алгоритм)
	Node* findNode(int key) const override;
	Node* findNode(Node* node, int key) const override;
	// Получение уровня вершины по ключу (возвращает индекс уровня или -1, если вершина не найдена) (имеет другой алгоритм)
	int getLevel(int key) const override;
	int getLevel(const Node* node, int key) const override;
	// Получение вектора (std::vector<int>), содержащего все ключи дерева по возрастанию (имеет другой алгоритм)
	std::vector<int> getSortedKeys() const override;
	std::vector<int> getSortedKeys(const Node* node) const override;
	// Оператор присваивания
	BinarySearchTree& operator=(const BinarySearchTree& other);
	// Оператор перемещения
	BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;
	// Итераторы для LNR обхода
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
private:
	// Вспомогательные методы
	bool removeNode(Node*& node, int key);
	void inOrderTraversal(const Node* node, std::vector<int>& keys) const;
	int getLevelHelper(const Node* node, int key, int currentLevel) const;
};