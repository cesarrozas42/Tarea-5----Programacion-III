#ifndef EJ4_DFSITERATOR_H
#define EJ4_DFSITERATOR_H

#include <stack>
#include <stdexcept>
#include <unordered_set>

#include "Graph.h"
#include "GraphIterator.h"

using namespace std;

class DFSIterator : public GraphIterator {
public:
    DFSIterator(const Graph& graph, int start) : graph_(graph), start_(start) {
        reset();
    }

    bool hasNext() const override {
        auto copy = stack_;
        while (!copy.empty() && visited_.count(copy.top())) copy.pop();
        return !copy.empty();
    }

    int next() override {
        skipVisited();
        if (stack_.empty()) throw out_of_range("DFSIterator: no hay mas nodos");

        const Node* current = stack_.top();
        stack_.pop();
        visited_.insert(current);

        const auto& nb = current->neighbors_;
        for (auto it = nb.rbegin(); it != nb.rend(); ++it) {
            if (!visited_.count(*it)) stack_.push(*it);
        }
        skipVisited();
        return current->getValue();
    }

    void reset() override {
        stack_ = {};
        visited_.clear();
        if (const Node* s = graph_.findNode(start_)) stack_.push(s);
    }

private:
    void skipVisited() {
        while (!stack_.empty() && visited_.count(stack_.top())) stack_.pop();
    }

    const Graph& graph_;
    int start_;
    stack<const Node*> stack_;
    unordered_set<const Node*> visited_;
};

#endif