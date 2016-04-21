#include "node.h"

Node::Node(double pos_x, double pos_y, double pos_z)
{
  this->x = pos_x;
  this->y = pos_y;
  this->z = pos_z;
}

double Node::eucldist(Node *toNode) {
  double out=0.0;
  out =  std::sqrt(std::pow(absdiff(toNode->getX(),this->x),2)+
                   std::pow(absdiff(toNode->getY(),this->y),2)+
                   std::pow(absdiff(toNode->getZ(),this->z),2));
  return out;
}
