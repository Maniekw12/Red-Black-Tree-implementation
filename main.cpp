#include <cassert>
#include <iostream>
#include "RedBlackTree.h" // lub odpowiednia nazwa pliku z Twoją klasą

void test_insert() {
    RedBlackTree<int> tree;
    // Wstaw kilka elementów
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);


    assert(tree.remove(5));
    assert(tree.remove(10));
    assert(tree.remove(15));

    assert(tree.empty());

    std::cout << "test_insert passed!\n";
}

void test_remove() {
    RedBlackTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    tree.printTree();
    bool removed = tree.remove(2);
    assert(removed);

    removed = tree.remove(99);
    assert(!removed);

    assert(tree.remove(1));
    assert(tree.remove(3));
    assert(tree.empty());

    std::cout << "test_remove passed!\n";
}

void test_root_is_black() {
    RedBlackTree<int> tree;
    // Wstawiamy kilka elementów
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);


    Node<int>* root = tree.getRoot();
    assert(root != nullptr );
    assert(root->color == BLACK);

    tree.remove(30);
    root = tree.getRoot();
    assert(root != nullptr);
    assert(root->color == BLACK);

    std::cout << "test_root_is_black passed!\n";
}

int main() {
    test_insert();
    test_remove();
    test_root_is_black();

    std::cout << "\nAll tests passed!\n";
    return 0;
}
