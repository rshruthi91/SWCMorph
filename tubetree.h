#ifndef TUBETREE_H
#define TUBETREE_H

#include "segment.h"

typedef QLinkedList<Segment*> Path;

class TubeTree
{
private:
    //QVector<Node> NodeList;
    QString infilepath;
    bool populated;
    QString vtk_major_version = "2";
    QString vtk_minor_version = "0";
public:
    //Use qDeleteAll for the following
    QMap<int,Node*> NodeList; //for checking and understanding
    QVector<Compartment*> CompartmentList;
    QVector<Segment*> Segments;
    QVector<Segment*> RootSegList;
    QVector<Path> Paths;
    int num_trees;

    TubeTree();
    TubeTree(QString swcfile);
    bool SWCReadNodes();
    bool makeCompartments();
    void makeSegments();
    void getRootSegments();
    void connectSegments();
    bool writeVtkPoly(QString filename);

    inline bool isPopulated(){return this->populated;}
};

#endif // TUBETREE_H
