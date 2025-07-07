//
// Created by Ashly Veliz on 6/07/25.
//

#ifndef PROYECTOS_NODE_H
#define PROYECTOS_NODE_H

template <typename T>
struct Node {
    T key;
    Node<T>* parent;
    Node<T>* child;
    Node<T>* left;
    Node<T>* right;
    int degree;
    bool mark;

    // Constructor por defecto
    Node() : key(T()), parent(nullptr), child(nullptr),
             left(this), right(this), degree(0), mark(false) {}

    // Constructor con valor
    Node(T value) : key(value), parent(nullptr), child(nullptr),
                    left(this), right(this), degree(0), mark(false) {}

    // Destructor que libera recursivamente los hijos
    ~Node() {
        if (child != nullptr) {
            Node<T>* start = child;
            Node<T>* current = child;

            do {
                Node<T>* next = current->right;
                delete current;
                current = next;
            } while (current != start);
        }
    }
};


#endif //PROYECTOS_NODE_H
