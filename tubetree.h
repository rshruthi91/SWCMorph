#ifndef TUBETREE_H
#define TUBETREE_H

#include "segmentlist.h"
//#include "node.h"
//#include "compartment.h"

class TubeTree
{
private:
    //QVector<Node> NodeList;
    QString infilepath;
public:
    //Use qDeleteAll for the following
    QVector<Node*> NodeList; //for checking and understanding
    QVector<Compartment*> CompartmentList;
    QVector<Segment*> Segments;
    QVector<SegmentList*> Trees;
    TubeTree();
    TubeTree(QString swcfile);
    bool SWCReadNodes();
    bool makeCompartments();
    void makeSegments();
};

#endif // TUBETREE_H
