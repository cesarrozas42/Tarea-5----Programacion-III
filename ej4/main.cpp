#include <iostream>
#include <memory>
#include <string>

#include "BFSIterator.h"
#include "DFSIterator.h"
#include "Graph.h"
#include "GraphIterator.h"

using namespace std;

void printTraversal(const string& label, GraphIterator& it) {
    cout << label << ": ";
    while (it.hasNext()) {
        cout << it.next() << " ";
    }
    cout << endl;
}

int main() {
    Graph g;
    g.addEdge(1,2);
    g.addEdge(1,3);
    g.addEdge(2,4);

    unique_ptr<GraphIterator> it =
        make_unique<BFSIterator>(g, 1);

    while(it->hasNext()) {
        cout << it->next() << " ";
    }
    cout << endl;
    it = make_unique<DFSIterator>(g, 1);
    printTraversal("DFS desde 1", *it);

    it = make_unique<BFSIterator>(g, 1);
    printTraversal("BFS desde 1", *it);
    Graph cyclic;
    cyclic.addEdge(1, 2);
    cyclic.addEdge(2, 3);
    cyclic.addEdge(3, 1);   // ciclo 1 -> 2 -> 3 -> 1
    cyclic.addEdge(3, 4);
    cyclic.addEdge(4, 4);   // auto-lazo
    cyclic.addEdge(2, 5);

    it = make_unique<BFSIterator>(cyclic, 1);
    printTraversal("BFS con ciclos", *it);

    it = make_unique<DFSIterator>(cyclic, 1);
    printTraversal("DFS con ciclos", *it);

    it->reset();
    printTraversal("DFS tras reset", *it);

    it = make_unique<BFSIterator>(g, 99);
    printTraversal("BFS desde nodo inexistente", *it);

    return 0;
}
