#include <iostream>
#include "BinarySearchTree.h"

int main()
{
    BinarySearchTree bst;

    // Test adding nodes
    bst.addNode(50);
    bst.addNode(30);
    bst.addNode(70);
    bst.addNode(20);
    bst.addNode(40);
    bst.addNode(60);
    bst.addNode(80);

    std::cout << "=== Binary Search Tree ===" << std::endl;
    std::cout << "Print by levels:" << std::endl;
    bst.printByLevels();

    std::cout << "\nSorted keys:" << std::endl;
    auto keys = bst.getSortedKeys();
    for (int key : keys) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "\nMin key: " << bst.getMinKey() << std::endl;
    std::cout << "Max key: " << bst.getMaxKey() << std::endl;
    std::cout << "Height: " << bst.getHeight() << std::endl;
    std::cout << "Count: " << bst.getCount() << std::endl;

    std::cout << "\nFind node with key 40:" << std::endl;
    Node* found = bst.findNode(40);
    if (found) {
        std::cout << "Found node with key " << found->getKey() << std::endl;
        std::cout << "Level: " << bst.getLevel(40) << std::endl;
    }

    std::cout << "\nRemove node with key 20:" << std::endl;
    if (bst.removeNode(20)) {
        std::cout << "Node removed successfully" << std::endl;
    }

    std::cout << "\nSorted keys after removal:" << std::endl;
    keys = bst.getSortedKeys();
    for (int key : keys) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    // Test copy constructor
    std::cout << "\n=== Copy Constructor Test ===" << std::endl;
    BinarySearchTree bst2(bst);
    std::cout << "Copied tree:" << std::endl;
    bst2.printByLevels();

    // Test move constructor
    std::cout << "\n=== Move Constructor Test ===" << std::endl;
    BinarySearchTree bst3(std::move(bst2));
    std::cout << "Moved tree:" << std::endl;
    bst3.printByLevels();

    // Test iterators
    std::cout << "\n=== LNR Iterator Test (range-based for loop) ===" << std::endl;
    std::cout << "Iterating through BST (LNR order):" << std::endl;
    for (int key : bst) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== Explicit Iterator Test ===" << std::endl;
    std::cout << "Using explicit iterator:" << std::endl;
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== Const Iterator Test ===" << std::endl;
    const BinarySearchTree& constBst = bst;
    std::cout << "Using const_iterator:" << std::endl;
    for (auto it = constBst.begin(); it != constBst.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== cbegin/cend Test ===" << std::endl;
    std::cout << "Using cbegin/cend:" << std::endl;
    for (auto it = bst.cbegin(); it != bst.cend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}