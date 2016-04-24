#ifndef SEGMENT_H
#define SEGMENT_H

#include "compartment.h"

class Segment
{
private:
    //Use qdeleteall to avoid memory leaks
    QLinkedList<Compartment*> CompartmentList;
public:
    Segment();

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
};

#endif // SEGMENT_H
