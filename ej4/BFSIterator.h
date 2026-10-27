#ifndef EJ4_BFSITERATOR_H
#define EJ4_BFSITERATOR_H

#include <queue>
#include <stdexcept>
#include <unordered_set>

#include "Graph.h"
#include "GraphIterator.h"

using namespace std;

class BFSIterator : public GraphIterator {
public:
    BFSIterator(const Graph& graph, int start) : graph_(graph), start_(start) {
        reset();
    }

    bool hasNext() const override { return !queue_.empty(); }

    int next() override {
        if (!hasNext()) throw out_of_range("BFSIterator: no hay mas nodos");

        const Node* current = queue_.front();
        queue_.pop();

        for (const Node* n : current->neighbors_) {
            if (visited_.insert(n).second) queue_.push(n);
        }
        return current->getValue();
    }

    void reset() override {
        queue_ = {};
        visited_.clear();
        if (const Node* s = graph_.findNode(start_)) {
            queue_.push(s);
            visited_.insert(s);
        }
    }

private:
    const Graph& graph_;
    int start_;
    queue<const Node*> queue_;
    unordered_set<const Node*> visited_;
};

#endif
