#include <iostream>
#include <utility> // dla std::swap
#include <string>
enum Color { RED, BLACK };



template <typename T>
class Node{
public:
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
    Node<T>* NIL;

    void destroySubtree(Node<T>* node);
    void rotateLeft(Node<T>* node);
    void rotateRight(Node<T>* node);

    void transplant(Node<T>* u, Node<T>* v);

    void fixInsert(Node<T>* node);
    void fixDelete(Node<T>* node);

    Node<T>* minValueNode(Node<T>* node);

public:
    RedBlackTree() {
        NIL = new Node<T>(T());
        NIL->color = BLACK;
        NIL->left = nullptr;
        NIL->right = nullptr;
        NIL->parent = nullptr;
        root = NIL;
    }

    ~RedBlackTree() {
        destroySubtree(root);
        delete NIL;
    }

    Color get_root_color() const {
        if (root == NIL) {
            return BLACK;
        }
        return root->color;
    }

    Node<T>* getRoot() const {
        return root;
    }

    void clear() {
        destroySubtree(root);
        root = NIL;
    }

    bool empty() const {
        return (root == NIL);
    }

    Node<T>* search(Node<T>* node, const T& item) const {
        if (node == NIL || node->value == item) {
            return node;
        }
        else if (item < node->value) {
            return search(node->left, item);
        }
        else {
            return search(node->right, item);
        }
    }

    void insert(const T& key);
    bool remove(T key);

    void printTree() {
        if (root == NIL)
            std::cout << "Tree is empty." << std::endl;
        else {
            std::cout << "Red-Black Tree:" << std::endl;
            printHelper(root, "", true);
        }
    }

    void printHelper(Node<T>* node, const std::string& prefix = "", bool isLast = true) {
        if (node == NIL) {
            return;
        }

        std::cout << prefix;

        std::cout << "-----";

        std::string color = (node->color == RED) ? "RED" : "BLACK";
        std::cout << node->value << " (" + color + ") " << std::endl;

        std::string newPrefix = prefix + (isLast ? "    " : "|   ");

        if (node->left != NIL || node->right != NIL) {
            printHelper(node->left, newPrefix, node->right == NIL);
            printHelper(node->right, newPrefix, true);
        }
    }
};

template <typename T>
void RedBlackTree<T>::destroySubtree(Node<T>* node) {
    if (node == NIL) return;
    destroySubtree(node->left);
    destroySubtree(node->right);
    delete node;
}

template <typename T>
void RedBlackTree<T>::rotateLeft(Node<T>* node) {
    Node<T>* child = node->right;
    node->right = child->left;
    if (child->left != NIL) {
        child->left->parent = node;
    }
    child->parent = node->parent;

    if (node->parent == nullptr) {
        root = child;
    } else if (node == node->parent->left) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }
    child->left = node;
    node->parent = child;
}

template <typename T>
void RedBlackTree<T>::rotateRight(Node<T>* node) {
    Node<T>* child = node->left;
    node->left = child->right;
    if (child->right != NIL) {
        child->right->parent = node;
    }
    child->parent = node->parent;

    if (node->parent == nullptr) {
        root = child;
    } else if (node == node->parent->right) {
        node->parent->right = child;
    } else {
        node->parent->left = child;
    }
    child->right = node;
    node->parent = child;
}

template <typename T>
void RedBlackTree<T>::insert(const T& key) {
    Node<T>* node = new Node<T>(key);
    node->left = NIL;
    node->right = NIL;

    Node<T>* parent = nullptr;
    Node<T>* current = root;

    while (current != NIL) {
        parent = current;
        if (node->value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    node->parent = parent;

    if (parent == nullptr) {
        root = node;
    } else if (node->value < parent->value) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    fixInsert(node);
}

template <typename T>
void RedBlackTree<T>::fixInsert(Node<T>* node) {
    Node<T>* parent = nullptr;
    Node<T>* grandparent = nullptr;

    while ((node != root) && (node->color == RED) && (node->parent->color == RED)) {

        parent = node->parent;
        grandparent = parent->parent;

        // Przypadek A: Ojciec jest lewym dzieckiem dziadka
        if (parent == grandparent->left) {
            Node<T>* uncle = grandparent->right;
            // 1. Wujek jest czerwony
            if (uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grandparent;
            } else {
                // 2. node jest prawym dzieckiem -
                if (node == parent->right) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }
                // 3. Wujek jest czarny, a wstawiany węzeł jest lewym dzieckiem ojca
                rotateRight(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        }
            // Przypadek B: Ojciec jest prawym dzieckiem dziadka
            // to samo co wyzej ale podmienione left z right
        else {
            Node<T>* uncle = grandparent->left;
            if (uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grandparent;
            } else {
                if (node == parent->left) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }
                rotateLeft(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        }
    }
    root->color = BLACK;
}

template <typename T>
void RedBlackTree<T>::transplant(Node<T>* u, Node<T>* v) {
    if (u->parent == nullptr) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

template <typename T>
Node<T>* RedBlackTree<T>::minValueNode(Node<T>* node) {
    while (node->left != NIL) {
        node = node->left;
    }
    return node;
}

template <typename T>
bool RedBlackTree<T>::remove(T key) {
    Node<T>* z = search(root, key);
    if (z == NIL) {
        return false;
    }

    Node<T>* y = z;
    Color y_original_color = y->color;
    Node<T>* x;

    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = minValueNode(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (y_original_color == BLACK) {
        fixDelete(x);
    }

    return true;
}

template <typename T>
void RedBlackTree<T>::fixDelete(Node<T>* node) {
    Node<T>* sibling;

    while (node != root && node->color == BLACK) {
        //1) Node to lewe dziecko rodzica
        if (node == node->parent->left) {
            sibling = node->parent->right;

            // Przypadek 1: brat czerwony
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }
            // Przypadek 2: brat ma 2 czarnych dzieci
            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                // Przypadek 2: brat ma 2 czarnych dzieci
                sibling->color = RED;
                node = node->parent;
            }
                // Przypadek 3: Prawy syn brata jest czarny
            else {
                if (sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }

                // Przypadek 4: Prawy syn brata jest czerwony
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        }
            //1) Node to prawe dziecko rodzica
            // wszyztkie przypadki tak samo z zamienionym lewym i prawym
        else {
            sibling = node->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }

            if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            }
            else {
                if (sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }

                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
}
