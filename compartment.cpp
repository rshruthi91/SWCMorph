#include "compartment.h"

Compartment::Compartment(Node *startNode, Node *endNode)
{
  this->start = startNode;
  this->end = endNode;
  this->length = this->start->eucldist(this->end);
}

double Compartment::getcylVolume() {
  return PI*std::pow((this->end->getR()),2)*this->length;
}

double Compartment::getcylSurface() {
  return 2*PI*(this->end->getR())*this->length;
}

double Compartment::getfrstmVolume() {
  return (PI/3)*this->length*(std::pow((this->end->getR()),2) +
                              std::pow((this->start->getR()),2) +
                              (this->end->getR())*(this->start->getR())
                              );
}

double Compartment::getfrstmSurface() {
  return PI*(this->end->getR() +
             this->start->getR()) *
      std::sqrt(std::pow(this->length,2)+
                std::pow(absdiff(this->end->getR(),
                                 this->start->getR()),2)
                );
}
