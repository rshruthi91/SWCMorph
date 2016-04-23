#include "compartment.h"

Compartment::Compartment(Node *startNode, Node *endNode)
{
  this->start = startNode;
  this->end = endNode;
  this->length = this->start->eucldist(this->end);
}

double Compartment::getcylVolume() {
  return PI*std::pow((this->end->getRadius()),2)*this->length;
}

double Compartment::getcylSurface() {
  return 2*PI*(this->end->getRadius())*this->length;
}

double Compartment::getfrstmVolume() {
  return (PI/3)*this->length*(std::pow((this->end->getRadius()),2) +
                              std::pow((this->start->getRadius()),2) +
                              (this->end->getRadius())*(this->start->getRadius())
                              );
}

double Compartment::getfrstmSurface() {
  return PI*(this->end->getRadius() +
             this->start->getRadius()) *
      std::sqrt(std::pow(this->length,2)+
                std::pow(absdiff(this->end->getRadius(),
                                 this->start->getRadius()),2)
                );
}
