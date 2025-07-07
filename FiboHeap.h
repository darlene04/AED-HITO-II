// Created by Ashly Veliz on 6/07/25.

#ifndef PROYECTOS_FIBOHEAP_H
#define PROYECTOS_FIBOHEAP_H
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <unordered_set>
#include "Node.h"
using namespace std;

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
    if (!minNode) throw runtime_error("Heap vacio");
    return minNode->key;
}

template <typename T>
void FiboHeap<T>::extractMin() {
    if (!minNode) return;

    Node<T>* z = minNode;

    if (z->child) {
        Node<T>* child = z->child;
        Node<T>* start = child;
        vector<Node<T>*> children;

        // almacena hijos en vector para evitar corrupcion en ciclo
        do {
            children.push_back(child);
            child = child->right;
        } while (child != start);

        for (Node<T>* c : children) {
            // elimina conexiones previas
            c->left = c->right = c;
            c->parent = nullptr;

            // inserta en la lista de raices
            c->left = minNode;
            c->right = minNode->right;
            minNode->right->left = c;
            minNode->right = c;
        }

        z->child = nullptr; // limpia referencia a hijos
    }

    // remueve z de la lista de raices
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
    if (!minNode) return;

    int maxDegree = log2(totalNodes) + 2;
    vector<Node<T>*> A(maxDegree, nullptr);

    vector<Node<T>*> rootList;
    Node<T>* current = minNode;

    do {
        rootList.push_back(current);
        current = current->right;
    } while (current != minNode);

    for (Node<T>* w : rootList) {
        Node<T>* x = w;
        int d = x->degree;
        while (A[d]) {
            Node<T>* y = A[d];
            if (x->key > y->key) swap(x, y); // mantiene el menor como raiz
            link(y, x); // y pasa a ser hijo de x
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }

    minNode = nullptr;
    for (Node<T>* node : A) {
        if (node) {
            node->left = node->right = node;
            if (!minNode) {
                minNode = node;
            } else {
                node->left = minNode;
                node->right = minNode->right;
                minNode->right->left = node;
                minNode->right = node;
                if (node->key < minNode->key)
                    minNode = node;
            }
        }
    }
}

template <typename T>
void FiboHeap<T>::link(Node<T>* y, Node<T>* x) {
    y->left->right = y->right;
    y->right->left = y->left;

    y->left = y->right = y;
    y->parent = x;

    if (!x->child) {
        x->child = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = false;
}

template <typename T>
void FiboHeap<T>::decreaseKey(Node<T>* x, T newKey) {
    if (!x || newKey > x->key) throw invalid_argument("Nueva clave invalida");

    x->key = newKey;
    Node<T>* y = x->parent;
    if (y && x->key < y->key) {
        cut(x, y);
        cascadingCut(y);
    }
    if (x->key < minNode->key) minNode = x;
}

template <typename T>
void FiboHeap<T>::cut(Node<T>* x, Node<T>* y) {
    if (x->right == x) y->child = nullptr;
    else {
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
        if (!y->mark) y->mark = true;
        else {
            cut(y, z);
            cascadingCut(z);
        }
    }
}

template <typename T>
void FiboHeap<T>::deleteNode(Node<T>* x) {
    decreaseKey(x, numeric_limits<T>::min());
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
        cout << "(Heap vacio)\n";
        return;
    }
    cout << "Raices:\n";
    printNode(minNode, 0);
}

template <typename T>
void FiboHeap<T>::printNode(Node<T>* node, int depth) {
    if (!node) return;
    Node<T>* current = node;
    unordered_set<Node<T>*> visited;
    do {
        if (visited.count(current)) break;
        visited.insert(current);

        for (int i = 0; i < depth; ++i) cout << "\u2502   ";
        cout << "\u251c─ " << current->key;
        if (current->mark) cout << " (marked)";
        cout << endl;

        if (current->child)
            printNode(current->child, depth + 1);

        current = current->right;
    } while (current != node);
}

template <typename T>
FiboHeap<T> FiboHeap<T>::Union(FiboHeap<T>& h1, FiboHeap<T>& h2) {
    FiboHeap<T> result;
    result.minNode = h1.minNode;
    if (!h1.minNode) {
        result.minNode = h2.minNode;
    } else if (h2.minNode) {
        Node<T>* h1Left = h1.minNode->left;
        Node<T>* h2Left = h2.minNode->left;

        h1.minNode->left = h2Left;
        h2Left->right = h1.minNode;
        h2.minNode->left = h1Left;
        h1Left->right = h2.minNode;

        if (h2.minNode->key < h1.minNode->key)
            result.minNode = h2.minNode;
    }
    result.totalNodes = h1.totalNodes + h2.totalNodes;
    h1.minNode = h2.minNode = nullptr;
    h1.totalNodes = h2.totalNodes = 0;
    return result;
}

#endif //PROYECTOS_FIBOHEAP_H