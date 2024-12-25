#include <iostream>
#include <utility> // dla std::swap
#include <string>
enum Color { RED, BLACK };

template <typename T>
struct Node {
    T value;
    Node *left;
    Node *right;
    Node *parent;
    Color color;

    Node(const T& val)
            : value(val),
            left(nullptr),
            right(nullptr),
            parent(nullptr),
            color(RED)
    {}
};

template <typename T>
class RedBlackTree {
    Node<T>* root;

    void destroySubtree(Node<T>* node);
    void rotateLeft(Node<T>* node);
    void rotateRight(Node<T>* node);

    void transplant(Node<T> *root, Node<T>* u, Node<T>* v);

    void fixInsert(Node<T>* node);

    Node<T>* minValueNode(Node<T>* node);

public:
    RedBlackTree() : root(nullptr) {}

    // Rekurencyjne usuwanie całego drzewa w destruktorze
    ~RedBlackTree() {
        destroySubtree(root);
        root = nullptr;
    }

    // Metoda do wyczyszczenia drzewa
    void clear() {
        destroySubtree(root);
        root = nullptr;
    }

    bool empty() const {
        return (root == nullptr);
    }

    // Szukanie węzła z określoną wartością
    Node<T>* search(Node<T>* node, const T& item) const {
        if (node == nullptr || node->value == item) {
            return node;
        }
        else if (item < node->value) {
            return search(node->left, item);
        }
        else {
            return search(node->right, item);
        }
    }

    // Wstawianie nowej wartości do drzewa
    void insert(const T& key);

    void remove(T key);

    void printTree()
    {
        if (root == nullptr)
            std::cout << "Tree is empty." << std::endl;
        else {
            std::cout << "Red-Black Tree:" << std::endl;
            printHelper(root, "", true);
        }
    }
    void printHelper(Node<T>* node, const std::string& prefix = "", bool isLast = true)
    {
        if(node == nullptr){
            return;
        }

        std::cout <<prefix;

        std::cout<<"-----";

        std::string color;
        if(node->color == RED){
            color = "RED";
        } else{
            color = "BLACK";
        }
        std::cout <<node->value <<" (" + color + ") "<<std::endl;

        std::string newPrefix = prefix + (isLast ? "    " : "|   ");

        if (node->left || node->right) {
            printHelper(node->left,  newPrefix, node->right == nullptr);
            printHelper(node->right, newPrefix, true);
        }
    }


};

template <typename T>
void RedBlackTree<T>::destroySubtree(Node<T>* node) {
    if (!node) return;
    destroySubtree(node->left);
    destroySubtree(node->right);
    delete node;
}

template <typename T>
void RedBlackTree<T>::rotateLeft(Node<T>* node) {
    Node<T>* child = node->right;
    node->right = child->left;
    if (child->left != nullptr) {
        child->left->parent = node;
    }
    child->parent = node->parent;

    if (node->parent == nullptr) {
        root = child;
    }
    else if (node == node->parent->left) {
        node->parent->left = child;
    }
    else {
        node->parent->right = child;
    }
    child->left = node;
    node->parent = child;
}

template <typename T>
void RedBlackTree<T>::rotateRight(Node<T>* node) {
    Node<T>* child = node->left;
    node->left = child->right;
    if (child->right != nullptr) {
        child->right->parent = node;
    }
    child->parent = node->parent;

    if (node->parent == nullptr) {
        root = child;
    }
    else if (node == node->parent->right) {
        node->parent->right = child;
    }
    else {
        node->parent->left = child;
    }
    child->right = node;
    node->parent = child;
}

template <typename T>
void RedBlackTree<T>::insert(const T& key) {
    // Tworzymy nowy węzeł, kolor domyślny to RED
    Node<T>* node = new Node<T>(key);

    Node<T>* parent = nullptr;
    Node<T>* current = root;

    // Znajdowanie odpowiedniego miejsca w drzewie
    while (current != nullptr) {
        parent = current;
        if (node->value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    node->parent = parent;

    // Jeżeli drzewo jest puste
    if (parent == nullptr) {
        root = node;
    }
        // Jeżeli klucz jest mniejszy od wartości w rodzicu
    else if (node->value < parent->value) {
        parent->left = node;
    }
        // W przeciwnym razie będzie prawym dzieckiem
    else {
        parent->right = node;
    }

    // Naprawa własności drzewa czerwono-czarnego
    fixInsert(node);
}


template <typename T>
void RedBlackTree<T>::fixInsert(Node<T>* node) {
    Node<T>* parent = nullptr;
    Node<T>* grandparent = nullptr;

    // Dopóki node nie jest korzeniem i ojciec węzła jest czerwony
    while ((node != root) && (node->color == RED) &&
           (node->parent->color == RED)) {

        parent = node->parent;
        grandparent = parent->parent;

        // Przypadek A: Ojciec jest lewym dzieckiem dziadka
        if (parent == grandparent->left) {
            Node<T>* uncle = grandparent->right;
            // 1. Wujek jest czerwony
            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grandparent;
            }
                // 2. Wujek jest czarny lub nie istnieje
            else {
                // a) node jest prawym dzieckiem -> rotacja w lewo na rodzicu
                if (node == parent->right) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }
                // b) rotacja w prawo na dziadku
                rotateRight(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        }
            // Przypadek B: Ojciec jest prawym dzieckiem dziadka
        else {
            Node<T>* uncle = grandparent->left;

            // 1. Wujek jest czerwony
            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grandparent;
            }
                // 2. Wujek jest czarny lub nie istnieje
            else {
                // a) node jest lewym dzieckiem -> rotacja w prawo na rodzicu
                if (node == parent->left) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }
                // b) rotacja w lewo na dziadku
                rotateLeft(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        }
    }
    // Korzeń zawsze jest czarny
    root->color = BLACK;
}

template <typename T>
void RedBlackTree<T>::transplant(Node<T> *root, Node<T> *u, Node<T> *v) {
    if (u->parent == nullptr) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}


//przerobic na boolean
// Usuwanie doprzeanalizować

template <typename T>
void RedBlackTree<T>::remove(T key) {
    Node<T>* node = root;
    Node<T>* z = nullptr;
    Node<T>* x = nullptr;
    Node<T>* y = nullptr;

    // 1. Szukamy węzła o wartości key w drzewie
    while (node != nullptr) {
        if (node->data == key) {
            z = node;   // Zapamiętujemy węzeł, który chcemy usunąć
        }

        if (node->data <= key) {
            node = node->right;
        }
        else {
            node = node->left;
        }
    }

    // 2. Sprawdzamy, czy znaleziono węzeł do usunięcia
    if (z == nullptr) {
        std::cout << "Key not found in the tree" << std::endl;
        return;
    }

    // 3. Przygotowanie zmiennych do usuwania
    y = z;
    Color yOriginalColor = y->color; // Zapamiętujemy oryginalny kolor z

    // a) Węzeł z ma pusty lewy podwęzeł
    if (z->left == nullptr) {
        x = z->right;
        transplant(root, z, z->right);
    }
    // b) Węzeł z ma pusty prawy podwęzeł
    else if (z->right == nullptr) {
        x = z->left;
        transplant(root, z, z->left);
    }
    // c) Węzeł z ma *oba* podwęzły (lewą i prawą gałąź)
    else {
        y = minValueNode(z->right);
        yOriginalColor = y->color;
        // zapisujemy kolor najmniejszego węzła
        x = y->right;
        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
        }
        else {
            // Zamieniamy y z jego prawym dzieckiem
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (yOriginalColor == BLACK) {
        fixDelete(x);
    }
}

template <typename T>
Node<T>* RedBlackTree<T>::minValueNode(Node<T>* node)
{
    Node<T>* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}


