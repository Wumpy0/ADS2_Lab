#include <iostream>
#include "BinaryTree.h"

int main()
{
    BinaryTree tree;
    for (int i = 0; i < 10; i++) {
        tree.addNode(i);
    }
    tree.removeSubtrees(2);
    tree.printHorizontal();
    try {
        std::cout << tree.getMinKey(tree.findNode(3)) << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    tree.clear();
    try {
        std::cout << tree.getMaxKey();
    }
    catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
        tree.addNode(-13);
        tree.addNode(13);
    }
    std::cout << tree.getMinKey();
    

    return 0;
}