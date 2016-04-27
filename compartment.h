#ifndef COMPARTMENT_H
#define COMPARTMENT_H

#include "node.h"

class Compartment
{
protected:
  Node* start;
  Node* end;
  bool paramsUptoDate;
  double length;
  double cylVolume;
  double cylSurface;
  double frstmVolume;
  double frstmSurface;
  double calc_cylVolume();
  double calc_cylSurface();
  double calc_frstmVolume();
  double calc_frstmSurface();
  void updateParams();
public:
  Compartment();

  inline double getlength(){
      if(!paramsUptoDate) updateParams();
      this->paramsUptoDate = true;
      return this->length;
  }
  inline void setStart(Node* startNode){
      this->paramsUptoDate = false;
      this->start = startNode;
  }
  inline void setEnd(Node* endNode){
      this->paramsUptoDate = false;
      this->end = endNode;
  }
  inline Node* getStart(){
      return this->start;
  }
  inline Node* getEnd(){
      return this->end;
  }
  inline double getcylVolume() {
      if(!paramsUptoDate) updateParams();
      this->paramsUptoDate = true;
      return cylVolume;
  }
  inline double getcylSurface(){
      if(!paramsUptoDate) updateParams();
      this->paramsUptoDate = true;
      return cylSurface;
  }
  inline double getfrstmVolume(){
      if(!paramsUptoDate) updateParams();
      this->paramsUptoDate = true;
      return frstmVolume;
  }
  inline double getfrstmSurface(){
      if(!paramsUptoDate) updateParams();
      this->paramsUptoDate = true;
      return frstmSurface;
  }
};

#endif // COMPARTMENT_H
