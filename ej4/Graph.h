#ifndef EJ4_GRAPH_H
#define EJ4_GRAPH_H

#include <algorithm>
#include <cstddef>
#include <memory>
#include <unordered_map>

#include "Node.h"

using namespace std;

// Grafo dirigido. La estructura interna (mapa de nodos y listas de
// adyacencia) no se expone: el cliente solo agrega aristas y recorre el
// grafo mediante un GraphIterator.
class Graph {
public:
    Graph() = default;
    Graph(const Graph&) = delete;             // los Node* internos no se copian
    Graph& operator=(const Graph&) = delete;

    void addNode(int value) { getOrCreate(value); }

    // Crea los nodos si no existen y evita aristas duplicadas.
    void addEdge(int from, int to) {
        Node* a = getOrCreate(from);
        Node* b = getOrCreate(to);
        if (find(a->neighbors_.begin(), a->neighbors_.end(), b) == a->neighbors_.end())
            a->neighbors_.push_back(b);
    }

    bool contains(int value) const { return findNode(value) != nullptr; }
    size_t size() const { return nodes_.size(); }

private:
    friend class BFSIterator;
    friend class DFSIterator;

    Node* findNode(int value) const {
        auto it = nodes_.find(value);
        return it == nodes_.end() ? nullptr : it->second.get();
    }

    Node* getOrCreate(int value) {
        auto& slot = nodes_[value];
        if (!slot) slot = make_unique<Node>(value);
        return slot.get();
    }

    unordered_map<int, unique_ptr<Node>> nodes_;
};

#endif // EJ4_GRAPH_H
