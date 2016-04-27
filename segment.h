#ifndef SEGMENT_H
#define SEGMENT_H

#include "compartment.h"

class Segment
{
private:
    //Use qdeleteall to avoid memory leaks
    bool root;
    QVector<Segment*> children;
    QVector<Segment*> parents;
//    double pathlength;
//    double euc_length;
//    double cylVolume;
//    double cylSurface;
//    double frstmVolume;
//    double frstmSurface;
    double calc_cylVolume();
    double calc_cylSurface();
    double calc_frstmVolume();
    double calc_frstmSurface();
public:
    Segment();
    double pathlength;
    double euc_length;
    double cylVolume;
    double cylSurface;
    double frstmVolume;
    double frstmSurface;
    QLinkedList<Compartment*> CompartmentList;
    inline Compartment* getStart(){
        if(CompartmentList.isEmpty()) return NULL;
        else return this->CompartmentList.first();
    }
    inline Compartment* getEnd(){
        if(CompartmentList.isEmpty()) return NULL;
        else return this->CompartmentList.last();
    }
    inline void addCompartment(Compartment* comp){
        CompartmentList.append(comp);
    }
    inline bool isRoot(){return this->root;}
    inline void setRoot(bool r){ this->root = r;}

    inline QVector<Segment*> getChildren(){
        return this->children;
    }

    inline QVector<Segment*> getParents(){
        return this->parents;
    }

    inline bool isChild(Segment* s){
        if(this->children.contains(s))
            return true;
        else
            return false;
    }

    inline bool isParent(Segment* s){
        if(this->parents.contains(s))
            return true;
        else
            return false;
    }

    inline void addChild(Segment* child) {
        this->children.append(child);
    }
    inline void addParent(Segment* par) {
        this->parents.append(par);
    }

    void updateParams();
};

#endif // SEGMENT_H
