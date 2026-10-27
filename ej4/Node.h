#ifndef EJ4_NODE_H
#define EJ4_NODE_H

#include <vector>

using namespace std;

class Graph;
class BFSIterator;
class DFSIterator;


class Node {
public:
    explicit Node(int value) : value_(value) {}
    int getValue() const { return value_; }

private:
    friend class Graph;
    friend class BFSIterator;
    friend class DFSIterator;

    int value_;
    vector<Node*> neighbors_;
};

#endif
