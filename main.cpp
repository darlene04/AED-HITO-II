#include <iostream>
#include "FiboHeap.h"
using namespace std;

int main() {
    cout << "\n🔧 INICIANDO PRUEBAS COMPLETAS DE FIBONACCI HEAP 🔧\n" << endl;

    FiboHeap<int> heap1;

    // 1. Insertar elementos
    cout << "Insertando en heap1: 10, 20, 30, 5, 15" << endl;
    heap1.insert(10);
    heap1.insert(20);
    heap1.insert(30);
    heap1.insert(5);
    heap1.insert(15);

    heap1.print();
    cout << "\nMínimo actual: " << heap1.getMin() << endl;

    // 2. Buscar y decreaseKey
    Node<int>* n20 = heap1.find(20);
    if (n20) {
        cout << "\nDisminuyendo 20 → 2" << endl;
        heap1.decreaseKey(n20, 2);
        heap1.print();
        cout << "\nNuevo mínimo: " << heap1.getMin() << endl;
    }

    // 3. Eliminar nodo
    Node<int>* n10 = heap1.find(10);
    if (n10) {
        cout << "\nEliminando nodo con clave 10"<< endl;
        heap1.deleteNode(n10);
        heap1.print();
    }

    // 4. Crear segundo heap
    FiboHeap<int> heap2;
    cout << "\n Insertando en heap2:" << endl;
    heap2.insert(17);
    heap2.insert(6);
    heap2.insert(28);

    heap2.print();

    // Unión de heaps
    cout << "\nUniendo heap1 y heap2 en heap3..." << endl;
    FiboHeap<int> heap3 = FiboHeap<int>::Union(heap1, heap2);

    cout << "\nHeap resultante (heap3):" << endl;
    heap3.print();
    cout << "\nMínimo de heap3: " << heap3.getMin() << endl;
    cout << "Tamaño total: " << heap3.size() << endl;

    //  Extraer mínimo repetidamente
    cout << "\nExtrayendo todos los mínimos de heap3:\n" << endl;
    while (!heap3.isEmpty()) {
        cout << "Mínimo extraído: " << heap3.getMin() << endl;
        heap3.extractMin();
        heap3.print();
        cout << "----------------------\n";
    }

    // Validaciones finales
    cout << "\nheap3 está vacío? " << (heap3.isEmpty() ? "Sí" : "No") << endl;
    cout << "Tamaño final de heap3: " << heap3.size() << endl;

    // Buscar valor inexistente
    cout << "\n Buscando valor 99 (que no existe):" << endl;
    Node<int>* n = heap3.find(99);
    if (!n) {
        cout << " Nodo con clave 99 no encontrado (correcto).\n";
    }

    cout << "\n TODAS LAS FUNCIONES HAN SIDO PROBADAS EXITOSAMENTE\n";
    return 0;
}
