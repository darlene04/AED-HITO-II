//
// Created by Ashly Veliz on 6/07/25.
//

#ifndef PROYECTOS_FIBOHEAP_H
#define PROYECTOS_FIBOHEAP_H
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include "Node.h"

template <typename T>
class FiboHeap {
private:
    Node<T>* minNode;
    int totalNodes;

    void consolidate();
    void link(Node<T>* y, Node<T>* x);
    void cut(Node<T>* x, Node<T>* y);
    void cascadingCut(Node<T>* y);
    Node<T>* findNodeRecursive(Node<T>* current, T key);

public:
    FiboHeap();
    ~FiboHeap();

    void insert(T key);
    T getMin() const;
    void extractMin();
    void decreaseKey(Node<T>* x, T newKey);
    void deleteNode(Node<T>* x);

    Node<T>* find(T key);
    bool isEmpty() const;
    int size() const;
    void print();
    void printNode(Node<T>* node, int depth = 0);

    static FiboHeap<T> Union(FiboHeap<T>& h1, FiboHeap<T>& h2);
};

// ====================== IMPLEMENTACIÓN ======================

template <typename T>
FiboHeap<T>::FiboHeap() : minNode(nullptr), totalNodes(0) {}

template <typename T>
FiboHeap<T>::~FiboHeap() {
    if (!minNode) return;

    Node<T>* current = minNode;
    do {
        Node<T>* next = current->right;
        delete current;
        current = next;
    } while (current != minNode);
}

template <typename T>
bool FiboHeap<T>::isEmpty() const {
    return minNode == nullptr;
}

template <typename T>
int FiboHeap<T>::size() const {
    return totalNodes;
}

template <typename T>
void FiboHeap<T>::insert(T key) {
    Node<T>* node = new Node<T>(key);

    if (!minNode) {
        minNode = node;
    } else {
        node->left = minNode;
        node->right = minNode->right;
        minNode->right->left = node;
        minNode->right = node;

        if (key < minNode->key) {
            minNode = node;
        }
    }

    totalNodes++;
}

template <typename T>
T FiboHeap<T>::getMin() const {
    if (!minNode)
        throw std::runtime_error("Heap vacío");
    return minNode->key;
}

template <typename T>
void FiboHeap<T>::extractMin() {
    if (!minNode) return;

    Node<T>* z = minNode;

    // Añadir hijos a la lista de raíces
    if (z->child) {
        Node<T>* child = z->child;
        do {
            Node<T>* next = child->right;

            // Insertar hijo en la raíz
            child->left->right = child->right;
            child->right->left = child->left;

            child->left = minNode;
            child->right = minNode->right;
            minNode->right->left = child;
            minNode->right = child;

            child->parent = nullptr;

            child = next;
        } while (child != z->child);
    }

    // Remover z de la raíz
    z->left->right = z->right;
    z->right->left = z->left;

    if (z == z->right) {
        minNode = nullptr;
    } else {
        minNode = z->right;
        consolidate();
    }

    delete z;
    totalNodes--;
}

template <typename T>
void FiboHeap<T>::consolidate() {
    int maxDegree = static_cast<int>(std::log2(totalNodes)) + 1;
    std::vector<Node<T>*> A(maxDegree, nullptr);

    std::vector<Node<T>*> roots;
    Node<T>* current = minNode;

    if (!current) return;

    do {
        roots.push_back(current);
        current = current->right;
    } while (current != minNode);

    for (Node<T>* w : roots) {
        Node<T>* x = w;
        int d = x->degree;
        while (A[d] != nullptr) {
            Node<T>* y = A[d];
            if (x->key > y->key) std::swap(x, y);
            link(y, x);
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }

    minNode = nullptr;

    for (Node<T>* node : A) {
        if (node) {
            if (!minNode) {
                node->left = node->right = node;
                minNode = node;
            } else {
                node->left = minNode;
                node->right = minNode->right;
                minNode->right->left = node;
                minNode->right = node;
                if (node->key < minNode->key) {
                    minNode = node;
                }
            }
        }
    }
}

template <typename T>
void FiboHeap<T>::link(Node<T>* y, Node<T>* x) {
    y->left->right = y->right;
    y->right->left = y->left;

    y->left = y->right = y;

    if (!x->child) {
        x->child = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }

    y->parent = x;
    x->degree++;
    y->mark = false;
}

template <typename T>
void FiboHeap<T>::decreaseKey(Node<T>* x, T newKey) {
    if (!x || newKey > x->key) {
        throw std::invalid_argument("Nueva clave inválida");
    }

    x->key = newKey;
    Node<T>* y = x->parent;

    if (y && x->key < y->key) {
        cut(x, y);
        cascadingCut(y);
    }

    if (x->key < minNode->key) {
        minNode = x;
    }
}

template <typename T>
void FiboHeap<T>::cut(Node<T>* x, Node<T>* y) {
    if (x->right == x) {
        y->child = nullptr;
    } else {
        if (y->child == x) y->child = x->right;
        x->left->right = x->right;
        x->right->left = x->left;
    }

    y->degree--;

    x->left = minNode;
    x->right = minNode->right;
    minNode->right->left = x;
    minNode->right = x;

    x->parent = nullptr;
    x->mark = false;
}

template <typename T>
void FiboHeap<T>::cascadingCut(Node<T>* y) {
    Node<T>* z = y->parent;
    if (z) {
        if (!y->mark) {
            y->mark = true;
        } else {
            cut(y, z);
            cascadingCut(z);
        }
    }
}

template <typename T>
void FiboHeap<T>::deleteNode(Node<T>* x) {
    decreaseKey(x, std::numeric_limits<T>::min());
    extractMin();
}

template <typename T>
Node<T>* FiboHeap<T>::findNodeRecursive(Node<T>* current, T key) {
    if (!current) return nullptr;

    Node<T>* start = current;
    do {
        if (current->key == key) return current;
        Node<T>* res = findNodeRecursive(current->child, key);
        if (res) return res;
        current = current->right;
    } while (current != start);

    return nullptr;
}

template <typename T>
Node<T>* FiboHeap<T>::find(T key) {
    return findNodeRecursive(minNode, key);
}

template <typename T>
void FiboHeap<T>::print() {
    if (!minNode) {
        std::cout << "(Heap vacío)\n";
        return;
    }

    std::cout << "Raíces:\n";
    printNode(minNode, 0);
}

template <typename T>
void FiboHeap<T>::printNode(Node<T>* node, int depth) {
    Node<T>* start = node;
    do {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << "- " << node->key << "\n";

        if (node->child)
            printNode(node->child, depth + 1);

        node = node->right;
    } while (node != start);
}
template <typename T>
FiboHeap<T> FiboHeap<T>::Union(FiboHeap<T>& h1, FiboHeap<T>& h2) {
    FiboHeap<T> result;

    result.minNode = h1.minNode;

    if (!h1.minNode) {
        result.minNode = h2.minNode;
    } else if (h2.minNode) {
        // unir las listas de raíces de h1 y h2
        Node<T>* h1Left = h1.minNode->left;
        Node<T>* h2Left = h2.minNode->left;

        h1.minNode->left = h2Left;
        h2Left->right = h1.minNode;

        h2.minNode->left = h1Left;
        h1Left->right = h2.minNode;

        // actualizar el minNode si es necesario
        if (h2.minNode->key < h1.minNode->key) {
            result.minNode = h2.minNode;
        }
    }

    result.totalNodes = h1.totalNodes + h2.totalNodes;

    // invalidar los heaps originales si deseas
    h1.minNode = nullptr;
    h2.minNode = nullptr;
    h1.totalNodes = 0;
    h2.totalNodes = 0;

    return result;
}

#endif //PROYECTOS_FIBOHEAP_H
