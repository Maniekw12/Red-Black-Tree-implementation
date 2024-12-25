#include <iostream>
#include "RedBlackTree.h"
#include <cassert>
int main() {

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




    return 0;
}
