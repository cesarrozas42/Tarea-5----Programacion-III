#ifndef EJ4_GRAPHITERATOR_H
#define EJ4_GRAPHITERATOR_H

using namespace std;

class GraphIterator {
public:
    virtual ~GraphIterator() = default;
    virtual bool hasNext() const = 0;
    virtual int next() = 0;
    virtual void reset() = 0;
};

#endif