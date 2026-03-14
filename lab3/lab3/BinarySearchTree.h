#pragma once

#include <iostream>
#include <vector>
#include "../../lab2/lab2/BinaryTree.h"

class BinarySearchTree : public BinaryTree
{
public:
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
private:
	// Вспомогательные методы
	bool removeNode(Node*& node, int key);
	void inOrderTraversal(const Node* node, std::vector<int>& keys) const;
	int getLevelHelper(const Node* node, int key, int currentLevel) const;
};