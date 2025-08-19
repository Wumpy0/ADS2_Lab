#include <iostream>
#include "BinaryTree.h"

int main()
{
    BinaryTree tree;
    for (int i = 0; i < 10; i++) {
        tree.addNode(i);
    }
    tree.printHorizontal();

    return 0;
}