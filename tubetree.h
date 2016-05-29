#ifndef TUBETREE_H
#define TUBETREE_H

#include "segment.h"

typedef QLinkedList<Segment*> Path;

class TubeTree
{
private:
    //QVector<Node> NodeList;
    QString infilepath;
    QFileInfo infileInfo;
    bool populated;
    QString vtk_major_version = "2";
    QString vtk_minor_version = "0";
    void writeParams(QJsonObject &json);

    int num_bifs;
    int num_terminals;

    double avg_segvolcyl;
    double min_segvolcyl;
    double max_segvolcyl;

    double avg_compvolcyl;
    double min_compvolcyl;
    double max_compvolcyl;

    double avg_segvolfrstm;
    double min_segvolfrstm;
    double max_segvolfrstm;

    double avg_compvolfrstm;
    double min_compvolfrstm;
    double max_compvolfrstm;
    
    double avg_segsurfcyl;
    double min_segsurfcyl;
    double max_segsurfcyl;

    double avg_compsurfcyl;
    double min_compsurfcyl;
    double max_compsurfcyl;

    double avg_segsurffrstm;
    double min_segsurffrstm;
    double max_segsurffrstm;

    double avg_compsurffrstm;
    double min_compsurffrstm;
    double max_compsurffrstm;

    double avg_segpathlen;
    double min_segpathlen;
    double max_segpathlen;

    double avg_segeuclen;
    double min_segeuclen;
    double max_segeuclen;

    double avg_complen;
    double min_complen;
    double max_complen;

    double total_cylvol;
    double total_frstmvol;
    double total_cylsurf;
    double total_frstmsurf;
    double total_len;

    bool globalstatsdone;
public:
    int vol_idx,vol_idy,vol_idz;
    QString filebasename;
    ParamsFormat Params;
    //Use qDeleteAll for the following
    QMap<int,Node*> NodeList; //for checking and understanding
    QVector<Compartment*> CompartmentList;
    QVector<Segment*> Segments;
    QVector<Segment*> RootSegList;
    QVector<Path> Paths;

    TubeTree();
    TubeTree(QString swcfile);
    bool SWCReadNodes();
    bool makeCompartments();
    void makeSegments();
    void getRootSegments();
    void connectSegments();
    bool writeVtkPoly(QString filename);

    inline bool isPopulated(){return this->populated;}

    void getSegStats();
    void getCompStats();
    bool writeJson(QString filename);
    bool writeDat(QString filename);
    bool write(QString filename);
};

#endif // TUBETREE_H
