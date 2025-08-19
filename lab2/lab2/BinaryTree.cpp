#include <iostream>
#include <queue>
#include <random>

#include "BinaryTree.h"

std::random_device BinaryTree::rd;
std::mt19937 BinaryTree::gen(BinaryTree::rd());

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
		destroyTree(node->left);
		destroyTree(node->right);
		delete node;
	}
}
Node* BinaryTree::copyTree(const Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    return new Node(node->key, copyTree(node->left), copyTree(node->right));
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
		if (node->left != nullptr) {
			destroyTree(node->left);
			node->left = nullptr;
		}
		if (node->right != nullptr) {
			destroyTree(node->right);
			node->right = nullptr;
		}
	}
}
void BinaryTree::removeSubtrees(int key) {
	Node* node = findNode(key);
	if (node != nullptr) {
		removeSubtrees(node);
	}
}

// IsEmpty (возвращает true, если дерево пусто)
bool BinaryTree::isEmpty() const {
	return root == nullptr;
}

// Получение высоты дерева
int BinaryTree::getHeight() const {
	return getHeightHelper(root);
}
int BinaryTree::getHeight(const Node* node) const {
	return getHeightHelper(node);
}
int BinaryTree::getHeightHelper(const Node* node) const {
	if (node == nullptr) {
		return 0;
	}
	return 1 + std::max(getHeightHelper(node->left), getHeightHelper(node->right));
}
int BinaryTree::getCount() const {
	return getCountHelper(root);
}
int BinaryTree::getCount(const Node* node) const {
	return getCountHelper(node);
}
int BinaryTree::getCountHelper(const Node* node) const {
	if (node == nullptr) {
		return 0;
	}
	return 1 + getCountHelper(node->left) + getCountHelper(node->right);
}

// Получение минимального/максимального ключа дерева
int BinaryTree::getMinKey() const {
	if (!root) {
		throw std::runtime_error("Tree is empty");
	}
	return getMinKey(root);
}
int BinaryTree::getMinKey(const Node* node) const {
	if (!node) {
		throw std::runtime_error("Node is null");
	}

	int result = node->key;

	if (node->left) {
		result = std::min(result, getMinKey(node->left));
	}
	if (node->right) {
		result = std::min(result, getMinKey(node->right));
	}

	return result;
}
int BinaryTree::getMaxKey() const {
	if (!root) {
		throw std::runtime_error("Tree is empty");
	}
	return getMaxKey(root);
}
int BinaryTree::getMaxKey(const Node* node) const {
	if (!node) {
		throw std::runtime_error("Node is null");
	}

	int result = node->key;

	if (node->left) {
		result = std::max(result, getMaxKey(node->left));
	}
	if (node->right) {
		result = std::max(result, getMaxKey(node->right));
	}

	return result;
}

// Добавление узла в дерево (методом случайного выбора поддерева)
void BinaryTree::addNode(int key) {
	if (!root) {
		root = new Node(key);
		return;
	}

	std::queue<Node*> q;
	q.push(root);

	while (!q.empty()) {
		const std::size_t levelSize = q.size();
		std::vector<Node**> levelFree;
		levelFree.reserve(levelSize * 2);

		for (std::size_t i = 0; i < levelSize; ++i) {
			Node* cur = q.front(); q.pop();

			if (cur->left == nullptr)  levelFree.push_back(&cur->left);
			else                       q.push(cur->left);

			if (cur->right == nullptr) levelFree.push_back(&cur->right);
			else                       q.push(cur->right);
		}

		if (!levelFree.empty()) {
			std::uniform_int_distribution<std::size_t> dist(0, levelFree.size() - 1);
			*levelFree[dist(gen)] = new Node(key);
			return;
		}
	}
}

// Поиск узла дерева по ключу
Node* BinaryTree::findNode(int key) const {
	return findNode(root, key);
}
Node* BinaryTree::findNode(Node* node, int key) const {
	if (node == nullptr || node->key == key) {
		return node;
	}

	Node* leftResult = findNode(node->left, key);
	if (leftResult != nullptr) {
		return leftResult;
	}

	return findNode(node->right, key);
}

// Вывод в консоль дерева в горизонтальном виде (самый правый потомок находится на первой строке, самый левый - на нижней)
void BinaryTree::printHorizontal() const {
	printHorizontalHelper(root, 0);
}
void BinaryTree::printHorizontal(const Node* node) const {
	printHorizontalHelper(node, 0);
}
void BinaryTree::printHorizontalHelper(const Node* node, int space) const {
	const int COUNT = 10;
	if (node == nullptr) {
		return;
	}

	space += COUNT;

	printHorizontalHelper(node->right, space);

	std::cout << std::endl;
	for (int i = COUNT; i < space; i++) {
		std::cout << " ";
	}
	std::cout << node->key << std::endl;

	printHorizontalHelper(node->left, space);
}