#include <iostream>
#include "RedBlackTree.h"
#include <cassert>
int main() {
    // Tutaj beda testy //

    RedBlackTree<int> tree;
    assert(tree.empty());
    tree.insert(10);
    assert(!tree.empty());

    tree.insert(15);
    tree.insert(20);


    tree.insert(6);
    tree.insert(5);
    tree.insert(7);
    tree.insert(8);


    tree.printTree();
    tree.remove(8);
    std::cout<<"------------------"<<std::endl;
    tree.printTree();
    return 0;
}
