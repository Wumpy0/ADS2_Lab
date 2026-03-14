#include <iostream>
#include "BinarySearchTree.h"

// ============================================================================
// РЕАЛИЗАЦИЯ КЛАССА BINARYSEARCHTREE
// ============================================================================

// Конструкторы (по умолчанию, копирования, перемещения)
BinarySearchTree::BinarySearchTree() : BinaryTree()
{
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other) : BinaryTree(other)
{
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept : BinaryTree(std::move(other))
{
}

// Деструктор
BinarySearchTree::~BinarySearchTree()
{
}

// Получение минимального ключа дерева (имеет другой алгоритм)
int BinarySearchTree::getMinKey() const
{
    if (!getRoot()) {
        throw std::runtime_error("Tree is empty");
    }
    return getMinKey(getRoot());
}

int BinarySearchTree::getMinKey(const Node* node) const
{
    if (!node) {
        throw std::runtime_error("Node is null");
    }

    // В BST минимальный элемент находится в левом углу дерева
    const Node* current = node;
    while (current->getLeft() != nullptr) {
        current = current->getLeft();
    }

    return current->getKey();
}

// Добавление узла в дерево (имеет другой алгоритм)
void BinarySearchTree::addNode(int key)
{
    if (!getRoot()) {
        getRootRef() = new Node(key);
        return;
    }

    Node* current = getRoot();
    while (true) {
        if (key < current->getKey()) {
            if (current->getLeft() == nullptr) {
                current->getLeftRef() = new Node(key);
                return;
            }
            current = current->getLeft();
        }
        else {
            if (current->getRight() == nullptr) {
                current->getRightRef() = new Node(key);
                return;
            }
            current = current->getRight();
        }
    }
}

// Удаление узла из дерева по ключу (возвращает true, если узел был удалён)
bool BinarySearchTree::removeNode(int key)
{
    return removeNode(getRootRef(), key);
}

bool BinarySearchTree::removeNode(Node*& node, int key)
{
    if (node == nullptr) {
        return false;
    }

    if (key < node->getKey()) {
        return removeNode(node->getLeftRef(), key);
    }
    else if (key > node->getKey()) {
        return removeNode(node->getRightRef(), key);
    }
    else {
        // Найден узел для удаления
        // Случай 1: узел без детей (лист)
        if (node->getLeft() == nullptr && node->getRight() == nullptr) {
            delete node;
            node = nullptr;
            return true;
        }
        // Случай 2-3: узел имеет одного или двух потомков (но не двух)
        // Заменяем узел на его ненулевого потомка (если он есть)
        if (node->getLeft() == nullptr || node->getRight() == nullptr) {
            Node* child = (node->getLeft() != nullptr) ? node->getLeft() : node->getRight();
            Node* temp = node;
            node = child;
            delete temp;
            return true;
        }
        // Случай 4: узел имеет двух потомков
        // Находим минимальный узел в правом поддереве (inorder successor)
        Node* minRight = node->getRight();
        while (minRight->getLeft() != nullptr) {
            minRight = minRight->getLeft();
        }

        // Копируем значение successor в текущий узел
        node->setKey(minRight->getKey());

        // Удаляем successor узел из правого поддерева
        return removeNode(node->getRightRef(), minRight->getKey());
    }
}

// Поиск узла дерева по ключу (имеет другой алгоритм)
Node* BinarySearchTree::findNode(int key) const
{
    return findNode(getRoot(), key);
}

Node* BinarySearchTree::findNode(Node* node, int key) const
{
    if (node == nullptr) {
        return nullptr;
    }

    if (key == node->getKey()) {
        return node;
    }
    else if (key < node->getKey()) {
        return findNode(node->getLeft(), key);
    }
    else {
        return findNode(node->getRight(), key);
    }
}

// Получение уровня вершины по ключу (возвращает индекс уровня или -1, если вершина не найдена)
int BinarySearchTree::getLevel(int key) const
{
    return getLevelHelper(getRoot(), key, 0);
}

int BinarySearchTree::getLevel(const Node* node, int key) const
{
    return getLevelHelper(node, key, 0);
}

int BinarySearchTree::getLevelHelper(const Node* node, int key, int currentLevel) const
{
    if (node == nullptr) {
        return -1;
    }

    if (node->getKey() == key) {
        return currentLevel;
    }
    else if (key < node->getKey()) {
        return getLevelHelper(node->getLeft(), key, currentLevel + 1);
    }
    else {
        return getLevelHelper(node->getRight(), key, currentLevel + 1);
    }
}

// Получение вектора (std::vector<int>), содержащего все ключи дерева по возрастанию
std::vector<int> BinarySearchTree::getSortedKeys() const
{
    return getSortedKeys(getRoot());
}

std::vector<int> BinarySearchTree::getSortedKeys(const Node* node) const
{
    std::vector<int> keys;
    inOrderTraversal(node, keys);
    return keys;
}

void BinarySearchTree::inOrderTraversal(const Node* node, std::vector<int>& keys) const
{
    if (node != nullptr) {
        inOrderTraversal(node->getLeft(), keys);
        keys.push_back(node->getKey());
        inOrderTraversal(node->getRight(), keys);
    }
}

// Оператор присваивания
BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other)
{
    BinaryTree::operator=(other);
    return *this;
}

// Оператор перемещения
BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept
{
    BinaryTree::operator=(std::move(other));
    return *this;
}