#ifndef SEGMENT_H
#define SEGMENT_H

#include "compartment.h"

class Segment
{
private:
    QLinkedList<Compartment> CompartmentList;
    Node *startNode;
    Node *endNode;
public:
    Segment();
};

#endif // SEGMENT_H
