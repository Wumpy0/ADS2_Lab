#pragma once

#include <iostream>
#include <vector>
#include <random>

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

	friend class BinaryTree;
};

class BinaryTree
{
public:
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
	int getMinKey() const;
	int getMinKey(const Node* node) const;
	int getMaxKey() const;
	int getMaxKey(const Node* node) const;
	// Добавление узла в дерево (методом случайного выбора поддерева)
	void addNode(int key);
	// Удаление узла из дерева по ключу (возвращает true, если узел был удалён)
	// Поиск узла дерева по ключу
	Node* findNode(int key) const;
	Node* findNode(Node* node, int key) const;
	// Проверка дерева на сбалансированность (возвращает true, если дерево является сбалансированным: высоты правого и левого поддеревьев отличаются не более, чем на единицу, и сами поддеревья также являются сбалансированными)
	// Получение уровня вершины по ключу (возвращает индекс уровня или -1, если вершина не найдена)
	// Получение вектора (std::vector<int>), содержащего все ключи дерева по возрастанию (обход вершин производить любым способом)
	// Вывод в консоль дерева в горизонтальном виде (самый правый потомок находится на первой строке, самый левый - на нижней)
	void printHorizontal() const;
	void printHorizontal(const Node* node) const;
	// Вывод в консоль дерева по уровням в консоль
	// Оператор присваивания
	// Оператор перемещения
private:
	// Вспомогательные методы
	void destroyTree(Node* node);
	Node* copyTree(const Node* node);
	void printHorizontalHelper(const Node* node, int space) const;
	int getHeightHelper(const Node* node) const;
	int getCountHelper(const Node* node) const;
private:
	Node* root;

	static std::random_device rd;
	static std::mt19937 gen;
};