#ifndef COMPARTMENT_H
#define COMPARTMENT_H

#include "node.h"

class Compartment
{
private:
  Node* start;
  Node* end;
  double length;
  double cylVolume;
  double cylSurface;
  double frstmVolume;
  double frstmSurface;
public:
  Compartment();

  double getcylVolume();
  double getcylSurface();
  double getfrstmVolume();
  double getfrstmSurface();

  double calc_cylVolume();
  double calc_cylSurface();
  double calc_frstmVolume();
  double calc_frstmSurface();
  void updateParams();

  inline double getlength(){return this->length;}
  inline void setStart(Node* startNode){this->start = startNode;}
  inline void setEnd(Node* endNode){this->end = endNode;}
  inline Node* getStart(){return this->start;}
  inline Node* getEnd(){return this->end;}
};

#endif // COMPARTMENT_H
