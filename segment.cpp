#include "segment.h"

Segment::Segment()
{
    this->CompartmentList.clear();
    root=false;
    qDeleteAll(this->children);
    children.clear();
    qDeleteAll(this->parents);
    parents.clear();
    this->pathlength=0.0;
    this->euc_length=0.0;
    this->cylVolume=0.0;
    this->cylSurface=0.0;
    this->frstmVolume=0.0;
    this->frstmSurface=0.0;
}

void Segment::updateParams() {
    QMutableLinkedListIterator<Compartment*> iter(this->CompartmentList);
    iter.toFront();
    while(iter.hasNext()){
        Compartment* c = iter.next();
        this->cylVolume += c->getcylVolume();
        this->cylSurface += c->getcylSurface();
        this->frstmSurface += c->getfrstmSurface();
        this->frstmVolume += c->getfrstmVolume();
        this->pathlength += c->getlength();
    }
    this->euc_length = this->CompartmentList.first()->getStart()->eucldist(
                this->CompartmentList.last()->getEnd());
}
