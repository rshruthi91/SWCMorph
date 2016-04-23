#ifndef TUBETREE_H
#define TUBETREE_H

#include "segment.h"
//#include "node.h"
//#include "compartment.h"

class TubeTree
{
private:
    Segment *soma;//If this exists. It is the head of the neuron tree
    //QVector<Node> NodeList;
    QString infilepath;
public:
    QVector<Node> NodeList; //for checking and understanding
    TubeTree();
    TubeTree(QString swcfile);
    bool SWCReadNodes();
};

#endif // TUBETREE_H
