#ifndef COMPARTMENT_H
#define COMPARTMENT_H

#include "node.h"

class Compartment
{
private:
  Node *start;
  Node *end;
  double length;
public:
  Compartment(Node *startNode,Node *endNode);
  double getcylVolume();
  double getcylSurface();
  double getfrstmVolume();
  double getfrstmSurface();
  inline double getlength(){return this->length;}
};

#endif // COMPARTMENT_H
