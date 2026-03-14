#include <iostream>
#include "BinaryTree.h"

int main()
{
    BinaryTree tree;

    tree.addNode(50);
    tree.addNode(30);
    tree.addNode(70);
    tree.addNode(20);
    tree.addNode(40);
    tree.addNode(60);
    tree.addNode(80);

    std::cout << "=== Binary Tree ===" << std::endl;
    std::cout << "Print by levels:" << std::endl;
    tree.printByLevels();

    std::cout << "\nMin key: " << tree.getMinKey() << std::endl;
    std::cout << "Max key: " << tree.getMaxKey() << std::endl;
    std::cout << "Height: " << tree.getHeight() << std::endl;
    std::cout << "Count: " << tree.getCount() << std::endl;

    std::cout << "\n=== BFS Iterator Test (range-based for loop) ===" << std::endl;
    std::cout << "Iterating through BTree (BFS order):" << std::endl;
    for (int key : tree) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== Explicit Iterator Test ===" << std::endl;
    std::cout << "Using explicit iterator:" << std::endl;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== Const Iterator Test ===" << std::endl;
    const BinaryTree& constTree = tree;
    std::cout << "Using const_iterator:" << std::endl;
    for (auto it = constTree.begin(); it != constTree.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== cbegin/cend Test ===" << std::endl;
    std::cout << "Using cbegin/cend:" << std::endl;
    for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}