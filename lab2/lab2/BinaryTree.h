#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <queue>

class Node
{
public:
	// Конструкторы (по умолчанию и с параметрами)
	Node();
	Node(int key);
	Node(int key, Node* left, Node* right);
	// Деструктор
	~Node();
	// Получение/изменение ключа узла (целое число)
	int getKey() const;
	void setKey(int key);
	// Получение/изменение потомков узла
	Node* getLeft() const;
	Node* getRight() const;
	void setLeft(Node* left);
	void setRight(Node* right);
private:
	int key;
	Node* left;
	Node* right;

	// Методы для получения ссылок на потомков (для наследников)
	friend class BinaryTree;
	friend class BinarySearchTree;
	Node*& getLeftRef();
	Node*& getRightRef();
};

class BinaryTree
{
public:
	// Итератор для BFS (Breadth-First Search) обхода - ширина дерева
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
		std::queue<Node*> nodeQueue;
	};

	// Константный итератор для BFS (Breadth-First Search) обхода
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
		std::queue<const Node*> nodeQueue;
	};

	// Конструкторы (по умолчанию, копирования, перемещения)
	BinaryTree();
	BinaryTree(const BinaryTree& other);
	BinaryTree(BinaryTree&& other) noexcept;
	// Деструктор
	~BinaryTree();
	// Получение корня дерева
	Node* getRoot() const;
	// Очистка дерева (удаление всех узлов)
	void clear();
	// Удаление поддеревьев узла
	void removeSubtrees(int key);
	void removeSubtrees(Node* node);
	// IsEmpty (возвращает true, если дерево пусто)
	bool isEmpty() const;
	// Получение высоты дерева
	int getHeight() const;
	int getHeight(const Node* node) const;
	// Получение количества узлов дерева
	int getCount() const;
	int getCount(const Node* node) const;
	// Получение минимального/максимального ключа дерева
	virtual int getMinKey() const;
	virtual int getMinKey(const Node* node) const;
	int getMaxKey() const;
	int getMaxKey(const Node* node) const;
	// Добавление узла в дерево (методом случайного выбора поддерева)
	virtual void addNode(int key);
	// Удаление узла из дерева по ключу (возвращает true, если узел был удалён)
	// Поиск узла дерева по ключу
	virtual Node* findNode(int key) const;
	virtual Node* findNode(Node* node, int key) const;
	// Проверка дерева на сбалансированность (возвращает true, если дерево является сбалансированным: высоты правого и левого поддеревьев отличаются не более, чем на единицу, и сами поддеревья также являются сбалансированными)
	bool isBalanced() const;
	bool isBalanced(const Node* node) const;
	// Получение уровня вершины по ключу (возвращает индекс уровня или -1, если вершина не найдена)
	virtual int getLevel(int key) const;
	virtual int getLevel(const Node* node, int key) const;
	// Получение вектора (std::vector<int>), содержащего все ключи дерева по возрастанию (обход вершин производить любым способом)
	virtual std::vector<int> getSortedKeys() const;
	virtual std::vector<int> getSortedKeys(const Node* node) const;
	// Вывод в консоль дерева в горизонтальном виде (самый правый потомок находится на первой строке, самый левый - на нижней)
	void printHorizontal() const;
	void printHorizontal(const Node* node) const;
	// Вывод в консоль дерева по уровням в консоль
	void printByLevels() const;
	void printByLevels(const Node* node) const;
	// Оператор присваивания
	BinaryTree& operator=(const BinaryTree& other);
	// Оператор перемещения
	BinaryTree& operator=(BinaryTree&& other) noexcept;
	// Итераторы для BFS обхода
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
protected:
	// Получение ссылки на корень (для наследников)
	Node*& getRootRef();
private:
	// Вспомогательные методы
	void destroyTree(Node* node);
	Node* copyTree(const Node* node);
	void printHorizontalHelper(const Node* node, int space) const;
	int getHeightHelper(const Node* node) const;
	int getCountHelper(const Node* node) const;
	bool isBalancedHelper(const Node* node) const;
	int getLevelHelper(const Node* node, int key, int currentLevel) const;
	void inOrderTraversal(const Node* node, std::vector<int>& keys) const;
private:
	Node* root;

	static std::random_device rd;
	static std::mt19937 gen;
};