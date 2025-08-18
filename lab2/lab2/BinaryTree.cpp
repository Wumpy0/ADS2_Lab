#include <iostream>
#include "BinaryTree.h"

// ============================================================================
// РЕАЛИЗАЦИЯ КЛАССА NODE
// ============================================================================

// Конструкторы (по умолчанию и с параметрами)
Node::Node() :
	key(0),
	left(nullptr),
	right(nullptr) {
}
Node::Node(int key) :
	key(key),
	left(nullptr),
	right(nullptr) {
}
Node::Node(int key, Node* left, Node* right) :
	key(key),
	left(left),
	right(right) {
}

// Деструктор
Node::~Node() {}

// Получение/изменение ключа узла (целое число)
int Node::getKey() const {
	return key;
}
void Node::setKey(int key) {
	this->key = key;
}

// Получение/изменение потомков узла
Node* Node::getLeft() const {
	return left;
}
Node* Node::getRight() const {
	return right;
}
void Node::setLeft(Node* left) {
	this->left = left;
}
void Node::setRight(Node* right) {
	this->right = right;
}

// ============================================================================
// РЕАЛИЗАЦИЯ КЛАССА BINARYTREE
// ============================================================================

// Конструкторы (по умолчанию, копирования, перемещения)
BinaryTree::BinaryTree() : root(nullptr) {}

BinaryTree::BinaryTree(const BinaryTree& other) : root(nullptr) {
	if (other.root != nullptr) {
		root = copyTree(other.root);
	}
}
BinaryTree::BinaryTree(BinaryTree&& other) noexcept : root(other.root) {
	other.root = nullptr;
}

// Деструктор
BinaryTree::~BinaryTree() {
	clear();
}

// Вспомогательные методы
void BinaryTree::destroyTree(Node* node) {
	if (node != nullptr) {
		destroyTree(node->getLeft());
		destroyTree(node->getRight());
		delete node;
	}
}
Node* BinaryTree::copyTree(const Node* node) {
	if (node == nullptr) {
		return nullptr;
	}
	return new Node(node->getKey(), copyTree(node->getLeft()), copyTree(node->getRight()));
}

// Получение корня дерева
Node* BinaryTree::getRoot() const {
	return root;
}

// Очистка дерева (удаление всех узлов)
void BinaryTree::clear() {
	destroyTree(root);
	root = nullptr;
}

// Удаление поддеревьев узла
void BinaryTree::removeSubtrees(Node* node) {
	if (node != nullptr) {
		if (node->getLeft() != nullptr) {
			destroyTree(node->getLeft());
			node->setLeft(nullptr);
		}
		if (node->getRight() != nullptr) {
			destroyTree(node->getRight());
			node->setRight(nullptr);
		}
	}
}

// IsEmpty (возвращает true, если дерево пусто)
bool BinaryTree::isEmpty() const {
	return root == nullptr;
}
