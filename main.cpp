#include <cassert>
#include <iostream>
#include "RedBlackTree.h"

// Funkcje pomocnicze do sprawdzania własności drzewa

template <typename T>
bool checkRedProperty(const Node<T>* node) {
    if (!node || node->value == T()) return true; // Obsługa węzłów NIL

    if (node->color == RED) {
        if (node->left && node->left->color == RED) return false;
        if (node->right && node->right->color == RED) return false;
    }

    return checkRedProperty(node->left) && checkRedProperty(node->right);
}

template <typename T>
int blackHeight(const Node<T>* node) {
    if (!node || node->value == T()) {
        return 1;
    }

    int leftBH = blackHeight(node->left);
    int rightBH = blackHeight(node->right);

    if (leftBH == -1 || rightBH == -1 || leftBH != rightBH) {
        return -1;
    }

    return leftBH + (node->color == BLACK ? 1 : 0);
}

template <typename T>
void assertRedBlackTreeProperties(const RedBlackTree<T>& rbt) {
    if (!rbt.empty()) {
        assert(rbt.get_root_color() == BLACK);
    }

    assert(checkRedProperty(rbt.getRoot()));

    int bh = blackHeight(rbt.getRoot());
    assert(bh != -1);
}

//---------------------------------------------- wlasciwe Testy ----------------------------------------------
void test_empty_tree() {
    RedBlackTree<int> tree;
    assert(tree.empty());
    assertRedBlackTreeProperties(tree);
    std::cout << "test empty tree passed!\n";
}

void test_insert() {
    RedBlackTree<int> tree;
    tree.insert(10);
    assert(!tree.empty());
    assertRedBlackTreeProperties(tree);
    std::cout << "test single insert passed!\n";

    for (int i : {10, 20, 30, 15, 25, 5, 87, 45, 92,
                  13, 67, 89, 33, 71, 42, 8, 95, 54,
                  27, 61, 36, 78, 19, 83, 50, 16, 97,
                  39, 64, 22, 74, 11, 58, 91, 47, 85}) {
        tree.insert(i);
        assertRedBlackTreeProperties(tree);
    }
    std::cout << "test multiple inserts passed!\n";
}

void test_remove(){
    RedBlackTree<int> tree;
    assert(!tree.remove(10));
    std::cout << "test removal from empty tree passed!\n";

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.clear();
    assert(!tree.remove(99));
    assertRedBlackTreeProperties(tree);
    std::cout << "test removal of nonexistent elements passed!\n";
    for (int i : {10, 20, 30, 15, 25, 5,
                  87, 45, 92, 13, 67, 89,
                  33, 71, 42, 8, 95, 54, 27,
                  61, 36, 78, 19, 83, 50, 16,
                  97, 39, 64, 22, 74, 11, 58,
                  91, 47, 85}) {
        tree.insert(i);
    }
    for (int i : {909090, 89,67,87,30,20,15}) {
        if(i == 909090){
            assert(!tree.remove(i));
        } else{
            assert(tree.remove(i));
        }
        assertRedBlackTreeProperties(tree);
    }
    std::cout << "test removal of existing elements passed!\n";
}

void test_clear() {
    RedBlackTree<int> tree;
    for (int i : {10, 20, 30, 15, 25, 5}) {
        tree.insert(i);
    }

    tree.clear();
    assert(tree.empty());
    assertRedBlackTreeProperties(tree);
    std::cout << "test clear passed!\n";
}



int main() {
    test_empty_tree();
    test_insert();
    test_remove();
    test_clear();

    std::cout << "\nAll tests passed!\n";
    return 0;
}
