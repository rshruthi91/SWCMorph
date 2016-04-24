#include "compartment.h"

Compartment::Compartment(){
    this->start = NULL;
    this->end = NULL;
    this->length = 0.0;
    this->cylSurface = 0.0;
    this->cylVolume = 0.0;
    this->frstmSurface = 0.0;
    this->frstmVolume = 0.0;
}

void Compartment::updateParams() {
    if(this->start==NULL or this->end==NULL){
        qDebug() << "Compartment is uninitialized";
        throw std::exception();
    }
    this->length = this->start->eucldist(this->end);
    this->cylVolume = calc_cylVolume();
    this->cylSurface = calc_cylSurface();
    this->frstmVolume = calc_frstmVolume();
    this->frstmSurface = calc_frstmSurface();
}

double Compartment::calc_cylVolume() {
  return PI*std::pow((this->end->getRadius()),2)*this->length;
}

double Compartment::calc_cylSurface() {
  return 2*PI*(this->end->getRadius())*this->length;
}

double Compartment::calc_frstmVolume() {
  return (PI/3)*this->length*(std::pow((this->end->getRadius()),2) +
                              std::pow((this->start->getRadius()),2) +
                              (this->end->getRadius())*(this->start->getRadius())
                              );
}

double Compartment::calc_frstmSurface() {
  return PI*(this->end->getRadius() +
             this->start->getRadius()) *
      std::sqrt(std::pow(this->length,2)+
                std::pow(absdiff(this->end->getRadius(),
                                 this->start->getRadius()),2)
                );
}
