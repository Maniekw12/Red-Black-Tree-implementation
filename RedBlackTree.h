#include <iostream>
#include <utility> // dla std::swap
#include <string>
#include <vector>
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

    void transplant(Node<T>* u, Node<T>* v);

    void fixInsert(Node<T>* node);
    void fixDelete(Node<T>* node);

    Node<T>* minValueNode(Node<T>* node);



public:
    RedBlackTree() : root(nullptr) {}

    Color get_root_color() const {
        if (root== nullptr){
            return BLACK;
        }
        return root->color;
    }

    Node<T> * getRoot() const {
        return root;
    }

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

    bool remove(T key);

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
    Node<T>* node = new Node<T>(key);

    Node<T>* parent = nullptr;
    Node<T>* current = root;

    while (current != nullptr) {
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
    }
    else if (node->value < parent->value) {
        parent->left = node;
    }
    else {
        parent->right = node;
    }

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
    root->color = BLACK;
}

template <typename T>
Node<T>* RedBlackTree<T>::minValueNode(Node<T>* node)
{
    Node<T>* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}



template <typename T>
void RedBlackTree<T>::transplant(Node<T>* u, Node<T>* v)
{
    if (u->parent == nullptr) {
        this->root = v;
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


template <typename T>
bool RedBlackTree<T>::remove(T key)
{
    Node<T>* node = this->root;
    Node<T>* z = nullptr;
    Node<T>* x = nullptr;
    Node<T>* y = nullptr;

    // 1. Szukamy węzła z wartością key
    while (node != nullptr) {
        if (node->value == key) {
            z = node;
        }
        // Idziemy w prawo, jeśli node->value <= key,
        // bo może jeszcze gdzieś w prawo być identyczny klucz.
        if (node->value <= key) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    // 2. Jeśli nie znaleziono węzła, zwróć false (lub wyświetl komunikat)
    if (z == nullptr) {
        std::cout << "Key not found in the tree\n";
        return false;
    }

    y = z;
    Color yOriginalColor = y->color;

    // 3a. Gdy z->left == nullptr
    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right);
    }
        // 3b. Gdy z->right == nullptr
    else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left);
    }
        // 3c. Gdy z ma oboje dzieci
    else {
        y = minValueNode(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
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

    if (yOriginalColor == BLACK) {
        fixDelete(x);
    }

    return true;
}


template <typename T>
void RedBlackTree<T>::fixDelete(Node<T>* node)
{
    if (node == nullptr) {
        return;
    }

    // Dopóki nie jesteśmy w korzeniu i węzeł jest czarny,
    // staramy się przywrócić własności drzewa czerwono-czarnego.
    while (node != this->root && node->color == BLACK) {

        // PRZYPADEK A: node jest lewym dzieckiem
        if (node == node->parent->left) {
            Node<T>* sibling = node->parent->right;

            // 1. Brat jest czerwony
            if (sibling != nullptr && sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }

            // 2. Oba dzieci brata są czarne (lub nullptr)
            if ((sibling == nullptr || sibling->left == nullptr  || sibling->left->color == BLACK) &&
                (sibling == nullptr || sibling->right == nullptr || sibling->right->color == BLACK))
            {
                if (sibling != nullptr) {
                    sibling->color = RED;
                }
                node = node->parent;
            }
            else {
                // 3. Prawe dziecko brata jest czarne, a lewe czerwone
                if (sibling != nullptr &&
                    (sibling->right == nullptr || sibling->right->color == BLACK))
                {
                    if (sibling->left != nullptr) {
                        sibling->left->color = BLACK;
                    }
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }
                // 4. Prawe dziecko brata jest czerwone
                if (sibling != nullptr) {
                    sibling->color = node->parent->color;
                }
                node->parent->color = BLACK;
                if (sibling != nullptr && sibling->right != nullptr) {
                    sibling->right->color = BLACK;
                }
                rotateLeft(node->parent);
                node = this->root;
            }
        }
            // PRZYPADEK B: node jest prawym dzieckiem (symetrycznie)
        else {
            Node<T>* sibling = node->parent->left;

            // 1. Brat jest czerwony
            if (sibling != nullptr && sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }

            // 2. Oboje dzieci brata są czarne
            if ((sibling == nullptr || sibling->left == nullptr  || sibling->left->color == BLACK) &&
                (sibling == nullptr || sibling->right == nullptr || sibling->right->color == BLACK))
            {
                if (sibling != nullptr) {
                    sibling->color = RED;
                }
                node = node->parent;
            }
            else {
                // 3. Lewe dziecko brata jest czarne, a prawe czerwone
                if (sibling != nullptr &&
                    (sibling->left == nullptr || sibling->left->color == BLACK))
                {
                    if (sibling->right != nullptr) {
                        sibling->right->color = BLACK;
                    }
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }
                // 4. Lewe dziecko brata jest czerwone
                if (sibling != nullptr) {
                    sibling->color = node->parent->color;
                }
                node->parent->color = BLACK;
                if (sibling != nullptr && sibling->left != nullptr) {
                    sibling->left->color = BLACK;
                }
                rotateRight(node->parent);
                node = this->root;
            }
        }
    }

    node->color = BLACK;
}

