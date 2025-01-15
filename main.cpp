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
    // Porównanie wartości z domyślną - sprawdza czy dotarliśmy do NIL
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
        // Korzeń w drzewie czerwono-czarnym powinien być czarny (o ile istnieje)
        assert(rbt.get_root_color() == BLACK);
    }
    // Sprawdzamy własność, że żadne dwa czerwone węzły nie są sąsiadami
    assert(checkRedProperty(rbt.getRoot()));
    // Sprawdzamy równą liczbę czarnych węzłów na każdej ścieżce od korzenia do liścia
    int bh = blackHeight(rbt.getRoot());
    assert(bh != -1);
}

//---------------------------------------------- wlasciwe Testy ----------------------------------------------
void test_empty_tree() {
    RedBlackTree<int> tree;
    assert(tree.empty());
    assertRedBlackTreeProperties(tree);
    std::cout << "Test empty tree passed!\n";
}

void test_insert() {
    RedBlackTree<int> tree;
    tree.insert(10);
    assert(!tree.empty());
    assertRedBlackTreeProperties(tree);
    std::cout << "Test single insert passed!\n";

    for (int i : {10, 20, 30, 15, 25, 5, 87, 45, 92,
                  13, 67, 89, 33, 71, 42, 8, 95, 54,
                  27, 61, 36, 78, 19, 83, 50, 16, 97,
                  39, 64, 22, 74, 11, 58, 91, 47, 85})
    {
        tree.insert(i);
        assertRedBlackTreeProperties(tree);
    }
    std::cout << "Test multiple inserts passed!\n";
}

void test_remove(){
    RedBlackTree<int> tree;
    // Próba usunięcia z pustego drzewa
    assert(!tree.remove(10));
    std::cout << "Test removal from empty tree passed!\n";

    // Wstawiamy i usuwamy kilka elementów
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.clear();
    assert(!tree.remove(99));    // Elementu nie ma w drzewie
    assertRedBlackTreeProperties(tree);
    std::cout << "Test removal of nonexistent elements passed!\n";

    // Wstawiamy ponownie elementy
    for (int i : {10, 20, 30, 15, 25, 5,
                  87, 45, 92, 13, 67, 89,
                  33, 71, 42, 8, 95, 54, 27,
                  61, 36, 78, 19, 83, 50, 16,
                  97, 39, 64, 22, 74, 11, 58,
                  91, 47, 85})
    {
        tree.insert(i);
    }
    for (int i : {909090, 89, 67, 87, 30, 20, 15}) {
        if(i == 909090){
            assert(!tree.remove(i));
        } else{
            assert(tree.remove(i));
        }
        assertRedBlackTreeProperties(tree);
    }
    std::cout << "Test removal of existing elements passed!\n";
}

void test_clear() {
    RedBlackTree<int> tree;
    for (int i : {10, 20, 30, 15, 25, 5}) {
        tree.insert(i);
    }

    tree.clear();
    assert(tree.empty());
    assertRedBlackTreeProperties(tree);
    std::cout << "Test clear passed!\n";
}


void test_remove_scenarios() {
    // 1) Usunięcie korzenia w drzewie z jednym elementem
    {
        RedBlackTree<int> singleNodeTree;
        singleNodeTree.insert(42);

        assert(!singleNodeTree.empty());
        assertRedBlackTreeProperties(singleNodeTree);
        bool removed = singleNodeTree.remove(42);


        assert(removed);
        assert(singleNodeTree.empty());
        assertRedBlackTreeProperties(singleNodeTree);
    }

    // 2) Usunięcie czerwonego liścia
    {
        RedBlackTree<int> tree;
        tree.insert(10);
        tree.insert(20);
        tree.insert(15);




        assertRedBlackTreeProperties(tree);

        bool removed = tree.remove(15);




        assert(removed);
        assertRedBlackTreeProperties(tree);
        std::cout << "Removal of red leaf passed!\n";
    }

    // 3) Usunięcie węzła czerwonego
    {
        RedBlackTree<int> tree;
        tree.insert(10);
        tree.insert(20);


        bool removed = tree.remove(20);


        assert(removed);
        assertRedBlackTreeProperties(tree);
        std::cout << "Removal of black leaf passed!\n";
    }

    // 4) Usunięcie węzła posiadającego dwoje dzieci
    {
        RedBlackTree<int> tree;
        tree.insert(15);
        tree.insert(10);
        tree.insert(20);
        tree.insert(8);
        tree.insert(12);
        tree.insert(17);
        tree.insert(25);


        bool removed = tree.remove(15);

        assert(removed);
        assertRedBlackTreeProperties(tree);
        std::cout << "Removal of node with two children passed!\n";
    }

    // 5) Usunięcie węzła z jednym dzieckiem
    {
        RedBlackTree<int> tree;
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        tree.insert(7);

        bool removed = tree.remove(5);

        assert(removed);
        assertRedBlackTreeProperties(tree);
        std::cout << "Removal of node with one child passed!\n";
    }

    // 6) Usunięcie korzenia, gdy korzeń ma dwoje dzieci
    {
        RedBlackTree<int> tree;
        tree.insert(20);
        tree.insert(10);
        tree.insert(30);
        tree.insert(5);
        tree.insert(15);
        tree.insert(25);
        tree.insert(35);

        bool removed = tree.remove(20);

        assert(removed);
        assertRedBlackTreeProperties(tree);
        std::cout << "Removal of root with two children passed!\n";
    }


    // 7) Usunięcie czarnego węzła z dwoma czerwonymi dziećmi
    {
        RedBlackTree<int> tree;
        tree.insert(10);
        tree.insert(5);
        tree.insert(20);
        tree.insert(15);
        tree.insert(25);


        bool removed = tree.remove(20);

        assert(removed);
        assertRedBlackTreeProperties(tree);
        std::cout << "Removal of red node with two black children passed!\n";
    }



}

int main() {
    test_empty_tree();
    test_insert();
    test_remove();
    test_clear();
    test_remove_scenarios();

    std::cout << "All tests passed!\n";
    return 0;
}
